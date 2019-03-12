#include "sobject.h"
#include "ui_sobject.h"

#include <sddl.h>
#include <AccCtrl.h>
#include <AclAPI.h>

#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QMouseEvent>
#include <locale.h>

#include "process.h"
#include "acepopup.h"

#include <QMenu>

#include <QDebug>

SObject::SObject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SObject)
{
    ui->setupUi(this);

   AcePopUpPtr = QSharedPointer<AcePopUp>::create();

   int_menu = new QMenu(this);
   low = new QAction("Low");
   med = new QAction("Medium");
   hi = new QAction("High");

   connect(low, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_LOW_RID);
   });

   connect(med, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_MEDIUM_RID);
   });

   connect(hi, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_HIGH_RID);
   });

   int_menu->addAction(low);
   int_menu->addAction(med);
   int_menu->addAction(hi);

   ui->integrity_tbutton->setMenu(int_menu);
   connect(ui->integrity_tbutton, &QToolButton::clicked, ui->integrity_tbutton, &QToolButton::showMenu);

   connect(AcePopUpPtr.get(), &AcePopUp::SendAceInformation, this, &SObject::AddAceSlot);
   connect(ui->AddACEButton, &QPushButton::clicked, AcePopUpPtr.get(), &AcePopUp::show);
   connect(ui->owner, &QLineEdit::editingFinished, this, &SObject::editOwner);

   connect(ui->AclTreeWidget, &QTreeWidget::clicked, [this](const QModelIndex &)
   {
       auto SelectedItem = ui->AclTreeWidget->selectedItems().first();

       if (nullptr == SelectedItem->parent())
       {
          ui->DeleteACE_Button->setEnabled(true);
       }
       else
          ui->DeleteACE_Button->setEnabled(false);
   });

   connect(ui->DeleteACE_Button, &QPushButton::clicked, [this](bool)
   {
        auto SelectedItem = ui->AclTreeWidget->selectedItems().first();

        if (nullptr == SelectedItem->parent())
        {
           int idx = ui->AclTreeWidget->indexOfTopLevelItem(SelectedItem);

           ui->DeleteACE_Button->setEnabled(false);

           if (0 != DeleteACE(idx))
           {
               QMessageBox::warning(this,
                                   "DeleteACE error",
                                    Proc_info::PrintAPIErrorMessage(GetLastError()),
                                    QMessageBox::Ok);
           }
           else
           {
               UpdateAclInfo();
           }
        }
   });
}

SObject::~SObject()
{
    delete ui;
}


void SObject::editOwner()
{
   std::wstring w_objpath = ObjectUrl.path().replace(0, 1, "").toStdWString();
   std::wstring new_owner = ui->owner->text().toStdWString();

   QString NewSid, Domain;
   if (0 != SetOwner(w_objpath.c_str(), new_owner.c_str(), NewSid, Domain))
   {
           QMessageBox::warning(this,
                               "SetOwner error",
                                Proc_info::PrintAPIErrorMessage(GetLastError()),
                                QMessageBox::Ok);

           ui->owner->undo();
   }
   else
   {
       ui->owner->setText(Domain + "/" + ui->owner->text());
       ui->ownerSid->setText(NewSid);
   }
}

void SObject::AddAceSlot(AceInfo *AceInfo_ptr)
{
    std::wstring w_uname = AceInfo_ptr->Username.toStdWString();

    if (0 != AddACE(ObjectUrl, w_uname.c_str(), AceInfo_ptr->AccessMask, AceInfo_ptr->AceType))
    {
        QMessageBox::warning(this,
                            "AddACE error",
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
    else
    {
        UpdateAclInfo();
    }

    delete AceInfo_ptr;
}

void SObject::SetIntegrityLevelSlot(DWORD level)
{
    if (0 != SetIntegrityLevel(ObjectUrl, level))
    {
        QMessageBox::warning(this,
                            "SetIntegrityLevel error",
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
    else
        UpdateIntLevel();
}

int SObject::DeleteACE(int index)
{
    setlocale(LC_ALL, "Russian");

    _SE_OBJECT_TYPE objtype = (ObjectType == "regkey") ? SE_REGISTRY_KEY : SE_FILE_OBJECT;

    PACL  ppDacl;
    PSECURITY_DESCRIPTOR ppSD;

    std::wstring str = UrlToPath(ObjectUrl).toStdWString();

    LPCWSTR pObjectName = str.c_str();

    DWORD retval = GetNamedSecurityInfoW(pObjectName,
                                        objtype,
                                        DACL_SECURITY_INFORMATION,
                                        nullptr,
                                        nullptr,
                                        &ppDacl,
                                        nullptr,
                                        &ppSD);

    if (retval != ERROR_SUCCESS)
    {
        SetLastError(retval);
        LocalFree(ppSD);
        return -1;
    }

    if (0 == DeleteAce(ppDacl, (DWORD)index))
    {
        LocalFree(ppSD);
        return -1;
    }

    SECURITY_DESCRIPTOR newSD;
    if (0 == InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION))
    {
        LocalFree(ppSD);
        return -1;
    }

    if (0 == SetSecurityDescriptorDacl(&newSD, TRUE, ppDacl, FALSE))
    {
        LocalFree(ppSD);
        return -1;
    }

    if (0 == SetFileSecurity(str.c_str(), DACL_SECURITY_INFORMATION, &newSD))
    {
        LocalFree(ppSD);
        return -1;
    }


    LocalFree(ppSD);
    return 0;
}

int SObject::GetIntegrityLevel(const QUrl &ObjectUrl, QString &IntegrityLevel)
{
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    if (hProcess == nullptr) return -1;

    HANDLE hToken;
    if (0 == OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_READ | TOKEN_QUERY, &hToken))
    {
        CloseHandle(hProcess);
        return -1;
    }

    if (FALSE == SetPrivilege(hToken,  SE_SECURITY_NAME, TRUE))
    {
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return -1;
    }

    CloseHandle(hProcess);
    CloseHandle(hToken);

    std::wstring w_fname = UrlToPath(ObjectUrl).toStdWString();

    DWORD integrityLevel = SECURITY_MANDATORY_MEDIUM_RID;
    PSECURITY_DESCRIPTOR pSD;
    PACL acl;
    DWORD err;

     err = GetNamedSecurityInfoW(w_fname.c_str(),
                                         SE_FILE_OBJECT,
                                         LABEL_SECURITY_INFORMATION,
                                         nullptr, nullptr, nullptr, &acl, &pSD);

     if (err == ERROR_SUCCESS)
     {
         if (acl != nullptr)
         {
            if (0 < acl->AceCount)
            {
                 SYSTEM_MANDATORY_LABEL_ACE* ace = nullptr;
                 if (GetAce(acl, 0, reinterpret_cast<void**>(&ace)))
                 {
                     SID* sid = reinterpret_cast<SID*>(&ace->SidStart);
                     integrityLevel = sid->SubAuthority[0];
                 }
                 else
                 {
                     LocalFree(pSD);
                     return -1;
                 }
             }
             else
             {
                 LocalFree(pSD);
                 return -1;
             }

             LPSTR stringSD;
             ULONG stringSDLen = 0;

             if (0 == ConvertSecurityDescriptorToStringSecurityDescriptorA(pSD,
                                                                           SDDL_REVISION_1,
                                                                           LABEL_SECURITY_INFORMATION,
                                                                           &stringSD, &stringSDLen))
             {
                 LocalFree(pSD);
                 return -1;
             }

             LocalFree(pSD);

             if (pSD == nullptr) return -1;
         }
       }
       else
       {
          return -1;
       }

        if (integrityLevel == SECURITY_MANDATORY_LOW_RID)
            IntegrityLevel = "Low";
        else if (integrityLevel == SECURITY_MANDATORY_MEDIUM_RID)
            IntegrityLevel = "Medium";
        else if (integrityLevel == SECURITY_MANDATORY_HIGH_RID)
            IntegrityLevel = "Hight";
        else
            return -1;


        return 0;
}

int SObject::SetIntegrityLevel(const QUrl &ObjectUrl, DWORD IntegrityLevel)
{
    std::string sidStr;

    switch (IntegrityLevel)
    {
    case SECURITY_MANDATORY_LOW_RID:
        sidStr = "S:(ML;;NR;;;LW)";
        break;
    case SECURITY_MANDATORY_MEDIUM_RID:
        sidStr = "S:(ML;;NR;;;ME)";
        break;
    case SECURITY_MANDATORY_HIGH_RID:
        sidStr = "S:(ML;;NR;;;HI)";
        break;
    default:
        break;
    }

    DWORD dwErr = ERROR_SUCCESS;
    PSECURITY_DESCRIPTOR pSD = nullptr;

    PACL pSacl = nullptr;
    BOOL fSaclPresent = FALSE;
    BOOL fSaclDefaulted = FALSE;

    std::wstring fname = UrlToPath(ObjectUrl).toStdWString();

    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, GetCurrentProcessId());

    if (hProcess == nullptr) return -1;

    HANDLE hToken;
    if (0 == OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_READ | TOKEN_QUERY, &hToken))
    {
        CloseHandle(hProcess);
        return -1;
    }

    if (FALSE == SetPrivilege(hToken,  SE_RESTORE_NAME, TRUE))
    {
        CloseHandle(hProcess);
        CloseHandle(hToken);
        return -1;
    }

    CloseHandle(hProcess);
    CloseHandle(hToken);

    if (ConvertStringSecurityDescriptorToSecurityDescriptorA(
        sidStr.c_str(), SDDL_REVISION_1, &pSD, nullptr))
    {
        if (GetSecurityDescriptorSacl(pSD, &fSaclPresent, &pSacl,
            &fSaclDefaulted))
        {
            dwErr = SetNamedSecurityInfoW(const_cast<wchar_t*>(fname.c_str()),
                SE_FILE_OBJECT, LABEL_SECURITY_INFORMATION,
                nullptr, nullptr, nullptr, pSacl);

            if (dwErr == ERROR_SUCCESS) {
                return 0;
            }
            else
            {
                SetLastError(dwErr);
                return -1;
            }
        }
        LocalFree(pSD);
        return -1;
    }
    else
    {
        return -1;
    }
}

int SObject::GetAclInfo(const QUrl &ObjectUrl, const QString &ObjectType, QList<QTreeWidgetItem*> &vec )
{
    setlocale(LC_ALL, "Russian");

    _SE_OBJECT_TYPE objtype = (ObjectType == "regkey") ? SE_REGISTRY_KEY : SE_FILE_OBJECT;
    SECURITY_INFORMATION SI = DACL_SECURITY_INFORMATION;

    PACL  ppDacl;
    PSECURITY_DESCRIPTOR ppSD;

    std::wstring str = UrlToPath(ObjectUrl).toStdWString();

    LPCWSTR pObjectName = str.c_str();

    DWORD retval = GetNamedSecurityInfoW(pObjectName,
                                        objtype,
                                        SI,
                                        nullptr,
                                        nullptr,
                                        &ppDacl,
                                        nullptr,
                                        &ppSD);

    if (retval != ERROR_SUCCESS)
    {
        SetLastError(retval);
        LocalFree(ppSD);
        return -1;
    }

    ACL_SIZE_INFORMATION  aclsizeinfo;

    if (0 == GetAclInformation(ppDacl,
                                &aclsizeinfo,
                                sizeof(aclsizeinfo),
                                AclSizeInformation))
   {
        LocalFree(ppSD);
        return -1;
   }


    DWORD NameBufferSize = 512;
    DWORD DomainBufferSize = 512;
    WCHAR Name[512];
    WCHAR DomainName[512];
    SID_NAME_USE eSidType;

    ACCESS_ALLOWED_ACE *pAce = nullptr;

    QString SID, SID_Type, ACE_Type, User;

    const char *SidTypeStrings[]
    {
         "",
         "SidTypeUser",
         "SidTypeGroup",
         "SidTypeDomain",
         "SidTypeAlias",
         "SidTypeWellKnownGroup",
         "SidTypeDeletedAccount",
         "SidTypeInvalid",
         "SidTypeUnknown",
         "SidTypeComputer",
         "SidTypeLabel",
         "SidTypeLogonSession"
    };

    for (DWORD cAce = 0; cAce < aclsizeinfo.AceCount; cAce++)
    {
        if (0 != GetAce(ppDacl, cAce, (LPVOID*)&pAce))
        {
            ZeroMemory(Name, sizeof(CHAR) * 512);
            ZeroMemory(DomainName, sizeof(CHAR) * 512);
            NameBufferSize = 512;
            DomainBufferSize = 512;

            if (0 == LookupAccountSidW(nullptr,
                            &pAce->SidStart,
                            Name,
                            &NameBufferSize,
                            DomainName,
                            &DomainBufferSize,
                            &eSidType))
            {
                LocalFree(ppSD);
                return -1;
            }

            LPSTR StringSid = new CHAR[512];
            ConvertSidToStringSidA(&pAce->SidStart, &StringSid);

            SID = QString(StringSid);
            LocalFree(StringSid);

            SID_Type = SidTypeStrings[eSidType];
            ACE_Type = AceTypeString(*&pAce->Header.AceType);
            User = QString::fromWCharArray(DomainName) + "/" + QString::fromWCharArray(Name);

            QTreeWidgetItem *root_item = new QTreeWidgetItem(QStringList{SID, User, SID_Type, ACE_Type});

            QVector<QStringList> access_rights = GetAcessRights(*&pAce->Mask);
            for (auto acc_right : access_rights)
                new QTreeWidgetItem(root_item, acc_right);

            vec.push_back(root_item);

        }
        else
        {
            LocalFree(ppSD);
            return -1;
        }
    }

    LocalFree(ppSD);
    return 0;
}

int SObject::GetOwnerInfo(const QUrl &ObjectUrl, const QString &ObjectType, QString &Owner, QString &SID)
{
    setlocale(LC_ALL, "Russian");

    _SE_OBJECT_TYPE objtype = (ObjectType == "regkey") ? SE_REGISTRY_KEY : SE_FILE_OBJECT;
    SECURITY_INFORMATION SI = OWNER_SECURITY_INFORMATION;

    PSID ppsidOwner;
    PACL  ppDacl;
    PSECURITY_DESCRIPTOR ppSD;

    std::wstring str = UrlToPath(ObjectUrl).toStdWString();

    LPCWSTR pObjectName = str.c_str();

    DWORD retval = GetNamedSecurityInfoW(pObjectName,
                                        objtype,
                                        SI,
                                        &ppsidOwner,
                                        nullptr,
                                        &ppDacl,
                                        nullptr,
                                        &ppSD);

    if (retval != ERROR_SUCCESS)
    {
        SetLastError(retval);
        LocalFree(ppSD);
        return -1;
    }

    DWORD NameBufferSize = 512;
    DWORD DomainBufferSize = 512;
    WCHAR Name[512];
    WCHAR DomainName[512];
    SID_NAME_USE eSidType;

    if (0 == LookupAccountSidW(nullptr,
                ppsidOwner,
                Name,
                &NameBufferSize,
                DomainName,
                &DomainBufferSize,
                &eSidType))
    {
        LocalFree(ppSD);
        return -1;
    }

    if (ppsidOwner == nullptr)  SID = "No SID";
    else
    {
        LPSTR StringSid = new CHAR[512];
        ConvertSidToStringSidA(ppsidOwner, &StringSid);

        SID   = QString(StringSid);
        LocalFree(StringSid);
    }

    Owner = QString::fromWCharArray(DomainName) + "/" + QString::fromWCharArray(Name);

    LocalFree(ppSD);
    return 0;
}

int SObject::SetOwner(LPCTSTR objname, LPCTSTR newuser, QString &NewSid, QString &Domain)
{
    HANDLE token;
    if (0 != OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token))
    {
            SetPrivilege(token, TEXT("SeTakeOwnershipPrivilege"), TRUE);
            SetPrivilege(token, TEXT("SeSecurityPrivilege"), TRUE);
            SetPrivilege(token, TEXT("SeBackupPrivilege"), TRUE);
            SetPrivilege(token, TEXT("SeRestorePrivilege"), TRUE);
    }
    else
    {
        return -1;
    }

    CloseHandle(token);

    DWORD len;
    GetFileSecurity(objname, OWNER_SECURITY_INFORMATION, nullptr, 0, &len);

    if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) return -1;

    PSECURITY_DESCRIPTOR security = (PSECURITY_DESCRIPTOR)malloc(sizeof(BYTE) * len);
    if (0 == InitializeSecurityDescriptor(security, SECURITY_DESCRIPTOR_REVISION))
    {
        LocalFree(security);
        return -1;
    }

    // Get the sid for the username
    DWORD domainbuf_size = 0;
    DWORD sidSize = 0;

    SID_NAME_USE sidUse;

     LookupAccountNameW(nullptr, newuser, nullptr,&sidSize,nullptr,&domainbuf_size, &sidUse);

     if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
     {
         LocalFree(security);
         return -1;
     }

     PBYTE sidPtr = new BYTE[sidSize];
     WCHAR *domainbuf = new WCHAR[domainbuf_size];

     if (0 == LookupAccountNameW(nullptr, newuser, sidPtr, &sidSize, domainbuf, &domainbuf_size, &sidUse))
     {
         LocalFree(security);
         delete [] domainbuf;
         delete [] sidPtr;
         return -1;
     }

     // Set the sid to be the new owner
     if (0 == SetSecurityDescriptorOwner(security, sidPtr, 0))
     {
        LocalFree(security);
        delete [] domainbuf;
        delete [] sidPtr;
        return -1;
     }

     // Save the security descriptor
     if (0 ==  SetFileSecurity(objname, OWNER_SECURITY_INFORMATION, security))
     {
         LocalFree(security);
         delete [] domainbuf;
         delete [] sidPtr;
         return -1;
     }

     LPWSTR SidString = new WCHAR[sidSize];
     ConvertSidToStringSidW(sidPtr, &SidString);

     NewSid = QString::fromWCharArray(SidString);
     Domain = QString::fromWCharArray(domainbuf);

     LocalFree(SidString);

     if (security)
         LocalFree(security);

     delete [] domainbuf;
     delete [] sidPtr;
     return 0;
}

int SObject::AddACE(const QUrl &ObjectUrl, LPCTSTR User, ACCESS_MASK AccessMask, BYTE AceType)
{
    setlocale(LC_ALL, "Russian");

    SECURITY_DESCRIPTOR *pSD;
    DWORD SD_Size;

    std::wstring str = UrlToPath(ObjectUrl).toStdWString();

    GetFileSecurity(str.c_str(), DACL_SECURITY_INFORMATION, nullptr, 0, &SD_Size);

    if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) return -1;

    pSD = (SECURITY_DESCRIPTOR*)malloc(SD_Size);
    if (0 == GetFileSecurity(str.c_str(), DACL_SECURITY_INFORMATION, pSD, SD_Size, &SD_Size))
    {
        LocalFree(pSD);
        return -1;
    }

    PACL oldDACL, newDACL;
    BOOL lpbDaclPresent, lpbDaclDefaulted;
    if (0 == GetSecurityDescriptorDacl(pSD, &lpbDaclPresent, &oldDACL, &lpbDaclDefaulted))
    {
       LocalFree(pSD);
       return -1;
    }

    EXPLICIT_ACCESS ex_access;

    ex_access.grfAccessPermissions = AccessMask;
    ex_access.grfAccessMode = (AceType == ACCESS_ALLOWED_ACE_TYPE) ? SET_ACCESS : DENY_ACCESS;
    ex_access.grfInheritance = NO_INHERITANCE;

    ex_access.Trustee.pMultipleTrustee = nullptr;
    ex_access.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
    ex_access.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
    ex_access.Trustee.TrusteeType = TRUSTEE_IS_UNKNOWN;
    ex_access.Trustee.ptstrName = const_cast<wchar_t*>(User);

    DWORD err_code = SetEntriesInAcl(1, &ex_access, oldDACL, &newDACL);

    if (err_code != ERROR_SUCCESS)
    {
        LocalFree(pSD);
        SetLastError(err_code);
        return -1;
    }

    SECURITY_DESCRIPTOR newSD;
    if (0 == InitializeSecurityDescriptor(&newSD, SECURITY_DESCRIPTOR_REVISION))
    {
        LocalFree(pSD);
        LocalFree(newDACL);
        return -1;
    }

    if (0 == SetSecurityDescriptorDacl(&newSD, TRUE, newDACL, FALSE))
    {
        LocalFree(pSD);
        LocalFree(newDACL);
        return -1;
    }

    if (0 == SetFileSecurity(str.c_str(), DACL_SECURITY_INFORMATION, &newSD))
    {
        LocalFree(pSD);
        LocalFree(newDACL);
        return -1;
    }

    LocalFree(pSD);
    LocalFree(newDACL);

    return 0;
}

int SObject::OpenNewObject(const QString &mode)
{
    QFileDialog Explorer(this);
    Explorer.setAcceptMode(QFileDialog::AcceptMode::AcceptOpen);
    Explorer.setViewMode(QFileDialog::ViewMode::Detail);
    Explorer.setFileMode((mode == "file") ? QFileDialog::ExistingFile : QFileDialog::DirectoryOnly);
    Explorer.setDirectory(QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation).first());

    if (Explorer.exec())
    {
        ObjectUrl = Explorer.selectedUrls().first();
        ObjectType = mode;

        UpdateIntLevel();
        UpdateOwnerInfo();
        UpdateAclInfo();

        this->show();
        return EXIT_SUCCESS;
    }

    return -1;
}

void SObject::UpdateAclInfo()
{
    QList<QTreeWidgetItem*> AclInfoVector;

    if (0 != GetAclInfo(ObjectUrl, ObjectType, AclInfoVector))
    {
        QMessageBox::warning(this,
                            "GetAclInfo error",
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
    else
    {
        for(int i = 0; i < ui->AclTreeWidget->topLevelItemCount(); i++)
        {
           QTreeWidgetItem *root = ui->AclTreeWidget->takeTopLevelItem(i);

           for (int i_child = 0; i_child < root->childCount(); i_child++)
           {
              delete root->takeChild(i_child);
           }

           delete root;
        }

        ui->AclTreeWidget->clear();
        ui->AclTreeWidget->addTopLevelItems(AclInfoVector);
    }
}

void SObject::UpdateOwnerInfo()
{
    QString Owner, SID;
    if (0 != GetOwnerInfo(ObjectUrl, ObjectType, Owner, SID))
    {
        Owner = "<unknown>";
        QMessageBox::warning(this,
                            "GetOwnerInfo error",
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
    else
    {
        ui->object_path->setText(UrlToPath(ObjectUrl));
        ui->owner->setText(Owner);
        ui->ownerSid->setText(SID);
    }
}

void SObject::UpdateIntLevel()
{
    QString int_level;
    if (0 != GetIntegrityLevel(ObjectUrl, int_level))
    {
        QMessageBox::warning(this,
                            "GetIntegrityLevel error",
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
    else
    {
        ui->integrity_tbutton->setText(int_level);
    }
}

QString SObject::UrlToPath(const QUrl &url)
{
    return url.path().replace(0, 1, "");
}

