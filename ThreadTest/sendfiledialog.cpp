#include "sendfiledialog.h"
#include "ui_sendfiledialog.h"

#include <QFileDialog>

SendFileDialog::SendFileDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SendFileDialog)
{
    ui->setupUi(this);
    sendTimes = 0;
    ui->progressBar->setValue(0);
    this->setWindowTitle(tr("Send file"));
    connect(&tcpClient, SIGNAL(readyRead()), this, SLOT(readHandler()));
}

void SendFileDialog::setSocket(QTcpSocket *socket)
{
    //tcpClient = socket;
}

void SendFileDialog::setJsonObject(const QJsonObject& obj)
{
    m_jsonObj = obj;
}

SendFileDialog::~SendFileDialog()
{
    delete ui;
}

void SendFileDialog::send()
{
    while(true) {
        outBlock = localFile->read(qMin(byteToWrite, loadSize));
        tcpClient.write(outBlock);
        byteToWrite -= outBlock.size();  //剩余数据大小
        qDebug() << outBlock.size() << endl;
        ui->progressBar->setMaximum(totalSize);
        ui->progressBar->setValue(totalSize - byteToWrite);

        if(byteToWrite <= 0) { //发送完毕
            ui->sendStatus_label->setText(tr("文件发送完毕!"));
            break;
        }
    }
}

void SendFileDialog::sendRequestMessage()
{
    byteToWrite = localFile->size();  //剩余数据的大小
    totalSize = localFile->size();

    loadSize = 4096;  //每次发送数据的大小
    QJsonDocument jsonDoc;
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);
    qDebug() << currentFileName << endl;

    m_jsonObj.insert("Content", currentFileName);
    m_jsonObj.insert("Type", "SendFile");
    jsonDoc.setObject(m_jsonObj);
    tcpClient.write(jsonDoc.toJson());
    qDebug() << jsonDoc.toJson() << endl;
    ui->sendStatus_label->show();
    ui->progressBar->setMaximum(totalSize);
    ui->progressBar->setValue(totalSize - byteToWrite);
}

void SendFileDialog::readHandler()
{
//    QByteArray message = tcpClient.readAll();
//    qDebug() << message << endl;
    send();
}


void SendFileDialog::on_open_pushButton_clicked()
{
    ui->sendStatus_label->setText(tr("正在打开文件..."));
    ui->progressBar->setValue(0);  //非第一次发送

    loadSize = 0;
    byteToWrite = 0;
    totalSize = 0;
    outBlock.clear();

    fileName = QFileDialog::getOpenFileName(this);
    localFile = new QFile(fileName);
    localFile->open(QFile::ReadOnly);

    ui->sendStatus_label->setText(tr("已打开文件 %1").arg(fileName));
}

void SendFileDialog::on_open_pushButton_2_clicked()
{
    tcpClient.disconnectFromHost();
    tcpClient.connectToHost("127.0.0.1", 10025);
    sendRequestMessage();
    //send();
    ui->sendStatus_label->setText(tr("正在发送文件 %1").arg(fileName));
}
