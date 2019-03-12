#include "acepopup.h"
#include "ui_acepopup.h"

#include <QPushButton>
#include <QCheckBox>

AcePopUp::AcePopUp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AcePopUp)
{
    ui->setupUi(this);

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &AcePopUp::TakeAceInformation);
    connect(ui->buttonBox->button(QDialogButtonBox::Cancel), &QPushButton::clicked, [this]()
    {
        ui->Username->clear();
        close();
    });
}

AcePopUp::~AcePopUp()
{
    delete ui;
}

void AcePopUp::TakeAceInformation()
{
    DWORD AccessRights[]
    {
        DELETE,
        GENERIC_ALL,
        GENERIC_EXECUTE,
        GENERIC_READ,
        GENERIC_WRITE,
        READ_CONTROL,
        SPECIFIC_RIGHTS_ALL,
        STANDARD_RIGHTS_ALL,
        STANDARD_RIGHTS_EXECUTE,
        STANDARD_RIGHTS_READ,
        STANDARD_RIGHTS_REQUIRED,
        STANDARD_RIGHTS_WRITE,
        SYNCHRONIZE,
        WRITE_DAC,
        WRITE_OWNER
    };

    QVector<QCheckBox*> AccessRightsCBoxes =
    {
       ui->_DELETE,
       ui->_GENERIC_ALL,
       ui->_GENERIC_EXECUTE,
       ui->_GENERIC_READ,
       ui->_GENERIC_WRITE,
       ui->_READ_CONTROL,
       ui->_SPECIFIC_RIGHTS_ALL,
       ui->_STANDARD_RIGHTS_ALL,
       ui->_STANDARD_RIGHTS_EXECUTE,
       ui->_STANDARD_RIGHTS_READ,
       ui->_STANDARD_RIGHTS_REQUIRED,
       ui->_STANDARD_RIGHTS_WRITE,
       ui->_SYNCHRONIZE,
       ui->_WRITE_DAC,
       ui->_WRITE_OWNER
    };

   AceInfo *AceInfo_ptr = new AceInfo();

   AceInfo_ptr->AceType = (ui->AccessAllowed->isChecked() == true) ? ACCESS_ALLOWED_ACE_TYPE :
                                                                      ACCESS_DENIED_ACE_TYPE;
   AceInfo_ptr->AccessMask = 0;
   for (int i = 0; i < AccessRightsCBoxes.size(); i++)
   {
       AceInfo_ptr->AccessMask |= (AccessRightsCBoxes[i]->isChecked() == true) ? AccessRights[i] : 0;
   }

   AceInfo_ptr->Username = ui->Username->text();

   /*In the end of ACL*/
   AceInfo_ptr->dwStartingAceIndex = MAXDWORD;

   this->close();

   emit SendAceInformation(AceInfo_ptr);

   return;
}
