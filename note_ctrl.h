#ifndef NOTE_CTRL_H
#define NOTE_CTRL_H
#include <msp430.h>

void NoteCtrl_Init(void);
void NoteCtrl_SetNote(unsigned char note_idx);
void NoteCtrl_ToggleWave(void);
unsigned char NoteCtrl_GetWaveMode(void);
void NoteCtrl_VolumeUp(void);
void NoteCtrl_VolumeDown(void);
unsigned char NoteCtrl_GetVolume(void);
unsigned char NoteCtrl_GetNoteIndex(void);

#endif
