#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>


//创建一个子进程并运行
void *func(void *arg)
{
    printf("接收到参数%d\n",*(int*)arg);
    printf("线程执行中\n");
    return NULL;
}
int main()
{
    pthread_t thread;
    int s;
    int arg = 100;
    s = pthread_create(&thread,NULL,func,&arg);
    if( s != 0 )
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    printf("主线程：等待子线程完成创建\n");
    s = pthread_join(thread,NULL);
    if(s!=0)
    {
        perror("pthread_join");
        exit(EXIT_FAILURE);
    }
    printf("主线程：子线程完成任务\n");
    return 0;

}




