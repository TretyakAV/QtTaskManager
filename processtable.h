#ifndef PROCESSTABLE_H
#define PROCESSTABLE_H

#include <QSharedPointer>
#include <QTreeWidget>
#include <QList>
#include "process.h"


class ProcessTable : public QTreeWidget
{
    Q_OBJECT
public:
    using ProcessPtr = QSharedPointer<Process>;
    using QTWidgetItemPtr = QSharedPointer<QTreeWidgetItem>;

    explicit ProcessTable(QWidget *parent = nullptr);

    void mousePressEvent(QMouseEvent *) override;

    void ShowProcesses();

signals:
    void RequestContextMenu(DWORD, QPoint);

private:
    void GetProcList();

     QList<ProcessPtr> ProcList;
};

#endif // PROCESSTABLE_H
