// //进程号和父进程号
// #include<stdio.h>
// #include<unistd.h>//getpid getppid
// int main()
// {
//     printf("进程号:%d\n",getpid());
//     printf("父进程号:%d\n",getppid());

// }


// //复习一下进程的内存分布： 文本段，数据段，BSS，栈，堆。
// #include<stdio.h>
// #include<stdlib.h>
// int a = 0;
// int b;
// int main()
// {
//     int c = 0;
//     int *d = (int*)malloc(sizeof(int));
//     printf("a:%p\n",&a);
//     printf("b:%p\n",&b);
//     printf("c:%p\n",&c);
//     printf("*d:%p\n",&d);
//     free(d);
//     return 0;
// }



// //命令行参数
// #include<stdio.h>
// #include<stdlib.h>
// int main(int argc,char *argv[])
// {
//     printf("argc: %d\n",argc);
//     for(int i = 0;i<argc;i++)
//     {
//         printf("argv[%d] = %s\n",i,argv[i]);
//         char **p;
//         for(p=argv;*p!=NULL;p++)
//         {
//             puts(*p);
//         }
//         exit(EXIT_SUCCESS);
//     }
// }


