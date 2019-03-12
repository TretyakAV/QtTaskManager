/********************************************************************************
** Form generated from reading UI file 'integritypopup.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_INTEGRITYPOPUP_H
#define UI_INTEGRITYPOPUP_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_IntegrityPopup
{
public:
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_3;
    QLabel *label;
    QToolButton *toolButton;
    QSpacerItem *horizontalSpacer_4;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_3;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *okButton;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *IntegrityPopup)
    {
        if (IntegrityPopup->objectName().isEmpty())
            IntegrityPopup->setObjectName(QStringLiteral("IntegrityPopup"));
        IntegrityPopup->resize(337, 118);
        verticalLayout_3 = new QVBoxLayout(IntegrityPopup);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer_3 = new QSpacerItem(40, 40, QSizePolicy::Fixed, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_3);

        label = new QLabel(IntegrityPopup);
        label->setObjectName(QStringLiteral("label"));
        QFont font;
        font.setPointSize(10);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        toolButton = new QToolButton(IntegrityPopup);
        toolButton->setObjectName(QStringLiteral("toolButton"));
        toolButton->setMinimumSize(QSize(150, 30));
        toolButton->setAutoRaise(false);
        toolButton->setArrowType(Qt::NoArrow);

        horizontalLayout->addWidget(toolButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_4);


        verticalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer_2);

        okButton = new QPushButton(IntegrityPopup);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout_3->addWidget(okButton);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_3->addItem(horizontalSpacer);


        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);


        verticalLayout_3->addLayout(verticalLayout_2);


        retranslateUi(IntegrityPopup);

        QMetaObject::connectSlotsByName(IntegrityPopup);
    } // setupUi

    void retranslateUi(QWidget *IntegrityPopup)
    {
        IntegrityPopup->setWindowTitle(QApplication::translate("IntegrityPopup", "Integrity Level", nullptr));
        label->setText(QApplication::translate("IntegrityPopup", "Integrity Level :", nullptr));
        toolButton->setText(QApplication::translate("IntegrityPopup", "...", nullptr));
        okButton->setText(QApplication::translate("IntegrityPopup", "Ok", nullptr));
    } // retranslateUi

};

namespace Ui {
    class IntegrityPopup: public Ui_IntegrityPopup {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_INTEGRITYPOPUP_H
