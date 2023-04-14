/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-03     wyk       the first version
 */
#include "myflash.h"

#define FLASH_DEVICE_NAME "GD25Q128"

static int rt_hw_spi_flash_init(void)
{
    __HAL_RCC_GPIOA_CLK_ENABLE();
    //此处为片选引脚
    rt_hw_spi_device_attach("spi1", "spi10", GPIOA, GPIO_PIN_4);

    //flash名称要正确
    if (RT_NULL == rt_sfud_flash_probe(FLASH_DEVICE_NAME, "spi10"))
    {
        return -RT_ERROR;
    };

    return RT_EOK;
}
/* 导出到自动初始化 */
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);

static int mnt_init(void)
{
    //mkfs("elm",FLASH_DEVICE_NAME);//挂载前需格式化
    if(dfs_mount(FLASH_DEVICE_NAME,"/","elm",0,0)==0) //挂载文件系统，参数：块设备名称、挂载目录、文件系统类型、读写标志、私有数据0
    {
        rt_kprintf("dfs mount success\r\n");
    }
    else
    {
        rt_kprintf("dfs mount failed\r\n");
    }
    fal_init();

    return RT_EOK;
}
INIT_ENV_EXPORT(mnt_init);

