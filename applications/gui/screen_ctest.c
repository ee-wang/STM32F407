/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-29     YAYA        the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "screen_ctest.h"
#include "screen_comm.h"
#include "current_regulate.h"
#include "string.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define CTEST_CID_BTN_START 3
#define CTEST_CID_BTN_STOP  4

#define CTEST_CID_TXT_TIP_1 9
#define CTEST_CID_TXT_TIP_2 10
#define CTEST_CID_TXT_TIP_3 11
#define CTEST_CID_TXT_TIP_4 12

#define CTEST_CID_RTC   2

#define CTEST_CID_RECORD    5
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
int temp_display1 = 2;      //冷态校验页面温度显示标志位

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void record1_get(int num)
{
    switch(num)
    {
    case 1:{

        if(finish_cold1 == true){
            struct str result1 ;
            char record1[30];
            strcpy(result1.number,"1");
            strcpy(result1.name, gui_name1);
            strcpy(result1.time,gui_r1_cold_time);
            strcpy(result1.state,"finish");
            strcat(record1, result1.number);
            strcat(record1,";");
            strcat(record1, result1.name);
            strcat(record1,";");
            strcat(record1, result1.time);
            strcat(record1,";");
            strcat(record1, result1.state);
            strcat(record1,";");
            rt_kprintf("cold_record = %s\n",record1);
            Record_Add(SCR_ID_CTST, CTEST_CID_RECORD, record1);
            StopTimer(SCR_ID_CTST, CTEST_CID_RTC);
            finish_cold1 = false;
        }
     }break;
     case 2:{

     }break;
     case 3:{

     }break;
     case 4:{
         if(finish_cold1 == true){
             struct str result1 ;
             char record1[30];
             strcpy(result1.number,"1");
             strcpy(result1.name, gui_name1);
             strcpy(result1.time,gui_r1_cold_time);
             strcpy(result1.state,"finish");
             strcat(record1, result1.number);
             strcat(record1,";");
             strcat(record1, result1.name);
             strcat(record1,";");
             strcat(record1, result1.time);
             strcat(record1,";");
             strcat(record1, result1.state);
             strcat(record1,";");
             Record_Add(SCR_ID_CTST, CTEST_CID_RECORD, record1);
             finish_cold1 = false;
         }
         if(finish_cold2 == true){
             struct str result2 ;
             char record2[30];
             strcpy(result2.number,"2");
             strcpy(result2.name, gui_name2);
             strcpy(result2.time,gui_r2_cold_time);
             strcpy(result2.state,"finish");
             strcat(record2, result2.number);
             strcat(record2,";");
             strcat(record2, result2.name);
             strcat(record2,";");
             strcat(record2, result2.time);
             strcat(record2,";");
             strcat(record2, result2.state);
             strcat(record2,";");
             Record_Add(SCR_ID_CTST, CTEST_CID_RECORD, record2);
             finish_cold2 = false;
         }
         if(finish_cold3 == true){
             struct str result3 ;
             char record3[30];
             strcpy(result3.number,"3");
             strcpy(result3.name, gui_name3);
             strcpy(result3.time,gui_r3_cold_time);
             strcpy(result3.state,"finish");
             strcat(record3, result3.number);
             strcat(record3,";");
             strcat(record3, result3.name);
             strcat(record3,";");
             strcat(record3, result3.time);
             strcat(record3,";");
             strcat(record3, result3.state);
             strcat(record3,";");
             Record_Add(SCR_ID_CTST, CTEST_CID_RECORD, record3);
             finish_cold3 = false;
         }
         if(finish_cold4 == true){
             struct str result4 ;
             char record4[30];
             strcpy(result4.number,"4");
             strcpy(result4.name, gui_name4);
             strcpy(result4.time,gui_r4_cold_time);
             strcpy(result4.state,"finish");
             strcat(record4, result4.number);
             strcat(record4,";");
             strcat(record4, result4.name);
             strcat(record4,";");
             strcat(record4, result4.time);
             strcat(record4,";");
             strcat(record4, result4.state);
             strcat(record4,";");
             Record_Add(SCR_ID_CTST, CTEST_CID_RECORD, record4);
             finish_cold4 = false;
         }

     }break;
     }
}

/* Public functions ----------------------------------------------------------*/
void screen_ctest_enter(void)
{
    if (temp_display1 == 0||temp_display1 == 2){
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_4 , 0);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_1 , 0);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_2 , 0);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_3 , 0);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_4 , 1);
    }else{
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_1 , 1);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_2 , 1);
        SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_3 , 1);
    }

}

void ctest_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    switch (ctrlid)
        {
            case CTEST_CID_BTN_START:{
                SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_4 , 0);
                SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_1 , 1);
                SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_2 , 1);
                SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_3 , 1);
                StartTimer(scrid, CTEST_CID_RTC);
//                rt_kprintf("gui_current=%d\n",gui_current);
//                rt_kprintf("num=%d\n",num);
//                rt_kprintf("type=%d\n",type);
                process_thread_init();
                temp_display1 = 1;

            }break;

            case CTEST_CID_BTN_STOP  :{
                if(state == 1)
                {
                    StopTimer(scrid, CTEST_CID_RTC);
                    SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_1 , 0);
                    SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_2 , 0);
                    SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_3 , 0);
                    SetControlVisiable(g_cur_screen.scrid, CTEST_CID_TXT_TIP_4 , 1);
                    end_process();
                    temp_display1 = 0;
                }
            }break;
        }
}
