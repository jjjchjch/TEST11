/**
 ****************************************************************************************************
 * @attention
 *
 * KEY1: PA0, 空闲下拉, 按下置高电平
 * KEY2: PA1, 空闲上拉, 按下置低电平
 * KEY3: PA4, 空闲上拉, 按下置低电平
 *
 ****************************************************************************************************
 */

#include "key.h"
#include "delay.h"

/**
 * @brief  按键初始化函数
 * @param  无
 * @retval 无
 */
void key_init(void)
{
    GPIO_InitTypeDef gpio_init_struct;

    KEY1_GPIO_CLK_ENABLE();
    KEY2_GPIO_CLK_ENABLE();
    KEY3_GPIO_CLK_ENABLE();

    /* KEY1: PA0, 下拉输入, 按下为高电平 */
    gpio_init_struct.Pin   = KEY1_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_PULLDOWN;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY1_GPIO_PORT, &gpio_init_struct);

    /* KEY2: PA1, 上拉输入, 按下为低电平 */
    gpio_init_struct.Pin   = KEY2_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY2_GPIO_PORT, &gpio_init_struct);

    /* KEY3: PA4, 上拉输入, 按下为低电平 */
    gpio_init_struct.Pin   = KEY3_GPIO_PIN;
    gpio_init_struct.Mode  = GPIO_MODE_INPUT;
    gpio_init_struct.Pull  = GPIO_PULLUP;
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(KEY3_GPIO_PORT, &gpio_init_struct);
}

/**
 * @brief  按键扫描函数（单次触发，不支持连按）
 * @param  mode: 0 = 不支持连按; 1 = 支持连按
 * @retval KEY1_PRES(1) / KEY2_PRES(2) / KEY3_PRES(3) / 0(无按键)
 */
uint8_t key_scan(uint8_t mode)
{
    static uint8_t key_up = 1; /* 按键松开标志 */

    if (mode)
    {
        key_up = 1; /* 支持连按模式: 每次调用都重置松开标志 */
    }

    if (key_up &&
        (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == GPIO_PIN_SET  ||
         HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == GPIO_PIN_RESET ||
         HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == GPIO_PIN_RESET))
    {
        delay_ms(10); /* 消抖 */
        key_up = 0;

        if (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == GPIO_PIN_SET)
        {
            return KEY1_PRES;
        }
        else if (HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == GPIO_PIN_RESET)
        {
            return KEY2_PRES;
        }
        else if (HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == GPIO_PIN_RESET)
        {
            return KEY3_PRES;
        }
    }
    else if (HAL_GPIO_ReadPin(KEY1_GPIO_PORT, KEY1_GPIO_PIN) == GPIO_PIN_RESET &&
             HAL_GPIO_ReadPin(KEY2_GPIO_PORT, KEY2_GPIO_PIN) == GPIO_PIN_SET   &&
             HAL_GPIO_ReadPin(KEY3_GPIO_PORT, KEY3_GPIO_PIN) == GPIO_PIN_SET)
    {
        key_up = 1; /* 所有按键已松开 */
    }

    return 0;
}
