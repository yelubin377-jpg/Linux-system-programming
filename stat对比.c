#include <stdio.h>
#include <sys/stat.h>

// stat vs lstat 行为对比实验


// 打印文件类型
// 用宏判断文件类型，这些宏定义在sys/stat.h里
void DaYinLeiXing(struct stat *WenJianXinXi)
{
    // 其实可以用switch 但是我觉得if-else看着清楚一点
    if (S_ISREG(WenJianXinXi->st_mode))       printf("普通文件");
    else if (S_ISDIR(WenJianXinXi->st_mode))  printf("目录");
    else if (S_ISLNK(WenJianXinXi->st_mode))  printf("符号链接");  // 这个是重点！
    else                                       printf("其他类型");
    // 还有S_ISCHR(字符设备)、S_ISBLK(块设备)等，用的少
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("用法: %s <文件名>\n", argv[0]);
        printf("试试: %s /bin/sh  (这通常是个符号链接)\n", argv[0]);

        return 1;
    }
    
    struct stat XinXi1, XinXi2;
    
    // stat: 跟随符号链接，返回目标文件信息
    // 防背刺：如果文件是符号链接，stat返回的是链接指向的文件的信息！
    // 这个坑我踩过 当时写ls不知道为什么链接显示不对
    printf("\n=== stat结果 ===\n");
    if (stat(argv[1], &XinXi1) == 0)
    {
        printf("类型: ");
        DaYinLeiXing(&XinXi1);
        printf("\ninode: %lu, 大小: %ld 字节\n", XinXi1.st_ino, XinXi1.st_size);
    }
    else
    {
        perror("stat失败");  // 文件不存在或权限不够
    }
    
    // lstat: 不跟随符号链接，返回链接本身信息
    // 想知道一个文件是不是符号链接，必须用lstat！
    // 这个是重点 考试可能考
    printf("\n=== lstat结果 ===\n");
    if (lstat(argv[1], &XinXi2) == 0)
    {
        printf("类型: ");
        DaYinLeiXing(&XinXi2);
        printf("\ninode: %lu, 大小: %ld 字节\n", XinXi2.st_ino, XinXi2.st_size);
    }
    else
    {
        perror("lstat失败");
    }
    
    // 对比一下inode
    if (XinXi1.st_ino != XinXi2.st_ino)
    {
        printf("\n[提示] inode不同，说明这是个符号链接！\n");
    }
    else
    {
        printf("\n[提示] inode相同，这不是符号链接\n");
    }
    
    return 0;
}

// 测试方法:
// 1. 创建测试文件: echo "hello" > test.txt
// 2. 创建符号链接: ln -s test.txt link.txt
// 3. 编译运行:
//    gcc stat对比.c -o stat对比
//    ./stat对比 test.txt   → 两者结果相同
//    ./stat对比 link.txt   → 结果不同！重点观察这个
//    ./stat对比 /bin/sh    → 这个也是链接 可以试试
//
// 关键区别：
// - stat:  符号链接 → 返回目标文件信息（穿透链接）
// - lstat: 符号链接 → 返回链接自身信息（不穿透）

