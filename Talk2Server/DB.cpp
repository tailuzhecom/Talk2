//
// Created by tailuzhecom on 18-1-18.
//

#include "DB.h"
#include <cstdio>
#include <iostream>

using namespace std;

DB::DB() {
    int res;
    conn = mysql_init(NULL);
    if(mysql_real_connect(conn, "127.0.0.1", "root", "134679asd", "chat", 0, NULL, 0)) {
        printf("connect success!\n");
    }
}

DB::~DB() {
    mysql_close(conn);
}

bool DB::exec(std::string sql) {
    if(mysql_query(conn, sql.c_str())) {
        cout << "Query error" << endl;
        cout << mysql_error(conn) << endl;
    }
    else {
        MYSQL_RES* result = mysql_use_result(conn);
        MYSQL_ROW row;
        //mysql_field_cout返回列数
        for(int i = 0; i < mysql_field_count(conn); i++) {
            row = mysql_fetch_row(result);
            if(row <= 0)
                break;
            for(int j = 0; j <mysql_num_fields(result); j++) {
                cout << row[j] << " ";
            }
            cout << endl;
        }
        mysql_free_result(result);
    }

    return false;
}

MYSQL_ROW DB::next() {
    return nullptr;
}
