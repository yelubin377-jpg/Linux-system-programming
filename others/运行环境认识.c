// 看看一个程序启动的时候都能拿到哪些运行环境信息
// 主要是命令行参数、进程信息、环境变量这些

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern char **environ;  // 这个是系统给的全局变量 存了所有环境变量


int main(int CanShuShu, char *CanShuZu[])
{
    // 命令行参数
    // argv[0]是程序自己的名字 后面的才是用户传的参数
    printf("MingLingHang CanShu (argc=%d):\n", CanShuShu);
    for (int i = 0; i < CanShuShu; i++)
        printf("  argv[%d] = \"%s\"\n", i, CanShuZu[i]);

    // 进程相关的信息
    printf("\nJinCheng XinXi:\n");
    printf("  PID  = %d\n", getpid());     // 当前进程ID
    printf("  PPID = %d\n", getppid());    // 父进程ID 一般是bash
    printf("  UID  = %d\n", getuid());     // 用户ID
    printf("  GID  = %d\n", getgid());
    // printf("  EUID = %d\n", geteuid());  // 有效用户ID 暂时用不到

    // 当前工作目录
    char HuanChong[256];
    if (getcwd(HuanChong, sizeof(HuanChong)))
        printf("\nGongZuo MuLu: %s\n", HuanChong);
    else
        printf("\nGongZuo MuLu HuoQu ShiBai\n");
    // 第一次用getcwd的时候 缓冲区给太小了 直接返回NULL 坑

    // 环境变量有多少个
    int ZongShu = 0;
    while (environ[ZongShu])
        ZongShu++;

    printf("\nHuanJing BianLiang: Gong %d Ge\n", ZongShu);
    // 随便看前面几个
    printf("Qian 5 Ge:\n");
    for (int i = 0; i < 5 && environ[i]; i++)
        printf("  [%d] %s\n", i, environ[i]);
    if (ZongShu > 5)
        printf("  ... HaiYou %d Ge\n", ZongShu - 5);

    // /proc/self 这个目录蛮有意思的 是当前进程的信息
    // Linux特有的 其他系统不一定有
    printf("\n/proc/self XinXi:\n");

    FILE *WenJian = fopen("/proc/self/cmdline", "r");
    if (WenJian)
    {
        printf("  cmdline: ");
        int ZiFu;
        while ((ZiFu = fgetc(WenJian)) != EOF)
            putchar(ZiFu ? ZiFu : ' ');
        // cmdline里参数之间用\0分隔 不是空格 所以要转一下
        printf("\n");
        fclose(WenJian);
    }

    // status文件有一堆进程状态信息 看前几行就行
    WenJian = fopen("/proc/self/status", "r");
    if (WenJian)
    {
        printf("  status QianJiHang:\n");
        char Hang[256];
        int HangShu = 0;
        while (fgets(Hang, sizeof(Hang), WenJian) && HangShu < 5)
        {
            printf("    %s", Hang);
            HangShu++;
        }
        fclose(WenJian);
    }

    // PATH和HOME对程序运行的影响
    printf("\nGuanJian BianLiang:\n");
    char *LuJing = getenv("PATH");
    char *ZhuMuLu = getenv("HOME");
    if (LuJing)
    {
        // PATH决定了 你在终端敲个命令 系统去哪些目录找可执行文件
        int ChangDu = 0;
        while (LuJing[ChangDu])
            ChangDu++;
        printf("  PATH: %d Ge ZiFu (JueZhiLe ChengXu SouSuo LuJing)\n", ChangDu);
    }
    if (ZhuMuLu)
        printf("  HOME: %s\n", ZhuMuLu);

    return 0;
}
