#include "mode_manager.h"
#include "keyboard.h"
#include "ui.h"
#include "note_ctrl.h"
#include "clip_effect.h"
#include "slide_effect.h"
#include "audio_dac.h"
#include "audio_player.h"
#include "songs.h"

static SystemMode_t current_mode = MODE_MENU;

/************************ static 函数前置声明（修复隐式声明报错）************************/
static void System_ResetToDefault(void);
static void Mode_Exit(SystemMode_t old_mode);
static void Mode_Enter(SystemMode_t new_mode);
static void Mode_Switch(SystemMode_t new_mode);
static void Global_KeyHandler(unsigned char pressed_key);
static void Menu_KeyHandler(unsigned char pressed_key);
static void Keyboard_KeyHandler(unsigned char pressed_key, unsigned char released_key);
static void SongPlay_KeyHandler(unsigned char pressed_key);

/************************ 系统复位：回到初始状态 ************************/
static void System_ResetToDefault(void)
{
    Player_Reset();
    AudioDAC_Mute();       // 强制静音
    NoteCtrl_SetNote(0);
    AudioDAC_SetVolume(10);

    if(NoteCtrl_GetWaveMode() != 0) {
        NoteCtrl_ToggleWave();
    }
    if(Clip_GetState() != 0) {
        Clip_Toggle();
    }
    if(Slide_GetState() != 0) {
        Slide_Toggle();
    }

    Slide_SetTarget(note_fcw[0]);
    Slide_SyncToTarget();
    Mode_Switch(MODE_MENU);
}


/************************ 模式退出清理 ************************/
static void Mode_Exit(SystemMode_t old_mode)
{
    if(old_mode == MODE_KEYBOARD) {
        AudioDAC_Mute();
    } else if(old_mode == MODE_SONG_PLAY) {
        Player_Stop();
        AudioDAC_Mute();
    }
}

/************************ 模式进入初始化 ************************/
static void Mode_Enter(SystemMode_t new_mode)
{
    if(new_mode == MODE_MENU) {
        UI_DrawMenu();
    } else if(new_mode == MODE_KEYBOARD) {
        AudioDAC_Refresh();
        UI_DrawKeyboardMode();
    } else if(new_mode == MODE_SONG_PLAY) {
        UI_DrawSongMode();
    }
}

/************************ 切换模式 ************************/
static void Mode_Switch(SystemMode_t new_mode)
{
    if(current_mode == new_mode) return;
    Mode_Exit(current_mode);
    current_mode = new_mode;
    Mode_Enter(current_mode);
}

/************************ 全局按键：全模式生效 ************************/
static void Global_KeyHandler(unsigned char pressed_key)
{
    if(pressed_key == 9) {
        NoteCtrl_VolumeUp();
    } else if(pressed_key == 10) {
        NoteCtrl_VolumeDown();
    } else if(pressed_key == 15) {
        // S16：全局一键复位到初始状态
        System_ResetToDefault();
    }
}

/************************ 主菜单按键处理 ************************/
static void Menu_KeyHandler(unsigned char pressed_key)
{
    if(pressed_key == 0) {
        Mode_Switch(MODE_KEYBOARD);
    } else if(pressed_key == 1) {
        Mode_Switch(MODE_SONG_PLAY);
    }
}

/************************ 演奏模式：按下发声、松开静音 ************************/
static void Keyboard_KeyHandler(unsigned char pressed_key, unsigned char released_key)
{
    if(pressed_key != KEY_NONE) {
        if(pressed_key < 8) {
            NoteCtrl_SetNote(pressed_key);
            AudioDAC_Unmute();  // 按下音符取消静音
        }
        else if(pressed_key == 8) {
            NoteCtrl_ToggleWave();
        }
        else if(pressed_key == 11) {
            Clip_Toggle();
        }
        else if(pressed_key == 12) {
            Slide_Toggle();
        }
        UI_DrawKeyboardMode();
    }

    // 松开音符键立即静音
    if(released_key != KEY_NONE) {
        if(released_key < 8) {
            AudioDAC_Mute();
        }
    }
}

/************************ 播放模式按键处理 ************************/
static void SongPlay_KeyHandler(unsigned char pressed_key)
{
    if(pressed_key == KEY_NONE) return;

    if(pressed_key == 0) {
        if(Player_GetState() == 0) Player_PrevSong();
    }
    else if(pressed_key == 1) {
        if(Player_GetState() == 0) Player_NextSong();
    }
    else if(pressed_key == 2) {
        if(Player_GetState() == 0) {
            // 开始播放，返回1表示被S16中断
            unsigned char ret = Player_Start();
            if(ret == 1) {
                System_ResetToDefault();
            }
            return;
        }
    }
    UI_DrawSongMode();
}

/************************ 模式初始化 ************************/
void Mode_Init(void)
{
    current_mode = MODE_MENU;
    Mode_Enter(MODE_MENU);
}

/************************ 模式主循环 ************************/
void Mode_MainLoop(void)
{
    unsigned char pressed, released;

    MatrixKey_Scan();
    pressed = MatrixKey_GetPressed();
    released = MatrixKey_GetReleased();

    // 全局按键优先处理
    if(pressed != KEY_NONE) {
        Global_KeyHandler(pressed);
        // 音量变化刷新界面
        if(pressed == 9 || pressed == 10) {
            switch(current_mode) {
                case MODE_KEYBOARD: UI_DrawKeyboardMode(); break;
                case MODE_SONG_PLAY: UI_DrawSongMode(); break;
                default: break;
            }
        }
        // S16复位后直接跳过本次业务按键处理
        if(pressed == 15) {
            return;
        }
    }

    // 分模式处理业务按键
    switch(current_mode) {
        case MODE_MENU:
            Menu_KeyHandler(pressed);
            break;
        case MODE_KEYBOARD:
            Keyboard_KeyHandler(pressed, released);
            break;
        case MODE_SONG_PLAY:
            SongPlay_KeyHandler(pressed);
            break;
        default:
            break;
    }
}
