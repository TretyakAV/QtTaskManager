#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "processtable.h"

namespace Ui {
class MainWindow;
}

class SObject;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private:
    Ui::MainWindow *ui;
    QSharedPointer<SObject> sobj;
};

#endif // MAINWINDOW_H
