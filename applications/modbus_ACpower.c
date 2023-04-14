/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2023-01-10     wyk       the first version
 */

/*         重要说明！！！！！！！！！
 int modbus_read_registers(modbus_t *ctx, int addr, int nb, uint16_t *dest);
 本函数的功能是用于读取远程设备的add地址开始的nb位（保持寄存器）的状态，读取结果以uint16_t位形式存储在dest数组中；
 对应于Modbus功能码0x03.
 成功时返回读取输入位的数目即nb，失败时返回-1.

 int modbus_write_registers(modbus_t *ctx, int addr, int nb, const uint16_t *src);
 本函数的功能是用于将数组src中的内容写到远程设备addr地址处的一组nb个寄存器；
 对应于Modbus功能码0x10.
 成功时返回写入寄存器个数即nb，失败时返回-1.

 写单个线圈或者单个离散量(功能码 0x5)
 int modbus_write_bit(modbus_t *ctx, int coil_addr, int status);
 *
 */
#include "modbus_ACpower.h"

//#define RS485_RE GET_PIN(B, 9)

//寄存器地址
#define POWER_RST 10000
#define POWER_SAVE 10001
#define POWER_MODE 30000
#define POWER_IRMS 31000
#define POWER_VRMS 31002
#define POWER_IPEAK 31004
#define POWER_VPEAK 31006
#define POWER_VA 31008
#define POWER_PREAL 31010
#define POWER_PF 31012
#define POWER_FREQUENCY 31014
#define POWER_VSET 31100
#define POWER_ISET 31102
#define POWER_FSET 31104
#define POWER_CIRCOUNTER 40000
#define POWER_SEQ1_VSET 40002
#define POWER_SEQ1_ISET 40004
#define POWER_SEQ1_FSET 40006
#define POWER_SEQ1_TRF 40008
#define POWER_SEQ1_TON 40010
#define POWER_SEQ1_EN 40012
#define POWER_SEQ2_VSET 40013
#define POWER_SEQ2_ISET 40015
#define POWER_SEQ2_FSET 40017
#define POWER_SEQ2_TRF 40019
#define POWER_SEQ2_TON 40021
#define POWER_SEQ2_EN 10023

uint8_t modbus_connect_or_not; //若modbus连接成功，则值为1;

modbus_t *ctx = RT_NULL; //创建modbus指针
uint16_t iset[2] = { 0, 0 }; //初始化电流数组【高16位，低16位】
uint16_t vset[2] = { 0, 0 }; //初始化电压数组【高16位，低16位】
uint16_t fset[2] = { 0, 0 }; //初始化频率数组【高16位，低16位】
uint16_t mode[5] = { 0, 1, 2, 3, 4 }; //3为输出，0为off

//初始化电流电压频率数据
int current = 0; //单位mA
int voltage = 5000; //单位mV
int frequency = 500; //单位hz/10

//进制转换，将一个int型的数据转换成两个16位的16进制数，高位在前，低位在后
static uint16_t *current_10to16(int cur)
{
    iset[0] = cur >> 16;
    iset[1] = cur & 0xffff;
    return iset;
}

static uint16_t *voltage_10to16(int vol)
{
    vset[0] = vol >> 16;
    vset[1] = vol & 0xffff;
    return vset;
}

static uint16_t *frequency_10to16(int fre)
{
    fset[0] = fre >> 16;
    fset[1] = fre & 0xffff;
    return fset;
}

//选择485端口连接，如果电流大于10000mA，则连接控制小电流输出的485端口，反之则连接控制大电流输出的485端口
void i_v_output(int cur, int vol)
{
    current = cur;
    voltage = vol;
    if (cur <= 10000)
    {
        modbus_set_slave(ctx, 1); //从机地址
        modbus_connect_or_not = 1;
        if (modbus_connect(ctx) == -1)
        {
            modbus_connect_or_not = 0;
            rt_kprintf("modbus connect failed\n");
            return;
        };
        modbus_set_response_timeout(ctx, 0, 1000000);
    }
    else if (cur > 10000 && cur <= 165000)
    {
        modbus_connect_or_not = 1;
        modbus_set_slave(ctx, 2); //从机地址
        if (modbus_connect(ctx) == -1)
        {
            modbus_connect_or_not = 0;
            rt_kprintf("modbus connect failed\n");
            return;
        };
        modbus_set_response_timeout(ctx, 0, 1000000);
    }
    else
    {
        rt_kprintf("current is not available\n");
        return;
    }
    modbus_write_registers(ctx, POWER_ISET, 2, current_10to16(current)); //设置电流大小
    rt_thread_mdelay(100);
    modbus_write_registers(ctx, POWER_VSET, 2, voltage_10to16(voltage)); //设置电压大小
    rt_thread_mdelay(100);
    modbus_write_registers(ctx, POWER_FSET, 2, frequency_10to16(frequency)); //设置频率大小
    rt_thread_mdelay(100);
    modbus_write_registers(ctx, POWER_MODE, 1, (mode + 3)); //输出
}

void i_v_close()
{
    modbus_write_registers(ctx, POWER_MODE, 1, (mode + 0)); //关闭电流输出
    modbus_connect_or_not = 0;
    modbus_close(ctx);
}

static void modbus_master_thread(void *param)
{

    ctx = modbus_new_rtu("dev/uart6", 19200, 'N', 8, 1);
    modbus_rtu_set_serial_mode(ctx, MODBUS_RTU_RS485);
    //modbus_rtu_set_rts(ctx, RS485_RE, MODBUS_RTU_RTS_UP);
    if (NULL == ctx)
    {
        rt_kprintf("Set modbus RTU failed!\n");
        return;
    }
    while (1)
    {
        rt_thread_mdelay(10000);
    }
    modbus_close(ctx);
    //8-释放modbus资源
    modbus_free(ctx);
}

int rtu_test_init(void)
{
    //rt_pin_mode(RS485_RE, PIN_MODE_OUTPUT);
    rt_thread_t tid;
    tid = rt_thread_create("modbus", modbus_master_thread, RT_NULL, 2048, 12, 10);
    if (tid != RT_NULL)
        rt_thread_startup(tid);
    return RT_EOK;
}
INIT_DEVICE_EXPORT(rtu_test_init);
