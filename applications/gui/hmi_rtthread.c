/**
  *****************************************************************************
  * @file    hmi_rtthread.c
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
#include "hmi_rtthread.h"
#include "hmi_screen.h"
#include "process.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define HMI_UART_NAME       "uart3"

#define DACAI_CMD_HEAD      (0XEE)                        //֡ͷ
#define DACAI_CMD_TAIL      (0XFFFCFFFF)                  //֡β

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static rt_device_t hmi_serial;
static struct rt_semaphore hmi_sem_rx;
static rt_mailbox_t hmi_mb_rx;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

static rt_err_t hmi_rx_callback(rt_device_t dev, rt_size_t size)
{
    /* ���ڽ��յ����ݺ�����жϣ����ô˻ص�������Ȼ���ͽ����ź��� */
    rt_sem_release(&hmi_sem_rx);
    return RT_EOK;
}

static void hmi_rx_thread(void *parameter)
{
    uint32_t rx_state = 0;          //����֡β���״̬
    uint16_t rx_pos = 0;            //��ǰָ��ָ��λ��
    char ch;
    uint8_t *pbuf = rt_malloc(CMD_MAX_SIZE*2);
    uint8_t *ping, *pang, *bptr;

    
    if (pbuf == RT_NULL)
    {
        rt_kprintf("hmi malloc fail\n");
        return;
    }

    ping = pbuf;
    pang = pbuf + CMD_MAX_SIZE;
    bptr = ping;

    while (1)
    {
        /* �Ӵ��ڶ�ȡһ���ֽڵ����ݣ�û�ж�ȡ����ȴ������ź��� */
        while (rt_device_read(hmi_serial, -1, &ch, 1) != 1)
        {
            rt_sem_take(&hmi_sem_rx, RT_WAITING_FOREVER);
        }
        
        //rt_kprintf("%c", ch);

         if (rx_pos == 0 && ch != DACAI_CMD_HEAD)        //ָ���һ���ֽڱ�����֡ͷ����������
         {
             continue;
         }
        
         if (rx_pos < CMD_MAX_SIZE)                      //��ֹ���������
         {
             bptr[rx_pos++] = ch;
         }

         rx_state = ((rx_state << 8) | ch);              //ƴ�����4���ֽڣ����һ��32λ����
         if (rx_state == DACAI_CMD_TAIL)                 //���4���ֽ���֡βƥ�䣬�õ�����֡
         {
             int size = rx_pos;                          //ָ���ֽڳ���
             rx_state = 0;                               //���¼��֡β��
             rx_pos = 0;                                 //��λָ��ָ��

             #if (CRC16_ENABLE)
             if (!CheckCRC16(bptr + 1, size - 5))        //ȥ��ָ��ͷβEE��βFFFCFFFF����5���ֽڣ�ֻ�������ݲ���CRC
                 continue;
             size -= 2;                                  //ȥ��CRC16��2�ֽڣ�
             #endif
            
             if (rt_mb_send(hmi_mb_rx, (rt_ubase_t)bptr) == RT_EOK)
             {
                 bptr = (bptr == ping) ? pang : ping;
             }
         }
         rt_thread_mdelay(10);
    }

}

static void hmi_rx_prase(void *arg)
{
    uint8_t *msg = RT_NULL;

    while (1)
    {
        if (rt_mb_recv(hmi_mb_rx, (rt_ubase_t *)&msg, RT_WAITING_FOREVER) == RT_EOK)
        {
            hmi_screen_prase((void *)msg);
        }
        rt_thread_mdelay(10);
    }
}

static void hmi_rx_update(void *data)
{
   while(1)
   {
       UpdateUI();
       rt_thread_mdelay(20);
   }
}

static void flag_check(void *flag)
{
    while(1)
    {
        if(hot_switch == true)
        {
            hmi_screen_goto(6);
            SetControlVisiable(g_cur_screen.scrid, 12, 0);
            StopTimer(6, 2);
            StartTimer(6, 2);
            hot_switch = false;

        }
        rt_thread_mdelay(20);
    }
}

int hmi_thread_init(void)
{
    rt_thread_t thread1;
    rt_thread_t thread2;
    rt_thread_t thread3;
    rt_thread_t thread4;

    hmi_serial = rt_device_find(HMI_UART_NAME);
    if (!hmi_serial)
    {
        return RT_ERROR;
    }

    hmi_mb_rx = rt_mb_create("hmimb", 4, RT_IPC_FLAG_FIFO);
    if (hmi_mb_rx == RT_NULL)
    {
        rt_kprintf("hmi_mb_rx create fail\n");
        return RT_ERROR;
    }

    thread1 = rt_thread_create("hmiprase", hmi_rx_prase, RT_NULL, 2048, 11, 10);
    if (thread1 != RT_NULL)
    {
        rt_thread_startup(thread1);
    }
    else
    {
        rt_kprintf("hmiprase thread create fail\n");
        return RT_ERROR;
    }

    rt_sem_init(&hmi_sem_rx, "hmi_sem_rx", 0, RT_IPC_FLAG_FIFO);

    /* �Զ�д���жϽ��շ�ʽ�򿪴����豸 */
    rt_device_open(hmi_serial, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    /* ���ý��ջص����� */
    rt_device_set_rx_indicate(hmi_serial, hmi_rx_callback);

    thread2 = rt_thread_create("hmirx", hmi_rx_thread, RT_NULL, 1024, 11, 10);
    if (thread2 != RT_NULL)
    {
        rt_thread_startup(thread2);
    }
    else
    {
        rt_kprintf("hmirx thread create fail\n");
        return RT_ERROR;
    }
    thread3 = rt_thread_create("hmiupdate", hmi_rx_update, RT_NULL, 4096, 12, 10);
    if (thread3 != RT_NULL)
    {
        rt_thread_startup(thread3);
    }
    else
    {
        rt_kprintf("hmiupdate thread create fail\n");
        return RT_ERROR;
    }
    thread4 = rt_thread_create("flagcheck", flag_check, RT_NULL, 1024, 13, 10);
    if (thread4 != RT_NULL)
    {
        rt_thread_startup(thread4);
    }
    else
    {
        rt_kprintf("flagcheck thread create fail\n");
        return RT_ERROR;
    }
    return RT_EOK;
}INIT_ENV_EXPORT(hmi_thread_init);


/* Public functions ----------------------------------------------------------*/

void hmi_tx_byte(uint8_t ch)
{
    rt_device_write(hmi_serial, 0, &ch, 1);
}


