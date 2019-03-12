#include "properties.h"
#include "ui_properties.h"
#include <QMessageBox>
#include <qdebug.h>

#include <QMenu>
#include <QAction>

#include "process.h"

Properties::Properties(QSharedPointer<Proc_info> proc_info, QWidget *parent)
    : QTabWidget(parent), ui(new Ui::Properties)
{
   this->proc_info = proc_info;
   ui->setupUi(this);

   ui->priv_table->horizontalHeader()->resizeSection(0, 330);
   ui->priv_table->horizontalHeader()->resizeSection(1, 169);
   ui->priv_table->setContextMenuPolicy(Qt::CustomContextMenu);
   ui->prop->setWindowTitle(QString("Process Properties, PID: %1").arg(proc_info->PPid()));

   context_menu = new QMenu();
   act_en = new QAction("Enable");
   act_dis = new QAction("Disable");

   context_menu->addAction(act_en);
   context_menu->addAction(act_dis);

   is_hconnected = false;

   /*Cell double clicked handler*/
   connect(ui->priv_table, &QTableWidget::cellDoubleClicked, [this](int row, int column)
   {
        if (column == 1) /*Value column*/
        {
            context_menu->popup(this->cursor().pos());

            if (is_hconnected == false)
            {
                connect(act_en,  &QAction::triggered, this, [this, row, column](bool){
                    this->OnOffPrivilege(row, column, TRUE);
                });
                connect(act_dis,  &QAction::triggered, this, [this, row, column](bool){
                    this->OnOffPrivilege(row, column, FALSE);
                });

                is_hconnected = true;
            }
        }
   });
}

Properties::~Properties()
{
    delete context_menu;
    delete act_en;
    delete act_dis;
    delete ui;
}

void Properties::ShowDLLs()
{
    QStringList dlls;
    if (0 == proc_info->GetAllDLLs(dlls))
    {
         ui->dll_list->addItems(dlls);
    }
    else
    {
        QMessageBox::warning(this,
                            "Show DLL's error, Pid: " + QString::number((int)proc_info->Pid()),
                             Proc_info::PrintAPIErrorMessage(GetLastError()),
                             QMessageBox::Ok);
    }
}

void Properties::ShowPrivs()
{
    QVector< QVector<QString> > Privs;
    if (0 == proc_info->GetPrivs(Privs))
    {
       for (auto vec : Privs)
       {
             QTableWidgetItem *name = new QTableWidgetItem(vec.first());
             QTableWidgetItem *value = new QTableWidgetItem(vec[1]);

             name->setFlags(name->flags() ^ Qt::ItemIsEditable);
             value->setFlags(value->flags() ^ Qt::ItemIsEditable);

             ui->priv_table->insertRow(ui->priv_table->rowCount());

             ui->priv_table->setItem(ui->priv_table->rowCount()-1, 0, name);
             ui->priv_table->setItem(ui->priv_table->rowCount()-1, 1, value);
       }
    }
    else
    {
         QMessageBox::warning(this,
                             "Show privileges error, Pid: " + QString::number((int)proc_info->Pid()),
                              Proc_info::PrintAPIErrorMessage(GetLastError()),
                              QMessageBox::Ok);
    }
}

void Properties::ShowSlot()
{
    ShowDLLs();
    ShowPrivs();
    this->show();
}

void Properties::OnOffPrivilege(int row, int column, bool flag)
{
   std::wstring wpriv_name = ui->priv_table->item(row, 0)->text().toStdWString();
   QString last_val = ui->priv_table->item(row, column)->text();

   DWORD pid = this->proc_info->Pid();

   HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
   if (hProcess == nullptr)
   {
         QMessageBox::warning(this,
                              "OpenProcess Fail, Pid: " + QString::number((int)pid),
                              Proc_info::PrintAPIErrorMessage(GetLastError()),
                              QMessageBox::Ok);
         return;
    }

   HANDLE hToken;
   if (0 == OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES, &hToken))
   {
       QMessageBox::warning(this,
                            "OpenProcessToken Fail, Pid: " + QString::number((int)pid),
                            Proc_info::PrintAPIErrorMessage(GetLastError()),
                            QMessageBox::Ok);

       CloseHandle(hProcess);
       return;
   }

    BOOL result = SetPrivilege(hToken, wpriv_name.c_str(), flag);

    CloseHandle(hProcess);
    CloseHandle(hToken);

    if (result == TRUE)
    {
       ui->priv_table->item(row, column)->setText((flag == TRUE) ? "Enabled" : "Disabled");
    }
    else
    {
         QMessageBox::warning(this,
                              "Set Privilege Error, Pid: " + QString::number((int)pid),
                              Proc_info::PrintAPIErrorMessage(GetLastError()),
                              QMessageBox::Ok);
         return;
    }
}
