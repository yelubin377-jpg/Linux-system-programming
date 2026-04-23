// 错误处理与perror使用

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

// 测试文件错误
void CeShiWenJianCuoWu()
{
    // 打开不存在的文件
    FILE *WenJian = fopen("/tmp/bu_cun_zai_wen_jian.txt", "r");  // 文件
    if (!WenJian)
    {
        perror("fopen ShiBai");                   // fopen失败
        printf("errno Zhi: %d\n", errno);         // errno值
        printf("CuoWu MiaoShu: %s\n", strerror(errno));  // 错误描述
    }
}

//  测试权限错误
void CeShiQuanXianCuoWu()
{
    //  尝试写入只读目录
    FILE *WenJian = fopen("/root/ceshi.txt", "w");  // 文件
    if (!WenJian)
        perror("QuanXian CuoWu");  // 权限错误
}

//  显示常见错误
void XianShiChangJianCuoWu()
{
    printf("\nChangJian errno:\n");  // 常见errno
    printf("  ENOENT(%d): %s\n", ENOENT, strerror(ENOENT));
    printf("  EACCES(%d): %s\n", EACCES, strerror(EACCES));
    printf("  EINVAL(%d): %s\n", EINVAL, strerror(EINVAL));
    printf("  ENOMEM(%d): %s\n", ENOMEM, strerror(ENOMEM));
}

int main(void)
{
    // stdout和stderr区别
    printf("[stdout] ZheShiZhengChang ShuChu\n");         // 这是正常输出
    fprintf(stderr, "[stderr] ZheShiCuoWu ShuChu\n");     // 这是错误输出
    
    printf("\nWenJian CuoWu CeShi:\n");  // 文件错误测试
    CeShiWenJianCuoWu();
    
    printf("\nQuanXian CuoWu CeShi:\n");  // 权限错误测试
    CeShiQuanXianCuoWu();
    
    XianShiChangJianCuoWu();
    
    //  文件读写演示
    printf("\nWenJian CaoZuo:\n");  // 文件操作
    FILE *WenJian = fopen("/tmp/ceshi_wenjian.txt", "w");  // 文件
    if (WenJian)
    {
        fprintf(WenJian, "ceshi shuju\n");  // 测试数据
        fclose(WenJian);
        printf("XieRu ChengGong\n");  // 写入成功
        
        WenJian = fopen("/tmp/ceshi_wenjian.txt", "r");
        if (WenJian)
        {
            char HuanChongQu[64];  // 缓冲区
            fgets(HuanChongQu, sizeof(HuanChongQu), WenJian);
            printf("DuQu: %s", HuanChongQu);  // 读取
            fclose(WenJian);
        }
    }
    
    return 0;
}
