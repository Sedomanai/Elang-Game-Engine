/********************************************************************************
** Form generated from reading UI file 'qt_texture_widget.ui'
**
** Created by: Qt User Interface Compiler version 6.2.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QT_TEXTURE_WIDGET_H
#define UI_QT_TEXTURE_WIDGET_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QScrollBar>
#include <QtWidgets/QWidget>
#include "qt_view_extension.h"

QT_BEGIN_NAMESPACE

class Ui_QElangTextureWidgetUI
{
public:
    QGridLayout *gridLayout;
    el::QElangView *view;
    QScrollBar *verti;
    QScrollBar *hori;

    void setupUi(QWidget *QElangTextureWidgetUI)
    {
        if (QElangTextureWidgetUI->objectName().isEmpty())
            QElangTextureWidgetUI->setObjectName(QString::fromUtf8("QElangTextureWidgetUI"));
        QElangTextureWidgetUI->resize(477, 444);
        gridLayout = new QGridLayout(QElangTextureWidgetUI);
        gridLayout->setSpacing(2);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        view = new el::QElangView(QElangTextureWidgetUI);
        view->setObjectName(QString::fromUtf8("view"));

        gridLayout->addWidget(view, 0, 0, 1, 1);

        verti = new QScrollBar(QElangTextureWidgetUI);
        verti->setObjectName(QString::fromUtf8("verti"));
        verti->setOrientation(Qt::Vertical);

        gridLayout->addWidget(verti, 0, 1, 2, 1);

        hori = new QScrollBar(QElangTextureWidgetUI);
        hori->setObjectName(QString::fromUtf8("hori"));
        hori->setOrientation(Qt::Horizontal);

        gridLayout->addWidget(hori, 1, 0, 1, 1);


        retranslateUi(QElangTextureWidgetUI);

        QMetaObject::connectSlotsByName(QElangTextureWidgetUI);
    } // setupUi

    void retranslateUi(QWidget *QElangTextureWidgetUI)
    {
        QElangTextureWidgetUI->setWindowTitle(QCoreApplication::translate("QElangTextureWidgetUI", "Texture Widget", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QElangTextureWidgetUI: public Ui_QElangTextureWidgetUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QT_TEXTURE_WIDGET_H
