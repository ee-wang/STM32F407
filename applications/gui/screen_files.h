/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-29     YAYA       the first version
 */
#ifndef __SCREEN_FILES_H__
#define __SCREEN_FILES_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/

/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/
extern void screen_files_enter(void);
extern void filesrecord_get(int);
extern void files_botton(uint16_t scrid, uint16_t ctrlid, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __SCREEN_FILES_H__ */
