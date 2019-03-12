#ifndef PROCESS_H
#define PROCESS_H

#include <QWidget>
#include <QObject>
#include <QTreeWidget>

#include <Windows.h>
#include <Psapi.h>
#include <tchar.h>
#include <TlHelp32.h>
#include <sddl.h>

#include <QSharedPointer>

#pragma comment(lib, "advapi32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "ntdll.lib")


BOOL SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
BOOL SetDebugPrivilege(HANDLE Pid, BOOL flag);

struct uOwner
{
    QString Name;
    QString Domain;
    QString sSID;
};

struct Proc_info
{
public:
    Proc_info(DWORD pid);

    QString parentName() {return ParentName;}
    QString pName(){ return Name;          }
    QString pPath(){ return Path;          }
    DWORD   Pid()  { return PID;           }
    DWORD   PPid() { return PPID;          }
    QString pType(){ return Type;          }

    QString pDEP()  { return stDEP;         }
    QString pASLR() { return stASLR;   }

    QString oSid() { return owner_info.sSID; }
    QString oName() { return owner_info.Name;}
    QString oDomain() { return owner_info.Domain;}

    void UpdateProcInfo();
    static QString PrintAPIErrorMessage(DWORD dwErr);

    int GetAllDLLs(QStringList &);
    int GetPrivs(QVector<QVector<QString>> &);
    int GetIntegrityLevel(QString &);
    int SetIntegrityLevel(DWORD level);

private:
    Proc_info(){}

    int GetPPID();
    int GetProcessPath();
    int GetProcessName(DWORD, int);
    int GetProcOwner();
    int GetProcType();

    int GetDepInfo();
    int GetAslrInfo();

    QString ParentName;
    QString Name;
    QString Path;
    DWORD PID;
    DWORD PPID;
    QString Type;

    QString stDEP;
    QString stASLR;

    uOwner owner_info;
};

class IntegrityPopup;
class Properties;

class Process : public QObject, public QTreeWidgetItem
{
    Q_OBJECT
     explicit Process(QTreeWidget *parent = nullptr);

public:
    Process(DWORD pid, QTreeWidget *parent = nullptr);
    QSharedPointer<Proc_info> info;

public slots:
    void OpenContextMenu(DWORD, QPoint);

private:
    QSharedPointer<Properties>  prop;
    QSharedPointer<IntegrityPopup> int_popup;

    QSharedPointer<QMenu> cmenu;
    QSharedPointer<QAction> properties;
    QSharedPointer<QAction> int_level;
};

#endif // PROCESS_H
