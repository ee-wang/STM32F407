/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-06     wyk       the first version
 */
#include <R_test.h>

#define R_TEST_DEV_NAME        "adc2"           /* 电阻测量 设备名称 */
#define REFER_VOLTAGE       249             /* 参考电压2.49V,数据精度乘以100保留2位小数*/
#define CONVERT_BITS        (1 << 12)       /* 转换位数为12位 */

static void Rx_config();
static float Rx_test();
static float r_test();
enum
{
    R1_CHANNEL = 2, /* R1 ADC通道 */
    R1_NO_CUR_CTRL = GET_PIN(B, 5), /* R1常开触点电流输出开关 */
    R1_NO_DET = GET_PIN(B, 4), /* 检测R1常开触点通断*/
    R1_NC_CUR_CTRL = GET_PIN(B, 3), /* R1常闭触点电流输出开关 */
    R1_NC_DET = GET_PIN(D, 7), /* 检测R1常闭触点通断*/
    R1_NC_NO_SW_CTRL = GET_PIN(E, 10), /* R1常开常闭触点电阻测量选择开关 1测量常开  0测量常闭*/
};

enum
{
    R2_CHANNEL = 3, /* R2 ADC通道 */
    R2_NO_CUR_CTRL = GET_PIN(D, 6), /* R2常开触点电流输出开关 */
    R2_NO_DET = GET_PIN(D, 5), /* 检测R2常开触点通断 */
    R2_NC_CUR_CTRL = GET_PIN(D, 4), /* R2常闭触点电流输出开关 */
    R2_NC_DET = GET_PIN(D, 3), /* 检测R2常闭触点通断 */
    R2_NC_NO_SW_CTRL = GET_PIN(E, 9), /* R2常开常闭触点电阻测量选择开关 1测量常开  0测量常闭*/
};

enum
{
    R3_CHANNEL = 12, /* R3 ADC通道 */
    R3_NO_CUR_CTRL = GET_PIN(C, 9), /* R3常开触点电流输出开关 */
    R3_NO_DET = GET_PIN(C, 8), /* 检测R3常开触点通断 */
    R3_NC_CUR_CTRL = GET_PIN(D, 15), /* R3常闭触点电流输出开关 */
    R3_NC_DET = GET_PIN(D, 14), /* 检测R3常闭触点通断 */
    R3_NC_NO_SW_CTRL = GET_PIN(E, 8), /* R3常开常闭触点电阻测量选择开关 1测量常开  0测量常闭*/
};

enum
{
    R4_CHANNEL = 11, /* R4 ADC通道 */
    R4_NO_CUR_CTRL = GET_PIN(D, 13), /* R4常开触点电流输出开关 */
    R4_NO_DET = GET_PIN(D, 12), /* 检测R4常开触点通断 */
    R4_NC_CUR_CTRL = GET_PIN(D, 11), /* R4常闭触点电流输出开关 */
    R4_NC_DET = GET_PIN(D, 10), /* 检测R4常闭触点通断 */
    R4_NC_NO_SW_CTRL = GET_PIN(E, 11), /* R4常开常闭触点电阻测量选择开关 1测量常开  0测量常闭*/
};

static rt_adc_device_t adc_dev;
static rt_uint32_t value, vol;
static rt_err_t ret = RT_EOK;

rt_uint8_t Rx, Nx; //Rx可选择R1,R2,R3,R4 //Nx可选择NO(常开),NC(常闭)
rt_uint8_t Rx_channel, Rx_Nx_cur_ctrl, Rx_Nx_det, Rx_nc_no_sw_ctrl; /* 定义各个变量 */

float rx_nx_test(uint16_t RX,uint16_t NX)
{
    Rx = RX;
    Nx = NX;
    return r_test();
}

static float r_test()
{
    /* 查找设备 */
    adc_dev = (rt_adc_device_t) rt_device_find(R_TEST_DEV_NAME);
    if (adc_dev == RT_NULL)
    {
        LOG_W("can't find %s device!\n", R_TEST_DEV_NAME);
        return RT_ERROR;
    }

    Rx_config(); //配置Rx的常开/常闭触点为待测电阻
    rt_thread_mdelay(10);
    return Rx_test(); //测电阻
}

static int Rx_init()
{
    rt_pin_mode(R1_NO_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R1_NO_DET, PIN_MODE_INPUT);
    rt_pin_mode(R1_NC_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R1_NC_DET, PIN_MODE_INPUT);
    rt_pin_mode(R1_NC_NO_SW_CTRL, PIN_MODE_OUTPUT);

    rt_pin_mode(R2_NO_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R2_NO_DET, PIN_MODE_INPUT);
    rt_pin_mode(R2_NC_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R2_NC_DET, PIN_MODE_INPUT);
    rt_pin_mode(R2_NC_NO_SW_CTRL, PIN_MODE_OUTPUT);

    rt_pin_mode(R3_NO_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R3_NO_DET, PIN_MODE_INPUT);
    rt_pin_mode(R3_NC_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R3_NC_DET, PIN_MODE_INPUT);
    rt_pin_mode(R3_NC_NO_SW_CTRL, PIN_MODE_OUTPUT);

    rt_pin_mode(R4_NO_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R4_NO_DET, PIN_MODE_INPUT);
    rt_pin_mode(R4_NC_CUR_CTRL, PIN_MODE_OUTPUT);
    rt_pin_mode(R4_NC_DET, PIN_MODE_INPUT);
    rt_pin_mode(R4_NC_NO_SW_CTRL, PIN_MODE_OUTPUT);

    rt_pin_write(R1_NC_NO_SW_CTRL, PIN_HIGH);
    rt_pin_write(R2_NC_NO_SW_CTRL, PIN_HIGH);
    rt_pin_write(R3_NC_NO_SW_CTRL, PIN_HIGH);
    rt_pin_write(R4_NC_NO_SW_CTRL, PIN_HIGH);
    return RT_EOK;
}
INIT_BOARD_EXPORT(Rx_init);

static float Rx_test()
{
    float u_out = 0;
    float R = 0;
    if (rt_pin_read(Rx_Nx_det) == PIN_LOW) //低电平说明触点接通
    {
        rt_pin_write(Rx_Nx_cur_ctrl, PIN_HIGH); //输出1A电流用于测试电阻
        rt_thread_mdelay(100);
        /* 使能设备 */
        ret = rt_adc_enable(adc_dev, Rx_channel);

        /* 读取采样值 */
        value = rt_adc_read(adc_dev, Rx_channel);

        /* 转换为对应电压值 */
        vol = value * REFER_VOLTAGE / CONVERT_BITS;

        u_out = vol / (100.0);

        /* 经过测试，线路阻抗为3.4毫欧 */
        R = u_out * 10 - 3.4;

        LOG_I("the R%d :  = :%.2f mOhm \n", Rx, R);

        /* 关闭通道 */
        ret = rt_adc_disable(adc_dev, Rx_channel);

        rt_thread_mdelay(100);
        rt_pin_write(Rx_Nx_cur_ctrl, PIN_LOW); //关闭1A的输出
        rt_adc_disable(adc_dev, Rx_channel);//关闭adc通道
        //rt_pin_write(Rx_nc_no_sw_ctrl, PIN_HIGH);
        return R;
    }
    else
    {
        LOG_W("The resistance to be measured is not connected!!!");
        return 0;
    }
}

static void Rx_config()
{
    switch (Rx)
    {
    case R1:
        Rx_channel = R1_CHANNEL;
        Rx_nc_no_sw_ctrl = R1_NC_NO_SW_CTRL;
        if (Nx == NO)
        {
            Rx_Nx_cur_ctrl = R1_NO_CUR_CTRL;
            Rx_Nx_det = R1_NO_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_HIGH);
        }
        else if (Nx == NC)
        {
            Rx_Nx_cur_ctrl = R1_NC_CUR_CTRL;
            Rx_Nx_det = R1_NC_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_LOW);
        }
        break;
    case R2:
        Rx_channel = R2_CHANNEL;
        Rx_nc_no_sw_ctrl = R2_NC_NO_SW_CTRL;
        if (Nx == NO)
        {
            Rx_Nx_cur_ctrl = R2_NO_CUR_CTRL;
            Rx_Nx_det = R2_NO_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_HIGH);
        }
        else if (Nx == NC)
        {
            Rx_Nx_cur_ctrl = R2_NC_CUR_CTRL;
            Rx_Nx_det = R2_NC_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_LOW);
        }
        break;
    case R3:
        Rx_channel = R3_CHANNEL;
        Rx_nc_no_sw_ctrl = R3_NC_NO_SW_CTRL;
        if (Nx == NO)
        {
            Rx_Nx_cur_ctrl = R3_NO_CUR_CTRL;
            Rx_Nx_det = R3_NO_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_HIGH);
        }
        else if (Nx == NC)
        {
            Rx_Nx_cur_ctrl = R3_NC_CUR_CTRL;
            Rx_Nx_det = R3_NC_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_LOW);
        }
        break;
    case R4:
        Rx_channel = R4_CHANNEL;
        Rx_nc_no_sw_ctrl = R4_NC_NO_SW_CTRL;
        if (Nx == NO)
        {
            Rx_Nx_cur_ctrl = R4_NO_CUR_CTRL;
            Rx_Nx_det = R4_NO_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_HIGH);
        }
        else if (Nx == NC)
        {
            Rx_Nx_cur_ctrl = R4_NC_CUR_CTRL;
            Rx_Nx_det = R4_NC_DET;
            rt_pin_write(Rx_nc_no_sw_ctrl, PIN_LOW);
        }
        break;
    }
}
