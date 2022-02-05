/********************************************************************************
** Form generated from reading UI file 'qt_find_texture_dialog.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_FIND_TEXTURE_DIALOG_H
#define UI_QT_FIND_TEXTURE_DIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>

QT_BEGIN_NAMESPACE

class Ui_QElangFindTextureDialogUI
{
public:
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *comboBox;

    void setupUi(QDialog *QElangFindTextureDialogUI)
    {
        if (QElangFindTextureDialogUI->objectName().isEmpty())
            QElangFindTextureDialogUI->setObjectName(QString::fromUtf8("QElangFindTextureDialogUI"));
        QElangFindTextureDialogUI->resize(228, 41);
        verticalLayout = new QVBoxLayout(QElangFindTextureDialogUI);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        label = new QLabel(QElangFindTextureDialogUI);
        label->setObjectName(QString::fromUtf8("label"));

        horizontalLayout->addWidget(label);

        comboBox = new QComboBox(QElangFindTextureDialogUI);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));

        horizontalLayout->addWidget(comboBox);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(QElangFindTextureDialogUI);

        QMetaObject::connectSlotsByName(QElangFindTextureDialogUI);
    } // setupUi

    void retranslateUi(QDialog *QElangFindTextureDialogUI)
    {
        QElangFindTextureDialogUI->setWindowTitle(QCoreApplication::translate("QElangFindTextureDialogUI", "Find Texture", nullptr));
        label->setText(QCoreApplication::translate("QElangFindTextureDialogUI", "Texture Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QElangFindTextureDialogUI: public Ui_QElangFindTextureDialogUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_FIND_TEXTURE_DIALOG_H
