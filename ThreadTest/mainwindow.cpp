#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <string.h>
#include <QMessageBox>
#include <QListView>
#include <QJsonParseError>
#include <QTextDocumentFragment>
#include <QImage>
#include <QImageReader>
#include <QTextDocument>
#include <QFile>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDatabase();
    this->setWindowTitle("Chat");
    m_socket = new QTcpSocket;
    m_socket->connectToHost("127.0.0.1", 10021);
    m_loginDialog = new LoginDialog;
    this->hide();
    m_loginDialog->show();
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(handleRead()));
    connect(m_loginDialog, SIGNAL(sendLoginMessage(QString,QString)), this, SLOT(loginDialog_slot(QString,QString)));
    m_socket->write(m_userName.toLatin1());
    QListWidgetItem* item1 = new QListWidgetItem;
    item1->setText("kd");
    QListWidgetItem* item2 = new QListWidgetItem;
    item2->setText("curry");
    ui->listWidget->addItem(item1);
    ui->listWidget->addItem(item2);

    QFile img(":/image/edittest.png");
    qDebug() << img.size() << endl;
    ui->Content_textEdit->append("<img src=qrc:/image/edittest.png>");
    ui->Content_textEdit->append("Hello");
    QTextImageFormat imageFormat;
    imageFormat.setName("qrc:/image/edittest.png");
    QTextCursor cursor = ui->Content_textEdit->textCursor();
    cursor.insertImage(imageFormat);

    m_sendFileDialog = new SendFileDialog(this);
    m_sendFileDialog->setSocket(m_socket);
    m_sendFileDialog->hide();
}

void MainWindow::initDatabase()
{
    if(QSqlDatabase::contains("qt_sql_default_connection")) {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else {
        database = QSqlDatabase::addDatabase("QSQLITE");
        database.setDatabaseName("MyDB.db");
        database.setUserName("tailuzhecom");
        database.setPassword("123456");
    }
    if(!database.open()) {
        qDebug() << "Error: Failed to connect database." <<
                    database.lastError();
    }
    else {
        qDebug() << "Success" << endl;
    }
    QSqlQuery sql_query;
    QString create_sql = "create table chatrecord(id int primary, from_ varchar(20), to_ varchar(20), content varchar(255));";
    sql_query.prepare(create_sql);
    if(!sql_query.exec()) {
        qDebug() << "Error: Fail to create table." << sql_query.lastError();
    }
    else {
        qDebug() << "Table created!";
    }

}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}

void MainWindow::handleRead()
{
    QByteArray message = m_socket->readAll();
    QJsonParseError jsonError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(message, &jsonError);
    qDebug() << jsonError.errorString() << endl;
    qDebug() << jsonDoc << endl;
    if(jsonDoc.isArray()) {
        qDebug() << "Is array" << endl;
    }
    if(jsonDoc.isObject()) {
        qDebug() << "Is object" << endl;
    }
    if(jsonDoc.isNull()) {
        qDebug() << "Is NULL" << endl;
    }

    QJsonObject obj = jsonDoc.object();
    QString type = obj.value("Type").toString();
    QString content = obj.value("Content").toString();
    QString from = obj.value("From").toString();

    if(type == "201") {  //Login success
        QMessageBox::information(this, "information", "Login success");
        m_loginDialog->hide();
        this->show();
    }
    else if(type == "404") {
        QMessageBox::warning(this, "Error", "Username or password is not correct.");
    }
    else if(type == "202") {  //GetChatRecord
        m_chatRecord[from] =  content;
    }
    else if(type == "203") {   //Receive message from other clients
        if(ui->to_lineEdit->text() == from)
            ui->Content_textEdit->insertPlainText(content+"\n");
        m_chatRecord[from].append(content+"\n");
        //ui->Content_textEdit->setText(content);
        QSqlQuery sql_query;
        QString insert_sql = "insert into chatrecord(from_, to_, content) values(?, ?, ?)";
        sql_query.prepare(insert_sql);
        sql_query.addBindValue(from);
        qDebug() << "userName" << m_userName << endl;
        sql_query.addBindValue(m_userName);
        sql_query.addBindValue(content);
        if(!sql_query.exec()) {
            qDebug() << sql_query.lastError();
        }
        else {
            qDebug() << "Insert success" << endl;
        }
    }
    else if(type == "204") {   //Send file
        qDebug() << "Start transfering file" << endl;
        m_sendFileDialog->send();
    }
}

void MainWindow::on_send_pushButton_clicked()
{
    qDebug() << "Click send button" << endl;
    QJsonObject obj;
    obj.insert("Type", "SendMessage");
    obj.insert("From", m_userName);
    QString to = ui->to_lineEdit->text();
    QString content = ui->message_lineEdit->text();
    obj.insert("To", to);
    obj.insert("Content", content);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(obj);
    qDebug() << jsonDoc.toJson() << endl;
    m_socket->write(jsonDoc.toJson());
}

void MainWindow::loginDialog_slot(QString userName, QString passwd)
{
    m_userName = userName;
    QJsonObject obj;
    obj.insert("Type", "Login");  //Type 0 : login
    obj.insert("UserName", userName);
    obj.insert("Passwd", passwd);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(obj);
    m_socket->write(jsonDoc.toJson());
}

void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString talkName = item->text();
    ui->Content_textEdit->clear();
    ui->to_lineEdit->setText(talkName);
    //Update SendFile json
    QJsonObject sendObj;
    sendObj.insert("Type", "SendFile");
    sendObj.insert("From", m_userName);
    sendObj.insert("To", talkName);
    m_sendFileDialog->setJsonObject(sendObj);

    if(m_chatRecord.count(talkName) == 0) {
        QString content = "";
        QSqlQuery sql_query;
        QString query_sql = "select content from chatrecord where from_ = ? and to_ = ?";
        sql_query.prepare(query_sql);
        sql_query.addBindValue(talkName);
        sql_query.addBindValue(m_userName);
        if(sql_query.exec()) {
            qDebug() << "Select success" << endl;
            while(sql_query.next()) {
                content += sql_query.value(0).toString() + "\n";
            }
            m_chatRecord[talkName] = content;
            ui->Content_textEdit->setPlainText(content);
            ui->Content_textEdit->moveCursor(QTextCursor::End);
        }
        else {
            qDebug() << sql_query.lastError() << endl;
        }
        qDebug() << "content: " << content << endl;
        //Get offline message
        QJsonObject obj;
        obj.insert("Type", "GetOfflineMessage");
        obj.insert("From", m_userName);
        obj.insert("To", item->text());
        obj.insert("Content", "");
        QJsonDocument jsonDoc;
        jsonDoc.setObject(obj);
        m_socket->write(jsonDoc.toJson());
    }
    else {    //Have inited the chat record
        ui->Content_textEdit->setPlainText(m_chatRecord[talkName]);
        ui->Content_textEdit->moveCursor(QTextCursor::End);
    }
}

void MainWindow::on_pushButton_clicked()
{
    m_sendFileDialog->show();
}
