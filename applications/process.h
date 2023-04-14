/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-25     wyk       the first version
 */
#ifndef APPLICATIONS_PROCESS_H_
#define APPLICATIONS_PROCESS_H_

#include <stdint.h>
#include <stdio.h>
#include "state_machine.h"
#include "rtthread.h"
#include "modbus_ACpower.h"
#include "R_test.h"

extern u_int32_t gui_current;
extern u_int8_t num;
extern u_int8_t type;
extern char gui_r1_cold_time[5], gui_r2_cold_time[5], gui_r3_cold_time[5], gui_r4_cold_time[5];
extern char gui_r1_hot_time[5], gui_r2_hot_time[5], gui_r3_hot_time[5], gui_r4_hot_time[5];
extern char gui_r1_nc[10], gui_r1_no[10], gui_r2_nc[10], gui_r2_no[10], gui_r3_nc[10], gui_r3_no[10], gui_r4_nc[10], gui_r4_no[10];
extern char *gui_r1_ans, *gui_r2_ans, *gui_r3_ans, *gui_r4_ans;

extern bool finish_cold1;      //继电1冷态校验完成标志位
extern bool finish_hot1;       //继电1热态校验完成标志位
extern bool finish_cold2;
extern bool finish_hot2;
extern bool finish_cold3;
extern bool finish_hot3;
extern bool finish_cold4;
extern bool finish_hot4;

extern bool file_check;
extern bool files_check;

extern bool hot_switch;        //   热态校验页面切换标志


struct relay
{
    uint16_t cold_time;
    uint16_t hot_time;
    float nc;
    float no;
    char *ans;
    bool cold_finish;
    bool hot_finish;
};

extern int answer;

extern int process_thread_init(void);
extern void end_process();

#endif /* APPLICATIONS_PROCESS_H_ */
