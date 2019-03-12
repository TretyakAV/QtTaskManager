/********************************************************************************
** Form generated from reading UI file 'sobject.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SOBJECT_H
#define UI_SOBJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SObject
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_4;
    QLabel *integrity_label;
    QSpacerItem *horizontalSpacer_4;
    QToolButton *integrity_tbutton;
    QSpacerItem *horizontalSpacer_3;
    QHBoxLayout *horizontalLayout_3;
    QLabel *owner_label;
    QSpacerItem *horizontalSpacer;
    QLineEdit *owner;
    QHBoxLayout *horizontalLayout_5;
    QLabel *ownerSid_label;
    QSpacerItem *horizontalSpacer_5;
    QLineEdit *ownerSid;
    QHBoxLayout *horizontalLayout_2;
    QLabel *object_label;
    QSpacerItem *horizontalSpacer_2;
    QLineEdit *object_path;
    QVBoxLayout *verticalLayout_2;
    QTreeWidget *AclTreeWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_6;
    QPushButton *AddACEButton;
    QPushButton *DeleteACE_Button;
    QSpacerItem *horizontalSpacer_7;

    void setupUi(QWidget *SObject)
    {
        if (SObject->objectName().isEmpty())
            SObject->setObjectName(QStringLiteral("SObject"));
        SObject->resize(656, 529);
        verticalLayout_3 = new QVBoxLayout(SObject);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        integrity_label = new QLabel(SObject);
        integrity_label->setObjectName(QStringLiteral("integrity_label"));
        QFont font;
        font.setPointSize(11);
        integrity_label->setFont(font);

        horizontalLayout_4->addWidget(integrity_label);

        horizontalSpacer_4 = new QSpacerItem(1, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);

        integrity_tbutton = new QToolButton(SObject);
        integrity_tbutton->setObjectName(QStringLiteral("integrity_tbutton"));
        integrity_tbutton->setMinimumSize(QSize(195, 0));

        horizontalLayout_4->addWidget(integrity_tbutton);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_3);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        owner_label = new QLabel(SObject);
        owner_label->setObjectName(QStringLiteral("owner_label"));
        owner_label->setFont(font);

        horizontalLayout_3->addWidget(owner_label);

        horizontalSpacer = new QSpacerItem(50, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);

        owner = new QLineEdit(SObject);
        owner->setObjectName(QStringLiteral("owner"));
        QFont font1;
        font1.setPointSize(10);
        owner->setFont(font1);
        owner->setReadOnly(false);

        horizontalLayout_3->addWidget(owner);


        verticalLayout->addLayout(horizontalLayout_3);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        ownerSid_label = new QLabel(SObject);
        ownerSid_label->setObjectName(QStringLiteral("ownerSid_label"));
        ownerSid_label->setFont(font);

        horizontalLayout_5->addWidget(ownerSid_label);

        horizontalSpacer_5 = new QSpacerItem(22, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);

        ownerSid = new QLineEdit(SObject);
        ownerSid->setObjectName(QStringLiteral("ownerSid"));
        ownerSid->setFont(font1);
        ownerSid->setReadOnly(true);

        horizontalLayout_5->addWidget(ownerSid);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        object_label = new QLabel(SObject);
        object_label->setObjectName(QStringLiteral("object_label"));
        object_label->setFont(font);

        horizontalLayout_2->addWidget(object_label);

        horizontalSpacer_2 = new QSpacerItem(10, 20, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        object_path = new QLineEdit(SObject);
        object_path->setObjectName(QStringLiteral("object_path"));
        object_path->setEnabled(true);
        object_path->setFont(font1);
        object_path->setReadOnly(true);

        horizontalLayout_2->addWidget(object_path);


        verticalLayout->addLayout(horizontalLayout_2);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        AclTreeWidget = new QTreeWidget(SObject);
        QFont font2;
        font2.setBold(true);
        font2.setWeight(75);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setFont(3, font2);
        __qtreewidgetitem->setFont(2, font2);
        __qtreewidgetitem->setFont(1, font2);
        __qtreewidgetitem->setFont(0, font2);
        AclTreeWidget->setHeaderItem(__qtreewidgetitem);
        AclTreeWidget->setObjectName(QStringLiteral("AclTreeWidget"));

        verticalLayout_2->addWidget(AclTreeWidget);


        verticalLayout->addLayout(verticalLayout_2);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_6);

        AddACEButton = new QPushButton(SObject);
        AddACEButton->setObjectName(QStringLiteral("AddACEButton"));

        horizontalLayout->addWidget(AddACEButton);

        DeleteACE_Button = new QPushButton(SObject);
        DeleteACE_Button->setObjectName(QStringLiteral("DeleteACE_Button"));
        DeleteACE_Button->setEnabled(false);

        horizontalLayout->addWidget(DeleteACE_Button);

        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_7);


        verticalLayout->addLayout(horizontalLayout);


        verticalLayout_3->addLayout(verticalLayout);


        retranslateUi(SObject);

        QMetaObject::connectSlotsByName(SObject);
    } // setupUi

    void retranslateUi(QWidget *SObject)
    {
        SObject->setWindowTitle(QApplication::translate("SObject", "Object Information", nullptr));
        integrity_label->setText(QApplication::translate("SObject", "Integrity Level :", nullptr));
        integrity_tbutton->setText(QApplication::translate("SObject", "...", nullptr));
        owner_label->setText(QApplication::translate("SObject", "Owner :", nullptr));
        ownerSid_label->setText(QApplication::translate("SObject", "Owner SID :", nullptr));
        object_label->setText(QApplication::translate("SObject", "Object Path : ", nullptr));
        QTreeWidgetItem *___qtreewidgetitem = AclTreeWidget->headerItem();
        ___qtreewidgetitem->setText(3, QApplication::translate("SObject", "ACE Type", nullptr));
        ___qtreewidgetitem->setText(2, QApplication::translate("SObject", "SID Type", nullptr));
        ___qtreewidgetitem->setText(1, QApplication::translate("SObject", "User", nullptr));
        ___qtreewidgetitem->setText(0, QApplication::translate("SObject", "SID", nullptr));
        AddACEButton->setText(QApplication::translate("SObject", "Add ACE", nullptr));
        DeleteACE_Button->setText(QApplication::translate("SObject", "Delete ACE", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SObject: public Ui_SObject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SOBJECT_H
