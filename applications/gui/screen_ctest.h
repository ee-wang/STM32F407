/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-29     YAYA       the first version
 */
#ifndef __SCREEN_CTEST_H__
#define __SCREEN_CTEST_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
struct str{
    char number[2];
    char name[256];
    char time[5];
    char state[5];
};
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/


/* Public function prototypes ------------------------------------------------*/
extern void record1_get(int);
extern void screen_ctest_enter(void);
extern void ctest_button(uint16_t scrid, uint16_t ctrlid, uint8_t state);


#ifdef __cplusplus
}
#endif

#endif /*__SCREEN_CTEST_H__ */
