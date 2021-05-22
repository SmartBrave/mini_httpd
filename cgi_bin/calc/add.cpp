#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<iostream>
using namespace std;

int main()
{
    char* n1,*n2;
    char* p;
    char method[1024],query_string[1024],buf[1024];
    //memset(method,'\0',sizeof(method));
    memset(query_string,'\0',sizeof(query_string));
    memset(buf,'\0',sizeof(buf));
    //sprintf(method,"%s",getenv("METHOD"));
    sprintf(query_string,"%s",getenv("QUERY_STRING"));
    p=query_string;
    while(*p!='\0'&&*p!='=')
        p++;
    if(*p=='\0')
    {
        n1="0";
    }
    else
    {
        n1=++p;
    }
    while(*p>='0'&&*p<='9')
        p++;
    *p++='\0';
    while(*p!='\0'&&*p!='=')
        p++;
    if(*p=='\0')
    {
        n2="0";
    }
    else
    {
        n2=++p;
    }
    printf("HTTP/1.1 200 OK\r\n\r\n");
    printf("<html><title>result</title><body><h1>The result is ");
    printf("%d",atoi(n1)+atoi(n2));
    printf("</h1></body></html>");
    return 0;
}
