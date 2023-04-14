/**
  *****************************************************************************
  * @file    screen_home.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "screen_home.h"
#include "screen_comm.h"
#include "temp_ctrl.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define HOME_CID_BTN_RTC               1
#define HOME_CID_BTN_COOLCHK           2
#define HOME_CID_BTN_HOTCHK            3
#define HOME_CID_BTN_REPORT            4
#define HOME_CID_BTN_SETUP            12

#define HOME_CID_TXT_TEMP             18
#define HOME_CID_TXT_VER              17

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void screen_home_enter(void)
{

    SetTextFloat(g_cur_screen.scrid, HOME_CID_TXT_TEMP, Temp, 2, 1);
}

void home_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
//    if (state != 0)
//        return;

    switch (ctrlid)
    {
    case HOME_CID_BTN_RTC    :{}break;
    case HOME_CID_BTN_COOLCHK:{

    }break;
    case HOME_CID_BTN_HOTCHK :{}break;
    case HOME_CID_BTN_REPORT :{}break;

    case HOME_CID_BTN_SETUP  :{
        hmi_screen_goto(SCR_ID_OPTION);
    }break;
    }
}

void home_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str)
{
    switch (ctrlid)
    {
    case HOME_CID_TXT_TEMP    :{}break;
    case HOME_CID_TXT_VER:{
    }break;
    }
}

