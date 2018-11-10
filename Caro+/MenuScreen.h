#ifndef _MENUSCREEN_H_
#define _MENUSCREEN_H_

void SplashScreen ();
int MainMenuScreen (int optionCursor);
void StatisticScreen ();
void DataScreen (bool EnableSave);
void OptionScreen ();
void PvPScreen (bool IsANewGame);
void PvCScreen ();

void Stat_Load ();
void Stat_Save ();

void Data_InitializeSlots ();
void Data_FinalizeSlots ();

void Option_InitializeOption (const char OptionFile[]);
#endif
