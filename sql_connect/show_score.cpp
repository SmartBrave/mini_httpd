#include<stdio.h>
#include<vector>
#include"sql_connect.h"
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;


#define _COMM_SIZE_ 1024

int main()
{
    char method[_COMM_SIZE_],post_data[_COMM_SIZE_],query_string[_COMM_SIZE_],content_length[_COMM_SIZE_];
    memset(method,'\0',sizeof(method));
    memset(post_data,'\0',sizeof(post_data));
    memset(query_string,'\0',sizeof(query_string));
    memset(content_length,'\0',sizeof(content_length));
    int length=0;
    int i=0;
    //strcpy(method,getenv("REQUEST_METHOD"));
    //strcpy(query_string,getenv("QUERY_STRING"));
    //strcpy(content_length,getenv("CONTENT_LENGTH"));
    length=atoi(content_length);
    for(i=0;i<length;++i)
    {
        read(0,&post_data[i],1);
    }
    post_data[i]='\0';
    printf("<html>\n");
    printf("<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>");
    cout<<"<body>"<<endl;
    sprintf(method,"get");
    if(strcasecmp(method,"get")==0)
    {
        const char* host="localhost";
        const char* user="root";
        const char* passwd="";
        const char* db="score";
        MYSQL_ROW row;
        MYSQL_FIELD *fd=NULL;
        vector<string> field_name;
        int fields;
        int i,j;
        for(i=0;i<5;++i)
        {
            MYSQL_RES* res;
            sql_connect conn(host,user,passwd,db);
            switch(i)
            {
                case 0:
                    conn.connect_to_table("FirstTerm","courseID int(4) primary key not null,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)");
                    conn.query("select * from FirstTerm");
                    res=conn.get_res();
                    fields=mysql_num_fields(res);
                    cout<<"<h1>第一学期</h1>"<<endl;
                    cout<<"<table border=1 cellspacing='0' width='900'>";
                    for(;fd=mysql_fetch_field(res);)
                    {
                        //field_name.push_back(fd->name);
                        printf("<th>%s</th>",fd->name);
                    }
                    while(row=mysql_fetch_row(res))
                    {
                        cout<<"<tr>"<<endl;
                        for(j=0;j<fields;++j)
                        {
                            cout<<"<td align='center'>"<<endl;
                            printf("%s\t",row[j]);
                            cout<<"</td>"<<endl;
                        }
                        cout<<"</tr>"<<endl;
                    }
                    cout<<"</table>"<<endl;
                    break;
                case 1:
                    conn.connect_to_table("SecondTerm","courseID int(4) primary key not null,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)");
                    conn.query("select * from SecondTerm");
                    res=conn.get_res();
                    fields=mysql_num_fields(res);
                    cout<<"<h1>第二学期</h1>"<<endl;
                    cout<<"<table border=1 cellspacing='0' width='900'>";
                    for(;fd=mysql_fetch_field(res);)
                    {
                        //field_name.push_back(fd->name);
                        printf("<th>%s</th>",fd->name);
                    }
                    while(row=mysql_fetch_row(res))
                    {
                        cout<<"<tr>"<<endl;
                        for(j=0;j<fields;++j)
                        {
                            cout<<"<td align='center'>"<<endl;
                            printf("%s\t",row[j]);
                            cout<<"</td>"<<endl;
                        }
                        cout<<"</tr>"<<endl;
                    }
                    cout<<"</table>"<<endl;
                    break;
                case 2:
                    conn.connect_to_table("ThirdTerm","courseID int(4) primary key not null,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)");
                    conn.query("select * from ThirdTerm");
                    res=conn.get_res();
                    fields=mysql_num_fields(res);
                    cout<<"<h1>第三学期</h1>"<<endl;
                    cout<<"<table border=1 cellspacing='0' width='900'>";
                    for(;fd=mysql_fetch_field(res);)
                    {
                        //field_name.push_back(fd->name);
                        printf("<th>%s</th>",fd->name);
                    }
                    while(row=mysql_fetch_row(res))
                    {
                        cout<<"<tr>"<<endl;
                        for(j=0;j<fields;++j)
                        {
                            cout<<"<td align='center'>"<<endl;
                            printf("%s\t",row[j]);
                            cout<<"</td>"<<endl;
                        }
                        cout<<"</tr>"<<endl;
                    }
                    cout<<"</table>"<<endl;
                    break;
                case 3:
                    conn.connect_to_table("FourthTerm","courseID int(4) primary key not null,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)");
                    conn.query("select * from FourthTerm");
                    res=conn.get_res();
                    fields=mysql_num_fields(res);
                    cout<<"<h1>第四学期</h1>"<<endl;
                    cout<<"<table border=1 cellspacing='0' width='900'>";
                    for(;fd=mysql_fetch_field(res);)
                    {
                        //field_name.push_back(fd->name);
                        printf("<th>%s</th>",fd->name);
                    }
                    while(row=mysql_fetch_row(res))
                    {
                        cout<<"<tr>"<<endl;
                        for(j=0;j<fields;++j)
                        {
                            cout<<"<td align='center'>"<<endl;
                            printf("%s\t",row[j]);
                            cout<<"</td>"<<endl;
                        }
                        cout<<"</tr>"<<endl;
                    }
                    cout<<"</table>"<<endl;
                    break;
                case 4:
                    conn.connect_to_table("FifthTerm","courseID int(4) primary key not null,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)");
                    conn.query("select * from FifthTerm");
                    res=conn.get_res();
                    fields=mysql_num_fields(res);
                    cout<<"<h1>第五学期</h1>"<<endl;
                    cout<<"<table border=1 cellspacing='0' width='900'>";
                    for(;fd=mysql_fetch_field(res);)
                    {
                        //field_name.push_back(fd->name);
                        printf("<th>%s</th>",fd->name);
                    }
                    while(row=mysql_fetch_row(res))
                    {
                        cout<<"<tr>"<<endl;
                        for(j=0;j<fields;++j)
                        {
                            cout<<"<td align='center'>"<<endl;
                            printf("%s\t",row[j]);
                            cout<<"</td>"<<endl;
                        }
                        cout<<"</tr>"<<endl;
                    }
                    cout<<"</table>"<<endl;
                    break;
                default:
                    break;

            }
        }
    }
    //else if(strcasecmp(method,"post"))
    //{
    //    printf("post data: %s</p><br/>",post_data);
    //}
    //else
    //    return 1;
    cout<<"<form action=\"../index.html\">"<<endl;
    cout<<"<input type=\"submit\" value=\"goto home\"/><br/>"<<endl;
    cout<<"</form>"<<endl;
    cout<<"</body>"<<endl;
    printf("</html>\n");
    return 0;
}
