/********************************************************************************
** Form generated from reading UI file 'sendfiledialog.ui'
**
** Created by: Qt User Interface Compiler version 5.7.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SENDFILEDIALOG_H
#define UI_SENDFILEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_SendFileDialog
{
public:
    QProgressBar *progressBar;
    QLabel *sendStatus_label;
    QLabel *label;
    QPushButton *open_pushButton;
    QPushButton *open_pushButton_2;

    void setupUi(QDialog *SendFileDialog)
    {
        if (SendFileDialog->objectName().isEmpty())
            SendFileDialog->setObjectName(QStringLiteral("SendFileDialog"));
        SendFileDialog->resize(400, 300);
        progressBar = new QProgressBar(SendFileDialog);
        progressBar->setObjectName(QStringLiteral("progressBar"));
        progressBar->setGeometry(QRect(120, 150, 241, 23));
        progressBar->setValue(24);
        sendStatus_label = new QLabel(SendFileDialog);
        sendStatus_label->setObjectName(QStringLiteral("sendStatus_label"));
        sendStatus_label->setGeometry(QRect(40, 230, 351, 18));
        label = new QLabel(SendFileDialog);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(30, 150, 71, 18));
        open_pushButton = new QPushButton(SendFileDialog);
        open_pushButton->setObjectName(QStringLiteral("open_pushButton"));
        open_pushButton->setGeometry(QRect(40, 70, 80, 26));
        open_pushButton_2 = new QPushButton(SendFileDialog);
        open_pushButton_2->setObjectName(QStringLiteral("open_pushButton_2"));
        open_pushButton_2->setGeometry(QRect(270, 70, 80, 26));

        retranslateUi(SendFileDialog);

        QMetaObject::connectSlotsByName(SendFileDialog);
    } // setupUi

    void retranslateUi(QDialog *SendFileDialog)
    {
        SendFileDialog->setWindowTitle(QApplication::translate("SendFileDialog", "Dialog", 0));
        sendStatus_label->setText(QString());
        label->setText(QApplication::translate("SendFileDialog", "PROCESS", 0));
        open_pushButton->setText(QApplication::translate("SendFileDialog", "OPEN", 0));
        open_pushButton_2->setText(QApplication::translate("SendFileDialog", "SEND", 0));
    } // retranslateUi

};

namespace Ui {
    class SendFileDialog: public Ui_SendFileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SENDFILEDIALOG_H
