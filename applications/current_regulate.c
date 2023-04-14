/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-06     wyk       the first version
 */
#include "current_regulate.h"

/* 电流调节 RS485 的任务也在这边写 ！！！ */

/* 电流调节 注意！！！ */
/* 要采集有效值，因为是交流电！！！ */



#define CURRENT_REGULATE_DEV_NAME        "adc3"      /* 电流调节 设备名称 */
#define CURRENT_REGULATE_CHANNEL     10           /* ADC 通道 */
#define REFER_VOLTAGE       249         /* 参考电压 2.49V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

static int current_regulate(int argc, char *argv[])
{
    rt_adc_device_t adc_dev;
    rt_uint32_t value, vol;
    rt_err_t ret = RT_EOK;

    /* 查找设备 */
    adc_dev = (rt_adc_device_t)rt_device_find(CURRENT_REGULATE_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        rt_kprintf("can't find %s device!\n", CURRENT_REGULATE_DEV_NAME);
        return RT_ERROR;
    }

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, CURRENT_REGULATE_CHANNEL);

    /* 读取采样值 */
    value = rt_adc_read(adc_dev, CURRENT_REGULATE_CHANNEL);

    /* 转换为对应电压值 */
    vol = value * REFER_VOLTAGE / CONVERT_BITS;
    rt_kprintf("the current_regulate voltage is :%d.%02d \n", vol / 100, vol % 100);

    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, CURRENT_REGULATE_CHANNEL);

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(current_regulate, current_regulate);
