#ifndef AUDIO_DAC_H
#define AUDIO_DAC_H
#include <msp430.h>
#include "songs.h"

void AudioDAC_Init(void);
void AudioDAC_SwitchSquare(void);
void AudioDAC_SwitchSine(void);

void AudioDAC_SetNote(unsigned char note_idx);
unsigned char AudioDAC_GetNote(void);

void AudioDAC_SetVolume(unsigned char vol);
unsigned char AudioDAC_GetVolume(void);

unsigned char AudioDAC_GetWaveMode(void);
void AudioDAC_Refresh(void);

// 新增：静音控制（不修改音量，仅控制输出）
void AudioDAC_Mute(void);
void AudioDAC_Unmute(void);
unsigned char AudioDAC_IsMuted(void);

void AudioDAC_SetPlayState(unsigned char state);
unsigned char AudioDAC_GetPlayState(void);

#endif
