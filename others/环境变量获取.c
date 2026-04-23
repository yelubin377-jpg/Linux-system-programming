

#include <stdio.h>
#include <stdlib.h>

// 封装一下 省得每次都写判空
void HuoQuBianLiang(const char *MingZi)
{
    char *Zhi = getenv(MingZi);
    if (!Zhi)
        printf("  %-10s -> (BuCunZai)\n", MingZi);
    else
        printf("  %-10s -> %s\n", MingZi, Zhi);
}

// 把PATH拆开一个一个显示 不然糊一坨根本看不清
void ChaiKaiPath()
{
    char *YuanShi = getenv("PATH");
    if (!YuanShi)
    {
        printf("  PATH BuCunZai?!\n");  // 一般不会走到这里
        return;
    }

    printf("  PATH LiMian De MuLu:\n");
    int XuHao = 1;
    char *Qi = YuanShi;

    while (*Qi)
    {
        char *Wei = Qi;
        while (*Wei && *Wei != ':')
            Wei++;
        // Qi到Wei之间就是一个路径
        printf("    [%2d] ", XuHao++);
        for (char *P = Qi; P < Wei; P++)
            putchar(*P);
        putchar('\n');

        if (!*Wei)
            break;
        Qi = Wei + 1;
    }
}

int main(void)
{
    printf("ChangJian HuanJingBianLiang:\n\n");

    // 挑几个常见的看看
    HuoQuBianLiang("HOME");
    HuoQuBianLiang("USER");
    HuoQuBianLiang("SHELL");
    HuoQuBianLiang("LANG");
    HuoQuBianLiang("PWD");     // 当前目录
    HuoQuBianLiang("TERM");
    HuoQuBianLiang("EDITOR");  // 这个大概率没设

    // PATH比较特殊 用冒号分隔了一堆目录 拆开看
    printf("\nPATH XiangXi:\n\n");
    ChaiKaiPath();

    // 演示一下getenv判空的重要性
    printf("\nPanKong YanShi:\n\n");

    char *Zhi = getenv("HOME");
    if (Zhi)
    {
        
    
        printf("  HOME: %s\n", Zhi);

    }
    
    
    Zhi = getenv("WO_SUI_BIAN_XIE_DE");  // 肯定不存在的变量
    if (!Zhi)
        printf("  WO_SUI_BIAN_XIE_DE: NULL (BuPanKong JiuHui SegFault!)\n");
    // 如果不判空直接printf("%s", Zhi)就炸了
    // 之前试过一次 段错误 找了半天bug

    Zhi = getenv("USER");
    if (Zhi)
        printf("  USER: %s\n", Zhi);

    return 0;
}
