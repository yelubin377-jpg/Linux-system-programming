// 命令行参数解析

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct
{
    char *MingZi;   // 名字
    int NianLing;   // 年龄
    int XiangXi;    // 详细
} XuanXiang;        // 选项

// 字符转整数
int ZiFuZhuanZhengShu(const char *ZiFu, int *JieGuo)  // 字符, 结果
{
    char *JieShu;  // 结束
    long Zhi = strtol(ZiFu, &JieShu, 10);  // 值
    if (*JieShu != '\0')
        return -1;
    *JieGuo = (int)Zhi;
    return 0;
}

// 解析参数
int JieXiCanShu(int CanShuShu, char *CanShuZu[], XuanXiang *XX)  // 参数数, 参数组, XX=选项
{
    for (int i = 1; i < CanShuShu; i++)
    {
        if (strcmp(CanShuZu[i], "-n") == 0 || strcmp(CanShuZu[i], "--name") == 0)
        {
            if (i + 1 >= CanShuShu)
                return -1;
            XX->MingZi = CanShuZu[++i];
        }
        else if (strcmp(CanShuZu[i], "-a") == 0 || strcmp(CanShuZu[i], "--age") == 0)
        {
            if (i + 1 >= CanShuShu)
                return -1;
            if (ZiFuZhuanZhengShu(CanShuZu[++i], &XX->NianLing) != 0)
                return -1;
        }
        else if (strcmp(CanShuZu[i], "-v") == 0)
        {
            XX->XiangXi = 1;
        }
        else if (strcmp(CanShuZu[i], "-h") == 0 || strcmp(CanShuZu[i], "--help") == 0)
        {
            printf("YongFa: %s [-n MingZi] [-a NianLing] [-v]\n", CanShuZu[0]);  // 用法: [-n 名字] [-a 年龄]
            return 1;
        }
    }
    return 0;
}

int main(int CanShuShu, char *CanShuZu[])  // 参数数, 参数组
{
    XuanXiang XX = {NULL, -1, 0};  // 选项
    
    int JieGuo = JieXiCanShu(CanShuShu, CanShuZu, &XX);  // 结果
    if (JieGuo != 0)
        return JieGuo == 1 ? 0 : 1;
    
    if (XX.XiangXi)  // 详细模式
    {
        printf("CanShu ShuLiang: %d\n", CanShuShu);  // 参数数量
        for (int i = 0; i < CanShuShu; i++)
            printf("  [%d] %s\n", i, CanShuZu[i]);
    }
    
    printf("\nJieXi JieGuo:\n");  // 解析结果
    printf("  MingZi: %s\n", XX.MingZi ? XX.MingZi : "(WeiSheZhi)");  // 名字: (未设置)
    printf("  NianLing: %d\n", XX.NianLing);  // 年龄
    
    if (XX.MingZi && XX.NianLing > 0)
        printf("\nNiHao %s, Ni JinNian %d Sui\n", XX.MingZi, XX.NianLing);  // 你好, 你今年x岁
    
    return 0;
}
