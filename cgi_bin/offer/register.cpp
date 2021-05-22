#include<iostream>
#include<string>
#include<unistd.h>
#include"../../sql_connect/sql.h"
using namespace std;

#define DEBUG
#undef DEBUG

int main()
{
    printf("Content-type: text/html\n\n");
    cout<<"<html>"<<endl;
    cout<<"<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"<<endl;
    cout<<"<body>"<<endl;

    char buf[1024];
    char us[1024],pw[1024];
    char* p=NULL;
    int i=0;
    int Length=atoi(getenv("CONTENT_LENGTH"));
    size_t size=read(0,buf,Length);
    buf[size]='\0';
    p=buf;
    bool flag=true;
    while(*p!='\0')
    {
        if(*p=='N' && *(p+1)=='o')
            flag=false;
        if(flag==false && *p==' ')
            flag=true;
        if(*p==' ' && flag==true)
            *p='&';
        p++;
    }
#ifdef DEBUG
    cout<<"buf: "<<buf<<"<br/>"<<endl;
#endif
    p=buf;
    i=0;
    while(*p++!='=');
    while(*p!='&')
    {
        us[i++]=*p++;
    }
    us[i]='\0';
    i=0;
    while(*p++!='=');
    while(*p!='\0')
    {
        pw[i++]=*p++;
    }
    pw[i]='\0';
#ifdef DEBUG
    cout<<"us: "<<us<<"<br/>"<<endl;
    cout<<"pw: "<<pw<<"<br/>"<<endl;
#endif
    const char* host="localhost";
    const char* user="root";
    const char* passwd="SmartBrave";
    //const char* passwd="";
    const char* db="offer";
    sql s(host,user,passwd,db);
    MYSQL_RES* result=NULL;
    MYSQL_ROW row;
    int rows,fields;
    MYSQL_FIELD *field;
    char buf1[1024];
    cout<<"<body>"<<endl;
    if(!s.connect_to_table("user","UserId int not null auto_increment,UserName char(20) not null,Passwd char(20) not null,primary key(UserID)"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
    string str;
    str="SELECT UserID from user where UserName='";
    str+=us;
    str+="';";
#ifdef DEBUG
    cout<<"str: "<<str<<"<br/>"<<endl;
#endif
    s.query(str.c_str());
    result=s.get_res();
    rows=mysql_num_rows(result);
    if(rows!=0)
    {
        cout<<"用户名已被使用!"<<endl;
        //cout<<"<form action=\"register\" method=\"POST\" emctype=\"text/plain\"><br/>"<<endl;
        //cout<<"<input type=\"hidden\" name=\"user\" value=\"";
        //cout<<us;
        //cout<<"\"/><br/>"<<endl;
        //cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
        //cout<<pw;
        //cout<<"\"/><br/>"<<endl;
        //cout<<"<input type=\"submit\" value=\"重新注册\"/><br/>"<<endl;;
        //cout<<"</form>"<<endl;
        cout<<"<a href=\"create_user\">重新注册</a>"<<endl;
        cout<<"<a href=\"login_offer\">登陆</a>"<<endl;
    }
    else
    {
        str="INSERT INTO user (UserName,Passwd) value('";
        str+=us;
        str+="','";
        str+=pw;
        str+="');";
#ifdef DEBUG
        cout<<"str: "<<str<<"<br/>"<<endl;
#endif
        s.query(str.c_str());
        cout<<"Success!"<<endl;
        cout<<"<form action=\"show_offer\" method=\"POST\" emctype=\"text/plain\"><br/>"<<endl;
        cout<<"<input type=\"hidden\" name=\"user\" value=\"";
        cout<<us;
        cout<<"\"/><br/>"<<endl;
        cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
        cout<<pw;
        cout<<"\"/><br/>"<<endl;
        cout<<"<input type=\"submit\" value=\"返回\"/><br/>"<<endl;;
    }

    cout<<"</body></html>";
    return 0;
}
