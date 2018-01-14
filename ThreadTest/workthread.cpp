#include "workthread.h"
#include <QDebug>

WorkThread::WorkThread()
{

}

void WorkThread::run()
{
    char message[2048];
    while (true) {
        m_socket->read(message, sizeof(message));
        qDebug() << "Get from server: " << message << endl;
    }
}

void WorkThread::setSocket(QTcpSocket *socket)
{
    m_socket = socket;
}
