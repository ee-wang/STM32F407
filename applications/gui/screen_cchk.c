/**
  *****************************************************************************
  * @file    screen_cchk.c
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03-10
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/
  
/* Includes ------------------------------------------------------------------*/
#include "screen_cchk.h"
#include "screen_comm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define CCHK_CID_BTN_RETURN             1
#define CCHK_CID_BTN_MODE               2
#define CCHK_CID_BTN_START              7

#define CCHK_CID_TXT_ELE                9

#define CCHK_CID_MENU_MOD               4
#define CCHK_CID_MENU_CNT               10

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
float current;
char gui_file_current[6];
//int model1;
//int count1;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void change_mode(uint8_t state)
{
    if(state==1){
        temp_command_thread();
    }else{
        end_tempcmd();
    }
}
/* Public functions ----------------------------------------------------------*/

void screen_cchk_enter(void)
{
//    g_cur_screen.NotifyButton = cchk_button;

}

void cchk_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    switch (ctrlid)
    {
       case CCHK_CID_BTN_MODE  :{
            //change_mode(state > 0 ? 1 : 0);
           change_mode(state);
        }break;
       case CCHK_CID_BTN_START:{
            if(state == 1)
            {
                if(name_flag!=1)
                {
                    hmi_screen_goto(SCR_ID_NAMEW);
                }
            }

       }break;
    }
}

void cchk_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str)
{
    float value = 0;
    sscanf(str,"%f",&value);
    switch (ctrlid)
    {
        case CCHK_CID_TXT_ELE:{
            gui_current = (int)(value*1000);
            sscanf(&value,"%s",gui_file_current);

        }break;
    }
}

void cchk_menu(uint16_t scrid, uint16_t ctrlid, uint8_t item, uint8_t state)
{
    switch(ctrlid)
    {
    case CCHK_CID_MENU_MOD:{
        //model1 = item;
        type = item+1;
    }break;
    case CCHK_CID_MENU_CNT:{
        //count1 = item;
        num = item+1;
    }break;
    }
}
