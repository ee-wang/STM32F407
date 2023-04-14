 /*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-29     YAYA       the first version
 */
#ifndef __SCREEN_FLIE_H__
#define __SCREEN_FLIE_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern int button;
/* Public function prototypes ------------------------------------------------*/
extern void filerecord_get(int);
extern void screen_file_enter(void);


#ifdef __cplusplus
}
#endif


#endif /* __SCREEN_FLIE_H__ */
