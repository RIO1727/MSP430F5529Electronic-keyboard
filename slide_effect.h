#ifndef SLIDE_EFFECT_H
#define SLIDE_EFFECT_H
#include <msp430.h>
#include "songs.h"

// 切换滑音开关
void Slide_Toggle(void);

// 获取滑音状态 0=关闭 1=开启
unsigned char Slide_GetState(void);

// 设置目标频率控制字
void Slide_SetTarget(unsigned long target_fcw);

// 每采样点执行一次滑音更新（中断中调用）
void Slide_Update(void);

// 获取当前频率控制字
unsigned long Slide_GetCurrentFcw(void);

// 立即同步当前频率到目标频率（关闭滑音/切换模式时调用）
void Slide_SyncToTarget(void);

#endif
