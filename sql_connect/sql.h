#ifndef _SQL_
#define _SQL_

#include<mysql/mysql.h>
#include<string>
#include<stdio.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

class sql{
public:
    sql(const char* host,const char* user,const char* passwd,const char* db);
    bool connect_to_table(const char* table,const char* str);
    bool insert(char* data);
    bool query(const char* str);
    MYSQL_RES* get_res();
    void show_info();
    ~sql();
private:
    MYSQL* conn;
    string _host;
    string _user;
    string _passwd;
    string _db;
    string _table;
    MYSQL_RES* _result;
};
#endif
