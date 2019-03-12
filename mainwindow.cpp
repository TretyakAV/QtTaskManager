#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "sobject.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sobj = QSharedPointer<SObject>::create();

    connect(ui->OpenFile, &QAction::triggered, [this](bool){
        sobj->OpenNewObject("file");
    });
    connect(ui->OpenFolder, &QAction::triggered, [this](bool){
        sobj->OpenNewObject("folder");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
