#include <msp430.h>
#include "system.h"
#include "keyboard.h"
#include "oled.h"
#include "songs.h"
#include "note_ctrl.h"
#include "clip_effect.h"
#include "slide_effect.h"
#include "audio_dac.h"
#include "audio_player.h"
#include "mode_manager.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;

    SetClock_8MHz();
    MatrixKey_Init();
    OLED_Init();

    AudioDAC_Init();
    NoteCtrl_Init();
    Player_Init();

    Slide_SetTarget(note_fcw[0]);
    Slide_SyncToTarget();

    __bis_SR_register(GIE);
    AudioDAC_SwitchSquare();

    Mode_Init();

    while(1)
    {
        Mode_MainLoop();
    }
}
