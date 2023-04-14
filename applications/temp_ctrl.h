/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-06     wyk       the first version
 */
#ifndef APPLICATIONS_TEMP_CTRL_H_
#define APPLICATIONS_TEMP_CTRL_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <rtdbg.h>


extern float Temp;          //位置1和位置2的平均温度值

extern int temp_command_thread(void);
extern void end_tempcmd();


#endif /* APPLICATIONS_TEMP_CTRL_H_ */
