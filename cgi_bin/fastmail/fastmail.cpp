#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string>
#include<json/json.h>
using namespace std;

int main()
{
    char buf[10240],url[1024];
    int mailnum=0;
    char* pc=NULL,*pn=NULL;
    memset(buf,'\0',sizeof(buf));
    memset(url,'\0',sizeof(url));
    sprintf(buf,"%s",getenv("QUERY_STRING"));
    pc=buf;
    while(pc!=NULL && *pc++!='=');
    pn=pc;
    while(pn!=NULL && *pn!='&')
    {
        pn++;
    }
    if(*pn=='&')
        *pn++='\0';
    while(pn!=NULL && *pn++!='=');
    const char* ip="118.194.40.5";
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(80);
    inet_aton(ip,&local.sin_addr);
    int sock=socket(AF_INET,SOCK_STREAM,0);
    if(sock==-1)
    {
        perror("socket");
        exit(1);
    }
    int ret=connect(sock,(struct sockaddr*)&local,sizeof(local));
    if(ret!=0)
    {
        perror("connect");
        exit(1);
    }
    //sprintf(url,"GET http://api.kuaidi100.com/api?id=%s&com=%s&nu=%s&valicode=1&show=2&muti=1&order=desc\r\n\r\n","c083b7a698683f5b",pc,pn);
    sprintf(url,"GET http://www.kuaidi100.com/applyurl?key=%s&com=%s&nu=%s\r\n\r\n","c083b7a698683f5b",pc,pn);
    send(sock,url,strlen(url),0);
    printf("HTTP/1.1 200 OK\r\n\r\n");
    size_t size=0;
    size_t capacity=sizeof(buf);
    while(size=read(sock,buf+size,capacity-size)>0)
    {
        capacity-=size;
    }
    cout<<"<html><head><meta charset=\"utf-8\"></head><title>查询结果</title>"<<endl;
    cout<<"<iframe name=\"kuaido100\" src=\""<<buf<<"\" width=\"600\" height=\"380\" marginwidth=\"0\" marginheight=\"0\" hspace=\"0\" vspace=\"0\" frameborder=\"0\" scrolling=\"no\"></iframe>"<<endl;
    cout<<"</html>"<<endl;
    //Json::Reader reader;
    //Json::Value value;
    //string status,state;
    //string message,time,context;
    //if(reader.parse(buf,value))
    //{
    //    message=value["message"].asString();
    //    status=value["status"].asString();
    //    if(atoi(status.c_str())==0)
    //    {
    //        cout<<"<p>暂时没有结果</p>";
    //        cout<<"</html>"<<endl;
    //        return 0;
    //    }
    //    else if(atoi(status.c_str())==1)
    //    {
    //        state=value["state"].asString();
    //        switch(atoi(state.c_str()))
    //        {
    //            case 0:
    //                cout<<"<p>正在运输</p>";
    //                break;
    //            case 1:
    //                cout<<"<p>已揽件</p>";
    //                break;
    //            case 2:
    //                cout<<"<p>货物寄送过程中出现了问题</p>";
    //                break;
    //            case 3:
    //                cout<<"<p>已签收</p>";
    //                break;
    //            case 4:
    //                cout<<"<p>已退回</p>";
    //                break;
    //            case 5:
    //                cout<<"<p>正在派件</p>";
    //                break;
    //            case 6:
    //                cout<<"<p>正在退回</p>";
    //                break;
    //        };
    //        const Json::Value arrayObj=value["data"];
    //        string time,context;
    //        for(int i=0;i<arrayObj.size();++i)
    //        {
    //            time=arrayObj[i]["time"].asString();
    //            context=arrayObj[i]["context"].asString();
    //            cout<<"<p>"<<time<<":"<<context<<"</p>"<<endl;
    //        }
    //        cout<<"</html>"<<endl;
    //    }
    //}
    return 0;
}
