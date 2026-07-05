#include "ui.h"
#include "oled.h"
#include "note_ctrl.h"
#include "clip_effect.h"
#include "slide_effect.h"
#include "audio_player.h"
#include "songs.h"

static const unsigned char *vol_str[11] = {
    "0 ", "1 ", "2 ", "3 ", "4 ", "5 ", "6 ", "7 ", "8 ", "9 ", "10"
};
static const unsigned char *clip_str[2] = {"OFF", "ON "};
static const unsigned char *slide_str[2] = {"OFF", "ON "};
static const unsigned char *play_state_str[3] = {"STOP", "PLAY", "PAUSE"};

// 清屏后绘制主菜单
void UI_DrawMenu(void)
{
    OLED_Clear();
    OLED_ShowString(24, 0, "Music System");
    OLED_ShowString(0, 2, "1. Play Keyboard");
    OLED_ShowString(0, 4, "2. Play Songs");
    OLED_ShowString(0, 6, "Press S1/S2 enter");
}

// 绘制演奏模式界面
void UI_DrawKeyboardMode(void)
{
    OLED_Clear();

    OLED_ShowString(0, 0, "Mode: Keyboard");

    OLED_ShowString(0, 2, "Note: ");
    OLED_ShowString(36, 2, (unsigned char*)note_name[NoteCtrl_GetNoteIndex()]);

    OLED_ShowString(0, 3, "Wave: ");
    if(NoteCtrl_GetWaveMode() == 0)
        OLED_ShowString(36, 3, "Square");
    else
        OLED_ShowString(36, 3, "Sine");

    OLED_ShowString(0, 5, "VOL: ");
    OLED_ShowString(30, 5, (unsigned char*)vol_str[NoteCtrl_GetVolume()]);

    OLED_ShowString(0, 6, "Clip:");
    OLED_ShowString(30, 6, (unsigned char*)clip_str[Clip_GetState()]);
    OLED_ShowString(54, 6, "Slide:");
    OLED_ShowString(90, 6, (unsigned char*)slide_str[Slide_GetState()]);

    OLED_ShowString(0, 7, "S16 back to menu");
}

void UI_DrawSongMode(void)
{
    unsigned char state = Player_GetState();
    unsigned char song_idx = Player_GetCurrentSong();

    OLED_Clear();

    OLED_ShowString(0, 0, "Mode: Song Play");

    OLED_ShowString(0, 2, "Song: ");
    OLED_ShowString(36, 2, (unsigned char*)Player_GetSongName());

    OLED_ShowString(0, 3, "No. ");
    OLED_ShowChar(30, 3, '1' + song_idx);
    OLED_ShowString(36, 3, "/");
    OLED_ShowChar(42, 3, '0' + TOTAL_SONGS);

    OLED_ShowString(0, 4, "State: ");
    OLED_ShowString(42, 4, (unsigned char*)play_state_str[state]);

    OLED_ShowString(0, 5, "VOL: ");
    OLED_ShowString(30, 5, (unsigned char*)vol_str[NoteCtrl_GetVolume()]);

    OLED_ShowString(0, 7, "S1prev S2next S3play/pause");
}
