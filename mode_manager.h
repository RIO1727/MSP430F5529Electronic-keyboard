#ifndef MODE_MANAGER_H
#define MODE_MANAGER_H
#include <msp430.h>

// 系统模式枚举
typedef enum {
    MODE_MENU = 0,      // 主菜单
    MODE_KEYBOARD,      // 演奏模式
    MODE_SONG_PLAY      // 播放模式
} SystemMode_t;

// 模式管理初始化
void Mode_Init(void);

// 模式主循环（按键检测+逻辑处理+界面刷新）
void Mode_MainLoop(void);

#endif
