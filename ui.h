#ifndef UI_H
#define UI_H
#include <msp430.h>

// 绘制主菜单界面
void UI_DrawMenu(void);

// 绘制演奏模式界面
void UI_DrawKeyboardMode(void);

// 绘制播放模式界面
void UI_DrawSongMode(void);

#endif
