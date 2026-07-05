#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H
#include <msp430.h>

// 播放器初始化
void Player_Init(void);

// 开始播放当前歌曲，返回0=播放完毕，1=被按键中断停止
unsigned char Player_Start(void);

// 暂停/继续切换
void Player_TogglePause(void);

// 停止播放
void Player_Stop(void);

// 复位播放器到初始状态（停止+歌曲回到第1首）
void Player_Reset(void);

// 上一首
void Player_PrevSong(void);

// 下一首
void Player_NextSong(void);

// 获取播放状态 0=停止 1=播放中 2=暂停中
unsigned char Player_GetState(void);

// 获取当前歌曲索引
unsigned char Player_GetCurrentSong(void);

// 获取当前歌曲名称
const unsigned char* Player_GetSongName(void);

// 检查是否请求停止
unsigned char Player_IsStopRequested(void);

#endif
