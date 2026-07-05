#ifndef OLED_H_
#define OLED_H_

#include <msp430.h>

/************************ 硬件引脚配置 ************************
 * SDA -> P3.0
 * SCL -> P3.1
 * SSD1306 I2C 写地址: 0x78 (器件地址 0x3C 左移 1 位)
 ************************************************************/
#define SDA_PIN     BIT0
#define SCL_PIN     BIT1
#define OLED_ADDR   0x78

/************************ 对外接口函数 ************************/
void OLED_Init(void);        // OLED 初始化
void OLED_Clear(void);       // 全屏清屏
void OLED_Set_Pos(unsigned char x, unsigned char y);  // 设置光标位置 (x:0~127, y:0~7)
void OLED_ShowChar(unsigned char x, unsigned char y, unsigned char chr);    // 显示单个 6x8 字符
void OLED_ShowString(unsigned char x, unsigned char y, unsigned char *str); // 显示字符串

#endif /* OLED_H_ */
