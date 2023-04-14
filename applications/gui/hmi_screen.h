/**
  *****************************************************************************
  * @file    hmi_screen.h
  * @author  ZhuQW
  * @version V1.0.0
  * @date    2023-03
  * @brief   
  *           
  *****************************************************************************
  * @note    
  *****************************************************************************
**/

#ifndef __HMI_SCREEN_H__
#define __HMI_SCREEN_H__

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#include "hmi_driver.h"

/* Public typedef ------------------------------------------------------------*/

typedef struct {
    uint16_t scrid;

    void (*pScreenExit)(void);          //ҳ����ת�˳���ǰҳ�洦��
    
    /*!
    *  \brief  ���������¼���Ӧ
    *  \param press 1���´�������3�ɿ�������
    *  \param x x����
    *  \param y y����
    */
    void (*NotifyTouchXY)(uint8_t press, uint16_t x, uint16_t y);

    /*!
    *  \brief  ��ť�ؼ�֪ͨ
    *  \details  ����ť״̬�ı�(�����GetControlValue)ʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param state ��ť״̬��0����1����
    */
    void (*NotifyButton)(uint16_t scrid, uint16_t ctrlid, uint8_t state);
    /*!
    *  \brief  �ı��ؼ�֪ͨ
    *  \details  ���ı�ͨ�����̸���(�����GetControlValue)ʱ��ִ�д˺���
    *  \details  �ı��ؼ����������ַ�����ʽ�·���MCU������ı��ؼ������Ǹ���ֵ��
    *  \details  ����Ҫ�ڴ˺����н��·��ַ�������ת�ظ���ֵ��
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param str �ı��ؼ�����
    */
    void (*NotifyText)(uint16_t scrid, uint16_t ctrlid, uint8_t *str);

    /*!
    *  \brief  �������ؼ�֪ͨ
    *  \details  ����GetControlValueʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param value ֵ
    */
    void (*NotifyProgress)(uint16_t scrid, uint16_t ctrlid, uint32_t val);

    /*!
    *  \brief  �������ؼ�֪ͨ
    *  \details  ���������ı�(�����GetControlValue)ʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param value ֵ
    */
    void (*NotifySlider)(uint16_t scrid, uint16_t ctrlid, uint32_t val);

    /*!
    *  \brief  �Ǳ�ؼ�֪ͨ
    *  \details  ����GetControlValueʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param value ֵ
    */
    void (*NotifyMeter)(uint16_t scrid, uint16_t ctrlid, uint32_t val);

    /*!
    *  \brief  �˵��ؼ�֪ͨ
    *  \details  ���˵���»��ɿ�ʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param item �˵�������
    *  \param state ��ť״̬��0�ɿ���1����
    */
    void (*NotifyMenu)(uint16_t scrid, uint16_t ctrlid, uint8_t item, uint8_t state);

    /*!
    *  \brief  ѡ��ؼ�֪ͨ
    *  \details  ��ѡ��ؼ��仯ʱ��ִ�д˺���
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    *  \param item ��ǰѡ��
    */
    void (*NotifySelector)(uint16_t scrid, uint16_t ctrlid, uint8_t item);
    /*!
    *  \brief  ��ʱ����ʱ֪ͨ����
    *  \param scrid ����ID
    *  \param ctrlid �ؼ�ID
    */
    void (*NotifyTimer)(uint16_t scrid, uint16_t ctrlid);
}SCREEN_t;


/* Public define -------------------------------------------------------------*/


/* Public typedef ------------------------------------------------------------*/
/* Public define -------------------------------------------------------------*/
/* Public macro --------------------------------------------------------------*/
/* Public variables ----------------------------------------------------------*/
//extern int flag;
extern SCREEN_t g_cur_screen;

/* Public function prototypes ------------------------------------------------*/

extern void hmi_screen_prase(uint8_t *arg);

extern void hmi_screen_goto(uint16_t scrid);

extern void UpdateUI();

#ifdef __cplusplus
}
#endif

#endif /* __HMI_SCREEN_H__ */

