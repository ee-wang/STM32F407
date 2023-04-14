/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-15     wyk       the first version
 */

#include "myflash.h"

int dir_init()
{
    mkdir("/information", 0x777);
    return RT_EOK;
}
INIT_APP_EXPORT(dir_init);


//复制文件函数
int file_copy(const char *dest_file_name, const char *src_file_name)
{
    int fd1 = open(dest_file_name, O_WRONLY | O_CREAT | O_APPEND | O_TRUNC, 0x777);
    if (fd1 == -1)
    {
        rt_kprintf("open file failed\n");
        return -1;
    }
    int fd2 = open(src_file_name, O_RDONLY);
    if (fd2 == -1)
    {
        rt_kprintf("open file failed\n");
        return -1;
    }
    char *buffer = (char *) malloc(2 * sizeof(char));
    int ans = 0;
    int k;
    do
    {
        memset(buffer, 0, sizeof(2));
        k = read(fd2, buffer, 1);
        if (k <= 0)
            break;
        write(fd1, buffer, 1);
        ans++;
    } while (k > 0);
    close(fd1);
    close(fd2);
    return RT_EOK;
}

//写一个表格
void write_csv()
{
    int fd1 = open("/information/machine.csv", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd1 == -1)
    {
        rt_kprintf("file open filed\r\n");
    }
    char buf[] = "hello\n";
    write(fd1, buf, sizeof(buf));
    close(fd1);
}
MSH_CMD_EXPORT(write_csv, write_csv);


//复制文件函数
void mycopy()
{
    file_copy("/udisk/machine.csv","/information/machine.csv");
}MSH_CMD_EXPORT(mycopy, mycopy);
