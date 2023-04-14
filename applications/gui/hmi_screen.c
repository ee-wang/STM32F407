/**
  *****************************************************************************
  * @file    hmi_screen.c
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
#include "hmi_screen.h"
#include "screen_comm.h"
#include  "temp_ctrl.h"

/* Private typedef -----------------------------------------------------------*/

#pragma pack(push)
#pragma pack(1)                           //���ֽڶ���
typedef struct{
    uint8_t  cmd_head;                    //֡ͷ
    uint8_t  cmd_type;                    //��������(UPDATE_CONTROL)
    uint8_t  ctrl_msg;                    //CtrlMsgType-ָʾ��Ϣ������
    uint16_t screen_id;                   //������Ϣ�Ļ���ID
    uint16_t control_id;                  //������Ϣ�Ŀؼ�ID
    uint8_t  control_type;                //�ؼ�����
    uint8_t  param[256];                  //�ɱ䳤�Ȳ��������256���ֽ�
    uint8_t  cmd_tail[4];                 //֡β
} CTRL_MSG, *PCTRL_MSG;
#pragma pack(pop)

#define NOTIFY_TOUCH_PRESS         0X01  //����������֪ͨ
#define NOTIFY_TOUCH_RELEASE       0X03  //�������ɿ�֪ͨ
#define NOTIFY_WRITE_FLASH_OK      0X0C  //дFLASH�ɹ�
#define NOTIFY_WRITE_FLASH_FAILD   0X0D  //дFLASHʧ��
#define NOTIFY_READ_FLASH_OK       0X0B  //��FLASH�ɹ�
#define NOTIFY_READ_FLASH_FAILD    0X0F  //��FLASHʧ��
#define NOTIFY_MENU                0X14  //�˵��¼�֪ͨ
#define NOTIFY_TIMER               0X43  //��ʱ����ʱ֪ͨ
#define NOTIFY_CONTROL             0XB1  //�ؼ�����֪ͨ
#define NOTIFY_READ_RTC            0XF7  //��ȡRTCʱ��
#define MSG_GET_CURRENT_SCREEN     0X01  //����ID�仯֪ͨ
#define MSG_GET_DATA               0X11  //�ؼ�����֪ͨ
#define NOTIFY_HandShake           0X55  //����֪ͨ

/* Public macro --------------------------------------------------------------*/

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //�ӻ�����ȡ16λ����
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)\
                      |(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])    //�ӻ�����ȡ32λ����

enum CtrlType{
    kCtrlUnknown = 0x0,
    kCtrlButton = 0x10,                   //��ť
    kCtrlText,                            //�ı�
    kCtrlProgress,                        //������
    kCtrlSlider,                          //������
    kCtrlMeter,                           //�Ǳ�
    kCtrlDropList,                        //�����б�
    kCtrlAnimation,                       //����
    kCtrlRTC,                             //ʱ����ʾ
    kCtrlGraph,                           //����ͼ�ؼ�
    kCtrlTable,                           //���ؼ�
    kCtrlMenu,                            //�˵��ؼ�
    kCtrlSelector,                        //ѡ��ؼ�
    kCtrlQRCode,                          //��ά��
};

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
//int flag = 1;
SCREEN_t g_cur_screen;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static void hmi_screen_enter(uint16_t scrid)
{
    if (g_cur_screen.scrid != scrid)
    {
        if (g_cur_screen.pScreenExit)
        {
            g_cur_screen.pScreenExit();
        }
    }
    
    memset((void *)&g_cur_screen, 0, sizeof(SCREEN_t));         //���ҳ����Ϣ
    
    g_cur_screen.scrid = scrid;
    
    switch (scrid)
    {
        case SCR_ID_HOME_1:{
            screen_home_enter();
        }break;
        
        case SCR_ID_CCHK:{
            screen_cchk_enter();
        }break;
        case SCR_ID_CTST:{
            screen_ctest_enter();
        }break;
        case SCR_ID_HTST:{
            screen_htest_enter();
        }break;
        case SCR_ID_FILE_1:{}break;

        default:{
            //δ֪ҳ�� �����⴦��
            //TODO
        }break;
    }
}

static void NotifyButton(uint16_t scrid, uint16_t ctrlid, uint8_t state)
{

    switch (scrid)
       {
           case SCR_ID_HOME_1:{
               home_button(scrid, ctrlid, state) ;
           }break;

           case SCR_ID_CCHK:{
               cchk_button(scrid, ctrlid, state);
           }break;
           case SCR_ID_CTST:{
               ctest_button(scrid, ctrlid, state);
           }break;
           case SCR_ID_HTST:{

           }break;
           case SCR_ID_GOON:{
               goon_button(scrid, ctrlid, state);
           }break;
           case SCR_ID_FILE_1:{
               file_button(scrid, ctrlid, state);
           }break;
           case SCR_ID_FILE_2:{

           }break;
           case SCR_ID_NAME:{
               name_button(scrid, ctrlid, state);
           }break;

           default:{

               //δ֪ҳ�� �����⴦��
               //TODO
           }break;
       }
}

static void NotifyText(uint16_t scrid, uint16_t ctrlid, uint8_t *str)
{
    switch (scrid)
           {
               case SCR_ID_HOME_1:{
                   home_text(scrid, ctrlid, str) ;
               }break;

               case SCR_ID_CCHK:{
                   cchk_text(scrid, ctrlid, str);
               }break;
               case SCR_ID_FILE_1:{

               }break;
               case SCR_ID_FILE_2:{

               }break;
               case SCR_ID_NAME:{
                   name_text(scrid, ctrlid, str);
               }break;


               default:{

                   //δ֪ҳ�� �����⴦��
                   //TODO
               }break;
           }
}

static void NotifyMenu(uint16_t scrid, uint16_t ctrlid, uint8_t item, uint8_t state)
{
    switch(scrid)
    {
    case SCR_ID_CCHK:{
        cchk_menu(scrid, ctrlid, item, state);
    }break;
    }
}
void UpdateUI()
{
    if(g_cur_screen.scrid == SCR_ID_HOME_1)
    {
        SetTextFloat(g_cur_screen.scrid, 18, Temp, 1, 1);
//      rt_thread_mdelay(20);
    }
    else if(g_cur_screen.scrid == SCR_ID_CTST)
    {

        SetTextFloat(g_cur_screen.scrid, 10, Temp, 1, 1);

        record1_get(num);

    }
    else if(g_cur_screen.scrid == SCR_ID_HTST)
    {
        SetTextFloat(g_cur_screen.scrid, 10, Temp, 1, 1);
        record2_get(num);
    }
    else if(g_cur_screen.scrid == SCR_ID_FILE_1)
    {
        filerecord_get(num);
    }
    else if(g_cur_screen.scrid == SCR_ID_FILE_2)
    {
        filesrecord_get(num);
    }
}

void hmi_screen_prase(uint8_t *arg)
{
    PCTRL_MSG msg = (PCTRL_MSG)arg;
    uint8_t cmd_type = msg->cmd_type;                            //ָ������
    uint8_t ctrl_msg = msg->ctrl_msg;                            //��Ϣ������
    uint8_t control_type = msg->control_type;                    //�ؼ�����
    uint16_t screen_id = PTR2U16(&msg->screen_id);               //����ID
    uint16_t control_id = PTR2U16(&msg->control_id);             //�ؼ�ID
    uint32_t value = PTR2U32(msg->param);                        //��ֵ
    SCREEN_t *pCurScreen = &g_cur_screen;

    switch (cmd_type)
    {
    case NOTIFY_TOUCH_PRESS:                                     //����������
    case NOTIFY_TOUCH_RELEASE:                                   //�������ɿ�
        if (pCurScreen->NotifyTouchXY)
        {
            uint8_t *bptr = (uint8_t *)&msg;
            pCurScreen->NotifyTouchXY(cmd_type, PTR2U16(bptr + 2), PTR2U16(bptr + 4));
        }
        break;
/*
    case NOTIFY_WRITE_FLASH_OK      : NotifyWriteFlash(1);break;
    case NOTIFY_WRITE_FLASH_FAILD   : NotifyWriteFlash(0);break;
    case NOTIFY_READ_FLASH_OK       : NotifyReadFlash(1, cmd_buffer + 2, size - 6);break;
    case NOTIFY_READ_FLASH_FAILD    : NotifyReadFlash(0, 0, 0);break;
    case NOTIFY_READ_RTC:
        NotifyReadRTC(cmd_buffer[2], cmd_buffer[3], cmd_buffer[4], cmd_buffer[5], cmd_buffer[6], cmd_buffer[7], cmd_buffer[8]);
        break;
*/
    case NOTIFY_CONTROL: {
        if (ctrl_msg == MSG_GET_CURRENT_SCREEN)                   //����ID�仯֪ͨ
        {
            hmi_screen_enter(screen_id);                         //�����л������ĺ���
        }
        else
        {
            switch (control_type)
            {
            case kCtrlButton:                                     //��ť�ؼ�
//                if (pCurScreen->NotifyButton)
//                {
//                    pCurScreen->NotifyButton(screen_id, control_id, msg->param[1]);
//                }
                NotifyButton(screen_id, control_id, msg->param[1]);
                break;
            case kCtrlText:                                       //�ı��ؼ�
//                if (pCurScreen->NotifyText)
//                {
//                    pCurScreen->NotifyText(screen_id, control_id, msg->param);
//                }
                NotifyText(screen_id, control_id, msg->param);
                break;
            case kCtrlProgress:                                   //�������ؼ�
//                if (pCurScreen->NotifyProgress)
//                {
//                    pCurScreen->NotifyProgress(screen_id, control_id, value);
//                }
//                NotifyProgress(screen_id, control_id, value);
                break;
            case kCtrlSlider:                                     //�������ؼ�
//                if (pCurScreen->NotifySlider)
//                {
//                    pCurScreen->NotifySlider(screen_id, control_id, value);
//                }
//                  NotifySlider(screen_id, control_id, value);
                break;
            case kCtrlMeter:                                      //�Ǳ�ؼ�
//                if (pCurScreen->NotifyMeter)
//                {
//                    pCurScreen->NotifyMeter(screen_id, control_id, value);
//                }
//                NotifyMeter(screen_id, control_id, value);
                break;
            case kCtrlMenu:                                       //�˵��ؼ�
//                if (pCurScreen->NotifyMenu)
//                {
//                    pCurScreen->NotifyMenu(screen_id, control_id, msg->param[0], msg->param[1]);
//                }
                NotifyMenu(screen_id, control_id, msg->param[0], msg->param[1]);
                break;
            case kCtrlSelector:                                   //ѡ��ؼ�
//                if (pCurScreen->NotifySelector)
//                {
//                    pCurScreen->NotifySelector(screen_id, control_id, msg->param[0]);
//                }

                break;
/*
            case kCtrlRTC:                                        //����ʱ�ؼ�
                if (pCurScreen->NotifyTimer)
                {
                    pCurScreen->NotifyTimer(screen_id, control_id);
                }
                break;
*/
            }
        }
        break;
    }
    //case NOTIFY_HandShake:break;
    }
}

/* Public functions ----------------------------------------------------------*/

void SendChar(uint8_t ch)
{
    hmi_tx_byte(ch);
}

/*
ҳ����ת
*/
void hmi_screen_goto(uint16_t scrid)
{
    SetScreen(scrid);
}

void hmi_screen_init(void)
{
    hmi_screen_goto(SCR_ID_HOME_1);
}INIT_APP_EXPORT(hmi_screen_init);

