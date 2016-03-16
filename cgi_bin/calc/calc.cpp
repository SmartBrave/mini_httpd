#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<iostream>
using namespace std;


#define _COMM_SIZE_ 1024

int main()
{
    char query_string[_COMM_SIZE_];
    char *a=query_string,*b;
    memset(query_string,'\0',sizeof(query_string));
    int i=0;
    sprintf(query_string,"%s",getenv("QUERY_STRING"));
    //for(i=0;i<length;++i)
    //{
    //    read(0,&post_data[i],1);
    //}
    //post_data[i]='\0';
    while(true)
    {
        if(*a=='=')
        {
            a++;
            break;
        }
        a++;
    }
    b=a;
    while(true)
    {
        if(*b=='&')
            *b='\0';
        else if(*b=='=')
        {
            b++;
            break;
        }
        b++;
    }
    cout<<"<html>"<<endl;
    //cout<<"<meta http-equiv=\"content-type\" content=\"text/html;charset=utf-8\"/>"<<endl;
    cout<<"<body>"<<endl;
    cout<<"<h1>a="<<a<<"</h1>"<<endl<<"<h1>b="<<b<<"</h1>"<<endl;
    cout<<"<h1>res:"<<atoi(a)+atoi(b)<<"</h1>"<<endl;
    //if(strcasecmp(method,"get")==0)
    //{
    //}
    //else if(strcasecmp(method,"post"))
    //{
    //    printf("post data: %s</p><br/>",post_data);
    //}
    //else
    //    return 1;
    cout<<"</body>"<<endl;
    cout<<"</html>"<<endl;
    return 0;
}
