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
    char cn[1024];
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
    cout<<"buf: "<<buf<<endl;
#endif
    p=buf;
    i=0;
    while(*p++!='=');
    if(*p=='&')
    {
        cout<<"<h2>未输入!</h2>"<<endl;
        cout<<"</body></html>"<<endl;
        exit(1);
    }
    while(*p!='&')
    {
        cn[i++]=*p++;
    }
    cn[i]='\0';
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
    cout<<"<br/>cn: "<<cn<<endl;
    cout<<"<br/>us: "<<us<<endl;
    cout<<"<br/>pw: "<<pw<<"<br/>"<<endl;
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
    string str;
    if(!s.connect_to_table("company","CompanyID int not null primary key auto_increment,CompanyName char(60)  not null"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
cont:
    str="select CompanyID from company where CompanyName='";
    str+=cn;
    str+="';";
#ifdef DEBUG
    cout<<"str: "<<str<<"<br/>"<<endl;
#endif
    
    s.query(str.c_str());
    result=s.get_res();
    if(result==NULL)
    {
        cout<<"error4!</body></html>"<<endl;
        exit(1);
    }
    rows=mysql_num_rows(result);
    if(rows==0)
    {
        str="insert into company(CompanyName) value('";
        str+=cn;
        str+="');";
#ifdef DEBUG
        cout<<"str: "<<str<<"<br/>"<<endl;
#endif
        s.query(str.c_str());
        goto cont;
    }
    row=mysql_fetch_row(result);
    int cid=atoi(row[0]);

    if(!s.connect_to_table("user","UserID int not null auto_increment primary key,UserName char(20) not null,Passwd char(20) not null"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
    str="select UserID from user where UserName='";
    str+=us;
    str+="';";
#ifdef DEBUG
    cout<<"str: "<<str<<"<br/>"<<endl;
#endif
    
    s.query(str.c_str());
    result=s.get_res();
    if(result==NULL)
    {
        cout<<"error4!</body></html>"<<endl;
        exit(1);
    }
    rows=mysql_num_rows(result);
    if(rows==0)
    {
        cout<<"<h2>禁止从站点外提交数据!</h2><br/>"<<endl;
        cout<<"</body></html>"<<endl;
        exit(1);
    }
    row=mysql_fetch_row(result);
    int uid=atoi(row[0]);

    if(!s.connect_to_table("schedule","CompanyID int not null, UserID int not null, NotDeliver bool not null default false, Deliverd bool not null default false, Exam bool not null default false, FirstInterview bool not null default false, SecondInterview bool not null default false, ThirdInterview bool not null default false, HRInterview bool not null default false, GetOffer bool not null default false, Note text, primary key(CompanyID,UserID)"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
    sprintf(buf,"INSERT INTO schedule (CompanyID,UserID) value(%d,%d);",cid,uid);
#ifdef DEBUG
    cout<<"buf: "<<buf<<"<br/>"<<endl;
#endif
    s.query(buf);
    cout<<"Success!"<<endl;

    cout<<"<form action=\"show_offer\" method=\"POST\">";
    cout<<"<input type=\"hidden\" name=\"user\" value=\"";
    cout<<us;
    cout<<"\"/>";
    cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
    cout<<pw;
    cout<<"\"/>";
    cout<<"<br/><input type=\"submit\" value=\"返回\"/>";
    cout<<"<br/></form>"<<endl;

    cout<<"</body></html>";
    return 0;
}
