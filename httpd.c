#include<stdio.h>
#include<assert.h>
#include<string.h>
#include<errno.h>
#include<stdlib.h>
#include<regex.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<sys/epoll.h>
#include<pthread.h>
#include<sys/stat.h>
#include<unistd.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/wait.h>
#include<sys/sendfile.h>
#include<arpa/inet.h>

#define CONFIG_SIZE 128
#define STRING_SIZE 1024
#define FILE_SIZE 5242880
const char* const ConfigName="./conf/config";
const char* const LogName="./log/log";
FILE* log_file=NULL;
#ifdef LOG
char log_buf[1024];
#endif

void clear_header(int sock);
static const char* get_config(const char* head,char* buf);
static int start_up(const char* ip,const char* port);
static int  accept_request(int sock);
static int get_line(int sock,char* buf,int size);
void echo_error_to_client(int sock,const char* error);
void echo_html(int sock,const char* path,int size);
void exe_cgi(int sock,const char* method,const char* path,const char* query_string);
void exe_php(const char* code,char* outresult);
int setnonblocking(int fd);
inline static int write_log(const char* func,int line, const char* ptr);

void handler(int sig)
{
#ifdef DEBUG
    printf("AAAAAAAAAAAAAAAAAAAAAAAAA____%d___AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\n",sig);
#endif
    signal(SIGPIPE,handler);
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

int main()
{
#ifdef LOG
    log_file=fopen(LogName,"a");
    if(log_file==NULL)
    {
        perror("open log file fail!");
        exit(1);
    }
    sprintf(log_buf,"\nThis server is started![DATE: %s,TIME: %s]",__DATE__,__TIME__);
    fwrite(log_buf,1,strlen(log_buf),log_file);
#endif
    signal(SIGPIPE,handler);
    //signal(SIGPIPE,SIG_IGN);
    char ip[STRING_SIZE];
    char port[STRING_SIZE];
    int ret;
    int i,n;
    pthread_t tid;
    int new_sock;
    struct sockaddr_in client_sock;
    socklen_t client_sock_len=sizeof(client_sock);

    get_config("IP",ip);
    get_config("port",port);
#ifdef DEBUG
    //printf("ip is %s\n",ip);
    //printf("port is %d\n",atoi(port));
#endif
    ret=regex(ip,"([1-9]{1}[0-9]{0,2}){1}(\\.[0-9]{1,3}){3}");
    if(ret!=0)
    {
        printf("IP is error\n");
        exit(1);
    }
    ret=regex(port,"[1-9]{1}[0-9]{0,4}");
    if(ret!=0)
    {
        printf("port is error!\n");
        exit(1);
    }
    int listen_sock=start_up(ip,port);
    if(listen_sock==-1)
    {
        perror("start_up");
        exit(1);
    }
#ifdef DEBUG
    printf("httpd running on %s:%s\n",ip,port);
#endif
#ifdef LOG
    sprintf(log_buf,"httpd running on %s:%s[DATE: %s,TIME: %s]\n",ip,port,__DATE__,__TIME__);
    fwrite(log_buf,1,strlen(log_buf),log_file);
#endif
#ifdef MULTITHREAD
#ifdef DEBUG
    printf("httpd running on block mode!\n");
#endif
    while(1)
    {
        new_sock=accept(listen_sock,(struct sockaddr*)&client_sock,&client_sock_len);
        if(new_sock<0)
        {
            perror("accept");
            exit(1);
        }
        tid=pthread_create(&tid,NULL,(void*(*)(void*))accept_request,(void*)&new_sock);
        if(tid<0)
        {
            perror("pthread_create");
            close(new_sock);
            exit(1);
        }
        //可以创建一个新的线程来detach,join并close sock;
        pthread_detach(tid);
    }
#endif //MULTITHREA
#ifdef EPOLL
#ifdef DEBUG
    printf("httpd running on epoll mode!\n");
#endif
    int efd;
    struct epoll_event event;
    struct epoll_event* events;
    char max_epoll_size[STRING_SIZE];
    get_config("max_epoll_size",max_epoll_size);
    efd=epoll_create(atoi(max_epoll_size));
    if(efd<0)
    {
        perror("epoll_create");
        exit(1);
    }
    event.data.fd=listen_sock;
    event.events=EPOLLIN | EPOLLET;
    ret=epoll_ctl(efd,EPOLL_CTL_ADD,listen_sock,&event);
    if(ret<0)
    {
        perror("epoll_ctl");
        exit(1);
    }
    setnonblocking(efd);
    events=(struct epoll_event*)calloc(atoi(max_epoll_size),sizeof(event));
    while(1)
    {
again:
        n=epoll_wait(efd,events,atoi(max_epoll_size),-1);
        if(n<0)
        {
            if(errno==EINTR)
                goto again;
            perror("epoll_wait");
            exit(1);
        }
        for(i=0;i<n;++i)
        {
            if((events[i].events & EPOLLERR) ||
               (events[i].events & EPOLLHUP) /*||
               !(events[i].events & EPOLLIN)*/)
            {
                perror("epoll_wait");
                close(events[i].data.fd);
                continue;
            }
            else if(listen_sock==events[i].data.fd)
            {
                new_sock=accept(listen_sock,(struct sockaddr*)&client_sock,&client_sock_len);
                if(new_sock<0)
                {
                    perror("accept");
                    exit(1);
                }
                setnonblocking(new_sock);
                event.data.fd=new_sock;
                event.events=EPOLLIN | EPOLLET;
                ret=epoll_ctl(efd,EPOLL_CTL_ADD,new_sock,&event);
                if(ret<0)
                {
                    perror("epoll_ctl");
                    exit(1);
                }
            }
            else if(events[i].events&EPOLLIN)
            {
                new_sock=events[i].data.fd;
                ret=epoll_ctl(efd,EPOLL_CTL_DEL,new_sock,&event);
                if(ret<0)
                {
                    perror("epoll_ctl");
                    exit(1);
                }
                if(ret=accept_request(events[i].data.fd)<0)
                {
#ifdef DEBUG
                    printf("accept_request errno: %d\n",ret);
#endif
                    perror("accept_request");
                }
                close(events[i].data.fd);
                //event.data.fd=new_sock;
                //event.events=EPOLLIN | EPOLLET;
                //close(new_sock);
            }
            else if(events[i].events&EPOLLOUT)
            {
            }
            else
            {
            }
        }
    }
    free(events);
#endif
#ifdef LOG
    fclose(log_file);
#endif
    return 0;
}

static int 
accept_request(int sock)
{
    char *method=NULL,*url=NULL,*query_string=NULL,*http_version=NULL;
    char buf[STRING_SIZE];
    char tmp[STRING_SIZE];
    char path[STRING_SIZE];
    int cgi=0;
    memset(buf,'\0',sizeof(buf));
    memset(path,'\0',sizeof(path));
    int size;
    size=get_line(sock,buf,sizeof(buf));
    if(size<1)
    {
        printf("size: %d,buf: %p,sizeof(buf):%d\n",size,buf,sizeof(buf));
        echo_error_to_client(sock,"400");
        perror("get_line");
        return -1;
    }
    //while(1)
    //{
    //    size=get_line(sock,tmp,sizeof(tmp));
    //    if(size<1)
    //    {
    //        echo_error_to_client(sock,"400");
    //        close(sock);
    //        return -2;
    //    }
    //    tmp[size]='\0';
    //    if(size==1&&tmp[size-1]=='\n')
    //    {
    //        break;
    //    }
    //    else
    //    {
    //        strcat(buf," ");
    //        strcat(buf,tmp);
    //    }
    //}
    
    //sscanf(buf,"%s %s %s",method,url,http_version);
    
    char* p=buf;
    while(*p!='\0'&&*p++==' ');
    method=p-1;
    while(*p!='\0'&&*p++!=' ');
    *(p-1)='\0';
#ifdef DEBUG
    printf("method: %s\n",method);
#endif

    while(*p!='\0'&&*p++==' ');
    url=p-1;
    while(*p!='\0'&&*p++!=' ');
    *(p-1)='\0';
#ifdef DEBUG
    printf("url: %s\n",url);
#endif

    while(*p!='\0'&&*p++==' ');
    http_version=p-1;
    while(*p!='\0'&&*p++!=' ');
    *(p-1)='\0';
#ifdef DEBUG
    printf("http_version: %s\n",http_version);
#endif
    //identify the correct path
    sprintf(path,"htdocs");
    int i=0,j=strlen("htdocs");
    while(url[i]!='\0'&&url[i]!='?')
    {
        path[j]=url[i];
        i++;
        j++;
    }
    path[j]='\0';
#ifdef DEBUG
    printf("path: %s\n",path);
#endif
    if(url[i]=='?')
    {
        cgi=1;
        url[i]='\0';
        query_string=url+i+1;
#ifdef DEBUG
    printf("query_string: %s\n",query_string);
#endif
    }
#ifdef DEBUG
    printf("\n");
#endif
    struct stat st;
    if(stat(path,&st)<0)
    {
        clear_header(sock);
        echo_error_to_client(sock,"404");
        return 1;
    }
    else
    {
        if(S_ISDIR(st.st_mode))
        {
            strcat(path,"index.html");
        }
        else if((st.st_mode & S_IXUSR)||
                (st.st_mode & S_IXGRP)||
                (st.st_mode & S_IXOTH))
        {
            cgi=1;
        }
        else
        {
            clear_header(sock);
            if(strstr(path,".php")!=NULL)
            {
                FILE* fp=fopen(path,"r");
                char *PhpCode=(char*)malloc(sizeof(char)*FILE_SIZE);//5M
                char *Result=(char*)malloc(sizeof(char)*FILE_SIZE);//5M
                int i;
                fread(PhpCode,1,FILE_SIZE,fp);
                fclose(fp);
                exe_php(PhpCode,Result);
                printf("phpcode:%s\n",PhpCode);
                printf("result:%s\n",Result);
                sprintf(Result,"200 OK\r\n\r\n<html>%s</html>",Result);
                send(sock,Result,strlen(Result),0);
                free(PhpCode);
                free(Result);
                return 0;
            }
        }
    }
    if(stat(path,&st)<0)
    {
        clear_header(sock);
        echo_error_to_client(sock,"404");
        return 1;
    }
    if(strcasecmp(method,"GET")==0)
    {
        if(cgi==1)
        {
            exe_cgi(sock,method,path,query_string);
        }
        else
        {
            clear_header(sock);
            echo_html(sock,path,st.st_size);
        }
    }
    else if(strcasecmp(method,"POST")==0)
    {
        exe_cgi(sock,method,path,query_string);
    }
    else if(strcasecmp(method,"HEAD")==0)
    {
        echo_error_to_client(sock,"200");
    }
    //else if(strcasecmp(http_version,"http/1.1") && strcasecmp(method,"PUT")==0)
    //{
    //    cig=1;
    //}
    //else if(strcasecmp(http_version,"http/1.1") && strcasecmp(method,"DELETE")==0)
    //{
    //}
    //else if(strcasecmp(http_version,"http/1.1") && strcasecmp(method,"TRACE")==0)
    //{
    //}
    //else if(strcasecmp(http_version,"http/1.1") && strcasecmp(method,"CONNECT")==0)
    //{
    //}
    //else if(strcasecmp(http_version,"http/1.1") && strcasecmp(method,"OPTIONS")==0)
    //{
    //}
    else
    {
        echo_error_to_client(sock,"501");
    }
    //close(sock);
    return 0;
}

void 
exe_cgi(int sock,const char* method,const char* path,const char* query_string)
{
    //int tochild[2];
    //int tofather[2];
    //int ret=pipe(tochild);
    //if(ret<0)
    //{
    //    perror("pipe_tochlid");
    //    exit(1);
    //}
    //ret=pipe(tofather);
    //if(ret<0)
    //{
    //    perror("pipe_tofather");
    //    exit(1);
    //}
    char buf[STRING_SIZE];
    int numchars=0;
    int content_length=0;
    if(strcasecmp(method,"get")==0)
    {
        clear_header(sock);
    }
    else if(strcasecmp(method,"post"))
    {
        do
        {
            memset(buf,'\0',sizeof(buf));
            numchars=get_line(sock,buf,sizeof(buf));
            if(strncasecmp(buf,"Content-Length:",strlen("Content-Length:"))==0)
            {
                content_length=atoi(&buf[strlen("Content-Length:")+1]);
            }
        }
        while(numchars>0&& strcmp(buf,"\n"));
        if(content_length==-1)
        {
            return ;
        }
    }
    pid_t id=fork();
    if(id==0)//child
    {
        //close(tochild[1]);
        //close(tofather[0]);

        //dup2(0,tochild[0]);
        //dup2(1,tofather[1]);
        dup2(sock,0);
        dup2(sock,1);
        //为什么putenv函数需要将method和query_string放到不同的buf里才可以成功put到env里面呢？否则后面put的会覆盖前面的？
        //char buf_method[STRING_SIZE];
        if(strcasecmp(method,"get"))
        {
            char buf_query_string[STRING_SIZE];
            sprintf(buf_query_string,"QUERY_STRING=%s\n",query_string);
            putenv(buf_query_string);
        }
        else if(strcasecmp(method,"post"))
        {
            char buf_query_string[STRING_SIZE];
            sprintf(buf_query_string,"QUERY_STRING=%s\n",query_string);
            putenv(buf_query_string);
            char buf_content_length[STRING_SIZE];
            sprintf(buf_content_length,"CONTENT_LENGTH=%s\n",content_length);
            putenv(buf_content_length);
        }
        //sprintf(buf_method,"METHOD=%s\n",method);
        //putenv(buf_method);
        execl(path,path,NULL);
        exit(1);
    }
    else if(id>0)//father
    {
        //close(tochild[0]);
        //close(tofather[1]);
        //char buf[STRING_SIZE];
        //while(1)
        //{
        //    ret=recv(tofather[0],buf,sizeof(buf)-1,0);
        //    if(ret>0)
        //    {
        //        buf[ret]='\0';
        //        send(sock,buf,sizeof(buf)-1,0);
        //    }
        //    else
        //    {
        //        break;
        //    }
        //}
        //可以让一个线程来wait子进程
        waitpid(id,NULL,0);
        //close(tochild[1]);
        //close(tofather[0]);
    }
    else
    {
        perror("fork");
        exit(1);
    }
}

void 
echo_html(int sock,const char* path,int size)
{
    if(path==NULL||size<0)
    {
        echo_error_to_client(sock,"500");
        return;
    }
    char buf[STRING_SIZE];
    int fd=open(path,O_RDONLY);
    if(fd<0)
    {
        write_log(__FUNCTION__,__LINE__,path);
        write_log(__FUNCTION__,__LINE__,"open path fail!");
        printf("AAAAAAAAAAAAAAAAAAAA\n");
        echo_error_to_client(sock,"500");
        perror("open fd");
        close(fd);
        return;
    }
    echo_error_to_client(sock,"200");
    if(sendfile(sock,fd,NULL,size)<0)
    {
        write_log(__FUNCTION__,__LINE__,"sendfile is fail!");
        perror("sendfile is fail!");
    }
#ifdef DEBUG
    printf("sendfiel chenggong!\n");
#endif
    close(fd);
}

void
clear_header(int sock)
{
    char buf[STRING_SIZE];
    int size;
    do
    {
        size=get_line(sock,buf,sizeof(buf));
#ifdef DEBUG
        //printf("%s",buf);
#endif
    }
    while(size>0 && strcmp(buf,"\n")!=0);
}

void
echo_error_to_client(int sock,const char* error)
{
    if(atoi(error)==404)
    {
#ifdef DEBUG
        printf("4040404040404040040\n");
#endif
        ///////////////////////
        //char* path="/home/Smart/code/C/myhttpd/output/htdocs/img/404.jpg";
        char* path="./htdocs/img/404.jpg";
        struct stat st;
        if(stat(path,&st)<0)
        {
            echo_error_to_client(sock,"500");
#ifdef DEBUG
            printf("path: %s\n",path);
#endif
            perror("stat");
            //close(sock);
            return;
        }
        int fd=open(path,O_RDONLY);
        if(fd<0)
        {
            echo_error_to_client(sock,"500");
            perror("open fd");
            close(fd);
            return;
        }
        echo_error_to_client(sock,"200");
        if(sendfile(sock,fd,NULL,st.st_size)<0)
        {
            perror("sendfile");
        }
        //pid_t id=fork();
        //if(id==0)
        //{
        //    dup2(sock,0);
        //    dup2(sock,1);
        //    execl("htdocs/cgi_bin/echo_404","htdocs/cgi_bin/echo_404");
        //    return;
        //}
        //else if(id==1)
        //{
        //    waitpid(id,NULL,0);
        //    return;
        //}
        //else
        //{
        //    perror("fork");
#ifdef DEBUG
        //    printf("forkforkforkforkforkforkforkforkforkfork\n");
#endif
        //    return;
        //}
    }
    else
    {
        char buf[STRING_SIZE];
        char stat[STRING_SIZE];
        char version[STRING_SIZE];
        memset(buf,'\0',sizeof(buf));
        memset(stat,'\0',sizeof(stat));
        memset(version,'\0',sizeof(version));
        get_config("HTTP_VERSION",version);
        get_config(error,stat);
        sprintf(buf,"%s %s %s\r\n\r\n",version,error,stat);
        send(sock,buf,strlen(buf),0);
    }
}

static int 
get_line(int sock,char* buf,int size)
{
    if(buf==NULL||size<=0)
        return -1;
    char ch='\0';
    int n=0;
    int i=0;
    while(i<size-1 && '\n'!=ch)
    {
        n=recv(sock,&ch,1,0);
        if(n>0)
        {
            if('\r'==ch)
            {
                n=recv(sock,&ch,1,MSG_PEEK);
                if(n>0&&'\n'==ch)
                {
                    recv(sock,&ch,1,0);
                }
                else
                {
                    ch='\n';
                }
            }
            buf[i++]=ch;
        }
        else
        {
            printf("i= %d\n",i);
            ch='\n';
        }
    }
    buf[i]='\0';
    return i;
}

static int 
start_up(const char* ip,const char* port)
{
    if(NULL==ip || NULL==port)
        return -1;
    int listen_sock=socket(AF_INET,SOCK_STREAM,0);
    if(listen_sock<0)
    {
        perror("socket");
        exit(1);
    }
    int flag=1;
    setsockopt(listen_sock,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(flag));
    struct sockaddr_in local;
    local.sin_family=AF_INET;
    local.sin_port=htons(atoi(port));
    local.sin_addr.s_addr=inet_addr(ip);
    if(bind(listen_sock,(struct sockaddr*)&local,sizeof(local))<0)
    {
        perror("bind");
        exit(1);
    }
    //if(listen(listen_sock,5)<0)
    if(listen(listen_sock,1)<0)
    {
        perror("listen");
        exit(1);
    }
    return listen_sock;
}
static const char* 
get_config(const char* head,char* buf)
{
    FILE* conf=fopen(ConfigName,"r");
    if(NULL==conf)
    {
        perror("config file");
        exit(1);
    }
    char line[CONFIG_SIZE];
    memset(line,'\0',sizeof(line));
    while(NULL!=fgets(line,sizeof(line),conf))
    {
        size_t length=strlen(line);
        if(line[length-1]=='\n'||line[length-1]=='\0')
            line[length-1]='\0';
        int i;
        for(i=0;i<length;++i)
        {
            if(line[i]==':')
            {
                line[i]='\0';
                if(0==strncasecmp(line,head,strlen(head)))
                {
                    sprintf(buf,"%s",line+i+1);
                    fclose(conf);
                    return (void*)1;
                }
            }
        }
    }
    fclose(conf);
    return NULL;
}
int setnonblocking(int fd)
{
    int old_option=fcntl(fd,F_GETFL);
    int new_option=old_option|O_NONBLOCK;
    fcntl(fd,F_SETFL,new_option);
    return old_option;
}

static int write_log(const char* func,int line,const char* ptr)
{
#ifdef LOG
    sprintf(log_buf,"file: %s,function: %s,line: %d : %s\n",__FILE__,func,line,ptr);
    fwrite(log_buf,1,strlen(log_buf),log_file);
#endif
    return 1;
}

void exe_php(const char* code,char* outresult)
{
    int fd[2];
    pipe(fd);
    pid_t id=fork();
    if(id<0)
    {
        perror("fork");
        return;
    }
    else if(id==0)
    {
        FILE* tmp=fopen("tmp.php","w");
        fwrite(code,1,FILE_SIZE,tmp);
        fclose(tmp);
        dup2(1,fd[0]);
        execl("/usr/bin/php","tmp.php");
        /////////////////
        printf("debugdebugdebugdebug\n");
        exit(1);
    }
    else
    {
        read(fd[1],outresult,FILE_SIZE-1);
        printf("outresult:%s\n",outresult);
        waitpid(id,NULL,0);
    }
}
