#include "note_ctrl.h"
#include "audio_dac.h"

void NoteCtrl_Init(void)
{
    AudioDAC_SetNote(0);
    AudioDAC_SetVolume(10);
}

void NoteCtrl_SetNote(unsigned char note_idx)
{
    AudioDAC_SetNote(note_idx);
    AudioDAC_Refresh();
}

void NoteCtrl_ToggleWave(void)
{
    if(AudioDAC_GetWaveMode() == 0) {
        AudioDAC_SwitchSine();
    } else {
        AudioDAC_SwitchSquare();
    }
}

unsigned char NoteCtrl_GetWaveMode(void)
{
    return AudioDAC_GetWaveMode();
}

void NoteCtrl_VolumeUp(void)
{
    unsigned char vol = AudioDAC_GetVolume();
    if(vol < 10) {
        AudioDAC_SetVolume(vol + 1);
        AudioDAC_Refresh();
    }
}

void NoteCtrl_VolumeDown(void)
{
    unsigned char vol = AudioDAC_GetVolume();
    if(vol > 0) {
        AudioDAC_SetVolume(vol - 1);
        AudioDAC_Refresh();
    }
}

unsigned char NoteCtrl_GetVolume(void)
{
    return AudioDAC_GetVolume();
}

unsigned char NoteCtrl_GetNoteIndex(void)
{
    return AudioDAC_GetNote();
}
