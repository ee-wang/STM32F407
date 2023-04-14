/**
  *****************************************************************************
  * @file    screen_comm.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/

#ifndef __SCREEN_COMM_H__

#define __SCREEN_COMM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "hmi_screen.h"

/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
/* Public function prototypes ------------------------------------------------*/

/* DACAI VirtualTFT SCREEN ID */
enum {
    SCR_ID_HOME_1 = 0,
    SCR_ID_HOME_2 = 1,
    SCR_ID_CCHK = 2,        //��̬У��
    SCR_ID_FILE_1 = 3,
    SCR_ID_FILE_2 = 4,
    SCR_ID_CTST = 5,
    SCR_ID_HTST = 6,
    SCR_ID_OPTION = 8,
    SCR_ID_GOON = 17,
    SCR_ID_NAME = 20,
    SCR_ID_NAMEW = 21,
};


/*----------------------------------------------------------------------------*/
#include "temp_ctrl.h"
#include "process.h"
#include "screen_home.h"
#include "screen_cchk.h"
#include "screen_ctest.h"
#include "screen_htest.h"
#include "screen_goon.h"
#include "screen_namewrt.h"
#include <gui/screen_file.h>
#include <gui/screen_files.h>



//....



#ifdef __cplusplus
}
#endif

#endif /* __SCREEN_COMM_H__ */

