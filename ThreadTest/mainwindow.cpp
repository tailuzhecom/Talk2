#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <string.h>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Chat");
    m_socket = new QTcpSocket;
    m_socket->connectToHost("127.0.0.1", 9999);
    m_loginDialog = new LoginDialog;
    this->hide();
    m_loginDialog->show();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(handleRead()));
    connect(m_loginDialog, SIGNAL(sendLoginMessage(QString,QString)), this, SLOT(loginDialog_slot(QString,QString)));
    m_socket->write(m_userName.toLatin1());

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleRead()
{
    char message[2048];
    m_socket->read(message, sizeof(message));
    qDebug() << message << endl;
    if(strcmp(message, "201") == 0) {  //Login success
        QMessageBox::information(this, "information", "Login success");
        m_loginDialog->hide();
        this->show();
    }
    else if(strcmp(message, "404") == 0) {
        QMessageBox::warning(this, "Error", "Username or password is not correct.");
    }
    else {
        ui->Content_textEdit->append(message);
        ui->Content_textEdit->append("\n");
    }
}

void MainWindow::on_send_pushButton_clicked()
{
    qDebug() << "Click send button" << endl;
    QJsonObject obj;
    obj.insert("From", m_userName);
    QString to = ui->to_lineEdit->text();
    QString content = ui->message_lineEdit->text();
    obj.insert("To", to);
    obj.insert("Content", content);
    QJsonArray array;
    array.append(obj);
    QJsonDocument jsonDoc(array);
    qDebug() << jsonDoc.toJson() << endl;
    m_socket->write(jsonDoc.toJson());
}

void MainWindow::loginDialog_slot(QString userName, QString passwd)
{
    QJsonObject obj;
    obj.insert("UserName", userName);
    obj.insert("Passwd", passwd);
    QJsonArray array;
    array.append(obj);
    QJsonDocument jsonDoc(array);
    m_socket->write(jsonDoc.toJson());
}
