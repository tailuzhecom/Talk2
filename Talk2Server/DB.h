//
// Created by tailuzhecom on 18-1-18.
//

#ifndef TALK2SERVER_DB_H
#define TALK2SERVER_DB_H

#include <mysql/mysql.h>
#include <string>

class DB {
public:
    DB();
    ~DB();
    bool exec(std::string sql);
    MYSQL_ROW next();

    MYSQL* conn;
    MYSQL_RES* result;
    MYSQL_ROW row;
};


#endif //TALK2SERVER_DB_H
