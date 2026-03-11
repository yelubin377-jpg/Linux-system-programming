
//fgets
// #include<stdio.h>
// #define MaxLen 4096
// int main()
// {
//     char line[MaxLen];
//     printf("请输入命令：");
//     fgets(line,MaxLen,stdin);
//     printf("你输入的命令是：%s",line);
//     return 0;
// }



//getcwd
// #include <stdio.h>
// #include <unistd.h>
// #define MAXLEN 4096
// int main()
// {
//     char cwd[MAXLEN];
//     if(getcwd(cwd,MAXLEN)!=NULL)
//     {
//         printf("当前工作目录：%s\n",cwd);
//     }
//     else
//     {
//         perror("getcwd");
//     }
//     return 0;
// }


//chdir
// #include<stdio.h>
// #include<unistd.h>
// #define MAXLEN 4096
// int main()
// {
//     char cwd[MAXLEN];
//     if(getcwd(cwd,MAXLEN)!=NULL)
//     {
//         printf("切换前目录:%s\n",cwd);
//     }


//     if(chdir("666")==0)
//     {
//         printf("success");
//     }
//     else
//     {
//         perror("chdir lose");
//         return 1;
//     }
    
//     if(getcwd(cwd,MAXLEN)!=NULL)
//     {
//         printf("\n切换后目录:%s\n",cwd);
//     }
//     return 0;
    
// }






// //fork()
// #include <stdio.h>
// #include <unistd.h>
// #include <sys/types.h>
//  int main()
//  {
//     pid_t pid =fork();
//     if(pid<0)
//     {
//         //错误处理
//         perror("fork失败");
//         return 1;   
//     }
//     else if(pid==0)
//     {
//         printf("子进程PID:%d\n",getpid());
//         printf("子进程返回值:%d\n",pid);//0
//     }
//     return 0;
//  }




//pipe(int pipefd[2])
// #include<stdio.h>
// #include<unistd.h>
// #include<string.h>
// #include <sys/wait.h>
// int main()
// {
//     int pipefd[2];
//     char buf[128];
//     pid_t pid;
    
//     //先创建管道
//     if(pipe(pipefd) == -1)
//     {
//         perror("pipe");
//         return 1;
//     }

//     //创建子进程
//     pid=fork();
//     if(pid<0)
//     {
//         perror("fork");
//         return 1;
//     }


//     if(pid == 0)
//     {
//         //子进程 从管道读取数据
//         close(pipefd[1]);//关闭不用的写端
//         int n = read(pipefd[0],buf,sizeof(buf));
//         if(n>0)
//         {
//             buf[n] = '\0';
//             printf("子进程收到父进程的信息:%s\n",buf);
//         }  
//         close(pipefd[0]);//关闭
//     }
//     else
//     {
//         //父进程：向管道写入数据
//         close(pipefd[0]);
//         const char *msg = "Hello";
//         write(pipefd[1],msg,strlen(msg));
//         close(pipefd[1]);
//         wait(NULL);//等待子进程结束
//     }
//     return 0;
    

// }
//注意这里会“进程分裂”，fork（）后分裂成两个进程，一个去if（父进程），一个去else（子进程）。






// //dup2(旧文件，新文件）  dup2后新文件指向旧文件
// #include<stdio.h>
// #include<unistd.h>  //dup2,close
// #include<fcntl.h>  //open
// //O_WRONLY 只写
// //O_CREAT 如果文件不存在，就创建
// //O_TRUNC 如果文件一存在，就把他的长度截断为0（清空内容）
// //0644 文件权限，文件所有者可读可写，其他人只读
// int main()
// {
//     int File = open("output.txt",O_WRONLY|O_CREAT|O_TRUNC,0644);
//     if(File < 0)
//     {
//         perror("open");
//         return 1;
//     }

//     int CunChu = dup(STDOUT_FILENO);
//     if(dup2(File,STDOUT_FILENO) == -1 )
//     {
//         perror("dup2");
//         close(File);
//         return 1;
//     }
//     close(File);
//     //此时输出端STDOUT被定位到了output.txt内
//     printf("Try it");//不会在终端显示
    
//     //现在开始恢复在终端输出
//     fflush(stdout);//先清理终端缓冲区
//     if(dup2(CunChu,STDOUT_FILENO)==-1)
//     {
//         perror("dup2");
//         close(CunChu);
//         return 1;
//     }
//     close(CunChu);
//     printf("Try again");//理论上此时在终端输出
//     return 0;

// }





//signal(信号类型，执行函数)
#include<stdio.h>//printf
#include<signal.h>//signal();
#include<stdlib.h> //exit

void CeShi(int sig)
{
    printf("收到");
    exit(0);
}
int main()
{
    signal(SIGTERM,CeShi);
    signal(SIGINT,CeShi);
    printf("程序运行中，请用合适信号退出");
    while(1);
    return 0;
}


