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
#include "screen_file.h"
#include "screen_comm.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define FILE_CID_BTN_EXPORT 3

#define FILE_CID_TXT_CONCLUSION_1   5
#define FILE_CID_TXT_CONCLUSION_2   6
#define FILE_CID_TXT_CONCLUSION_3   7
#define FILE_CID_TXT_CONCLUSION_4   8

#define FILE_CID_RECORD 4
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

struct file
{
    char file1_sn[2];
    char file1_name[256];
    char file1_current[7];
    char file1_no[10];
    char file1_nc[10];
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/

void filerecord_get(int num)
{
    switch(num)
    {
    case 1:{
        if(file_check == true){
                struct file file_result1;
                char file_record1[25];
                strcpy(file_result1.file1_sn,"1");
                strcpy(file_result1.file1_name, gui_name1);
                strcpy(file_result1.file1_current,gui_file_current);
                strcpy(file_result1.file1_no, gui_r1_no);
                strcpy(file_result1.file1_nc, gui_r1_nc);
                strcat(file_record1, file_result1.file1_sn);
                strcat(file_record1,";");
                strcat(file_record1, file_result1.file1_name);
                strcat(file_record1,";");
                strcat(file_record1, file_result1.file1_current);
                strcat(file_record1,";");
                strcat(file_record1, file_result1.file1_no);
                strcat(file_record1,"/");
                strcat(file_record1, file_result1.file1_nc);
                strcat(file_record1,";");
                Record_Add(SCR_ID_FILE_1, FILE_CID_RECORD, file_record1);
                SetTextValue(SCR_ID_FILE_1, FILE_CID_TXT_CONCLUSION_1, gui_r1_ans);
                file_check = false;
                rt_kprintf("file_record1 = %s\n", file_record1);
            }
        }break;
     }

}

void screen_file_enter(void)
{

}


void file_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    switch (ctrlid)
        {
            case FILE_CID_BTN_EXPORT:{

                                                //数据导出
            }break;
        }
}


