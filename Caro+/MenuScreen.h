#ifndef _MENUSCREEN_H_
#define _MENUSCREEN_H_

void SplashScreen ();
int MainMenuScreen (int optionCursor);
void StatisticScreen ();
bool DataScreen (bool EnableSave);
void OptionScreen ();
void Play_ClearLogicalBoard ();
void PlayScreen (bool IsANewGame);

void Stat_Load ();
void Stat_Save ();

void Data_InitializeSlots ();
void Data_FinalizeSlots ();

void Option_InitializeOption (const char OptionFile[]);
void Option_FinalizeOption (const char OptionFile[]);
#endif
