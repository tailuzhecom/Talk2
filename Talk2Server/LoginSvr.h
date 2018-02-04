//
// Created by tailuzhecom on 18-1-31.
//

#ifndef TALK2SVR_LOGINSVR_H
#define TALK2SVR_LOGINSVR_H

#include <muduo/net/EventLoop.h>
#include <muduo/net/InetAddress.h>
#include <muduo/net/TcpServer.h>
#include <string>
#include <vector>
#include <map>
#include "DB.h"

using namespace std;

class LoginServer
{
public:
    LoginServer(muduo::net::EventLoop* loop,
               const muduo::net::InetAddress& listenAddr);

    void start();  // calls server_.start();

private:
    void onConnection(const muduo::net::TcpConnectionPtr& conn);
    void splitEx(const string& src, string separate_character, vector<string>& vctStr);
    void onMessage(const muduo::net::TcpConnectionPtr& conn,
                   muduo::net::Buffer* buf,
                   muduo::Timestamp time);

    muduo::net::TcpServer server_;
    std::map<std::string, muduo::net::TcpConnectionPtr> m_conn_map;
    DB g_db;
};

#endif //TALK2SVR_LOGINSVR_H
