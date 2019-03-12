/********************************************************************************
** Form generated from reading UI file 'properties.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PROPERTIES_H
#define UI_PROPERTIES_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Properties
{
public:
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *prop;
    QWidget *dlls;
    QVBoxLayout *verticalLayout_4;
    QVBoxLayout *verticalLayout_2;
    QListWidget *dll_list;
    QWidget *privs;
    QHBoxLayout *horizontalLayout_2;
    QTableWidget *priv_table;

    void setupUi(QWidget *Properties)
    {
        if (Properties->objectName().isEmpty())
            Properties->setObjectName(QStringLiteral("Properties"));
        Properties->setEnabled(true);
        Properties->resize(575, 428);
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(Properties->sizePolicy().hasHeightForWidth());
        Properties->setSizePolicy(sizePolicy);
        horizontalLayout_3 = new QHBoxLayout(Properties);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        prop = new QTabWidget(Properties);
        prop->setObjectName(QStringLiteral("prop"));
        dlls = new QWidget();
        dlls->setObjectName(QStringLiteral("dlls"));
        verticalLayout_4 = new QVBoxLayout(dlls);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        dll_list = new QListWidget(dlls);
        dll_list->setObjectName(QStringLiteral("dll_list"));

        verticalLayout_2->addWidget(dll_list);


        verticalLayout_4->addLayout(verticalLayout_2);

        prop->addTab(dlls, QString());
        privs = new QWidget();
        privs->setObjectName(QStringLiteral("privs"));
        horizontalLayout_2 = new QHBoxLayout(privs);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        priv_table = new QTableWidget(privs);
        if (priv_table->columnCount() < 2)
            priv_table->setColumnCount(2);
        QFont font;
        font.setBold(true);
        font.setWeight(75);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        __qtablewidgetitem->setTextAlignment(Qt::AlignCenter);
        __qtablewidgetitem->setFont(font);
        priv_table->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QFont font1;
        font1.setBold(true);
        font1.setItalic(false);
        font1.setUnderline(false);
        font1.setWeight(75);
        font1.setStrikeOut(false);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        __qtablewidgetitem1->setTextAlignment(Qt::AlignHCenter|Qt::AlignTop);
        __qtablewidgetitem1->setFont(font1);
        priv_table->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        priv_table->setObjectName(QStringLiteral("priv_table"));
        priv_table->setMinimumSize(QSize(535, 365));
        priv_table->setMaximumSize(QSize(535, 16777215));
        priv_table->setColumnCount(2);
        priv_table->horizontalHeader()->setMinimumSectionSize(50);
        priv_table->verticalHeader()->setMinimumSectionSize(50);

        horizontalLayout_2->addWidget(priv_table);

        prop->addTab(privs, QString());

        horizontalLayout_3->addWidget(prop);


        retranslateUi(Properties);

        prop->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(Properties);
    } // setupUi

    void retranslateUi(QWidget *Properties)
    {
        Properties->setWindowTitle(QApplication::translate("Properties", "Process Properties", nullptr));
        prop->setTabText(prop->indexOf(dlls), QApplication::translate("Properties", "DLL's", nullptr));
        QTableWidgetItem *___qtablewidgetitem = priv_table->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QApplication::translate("Properties", "Name", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = priv_table->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QApplication::translate("Properties", "Value", nullptr));
        prop->setTabText(prop->indexOf(privs), QApplication::translate("Properties", "Privileges", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Properties: public Ui_Properties {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PROPERTIES_H
