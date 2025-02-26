#include "defines.h"
#include "defines_battle.h"
#include "../include/event_data.h"
#include "../include/fieldmap.h"
#include "../include/metatile_behavior.h"
#include "../include/overworld.h"
#include "../include/constants/metatile_behaviors.h"
#include "../include/constants/trainer_classes.h"

#include "../include/new/battle_terrain.h"
#include "../include/new/dns.h"
#include "../include/new/overworld.h"
#include "../include/new/helper_functions.h"

#define gBattleTerrainTable ((struct BattleBackground*) *((u32*) 0x800F320))
#ifdef UNBOUND
#define gBattleTerrainTableEvening ((struct BattleBackground*) 0x88C8720) //For Unbound
#define gBattleTerrainTableNight ((struct BattleBackground*) 0x88C8A30) //For Unbound
#endif

extern const struct BattleBackground gAttackTerrainTable[];

//This file's functions:
static void LoadBattleBG_EntryOverlay(u8 terrainId);
static u8 TryLoadAlternateAreaTerrain(u8 terrain);

u8 BattleSetup_GetTerrainId(void)
{
    u16 tileBehavior;
    s16 x, y;
	u8 terrain = BATTLE_TERRAIN_PLAIN;

    PlayerGetDestCoords(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);

	if (VarGet(BATTLE_BG_VAR))
		terrain = VarGet(BATTLE_BG_VAR);
	
	else
	{
		//Maybe add check for fishing byte?
		if (MetatileBehavior_IsTallGrass(tileBehavior))
		{
			#ifdef UNBOUND
			if (IsCurrentAreaAutumn())
				terrain = BATTLE_TERRAIN_AUTUMN_GRASS;
			else
			#endif
				terrain = BATTLE_TERRAIN_GRASS;
		}
		else if (tileBehavior == MB_VERY_TALL_GRASS) //The old useless function for this check just returns 0
			terrain = BATTLE_TERRAIN_LONG_GRASS;
		else if (MetatileBehavior_IsSandOrDeepSand(tileBehavior))
			terrain = BATTLE_TERRAIN_SAND;
		else if (MetatileBehavior_IsMountain(tileBehavior))
			terrain = BATTLE_TERRAIN_MOUNTAIN;
		
		#ifdef BRIDGE_FIX
			else if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
			{
				if (MetatileBehavior_IsDeepOrOceanWater(tileBehavior))
					terrain = BATTLE_TERRAIN_WATER;
				else if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior))
					terrain = BATTLE_TERRAIN_POND;
			}
		#else
			else if (MetatileBehavior_IsDeepOrOceanWater(tileBehavior))
				terrain = BATTLE_TERRAIN_WATER;
			else if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior))
				terrain = BATTLE_TERRAIN_POND;
		#endif

		switch (GetCurrentMapType()) {
			case MAP_TYPE_UNDERGROUND:
				if (MetatileBehavior_IsIndoorEncounter(tileBehavior))
				{
					terrain = BATTLE_TERRAIN_INSIDE; 
					break;
				}
				
				#ifdef UNBOUND
					terrain = BATTLE_TERRAIN_CAVE;
				#else
					if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior))
						terrain = BATTLE_TERRAIN_POND;
					else
						terrain = BATTLE_TERRAIN_CAVE;
				#endif
				break;
				
			case MAP_TYPE_INDOOR:
			case MAP_TYPE_SECRET_BASE:
				terrain = BATTLE_TERRAIN_INSIDE;
				break;
				
			case MAP_TYPE_UNDERWATER:
				terrain = BATTLE_TERRAIN_UNDERWATER;
				break;
				
			case MAP_TYPE_6:
				if (MetatileBehavior_IsSurfableWaterOrUnderwater(tileBehavior))
					terrain = BATTLE_TERRAIN_WATER;
				else
					terrain = BATTLE_TERRAIN_PLAIN;
		}
	}
		
	terrain = TryLoadAlternateAreaTerrain(terrain); //Loads special BG's if you're surfing
    return terrain;
}

u8 LoadBattleBG_TerrainID(void) {
	u8 terrain = gBattleTerrain;

	if (gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER))
	{
		#ifdef UNBOUND
			terrain = BATTLE_TERRAIN_INSIDE;
		#else
			terrain =  10;
		#endif
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_POKE_DUDE)
	{
		gBattleTerrain = 0;
		terrain = 0;
	}
	else if (VarGet(BATTLE_BG_VAR))
	{
		terrain = VarGet(BATTLE_BG_VAR);
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_TRAINER)
	{
		u8 trainerClass = gTrainers[gTrainerBattleOpponent_A].trainerClass;
		u8 trainerClassB = gTrainers[SECOND_OPPONENT].trainerClass;
		#ifdef OVERWRITE_BG_FOR_LEADER_CHAMPION
			if (trainerClass == CLASS_LEADER || trainerClassB == CLASS_LEADER)
			{
				terrain = 12;
			}
			else if (trainerClass == CLASS_CHAMPION || trainerClassB == CLASS_CHAMPION)
			{
				terrain = 19;
			}
			else if (GetCurrentMapBattleScene() != 0)
			{
				terrain = LoadBattleBG_SpecialTerrainID(GetCurrentMapBattleScene());
			}
		#else
			trainerClass += trainerClassB; //Useless statement so it compiles without warning
			if (GetCurrentMapBattleScene() != 0)
			{
				terrain = LoadBattleBG_SpecialTerrainID(GetCurrentMapBattleScene());
			}
		#endif
			else
				terrain = gBattleTerrain;
	}
	else
	{
		if (GetCurrentMapBattleScene() != 0)
		{
			terrain = LoadBattleBG_SpecialTerrainID(GetCurrentMapBattleScene());
		}
		else
			terrain = gBattleTerrain;
	}
	
	terrain = TryLoadAlternateAreaTerrain(terrain);

	return terrain;
}

void DrawBattleEntryBackground(void)
{
  /*  if (gBattleTypeFlags & BATTLE_TYPE_LINK)
    {
        LZDecompressVram(gUnknown_08E7737C, (void*)(BG_CHAR_ADDR(1)));
        LZDecompressVram(gUnknown_08E77598, (void*)(VRAM + 0x10000));
        LoadCompressedPalette(gUnknown_08E77570, 0x60, 0x20);
        SetBgAttribute(1, BG_ATTR_SCREENSIZE, 1);
        SetGpuReg(REG_OFFSET_BG1CNT, 0x5C04);
        CopyToBgTilemapBuffer(1, gUnknown_08E77464, 0, 0);
        CopyToBgTilemapBuffer(2, gUnknown_08E77464, 0, 0);
        CopyBgTilemapBufferToVram(1);
        CopyBgTilemapBufferToVram(2);
        SetGpuReg(REG_OFFSET_WININ, 0x36);
        SetGpuReg(REG_OFFSET_WINOUT, 0x36);
        gBattle_BG1_Y = 0xFF5C;
        gBattle_BG2_Y = 0xFF5C;
        LoadCompressedSpriteSheetUsingHeap(&gUnknown_08248318);
    }
	else*/
	{
		u8 terrain = LoadBattleBG_TerrainID();
		LoadBattleBG_EntryOverlay(terrain);
	}
}

void LoadBattleBG_Background(u8 terrainId) {
	struct BattleBackground* table = gBattleTerrainTable;

	if (TerrainType) //A terrain like Electric Terrain is active
	{
		LZDecompressVram(gAttackTerrainTable[TerrainType - 1].tileset, (void*) 0x6008000);
		LZDecompressVram(gAttackTerrainTable[TerrainType - 1].tilemap, (void*) 0x600d000);
		LoadCompressedPalette(gAttackTerrainTable[TerrainType - 1].palette, 0x20, 0x60);
		return;
	}
	
	#ifdef UNBOUND //Load different BGs depending on time of day
		u8 mapType = GetCurrentMapType();
		if (!IsMapTypeIndoors(mapType) && IsMapTypeOutdoors(mapType))
		{
			if (IsNightTime())
				table = gBattleTerrainTableNight;
			else if (IsEvening())
				table = gBattleTerrainTableEvening;
		}
	#endif
	
	LZDecompressVram(table[terrainId].tileset, (void*) 0x6008000);
	LZDecompressVram(table[terrainId].tilemap, (void*) 0x600d000);
	LoadCompressedPalette(table[terrainId].palette, 0x20, 0x60);
	
	#ifdef DNS_IN_BATTLE
		DNSBattleBGPalFade();
	#endif
}

static void LoadBattleBG_EntryOverlay(u8 terrainId)
{
	struct BattleBackground* table = gBattleTerrainTable;

	#ifdef UNBOUND //Load different BGs depending on time of day
	u8 mapType = GetCurrentMapType();
	if (!IsMapTypeIndoors(mapType) && IsMapTypeOutdoors(mapType))
	{
		if (IsNightTime())
			table = gBattleTerrainTableNight;
		else if (IsEvening())
			table = gBattleTerrainTableEvening;
	}
	#endif
	
    LZDecompressVram(table[terrainId].entryTileset, (void*) 0x6004000);
    LZDecompressVram(table[terrainId].entryTilemap, (void*) 0x600E000);
}

static u8 TryLoadAlternateAreaTerrain(u8 terrain)
{
#ifdef UNBOUND
    u16 tileBehavior;
    s16 x, y;

    PlayerGetDestCoords(&x, &y);
    tileBehavior = MapGridGetMetatileBehaviorAt(x, y);

	if (TestPlayerAvatarFlags(PLAYER_AVATAR_FLAG_SURFING))
	{
		switch (terrain) {
			case BATTLE_TERRAIN_CAVE:
				terrain = BATTLE_TERRAIN_WATER_IN_CAVE;
				break;
			case BATTLE_TERRAIN_VOLCANO:
				terrain = BATTLE_TERRAIN_LAVA_IN_VOLCANO;
				break;
			case BATTLE_TERRAIN_FOREST:
				terrain = BATTLE_TERRAIN_WATER_IN_FOREST;
				break;
			case BATTLE_TERRAIN_DARK_CAVE:
				terrain = BATTLE_TERRAIN_DARK_CAVE_WATER;
				break;
			case BATTLE_TERRAIN_SNOW_CAVE:
				terrain = BATTLE_TERRAIN_WATER_IN_SNOW_CAVE;
				break;
		}
	}
		
	if (terrain == BATTLE_TERRAIN_SNOW_CAVE
	&&  MetatileBehavior_IsIce(tileBehavior))
		terrain = BATTLE_TERRAIN_ICE_IN_CAVE;
	else if (terrain == BATTLE_TERRAIN_PLAIN
	&& IsCurrentAreaAutumn())
		terrain = BATTLE_TERRAIN_AUTUMN_PLAIN;
#endif

	return terrain;
}

bool8 MetatileBehavior_IsIce(u8 metatileBehavior)
{
    if (metatileBehavior == MB_ICE
	||  metatileBehavior == MB_THIN_ICE
	||  metatileBehavior == MB_CRACKED_ICE)
        return TRUE;
    else
        return FALSE;
}

bool8 IsTerrainMoveIndoors(void)
{
	if (gBattleTypeFlags & (BATTLE_TYPE_FRONTIER | BATTLE_TYPE_LINK | BATTLE_TYPE_TRAINER_TOWER | BATTLE_TYPE_EREADER_TRAINER))
		return TRUE;
	
	return FALSE;
}
