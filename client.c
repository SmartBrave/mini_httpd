#include<stdio.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>

#define _COMM_SIZE_ 1024

void usage(const char* msg)
{
    printf("usage : %s [ip] [port]\n",msg);
}

int main(int argc,char* argv[],char* env[])
{
    if(argc!=3)
    {
        usage(argv[0]);
        exit(1);
   }
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(argv[2]));
    inet_aton(argv[1],&local.sin_addr);
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
#ifdef _DEBUG_
    printf("connect success!\n");
#endif
    char buf[_COMM_SIZE_];
    memset(buf,'\0',sizeof(buf));
    ssize_t sz=0;
    gets(buf);
    int len=strlen(buf);
    buf[len]='\n';
    sz=send(sock,buf,len+1,0);
    buf[0]='\n';
    send(sock,buf,strlen(buf),0);
    //fflush(sock);
    if(sz<=0)
    {
        perror("send");
        close(sock);
        exit(1);
    }
    memset(buf,'\0',sizeof(buf));
    while(recv(sock,buf,sizeof(buf)-1,0)>0)
    {
        printf("%s",buf);
        memset(buf,'\0',sizeof(buf));
    }
    return 0;
}
