/**
  *****************************************************************************
  * @file    screen_home.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/

#ifndef __SCREEN_HOME_H__
#define __SCREEN_HOME_H__

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

extern void screen_home_enter(void);
extern void home_button(uint16_t scrid, uint16_t ctrlid, uint8_t state);
extern void home_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str);

#ifdef __cplusplus
}
#endif

#endif /* __SCREEN_HOME_H__ */

