/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-05     YAYA       the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "screen_files.h"
#include "screen_comm.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define NAME_CID_BTN_YES 2

#define NAME_CID_TXT_NUMBER_1   3
#define NAME_CID_TXT_NUMBER_2   4
#define NAME_CID_TXT_NUMBER_3   5
#define NAME_CID_TXT_NUMBER_4   6
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char name1[5];
char name2[5];
char name3[5];
char name4[5];
char gui_name1[5];
char gui_name2[5];
char gui_name3[5];
char gui_name4[5];
int name_flag;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void name_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    if(ctrlid == NAME_CID_BTN_YES)
    {
        if(state == 1){
            sscanf(name1,"%s",gui_name1);
            sscanf(name2,"%s",gui_name2);
            sscanf(name3,"%s",gui_name3);
            sscanf(name4,"%s",gui_name4);
            name_flag = 1;
            rt_kprintf("gui_name1=%s\n",gui_name1);
            rt_kprintf("gui_name2=%s\n",gui_name2);
            rt_kprintf("gui_name3=%s\n",gui_name3);
            rt_kprintf("gui_name4=%s\n",gui_name4);
        }
    }
}

void name_text(uint16_t scrid, uint16_t ctrlid , uint8_t *str)
{

    switch (ctrlid)
    {
        case NAME_CID_TXT_NUMBER_1:{
            sscanf(str,"%s",name1);

        }break;
        case NAME_CID_TXT_NUMBER_2:{
            sscanf(str,"%s",name2);

        }break;
        case NAME_CID_TXT_NUMBER_3:{
            sscanf(str,"%s",name3);

        }break;
        case NAME_CID_TXT_NUMBER_4:{
            sscanf(str,"%s",name4);

        }break;
    }
}
