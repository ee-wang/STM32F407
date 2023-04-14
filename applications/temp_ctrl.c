/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-06     wyk       the first version
 */
#include "temp_ctrl.h"
#include "math.h"


/* 温度采集 注意！！！ */

#define HEAT_CTRL  GET_PIN(E, 2)  //加热控制
#define COLD_CTRL  GET_PIN(E, 3)  //制冷控制
#define DOOR_CLOSE GET_PIN(E,12)  //检测箱门是否关闭，1为打开，0为关闭

#define TEMP_DEV_NAME        "adc1"      /* 温度采集 设备名称 */
#define TEMP_ADC1             0           /* ADC 通道 */
#define TEMP_ADC2     1           /* ADC 通道 */
#define REFER_VOLTAGE       249         /* 参考电压 2.49V,数据精度乘以100保留2位小数*/
#define TEMP_REF_VOL       320         /* 温度参考电压 2.49V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)   /* 转换位数为12位 */

#define NTC_R           10000         //热敏电阻NTC默认值为10k
#define NTC_B           3950
#define T             (273.15+25.0)   //热敏电阻为10k时室温为25摄氏度

int16_t NTC_R1;     //热敏电阻NTC1
int16_t NTC_R2;     //热敏电阻NTC2
float Temp1;         //位置1的温度值
float Temp2;         //位置2的温度值
float Temp;          //位置1和位置2的平均温度值

rt_thread_t control = RT_NULL;

rt_adc_device_t adc_dev;
rt_uint32_t value, vol1, vol2;
rt_err_t ret = RT_EOK;

int temp_ctrl(int argc, char *argv[])
{
    /* 查找设备 */
    adc_dev = (rt_adc_device_t) rt_device_find(TEMP_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        LOG_W("can't find %s device!\n", TEMP_DEV_NAME);
        return RT_ERROR;
    }

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, TEMP_ADC1);

    /* 读取采样值 */
    value = rt_adc_read(adc_dev, TEMP_ADC1);

    /* 转换为对应电压值 */
    vol1 = value * REFER_VOLTAGE / CONVERT_BITS;

    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, TEMP_ADC1);

    /* 使能设备 */
    ret = rt_adc_enable(adc_dev, TEMP_ADC2);

    /* 读取采样值 */
    value = rt_adc_read(adc_dev, TEMP_ADC2);

    /* 转换为对应电压值 */
    vol2 = value * REFER_VOLTAGE / CONVERT_BITS;

    /* 关闭通道 */
    ret = rt_adc_disable(adc_dev, TEMP_ADC2);

    /* 计算TEMP_ADC1采到的热敏电阻的阻值 */
    NTC_R1 = (10000 * vol1) / (TEMP_REF_VOL - vol1);
    LOG_I("the NTC_R1 is :%d \n", NTC_R1);

    /* 计算TEMP_ADC2采到的热敏电阻的阻值 */
    NTC_R2 = (10000 * vol1) / (TEMP_REF_VOL - vol2);
    LOG_I("the NTC_R2 is :%d \n", NTC_R2);

    Temp1 = 1 / (log((float) NTC_R1 / (float) NTC_R) / (float) NTC_B + 1 / T);
    Temp1 = Temp1 - 273.15;        //最终的温度值
    LOG_I("the temperature is :%.1f \n", Temp1);

    Temp2 = 1 / (log((float) NTC_R2 / (float) NTC_R) / (float) NTC_B + 1 / T);
    Temp2 = Temp2 - 273.15;        //最终的温度值
    LOG_I("the temperature is :%.1f \n", Temp2);

    Temp=(Temp1+Temp2)/2;
    LOG_I("the temperature is :%.1f \n", Temp);
    return ret;
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(temp_ctrl, temp_ctrl);

#define THREAD_PRIORITY         27      //线程优先级
#define THREAD_STACK_SIZE       1024     //线程栈大小
#define THREAD_TIMESLICE        5       //线程的时间片大小

static void temp_obser_thread(void *param)
{
    while (1)
    {
        /* 线程 1每隔5S更新温度值 */
        temp_ctrl(0,0);
        rt_thread_mdelay(5000);
    }
}
static void temp_contr_thread(void *param)
{
    while(1)
    {

        /* 线程 2根据所测温度值进行温度控制*/
        if(Temp<24.5)
        {
            rt_pin_mode(HEAT_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(HEAT_CTRL, PIN_HIGH);
            rt_pin_mode(COLD_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(COLD_CTRL, PIN_LOW);
        }
        else if (Temp>25.5)
        {
            rt_pin_mode(HEAT_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(HEAT_CTRL, PIN_LOW);
            rt_pin_mode(COLD_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(COLD_CTRL, PIN_HIGH);
        }
        else if((Temp>=24.5) && (Temp<=25.5))
        {
            rt_pin_mode(HEAT_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(HEAT_CTRL, PIN_LOW);
            rt_pin_mode(COLD_CTRL, PIN_MODE_OUTPUT);
            rt_pin_write(COLD_CTRL, PIN_LOW);
        }

    }
}

int temp_detect_thread(void)
{
    rt_thread_t obser = RT_NULL;
    obser=rt_thread_create("thermometry",
            temp_obser_thread, RT_NULL,
            THREAD_STACK_SIZE,
            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (obser != RT_NULL)
    {
        rt_thread_startup(obser);
    }
    return RT_EOK;
}
/*温度检测线程一开始自动运行*/
INIT_COMPONENT_EXPORT(temp_detect_thread);

int temp_command_thread(void)
{
    control=rt_thread_create("tempcmd",
            temp_contr_thread, RT_NULL,
            THREAD_STACK_SIZE,
            THREAD_PRIORITY, THREAD_TIMESLICE);
    if (control != RT_NULL)
    {
        rt_kprintf("start tempcmd\n");
        rt_thread_startup(control);
    }
    return RT_EOK;
}
/*温度控制线程输入命令运行*/
MSH_CMD_EXPORT(temp_command_thread,temp_command_thread);

void end_tempcmd()
{
    if (rt_thread_find("tempcmd") != RT_NULL)
    {
        rt_kprintf("end tempcmd\n");
        rt_thread_delete(control);
    }
}
MSH_CMD_EXPORT(end_tempcmd, end_tempcmd);

