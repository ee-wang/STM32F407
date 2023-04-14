/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-05     YAYA       the first version
 */
#ifndef _SCREEN_NAMEWRT_H_
#define _SCREEN_NAMEWRT_H_

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/

extern char gui_name1[5];
extern char gui_name2[5];
extern char gui_name3[5];
extern char gui_name4[5];

int name_flag;

/* Public function prototypes ------------------------------------------------*/
extern void name_button(uint16_t scrid, uint16_t ctrlid, uint8_t state);
extern void name_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str);

#ifdef __cplusplus
}
#endif


#endif /* _SCREEN_NAMEWRT_H_ */
