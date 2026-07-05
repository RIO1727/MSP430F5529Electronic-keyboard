#include "audio_dac.h"
#include "songs.h"
#include "slide_effect.h"
#include "clip_effect.h"

static unsigned long phase_acc = 0;
static unsigned char current_note = 0;
static unsigned char current_volume = 10;
static unsigned char current_wave = 0;
static unsigned char play_state = 0;
static unsigned char mute_flag = 0;  // 新增：静音状态标志 0=正常 1=静音

void AudioDAC_Init(void)
{
    phase_acc = 0;
    current_note = 0;
    current_volume = 10;
    current_wave = 0;
    play_state = 0;
    mute_flag = 1;  // 初始默认静音，不发声
    P2DIR |= BIT0;
    P2SEL |= BIT0;
}

void AudioDAC_SwitchSquare(void)
{
    current_wave = 0;
    TA1CCTL0 &= ~CCIE;
    TA1CTL = 0;
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
    P2SEL |= BIT0;
    AudioDAC_Refresh();
}

void AudioDAC_SwitchSine(void)
{
    current_wave = 1;
    TA1CTL = 0;
    phase_acc = 0;

    TA1CCR0 = 255;
    TA1CCR1 = 128;
    TA1CCTL1 = OUTMOD_7;
    TA1CCTL0 = CCIE;
    TA1CTL = TASSEL_2 + MC_1 + TACLR;
    P2SEL |= BIT0;
    AudioDAC_Refresh();
}

void AudioDAC_SetNote(unsigned char note_idx)
{
    if(note_idx >= NOTE_COUNT) return;
    current_note = note_idx;
}

unsigned char AudioDAC_GetNote(void)
{
    return current_note;
}

void AudioDAC_SetVolume(unsigned char vol)
{
    if(vol > 10) vol = 10;
    current_volume = vol;
}

unsigned char AudioDAC_GetVolume(void)
{
    return current_volume;
}

unsigned char AudioDAC_GetWaveMode(void)
{
    return current_wave;
}

// 统一刷新输出：静音优先
void AudioDAC_Refresh(void)
{
    if(current_wave == 0) {
        // 方波模式
        TA1CCR0 = note_ccr0_sq[current_note];
        if(mute_flag || current_volume == 0) {
            TA1CCR1 = 0;
        } else {
            TA1CCR1 = ((unsigned long)(TA1CCR0 + 1) * current_volume) / 20;
        }
    } else {
        // 正弦波模式：中断内处理静音，这里只更新目标频率
        Slide_SetTarget(note_fcw[current_note]);
    }
}

// 开启静音
void AudioDAC_Mute(void)
{
    mute_flag = 1;
    AudioDAC_Refresh();
}

// 取消静音
void AudioDAC_Unmute(void)
{
    mute_flag = 0;
    AudioDAC_Refresh();
}

unsigned char AudioDAC_IsMuted(void)
{
    return mute_flag;
}

void AudioDAC_SetPlayState(unsigned char state)
{
    play_state = state;
}

unsigned char AudioDAC_GetPlayState(void)
{
    return play_state;
}

/************************ TA1 中断：正弦波输出 ************************/
#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    unsigned char raw;
    signed short diff;
    signed short scaled;
    unsigned char result;

    // 暂停或静音状态直接输出中值
    if(play_state == 2 || mute_flag) {
        TA1CCR1 = 128;
        return;
    }

    // 1. 滑音更新
    Slide_Update();

    // 2. 相位累加 + 查表
    phase_acc += Slide_GetCurrentFcw();
    raw = sin_table[(unsigned char)(phase_acc >> 24)];

    // 3. 削波
    raw = Clip_Process(raw);

    // 4. 音量缩放
    if(current_volume == 0) {
        result = 128;
    }
    else if(current_volume == 10) {
        result = raw;
    }
    else {
        diff = (signed short)((signed char)raw - 128);
        scaled = (diff * (signed short)volume_scale[current_volume]) >> 8;
        result = 128 + (unsigned char)scaled;
    }

    TA1CCR1 = result;
}
