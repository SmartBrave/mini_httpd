#include"../../sql_connect/sql.h"
#include<string.h>
#include<map>
#include<list>
#include<vector>
#include<string>
#include<stdio.h>
#include<unistd.h>
#include<iostream>
using namespace std;

#define DEBUG
#undef DEBUG


int getcid(char* buf)
{
    if(buf==NULL)
        return -1;
    char* p=buf;
    while(*p!='\0')
    {
        if(*p=='_')
        {
            //*p='\0';
            break;
        }
        p++;
    }
    return atoi(buf);
}

char* getsimple(char* buf)
{
    if(buf==NULL)
        return NULL;
    char* p=buf;
    while(*p!='\0')
    {
        if(*p=='_')
        {
            //*p='\0';
            return p+1;
        }
        p++;
    }
    return NULL;
}

char* getnotesimple(char* buf,char** value)
{
    if(buf==NULL)
        return NULL;
    char* p=buf;
    char* tmp=NULL;
    while(*p!='\0')
    {
        if(*p=='_')
        {
            tmp=p+1;
            *p='\0';
        }
        if(*p=='=')
        {
            *p='\0';
            *value=p+1;
            return tmp;
        }
        p++;
    }
    return NULL;
}
void modify(char* buf)
{
    if(buf==NULL)
        return;
    char* p=buf;
    while(*p!='\0')
    {
        switch(*p)
        {
            case '+':
                *p=' ';
                break;
            case '%':
                p++;
                switch(*p)
                {
                    case '2':
                        p++;
                        switch(*p)
                        {
                            case '1':
                                break;
                            default:
                                break;
                        }
                        break;
                    default:
                        break;
                }
                break;
            default:
                break;
        }
        p++;
    }
}


int main()
{
    cout<<"Content-type: text/html\n\n"<<endl;

    cout<<"<html>"<<endl;
    cout<<"<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"<<endl;
    cout<<"<body>"<<endl;

    //int Length=atoi(getenv("CONTENT_LENGTH"));
    char buf[1024];
    //size_t size=read(0,buf,Length);
    //buf[size]='\0';
    sprintf(buf,"%s",getenv("QUERY_STRING"));
    int Length=strlen(buf);
    char* p=NULL; 
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
    cout<<"Length: "<<Length<<"<br/>"<<endl;
    cout<<"buf: "<<buf<<"<br/>"<<endl;
#endif
    p=buf;
    int i,j,k; 
    char us[1024],pw[1024];
    //sscanf(buf,"user=%s passwd=%s",us,pw);
    i=0;
    while(*p++!='=');
    while(*p!='&')
    {
        us[i++]=*p++;
    }
    us[i]='\0';
    i=0;
    while(*p++!='=');
    while(*p!='&')
    {
        pw[i++]=*p++;
    }
    pw[i]='\0';
    strcpy(buf,p+1);
    p=buf;
    i=0;
    //while(p!='\0')
    //{
    //    printf("%x ",*p);
    //    if(*p==' ' || *p==0x9)
    //        i++;
    //    if(i==2)
    //        break;
    //    p++;
    //}
    //strcpy(buf,p+1);
    //可以验证user和passwd正确后再执行数据库操作
#ifdef DEBUG
    cout<<"buf: "<<buf<<"<br/>"<<endl;
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
    if(!s.connect_to_table("schedule","CompanyID int NOT NULL AUTO_INCREMENT PRIMARY KEY,CompanyName char(60) NOT NULL,  NotDeliver bool NOT NULL,  Deliverd bool NOT NULL,  Exam bool NOT NULL,  FirstInterview bool NOT NULL,  SecondInterview bool NOT NULL,  ThirdInterview bool NOT NULL,  HRInterview bool NOT NULL,  GetOffer bool NOT NULL,  Note text,  UserID int NOT NULL"))
    {
        cout<<"error1!</body></html>"<<endl;
        exit(1);
    }
    const char* simple[]={"CID","CName","ND","Dd","Ex","FI","SI","TI","HRI","GO","No","UID"};
    const char* full[]={"CompanyID","CompanyName","NotDeliver","Deliverd","Exam","FirstInterview","SecondInterview","ThirdInterview","HRInterview","GetOffer","Note","UserID"};
    char* cur=NULL;
    char* tmp=NULL;
    map<string,int> msi;
    for(i=0;i<sizeof(simple)/sizeof(simple[0]);++i)
    {
        msi.insert(pair<string,int>(simple[i],i));
    }
    char MaxID_c[12];
    int MaxID_i;
    for(i=Length-1;i>=0;--i)
    {
        if(buf[i]=='&')
            break;
    }
    j=0;
    i++;
    while(buf[i]!='_')
        MaxID_c[j++]=buf[i++];
    MaxID_c[j]='\0';
    MaxID_i=atoi(MaxID_c);
#ifdef DEBUG
    cout<<"MaxID_c: "<<MaxID_c<<endl;
    cout<<"<br/>MaxID_i: "<<MaxID_i<<"<br/>"<<endl;
#endif

    cur=buf;
    tmp=buf;
    Length=strlen(buf);
    list<string> ls;
    list<string> note;
    while(cur<buf+Length)
    {
        if(*cur=='=')
        {
            if(*(cur-1)=='o' && *(cur-2)=='N')
            {
                while(*cur!='\0' && *cur!='&')
                    cur++;
                *cur='\0';
                note.push_back(tmp);
                tmp=cur+1;
            }
            else
            {
                *cur='\0';
                ls.push_back(tmp);
            }
        }
        else if(*cur=='&')
        {
            tmp=cur+1;
        }
        cur++;
    }
    list<string>::iterator it;
    //for(it=note.begin();it!=note.end();++it)
    //{
    //    int length=it->size();
    //    int i;
    //    for(i=0;i<length;++i)
    //    {
    //        if((*it)[i]=='%')
    //        {
    //            string tmp;
    //            char ch;
    //            tmp.push_back((*it)[i]);
    //            tmp.push_back((*it)[i+1]);
    //            tmp.push_back((*it)[i+2]);
    //            sscanf(tmp.c_str(),"%x",&ch);
    //            it->erase(it->begin()+i,it->begin()+i+3);
    //            it->insert(it->begin()+i,ch);
    //        }
    //    }
    //}
#ifdef DEBUG
    cout<<"list: "<<endl;
    for(it=ls.begin();it!=ls.end();++it)
    {
        cout<<*it<<"&nbsp;&nbsp;"<<endl;
    }
    cout<<"note: "<<endl;
    for(it=note.begin();it!=note.end();++it)
    {
        cout<<*it<<"&nbsp;&nbsp;"<<endl;
    }
#endif
    vector<vector<bool> > v;
    v.resize(MaxID_i+1);
    for(i=1;i<MaxID_i+1;++i)
    {
        v[i].resize(8);
        for(j=0;j<8;++j)
        {
            v[i][j]=false;
        }
    }
#ifdef DEBUG
    cout<<"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"<<"<br/>"<<endl;
#endif
    it=ls.begin();
    for(it=ls.begin();it!=ls.end();++it)
    {
        sprintf(buf,"%s",(*it).c_str());
        int id=getcid(buf);
#ifdef DEBUG
        //cout<<"buf: "<<buf<<"<br/>id: "<<id<<"<br/>"<<endl;
        //cout<<"BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB"<<"<br/>"<<endl;
        //cout<<"msi.size(): "<<msi.size()<<"<br/>"<<endl;
        //cout<<"getsimple(buf): "<<getsimple(buf)<<"<br/>"<<endl;
        //cout<<"msi[getsimple(buf)]-2: "<<msi[getsimple(buf)]-2<<"<br/>"<<endl;
        cout<<"id: "<<id<<"<br/>"<<endl;
        //cout<<"v[id][msi[getsimple(buf)]-2]"<<v[id][msi[getsimple(buf)]-2]<<"<br/>"<<endl;
#endif
        char* p=getsimple(buf);
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        if(p!=NULL)
            v[id][msi[p]-2]=true;
#ifdef DEBUG
        cout<<"pos: "<<msi[getsimple(buf)]<<"<br/>"<<endl;
#endif
    }
    string str;
#ifdef DEBUG
    cout<<"CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC"<<"<br/>"<<endl;
    cout<<"v.size(): "<<v.size()<<"<br/>"<<endl;
    cout<<"note.size(): "<<note.size()<<"<br/>"<<endl;
#endif
    for(i=1;i<v.size();++i)
    {
#ifdef DEBUG
        cout<<"beginbeginbewginbeginbegin"<<"<br/>"<<endl;
#endif
        str.clear();
        str="UPDATE schedule SET ";
        for(j=0;j<v[i].size();++j)
        {
            str+=full[j+2];
            if(v[i][j]==true)
            {
                str+="=true,";
            }
            else
            {
                str+="=false,";
            }
        }
#ifdef DEBUG
        cout<<"DDDDDDDDDDDDDDDDDDDDDDDDDDDDDDDD"<<"<br/>"<<endl;
#endif
        sprintf(buf,"%s",note.front().c_str());
#ifdef DEBUG
        cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<"<br/>"<<endl;
#endif
        char* tmp=NULL;
        char** value=&tmp;
        const char* p=full[msi[getnotesimple(buf,value)]];
        if(atoi(buf)==i)
        {
            note.pop_front();
            str+=p;
        }
        else
        {
            str+="Note";
        }
        modify(*value);
        str+="='";
        str+=*value;
        str+="'";

        str+=" WHERE CompanyID=";
        sprintf(buf,"%d",i);
        str+=buf;
        str+=" and UserID=(select UserID from user where UserName='";
        str+=us;
        str+="');";
#ifdef DEBUG
        cout<<"str: "<<str<<"<br/>"<<endl;
#endif
        s.query(str.c_str());
    }
#ifdef DEBUG
    cout<<"RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR"<<endl;
#endif
    cout<<"<p>Success!</p>"<<endl;
    cout<<"<form action=\"show_offer\" method=\"POST\" emctype=\"text/plain\">";
    cout<<"<input type=\"hidden\" name=\"user\" value=\"";
    cout<<us;
    cout<<"\"/>";
    cout<<"<input type=\"hidden\" name=\"passwd\" value=\"";
    cout<<pw;
    cout<<"\"/>";
    cout<<"<br/><input type=\"submit\" value=\"返回\"/>";
    cout<<"<br/></form>"<<endl;
    cout<<"</body></html>"<<endl;
    return 0;
}
