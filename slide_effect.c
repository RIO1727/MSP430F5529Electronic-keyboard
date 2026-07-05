#include "slide_effect.h"

static unsigned char slide_enable = 0;     // 滑音开关
static unsigned long current_fcw = 0;      // 当前频率控制字
static unsigned long target_fcw = 0;       // 目标频率控制字

void Slide_Toggle(void)
{
    slide_enable ^= 1;
    if(!slide_enable) {
        Slide_SyncToTarget();
    }
}

unsigned char Slide_GetState(void)
{
    return slide_enable;
}

void Slide_SetTarget(unsigned long target)
{
    target_fcw = target;
    if(!slide_enable) {
        current_fcw = target_fcw;
    }
}

void Slide_Update(void)
{
    if(slide_enable && current_fcw != target_fcw) {
        if(current_fcw < target_fcw) {
            current_fcw += SLIDE_STEP;
            if(current_fcw > target_fcw) current_fcw = target_fcw;
        } else {
            current_fcw -= SLIDE_STEP;
            if(current_fcw < target_fcw) current_fcw = target_fcw;
        }
    }
}

unsigned long Slide_GetCurrentFcw(void)
{
    return current_fcw;
}

void Slide_SyncToTarget(void)
{
    current_fcw = target_fcw;
}
