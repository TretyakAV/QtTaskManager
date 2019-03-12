/********************************************************************************
** Form generated from reading UI file 'acepopup.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ACEPOPUP_H
#define UI_ACEPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_AcePopUp
{
public:
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *Username_label;
    QLineEdit *Username;
    QHBoxLayout *horizontalLayout_2;
    QVBoxLayout *AccessRightsLayout;
    QLabel *AccessRightsLabel;
    QCheckBox *_GENERIC_READ;
    QCheckBox *_WRITE_OWNER;
    QCheckBox *_SYNCHRONIZE;
    QCheckBox *_STANDARD_RIGHTS_REQUIRED;
    QCheckBox *_STANDARD_RIGHTS_READ;
    QCheckBox *_STANDARD_RIGHTS_WRITE;
    QCheckBox *_STANDARD_RIGHTS_EXECUTE;
    QCheckBox *_STANDARD_RIGHTS_ALL;
    QCheckBox *_SPECIFIC_RIGHTS_ALL;
    QCheckBox *_GENERIC_WRITE;
    QCheckBox *_GENERIC_EXECUTE;
    QCheckBox *_GENERIC_ALL;
    QCheckBox *_DELETE;
    QCheckBox *_READ_CONTROL;
    QCheckBox *_WRITE_DAC;
    QSpacerItem *verticalSpacer_2;
    QGroupBox *AceTypeGroupBox;
    QVBoxLayout *verticalLayout;
    QRadioButton *AccessAllowed;
    QRadioButton *AccessDenied;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_2;
    QDialogButtonBox *buttonBox;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *AcePopUp)
    {
        if (AcePopUp->objectName().isEmpty())
            AcePopUp->setObjectName(QStringLiteral("AcePopUp"));
        AcePopUp->resize(519, 501);
        verticalLayout_2 = new QVBoxLayout(AcePopUp);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        Username_label = new QLabel(AcePopUp);
        Username_label->setObjectName(QStringLiteral("Username_label"));
        QFont font;
        font.setPointSize(11);
        Username_label->setFont(font);

        horizontalLayout->addWidget(Username_label);

        Username = new QLineEdit(AcePopUp);
        Username->setObjectName(QStringLiteral("Username"));
        Username->setFont(font);

        horizontalLayout->addWidget(Username);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        AccessRightsLayout = new QVBoxLayout();
        AccessRightsLayout->setObjectName(QStringLiteral("AccessRightsLayout"));
        AccessRightsLabel = new QLabel(AcePopUp);
        AccessRightsLabel->setObjectName(QStringLiteral("AccessRightsLabel"));
        AccessRightsLabel->setFont(font);

        AccessRightsLayout->addWidget(AccessRightsLabel);

        _GENERIC_READ = new QCheckBox(AcePopUp);
        _GENERIC_READ->setObjectName(QStringLiteral("_GENERIC_READ"));

        AccessRightsLayout->addWidget(_GENERIC_READ);

        _WRITE_OWNER = new QCheckBox(AcePopUp);
        _WRITE_OWNER->setObjectName(QStringLiteral("_WRITE_OWNER"));
        _WRITE_OWNER->setChecked(false);
        _WRITE_OWNER->setTristate(false);

        AccessRightsLayout->addWidget(_WRITE_OWNER);

        _SYNCHRONIZE = new QCheckBox(AcePopUp);
        _SYNCHRONIZE->setObjectName(QStringLiteral("_SYNCHRONIZE"));

        AccessRightsLayout->addWidget(_SYNCHRONIZE);

        _STANDARD_RIGHTS_REQUIRED = new QCheckBox(AcePopUp);
        _STANDARD_RIGHTS_REQUIRED->setObjectName(QStringLiteral("_STANDARD_RIGHTS_REQUIRED"));

        AccessRightsLayout->addWidget(_STANDARD_RIGHTS_REQUIRED);

        _STANDARD_RIGHTS_READ = new QCheckBox(AcePopUp);
        _STANDARD_RIGHTS_READ->setObjectName(QStringLiteral("_STANDARD_RIGHTS_READ"));

        AccessRightsLayout->addWidget(_STANDARD_RIGHTS_READ);

        _STANDARD_RIGHTS_WRITE = new QCheckBox(AcePopUp);
        _STANDARD_RIGHTS_WRITE->setObjectName(QStringLiteral("_STANDARD_RIGHTS_WRITE"));

        AccessRightsLayout->addWidget(_STANDARD_RIGHTS_WRITE);

        _STANDARD_RIGHTS_EXECUTE = new QCheckBox(AcePopUp);
        _STANDARD_RIGHTS_EXECUTE->setObjectName(QStringLiteral("_STANDARD_RIGHTS_EXECUTE"));

        AccessRightsLayout->addWidget(_STANDARD_RIGHTS_EXECUTE);

        _STANDARD_RIGHTS_ALL = new QCheckBox(AcePopUp);
        _STANDARD_RIGHTS_ALL->setObjectName(QStringLiteral("_STANDARD_RIGHTS_ALL"));

        AccessRightsLayout->addWidget(_STANDARD_RIGHTS_ALL);

        _SPECIFIC_RIGHTS_ALL = new QCheckBox(AcePopUp);
        _SPECIFIC_RIGHTS_ALL->setObjectName(QStringLiteral("_SPECIFIC_RIGHTS_ALL"));

        AccessRightsLayout->addWidget(_SPECIFIC_RIGHTS_ALL);

        _GENERIC_WRITE = new QCheckBox(AcePopUp);
        _GENERIC_WRITE->setObjectName(QStringLiteral("_GENERIC_WRITE"));

        AccessRightsLayout->addWidget(_GENERIC_WRITE);

        _GENERIC_EXECUTE = new QCheckBox(AcePopUp);
        _GENERIC_EXECUTE->setObjectName(QStringLiteral("_GENERIC_EXECUTE"));

        AccessRightsLayout->addWidget(_GENERIC_EXECUTE);

        _GENERIC_ALL = new QCheckBox(AcePopUp);
        _GENERIC_ALL->setObjectName(QStringLiteral("_GENERIC_ALL"));

        AccessRightsLayout->addWidget(_GENERIC_ALL);

        _DELETE = new QCheckBox(AcePopUp);
        _DELETE->setObjectName(QStringLiteral("_DELETE"));

        AccessRightsLayout->addWidget(_DELETE);

        _READ_CONTROL = new QCheckBox(AcePopUp);
        _READ_CONTROL->setObjectName(QStringLiteral("_READ_CONTROL"));

        AccessRightsLayout->addWidget(_READ_CONTROL);

        _WRITE_DAC = new QCheckBox(AcePopUp);
        _WRITE_DAC->setObjectName(QStringLiteral("_WRITE_DAC"));

        AccessRightsLayout->addWidget(_WRITE_DAC);

        verticalSpacer_2 = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        AccessRightsLayout->addItem(verticalSpacer_2);


        horizontalLayout_2->addLayout(AccessRightsLayout);

        AceTypeGroupBox = new QGroupBox(AcePopUp);
        AceTypeGroupBox->setObjectName(QStringLiteral("AceTypeGroupBox"));
        AceTypeGroupBox->setFont(font);
        verticalLayout = new QVBoxLayout(AceTypeGroupBox);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        AccessAllowed = new QRadioButton(AceTypeGroupBox);
        AccessAllowed->setObjectName(QStringLiteral("AccessAllowed"));
        AccessAllowed->setChecked(true);

        verticalLayout->addWidget(AccessAllowed);

        AccessDenied = new QRadioButton(AceTypeGroupBox);
        AccessDenied->setObjectName(QStringLiteral("AccessDenied"));
        AccessDenied->setChecked(false);

        verticalLayout->addWidget(AccessDenied);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);


        horizontalLayout_2->addWidget(AceTypeGroupBox);


        verticalLayout_3->addLayout(horizontalLayout_2);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_2);

        buttonBox = new QDialogButtonBox(AcePopUp);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        horizontalLayout_4->addWidget(buttonBox);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_4);


        verticalLayout_2->addLayout(verticalLayout_3);


        retranslateUi(AcePopUp);

        QMetaObject::connectSlotsByName(AcePopUp);
    } // setupUi

    void retranslateUi(QWidget *AcePopUp)
    {
        AcePopUp->setWindowTitle(QApplication::translate("AcePopUp", "New ACE", nullptr));
        Username_label->setText(QApplication::translate("AcePopUp", "Username :", nullptr));
        AccessRightsLabel->setText(QApplication::translate("AcePopUp", "Access Rights :", nullptr));
        _GENERIC_READ->setText(QApplication::translate("AcePopUp", "GENERIC_READ", nullptr));
        _WRITE_OWNER->setText(QApplication::translate("AcePopUp", "GENERIC_WRITE", nullptr));
        _SYNCHRONIZE->setText(QApplication::translate("AcePopUp", "GENERIC_EXECUTE", nullptr));
        _STANDARD_RIGHTS_REQUIRED->setText(QApplication::translate("AcePopUp", "GENERIC_ALL", nullptr));
        _STANDARD_RIGHTS_READ->setText(QApplication::translate("AcePopUp", "DELETE", nullptr));
        _STANDARD_RIGHTS_WRITE->setText(QApplication::translate("AcePopUp", "READ_CONTROL", nullptr));
        _STANDARD_RIGHTS_EXECUTE->setText(QApplication::translate("AcePopUp", "WRITE_DAC", nullptr));
        _STANDARD_RIGHTS_ALL->setText(QApplication::translate("AcePopUp", "WRITE_OWNER", nullptr));
        _SPECIFIC_RIGHTS_ALL->setText(QApplication::translate("AcePopUp", "SYNCHRONIZE", nullptr));
        _GENERIC_WRITE->setText(QApplication::translate("AcePopUp", "STANDARD_RIGHTS_REQUIRED", nullptr));
        _GENERIC_EXECUTE->setText(QApplication::translate("AcePopUp", "STANDARD_RIGHTS_READ", nullptr));
        _GENERIC_ALL->setText(QApplication::translate("AcePopUp", "STANDARD_RIGHTS_WRITE", nullptr));
        _DELETE->setText(QApplication::translate("AcePopUp", "STANDARD_RIGHTS_EXECUTE", nullptr));
        _READ_CONTROL->setText(QApplication::translate("AcePopUp", "STANDARD_RIGHTS_ALL", nullptr));
        _WRITE_DAC->setText(QApplication::translate("AcePopUp", "SPECIFIC_RIGHTS_ALL", nullptr));
        AceTypeGroupBox->setTitle(QApplication::translate("AcePopUp", "ACE Type ", nullptr));
        AccessAllowed->setText(QApplication::translate("AcePopUp", "Access Allowed", nullptr));
        AccessDenied->setText(QApplication::translate("AcePopUp", "Access Denied", nullptr));
    } // retranslateUi

};

namespace Ui {
    class AcePopUp: public Ui_AcePopUp {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ACEPOPUP_H
