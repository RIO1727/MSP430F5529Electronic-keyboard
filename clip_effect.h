#ifndef CLIP_EFFECT_H
#define CLIP_EFFECT_H
#include <msp430.h>

// 切换削波开关
void Clip_Toggle(void);

// 获取削波状态 0=关闭 1=开启
unsigned char Clip_GetState(void);

// 对采样值执行削波处理（中断中调用）
unsigned char Clip_Process(unsigned char raw_sample);

#endif
