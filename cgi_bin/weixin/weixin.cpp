#include<iostream>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<openssl/sha.h>
#include<json/json.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
using namespace std;


int main()
{
    char* appid="wx8be4cb337e80be6a";
    char* appsecert="87ac10a1a62798a84df37ff0a13f162f";
    char buf[10240],url[1024];
    memset(buf,'\0',sizeof(buf));
    const char* ip="117.135.172.156";
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
    sprintf(url,"GET https://api.weixin.qq.com/cgi-bin/token?grant_type=client_credential&appid=%s&secret=%s\r\n\r\n",appid,appsecert);
    send(sock,url,strlen(url),0);
    size_t size=0;
    size_t capacity=sizeof(buf);
    while(size=read(sock,buf+size,capacity-size)>0)
    {
        capacity-=size;
    }
    Json::Reader reader;
    Json::Value value;
    string access_token,expires_in;
    if(reader.parse(buf,value))
    {
        access_token=value["access_token"].asString();
        expires_in=value["expires_in"].asString();
    }
    cerr<<access_token<<endl<<expires_in<<endl;
    return 0;
}











//int main()
//{
//    //验证token
//    //char* token="Smart";
//    //char buf[2048];
//    //char* pe;
//    //int i;
//    //char signature[1024],echostr[1024],timestamp[1024],nonce[1024];
//    //memset(buf,'\0',sizeof(buf));
//    //memset(signature,'\0',sizeof(signature));
//    //memset(echostr,'\0',sizeof(echostr));
//    //memset(timestamp,'\0',sizeof(timestamp));
//    //memset(nonce,'\0',sizeof(nonce));
//    //sprintf(buf,"%s",getenv("QUERY_STRING"));
//    //cerr<<buf<<endl;
//    //i=0;
//    //pe=strstr(buf,"signature");
//    //pe+=(strlen("signature")+1);
//    //while(*pe!='\0'&&*pe!='&')
//    //{
//    //    signature[i++]=*pe;
//    //    pe++;
//    //}
//    //signature[i++]='\0';
//    //i=0;
//    //pe=strstr(buf,"echostr");
//    //pe+=(strlen("echostr")+1);
//    //while(*pe!='\0'&&*pe!='&')
//    //{
//    //    echostr[i++]=*pe;
//    //    pe++;
//    //}
//    //echostr[i++]='\0';
//    //i=0;
//    //pe=strstr(buf,"timestamp");
//    //pe+=(strlen("timestamp")+1);
//    //while(*pe!='\0'&&*pe!='&')
//    //{
//    //    timestamp[i++]=*pe;
//    //    pe++;
//    //}
//    //timestamp[i++]='\0';
//    //i=0;
//    //pe=strstr(buf,"nonce");
//    //pe+=(strlen("nonce")+1);
//    //while(*pe!='\0'&&*pe!='&')
//    //{
//    //    nonce[i++]=*pe;
//    //    pe++;
//    //}
//    //nonce[i++]='\0';
//
//    char* token="Smart";
//    char buf[2048];
//    char* pe;
//    int i;
//    char signature[1024],echostr[1024],timestamp[1024],nonce[1024];
//    memset(buf,'\0',sizeof(buf));
//    memset(signature,'\0',sizeof(signature));
//    memset(echostr,'\0',sizeof(echostr));
//    memset(timestamp,'\0',sizeof(timestamp));
//    memset(nonce,'\0',sizeof(nonce));
//    sprintf(buf,"%s",getenv("QUERY_STRING"));
//    cerr<<buf<<endl;
//    i=0;
//    pe=strstr(buf,"signature");
//    pe+=(strlen("signature")+1);
//    while(*pe!='\0'&&*pe!='&')
//    {
//        signature[i++]=*pe;
//        pe++;
//    }
//    signature[i++]='\0';
//    i=0;
//    pe=strstr(buf,"echostr");
//    pe+=(strlen("echostr")+1);
//    while(*pe!='\0'&&*pe!='&')
//    {
//        echostr[i++]=*pe;
//        pe++;
//    }
//    echostr[i++]='\0';
//    i=0;
//    pe=strstr(buf,"timestamp");
//    pe+=(strlen("timestamp")+1);
//    while(*pe!='\0'&&*pe!='&')
//    {
//        timestamp[i++]=*pe;
//        pe++;
//    }
//    timestamp[i++]='\0';
//    i=0;
//    pe=strstr(buf,"nonce");
//    pe+=(strlen("nonce")+1);
//    while(*pe!='\0'&&*pe!='&')
//    {
//        nonce[i++]=*pe;
//        pe++;
//    }
//    nonce[i++]='\0';
//    //cerr<<"BBBBBBBBBBBBBBBBBBBBBBBBBB\n"<<endl;
//    //char* first,*second,*third;
//    //if(strcmp(token,timestamp)<0)
//    //{
//    //    first=token;
//    //    second=timestamp;
//    //}
//    //else
//    //{
//    //    first=timestamp;
//    //    second=token;
//    //}
//    //if(strcmp(first,nonce)<0)
//    //{
//    //    if(strcmp(second,nonce)<0)
//    //    {
//    //        third=nonce;
//    //    }
//    //    else
//    //    {
//    //        third=second;
//    //        second=nonce;
//    //    }
//    //}
//    //else
//    //{
//    //    third=second;
//    //    second=first;
//    //    first=nonce;
//    //}
//    //sprintf(buf,"%s%s%s",first,second,third);
//    //SHA_CTX s;
//    //SHA1_Init(&s);
//    //SHA1_Update(&s,buf,strlen(buf));
//    //printf("HTTP/1.1 200 OK\r\n\r\n");
//    //cout<<echostr<<endl;
//    //cerr<<echostr<<endl;
//    //printf("HTTP/1.1 200 OK\r\n\r\n");
//    //cout<<echostr<<endl;
//    return 0;
//}
