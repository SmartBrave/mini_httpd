#include<stdio.h>
#include<stdlib.h>

int main()
{
    char* n1,*n2;
    char* p;
    char method[1024],query_string[1024];
    sprintf(method,"%s",getenv("METHOD"));
    sprintf(query_string,"%s",getenv("QUERY_STRING"));
    p=method;
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
    printf("<html><title>result</title><h1>The result is ");
    printf("%d",atoi(n1)+atoi(n2));
    printf("</h1></html>");
    return 0;
}
