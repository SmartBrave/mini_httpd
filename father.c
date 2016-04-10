#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>

int main()
{
    pid_t id;
    id=fork();
    if(id==0)
    {
        execl("/home/Smart/code/C/myhttpd/output/httpd",NULL);
        printf("exec is failed!\n");
        exit(2);
    }
    else if(id>0)
    {
        int status;
        if(waitpid(id,&status,0)==id)
        {
            printf("status:%d\n",status);
            printf("WIFEXITED(status):%d\n",WIFEXITED(status));
            printf("WEXITSTATUS(status):%d\n",WEXITSTATUS(status));
            printf("WIFSIGNALED(status):%d\n",WIFSIGNALED(status));
            printf("WTERMSIG(status):%d\n",WTERMSIG(status));
#ifdef WCOREDUMP
            printf("WCOREDUMP(status):%d\n",WCOREDUMP(status));
#endif
            printf("WSTOPSIG(status):%d\n",WSTOPSIG(status));
            printf("WIFCONTINUED(status):%d\n",WIFCONTINUED(status));
        }
        else
        {
            printf("waitpid is failed!\n");
        }
    }
    else
    {
        printf("fork is failed!\n");
    }
    return 0;
}
