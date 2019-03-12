#ifndef SOBJECT_H
#define SOBJECT_H

#include <QWidget>
#include <Windows.h>
#include <QUrl>

namespace Ui {
class SObject;
}

const char *AceTypeString(BYTE AceConstant);
QVector<QStringList> GetAcessRights(ACCESS_MASK access_mask);

class QTreeWidgetItem;
class AcePopUp;
class QMenu;
struct AceInfo;

class SObject : public QWidget
{
    Q_OBJECT

public:
    explicit SObject(QWidget *parent = nullptr);
    ~SObject() override;

    int OpenNewObject(const QString &);

    void UpdateAclInfo();
    void UpdateOwnerInfo();
    void UpdateIntLevel();

    void SetIntegrityLevelSlot(DWORD level);

    QString UrlToPath(const QUrl &url);

public slots:
    void editOwner();
    void AddAceSlot(AceInfo*);

private:
    int GetAclInfo(const QUrl &ObjectUrl, const QString &ObjectType, QList<QTreeWidgetItem*> &);
    int GetOwnerInfo(const QUrl &ObjectUrl, const QString &ObjectType, QString&, QString&);
    int SetOwner(LPCTSTR objname, LPCTSTR newuser, QString &NewSid, QString &Domain);

    int AddACE(const QUrl &ObjectUrl, LPCTSTR User, ACCESS_MASK AccessMask, BYTE AceType);
    int DeleteACE(int index);

    int GetIntegrityLevel(const QUrl &ObjectUrl, QString &IntegrityLevel);
    int SetIntegrityLevel(const QUrl &ObjectUrl, DWORD IntegrityLevel);

    Ui::SObject *ui;

    QSharedPointer<AcePopUp> AcePopUpPtr;

    QUrl ObjectUrl;
    QString ObjectType;

    QMenu *int_menu;
    QAction *low;
    QAction *hi;
    QAction *med;
};

#endif // SOBJECT_H
