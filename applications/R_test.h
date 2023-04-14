/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2022-12-06     wyk       the first version
 */
#ifndef APPLICATIONS_R_TEST_H_
#define APPLICATIONS_MYADC_H_

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include <rtdbg.h>
#define R1   1
#define R2   2
#define R3   3
#define R4   4
#define NO 0                                    /* 常开触点 */
#define NC 1                                    /* 常闭触点 */
float rx_nx_test(uint16_t RX,uint16_t NX);

#endif /* APPLICATIONS_R_TEST_H_ */
