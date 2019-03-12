#include "processtable.h"
#include <Windows.h>
#include <locale.h>
#include <qdebug.h>

ProcessTable::ProcessTable(QWidget *parent) : QTreeWidget(parent)
{
    setlocale(LC_ALL, "Russian");

    //Set the context menu
    this->setContextMenuPolicy(Qt::CustomContextMenu);

    //Set SE_DEBUG, SE_SECURITY_NAME  privilege
    SetDebugPrivilege(GetCurrentProcess(), TRUE);

    try
    {
        GetProcList();
        ShowProcesses();
    }
    catch (const std::exception &ex)
    {
        qDebug() << ex.what();
    }

}

void ProcessTable::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
    {
        QTreeWidgetItem *item = this->itemAt(e->pos());

        if (item == nullptr) return;

        QVariant data =  item->data(0, Qt::UserRole);
        ProcessPtr p = data.value<ProcessPtr>();

        if (p != nullptr)
            emit RequestContextMenu(p->info->Pid(), this->viewport()->mapToGlobal(e->pos()));
    }
    else if (e->button() == Qt::LeftButton)
    {
        QTreeWidgetItem *cur = this->itemAt(e->pos());

        cur->setExpanded(!cur->isExpanded());
    }
}


void ProcessTable::ShowProcesses()
{
    this->clear();
    for (auto p : ProcList)
    {
       QTreeWidgetItem *item = new QTreeWidgetItem
                (
                    this,
                    QStringList
                    (
                        {p->info->pName(),
                         QString::number(p->info->Pid()),
                         p->info->pType(),
                         p->info->oName(),
                         p->info->oSid()}
                     )
                 );

       /*
        * 1. Path.              +
        * 2. Parent name.       +
        * 3. Parent PID.        +
        * 4. DEP, ASLR using.   +
        * 5. DLL list.          +
        */

       //Set Process Pointer
      QVariant var; var.setValue(p);

      item->setData(0, Qt::UserRole, var);

      QList<QTreeWidgetItem*> child_list =
      {
          new QTreeWidgetItem(item, QStringList({"Path:", p->info->pPath()})),
          new QTreeWidgetItem(item, QStringList({"PPID:", QString::number(p->info->PPid())})),
          new QTreeWidgetItem(item, QStringList({"Parent process:", p->info->parentName()})),

          new QTreeWidgetItem(item, QStringList({"DEP policy:", p->info->pDEP()})),
          new QTreeWidgetItem(item, QStringList({"ASLR policy:", p->info->pASLR()}))
      };

       item->addChildren(child_list);
       this->addTopLevelItem(item);
    }
}

void ProcessTable::GetProcList()
{
    DWORD ProcessesPID[2048];
    DWORD CountProc;

    if (!EnumProcesses(ProcessesPID, sizeof(ProcessesPID), &CountProc))
    {
        throw std::exception("GetProcList");
    }

    CountProc /= sizeof(DWORD);

    for (DWORD i = 0 ; i < CountProc; i++)
    {
        auto pp = ProcessPtr::create(ProcessesPID[i]);

        connect(this, SIGNAL(RequestContextMenu(DWORD, QPoint)),
                pp.get(), SLOT(OpenContextMenu(DWORD, QPoint)));

        ProcList.append(pp);
    }
}


