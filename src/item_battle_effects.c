#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_string_ids.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "../include/new/helper_functions.h"
#include "../include/new/item_battle_effects.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/stat_buffs.h"

#define BattleScript_ItemHealHP_Ret (u8*) 0x81D9622
#define BattleScript_ItemHealHP_End2 (u8*) 0x81D961C
#define BattleScript_WhiteHerbRet (u8*) 0x81D95DF
#define BattleScript_WhiteHerbEnd2 (u8*) 0x81D95D9

extern u8 gStatusConditionString_DisableProblem[];
extern u8 gStatusConditionString_EncoreProblem[];
extern u8 gStatusConditionString_MentalState[];
extern u8 gStatusConditionString_TauntProblem[];

enum
{
    ITEM_NO_EFFECT, // 0
    ITEM_STATUS_CHANGE, // 1
    ITEM_EFFECT_OTHER, // 2
    ITEM_PP_CHANGE, // 3
    ITEM_HP_CHANGE, // 4
    ITEM_STATS_CHANGE, // 5
};

enum
{
    FLAVOR_SPICY, // 0
    FLAVOR_DRY, // 1
    FLAVOR_SWEET, // 2
    FLAVOR_BITTER, // 3
    FLAVOR_SOUR, // 4
};

//This file's functions:
static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 DoPluck);
static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 DoPluck);
static u8 RaiseStatsContactItem(u8 bank, u8 stat, bool8 DoPluck);
static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 DoPluck);

u8 ItemBattleEffects(u8 caseID, u8 bank, bool8 moveTurn, bool8 DoPluck)
{
    int i = 0;
    u8 effect = ITEM_NO_EFFECT;
    u8 changedPP = 0;
    u8 bankHoldEffect, atkHoldEffect;
    u8 bankQuality, atkQuality;
    u16 atkItem;
	u8 moveSplit = CalcMoveSplit(gBankAttacker, gCurrentMove);
	
	if (DoPluck) {
		bankHoldEffect = ItemId_GetHoldEffect(gLastUsedItem);
		bankQuality = ItemId_GetHoldEffectParam(gLastUsedItem);
	}
	else {
		gLastUsedItem = gBattleMons[bank].item;
		if (GetPocketByItemId(gLastUsedItem) == POCKET_BERRY_POUCH
		&& AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, bank, ABILITY_UNNERVE, 0, 0)) {
			bankHoldEffect = 0;
			bankQuality = 0;		
		}
		else {
			bankHoldEffect = ITEM_EFFECT(bank);
			bankQuality = ITEM_QUALITY(bank);
		}
	}

    atkItem = gBattleMons[gBankAttacker].item;
    atkHoldEffect = ItemId_GetHoldEffect(atkItem);
    atkQuality = ItemId_GetHoldEffectParam(atkItem);

/*    // def variables are unused
    defItem = gBattleMons[gBankTarget].item;
	defHoldEffect = ItemId_GetHoldEffect(defItem);
    defQuality = ItemId_GetHoldEffectParam(defItem);
*/

    switch (caseID) {
    case ItemEffects_SwitchIn:
        switch (bankHoldEffect) {
			case ITEM_EFFECT_DOUBLE_PRIZE:
				gBattleStruct->moneyMultiplier *= 2;
				break;
				
			case ITEM_EFFECT_RESTORE_STATS:
				for (i = 0; i < BATTLE_STATS_NO-1; i++) {
					if (gBattleMons[bank].statStages[i] < 6) {
						gBattleMons[bank].statStages[i] = 6;
						effect = ITEM_STATS_CHANGE;
					}
				}
				
				if (effect) {
					gBattleScripting->bank = bank;
					gStringBank = bank;
					gActiveBattler = gBankAttacker = bank;
					BattleScriptExecute(BattleScript_WhiteHerbEnd2);
				}
				break;
				
			case ITEM_EFFECT_SEEDS: ;
				u8 stat = 0;
				
				if (bankQuality && bankQuality == TerrainType)
				{
					switch (TerrainType) {
						case ELECTRIC_TERRAIN:
						case GRASSY_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_DEF))
							{
								stat = STAT_STAGE_DEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
						case MISTY_TERRAIN:
						case PSYCHIC_TERRAIN:
							if (STAT_CAN_RISE(bank, STAT_STAGE_SPDEF))
							{
								stat = STAT_STAGE_SPDEF;
								effect = ITEM_STATS_CHANGE;
							}
							break;
					}
					
					if (effect)
					{
						PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
						gBattleScripting->statChanger = INCREASE_1 | stat;
						gBattleScripting->animArg1 = 0xE + stat;
						gBattleScripting->animArg2 = 0;
						gBattleScripting->bank = gBankTarget = gEffectBank = gActiveBattler = bank;
						BattleScriptExecute(BattleScript_RaiseStatsItemEnd2);
					}
				}
		}
        break;
	
    case ItemEffects_EndTurn:
		gBattleScripting->bank = bank;
		
        if (gBattleMons[bank].hp) {
            switch (bankHoldEffect) {
            case ITEM_EFFECT_RESTORE_HP:
                if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2) || (DoPluck && gBattleMons[bank].hp != gBattleMons[bank].maxHP)) {
					if (gLastUsedItem == ITEM_SITRUS_BERRY)
						gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 4);
					else {
						gBattleMoveDamage = bankQuality;
						if (gBattleMons[bank].hp + bankQuality > gBattleMons[bank].maxHP)
							gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
					}
					gBattleMoveDamage *= -1;
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet;
					}
					else
							BattleScriptExecute(BattleScript_ItemHealHP_RemoveItemEnd2);
					effect = ITEM_HP_CHANGE;
                }
                break;
				
            case ITEM_EFFECT_RESTORE_PP:
                if (!moveTurn || DoPluck) 
				{
                    u8 ppBonuses, maxPP;
                    u16 move;
					pokemon_t* poke = GetBankPartyData(bank);

                    for (i = 0; i < 4; ++i) 
					{
                        move = GetMonData(poke, MON_DATA_MOVE1 + i, 0);
                        changedPP = GetMonData(poke, MON_DATA_PP1 + i, 0);
                        ppBonuses = GetMonData(poke, MON_DATA_PP_BONUSES, 0);
						
						if (DoPluck)
						{
							maxPP = CalculatePPWithBonus(move, ppBonuses, i);
							if (changedPP < maxPP)
								break;
						}
                        else if (move && changedPP == 0)
                            break;
                    }
					
                    if (i < MAX_MON_MOVES) 
					{
                        maxPP = CalculatePPWithBonus(move, ppBonuses, i);
                        if (changedPP + bankQuality > maxPP)
                            changedPP = maxPP;
                        else
                            changedPP = changedPP + bankQuality;
							
						PREPARE_MOVE_BUFFER(gBattleTextBuff1, move);
						
						if (DoPluck)
						{
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_BerryPPHealRet;
						}
						else
							BattleScriptExecute(BattleScript_BerryPPHealEnd2);
						
						
						gActiveBattler = bank;
                        EmitSetMonData(0, i + REQUEST_PPMOVE1_BATTLE, 0, 1, &changedPP);
                        MarkBufferBankForExecution(gActiveBattler);
                        effect = ITEM_PP_CHANGE;
                    }
                }
                break;
				
            case ITEM_EFFECT_RESTORE_STATS:
                for (i = 0; i < BATTLE_STATS_NO-1; ++i) {
                    if (gBattleMons[bank].statStages[i] < 6) {
                        gBattleMons[bank].statStages[i] = 6;
                        effect = ITEM_STATS_CHANGE;
                    }
                }
				
                if (effect) 
				{
                    gBattleScripting->bank = gActiveBattler = gStringBank = bank;
					if (moveTurn)
					{
						BattleScriptPushCursor();
						if (DoPluck) //Fling
							gBattlescriptCurrInstr = BattleScript_WhiteHerbFling;
						else
							gBattlescriptCurrInstr = BattleScript_WhiteHerbRet;
					}
					else
						BattleScriptExecute(BattleScript_WhiteHerbEnd2);
                }
                break;
				
            case ITEM_EFFECT_LEFTOVERS:
			LEFTOVERS_HEAL:
                if (!moveTurn && gBattleMons[bank].hp < gBattleMons[bank].maxHP
				&&  !gNewBS->HealBlockTimers[bank])
				{
                    gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
                    if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                        gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    effect = ITEM_HP_CHANGE;
                    RecordItemBattle(bank, bankHoldEffect);
                }
                break;
			
            case ITEM_EFFECT_CONFUSE_SPICY:
				effect = ConfusionBerries(bank, FLAVOR_SPICY, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_CONFUSE_DRY:
				effect = ConfusionBerries(bank, FLAVOR_DRY, moveTurn, DoPluck);
				break;

            case ITEM_EFFECT_CONFUSE_SWEET:
				effect = ConfusionBerries(bank, FLAVOR_SWEET, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_CONFUSE_BITTER:
				effect = ConfusionBerries(bank, FLAVOR_BITTER, moveTurn, DoPluck);
				break;

            case ITEM_EFFECT_CONFUSE_SOUR:
				effect = ConfusionBerries(bank, FLAVOR_SOUR, moveTurn, DoPluck);
				break;
			
            case ITEM_EFFECT_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_ATK, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_DEF, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_SPEED_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPEED, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_SP_ATTACK_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPATK, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_SP_DEFENSE_UP:
				effect = StatRaiseBerries(bank, STAT_STAGE_SPDEF, moveTurn, DoPluck);
				break;
				
            case ITEM_EFFECT_CRITICAL_UP:
                if ((PINCH_BERRY_CHECK(bank) || DoPluck)
				&& !(gBattleMons[bank].status2 & STATUS2_FOCUS_ENERGY)) {
                    gBattleMons[bank].status2 |= STATUS2_FOCUS_ENERGY;
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryFocusEnergyRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryFocusEnergyEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
				
            case ITEM_EFFECT_RANDOM_STAT_UP:
                if ((PINCH_BERRY_CHECK(bank) || DoPluck)
				&& !((StatsMaxed(bank) && ABILITY(bank) != ABILITY_CONTRARY) || (StatsMinned(bank) && ABILITY(bank) == ABILITY_CONTRARY))) {
                    do {
                        i = umodsi(Random(), 5);
                    } while (gBattleMons[bank].statStages[STAT_STAGE_ATK-1 + i] == 12);

                        gBattleTextBuff1[0] = 0xFD;
                        gBattleTextBuff1[1] = 5;
                        gBattleTextBuff1[2] = i + 1;
                        gBattleTextBuff1[3] = EOS;

                        gBattleTextBuff2[0] = 0xFD;
                        gBattleTextBuff2[1] = 0;
                        gBattleTextBuff2[2] = 0xD1;
                        gBattleTextBuff2[3] = 0xD1 >> 8;
                        gBattleTextBuff2[4] = 0;
                        gBattleTextBuff2[5] = 0xD2;
                        gBattleTextBuff2[6] = 0xD2 >> 8;
                        gBattleTextBuff2[7] = EOS;

                        gEffectBank = bank;
                        gBattleScripting->statChanger = 0x21 + i;
                        gBattleScripting->animArg1 = 0x21 + i + 6;
                        gBattleScripting->animArg2 = 0;
						
						if (moveTurn || DoPluck) {
							BattleScriptPushCursor();
							gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
						}
						else
							BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
                        effect = ITEM_STATS_CHANGE;
                    }
                break;
			
            case ITEM_EFFECT_CURE_PAR:
                if (gBattleMons[bank].status1 & STATUS_PARALYSIS)
                {
                    gBattleMons[bank].status1 &= ~(STATUS_PARALYSIS);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureParRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePrlzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
            case ITEM_EFFECT_CURE_PSN:
                if (gBattleMons[bank].status1 & STATUS_PSN_ANY)
                {
                    gBattleMons[bank].status1 &= ~(STATUS_PSN_ANY | STATUS_TOXIC_COUNTER);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCurePsnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCurePsnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
            case ITEM_EFFECT_CURE_BRN:
                if (gBattleMons[bank].status1 & STATUS_BURN)
                {
                    gBattleMons[bank].status1 &= ~(STATUS_BURN);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureBrnRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureBrnEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
            case ITEM_EFFECT_CURE_FRZ:
                if (gBattleMons[bank].status1 & STATUS_FREEZE)
                {
                    gBattleMons[bank].status1 &= ~(STATUS_FREEZE);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureFrzRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureFrzEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
            case ITEM_EFFECT_CURE_SLP:
                if (gBattleMons[bank].status1 & STATUS_SLEEP)
                {
                    gBattleMons[bank].status1 &= ~(STATUS_SLEEP);
                    gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureSlpRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureSlpEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
            case ITEM_EFFECT_CURE_CONFUSION:
                if (gBattleMons[bank].status2 & STATUS2_CONFUSION)
                {
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureConfusionRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureConfusionEnd2);
                    effect = ITEM_EFFECT_OTHER;
                }
                break;
			
            case ITEM_EFFECT_CURE_STATUS:
                if (gBattleMons[bank].status1 & STATUS_ANY || gBattleMons[bank].status2 & STATUS2_CONFUSION) {
                    i = 0;
                    if (gBattleMons[bank].status1 & STATUS_PSN_ANY) {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Poison);
                        i++;
                    }
					
                    if (gBattleMons[bank].status1 & STATUS_SLEEP) {
                        gBattleMons[bank].status2 &= ~(STATUS2_NIGHTMARE);
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Sleep);
                        i++;
                    }
					
                    if (gBattleMons[bank].status1 & STATUS_PARALYSIS) {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Paralysis);
                        i++;
                    }
					
                    if (gBattleMons[bank].status1 & STATUS_BURN) {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Burn);
                        i++;
                    }
					
                    if (gBattleMons[bank].status1 & STATUS_FREEZE) {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Ice);
                        i++;
                    }
					
                    if (gBattleMons[bank].status2 & STATUS2_CONFUSION) {
                        StringCopy(gBattleTextBuff1, gStatusConditionString_Confusion);
                        i++;
                    }
					
                    if (i < 2)
                        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                    else
                        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
                    gBattleMons[bank].status1 = 0;
                    gBattleMons[bank].status2 &= ~(STATUS2_CONFUSION);
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_BerryCureChosenStatusRet;
					}
					else
						BattleScriptExecute(BattleScript_BerryCureChosenStatusEnd2);
                    effect = ITEM_STATUS_CHANGE;
                }
                break;
			
			//Find out what strings displays in S/M
            case ITEM_EFFECT_CURE_ATTRACT:
				i = 0;
                if (gBattleMons[bank].status2 & STATUS2_INFATUATION)
				{
                    gBattleMons[bank].status2 &= ~(STATUS2_INFATUATION);
                    StringCopy(gBattleTextBuff1, gStatusConditionString_Love);
					++i;
				}
				if (gDisableStructs[bank].disabledMove)
				{
					gDisableStructs[bank].disabledMove = 0;
					gDisableStructs[bank].disableTimer1 = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_DisableProblem);
					++i;
                }
				if (gDisableStructs[bank].encoredMove)
				{
					gDisableStructs[bank].encoredMove = 0; //Same as end turn clear
					gDisableStructs[bank].encoreTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_EncoreProblem);
					++i;
                }
				if (gDisableStructs[bank].tauntTimer) 
				{
					gDisableStructs[bank].tauntTimer = 0;
					StringCopy(gBattleTextBuff1, gStatusConditionString_TauntProblem);
					++i;
				}
				if (i == 0)
					break;
				
				else if (i >= 2)
					StringCopy(gBattleTextBuff1, gStatusConditionString_MentalState);
				
				if (moveTurn)
				{
					BattleScriptPushCursor();
					
					if (DoPluck) //Fling
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusFling;
					else
						gBattlescriptCurrInstr = BattleScript_HerbCureChosenStatusRet;
				}
				else
					BattleScriptExecute(BattleScript_HerbCureChosenStatusEnd2);

                gBattleCommunication[MULTISTRING_CHOOSER] = 0;
                effect = ITEM_EFFECT_OTHER;	
                break;
			
			
			case ITEM_EFFECT_MICLE_BERRY:
				if (PINCH_BERRY_CHECK(bank) || DoPluck) {
					gNewBS->MicleBerryBits |= (1 << bank);
				
					if (moveTurn || DoPluck) {
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_MicleBerryRet;
					}
					else
						BattleScriptExecute(BattleScript_MicleBerryEnd2);
                    effect = ITEM_EFFECT_OTHER;
				}
				break;
			
			case ITEM_EFFECT_STICKY_BARB:
			STICKY_BARB_HURT:
				if (!moveTurn && ABILITY(bank) != ABILITY_MAGICGUARD) {
					gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 8);
					BattleScriptExecute(BattleScript_BlackSludgeHurt);
					effect = ITEM_EFFECT_OTHER;
				}
				break;
			
			case ITEM_EFFECT_BLACK_SLUDGE:
				if (!moveTurn) {
					if (IsOfType(bank, TYPE_POISON))
						goto LEFTOVERS_HEAL;
					else
						goto STICKY_BARB_HURT;
				}
			}
	
            if (effect)
            {
				if (GetPocketByItemId(gLastUsedItem) == POCKET_BERRY_POUCH)
					gNewBS->BelchCounters |= gBitTable[gBattlerPartyIndexes[bank]];
				
                gBattleScripting->bank = bank;
                gStringBank = bank;
                gActiveBattler = bank;
                switch (effect) {
					case ITEM_STATUS_CHANGE:
						EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[bank].status1);
						MarkBufferBankForExecution(gActiveBattler);
						break;
					case ITEM_PP_CHANGE:
						if (!(gBattleMons[bank].status2 & STATUS2_TRANSFORMED) && !(gDisableStructs[bank].mimickedMoves & gBitTable[i]))
							gBattleMons[bank].pp[i] = changedPP;
						break;
					case ITEM_HP_CHANGE:
						EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[bank].hp);
						MarkBufferBankForExecution(gActiveBattler);
						break;	
				}
            }
        }
        break;
	
    case ItemEffects_ContactTarget:
		if (DoPluck)
			gBattleScripting->bank = bank;
		else
			gBattleScripting->bank = bank = gBankTarget;
		
		if (SheerForceCheck())
			break;
		
		switch (bankHoldEffect) {
            case ITEM_EFFECT_ROCKY_HELMET:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& ABILITY(gBankAttacker) != ABILITY_MAGICGUARD
				&& CheckContact(gCurrentMove, gBankAttacker)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
				&& gBattleMons[gBankAttacker].hp) 
				{
                    gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gBankAttacker].maxHP, 6));
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_RockyHelmetDamage;
                    RecordItemBattle(bank, bankHoldEffect);
					gActiveBattler = gBankAttacker;
					effect = ITEM_HP_CHANGE;
				}
				break;
				
			case ITEM_EFFECT_WEAKNESS_POLICY:
				if (TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& gBattleMons[bank].hp
				&& (STAT_CAN_RISE(bank, STAT_STAGE_ATK) || STAT_CAN_RISE(bank, STAT_STAGE_SPATK))
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)) 
				{
					gEffectBank = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_WeaknessPolicy;
					gActiveBattler = gBankTarget;
					effect = ITEM_STATS_CHANGE;
				}
				break;
			
			case ITEM_EFFECT_ABSORB_BULB:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPATK, TYPE_WATER);
				break;
				
			case ITEM_EFFECT_AIR_BALLOON:
				if (TOOK_DAMAGE(bank) && MOVE_HAD_EFFECT) 
				{
					gEffectBank = bank;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AirBallooonPop;
					gActiveBattler = bank;
					effect = ITEM_EFFECT_OTHER;
				}
				break;
			
			case ITEM_EFFECT_CELL_BATTERY:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ELECTRIC);
				break;
				
			case ITEM_EFFECT_LUMINOUS_MOSS:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_SPDEF, TYPE_WATER);
				break;
				
			case ITEM_EFFECT_SNOWBALL:
				effect = RaiseStatsContactItem(bank, STAT_STAGE_ATK, TYPE_ICE);
				break;
				
			case ITEM_EFFECT_JABOCA_ROWAP_BERRY:	;
				if (!DoPluck) 
				{
					if (TOOK_DAMAGE(bank)
					&& MOVE_HAD_EFFECT
					&& gBattleMons[gBankAttacker].hp
					&& gBattleMons[bank].hp
					&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
					&& moveSplit == bankQuality) 
					{					
						gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 8);
						gEffectBank = gBankAttacker;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_JabocaRowapBerry;
						gActiveBattler = gBankAttacker;
						effect = ITEM_HP_CHANGE;
					}
				}
				break;
				
			case ITEM_EFFECT_KEE_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_DEF, SPLIT_PHYSICAL, DoPluck);
				break;
				
			case ITEM_EFFECT_MARANGA_BERRY:
				effect = KeeMaranagaBerryFunc(bank, STAT_STAGE_SPDEF, SPLIT_SPECIAL, DoPluck);
				break;
				
			case ITEM_EFFECT_ENIGMA_BERRY:
				if (!DoPluck
				&& TOOK_DAMAGE(bank)
				&& gMoveResultFlags == MOVE_RESULT_SUPER_EFFECTIVE
				&& gBattleMons[bank].hp
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)) 
				{
					gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 4);
                    if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
                        gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
                    gBattleMoveDamage *= -1;
                    BattleScriptExecute(BattleScript_ItemHealHP_End2);
                    effect = ITEM_HP_CHANGE;
				}
				break;
			
			case ITEM_EFFECT_STICKY_BARB:
				if (TOOK_DAMAGE(bank)
				&& MOVE_HAD_EFFECT
				&& CheckContact(gCurrentMove, gBankAttacker)
				&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
				&& gBattleMons[gBankAttacker].hp
				&& gBattleMons[gBankAttacker].item == 0) 
				{
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_StickyBarbTransfer;
					gActiveBattler = gBankAttacker;
					effect = ITEM_EFFECT_OTHER;
				}
				break;
		}
		
		if (effect && GetPocketByItemId(gLastUsedItem)== POCKET_BERRY_POUCH)
			gNewBS->BelchCounters |= gBitTable[gBattlerPartyIndexes[bank]];
		
	    if (effect == REQUEST_HP_BATTLE) 
		{
            gBattleScripting->bank = bank;
            gStringBank = bank;
            gActiveBattler = bank;
            EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].status1);
            MarkBufferBankForExecution(gActiveBattler);
            break;
        }
			
        break;
	
    case ItemEffects_ContactAttacker:
        if (gBattleMoveDamage)
        {
            switch (atkHoldEffect)
            {
            case ITEM_EFFECT_FLINCH:
				if (ABILITY(gBankAttacker) == ABILITY_SERENEGRACE || gNewBS->RainbowTimers[SIDE(gBankAttacker)])
					bankQuality *= 2;
                if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) 
				&& ABILITY(gBankTarget) != ABILITY_STENCH
                && TOOK_DAMAGE(gBankTarget)
				&& MOVE_HAD_EFFECT
                && (umodsi(Random(), 100)) < bankQuality
                && !CheckTableForMove(gCurrentMove, FlinchMoveTable)
                && gBattleMons[gBankTarget].hp)
                {
                    gBattleMons[gBankTarget].status2 |= STATUS2_FLINCHED;
                }
                break;
            case ITEM_EFFECT_SHELL_BELL:
                if (MOVE_HAD_EFFECT
                && gSpecialStatuses[gBankTarget].moveturnLostHP != 0
                && gSpecialStatuses[gBankTarget].moveturnLostHP != 0xFFFF
                && gBankAttacker != gBankTarget
                && gBattleMons[gBankAttacker].hp != gBattleMons[gBankAttacker].maxHP
                && gBattleMons[gBankAttacker].hp)
                {
                    gLastUsedItem = atkItem;
                    gStringBank = gBankAttacker;
                    gBattleScripting->bank = gBankAttacker;
                    gBattleMoveDamage = udivsi(gSpecialStatuses[gBankTarget].moveturnLostHP, atkQuality) * -1;
                    if (gBattleMoveDamage == 0)
                        gBattleMoveDamage = -1;
                    gSpecialStatuses[gBankTarget].moveturnLostHP = 0;
                    BattleScriptPushCursor();
                    gBattlescriptCurrInstr = BattleScript_ItemHealHP_Ret;
                    effect++;
                }
                break;
            }
        }
        break;
    }

    return effect;
}

static u8 ConfusionBerries(u8 bank, u8 flavour, bool8 moveTurn, bool8 DoPluck) {
	u8 effect = 0;
	
	#ifdef OLD_CONFUSION_HEAL_BERRIES
    if ((gBattleMons[bank].hp <= gBattleMons[bank].maxHP / 2 && !moveTurn)
	|| (DoPluck && gBattleMons[bank].hp != gBattleMons[bank].maxHP)) {
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 8;
        gBattleTextBuff1[2] = flavour;
        gBattleTextBuff1[3] = EOS;
        gBattleMoveDamage = gBattleMons[bank].maxHP / 8;
        if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
        if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
            gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
        gBattleMoveDamage *= -1;
		if (moveTurn || DoPluck) {
			BattleScriptPushCursor();
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet
		}
		else
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_ItemHealHP_RemoveItemEnd2);
        effect = ITEM_HP_CHANGE;
    }	
	#else
    if (PINCH_BERRY_CHECK(bank) || (DoPluck && gBattleMons[bank].hp != gBattleMons[bank].maxHP)) {
        gBattleTextBuff1[0] = 0xFD;
        gBattleTextBuff1[1] = 8;
        gBattleTextBuff1[2] = flavour;
        gBattleTextBuff1[3] = EOS;
        gBattleMoveDamage = gBattleMons[bank].maxHP / 2;
        if (gBattleMoveDamage == 0)
			gBattleMoveDamage = 1;
        if (gBattleMons[bank].hp + gBattleMoveDamage > gBattleMons[bank].maxHP)
            gBattleMoveDamage = gBattleMons[bank].maxHP - gBattleMons[bank].hp;
        gBattleMoveDamage *= -1;
		if (moveTurn || DoPluck) {
			BattleScriptPushCursor();
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				gBattlescriptCurrInstr = BattleScript_BerryConfuseHealRet;
			else
				gBattlescriptCurrInstr = BattleScript_ItemHealHP_RemoveItemRet;
		}
		else
			if (GetPokeFlavourRelation(gBattleMons[bank].personality, flavour) < 0)
				BattleScriptExecute(BattleScript_BerryConfuseHealEnd2);
			else
				BattleScriptExecute(BattleScript_ItemHealHP_RemoveItemEnd2);
        effect = ITEM_HP_CHANGE;
    }
	#endif
	return effect;
}

static u8 StatRaiseBerries(u8 bank, u8 stat, bool8 moveTurn, bool8 DoPluck) {
	u8 effect = 0;
    if ((PINCH_BERRY_CHECK(bank) || DoPluck) && STAT_CAN_RISE(bank, stat)) {
	
        PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
		PREPARE_STRING_BUFFER(gBattleTextBuff2, STRINGID_STATROSE);

        gEffectBank = bank;
        gBattleScripting->statChanger = INCREASE_1 | stat;
        gBattleScripting->animArg1 = 0xE + stat;
        gBattleScripting->animArg2 = 0;
		if (moveTurn || DoPluck) {
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		}
		else
			BattleScriptExecute(BattleScript_BerryStatRaiseEnd2);
        effect = ITEM_STATS_CHANGE;
    }
	return effect;
}

static u8 RaiseStatsContactItem(u8 bank, u8 stat, u8 moveType) {
	u8 effect = 0;
	u8 backupUser = gBankAttacker;
	gBankAttacker = bank;
	
	if (TOOK_DAMAGE(bank)
	&& gBattleStruct->dynamicMoveType == moveType
	&& gBattleMons[bank].hp
	&& !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0))
	{
		PREPARE_STAT_BUFFER(gBattleTextBuff1, stat);
		
        gEffectBank = bank;
        gBattleScripting->animArg1 = 0xE + stat;
        gBattleScripting->animArg2 = 0;
		
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_RaiseStatsItem;
		effect = ITEM_STATS_CHANGE;
	}
	
	gBankAttacker = backupUser;
	return effect;
}

static u8 KeeMaranagaBerryFunc(u8 bank, u8 stat, u8 split, bool8 DoPluck) {
	u8 effect = 0;
	u8 backupUser = gBankAttacker;
	gBankAttacker = bank;
	
	if (((TOOK_DAMAGE(bank) && CalcMoveSplit(gBankAttacker, gCurrentMove) == split && !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank)) || DoPluck)
	&& gBattleMons[bank].hp 
	&& !ChangeStatBuffs(SET_STAT_BUFF_VALUE(1), stat, MOVE_EFFECT_AFFECTS_USER | MOVE_EFFECT_CERTAIN, 0)) 
	{
		gEffectBank = gBankAttacker;
		gBattleScripting->animArg1 = 0xE + stat;
        gBattleScripting->animArg2 = 0;
		
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_BerryStatRaiseRet;
		effect = ITEM_STATS_CHANGE;
	}
	
	gBankAttacker = backupUser;
	return effect;
}
