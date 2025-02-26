#pragma once

#include "../global.h"
#include "../../src/config.h"

#define NUM_TMSHMS NUM_TMS + NUM_HMS	// never change this

extern const u16 Fairy_TM_DiskPal[];

u8* ItemId_GetName(u16 itemId);
u8 ItemId_GetHoldEffect(u16 itemId);
u8 ItemId_GetHoldEffectParam(u16 itemId);
u8 ItemId_GetSecondaryId(u16 itemId);
bool8 IsMegaStone(u16 item);
bool8 IsZCrystal(u16 item);
bool8 IsBerry(u16 item);
bool8 IsTMHM(u16 item);
void CopyTMName(u8* dst, u16 itemId);

u32 CanMonLearnTMHM(struct Pokemon* mon, u8 tm);