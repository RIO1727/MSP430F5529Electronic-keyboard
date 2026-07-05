#include "clip_effect.h"

static unsigned char clip_enable = 0;  // 削波开关：0=关闭, 1=开启

void Clip_Toggle(void)
{
    clip_enable ^= 1;
}

unsigned char Clip_GetState(void)
{
    return clip_enable;
}

unsigned char Clip_Process(unsigned char raw_sample)
{
    if(clip_enable) {
        if(raw_sample > 192) raw_sample = 192;
        else if(raw_sample < 64) raw_sample = 64;
    }
    return raw_sample;
}
