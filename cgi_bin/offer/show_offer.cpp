#include<iostream>
#include<typeinfo>
#include<unistd.h>
#include<string.h>
#include"../../sql_connect/sql.h"
using namespace std;

#define DEBUG
#undef DEBUG

int main()
{
    printf("Content-type: text/html\n\n");
    //char url[1024];
    char* p=NULL;
    char buf[1024];
    int i,j,k; 
    //if((p=getenv("QUERY_STRING"))==NULL)
    //{
    //    cout<<"<html><body>"<<endl;
    //    cout<<"error1!"<<endl;
    //    cout<<"</body></html>";
    //}
    cout<<"<html>"<<endl;
    cout<<"<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"<<endl;
    cout<<"<body>"<<endl;
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
    p=buf;
    char us[1024],pw[1024];
    i=0;
    //sscanf(buf,"user=%s passwd=%s",us,pw);
#ifdef DEBUG
    cout<<"size: "<<size<<"<br/>"<<endl;
    cout<<"buf: "<<buf<<"<br/>"<<endl;
#endif
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
#ifdef DEUBG
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
    cout<<"<body>"<<endl;
    if(!s.connect_to_table("user","UserId int not null auto_increment,UserName char(20) not null,Passwd char(20) not null,primary key(UserID)"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
#ifdef DEBUG
    cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<"<br/>"<<endl;
#endif
    string str="SELECT UserID FROM user WHERE UserName='";
    str+=us;
    str+="';";
#ifdef DEBUG
    cout<<"str: "<<str<<"<br/>"<<endl;
#endif
    s.query(str.c_str());
    //if(!s.query(buf))
    //{
    //    cout<<"error3!</body></html>"<<endl;
    //    exit(1);
    //}
    result=s.get_res();
    if(result==NULL)
    {
        cout<<"error4!</body></html>"<<endl;
        exit(1);
    }
    rows=mysql_num_rows(result);
    fields=mysql_num_fields(result);
#ifdef DEBUG
    cout<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"<<"<br/>"<<endl;
#endif
    if(rows==0)
    {
        cout<<"对不起，您还未注册!"<<endl;
        //cout<<"<a href=\"register\">注册</a>"<<endl;
        cout<<"<form action=\"register\" method=\"POST\">"<<endl;
        cout<<"用户名:<input type=\"text\" name=\"user\"/>"<<endl;
        cout<<"<br/>密&nbsp码:<input type=\"password\" name=\"passwd\"/>"<<endl;
        cout<<"<br/><input type=\"submit\" value=\"注册\"/>"<<endl;
        cout<<"</form>"<<endl;
    }
    else
    {
        //str.pop_back();
        str.erase(str.begin()+str.size()-1,str.end());
        str+=" AND Passwd='";
        str+=pw;
        str+="';";
        s.query(str.c_str());
        result=s.get_res();
        if(result==NULL)
        {
            cout<<"error4!</body></html>"<<endl;
            exit(1);
        }
        rows=mysql_num_rows(result);
        fields=mysql_num_fields(result);
        if(rows==0)
        {
            cout<<"用户名或密码错误!"<<endl;
            cout<<"<a href=\"login_offer\">返回</a>"<<endl;
            cout<<"</body></html>";
            exit(1);
        }
    }
    //for(i=0;i<rows;++i)
    //{
    //    row=mysql_fetch_row(result);
    //    for(j=0;j<fields;++j)
    //    {
    //        cout<<row[j]<<endl;
    //    }
    //}
    int id=0;
    row=mysql_fetch_row(result);
    //cout<<row[0]<<endl;
    id=atoi(row[0]);
#ifdef DEBUG
    cout<<"CCCCCCCCCCCCCCCCCCCCCCCCCCCCC"<<"<br/>"<<endl;
#endif
    if(!s.connect_to_table("schedule","CompanyID int not null, UserID int not null, NotDeliver bool not null default false, Deliverd bool not null default false, Exam bool not null default false, FirstInterview bool not null default false, SecondInterview bool not null default false, ThirdInterview bool not null default false, HRInterview bool not null default false, GetOffer bool not null default false, Note text, primary key(CompanyID,UserID)"))
    {
        cout<<"error2!</body></html>"<<endl;
        exit(1);
    }
#ifdef DEBUG
    cout<<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"<<"<br/>"<<endl;
#endif
    //sprintf(buf,"select * from schedule where UserID='%d';",id);
    str="select schedule.CompanyID as CID,CompanyName as CN,NotDeliver as ND,Deliverd as Dd,Exam as Ex,FirstInterview as FI,SecondInterview as SI,ThirdInterview as TI,HRInterview as HRI,GetOffer as GO,Note as No from schedule,company,user where schedule.UserID=user.UserID and schedule.CompanyId=company.CompanyID and user.UserName='";
    str+=us;
    str+="' order by CID asc;";
#ifdef DEBUG
    cout<<"str: "<<str<<"<br/>"<<endl;
    cout<<"EEEEEEEEEEEEEEEEEEEEEEEEEE"<<"<br/>"<<endl;
#endif
    s.query(str.c_str());
    //if(!s.query(buf))
    //{
    //    cout<<"error3!</body></html>"<<endl;
    //    exit(1);
    //}
    result=s.get_res();
    if(result==NULL)
    {
        cout<<"error4!</body></html>"<<endl;
        exit(1);
    }
    rows=mysql_num_rows(result);
    fields=mysql_num_fields(result);
#ifdef DEBUG
    cout<<"rows: "<<rows<<"<br/>"<<endl;
    cout<<"fields: "<<fields<<"<br/>"<<endl;
#endif
    if(rows==0)
    {
        cout<<"<p>注意:公司名称和备注栏中请不要填写中文,请使用半角英文及数字,否则会出现乱码问题,谢谢配合!</p>"<<endl;
        cout<<"<form action=\"add\" method=\"POST\" ><br/>"<<endl;
        cout<<"公司:<input type=\"text\" name=\"CompanyName\"/> <br/>"<<endl;
        cout<<"<input type=\"hidden\" name=\"user\" value=\"";
        cout<<us;
        cout<<"\"/>";
        cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
        cout<<pw;
        cout<<"\"/>";
        cout<<"<input type=\"submit\" value=\"添加\"/><br/>"<<endl;
        cout<<"</form><br/>"<<endl;
    }
    else
    {
#ifndef DEBUG
        cout<<"<p>注意:公司名称和备注栏中请不要填写中文,请使用半角英文及数字,否则会出现乱码问题,谢谢配合!</p>"<<endl;
        cout<<"<p>目前暂没有开发删除功能,如需要删除,请联系我.我的微信是:779850695.</p>"<<endl;
        cout<<"<form action=\"save\" method=\"GET\">"<<endl;;
        cout<<"<table border=1 cellspacing='0' width='900'>"<<endl;;
        cout<<"<tr><th>公司</th>\n<th>待投简历</th>\n<th>已投递</th>\n<th>已笔试</th>\n<th>一面</th>\n<th>二面</th>\n<th>三面</th>\n<th>HR面</th>\n<th>获得offer</th>\n<th>备注</th>\n</tr>"<<endl;
        cout<<"<input type=\"hidden\" name=\"user\" value=\"";
        cout<<us;
        cout<<"\"/>";
        cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
        cout<<pw;
        cout<<"\"/>";
        const char* arr[]={"CID","CName","ND","Dd","Ex","FI","SI","TI","HRI","GO","No","UID"};
#endif
        for(i=0;i<rows;++i)
        {
            row=mysql_fetch_row(result);
#ifndef DEBUG
            cout<<"<tr>";
            //CompanyName
            cout<<"<td align=\"center\">"<<endl;
            cout<<row[1]<<endl;
#else
            cout<<"CompanyID: "<<row[0]<<"&nbsp;&nbsp;CompanyName: "<<row[1]<<"<br/>"<<endl;
            cout<<"fields: "<<fields<<"<br/>"<<endl;
            rows=mysql_num_rows(result);
            fields=mysql_num_fields(result);
            cout<<"rows: "<<rows<<"<br/>"<<endl;
            cout<<"fields: "<<fields<<"<br/>"<<endl;
            //for(int i=0;i<fields;++i)
            //{
            //    cout<<"i: "<<i<<"&nbsp;&nbsp;row[i]; "<<row[i]<<"<br/>"<<endl;
            //}
#endif
#ifndef DEBUG
            cout<<"</td>"<<endl;
            //Other
#endif
            for(j=2;j<fields-1;++j)
            {
#ifdef DEBUG
                cout<<"i: "<<i<<"&nbsp;&nbsp;j: "<<j<<"&nbsp;&nbsp;"<<"fields[j]: "<<row[j]<<"<br/>"<<endl;
#else
                cout<<"<td align='center'>"<<endl;
                if(atoi(row[j])==0)
                {
                    cout<<"<input type=\"checkbox\" name=\"";
                    cout<<row[0]<<"_"<<arr[j];
                    cout<<"\" />"<<endl;
                }
                else
                {
                    cout<<"<input type=\"checkbox\" name=\"";
                    cout<<row[0]<<"_"<<arr[j];
                    cout<<"\" checked=\"checked\"/>"<<endl;
                }
                cout<<"</td>"<<endl;
#endif
            }
#ifndef DEBUG
            cout<<"<td align='center'>"<<endl;
            cout<<"<input type=\"text\" name=\"";
            cout<<row[0]<<"_"<<arr[fields-1];
            cout<<"\" value=\"";
            if(row[fields-1]!=NULL)
            {
                cout<<row[fields-1]<<endl;
            }
            else
            {
                cout<<"None"<<endl;
                //cout<<"test"<<endl;
                //cout<<row[fields-2]<<endl;
            }
#endif
#ifdef DEBUG
            if(row[fields-1]!=NULL)
            {
                cout<<"row[fields-1]: "<<row[fields-1]<<"<br/>"<<endl;
                cout<<"Note: "<<row[fields-1]<<"<br/>"<<endl;
            }
#else
            cout<<"\"enctype=\"text/plain\"/>"<<endl;
            cout<<"</td>"<<endl;
            cout<<"</tr>"<<endl;
#endif
#ifdef DEBUG
            cout<<"FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"<<"<br/>"<<endl;
#endif
        }
#ifndef DEBUG
        cout<<"</table><br/>"<<endl;
        cout<<"<input type=\"submit\" value=\"保存\"/>"<<endl;
        cout<<"</form>"<<endl;
#endif
        cout<<"<form action=\"add\" method=\"POST\">";
        cout<<"添加公司:";
        cout<<"<input type=\"text\" name=\"CompanyName\"/>";
        cout<<"<input type=\"hidden\" name=\"user\" value=\"";
        cout<<us;
        cout<<"\"/>";
        cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
        cout<<pw;
        cout<<"\"/>";
        cout<<"<br/><input type=\"submit\" value=\"添加\"/>";
        cout<<"<br/></form>"<<endl;
    }
    cout<<"</body></html>";
    return 0;
}
