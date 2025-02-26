#pragma once

#include "global.h"

u8 __attribute__((long_call)) CreateMonIcon(u16 species, void (*callback)(struct Sprite *), s16 x, s16 y, u8 subpriority, u32 personality, bool32 extra);
const u8* __attribute__((long_call)) GetMonIconTiles(u16 species, bool32 extra);
void __attribute__((long_call)) LoadMonIconPalettes(void);
void __attribute__((long_call)) SafeLoadMonIconPalette(u16 species);
void __attribute__((long_call)) LoadMonIconPalette(u16 species);
void __attribute__((long_call)) FreeMonIconPalettes(void);
void __attribute__((long_call)) SafeFreeMonIconPalette(u16 species);
void __attribute__((long_call)) FreeMonIconPalette(u16 species);
void __attribute__((long_call)) SpriteCB_PokeIcon(struct Sprite *sprite);
const u16* __attribute__((long_call)) GetValidMonIconPalettePtr(u16 species);
u8 __attribute__((long_call)) GetValidMonIconPalIndex(u16 species);
u8 __attribute__((long_call)) GetSpeciesIconPaletteIndex(u16 species);
const u16* __attribute__((long_call)) GetValidMonIconPalettePtr(u16 species);
u8 __attribute__((long_call)) UpdateMonIconFrame(struct Sprite *sprite);