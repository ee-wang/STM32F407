/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-03-29     YAYA       the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "screen_files.h"
#include "screen_comm.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FILES_CID_BTN_EXPORT 3

#define FILES_CID_TXT_CONCLUSION_1   5
#define FILES_CID_TXT_CONCLUSION_2   6
#define FILES_CID_TXT_CONCLUSION_3   7
#define FILES_CID_TXT_CONCLUSION_4   8

#define FILES_CID_RECORD 4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct files
{
    char file2_sn[2];
    char file2_name[256];
    char coldtime[5];
    char hottime[5];
};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void screen_files_enter(void)
{

}

void files_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    switch (ctrlid)
        {
            case FILES_CID_BTN_EXPORT:{

                                                //数据导出
            }break;
        }
}

void filesrecord_get(int num)
{
    switch(num)
    {
    case 1:{
        if(files_check == true)
        {
            struct files files_result1;
            char files_record1[20];
            strcpy(files_result1.file2_sn,"1");
            strcpy(files_result1.file2_name,gui_name1);
            strcpy(files_result1.coldtime,gui_r1_cold_time);
            strcpy(files_result1.hottime,gui_r1_hot_time);
            strcat(files_record1, files_result1.file2_sn);
            strcat(files_record1,";");
            strcat(files_record1, files_result1.file2_name);
            strcat(files_record1,";");
            strcat(files_record1, files_result1.coldtime);
            strcat(files_record1,";");
            strcat(files_record1, files_result1.hottime);
            strcat(files_record1,";");
            Record_Add(SCR_ID_FILE_2, FILES_CID_RECORD, files_record1);
            SetTextValue(SCR_ID_FILE_2, FILES_CID_TXT_CONCLUSION_1, gui_r1_ans);
            files_check = false;
            rt_kprintf("files_record1 = %s\n",files_record1);
        }

    }break;
    case 2:{

    }break;
    case 3:{

    }break;
    case 4:{

    }break;

    }
}

