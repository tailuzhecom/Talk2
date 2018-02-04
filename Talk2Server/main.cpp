#include <iostream>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <thread>
#include <strings.h>
#include <cstring>
#include <mysql/mysql.h>
#include <map>
#include "json.hpp"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "DB.h"
#include "LoginSvr.h"
#include <muduo/net/EventLoop.h>

#define MAXEVENTS 64
#define BUFFERSIZE 4096

using namespace std;
using namespace rapidjson;

//
//static int make_socket_non_blocking(int sfd) {
//    int flags, s;
//
//    flags = fcntl(sfd, F_GETFL, 0);
//    if(flags == -1) {
//        perror("fcntl");
//        return -1;
//    }
//    flags |= O_NONBLOCK;
//    s = fcntl(sfd, F_SETFL, flags);
//    if(s == -1) {
//        perror("fcntl");
//        return -1;
//    }
//    return 0;
//}
//
//static int create_and_bind(char* port) {
//    addrinfo hints;
//    addrinfo *result, *rp;
//    int s, sfd;
//
//    memset(&hints, 0, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_PASSIVE;
//
//    s = getaddrinfo(NULL, port, &hints, &result);
//    if(s != 0) {
//        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
//        return -1;
//    }
//
//    for(rp = result; rp != NULL; rp = rp->ai_next) {
//        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
//        if(sfd == -1)
//            continue;
//
//        s = bind(sfd, rp->ai_addr, rp->ai_addrlen);
//        if(s == 0) {
//            break;
//        }
//        close(sfd);
//    }
//    if(rp == NULL) {
//        fprintf(stderr, "Could not bind\n");
//        return -1;
//    }
//    freeaddrinfo(result);
//    return sfd;
//}
//
//DB g_db;
//map<string, int> g_clientConn;
//
//int main(int argc, char* argv[]) {
//    int sfd, s;
//    int efd;
//    epoll_event event;
//    epoll_event events[MAXEVENTS];
//
//    if(argc != 2) {
//        fprintf(stderr, "Usage: %s [port]\n", argv[0]);
//        exit(EXIT_FAILURE);
//    }
//
//    sfd = create_and_bind(argv[1]);
//    if(sfd == -1)
//        abort();
//
//    s = make_socket_non_blocking(sfd);
//    if(s == -1)
//        abort();
//
//    s = listen(sfd, SOMAXCONN);
//    if(s == -1) {
//        perror("listen");
//        abort();
//    }
//
//    efd = epoll_create1(0);
//    if(efd == -1) {
//        perror("epoll_create");
//        abort();
//    }
//
//    event.data.fd = sfd;
//    event.events = EPOLLIN | EPOLLET;
//    s = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &event);
//    if(s == -1) {
//        perror("epoll_ctl");
//        abort();
//    }
//
//    while(1) {
//        int n, i;
//
//        n = epoll_wait(efd, events, MAXEVENTS, -1);
//        for(i = 0; i < n; i++) {
//            if((events[i].events & EPOLLERR) ||
//                    (events[i].events & EPOLLHUP) ||
//                    (!(events[i].events & EPOLLIN))) {
//                fprintf(stderr, "epoll error\n");
//                continue;
//            }
//            else if(sfd == events[i].data.fd) {
//                while(1) {
//                    //产生新连接
//                    sockaddr in_addr;
//                    socklen_t in_len;
//                    int infd;
//                    char hbuf[NI_MAXHOST], sbuf[NI_MAXSERV];
//
//                    in_len = sizeof(in_addr);
//                    infd = accept(sfd, &in_addr, &in_len);
//                    if(infd == -1) {
//                        if((errno == EAGAIN) ||
//                                (errno == EWOULDBLOCK)) {
//                            break;
//                        }
//                        else {
//                            perror("accept");
//                            break;
//                        }
//                    }
//
//                    s = make_socket_non_blocking(infd);
//                    if(s == -1)
//                        abort();
//
//                    event.data.fd = infd;
//                    event.events = EPOLLIN | EPOLLET;
//                    s = epoll_ctl(efd, EPOLL_CTL_ADD, infd, &event);
//                    if(s == -1) {
//                        perror("epoll_ctl");
//                        abort();
//                    }
//                }
//                continue;
//            }
//            else {
//                //可读
//                int done = 0;
//
//                while(1) {
//                    ssize_t count;
//                    char buf[2048];
//
//                    count = read(events[i].data.fd, buf, sizeof(buf));
//                    if(count == -1) {
//                        if(errno != EAGAIN) {
//                            perror("read");
//                            done = 1;
//                        }
//                        break;
//                    }
//                    else if(count == 0) {
//                        done = -1;
//                        break;
//                    }
//
//                    //标准输出
//                    cout << "RequestMessage: " << endl;
//                    write(1, buf, count);
//                    cout << endl;
//                    rapidjson::Document doc;
//                    doc.Parse(buf, count);
//                    string messageType = doc["Type"].GetString();
//                    if(messageType == "Login") {
//                        string userName = doc["UserName"].GetString();
//                        string passwd = doc["Passwd"].GetString();
//                        //登录验证
//                        string sql = "select * from login";
//                        mysql_query(g_db.conn, sql.c_str());
//                        MYSQL_RES *res = mysql_store_result(g_db.conn);
//                        int row_count = mysql_num_rows(res);
//                        int field_count = mysql_num_fields(res);
//                        MYSQL_ROW row = NULL;
//                        row = mysql_fetch_row(res);
//                        while (row != NULL) {
//                            //登录成功
//                            if (strcmp(row[0], userName.c_str()) == 0 && strcmp(row[1], passwd.c_str()) == 0) {
//                                cout << "Login success" << endl;
//                                //增加用户名对应的连接
//                                StringBuffer stringBuffer;
//                                rapidjson::Writer<StringBuffer> writer(stringBuffer);
//                                writer.StartObject();
//                                writer.Key("Type");
//                                writer.String("201");
//                                writer.Key("From");
//                                writer.String(userName.c_str());
//                                writer.Key("Content");
//                                writer.String("");
//                                writer.EndObject();
//                                g_clientConn[userName] = events[i].data.fd;
//                                write(events[i].data.fd, stringBuffer.GetString(), stringBuffer.GetSize());
//                                break;
//                            }
//                            cout << endl;
//                            row = mysql_fetch_row(res);
//                        }
//                        mysql_free_result(res);
//
//                        if (s == -1) {
//                            perror("write");
//                            abort();
//                        }
//                    }
//                    else if(messageType == "SendMessage") {
//                        string fromUser = doc["From"].GetString();
//                        string toUser = doc["To"].GetString();
//                        string content = fromUser + ": " + doc["Content"].GetString();
//                        cout << "From User: " << fromUser << endl;
//                        cout << "To User: " << toUser << endl;
//                        cout << "Content" << content << endl;
//                        if(g_clientConn.count(toUser)) {  //如果要接受信息的用户在线
//                            StringBuffer s;
//                            Writer<StringBuffer> writer(s);
//                            writer.StartObject();
//                            writer.Key("Type");
//                            writer.String("203");
//                            writer.Key("Content");
//                            writer.String(content.c_str());
//                            writer.Key("From");
//                            writer.String(fromUser.c_str());
//                            writer.EndObject();
//                            write(g_clientConn[toUser], s.GetString(), s.GetSize());
//                        }
//                        else {  //如果要接受信息的用户不在线
//                            StringBuffer s;
//                            Writer<StringBuffer> writer(s);
//                            writer.StartObject();
//                            writer.Key("Type");
//                            writer.String("203");
//                            writer.Key("Content");
//                            writer.String("该用户不在线");
//                            writer.Key("From");
//                            writer.String(toUser.c_str());
//                            writer.EndObject();
//                            write(g_clientConn[fromUser], s.GetString(), s.GetSize());
//
//                            //在数据库中存储离线消息
//                            string sql = "insert into offline_message(from_, to_, content)"
//                                                 " values('" + fromUser + "', '" + toUser +
//                                                 "', '" + content + "')";
//                            int res = mysql_query(g_db.conn, sql.c_str());
//                            if(res) {
//                                cout << "insert error" << endl;
//                            }
//                        }
//                    }
//                    else if(messageType == "GetOfflineMessage") {   //拉取离线信息
//                        string fromUser = doc["From"].GetString();
//                        string toUser = doc["To"].GetString();
//                        string content = fromUser + ": " + doc["Content"].GetString();
//                        cout << fromUser << ": GetOfflineMessage" << endl;
//                        string sql = "select content from offline_message where from_ = '" +
//                                toUser + "' and to_ = '" + fromUser + "'";
//                        mysql_query(g_db.conn, sql.c_str());
//                        MYSQL_RES* res = mysql_store_result(g_db.conn);
//                        MYSQL_ROW row = NULL;
//                        int row_count = mysql_num_rows(res);
//                        int field_count = mysql_num_fields(res);
//                        row = mysql_fetch_row(res);
//                        while(row != NULL) {
//                            StringBuffer s;
//                            Writer<StringBuffer> writer(s);
//                            writer.StartObject();
//                            writer.Key("From");
//                            writer.String(toUser.c_str());
//                            writer.Key("Type");
//                            writer.String("203");
//                            writer.Key("Content");
//                            writer.String(row[0]);
//                            writer.EndObject();
//                            write(g_clientConn[fromUser], s.GetString(), s.GetSize());
//                            row = mysql_fetch_row(res);
//                        }
//
//                        //删除已拉取的消息
//                        sql = "delete from offline_message where from_ = '" + toUser +"' and to_ = '" + fromUser + "'";
//                        if(mysql_query(g_db.conn, sql.c_str())) {
//                            cout << "delete error" << endl;
//                        }
//                    }
//                    else if(messageType == "SendFile") {
//                        StringBuffer s;
//                        Writer<StringBuffer> writer(s);
//                        writer.StartObject();
//                        writer.Key("Type");
//                        writer.String("204");   //可以开始接收文件，需要设置此回应否则会发生RequestMessage和文件信息粘包的问题
//                        writer.Key("Content");
//                        writer.String("");
//                        writer.Key("From");
//                        writer.String("");
//                        writer.EndObject();
//                        write(events[i].data.fd, s.GetString(), s.GetSize());
//                        char buffer[BUFFERSIZE];
//                        bzero(buffer, BUFFERSIZE);
//                        string fileName = doc["Content"].GetString();
//                        string fromUser = doc["From"].GetString();
//                        string toUser = doc["To"].GetString();
//                        FILE* fp = fopen(fileName.c_str(), "wb");
//                        if(fp == NULL) {
//                            cout << "Open file failed." << endl;
//                        }
//                        memset(buffer, 0, BUFFERSIZE);
//                        int length = 0;
//                        while((length = recv(events[i].data.fd, buffer, BUFFERSIZE, 0)) > 0) {
//                            if(fwrite(buffer, sizeof(char), length, fp) < length) {
//                                printf("File: %s write failed\n", fileName.c_str());
//                                break;
//                            }
//                            memset(buffer, 0, BUFFERSIZE);
//                        }
//                        fclose(fp);
//                        printf("File transfer success!\n");
//                    }
//                }
//
//                if(done) {
//                    printf("Closed connection on descriptor %d\n",
//                                events[i].data.fd);
//                    //用户下线
//                    for(auto iter = g_clientConn.begin(); iter != g_clientConn.end(); ++iter) {
//                        if(iter->second == events[i].data.fd) {
//                            g_clientConn.erase(iter);
//                            break;
//                        }
//                    }
//                    close(events[i].data.fd);
//                }
//            }
//        }
//    }
//    close(sfd);
//    return EXIT_SUCCESS;
//}

int main() {
    muduo::net::EventLoop loop;
    muduo::net::InetAddress listenAddr(10021);
    LoginServer login_server(&loop, listenAddr);
    login_server.start();
    loop.loop();

    return 0;
}