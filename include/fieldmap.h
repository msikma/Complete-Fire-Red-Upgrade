#pragma once

#include "global.h"

#define NUM_PALS_IN_PRIMARY 7
#define NUM_PALS_TOTAL 13

extern struct MapCoords {
    int width;
    int height;
} gUnknown_03005DC0;

u8 __attribute__((long_call)) MetatileBehavior_GetLowerBytes(const u32 a, const u8 b);
u32 __attribute__((long_call)) MapGridGetMetatileIdAt(int x, int y);
u16 __attribute__((long_call)) MapGridGetMetatileField(u16 x, u16 y, u8 a);
u32 __attribute__((long_call)) MapGridGetMetatileBehaviorAt(int, int);
void __attribute__((long_call)) ApplySpecialMapPalette(u16 a0, u16 a1);
u8 __attribute__((long_call)) MapGridGetZCoordAt(int x, int y);
bool8 __attribute__((long_call)) MapGridIsImpassableAt(s16 x, s16 y);
void __attribute__((long_call)) save_serialize_map(void);

/*
u32 MapGridGetMetatileIdAt(int, int);
u32 MapGridGetMetatileBehaviorAt(int, int);
void MapGridSetMetatileIdAt(int, int, u16);
void MapGridSetMetatileEntryAt(int, int, u16);
void GetCameraCoords(u16*, u16*);
s32 GetMapBorderIdAt(s16, s16);
bool32 CanCameraMoveInDirection(u8);
u16 GetBehaviorByMetatileId(u16 metatileId);
*/