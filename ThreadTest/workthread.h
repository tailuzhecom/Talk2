#ifndef WORKTHREAD_H
#define WORKTHREAD_H
#include <QThread>
#include <QtNetwork/qtcpsocket.h>

class WorkThread : public QThread
{
    Q_OBJECT
public:
    WorkThread();
    void run();
    void setSocket(QTcpSocket* socket);

signals:
    void sendMessage(QString);

private:
    QTcpSocket* m_socket;
};

#endif // WORKTHREAD_H
