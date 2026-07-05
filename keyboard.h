#ifndef KEYBOARD_H
#define KEYBOARD_H
#include <msp430.h>

#define KEY_NONE 0xFF

void MatrixKey_Init(void);
// 扫描一次键盘（非阻塞，更新内部状态）
void MatrixKey_Scan(void);
// 获取刚按下的按键（单次事件，读取后清除）
unsigned char MatrixKey_GetPressed(void);
// 获取刚松开的按键（单次事件，读取后清除）
unsigned char MatrixKey_GetReleased(void);
// 获取当前持续按下的按键
unsigned char MatrixKey_GetCurrent(void);

#endif
