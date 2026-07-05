#ifndef SONGS_H
#define SONGS_H
#include <msp430.h>

/************************ 常量配置 ************************/
#define SLIDE_STEP      4096UL
#define NOTE_MS         400     // 每拍时长（ms）
#define TOTAL_SONGS     3       // 歌曲总数
#define VOLUME_LEVELS   11      // 音量等级数
#define NOTE_COUNT      8       // 音符数量

/************************ 外部数据声明 ************************/
extern const unsigned char sin_table[256];
extern const unsigned int note_ccr0_sq[NOTE_COUNT];
extern const unsigned long note_fcw[NOTE_COUNT];
extern const unsigned char *note_name[NOTE_COUNT];
extern const unsigned char volume_scale[VOLUME_LEVELS];

extern const unsigned char *song_ptrs[TOTAL_SONGS];
extern const unsigned char song_lens[TOTAL_SONGS];
extern const unsigned char *song_names[TOTAL_SONGS];

#endif
