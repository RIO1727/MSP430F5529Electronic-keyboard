#include "audio_player.h"
#include "note_ctrl.h"
#include "audio_dac.h"
#include "keyboard.h"
#include "songs.h"
#include "ui.h"

static unsigned char is_playing = 0;
static unsigned char current_song = 0;
static unsigned char stop_request = 0;

static void Player_DelayMs(unsigned int ms);

void Player_Init(void)
{
    is_playing = 0;
    current_song = 0;
    stop_request = 0;
}

unsigned char Player_Start(void)
{
    unsigned char i;
    unsigned char saved_note;

    // 仅保存音符，不再保存音量（音量全局生效）
    saved_note = NoteCtrl_GetNoteIndex();
    stop_request = 0;

    is_playing = 1;
    AudioDAC_SetPlayState(1);
    AudioDAC_Unmute();  // 播放开始取消静音
    UI_DrawSongMode();

    for(i = 0; i < song_lens[current_song]; i++)
    {
        if(stop_request) break;

        MatrixKey_Scan();
        unsigned char pressed = MatrixKey_GetPressed();

        // 全局音量：实时生效，不影响其他逻辑
        if(pressed == 9) {
            NoteCtrl_VolumeUp();
            UI_DrawSongMode();
        } else if(pressed == 10) {
            NoteCtrl_VolumeDown();
            UI_DrawSongMode();
        }
        // 播放/暂停
        else if(pressed == 2) {
            Player_TogglePause();
            UI_DrawSongMode();
        }
        // 上一首
        else if(pressed == 0) {
            Player_PrevSong();
            i = 0;
            UI_DrawSongMode();
        }
        // 下一首
        else if(pressed == 1) {
            Player_NextSong();
            i = 0;
            UI_DrawSongMode();
        }
        // S16：中断播放
        else if(pressed == 15) {
            Player_Stop();
            break;
        }

        unsigned char n = song_ptrs[current_song][i];
        if(n == 255) {
            // 休止符：用静音实现，不修改音量
            AudioDAC_Mute();
        } else {
            // 正常音符：设置音符+取消静音
            NoteCtrl_SetNote(n);
            AudioDAC_Unmute();
        }

        Player_DelayMs(NOTE_MS);
    }

    // 播放结束清理
    is_playing = 0;
    unsigned char interrupted = stop_request;
    stop_request = 0;
    AudioDAC_SetPlayState(0);
    AudioDAC_Mute();  // 播放结束保持静音

    // 恢复播放前的音符，音量不恢复（保持用户调节后的结果）
    NoteCtrl_SetNote(saved_note);
    UI_DrawSongMode();

    return interrupted ? 1 : 0;
}

void Player_TogglePause(void)
{
    if(is_playing == 1) {
        is_playing = 2;
        AudioDAC_SetPlayState(2);
        AudioDAC_Mute();
    } else if(is_playing == 2) {
        is_playing = 1;
        AudioDAC_SetPlayState(1);
        AudioDAC_Unmute();
    }
}

void Player_Stop(void)
{
    stop_request = 1;
    is_playing = 0;
    AudioDAC_SetPlayState(0);
    AudioDAC_Mute();
}

void Player_Reset(void)
{
    stop_request = 1;
    is_playing = 0;
    current_song = 0;
    AudioDAC_SetPlayState(0);
    AudioDAC_Mute();
}

void Player_PrevSong(void)
{
    if(current_song == 0)
        current_song = TOTAL_SONGS - 1;
    else
        current_song--;
}

void Player_NextSong(void)
{
    current_song++;
    if(current_song >= TOTAL_SONGS) current_song = 0;
}

unsigned char Player_GetState(void)
{
    return is_playing;
}

unsigned char Player_GetCurrentSong(void)
{
    return current_song;
}

const unsigned char* Player_GetSongName(void)
{
    return song_names[current_song];
}

unsigned char Player_IsStopRequested(void)
{
    return stop_request;
}

/************************ 延时：实时检测按键 ************************/
static void Player_DelayMs(unsigned int ms)
{
    unsigned int i;
    for(i = 0; i < ms; i += 10) {
        if(stop_request) return;

        MatrixKey_Scan();
        unsigned char pressed = MatrixKey_GetPressed();

        // 全局音量：延时时也能实时调节
        if(pressed == 9) {
            NoteCtrl_VolumeUp();
            UI_DrawSongMode();
        } else if(pressed == 10) {
            NoteCtrl_VolumeDown();
            UI_DrawSongMode();
        }
        // 暂停/继续
        else if(pressed == 2) {
            Player_TogglePause();
            UI_DrawSongMode();
            while(is_playing == 2) {
                if(stop_request) return;
                MatrixKey_Scan();
                unsigned char wait_key = MatrixKey_GetPressed();
                if(wait_key == 2) {
                    Player_TogglePause();
                    UI_DrawSongMode();
                    break;
                } else if(wait_key == 0) {
                    Player_PrevSong();
                    UI_DrawSongMode();
                    return;
                } else if(wait_key == 1) {
                    Player_NextSong();
                    UI_DrawSongMode();
                    return;
                } else if(wait_key == 15) {
                    Player_Stop();
                    return;
                }
            }
        }
        // 上下首
        else if(pressed == 0) {
            Player_PrevSong();
            UI_DrawSongMode();
            return;
        } else if(pressed == 1) {
            Player_NextSong();
            UI_DrawSongMode();
            return;
        }
        // S16中断
        else if(pressed == 15) {
            Player_Stop();
            return;
        }

        __delay_cycles(80000);
    }
}
