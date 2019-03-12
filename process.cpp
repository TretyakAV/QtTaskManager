#include "process.h"
#include <locale.h>
#include <qdebug.h>

#include <QMenu>
#include <QListWidget>
#include <QAction>

#include <vector>
#include <string>
#include <algorithm>

#include "int_popup.h"
#include "properties.h"

#define SELF 0
#define PARENT 1

Process::Process(QTreeWidget *parent) : QObject(), QTreeWidgetItem(parent)
{
}

Process::Process(DWORD pid, QTreeWidget *parent)
       : QObject(), QTreeWidgetItem(parent)
{
    setlocale(LC_ALL, "Russian");

    info = QSharedPointer<Proc_info>::create(pid);

    prop =  QSharedPointer<Properties>::create(info);
    int_popup = QSharedPointer<IntegrityPopup>::create(info);

    /* Create an object context menu */
    cmenu = QSharedPointer<QMenu>::create();

    /* Create actions to the context menu */
    properties = QSharedPointer<QAction>::create("Properties");
    int_level = QSharedPointer<QAction>::create("Integrity Level");

    connect(properties.get(), &QAction::triggered, prop.get(), &Properties::ShowSlot);
    connect(int_level.get(), &QAction::triggered, int_popup.get(), &IntegrityPopup::ShowIntegrityLevel);

    cmenu->addAction(properties.get());
    cmenu->addAction(int_level.get());
}

void Process::OpenContextMenu(DWORD pid, QPoint qpoint)
{
   if (pid != info->Pid()) return;
   else
   {
       cmenu->popup(qpoint);
   }
}

int Proc_info::GetPPID()
{
    HANDLE hSnapshot;
    PROCESSENTRY32 pe32;
    DWORD ppid = 0;

    hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    try
    {
        if (hSnapshot == INVALID_HANDLE_VALUE)
                throw std::exception();

        ZeroMemory(&pe32, sizeof(pe32));
        pe32.dwSize = sizeof(pe32);

        if (!Process32First(hSnapshot, &pe32))
                throw std::exception();
        do
        {
               if (pe32.th32ProcessID == PID)
               {
                    ppid = pe32.th32ParentProcessID;
                    break;
               }
        }
        while (Process32Next(hSnapshot, &pe32));

//         if (ppid == 0)
//               throw std::exception();
    }
    catch (const std::exception &)
    {
        if (hSnapshot != INVALID_HANDLE_VALUE)
                CloseHandle(hSnapshot);

        return -1;
    }

    PPID = ppid;
    return 0;
}

int Proc_info::GetProcessPath()
{
        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

        // Get the process name.
        if (nullptr != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod),
                &cbNeeded))
            {
                WCHAR wPath[2048];
                if (GetModuleFileNameExW(hProcess, hMod, wPath,
                    sizeof(wPath) / sizeof(WCHAR)) == 0)
                {
                    return -1;
                }

                Path = QString::fromWCharArray(wPath);
            }
            else  return -1;
        }
        else return -1;

        // Release the handle to the process.
        CloseHandle(hProcess);
        return 0;
}

int Proc_info::GetProcessName(DWORD _PID, int flag)
{
        // Get a handle to the process.
        HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, _PID);

        // Get the process name.
        if (nullptr != hProcess)
        {
            HMODULE hMod;
            DWORD cbNeeded;

            if (EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded))
            {
                if (flag == SELF)
                {
                    char _Name[MAX_PATH];
                    GetModuleBaseNameA(hProcess, hMod, _Name, sizeof(_Name) / sizeof(CHAR));
                    Name = QString(_Name);
                }
                else if (flag == PARENT)
                {
                    char _ParentName[MAX_PATH];
                    GetModuleBaseNameA(hProcess, hMod, _ParentName, sizeof(_ParentName) / sizeof(CHAR));
                    ParentName = QString(_ParentName);
                }
            }
            else return -1;
        }
        else
        {
            //qDebug() << PID;
            return -1;
        }

        // Release the handle to the process.
        CloseHandle(hProcess);
        return 0;
}

int Proc_info::GetProcOwner()
{
    HANDLE hProcess;
    if ((hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)) == NULL) return -1;

    HANDLE hToken;
    if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))  return -1;

    BYTE buf[512];
    DWORD cb = sizeof(buf);

    if (GetTokenInformation(hToken, TokenUser, buf, cb, &cb) == 0)
        return -1;

    TCHAR szName[MAX_PATH];
    DWORD dwName = MAX_PATH;
    TCHAR szDomain[MAX_PATH];
    DWORD dwDomain = MAX_PATH;
    SID_NAME_USE snu;

    if (0 == LookupAccountSidW
            (NULL,
             ((TOKEN_USER *)(buf))->User.Sid,
             szName,
             &dwName,
             szDomain,
             &dwDomain,
             &snu))
    {
        return -1;
    }

    PSID pSid = ((TOKEN_USER *)(buf))->User.Sid;

   owner_info.Name = QString::fromWCharArray(szName);
   owner_info.Domain = QString::fromWCharArray(szDomain);

    if (pSid != NULL)
    {
        LPSTR sStringSid = NULL;
        if (ConvertSidToStringSidA(pSid, &sStringSid))
        {
           owner_info.sSID = QString(sStringSid);
           LocalFree(sStringSid);
        }
        else
        {
            CloseHandle(hToken);
            CloseHandle(hProcess);
            return -1;
        }

        CloseHandle(hToken);
        CloseHandle(hProcess);
        return 0;
    }
    else return -1;
}

int Proc_info::GetProcType()
{
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
            PROCESS_VM_READ,
            FALSE, PID);
        bool isX86;

    auto IsWow64 = [] (HANDLE proc) -> BOOL
    {
        BOOL bIsWow64 = FALSE;

        typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
        LPFN_ISWOW64PROCESS fnIsWow64Process;
        fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

        if (nullptr != fnIsWow64Process)
        {
            if (!fnIsWow64Process(proc, &bIsWow64))
            {
                throw std::exception();
            }
        }
        return bIsWow64;
    };
    auto IsX86Process = [&IsWow64] (HANDLE proc) -> BOOL
    {
        SYSTEM_INFO systemInfo = { 0 };
        GetNativeSystemInfo(&systemInfo);

        // x86 environment
        if (systemInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_INTEL)
            return true;

        // Check if the process is an x86 process that is running on x64 environment.
        // IsWow64 returns true if the process is an x86 process
        return IsWow64(proc);
    };

        if (nullptr != hProcess)
        {
            try {
                isX86 = IsX86Process(hProcess);
            } catch (const std::exception &)
            {
                return -1;
            }

            if (isX86 == true)
               Type = "x86";
            else
               Type = "x64";
        }
        else return -1;

        CloseHandle(hProcess);
        return 0;
}

int Proc_info::GetDepInfo()
{
        HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, PID);

        if (NULL == hProcess)
        {
            return -1;
        }

        DWORD Flags;
        BOOL Permanent = FALSE;

        if (FALSE == GetProcessDEPPolicy(hProcess, &Flags, &Permanent))
        {
            CloseHandle(hProcess);

            if (Type == "x64")
            {
                stDEP = "Enabled (permanent)";
                return 0;
            }

            return -1;
        }
        else
        {
                if (Flags == 0)
                   stDEP = (Permanent == TRUE) ? "Disabled (permanent)" : "Disabled";
                else if (Flags == 0x00000001 || Flags == 3)
                   stDEP = (Permanent == TRUE) ? "Enabled (permanent)" : "Enabled";
                else if (Flags == 0x00000002)
                   stDEP = "DEP-ATL thunk emulation is disabled for the specified process.";
        }

        CloseHandle(hProcess);
        return 0;
}

int Proc_info::GetAslrInfo()
{
    IMAGE_DOS_HEADER pDos = {0};
    IMAGE_NT_HEADERS pNT  = {0};
    void *BaseAddress;

    // create module snapshot
    MODULEENTRY32 ME32;
    HANDLE hModule  = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, PID);

    if (INVALID_HANDLE_VALUE == hModule) return -1;

    ME32.dwSize = sizeof(ME32);

    if( Module32First( hModule, &ME32 ) == FALSE )
    {
        CloseHandle(hModule);
        return -1;
    }

    CloseHandle(hModule);

    // get base address of my module
    BaseAddress = ME32.modBaseAddr;

    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, PID);

    if (hProcess == NULL) return -1;

    // read BaseAddress and set the IMAGE_DOS_HEADER struct
    if(0 == ReadProcessMemory(hProcess, BaseAddress, (char*)&pDos, sizeof(IMAGE_DOS_HEADER), 0))
    {
         CloseHandle(hProcess);
         return -1;
    }

    // BaseAddress + e_lfanew points to the NT Header struct, I read it here
    if(0 == ReadProcessMemory(hProcess, (void*)((unsigned long)BaseAddress + pDos.e_lfanew), &pNT, sizeof(IMAGE_NT_HEADERS), 0))
    {
        CloseHandle(hProcess);
        return -1;
    }

    CloseHandle(hProcess);

    if (pNT.Signature == IMAGE_NT_SIGNATURE) // this condition returns TRUE
    {
        if (pNT.OptionalHeader.DllCharacteristics & IMAGE_DLLCHARACTERISTICS_DYNAMIC_BASE)
            stASLR = "Enabled";
        else
            stASLR = "Disabled";
        return 0;
    }

    return -1;
}

int Proc_info::GetAllDLLs(QStringList &DLLs)
{
    HMODULE hMods[1024];
    HANDLE hProcess;
    DWORD cbNeeded;
    unsigned int i;

    // Get a handle to the process.
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, PID);

    if (NULL == hProcess) return -1;

    auto CutOffName = [](std::string &name) -> void
    {
        size_t pos = name.find_last_of('\\');
        name.replace(0, pos + 1, "");
    };

     // Get a list of all the modules in this process.
     if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
     {
            for (i = 0; i < (cbNeeded / sizeof(HMODULE)); i++)
            {
                TCHAR szModName[MAX_PATH];

                // Get the full path to the module's file.

                if (GetModuleFileNameEx(hProcess, hMods[i], szModName,
                    sizeof(szModName) / sizeof(TCHAR)))
                {
                    // Print the module name and handle value.
                    std::string str = QString::fromWCharArray(szModName).toStdString();
                    CutOffName(str);

                    DLLs.push_back(QString::fromStdString(str));
                }
            }
     }
     else
     {
         CloseHandle(hProcess);
         return -1;
     }

    std::sort(DLLs.begin(), DLLs.end());

    // Release the handle to the process.
    CloseHandle(hProcess);

    return 0;
}

int Proc_info::GetPrivs(QVector< QVector<QString> > &Privs)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS,
            FALSE, PID);

        if (nullptr == hProcess)   return -1;

        HANDLE hToken;
        OpenProcessToken(hProcess, TOKEN_QUERY, &hToken);

        if (hToken == nullptr)
        {
            CloseHandle(hProcess);
            return -1;
         }

        DWORD cb = 2048;
        CHAR * buf[2048];
        if (GetTokenInformation(hToken, TokenPrivileges, &buf, cb, &cb))
        {
            for (int i = 0; i < ((_TOKEN_PRIVILEGES*)buf)->PrivilegeCount; i++)
            {
               DWORD dwSize = 0;
               LookupPrivilegeNameA(nullptr,
                                    &((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Luid,
                                    nullptr,
                                    &dwSize);

                LPSTR szName = new CHAR[1024];
                if (0 == LookupPrivilegeNameA(nullptr,
                                               &((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Luid,
                                               szName,
                                               &dwSize))
                {
                    return -1;
                }

                std::string str = szName;

                Privs.resize(i + 1);
                Privs[i].push_back(QString::fromStdString(str));

                // Display the privilege state.
                if (((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED)
                {
                    Privs[i].push_back("Enabled");
                }
                else if (((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Attributes & SE_PRIVILEGE_ENABLED_BY_DEFAULT)
                    Privs[i].push_back("Enabled by default");

                else if (((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Attributes & SE_PRIVILEGE_REMOVED)
                    Privs[i].push_back("Removed.");

                else if (((_TOKEN_PRIVILEGES*)buf)->Privileges[i].Attributes & SE_PRIVILEGE_USED_FOR_ACCESS)
                    Privs[i].push_back("Used for access");

                else
                    Privs[i].push_back("Disabled");
            }

        }
        else
        {
            CloseHandle(hToken);
            CloseHandle(hProcess);
            return -1;
        }

        CloseHandle(hToken);
        CloseHandle(hProcess);
        return 0;
}

int Proc_info::GetIntegrityLevel(QString &Integrity_lvl)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (hProcess == nullptr)    return -1;

    HANDLE hToken;
    if (0 == OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
    {
        CloseHandle(hProcess);
        return -1;
    }

    if (hToken == nullptr)
    {
        CloseHandle(hProcess);
        return -1;
    }

    BYTE buf[512];
    DWORD cb = sizeof(buf);

    if (GetTokenInformation(hToken, TokenIntegrityLevel, buf, cb, &cb))
    {
        PDWORD err = GetSidSubAuthority(((TOKEN_MANDATORY_LABEL*)(buf))->Label.Sid,
            (DWORD)(UCHAR)(*GetSidSubAuthorityCount(((TOKEN_MANDATORY_LABEL*)(buf))->Label.Sid) - 1));

        if (err == nullptr)
        {
            qDebug() << "536";
            return -1;
         }

        DWORD dwIntegrityLevel = *err;
        if (dwIntegrityLevel == SECURITY_MANDATORY_LOW_RID)
        {
            Integrity_lvl = "Low Process";
        }
        else if (dwIntegrityLevel == SECURITY_MANDATORY_UNTRUSTED_RID)
        {
            Integrity_lvl = "Untrusted Process";
        }
        else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID && dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
        {
            Integrity_lvl = "Medium Process";
        }
        else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
        {
            Integrity_lvl = "High Integrity Process";
        }
        else if (dwIntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID)
        {
            Integrity_lvl = "System Integrity Process";
        }
        else
        {
            CloseHandle(hToken);
            CloseHandle(hProcess);
            return -1;
        }
    }
    else
    {
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return -1;
    }

    CloseHandle(hToken);
    CloseHandle(hProcess);
    return 0;
}

int Proc_info::SetIntegrityLevel(DWORD level)
{
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);

    if (nullptr == hProcess)
    {
        return -1;
    }

    HANDLE hToken;
    if (FALSE == OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken))
    {
        CloseHandle(hProcess);
        return -1;
    }

    if (hToken == nullptr)
    {
        CloseHandle(hProcess);
        return -1;
    }

    BYTE buf[512];
    DWORD cb = sizeof(buf);

    if (GetTokenInformation(hToken, TokenIntegrityLevel,
        buf, cb, &cb))
    {
        std::string sidStr;

        switch (level)
        {
        case SECURITY_MANDATORY_UNTRUSTED_RID:
            sidStr = "S-1-16-0";
            break;
        case SECURITY_MANDATORY_LOW_RID:
            sidStr = "S-1-16-4096";
            break;
        case SECURITY_MANDATORY_MEDIUM_RID:
            sidStr = "S-1-16-8192";
            break;
        case SECURITY_MANDATORY_HIGH_RID:
            sidStr = "S-1-16-12288";
            break;
        case SECURITY_MANDATORY_SYSTEM_RID:
            sidStr = "S-1-16-16384";
            break;
        default:
            break;
        }

        PSID NewSid;

        bool err = ConvertStringSidToSidA(
            sidStr.c_str(),
            &NewSid
        );

        if (err == FALSE)
        {
            CloseHandle(hToken);
            CloseHandle(hProcess);
            return -1;
        }

        TOKEN_MANDATORY_LABEL tml = { 0 };
        tml.Label.Attributes = SE_GROUP_INTEGRITY;
        tml.Label.Sid = NewSid;

        if (SetTokenInformation(hToken, TokenIntegrityLevel, &tml, sizeof(TOKEN_MANDATORY_LABEL) + ::GetSidLengthRequired(1)) == 0)
        {
            CloseHandle(hToken);
            CloseHandle(hProcess);

            qDebug() << "643";

            return -1;
        }

    }
    else
    {
        CloseHandle(hToken);
        CloseHandle(hProcess);
        return -1;
    }

    CloseHandle(hToken);
    CloseHandle(hProcess);
    return 0;
}

Proc_info::Proc_info(DWORD pid)
{
    PID = pid;
    UpdateProcInfo();
}

void Proc_info::UpdateProcInfo()
{
    if (GetProcessName(PID, SELF) != 0)
    {
        Name = "<unknown>";
        qDebug() << "GetProcessName(SELF). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetProcessPath() != 0)
    {
        Path = "<unknown>";
        qDebug() << "GetProcessPath(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetPPID() != 0)
    {
        PPID = 0;
        qDebug() << "GetPPID(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetProcessName(PPID, PARENT) != 0)
    {
        ParentName = "<non-existent process>";
        qDebug() << "GetProcessName(PARENT). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetProcOwner() != 0)
    {
       owner_info.Domain = "<unknown>";
       owner_info.Name  = "<unknown>";
       owner_info.sSID = "<unknown>";
       qDebug() << "GetProcOwner(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetProcType() != 0)
    {
        Type = "<unknown>";
        qDebug() << "GetProcType(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetDepInfo() != 0)
    {
        stDEP = "<unknown>";
        qDebug() << "GetDepInfo(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }

    if (GetAslrInfo() != 0)
    {
        stASLR = "<unknown>";
        qDebug() << "GetAslrInfo(). Pid: " << PID << " -- " << PrintAPIErrorMessage(GetLastError()).trimmed();
    }


}
