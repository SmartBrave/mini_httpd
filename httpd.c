/* Smart's webserver */
/* This is a simple webserver.
 * Created November 2016 by SmartBrave Blackstone.
 */
#include<sys/socket.h>
#include<stdio.h>
#include<regex.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<pthread.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

/*********************************************
 * print msg to log file:[time]function:msg
 * *****************************************/
#define PRINT_IN_LOG(msg) fprintf(log,"file:%s,line:%4d,[%s:%s]:%s\n",__FILE__,__LINE__,__DATE__,__TIME__,msg),fflush(log);
#define ISSPACE(ch) (ch==' '?1:0)//   () !!!
#define SERVER_STRING "Server: http/0.1.0\r\n"
#define _COMM_SIZE_ 1024

int startup(const char* ip,short *port,FILE* log);
void* accept_request(void* sock);
int regex(const char *string,const char* pattern);
void clear_header(int sock);
void exe_cgi(int sock,const char *path,const char*method,const char*query_string);
void echo_html(int sock,const char* path,ssize_t size);
void cannot_execute(int sock);
void echo_error_to_client(int sock,int error_code);
int get_line(int fd,char* buf,int max_len);
void* accept_request(void* arg);
void bad_request(int sock);
void not_found(int client);
void cannot_execute(int sock);
void unimplemented(int sock);

/*****************************************************
 * main exit status:
 * 0:
 * 1: IP is error!
 * 2: port is error!
 * 3: create socket field!
 * 4: bind field!
 * 5: create port field!
 * 6: listen field!
 * 7: create connection field!
 * 8: create thread field!
 * 9:
 * 10:
 * 11:
 * 12:
 * 13:
 * 14:
 * 15:
 * 16:
 * 17:
 * **************************************************/


int main(int argc,char* argv[])
{
    FILE* log=fopen("./log/httpd_log.txt","a+");
    fprintf(log,"\n");
    const char* ip=NULL;
    unsigned short port=0;
    int server_sock=0,client_sock=0;
    int client_sock_len=sizeof(client_sock);
    int ret=0;
    pthread_t tid;

    switch(argc)
    {
        case 1://IP and port are not given by caller
            ip="INADDR_ANY";
            break;
        case 3://IP and port are given by caller
               //IP is seven in case 2 because have no break
            ret=regex(argv[2]/*port*/,"[1-9]{1}[0-9]{1,}");
            PRINT_IN_LOG("regex(port)");
            if(ret==0)
            {
                port=atoi(argv[2]);
            }
            else
            {
                fprintf(stderr,"IP is error\n");
                exit(2);
            }
        case 2://IP is given,but port is not given
            ret=regex(argv[1]/*ip*/,"([1-9]{1}[0-9]{0,2}){1}(\\.[0-9]{1,3}){3}");
            PRINT_IN_LOG("regex(IP)");
            if(ret==0)
            {
                ip=argv[1];
                //inet_aton(argv[1],&ipn);
            }
            else
            {
                fprintf(stderr,"IP is error\n");
                exit(1);
            }
            break;
        default:
            ;
            break;
    }
    ip="192.168.1.2";
    port=8080;
    server_sock=startup(ip,&port,log);
    PRINT_IN_LOG("startup()");
    printf("httpd running on %s:%d\n",ip,port);
    while(1)
    {
        client_sock=accept(server_sock,(struct sockaddr*)&client_sock,&client_sock_len);
        //PRINT_IN_LOG("accept()");
        if(client_sock==-1)
        {
            perror("accept");
            exit(7);
        }
        ret=pthread_create(&tid,NULL,accept_request,(void*)client_sock);
        //PRINT_IN_LOG("pthread_create()");
        if(ret!=0)
        {
            perror("pthread_create()");
            exit(8);
        }
    }
    close(server_sock);//close server
    fclose(log);
    return 0;
}



int startup(const char* ipc,short *port,FILE* log)
{
    if(ipc==NULL||port==NULL)
        return -1;
    struct sockaddr_in local;
    int ret=0;
    int flag=1;
    int local_len=sizeof(local);
    int sock=socket(AF_INET,SOCK_STREAM,0);
    setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    PRINT_IN_LOG("socket(AF_INET,SOCK_STREAM,0)");
    if(sock==-1)
    {
        perror("socket()");
        exit(3);
    }
    memset(&local,0,local_len);
    local.sin_family=AF_INET;
    local.sin_port=htons(*port);
    if(strcasecmp(ipc,"INADDR_ANY")==0)
    {
        local.sin_addr.s_addr=htonl(INADDR_ANY);
    }
    else
    {
        local.sin_addr.s_addr=inet_addr(ipc);
    }
    if(ret=bind(sock,(struct sockaddr*)&local,local_len),ret!=0)
    {
        perror("bind()");
        exit(4);
    }
    if(*port==0)
    {
        if(getsockname(sock,(struct sockaddr*)&local,&local_len)==-1)
        {
            perror("getsockname()");
            exit(5);
        }
        *port=ntohs(local.sin_port);
    }
    if(listen(sock,1024)<0)
    {
        perror("listen()");
        exit(6);
    }
    return sock;
}


/**********************************************
*  match a regular expression
*  return 0 if match success,else return -1
**********************************************/
int regex(const char *string,const char* pattern)
{
    regex_t reg;
    int flags=REG_EXTENDED;
    regmatch_t pmatch[1];
    const size_t nmatch=1;
    if(regcomp(&reg,pattern,flags)!=0)
    {
        perror("regcomp");
        exit(9);
    }
    if(regexec(&reg,string,nmatch,pmatch,0)==REG_NOMATCH)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/*********************************************************
 * get a line from fd,and save the data in the buffer
 * pointer by buf
 * return val:return the bytes of real read
 *            on error,return -1;
 * *****************************************************/
int get_line(int fd,char* buf,int max_len)
{
    if(buf==NULL||max_len<0)
        return -1;
    int i=0,n=0;
    char c=0;
    while(i<max_len-1&&c!='\n')
    {
        n=recv(fd,&c,1,0);
        if(n>0)
        {
            if(c=='\r')
            {
                n=recv(fd,&c,1,MSG_PEEK);
                if(n>0&&c=='\n')
                {
                    recv(fd,&c,1,0);
                }
                else
                {
                    c='\n';
                }
            }
            buf[i++]=c;
        }
        else
        {
            c='\n';
        }
    }
    buf[i]='\0';
    return i;
}

void* accept_request(void* arg)
{
    pthread_detach(pthread_self());
    int sock=(int)arg;
    int cgi=0;
    char method[_COMM_SIZE_];
    char url[_COMM_SIZE_];
    char path[_COMM_SIZE_];
    char version_http[_COMM_SIZE_];
    char buf[_COMM_SIZE_];
    char* query_string=NULL;
    memset(method,'\0',sizeof(method));
    memset(url,'\0',sizeof(url));
    memset(path,'\0',sizeof(path));
    memset(version_http,'\0',sizeof(version_http));
    memset(buf,'\0',sizeof(buf));
    int i=0,j=0;
    int ret=0;

    ret=get_line(sock,buf,sizeof(buf));
    if(ret==-1)
    {
        perror("get_line");
        exit(1);
    }
    while(j<sizeof(buf) && i<sizeof(method)-1 && !ISSPACE(buf[j]))
    {
        method[i++]=buf[j++];
    }
    method[i]='\0';
    while(j<sizeof(buf) && ISSPACE(buf[j]) && j++);
    i=0;
    while(j<sizeof(buf) && i<sizeof(url)-1 && !ISSPACE(buf[j]))
    {
        url[i++]=buf[j++];
    }
    while(j<sizeof(buf) && ISSPACE(buf[j]) && j++);
    i=0;
    while(i<sizeof(buf) && i<sizeof(url)-1 && version_http[i-1]!='\n')
    {
        version_http[i++]=buf[j++];
    }
    if(version_http[strlen(version_http)-1]=='\n')
        version_http[strlen(version_http)-1]='\0';
    i=0;
    while(i<strlen(url) && url[i]!='?')
        i++;
    if(url[i]=='?')
    {
        cgi=1;
        url[i]='\0';
        query_string=url+i+1;
    }
    if(url[strlen(url)-1]=='/')
    {
        sprintf(path,"htdocs%s",url);
    }
    else
        sprintf(path,"htdocs%s",url);
#ifdef _DEBUG_
    printf("method: %s\nurl: %s\nquery_string: %s\nversion_http: %s\npath: %s\n\n",method,url,query_string,version_http,path);
#endif
    if(strcasecmp(method,"get")!=0&&strcasecmp(method,"post")!=0)
    {
        echo_error_to_client(sock,400);
        return (void*)-1;
    }
    if(strcasecmp(method,"get")==0)//method is get
    {
        if(query_string!=NULL)
        {
            cgi=1;
        }
    }
    else//method is post
    {
        cgi=1;
    }
    struct stat st;
    if(stat(path,&st)<0)//file is not exist
    {
        clear_header(sock);
        echo_error_to_client(sock,404);
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,"/");
            strcat(path,"index.html");
        }
        else if(st.st_mode & S_IXUSR || st.st_mode & S_IXGRP || st.st_mode & S_IXOTH)
        {
            cgi=1;
        }
        else
        {
        }
        if(cgi)
        {
            exe_cgi(sock,path,method,query_string);
        }
        else
        {
            clear_header(sock);
            echo_html(sock,path,st.st_size);
        }
    }
    close(sock);
    return NULL;
}
void clear_header(int sock)
{
    char buf[_COMM_SIZE_];
    int ret=0;
    memset(buf,'\0',sizeof(buf));
    do
    {
        ret=get_line(sock,buf,sizeof(buf));
    }while(ret>0&&buf[0]!='\n');
    //while(ret>0&&strcmp(buf,'\n')!=0);
}
void exe_cgi(int sock,const char *path,const char*method,const char*query_string)
{
    char buf[_COMM_SIZE_];
    int ret=1;
    int content_length=0;
    int cgi_input[2],cgi_output[2];
    int id;
    int i;

    if(strcasecmp(method,"get")==0)
    {
        clear_header(sock);
    }
    else if(strcasecmp(method,"post")==0)
    {
        while(ret>0&&strncmp(buf,"Content-Length:",strlen("Content-Length:"))!=0)
        {
            ret=get_line(sock,buf,sizeof(buf));
        }
        clear_header(sock);
        if(ret<=0)
        {
            bad_request(sock);
            return;
        }
        content_length=atoi(buf);
    }
    memset(buf,'\0',sizeof(buf));
    sprintf(buf,"HTTP/1.0 200 OK\r\n\r\n");
    send(sock,buf,strlen(buf),0);
    //ret=pipe(cgi_input);
    //if(ret!=0)
    //{
    //    cannot_execute(sock);
    //}
    //ret=pipe(cgi_output);
    //if(ret!=0)
    //{
    //    cannot_execute(sock);
    //}
    id=fork();
    if(id<0)
    {
        cannot_execute(sock);
    }
    if(id==0)//child
    {
        char method_env[_COMM_SIZE_];
        char query_string_env[_COMM_SIZE_];
        char length_env[_COMM_SIZE_];
        //close(cgi_input[1]);
        //close(cgi_output[0]);
        //dup2(cgi_input[0],0);
        //dup2(cgi_output[1],1);
        dup2(sock,0);
        dup2(sock,1);
        memset(method_env,'\0',sizeof(method_env));
        sprintf(method_env,"REQUEST_METHOD=%s\n",method);
        putenv(method_env);
        if(strcasecmp(method,"get")==0)
        {
            memset(query_string_env,'\0',sizeof(query_string));
            sprintf(query_string_env,"QUERY_STRING=%s\n",query_string);
            putenv(query_string_env);
        }
        else
        {
            memset(length_env,'\0',sizeof(length_env));
            sprintf(length_env,"CONTENT_LENGTH=%d\n",content_length);
            putenv(length_env);
        }
        execl(path,path,NULL);
        exit(1);
    }
    else//father
    {
        char c;
        //close(cgi_input[0]);
        //close(cgi_output[1]);
        if(strcasecmp("post",method)==0)
        {
            for(i=0;i<content_length;++i)
            { 
                recv(sock,&c,1,0);
                write(cgi_input[1],&c,1);
            }
        }
        //while(read(cgi_output[0],&c,1)>0)
        //{
        //    send(sock,&c,1,0);
        //}
        //send(sock,"\r\n",strlen("\r\n"),0);
        //close(cgi_input[1]);
        //close(cgi_output[0]);
        waitpid(id,NULL,0);
    }
}
void echo_html(int sock,const char* path,ssize_t size)
{
    if(path==NULL)
    {
        return;
    }
    int fd=open(path,O_RDONLY);
    if(fd<0)
    {
        echo_error_to_client(sock,500);
        return;
    }
    char buf[_COMM_SIZE_];
    memset(buf,'\0',sizeof(buf));
    strncpy(buf,"HTTP/1.1",strlen("HTTP/1.1")+1);
    strcat(buf," 200 OK");
    strcat(buf,"\r\n\r\n");
    send(sock,buf,strlen(buf),0);
    //send(sock,"\r\n",strlen("\r\n"),0);
    if(sendfile(sock,fd,NULL,size)<0)
    {
        close(fd);
        return;
    }
    close(fd);
}
/******************************************************ot:*
 * Give a client a 400 bad request status message.
 * *****************************************************/
void bad_request(int sock)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.1 400 BAD REQUEST\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"Content_type: text/html\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"<p>you enter message is a bad request!\r\n</p>");
    send(sock,buf,sizeof(buf),0);
}
/*********************************************************
 * Give a client a 404 not found status message.
 * ******************************************************/
void not_found(int client)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.1 404 NOT FOUND\r\n\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,SERVER_STRING);
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"Content-Type: text/html\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"<html><title>Not Found</title>\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"<body><p>The server could not fulfill\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"you request not found because the resource specified is unavailable or nonexistent.\r\n");
    send(client,buf,sizeof(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(client,buf,sizeof(buf),0);
}
/*********************************************************
 * Give a client a 500 cannot execute status message.
 * ******************************************************/
void cannot_execute(int sock)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.1 500 Internal Server Error\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"Content-type: text/html\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"\r\n");
    send(sock,buf,sizeof(buf),0);
    sprintf(buf,"<p>Error prohibited CGI execution.\r\n</p>");
    send(sock,buf,sizeof(buf),0);
}
/*********************************************************
 * Give a client a 501 uniplemented status message.
 * ******************************************************/
void unimplemented(int sock)
{
    char buf[1024];
    sprintf(buf,"HTTP/1.1 501 Method Not Implemented\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,SERVER_STRING);
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"Content-Type: text/htlm\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"<html><head><title>Method Not Implemented\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"</title></head>\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"<body><p>HTTP request method not supported.\r\n");
    send(sock,buf,strlen(buf),0);
    sprintf(buf,"</body></html>\r\n");
    send(sock,buf,strlen(buf),0);
}
void echo_error_to_client(int sock,int error_code)
{
    switch(error_code)
    {
        case 400://Bad Request
            bad_request(sock);
            break;
        case 404://Not Found
            not_found(sock);
            break;
        case 500://Cannot Execute
            cannot_execute(sock);
            break;
        case 501://Unimplemented
            unimplemented(sock);
            break;
        default:
            break;
    }
}
