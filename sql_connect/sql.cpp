#include"sql.h"

sql::sql(const char* host,const char* user,const char* passwd,const char* db)
    :_host(host),_user(user),_passwd(passwd),_db(db)
{
    char buf[1024];
    conn=mysql_init(NULL);
    _result=NULL;
    if(conn==NULL)
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        exit(1);
    }
    if(mysql_real_connect(conn,_host.c_str(),_user.c_str(),_passwd.c_str(),_db.c_str(),0,NULL,0)==NULL)
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        exit(1);
    }
    sprintf(buf,"CREATE DATABASE IF NOT EXISTS %s;",_db.c_str());
    if(mysql_query(conn,buf))
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        exit(1);
    }
}

bool sql::connect_to_table(const char* table,const char* str)
{
    if(table==NULL || str==NULL)
        return false;
    char buf[1024];
    _table=table;
    sprintf(buf,"CREATE TABLE IF NOT EXISTS %s(%s);",table,str);
    if(mysql_query(conn,buf))
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        return false;
        //exit(1);
    }
    return true;
}

bool sql::insert(char* data)
{
    char buf[1024];
    sprintf(buf,"INSERT INTO %s VALUES (%s);",_table.c_str(),data);
    if(mysql_query(conn,buf))
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        return false;
        //exit(1);
    }
    return true;
}

bool sql::query(const char* str)
{
    if(mysql_query(conn,str))
    {
        printf("Error %u: %s\n",mysql_errno(conn),mysql_error(conn));
        return false;
        //exit(1);
    }
    //if(NULL!=this->_result)
    //{
    //    mysql_free_result(_result);
    //}
    _result=mysql_store_result(conn);
    //return 1;
}

MYSQL_RES* sql::get_res()
{
    return _result;
}

void sql::show_info()
{
}

sql::~sql()
{
    if(_result!=NULL)
        mysql_free_result(_result);
    mysql_close(conn);
}

//int main()
//{
//    return 0;
//}
