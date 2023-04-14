/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-03     YAYA       the first version
 */
#ifndef _SCREEN_GOON_H_
#define _SCREEN_SCREEN_H_

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


extern void goon_button(uint16_t scrid, uint16_t ctrlid, uint8_t state);


#ifdef __cplusplus
}
#endif


#endif /* _SCREEN_GOON_H_ */
