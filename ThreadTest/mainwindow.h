#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <workthread.h>
#include <QtNetwork/QTcpSocket>
#include <QListWidgetItem>
#include <QtSql/QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

#include "logindialog.h"
#include "sendfiledialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    void initDatabase();
    void initFriendsList();
    ~MainWindow();

private slots:
    void handleRead();
    void on_send_pushButton_clicked();
    void loginDialog_slot(QString userName, QString passwd);
    void on_listWidget_itemClicked(QListWidgetItem *item);
    void on_pushButton_clicked();

    void on_add_pushButton_clicked();

    void on_delete_pushButton_clicked();

private:
    Ui::MainWindow *ui;
    WorkThread* t1;
    QTcpSocket* m_socket;
    QString m_userName;
    LoginDialog* m_loginDialog;
    QMap<QString, QString> m_chatRecord;
    QSqlDatabase database;
    SendFileDialog* m_sendFileDialog;
};

#endif // MAINWINDOW_H
