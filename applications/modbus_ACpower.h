/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-10     wyk       the first version
 */
#ifndef APPLICATIONS_MODBUS_ACPOWER_H_
#define APPLICATIONS_MODBUS_TEST_H_

#include "modbus.h"
#include "stdio.h"
#include "string.h"
#include <rtthread.h>
#include <stdlib.h>
#include <rtdevice.h>
#include "drv_common.h"
#include "dfs_fs.h"
#include "fal.h"
extern int voltage;
extern uint8_t modbus_connect_or_not;
void i_v_output(int cur ,int vol);
void i_v_close();
#endif /* APPLICATIONS_MODBUS_ACPOWER_H_ */
