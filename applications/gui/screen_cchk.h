/**
  *****************************************************************************
  * @file    screen_cchk.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03-10
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/

#ifndef __SCREEN_CCHK_H__
#define __SCREEN_CCHK_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
extern float current;
extern char gui_file_current[6];

/* Public function prototypes ------------------------------------------------*/

extern void screen_cchk_enter(void);
extern void cchk_button(uint16_t scrid, uint16_t ctrlid, uint8_t state);
extern void cchk_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str);
extern void cchk_menu(uint16_t scrid, uint16_t ctrlid, uint8_t item, uint8_t state);

#ifdef __cplusplus
}
#endif

#endif /* __SCREEN_CCHK_H__ */

