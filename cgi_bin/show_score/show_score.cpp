#include"../../sql_connect/sql.h"
#include<stdio.h>
#include<iostream>
using namespace std;

int main()
{
    const char* host="localhost";
    const char* user="root";
    const char* passwd="";
    const char* db="score";
    const char* arr[5]={"FirstTerm","SecondTerm","ThirdTerm","FourthTerm","FifthTerm"};
    sql s(host,user,passwd,db);
    MYSQL_RES* result=NULL;
    MYSQL_ROW row;
    int rows,fields;
    MYSQL_FIELD *field;
    char buf[1024];
    int i,j,k; 
    cout<<"<html>"<<endl;
    cout<<"<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>\n"<<endl;
    cout<<"<body>"<<endl;
    for(i=0;i<5;++i)
    {
        if(!s.connect_to_table(arr[i],"courseID int(4) not null primary key,course_name varchar(100) not null,credit double(2,1),origin_score varchar(10),transfer_score varchar(5),grade_point int(4),teacher varchar(20)"))
        {
            printf("continue\n");
            continue;
        }
        sprintf(buf,"select * from %s;",arr[i]);
        s.query(buf);
        //if(!s.query(buf))
        //{
        //    continue;
        //}
        result=s.get_res();
        if(result==NULL)
        {
            continue;
        }
        rows=mysql_num_rows(result);
        fields=mysql_num_fields(result);
        cout<<"<h1>第"<<i+1<<"学期</h1>"<<endl;
        cout<<"<table border=1 cellspacing='0' width='900'>"<<endl;;
        for(j=0;field=mysql_fetch_field(result);++j)
        {
            printf("<th>%s</th>\n",field->name);
        }
        for(j=0;j<rows;++j)
        {
            row=mysql_fetch_row(result);
            cout<<"<tr>";
            for(k=0;k<fields;++k)
            {
                cout<<"<td align='center'>"<<endl;
                printf("%s\t",row[k]);
                cout<<"</td>"<<endl;
            }
            cout<<"</tr>";
        }
        cout<<"</table>"<<endl;

    }
    return 0;
}
