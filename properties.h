#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <QTabWidget>

namespace Ui {
class Properties;
}

class QMenu;
struct Proc_info;

class Properties : public QTabWidget
{
    Q_OBJECT

public:
    explicit Properties(QSharedPointer<Proc_info> proc_info, QWidget *parent = nullptr);
    ~Properties();

    void ShowDLLs();
    void ShowPrivs();

public slots:
    void ShowSlot();

public:
    void OnOffPrivilege(int row, int column, bool flag);

private:
    Ui::Properties *ui;
    QSharedPointer<Proc_info> proc_info;

    QMenu *context_menu;
    QAction *act_en;
    QAction *act_dis;

    bool is_hconnected;
};

#endif // PROPERTIES_H
