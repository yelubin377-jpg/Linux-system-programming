// //进程号和父进程号
// #include<stdio.h>
// #include<unistd.h>//getpid getppid
// int main()
// {
//     printf("进程号:%d\n",getpid());
//     printf("父进程号:%d\n",getppid());

// }

#include<stdio.h>
#include<stdlib.h>
int a = 0;
int b;
int main()
{
    int c = 0;
    int *d = (int*)malloc(sizeof(int));
    printf("a:%p\n",&a);
    printf("b:%p\n",&b);
    printf("c:%p\n",&c);
    printf("*d:%p\n",&d);
    free(d);
    return 0;
}
