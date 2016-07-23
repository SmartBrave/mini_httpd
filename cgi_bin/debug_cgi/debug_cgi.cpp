#include<stdio.h>
#include<unistd.h>
#include <iostream>
#include<stdlib.h>
#include<string.h>
#define _COMM_SIZE_ 1024
using namespace std;

int main()
{
    char method[_COMM_SIZE_],post_data[_COMM_SIZE_],query_string[_COMM_SIZE_],content_length[_COMM_SIZE_];
    memset(method,'\0',sizeof(method));
    memset(post_data,'\0',sizeof(post_data));
    memset(query_string,'\0',sizeof(query_string));
    memset(content_length,'\0',sizeof(content_length));
    int length=0;
    int i=0;
    for(i=0;i<length;++i)
    {
        read(0,&post_data[i],1);
    }
    post_data[i]='\0';
    printf("<html>");
    printf("<h1>hello world!</h1>\n");
    if(strcasecmp(method,"get")==0)
    {
        cout<<"<p>method: "<<getenv("REQUEST_METHOD")<<"<br/>"<<endl;
        cout<<"<p>query_string: "<<getenv("QUERY_STRING")<<"<br/>"<<endl;
    }
    else if(strcasecmp(method,"post"))
    {
        printf("post data: %s</p><br/>",post_data);
    }
    else
        return 1;
    printf("</html>");
    return 0;
}
