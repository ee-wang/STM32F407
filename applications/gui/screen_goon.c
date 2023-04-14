/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-04-03     YAYA       the first version
 */
/* Includes ------------------------------------------------------------------*/
#include "screen_goon.h"
#include "screen_comm.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define GOON_CID_BTN_YES             3
#define GOON_CID_BTN_NO              2


/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Public functions ----------------------------------------------------------*/
void goon_button(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{
    switch (ctrlid)
    {
        case GOON_CID_BTN_NO:{
            hmi_screen_goto(SCR_ID_CCHK);
//            answer = 2;

        }break;

        case GOON_CID_BTN_YES  :{

            hmi_screen_goto(SCR_ID_HTST);

        }break;
    }
}
