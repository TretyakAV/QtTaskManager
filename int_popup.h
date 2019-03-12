#ifndef INT_POPUP_H
#define INT_POPUP_H

#include <QWidget>
#include <Windows.h>

namespace Ui {
class IntegrityPopup;
}

struct Proc_info;
class QMenu;

class IntegrityPopup : public QWidget
{
    Q_OBJECT

public:
    explicit IntegrityPopup(QSharedPointer<Proc_info> proc_info, QWidget *parent = nullptr);
    ~IntegrityPopup();

    void SetIntegrityLevelSlot(DWORD level);

public slots:
    void ShowIntegrityLevel();

private:
    Ui::IntegrityPopup *ui;
    QSharedPointer<Proc_info> proc_info;

     QMenu *int_menu;
     QAction *unt;
     QAction *low;
     QAction *hi;
     QAction *med;
     QAction *sys;
};

#endif // INT_POPUP_H
