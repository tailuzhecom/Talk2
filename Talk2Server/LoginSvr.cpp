//
// Created by tailuzhecom on 18-1-31.
//
#include <iostream>
#include "LoginSvr.h"
#include <muduo/base/Logging.h>
#include "rapidjson/stringbuffer.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using namespace std;
using namespace rapidjson;
using namespace std::placeholders;

LoginServer::LoginServer(muduo::net::EventLoop* loop,
                       const muduo::net::InetAddress& listenAddr)
        : server_(loop, listenAddr, "LoginServer")
{
    server_.setConnectionCallback(
            std::bind(&LoginServer::onConnection, this, _1));
    server_.setMessageCallback(
            std::bind(&LoginServer::onMessage, this, _1, _2, _3));
}

void LoginServer::start()
{
    server_.start();
}

void LoginServer::onConnection(const muduo::net::TcpConnectionPtr& conn)
{
    LOG_INFO << "EchoServer - " << conn->peerAddress().toIpPort() << " -> "
              << conn->localAddress().toIpPort() << " is "
              << (conn->connected() ? "UP" : "DOWN");
}

void LoginServer::onMessage(const muduo::net::TcpConnectionPtr& conn,
                           muduo::net::Buffer* buf,
                           muduo::Timestamp time)
{
    muduo::string msg(buf->retrieveAllAsString());
    LOG_INFO << conn->name() << " echo " << msg.size() << " bytes, "
              << "data received at " << time.toString();
    std::cout << msg.data() << std::endl;
    ssize_t count;

    //标准输出
    cout << "RequestMessage: " << endl;
    cout << endl;
    rapidjson::Document doc;
    doc.Parse(msg.c_str(), msg.size());
    string messageType = "";
    if(doc.HasMember("Type"))
        messageType = doc["Type"].GetString();
    if(messageType == "Login") {
        string userName = doc["UserName"].GetString();
        string passwd = doc["Passwd"].GetString();
        //登录验证
        string sql = "select * from login";
        mysql_query(g_db.conn, sql.c_str());
        MYSQL_RES *res = mysql_store_result(g_db.conn);
        int row_count = mysql_num_rows(res);
        int field_count = mysql_num_fields(res);
        MYSQL_ROW row = NULL;
        row = mysql_fetch_row(res);
        while (row != NULL) {
            //登录成功
            if (strcmp(row[0], userName.c_str()) == 0 && strcmp(row[1], passwd.c_str()) == 0) {
                cout << "Login success" << endl;
                //增加用户名对应的连接
                m_conn_map[userName] = conn;
                StringBuffer stringBuffer;
                rapidjson::Writer<StringBuffer> writer(stringBuffer);
                writer.StartObject();
                writer.Key("Type");
                writer.String("201");
                writer.Key("From");
                writer.String(userName.c_str());
                writer.Key("Content");
                writer.String("");
                writer.EndObject();
                conn->send(stringBuffer.GetString(), stringBuffer.GetSize());
                //write(events[i].data.fd, stringBuffer.GetString(), stringBuffer.GetSize());
                break;
            }
            cout << endl;
            row = mysql_fetch_row(res);
        }
        mysql_free_result(res);
    }
    else if(messageType == "GetFriendsList") {
        string from_user = doc["From"].GetString();
        string sql = "select friends from friendslist where user = '" + from_user + "'";
        mysql_query(g_db.conn, sql.c_str());
        MYSQL_RES* res = mysql_store_result(g_db.conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        string friends_name = row[0];
        mysql_free_result(res);

        std::vector<std::string> name_vec;
        splitEx(friends_name, "&", name_vec);  //split name by '&'

        StringBuffer s;
        rapidjson::Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("Type");
        writer.String("205");
        writer.Key("FriendsList");
        writer.StartArray();
        for(const auto& name : name_vec)
            writer.String(name.c_str());
        writer.EndArray();
        writer.EndObject();

        conn->send(s.GetString(), s.GetSize());
    }
    else if(messageType == "AddFriend") {
        string from_user = doc["From"].GetString();
        string to_user=  doc["To"].GetString();

        string sql = "select friends from friendslist where user = '" + from_user + "'";
        mysql_query(g_db.conn, sql.c_str());
        MYSQL_RES* res = mysql_store_result(g_db.conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        string friends_name = row[0];
        mysql_free_result(res);

        std::vector<std::string> name_vec;
        splitEx(friends_name, "&", name_vec);

        bool friend_exist = false;
        for(const auto& name : name_vec) {
            if(name == to_user) {
                friend_exist = true;
                break;
            }
        }
        if(!friend_exist) {
            if(friends_name == "")
                friends_name += to_user;
            else
                friends_name += "&" + to_user;
            std::string update_sql = "update friendslist set friends = '" + friends_name
                                     + "' where user = '" + from_user + "'";
            int res = mysql_query(g_db.conn, update_sql.c_str());
            if(res) {
                std::cout << "update failed" << std::endl;
            }
        }


        StringBuffer s;
        rapidjson::Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("Type");
        writer.String("206");
        writer.Key("Content"); //new friend's name
        writer.String(to_user.c_str());
        writer.EndObject();

        conn->send(s.GetString(), s.GetSize());
    }
    else if(messageType == "DeleteFriend") {
        string from_user = doc["From"].GetString();
        string to_user=  doc["To"].GetString();

        string sql = "select friends from friendslist where user = '" + from_user + "'";
        mysql_query(g_db.conn, sql.c_str());
        MYSQL_RES* res = mysql_store_result(g_db.conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        string friends_name = row[0];
        mysql_free_result(res);

        std::vector<std::string> name_vec;
        splitEx(friends_name, "&", name_vec);

        bool friend_exist = false;
        for(auto iter = name_vec.begin(); iter != name_vec.end(); ++iter) {
            if(*iter == to_user) {
                name_vec.erase(iter);
                break;
            }
        }

        friends_name = "";
        for(const auto& name : name_vec) {
            if(friends_name == "")
                friends_name += name;
            else
                friends_name += "&" + name;
        }


        std::string update_sql = "update friendslist set friends = '" + friends_name
                                 + "' where user = '" + from_user + "'";

        if(mysql_query(g_db.conn, update_sql.c_str())) {
            std::cout << "update failed" << std::endl;
        }

        StringBuffer s;
        rapidjson::Writer<StringBuffer> writer(s);
        writer.StartObject();
        writer.Key("Type");
        writer.String("207");
        writer.EndObject();

        conn->send(s.GetString(), s.GetSize());
    }
    else if(messageType == "SendMessage") {
        string fromUser = doc["From"].GetString();
        string toUser = doc["To"].GetString();
        string content = fromUser + ": " + doc["Content"].GetString();

        if(m_conn_map.count(toUser)) {  //如果要接受信息的用户在线
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("Type");
            writer.String("203");
            writer.Key("Content");
            writer.String(content.c_str());
            writer.Key("From");
            writer.String(fromUser.c_str());
            writer.EndObject();
            m_conn_map[toUser]->send(s.GetString(), s.GetSize());
        }
        else {  //如果要接受信息的用户不在线
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("Type");
            writer.String("203");
            writer.Key("Content");
            writer.String("该用户不在线");
            writer.Key("From");
            writer.String(toUser.c_str());
            writer.EndObject();
            conn->send(s.GetString(), s.GetSize());

            //在数据库中存储离线消息
            string sql = "insert into offline_message(from_, to_, content)"
                                 " values('" + fromUser + "', '" + toUser +
                         "', '" + content + "')";
            int res = mysql_query(g_db.conn, sql.c_str());
            if(res) {
                cout << "insert error" << endl;
            }
        }
    }
    else if(messageType == "GetOfflineMessage") {   //拉取离线信息
        string fromUser = doc["From"].GetString();
        string toUser = doc["To"].GetString();
        string content = fromUser + ": " + doc["Content"].GetString();
        cout << fromUser << ": GetOfflineMessage" << endl;
        string sql = "select content from offline_message where from_ = '" +
                     toUser + "' and to_ = '" + fromUser + "'";
        mysql_query(g_db.conn, sql.c_str());
        MYSQL_RES* res = mysql_store_result(g_db.conn);
        MYSQL_ROW row = NULL;
        int row_count = mysql_num_rows(res);
        int field_count = mysql_num_fields(res);
        row = mysql_fetch_row(res);
        while(row != NULL) {
            StringBuffer s;
            Writer<StringBuffer> writer(s);
            writer.StartObject();
            writer.Key("From");
            writer.String(toUser.c_str());
            writer.Key("Type");
            writer.String("203");
            writer.Key("Content");
            writer.String(row[0]);
            writer.EndObject();
            conn->send(s.GetString(), s.GetSize());
            row = mysql_fetch_row(res);
        }

        //删除已拉取的消息
        sql = "delete from offline_message where from_ = '" + toUser +"' and to_ = '" + fromUser + "'";
        if(mysql_query(g_db.conn, sql.c_str())) {
            cout << "delete error" << endl;
        }
    }
//    else if(messageType == "SendFile") {
//        StringBuffer s;
//        Writer<StringBuffer> writer(s);
//        writer.StartObject();
//        writer.Key("Type");
//        writer.String("204");   //可以开始接收文件，需要设置此回应否则会发生RequestMessage和文件信息粘包的问题
//        writer.Key("Content");
//        writer.String("");
//        writer.Key("From");
//        writer.String("");
//        writer.EndObject();
//        write(events[i].data.fd, s.GetString(), s.GetSize());
//        char buffer[BUFFERSIZE];
//        bzero(buffer, BUFFERSIZE);
//        string fileName = doc["Content"].GetString();
//        string fromUser = doc["From"].GetString();
//        string toUser = doc["To"].GetString();
//        FILE* fp = fopen(fileName.c_str(), "wb");
//        if(fp == NULL) {
//            cout << "Open file failed." << endl;
//        }
//        memset(buffer, 0, BUFFERSIZE);
//        int length = 0;
//        while((length = recv(events[i].data.fd, buffer, BUFFERSIZE, 0)) > 0) {
//            if(fwrite(buffer, sizeof(char), length, fp) < length) {
//                printf("File: %s write failed\n", fileName.c_str());
//                break;
//            }
//            memset(buffer, 0, BUFFERSIZE);
//        }
//        fclose(fp);
//        printf("File transfer success!\n");
//    }
}

void LoginServer::splitEx(const string &src, string separate_character, vector<string> &vctStr) {
    vector<string> strs;

    int separate_characterLen = separate_character.size();//分割字符串的长度,这样就可以支持如“,,”多字符串的分隔符
    int lastPosition = 0, index = -1;
    while (-1 != (index = src.find(separate_character, lastPosition)))
    {
        vctStr.push_back(src.substr(lastPosition, index - lastPosition));
        lastPosition = index + separate_characterLen;
    }
    string lastString = src.substr(lastPosition);//截取最后一个分隔符后的内容
    if (!lastString.empty())
        vctStr.push_back(lastString);//如果最后一个分隔符后还有内容就入队
}


