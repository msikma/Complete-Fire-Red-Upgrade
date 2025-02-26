#pragma once

#include "../global.h"

//Exported Functions
u8 GetFrontierTrainerClassId(u16 trainerId, u8 battlerNum);
void CopyFrontierTrainerName(u8* dst, u16 trainerId, u8 battlerNum);
const u8* GetFrontierTrainerName(u16 trainerId, u8 battlerNum);
void CopyFrontierTrainerText(u8 whichText, u16 trainerId, u8 battlerNum);
u8 GetFrontierTrainerFrontSpriteId(u16 trainerId, u8 battlerNum);
u16 TryGetSpecialFrontierTrainerMusic(u16 trainerId, u8 battlerNum);
u32 GetAIFlagsInBattleFrontier(unusedArg u8 bank);
u8 GetNumMonsOnTeamInFrontier(void);
u16 GetCurrentBattleTowerStreak(void);
u16 GetMaxBattleTowerStreakForTier(u8 tier);

//Functions Hooked In:
u16 sp052_GenerateTowerTrainer(void);
void sp053_LoadFrontierIntroBattleMessage(void);
u16 sp054_GetBattleTowerStreak(void);
void sp055_UpdateBattleTowerStreak(void);
u16 sp056_DetermineBattlePointsToGive(void);

//Exported Constants
//#define TOTAL_SPREADS 0x4A0 //sizeof(gFrontierSpreads) / sizeof(struct BattleTowerSpreads)

#define NUM_MALE_NAMES gNumMaleFrontierTrainerNames
#define NUM_FEMALE_NAMES gNumFemaleFrontierTrainerNames
#define NUM_TOWER_TRAINERS gNumTowerTrainers
#define NUM_SPECIAL_TOWER_TRAINERS gNumSpecialTowerTrainers

#define FRONTIER_BRAIN_TID 0x397 //Trainer Index
#define BATTLE_TOWER_SPECIAL_TID 0x398 //Trainer Index
#define BATTLE_TOWER_TID 0x399 //Trainer Index

enum 
{
	FRONTIER_BEFORE_TEXT,
	FRONTIER_PLAYER_LOST_TEXT,
	FRONTIER_PLAYER_WON_TEXT,
};

enum BattleTowerBattleTypes
{
	BATTLE_TOWER_SINGLE,
	BATTLE_TOWER_DOUBLE,
	BATTLE_TOWER_MULTI,
	BATTLE_TOWER_LINK_MULTI,
};

#define NUM_TOWER_BATTLE_TYPES (BATTLE_TOWER_LINK_MULTI + 1)

enum BattleTowerFormats
{
	BATTLE_TOWER_STANDARD,
	BATTLE_TOWER_FREE_FOR_ALL,
	BATTLE_TOWER_OU,
	BATTLE_TOWER_UBER,
	BATTLE_TOWER_LITTLE_CUP,
	BATTLE_TOWER_MIDDLE_CUP,
};

#define NUM_FORMATS (BATTLE_TOWER_MIDDLE_CUP + 1)

enum BattleTowerPartySizes
{
	BATTLE_TOWER_SIZE_STANDARD,
	BATTLE_TOWER_SIZE_1V1,
	BATTLE_TOWER_SIZE_2V2,
	BATTLE_TOWER_SIZE_3V3,
	BATTLE_TOWER_SIZE_4V4,
	BATTLE_TOWER_SIZE_5V5,
	BATTLE_TOWER_SIZE_6V6,
};

enum BattleTowerGenders
{
	BATTLE_TOWER_MALE,
	BATTLE_TOWER_FEMALE,
};

enum BattlerIds
{
	FRONTIER_TRAINER_A,
	FRONTIER_TRAINER_B,
	FRONTIER_PARTNER,
};

enum FrontierSpreadAbilityNumbers
{
	FRONTIER_ABILITY_1,
	FRONTIER_ABILITY_2,
	FRONTIER_ABILITY_HIDDEN,
};

//Exported Data Structures
extern const u16 gNumMaleFrontierTrainerNames;
extern const u16 gNumFemaleFrontierTrainerNames;
extern const u16 gNumTowerTrainers;
extern const u16 gNumSpecialTowerTrainers;

struct BattleTowerSpread
{
    u16 species;
	u8 nature;
	u8 hpIv;
	u8 atkIv;
	u8 defIv;
	u8 spdIv;
	u8 spAtkIv;
	u8 spDefIv;
	u8 hpEv;
	u8 atkEv;
	u8 defEv;
	u8 spdEv;
	u8 spAtkEv;
	u8 spDefEv;
	u8 ability;
	u16 item;
	u16 moves[4];
	u8 ball;
    u8 _1; // 0x00
	u32 _2; // 0x00000000
};

extern const struct BattleTowerSpread gFrontierSpreads[];
//#define gFrontierSpreads ((const struct BattleTowerSpread*) 0x89DFA00)

struct BattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	const u8* preBattleText;
	const u8* playerWinText;
	const u8* playerLoseText;
};

extern const struct BattleTowerTrainer gTowerTrainers[];

struct SpecialBattleTowerTrainer
{
    u16 owNum;
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	const u8* name;
	const u8* preBattleText;
	const u8* playerWinText;
	const u8* playerLoseText;
	const struct BattleTowerSpread* spreads;
	u16 spreadSize;
	u16 songId;
};

extern const struct SpecialBattleTowerTrainer gSpecialTowerTrainers[];

struct FrontierBrain
{
	u8 trainerClass;
	u8 trainerSprite;
	u8 gender;
	const u8* name;
	const struct BattleTowerSpread* spreads;
	u16 spreadSize;
	u16 songId;
};

extern const struct FrontierBrain gFrontierBrains[];

extern u16 gBattleTowerStreaks[NUM_TOWER_BATTLE_TYPES][NUM_FORMATS][/*INVERSE*/ 2][/*LEVEL*/ 4][/*CURRENT_OR_MAX*/ 2]; //0x2026840

extern const species_t StandardSpeciesBanList[];
extern const species_t OU_SpeciesBanList[];
extern const species_t LittleCup_SpeciesList[];
extern const ability_t OU_AbilityBanList[];
extern const item_t StandardItemBanList[];
extern const item_t OU_ItemBanList[];
extern const move_t SmogonMoveBanList[];
extern const move_t LittleCup_MoveBanList[];
