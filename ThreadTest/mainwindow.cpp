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

    ui->listWidget->setWindowOpacity(0.7);

    QFile img(":/image/edittest.png");
    qDebug() << img.size() << endl;
    QTextImageFormat imageFormat;
    imageFormat.setName("qrc:/image/edittest.png");


    m_toolBar = new QToolBar;
    m_toolBar->addAction(ui->actionFont);
    m_toolBar->addAction(ui->emoj);
    m_toolBar->addAction(ui->actionSend_pic);

    ui->listWidget->setIconSize(QSize(100, 50));

    //ui->verticalLayout_2->addChildLayout(ui->horizontalLayout_2);
    QVBoxLayout* right_vertical_layout = new QVBoxLayout;
    right_vertical_layout->addWidget(ui->textBrowser);
    right_vertical_layout->addWidget(m_toolBar);
    right_vertical_layout->addLayout(ui->horizontalLayout_2);

    QHBoxLayout* main_layout = new QHBoxLayout;
    main_layout->addWidget(ui->splitter);
    main_layout->addLayout(right_vertical_layout);
    m_sendFileDialog = new SendFileDialog(this);
    m_sendFileDialog->setSocket(m_socket);
    m_sendFileDialog->hide();

    ui->send_pushButton->setIcon(QIcon(":/image/talk_windows/send.png"));
    ui->send_pushButton->setIconSize(QSize(50, 20));
    ui->add_pushButton->setIcon(QIcon(":/image/talk_windows/add_friend.png"));
    ui->delete_pushButton->setIcon(QIcon(":/image/talk_windows/delete_friend.png"));
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

void MainWindow::initFriendsList()
{
    QJsonObject obj;
    obj.insert("Type", "GetFriendsList");
    obj.insert("From", m_userName);
    QJsonDocument doc;
    doc.setObject(obj);
    m_socket->write(doc.toJson());
}

MainWindow::~MainWindow()
{
    database.close();
    delete ui;
}

//Handle the message from server
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
    QString content, from;
    if(obj.contains("Content"))
        content = obj.value("Content").toString();
    if(obj.contains("From"))
        from = obj.value("From").toString();

    if(type == "201") {  //Login success
        QMessageBox::information(this, "information", "Login success");
        m_loginDialog->hide();
        this->show();
        initFriendsList();
    }
    else if(type == "404") {
        QMessageBox::warning(this, "Error", "Username or password is not correct.");
    }
    else if(type == "202") {  //GetChatRecord
        m_chatRecord[from] =  content;
    }
    else if(type == "203") {   //Receive message from other clients
//        if(m_toUser == from)
//            ui->Content_textEdit->insertPlainText(content+"\n");
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
    else if(type == "205") {   //GetFriendsList
        if(obj.contains("FriendsList")) {
            QJsonValue value = obj.value("FriendsList");
            if(value.isArray()) {
                QJsonArray array = value.toArray();
                int nSize = array.size();
                for(int i = 0; i < nSize; i++) {
                    QJsonValue nameValue = array.at(i);
                    QListWidgetItem* item = new QListWidgetItem;
                    item->setIcon(QIcon(":/image/contacts_photo/normal.png"));
                    item->setText(nameValue.toString());
                    ui->listWidget->addItem(item);
                }
            }
        }
    }
    else if(type == "206") {    //
        QMessageBox::information(this, "Message", "Add friend success!");
        QListWidgetItem* item = new QListWidgetItem;
        item->setIcon(QIcon(":/image/contacts_photo/normal.png"));
        item->setText(content);
        ui->listWidget->addItem(item);
    }
    else if(type == "207") {
        QMessageBox::information(this, "Message", "Delete friend success");
    }
}

//Send button slot
void MainWindow::on_send_pushButton_clicked()
{
    qDebug() << "Click send button" << endl;
    QJsonObject obj;
    obj.insert("Type", "SendMessage");
    obj.insert("From", m_userName);
    QString to = m_toUser;
    QString content = ui->message_lineEdit->text();
    obj.insert("To", to);
    obj.insert("Content", content);
    QJsonDocument jsonDoc;
    jsonDoc.setObject(obj);
    qDebug() << jsonDoc.toJson() << endl;
    m_socket->write(jsonDoc.toJson());
}

//Receive data from login dialog
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

//FriendList
void MainWindow::on_listWidget_itemClicked(QListWidgetItem *item)
{
    QString talkName = item->text();
//    ui->Content_textEdit->clear();
    m_toUser = talkName;
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
//            ui->Content_textEdit->setPlainText(content);
//            ui->Content_textEdit->moveCursor(QTextCursor::End);
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
//        ui->Content_textEdit->setPlainText(m_chatRecord[talkName]);
//        ui->Content_textEdit->moveCursor(QTextCursor::End);
    }
}

//SendFile button slot
void MainWindow::on_pushButton_clicked()
{
    m_sendFileDialog->show();
}

//Add button slot
void MainWindow::on_add_pushButton_clicked()
{
    QJsonObject obj;
    obj.insert("Type", "AddFriend");
    obj.insert("From", m_userName);
    QString friend_name = QInputDialog::getText(this, "Add", "Enter friend's name: ");
    obj.insert("To", friend_name);
    QJsonDocument doc;
    doc.setObject(obj);
    m_socket->write(doc.toJson());
}

//Delete button slot
void MainWindow::on_delete_pushButton_clicked()
{
    if(ui->listWidget->currentItem()) {
        QJsonObject obj;
        obj.insert("Type", "DeleteFriend");
        obj.insert("From", m_userName);
        obj.insert("To", ui->listWidget->currentItem()->data(0).toString());
        QJsonDocument doc;
        doc.setObject(obj);

        ui->listWidget->takeItem(ui->listWidget->currentRow());

        m_socket->write(doc.toJson());
    }
    else {
        QMessageBox::information(this, "error", "No selected friend!");
    }
}
