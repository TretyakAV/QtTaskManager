#include "int_popup.h"
#include "process.h"
#include "ui_integritypopup.h"

#include <QMenu>
#include <QAction>
#include <QMessageBox>

IntegrityPopup::IntegrityPopup(QSharedPointer<Proc_info> proc_info, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::IntegrityPopup)
{
    this->proc_info = proc_info;
    ui->setupUi(this);

    connect(ui->toolButton, &QToolButton::clicked, ui->toolButton, &QToolButton::showMenu);
    connect(ui->okButton,   &QPushButton::clicked, this, &QWidget::close);

    int_menu = new QMenu(this);
    unt = new QAction("Untrusted Process");
    low = new QAction("Low Process");
    med = new QAction("Medium Process");
    hi = new QAction("High Integrity Process");
    sys = new QAction("System Integrity Process");

    connect(unt, &QAction::triggered, [this](bool){
         SetIntegrityLevelSlot(SECURITY_MANDATORY_UNTRUSTED_RID);
    });
    connect(low, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_LOW_RID);
    });
    connect(med, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_MEDIUM_RID);
    });
    connect(hi, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_HIGH_RID);
    });
    connect(sys, &QAction::triggered, [this](bool){
        SetIntegrityLevelSlot(SECURITY_MANDATORY_SYSTEM_RID);
    });

    int_menu->addAction(unt);
    int_menu->addAction(low);
    int_menu->addAction(med);
    int_menu->addAction(hi);
    int_menu->addAction(sys);

    ui->toolButton->setMenu(int_menu);
}

IntegrityPopup::~IntegrityPopup()
{
    delete unt;
    delete int_menu;
    delete low;
    delete med;
    delete hi;
    delete sys;
    delete ui;
}

void IntegrityPopup::SetIntegrityLevelSlot(DWORD level)
{
    if (0 == proc_info->SetIntegrityLevel(level))
    {
        ShowIntegrityLevel();
    }
    else
    {
        QMessageBox::warning(this,
                            "Set Integrity Level Fail, Pid: " + QString::number((int)proc_info->Pid()),
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }

}

void IntegrityPopup::ShowIntegrityLevel()
{
    QString int_level;
    if (0 == proc_info->GetIntegrityLevel(int_level))
    {
        ui->toolButton->setText(int_level);
    }
    else
    {
        QMessageBox::warning(this,
                             "Show Integrity Level Fail, Pid: " + QString::number((int)proc_info->Pid()),
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
        return;
    }

    this->show();
}

