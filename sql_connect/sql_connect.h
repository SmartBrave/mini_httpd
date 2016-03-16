#ifndef _SQL_CONNECT_
#define _SQL_CONNECT_
#include<string>
#include<string.h>
#include<stdlib.h>
#include<iostream>
#include<mysql/mysql.h>
#include<stdio.h>
#include"sql_connect.h"
using namespace std;
class sql_connect{
public:
    sql_connect(const char* host,const char* user,const char* passwd,const char* db)
    :_host(host),_user(user),_passwd(passwd),_db(db)
{
    char buf[1024];
    _result=NULL;
    _mysql_base=mysql_init(NULL);
    if(_mysql_base==NULL)
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
    if(mysql_real_connect(_mysql_base,_host.c_str(),_user.c_str(),_passwd.c_str(),NULL,0,NULL,0)==NULL)
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"create database if not exists %s",_db.c_str());
    if(mysql_query(_mysql_base,buf))
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"use %s",_db.c_str());
    if(mysql_query(_mysql_base,buf))
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
    
}
bool connect_to_table(const char* table,const char* tmp)
{
    if(table==NULL)
        return false;
    char buf[1024];
    _table=table;
    sprintf(buf,"create table if not exists %s(%s)",_table.c_str(),tmp);
    if(mysql_query(_mysql_base,buf))
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
}
bool insert(char* data)
{
    char buf[1024];
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"insert into %s values (%s)",_table.c_str(),data);
    if(mysql_query(_mysql_base,buf))
    {
        printf("%s,%d:error %u: %s\n",__FILE__,__LINE__,mysql_errno(_mysql_base),mysql_error(_mysql_base));
        exit(1);
    }
    return 1;
}
bool query(const char* str)
{
    mysql_query(_mysql_base,str);
    _result=mysql_store_result(_mysql_base);
}
MYSQL_RES* get_res()
{
    return _result;
}
void show_info()
{
    MYSQL_ROW row;
    MYSQL_RES* _result;
    MYSQL_FIELD* fields;
    char buf[1024];
    int i,j;
    //memset(buf,'\0',sizeof(buf));
    //sprinf(_mysql_base,"use %s",_table);
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"select * from %s",_table.c_str());
    mysql_query(_mysql_base,buf);
    _result=mysql_store_result(_mysql_base);
    while(row=mysql_fetch_row(_result))
    {
        while(fields=mysql_fetch_field(_result))
        {
            printf("%s\t",row[i]);
        }
        cout<<endl;
    }
}
~sql_connect()
{
    if(_result!=NULL)
        mysql_free_result(_result);
    mysql_close(_mysql_base);
}
private:
    MYSQL* _mysql_base;
    string _host;
    string _user;
    string _passwd;
    string _db;
    string _table;
    MYSQL_RES* _result;
};


#endif
