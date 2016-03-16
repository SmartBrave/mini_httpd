#include"../../sql_connect/sql_connect.h"
#include<iostream>
using namespace std;

int main()
{
    char method[1024],query_string[1024],buf[1024];
    char *account=query_string,*pd;
    memset(method,'\0',sizeof(method));
    memset(query_string,'\0',sizeof(query_string));
    memset(buf,'\0',sizeof(buf));
    sprintf(method,"%s",getenv("REQUEST_METHOD"));
    sprintf(query_string,"%s",getenv("QUERY_STRING"));
    while(true)
    {
        if(*account=='=')
        {
            account++;
            break;
        }
        account++;
    }
    pd=account;
    while(true)
    {
        if(*pd=='&')
            *pd='\0';
        else if(*pd=='=')
        {
            pd++;
            break;
        }
        pd++;
    }
    const char* host="localhost";
    const char* user="root";
    const char* passwd="";
    const char* db="user"; 
    sql_connect conn(host,user,passwd,db);
    conn.connect_to_table("register","account varchar(20) not null primary key,passwd varchar(20)");
    sprintf(buf,"'%s','%s'",account,pd);
    cout<<"<html>"<<endl;
    cout<<"<body>"<<endl;
    if(conn.insert(buf))
    {
        cout<<"<h1>Success!</h1>"<<endl;
    }
    else
    {
        cout<<"<h1>Failed!</h1>"<<endl;
    }
    cout<<"</body>"<<endl;
    cout<<"</html>"<<endl;
    return 0;
}
