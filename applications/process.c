/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-24     wyk       the first version
 */
#include "process.h"
#include "gui\hmi_driver.h"
#include "gui\screen_comm.h"

#define R1_CTRL           GET_PIN(E, 13)
#define R2_CTRL           GET_PIN(E, 14)
#define R3_CTRL           GET_PIN(E, 15)
#define R4_CTRL           GET_PIN(B, 10)
#define R5_CTRL           GET_PIN(B, 12)
#define R6_CTRL           GET_PIN(B, 13)
#define R7_CTRL           GET_PIN(B, 14)
#define R8_CTRL           GET_PIN(B, 15)

#define R1_NC_DET  GET_PIN(D, 7)
#define R1_NO_DET  GET_PIN(B, 4)
#define R2_NC_DET  GET_PIN(D, 3)
#define R2_NO_DET  GET_PIN(D, 5)
#define R3_NC_DET  GET_PIN(D, 14)
#define R3_NO_DET  GET_PIN(C, 8)
#define R4_NC_DET  GET_PIN(D, 10)
#define R4_NO_DET  GET_PIN(D, 12)

#define MULTIPLIER        3/2

rt_thread_t process = RT_NULL;

u_int8_t num=1; //热继数量
u_int8_t type=1; //热继型号
u_int32_t gui_current=4000; //设定电流

char gui_r1_cold_time[5], gui_r2_cold_time[5], gui_r3_cold_time[5], gui_r4_cold_time[5];
char gui_r1_hot_time[5], gui_r2_hot_time[5], gui_r3_hot_time[5], gui_r4_hot_time[5];
char gui_r1_nc[10], gui_r1_no[10], gui_r2_nc[10], gui_r2_no[10], gui_r3_nc[10], gui_r3_no[10], gui_r4_nc[10],
        gui_r4_no[10];
char *gui_r1_ans, *gui_r2_ans, *gui_r3_ans, *gui_r4_ans;

bool finish_cold1;      //继电1冷态校验完成标志位
bool finish_hot1;       //继电1热态校验完成标志位
bool finish_cold2;
bool finish_hot2;
bool finish_cold3;
bool finish_hot3;
bool finish_cold4;
bool finish_hot4;

bool file_check;
bool files_check;
bool hot_switch;        //   热态校验页面切换标志

enum
{
    TA25 = 1, TA42, TA75, TA80
};

void gate_init()
{
    switch (num)
    {
    case 1:
        rt_pin_write(R1_CTRL, PIN_LOW);
        rt_pin_write(R2_CTRL, PIN_LOW);
        rt_pin_write(R3_CTRL, PIN_LOW);
        rt_pin_write(R4_CTRL, PIN_LOW);
        rt_pin_write(R5_CTRL, PIN_LOW);
        rt_pin_write(R6_CTRL, PIN_LOW);
        rt_pin_write(R7_CTRL, PIN_LOW);
        rt_pin_write(R8_CTRL, PIN_LOW);
        break;
    case 2:
        rt_pin_write(R1_CTRL, PIN_LOW);
        rt_pin_write(R2_CTRL, PIN_LOW);
        rt_pin_write(R3_CTRL, PIN_LOW);
        rt_pin_write(R4_CTRL, PIN_LOW);
        rt_pin_write(R5_CTRL, PIN_LOW);
        rt_pin_write(R6_CTRL, PIN_HIGH); //打开
        rt_pin_write(R7_CTRL, PIN_LOW);
        rt_pin_write(R8_CTRL, PIN_HIGH); //打开
        break;
    case 3:
        rt_pin_write(R1_CTRL, PIN_LOW);
        rt_pin_write(R2_CTRL, PIN_LOW);
        rt_pin_write(R3_CTRL, PIN_LOW);
        rt_pin_write(R4_CTRL, PIN_HIGH);  //打开
        rt_pin_write(R5_CTRL, PIN_LOW);
        rt_pin_write(R6_CTRL, PIN_HIGH); //打开
        rt_pin_write(R7_CTRL, PIN_LOW);
        rt_pin_write(R8_CTRL, PIN_HIGH); //打开
        break;
    case 4:
        rt_pin_write(R1_CTRL, PIN_LOW);
        rt_pin_write(R2_CTRL, PIN_HIGH); //打开
        rt_pin_write(R3_CTRL, PIN_LOW);
        rt_pin_write(R4_CTRL, PIN_LOW); //打开
        rt_pin_write(R5_CTRL, PIN_LOW);
        rt_pin_write(R6_CTRL, PIN_HIGH); //打开
        rt_pin_write(R7_CTRL, PIN_LOW);
        rt_pin_write(R8_CTRL, PIN_HIGH); //打开
        break;
    }
}

void check_pass(struct relay *r)
{
    if (type == TA25)
    {
        if ((*r).cold_time >= 1 && (*r).cold_time <= 157 && (*r).hot_time >= 1 && (*r).hot_time <= 40)
            (*r).ans = "pass";
        else
            (*r).ans = "fail";
    }
    else
    {
        if ((*r).cold_time >= 1 && (*r).cold_time <= 328 && (*r).hot_time >= 1 && (*r).hot_time <= 102)
            (*r).ans = "pass";
        else
            (*r).ans = "fail";
    }
}

void gui_to_string(struct relay r1, struct relay r2, struct relay r3, struct relay r4)
{
    gui_r1_ans = r1.ans;
    gui_r2_ans = r2.ans;
    gui_r3_ans = r3.ans;
    gui_r4_ans = r4.ans;
}

void process_entry()
{
    bool finish_cold_all = false;
    bool finish_hot_all = false;
    u_int16_t cold_time = 0, hot_time = 0;
    gate_init(); //投切的继电器初始化
    struct relay r1 = { 0, 0, 0, 0, NULL, false, false };
    struct relay r2 = { 0, 0, 0, 0, NULL, false, false };
    struct relay r3 = { 0, 0, 0, 0, NULL, false, false };
    struct relay r4 = { 0, 0, 0, 0, NULL, false, false };
//    gui_current = (int)current*1000;
    switch (num)
    {
    case 1:
        //r1.nc = rx_nx_test(R1, NC); //测量常闭触点阻值，单位为毫欧
        i_v_output(gui_current * MULTIPLIER, voltage); //输出电流
        rt_kprintf("gui_current = %d\n",gui_current);
        rt_kprintf("voltage = %d\n",voltage);
        while (finish_cold_all != true) //如果冷态校验还没完成
        {
            rt_kprintf("cold...\n");
            rt_thread_mdelay(1000);
            ++cold_time;
            if (rt_pin_read(R1_NC_DET) == PIN_HIGH) //如果常闭触点断开
            {
                r1.cold_finish = true; //冷态校验完成
                finish_cold1 = true;
                finish_cold_all = true; //所有热继冷态校验完成
                i_v_close(); //关闭电流输出
            }
        }

        r1.cold_time = cold_time;
        sprintf(gui_r1_cold_time, "%u", r1.cold_time); //传冷态时间
        finish_cold1 = true;
        //r1.no = rx_nx_test(R1, NO); //测量常开触点阻值，单位为毫欧

        sprintf(gui_r1_nc, "%.2f", r1.nc); //传常闭触点阻值

        sprintf(gui_r1_no, "%.2f", r1.nc); //传常开触点阻值

        while (rt_pin_read(R1_NC_DET) == PIN_HIGH) //等待复位
        {
            rt_kprintf("cold--hot..\n");
            rt_thread_mdelay(1000);
        }

        rt_kprintf("start hot check...\n");

        hot_switch = true;

        i_v_output(gui_current * MULTIPLIER, voltage); //常开触点断开，此时输出电流进行热态校验

        while (r1.hot_finish != true) //如果热态校验还没完成
        {
            if (rt_pin_read(R1_NC_DET) == PIN_HIGH) //如果常开触点导通
            {
                r1.hot_finish = true; //热态校验完成
                finish_hot1 = true;
                finish_hot_all = true; //所有热继电器热态校验完成
                file_check = true;
                files_check = true;
                i_v_close(); //关闭电流输出
            }
            rt_kprintf("hot...\n");
            rt_thread_mdelay(1000);
            ++hot_time;
        }

        r1.hot_time = hot_time;
        sprintf(gui_r1_hot_time, "%u", r1.hot_time);

        check_pass(&r1);
        break;
    case 2:
        r1.nc = rx_nx_test(R1, NC);
        r2.nc = rx_nx_test(R2, NC); //测量常闭触点阻值，单位为毫欧
        i_v_output(gui_current * MULTIPLIER, voltage); //输出电流
        while (finish_cold_all != true) //如果冷态校验还没完成
        {
            if (r1.cold_finish != true)
            {
                if (rt_pin_read(R1_NC_DET) == PIN_HIGH) //如果r1常闭触点断开
                {
                    r1.cold_finish = true; //r1冷态校验完成
                    finish_cold1 = true;
                    rt_pin_write(R7_CTRL, PIN_HIGH);
                    rt_pin_write(R8_CTRL, PIN_LOW); //投切热继电器1
                    r1.cold_time = cold_time;
                }
            }
            if (r2.cold_finish != true)
            {
                if (rt_pin_read(R2_NC_DET) == PIN_HIGH) //如果r2常闭触点断开
                {
                    r2.cold_finish = true; //r2冷态校验完成
                    finish_cold2 = true;
                    rt_pin_write(R5_CTRL, PIN_HIGH);
                    rt_pin_write(R6_CTRL, PIN_LOW); //投切热继电器2
                    r2.cold_time = cold_time;
                }
            }
            if (r1.cold_finish == true && r2.cold_finish == true)
            {
                finish_cold_all = true; //所有热继电器校验完成
                i_v_close(); //关闭电流输出
                rt_pin_write(R8_CTRL, PIN_HIGH);
                rt_pin_write(R7_CTRL, PIN_LOW); //投切热继电器1
                rt_pin_write(R8_CTRL, PIN_HIGH);
                rt_pin_write(R7_CTRL, PIN_LOW); //投切热继电器2
            }
            rt_kprintf("cold...\n");
            rt_thread_mdelay(1000);
            ++cold_time;
        }

        r1.no = rx_nx_test(R1, NO); //测量常开触点阻值，单位为毫欧
        r2.no = rx_nx_test(R2, NO); //测量常开触点阻值，单位为毫欧
        sprintf(gui_r1_nc, "%.2f", r1.nc); //传常闭触点阻值
        sprintf(gui_r1_no, "%.2f", r1.nc); //传常开触点阻值
        sprintf(gui_r2_nc, "%.2f", r2.nc); //传常闭触点阻值
        sprintf(gui_r2_no, "%.2f", r2.nc); //传常开触点阻值
        sprintf(gui_r1_cold_time, "%u", r1.cold_time); //传冷态时间
        sprintf(gui_r2_cold_time, "%u", r2.cold_time); //传冷态时间

        while (rt_pin_read(R1_NC_DET) == PIN_LOW && rt_pin_read(R2_NC_DET) == PIN_LOW) //等待复位
        {
            rt_kprintf("cold--hot..\n");
            rt_thread_mdelay(1000);
        }
        rt_thread_mdelay(1000);
        rt_kprintf("start hot check...\n");

        hot_switch = true;

        i_v_output(gui_current * MULTIPLIER, voltage); //所有热继电器复位完成后,输出电流进行热态校验

        while (finish_hot_all != true) //如果热态校验还没完成
        {
            if (r1.hot_finish != true)
            {
                if (rt_pin_read(R1_NC_DET) == PIN_HIGH) //如果r1常闭触点断开
                {
                    r1.hot_finish = true; //r1冷态校验完成
                    finish_hot1 = true;
                    rt_pin_write(R7_CTRL, PIN_HIGH);
                    rt_pin_write(R8_CTRL, PIN_LOW); //投切热继电器1
                    r1.hot_time = hot_time;
                }
            }
            if (r2.hot_finish != true)
            {
                if (rt_pin_read(R2_NC_DET) == PIN_HIGH) //如果r2常闭触点断开
                {
                    r2.hot_finish = true; //r2冷态校验完成
                    finish_hot2 = true;
                    rt_pin_write(R5_CTRL, PIN_HIGH);
                    rt_pin_write(R6_CTRL, PIN_LOW); //投切热继电器2
                    r2.hot_time = hot_time;
                }
            }
            if (r1.hot_finish == true && r2.hot_finish == true)
            {
                finish_hot_all = true; //所有热继电器校验完成
                i_v_close(); //关闭电流输出
                rt_pin_write(R8_CTRL, PIN_HIGH);
                rt_pin_write(R7_CTRL, PIN_LOW); //投切热继电器1
                rt_pin_write(R8_CTRL, PIN_HIGH);
                rt_pin_write(R7_CTRL, PIN_LOW); //投切热继电器2
            }
            rt_kprintf("hot...\n");
            rt_thread_mdelay(1000);
            ++hot_time;
        }
        sprintf(gui_r1_hot_time, "%u", r1.hot_time); //传热态时间
        sprintf(gui_r2_hot_time, "%u", r2.hot_time); //传热态时间
        check_pass(&r1);
        check_pass(&r2);
        break;
    case 3:

        break;
    case 4:

        break;
    }
    gui_to_string(r1, r2, r3, r4);
    file_check = true;
    files_check = true;
}

int process_thread_init(void)
{
    process = rt_thread_create("process", process_entry, RT_NULL, 4096, 15, 10);
    if (process != RT_EOK)
    {
        rt_kprintf("start process\n");
        rt_thread_startup(process);
    }
    return RT_EOK;
}
MSH_CMD_EXPORT(process_thread_init, process_thread_init);

void end_process()
{
    if (rt_thread_find("process") != RT_NULL)
    {
        if (modbus_connect_or_not == 1)
        {
            i_v_close();
        }
        rt_kprintf("end process\n");
        rt_thread_delete(process);
    }
}
MSH_CMD_EXPORT(end_process, end_process);
