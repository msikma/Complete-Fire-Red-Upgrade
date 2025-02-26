#pragma once

#include "../global.h"

/**
 * \file dexnav.h
 * \brief Contains a port of the DexNav system created by FBI on Pokecommunity:
 *		  https://www.pokecommunity.com/showthread.php?p=9998647
 */
 
//Exported Functions
void InitDexNavHUD(u16 species, u8 environment);
bool8 IsDexNavHudActive(void);
void TryFreeDexnavState(void);
u16 RandRange(u16 min, u16 max);

//Functions Hooked In
u8 SelectHookRestore(void);
//u8 PokeToolsFunc(void);
u8 ExecDexNav(void);
void ToolSelection(u8 taskId);