/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-11-18     wyk       the first version
 */
#include "myrtc.h"

#define RTC_NAME       "rtc"
#define NOW_DATE        2022, 12, 3
#define NOW_TIME        22, 55, 10

static int rtc_sample(int argc, char *argv[])
{
    rt_err_t ret = RT_EOK;
    time_t now;
    rt_device_t device = RT_NULL;

    /*寻找设备*/
    device = rt_device_find(RTC_NAME);
    if (!device)
    {
      LOG_E("find %s failed!", RTC_NAME);
      return RT_ERROR;
    }

    /*初始化RTC设备*/
    if(rt_device_open(device, 0) != RT_EOK)
    {
      LOG_E("open %s failed!", RTC_NAME);
      return RT_ERROR;
    }

    /* 设置日期 */
    ret = set_date(NOW_DATE);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC date failed\n");
        return ret;
    }

    /* 设置时间 */
    ret = set_time(NOW_TIME);
    if (ret != RT_EOK)
    {
        rt_kprintf("set RTC time failed\n");
        return ret;
    }

    /* 延时3秒 */
    rt_thread_mdelay(3000);

    /* 获取时间 */
    now = time(RT_NULL);
    rt_kprintf("%s\n", ctime(&now));

    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(rtc_sample, rtc sample);
