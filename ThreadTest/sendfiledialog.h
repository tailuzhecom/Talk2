#ifndef SENDFILEDIALOG_H
#define SENDFILEDIALOG_H

#include <QDialog>
#include <QFile>
#include <QTcpSocket>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace Ui {
class SendFileDialog;
}

class SendFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SendFileDialog(QWidget *parent = 0);
    void setSocket(QTcpSocket* socket);
    void setJsonObject(const QJsonObject& obj);
    void send();
    ~SendFileDialog();

private slots:
    void sendRequestMessage(); //传送文件头信息
    void on_open_pushButton_clicked();
    void on_open_pushButton_2_clicked();

private:
    Ui::SendFileDialog *ui;

    QTcpSocket* tcpClient;
    QFile *localFile;
    QString fileName;  //文件名

    QByteArray outBlock;  //分次传
    qint64 loadSize;  //每次发送数据的大小
    qint64 byteToWrite;  //剩余数据大小
    qint64 totalSize;  //文件总大小

    int sendTimes;  //用来标记是否为第一次发送，第一次以后连接信号触发，后面的则手动调用
    QJsonObject m_jsonObj;
};

#endif // SENDFILEDIALOG_H
