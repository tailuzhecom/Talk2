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
    connect(tcpClient, SIGNAL(connected()), this, SLOT(send()));  //当连接成功时，就开始传送文件
    connect(tcpClient, SIGNAL(bytesWritten(qint64)), this, SLOT(goOnSend(qint64)));
}

void SendFileDialog::setSocket(QTcpSocket *socket)
{
    tcpClient = socket;
}

void SendFileDialog::QJsonObject(const SendFileDialog::QJsonObject &obj)
{
    m_jsonObj = obj;
}

SendFileDialog::~SendFileDialog()
{
    delete ui;
}

void SendFileDialog::send()
{
    byteToWrite = localFile->size();  //剩余数据的大小
    totalSize = localFile->size();

    loadSize = 4*1024;  //每次发送数据的大小
    QJsonDocument jsonDoc;
    jsonDoc.setObject(m_jsonObj);
    tcpClient->write(jsonDoc.toJson());

    QDataStream out(&outBlock, QIODevice::WriteOnly);
    QString currentFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    out<< qint64(0) << qint64(0) << currentFileName;

    totalSize += outBlock.size();  //总大小为文件大小加上文件名等信息大小
    byteToWrite += outBlock.size();

    out.device()->seek(0);  //回到字节流起点来写好前面连个qint64，分别为总大小和文件名等信息大小
    out<<totalSize<<qint64(outBlock.size());

    tcpClient->write(outBlock);  //将读到的文件发送到套接字

    ui->sendStatus_label->show();
    ui->progressBar->setMaximum(totalSize);
    ui->progressBar->setValue(totalSize - byteToWrite);
}

void SendFileDialog::goOnSend(qint64 numBytes)
{
    byteToWrite -= numBytes;  //剩余数据大小
    outBlock = localFile->read(qMin(byteToWrite, loadSize));
    tcpClient->write(outBlock);

    ui->progressBar->setMaximum(totalSize);
    ui->progressBar->setValue(totalSize - byteToWrite);

    if(byteToWrite == 0)  //发送完毕
        ui->sendStatus_label->setText(tr("文件发送完毕!"));
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
    send();
    ui->sendStatus_label->setText(tr("Sending file %1").arg(fileName));
}
