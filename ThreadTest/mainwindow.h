#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <workthread.h>
#include <QtNetwork/QTcpSocket>

#include "logindialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void handleRead();
    void on_send_pushButton_clicked();
    void loginDialog_slot(QString userName, QString passwd);

private:
    Ui::MainWindow *ui;
    WorkThread* t1;
    QTcpSocket* m_socket;
    QString m_userName;
    LoginDialog* m_loginDialog;
};

#endif // MAINWINDOW_H
