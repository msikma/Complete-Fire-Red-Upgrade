#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"
#include "../include/money.h"
#include "../include/random.h"
#include "../include/constants/items.h"

#include "../include/new/ability_battle_effects.h"
#include "../include/new/ability_battle_scripts.h"
#include "../include/new/ability_tables.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/attackcanceler.h"
#include "../include/new/battle_strings.h"
#include "../include/new/battle_terrain.h"
#include "../include/new/damage_calc.h"
#include "../include/new/general_bs_commands.h"
#include "../include/new/helper_functions.h"
#include "../include/new/item_battle_scripts.h"
#include "../include/new/move_battle_scripts.h"
#include "../include/new/move_tables.h"
#include "../include/new/pickup_items.h"
#include "../include/new/switching.h"

#define TEXT_BUFFER_SIDE_STATUS(move, status, side) 			\
{																\
	gSideAffecting[side] &= ~status;							\
    gBattleTextBuff1[0] = 0xFD;									\
    gBattleTextBuff1[1] = 0x2;									\
    gBattleTextBuff1[2] = (move & 0xFF);						\
    gBattleTextBuff1[3] = move >> 8;							\
    gBattleTextBuff1[4] = EOS;									\
	BattleScriptPushCursor();									\
	gBattlescriptCurrInstr = BattleScript_SideStatusWoreOffRet;	\
}

//This file's functions:
static void TryContraryChangeStatAnim(u8 bank, u16* argumentPtr);
static item_t ChoosePickupItem(u8 level);

const u16 gMissStringIds[] =
{
    STRINGID_PKMNAVOIDEDATTACK, STRINGID_PKMNPROTECTEDITSELF,
    STRINGID_PKMNAVOIDEDATTACK, STRINGID_AVOIDEDDAMAGE,
    STRINGID_PKMNMAKESGROUNDMISS,
	0x184, //Crafty Shield
	0x184, //Mat Block
	0x184, //Quick Guard
	0x184, //Wide Guard
};

const u8* const EntryHazardsStrings[] =
{
	SpikesLayString,
	StealthRockLayString,
	ToxicSpikesLayString,
	StickyWebLayString,
};

void atk02_attackstring(void)
{
	u8 moveType = gBattleStruct->dynamicMoveType;

    if (gBattleExecBuffer) return;
	
	if (gBattlescriptCurrInstr == BattleScript_ButItFailedAttackstring)
		gMoveResultFlags |= MOVE_RESULT_FAILED;

    if (!(gHitMarker & (HITMARKER_NO_ATTACKSTRING | HITMARKER_ATTACKSTRING_PRINTED))) {

        PrepareStringBattle(STRINGID_USEDMOVE, gBankAttacker);

		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;

		if (!gNewBS->DancerInProgress && gCurrentMove != MOVE_COPYCAT) {
			gNewBS->LastUsedMove = gCurrentMove;
			gNewBS->LastUsedTypes[gBankAttacker] = moveType;

			if (!CheckTableForMove(gCurrentMove, MovesThatCallOtherMovesTable)) {
				if (gLastPrintedMoves[gBankAttacker] == gCurrentMove)
					gNewBS->MetronomeCounter[gBankAttacker] = MathMin(100, gNewBS->MetronomeCounter[gBankAttacker] + 20);
				else
					gNewBS->MetronomeCounter[gBankAttacker] = 0;
			}
		}

		if (gCurrentMove != MOVE_STRUGGLE && !(gHitMarker & HITMARKER_UNABLE_TO_USE_MOVE))
		{
			if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_GEM
			&&  ITEM_QUALITY(gBankAttacker) == moveType
			&&  SPLIT(gCurrentMove) != SPLIT_STATUS
			&& !(gMoveResultFlags & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
			&& !(TypeCalc(gCurrentMove, gBankAttacker, gBankTarget, GetBankPartyData(gBankAttacker), 0) & (MOVE_RESULT_MISSED | MOVE_RESULT_DOESNT_AFFECT_FOE | MOVE_RESULT_FAILED))
			&& gBattleMoves[gCurrentMove].effect != EFFECT_PLEDGE
			&& AttacksThisTurn(gBankAttacker, gCurrentMove) == 2)
			{
				gLastUsedItem = ITEM(gBankAttacker);
				gNewBS->GemHelper = TRUE;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_Gems;
			}

			if (ABILITY(gBankAttacker) == ABILITY_PROTEAN 
			&& !(gMoveResultFlags & MOVE_RESULT_FAILED)
			&& !CheckTableForMove(gCurrentMove, MovesThatCallOtherMovesTable))
			{
				if (gBattleMons[gBankAttacker].type1 != moveType
				||  gBattleMons[gBankAttacker].type2 != moveType
				|| (gBattleMons[gBankAttacker].type3 != moveType && gBattleMons[gBankAttacker].type3 != TYPE_BLANK))
				{
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_AbilityChangedType;
				}
			}
		}

		gHitMarker |= HITMARKER_ATTACKSTRING_PRINTED;
    }

	else {
		gBattlescriptCurrInstr++;
		gBattleCommunication[MSG_DISPLAY] = 0;
	}
}

void BufferAttackerItem(void)
{
	gLastUsedItem = ITEM(gBankAttacker);
}

void atk03_ppreduce(void) {
    s32 ppToDeduct = 1;

    if (gBattleExecBuffer) return;

    if (!gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure) {
        switch (gBattleMoves[gCurrentMove].target) {
			case MOVE_TARGET_FOES_AND_ALLY:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_ON_FIELD, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			case MOVE_TARGET_BOTH:
			case MOVE_TARGET_OPPONENTS_FIELD:
				ppToDeduct += AbilityBattleEffects(ABILITYEFFECT_COUNT_OTHER_SIDE, gBankAttacker, ABILITY_PRESSURE, 0, 0);
				break;

			default:
				if (gBankAttacker != gBankTarget && gBattleMons[gBankTarget].ability == ABILITY_PRESSURE)
					ppToDeduct++;
				break;
        }
    }

    if (!(gHitMarker & (HITMARKER_NO_PPDEDUCT | HITMARKER_NO_ATTACKSTRING))
	&& gNewBS->ParentalBondOn != 1
	&& !gNewBS->DancerInProgress
	&& gBattleMons[gBankAttacker].pp[gCurrMovePos]) {
        gProtectStructs[gBankAttacker].notFirstStrike = 1;

        if (gBattleMons[gBankAttacker].pp[gCurrMovePos] > ppToDeduct)
            gBattleMons[gBankAttacker].pp[gCurrMovePos] -= ppToDeduct;
        else
            gBattleMons[gBankAttacker].pp[gCurrMovePos] = 0;

        if (!(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)
            && !((gDisableStructs[gBankAttacker].mimickedMoves) & gBitTable[gCurrMovePos]))
        {
            gActiveBattler = gBankAttacker;
            EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + gCurrMovePos, 0, 1, &gBattleMons[gBankAttacker].pp[gCurrMovePos]);
            MarkBufferBankForExecution(gBankAttacker);
        }
    }

    gHitMarker &= ~(HITMARKER_NO_PPDEDUCT);
    gBattlescriptCurrInstr++;
}

void atk09_attackanimation(void)
{
    if (gBattleExecBuffer) return;

	if (ITEM_EFFECT(gBankTarget) == ITEM_EFFECT_WEAKNESS_BERRY && !AbilityBattleEffects(ABILITYEFFECT_CHECK_OTHER_SIDE, gBankTarget, ABILITY_UNNERVE, 0, 0))
	{
		if ((gMoveResultFlags & MOVE_RESULT_SUPER_EFFECTIVE && ITEM_QUALITY(gBankTarget) == gBattleStruct->dynamicMoveType)
		||  (ITEM_QUALITY(gBankTarget) == TYPE_NORMAL && gBattleStruct->dynamicMoveType == TYPE_NORMAL)) //Chilan Berry
		{
			gLastUsedItem = ITEM(gBankTarget);
			gBattleScripting->bank = gBankTarget;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WeaknessBerryActivate;
			return;
		}
	}

    if ((gHitMarker & HITMARKER_NO_ANIMATIONS) && (gCurrentMove != MOVE_TRANSFORM && gCurrentMove != MOVE_SUBSTITUTE))
    {
		if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
			gNewBS->attackAnimationPlayed = TRUE;

        BattleScriptPush(gBattlescriptCurrInstr + 1);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
        gBattleScripting->animTurn++;
        gBattleScripting->animTargetsHit++;
    }
    else
    {
        if (gNewBS->ParentalBondOn == 1
		|| (gBattleMoves[gCurrentMove].target & (MOVE_TARGET_BOTH | MOVE_TARGET_FOES_AND_ALLY | MOVE_TARGET_DEPENDS) && gBattleScripting->animTargetsHit)
		|| (gCurrentMove == MOVE_DEFOG && gBattleScripting->animTargetsHit))
        {
            gBattlescriptCurrInstr++;
            return;
        }
        if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT))
        {
            u8 multihit;

            gActiveBattler = gBankAttacker;

            if (gBattleMons[gBankTarget].status2 & STATUS2_SUBSTITUTE)
                multihit = gMultiHitCounter;
            else if (gMultiHitCounter != 0 && gMultiHitCounter != 1)
            {
                if (gBattleMons[gBankTarget].hp <= gBattleMoveDamage)
                    multihit = 1;
                else
                    multihit = gMultiHitCounter;
            }
            else
                multihit = gMultiHitCounter;

			gNewBS->attackAnimationPlayed = TRUE;
            EmitMoveAnimation(0, gCurrentMove, gBattleScripting->animTurn, gBattleMovePower, gBattleMoveDamage, gBattleMons[gBankAttacker].friendship, &gDisableStructs[gBankAttacker], multihit);
            gBattleScripting->animTurn += 1;
            gBattleScripting->animTargetsHit += 1;
            MarkBufferBankForExecution(gBankAttacker);
            gBattlescriptCurrInstr++;
        }
        else
        {
            BattleScriptPush(gBattlescriptCurrInstr + 1);
            gBattlescriptCurrInstr = BattleScript_Pausex20;
        }
    }
}

void atk0B_healthbarupdate(void) {
    if (gBattleExecBuffer) return;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG)) {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE
		&& gDisableStructs[gActiveBattler].substituteHP
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !gNewBS->bypassSubstitute)
            PrepareStringBattle(STRINGID_SUBSTITUTEDAMAGED, gActiveBattler);

		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE
		&& gBattleMons[gActiveBattler].species == SPECIES_MIMIKYU
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
		{
			gBattleScripting->bank = gBankTarget;
			BattleScriptPush(gBattlescriptCurrInstr + 2);
			gBattlescriptCurrInstr = BattleScript_MimikyuTookDamage;
			return;
		}

        else
		{
            s16 healthValue;
            s32 currDmg = gBattleMoveDamage;
            s32 maxPossibleDmgValue = 0xFFFF; //Ensures that huge damage values don't change sign
            if (currDmg <= maxPossibleDmgValue)
                healthValue = currDmg;
            else
                healthValue = maxPossibleDmgValue;

            EmitHealthBarUpdate(0, healthValue);
            MarkBufferBankForExecution(gActiveBattler);

            if (SIDE(gActiveBattler) == B_SIDE_PLAYER && gBattleMoveDamage > 0)
                gBattleResults->unk5_0 = 1;
        }
    }
    gBattlescriptCurrInstr += 2;
}

void atk0C_datahpupdate(void) {
    if (gBattleExecBuffer) return;

    if (!(gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gHitMarker & HITMARKER_NON_ATTACK_DMG)) {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

		//If Substitute Up
        if (gBattleMons[gActiveBattler].status2 & STATUS2_SUBSTITUTE
		&& gDisableStructs[gActiveBattler].substituteHP
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !gNewBS->bypassSubstitute) {
            if (gDisableStructs[gActiveBattler].substituteHP >= gBattleMoveDamage) {
                if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gBattleMoveDamage;
                gDisableStructs[gActiveBattler].substituteHP -= gBattleMoveDamage;
                gHpDealt = gBattleMoveDamage;
            }

            else {
                if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gDisableStructs[gActiveBattler].substituteHP;
                gHpDealt = gDisableStructs[gActiveBattler].substituteHP;
                gDisableStructs[gActiveBattler].substituteHP = 0;
            }

			gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
            //Check Substitute Fading
            if (gDisableStructs[gActiveBattler].substituteHP == 0) {
                gBattlescriptCurrInstr += 2;
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_SubstituteFade;
                return;
            }
        }

		//No Substitute
		else if (ABILITY(gActiveBattler) == ABILITY_DISGUISE
		&& gBattleMons[gActiveBattler].species == SPECIES_MIMIKYU
		&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
		&& !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED)) {
			if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
				gSpecialStatuses[gActiveBattler].moveturnLostHP = 1;
			gHpDealt = 1;
			gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
			gMoveResultFlags = 0;

			if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_PHYSICAL) {
				gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;
				gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
			}
			else if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_SPECIAL) {
				gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
				gSpecialStatuses[gActiveBattler].moveturnLostHP_special = gHpDealt;
				gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
				gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
			}

			gBattleScripting->bank = gActiveBattler;
			DoFormChange(gActiveBattler, SPECIES_MIMIKYU_BUSTED, TRUE, FALSE);
			gBattlescriptCurrInstr += 2;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_MimikyuTransform;
			return;
		}

        else
		{
            if (gBattleMoveDamage < 0) { // HP goes up
                gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                if (gBattleMons[gActiveBattler].hp > gBattleMons[gActiveBattler].maxHP)
                    gBattleMons[gActiveBattler].hp = gBattleMons[gActiveBattler].maxHP;
            }

            else { // HP goes down
                if (gHitMarker & HITMARKER_x20)
                    gHitMarker &= ~(HITMARKER_x20);

                else {
                    gTakenDmg[gActiveBattler] += gBattleMoveDamage;
                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
                        gTakenDmgBanks[gActiveBattler] = gBankAttacker;
                    else
                        gTakenDmgBanks[gActiveBattler] = gBankTarget;
                }


                if (gBattleMons[gActiveBattler].hp > gBattleMoveDamage) {
                    gBattleMons[gActiveBattler].hp -= gBattleMoveDamage;
                    gHpDealt = gBattleMoveDamage;
                }

                else {
                    gHpDealt = gBattleMons[gActiveBattler].hp;
                    gBattleMons[gActiveBattler].hp = 0;
                }


                if (!gSpecialStatuses[gActiveBattler].moveturnLostHP && !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
                    gSpecialStatuses[gActiveBattler].moveturnLostHP = gHpDealt;

                if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_PHYSICAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
                    gProtectStructs[gActiveBattler].physicalDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].moveturnLostHP_physical = gHpDealt;

                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET) {
                        gProtectStructs[gActiveBattler].physicalBank = gBankAttacker;
                        gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankAttacker;
						gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
                    }

                    else {
                        gProtectStructs[gActiveBattler].physicalBank = gBankTarget;
                        gSpecialStatuses[gActiveBattler].moveturnPhysicalBank = gBankTarget;
                    }
                }

                else if (CalcMoveSplit(gActiveBattler, gCurrentMove) == SPLIT_SPECIAL
				&& !(gHitMarker & HITMARKER_NON_ATTACK_DMG))
				{
                    gProtectStructs[gActiveBattler].specialDmg = gHpDealt;
                    gSpecialStatuses[gActiveBattler].moveturnLostHP_special = gHpDealt;

                    if (gBattlescriptCurrInstr[1] == BS_GET_TARGET) {
                        gProtectStructs[gActiveBattler].specialBank = gBankAttacker;
                        gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankAttacker;
						gNewBS->AttackerDidDamageAtLeastOnce = TRUE;
                    }

                    else {
                        gProtectStructs[gActiveBattler].specialBank = gBankTarget;
                        gSpecialStatuses[gActiveBattler].moveturnSpecialBank = gBankTarget;
                    }
                }

				if (gStatuses3[gActiveBattler] & STATUS3_ILLUSION
				&& !(gHitMarker & HITMARKER_IGNORE_SUBSTITUTE)
				&& gBattleMons[gActiveBattler].hp == 0)
				{
					gBattleScripting->bank = gActiveBattler;
					gStatuses3[gActiveBattler] &= ~(STATUS3_ILLUSION);
					BattleScriptPush(gBattlescriptCurrInstr + 2);
					gBattlescriptCurrInstr = BattleScript_IllusionBrokenFaint - 2;
				}

				gHitMarker &= ~(HITMARKER_IGNORE_SUBSTITUTE);
            }
            gHitMarker &= ~(HITMARKER_NON_ATTACK_DMG);
            EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &gBattleMons[gActiveBattler].hp);
            MarkBufferBankForExecution(gActiveBattler);
        }
    }
    else
    {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
        if (gSpecialStatuses[gActiveBattler].moveturnLostHP == 0)
            gSpecialStatuses[gActiveBattler].moveturnLostHP = 0xFFFF;
    }
    gBattlescriptCurrInstr += 2;
}

void atk0D_critmessage(void) {
    if (!gBattleExecBuffer) {
        if (gCritMultiplier > 100 && !(gMoveResultFlags & MOVE_RESULT_NO_EFFECT)) {
            PrepareStringBattle(STRINGID_CRITICALHIT, gBankAttacker);
            gBattleCommunication[MSG_DISPLAY] = 1;
        }
        gBattlescriptCurrInstr++;
    }
}

void atk0F_resultmessage(void) {
    u32 stringId = 0;

    if (gBattleExecBuffer) return;

    if (gMoveResultFlags & MOVE_RESULT_MISSED && (!(gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE) || gBattleCommunication[6] > 2))
	{
		if (gBattleCommunication[6] > 2) //Levitate + Wonder Guard
		{
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattleScripting->bank = gBankTarget;
			gBattlescriptCurrInstr = BattleScript_AvoidedMoveWithAbility;
			gBattleCommunication[MSG_DISPLAY] = 0;
			return;
		}
		else
		{
			stringId = gMissStringIds[gBattleCommunication[6]];
			gBattleCommunication[MSG_DISPLAY] = 1;
		}
    }

    else {
        gBattleCommunication[MSG_DISPLAY] = 1;
        switch (gMoveResultFlags & (u8)(~(MOVE_RESULT_MISSED))) {
        case MOVE_RESULT_SUPER_EFFECTIVE:
            stringId = STRINGID_SUPEREFFECTIVE;
            break;

        case MOVE_RESULT_NOT_VERY_EFFECTIVE:
            stringId = STRINGID_NOTVERYEFFECTIVE;
            break;

        case MOVE_RESULT_ONE_HIT_KO:
            stringId = STRINGID_ONEHITKO;
            break;

        case MOVE_RESULT_FOE_ENDURED:
			if (gNewBS->EnduranceHelper == ENDURE_STURDY)
			{
				gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED);
				gNewBS->EnduranceHelper = 0;
				gProtectStructs[gBankTarget].enduredSturdy = 0;
				gBattleScripting->bank = gBankTarget;
				BattleScriptPushCursor();
				gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				return;
			}
			else
				stringId = STRINGID_PKMNENDUREDHIT;
            break;

        case MOVE_RESULT_FAILED:
            stringId = STRINGID_BUTITFAILED;
            break;

        case MOVE_RESULT_DOESNT_AFFECT_FOE:
            stringId = STRINGID_ITDOESNTAFFECT;
            break;

        case MOVE_RESULT_FOE_HUNG_ON:
            gLastUsedItem = gBattleMons[gBankTarget].item;
            gStringBank = gBankTarget;
            gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
            BattleScriptPushCursor();
			if (gNewBS->EnduranceHelper == ENDURE_FOCUS_SASH) {
				gNewBS->EnduranceHelper = 0;
				gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
			}
			else
				gBattlescriptCurrInstr = BattleScript_HangedOnMsg;
            return;

        default:
            if (gMoveResultFlags & MOVE_RESULT_DOESNT_AFFECT_FOE)
                stringId = STRINGID_ITDOESNTAFFECT;

            else if (gMoveResultFlags & MOVE_RESULT_ONE_HIT_KO) {
                gMoveResultFlags &= ~(MOVE_RESULT_ONE_HIT_KO);
                gMoveResultFlags &= ~(MOVE_RESULT_SUPER_EFFECTIVE);
                gMoveResultFlags &= ~(MOVE_RESULT_NOT_VERY_EFFECTIVE);
                BattleScriptPushCursor();
                gBattlescriptCurrInstr = BattleScript_OneHitKOMsg;
                return;
            }

            else if (gMoveResultFlags & MOVE_RESULT_FOE_ENDURED) {
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
				if (gNewBS->EnduranceHelper == ENDURE_STURDY) {
					gNewBS->EnduranceHelper = 0;
					gProtectStructs[gBankTarget].enduredSturdy = 0;
					gBattleScripting->bank = gBankTarget;
					gBattlescriptCurrInstr = BattleScript_EnduredSturdy;
				}
				else
					gBattlescriptCurrInstr = BattleScript_EnduredMsg;
                return;
            }

            else if (gMoveResultFlags & MOVE_RESULT_FOE_HUNG_ON) {
                gLastUsedItem = gBattleMons[gBankTarget].item;
                gStringBank = gBankTarget;
                gMoveResultFlags &= ~(MOVE_RESULT_FOE_ENDURED | MOVE_RESULT_FOE_HUNG_ON);
                BattleScriptPushCursor();
                if (gNewBS->EnduranceHelper == ENDURE_FOCUS_SASH) {
					gNewBS->EnduranceHelper = 0;
					gBattlescriptCurrInstr = BattleScript_HangedOnFocusSash;
				}
				else
					gBattlescriptCurrInstr = BattleScript_HangedOnMsg;

				gSpecialStatuses[gBankTarget].focusBanded = FALSE;
                return;
            }

            else if (gMoveResultFlags & MOVE_RESULT_FAILED)  {
                stringId = STRINGID_BUTITFAILED;
            }

            else {
                gBattleCommunication[MSG_DISPLAY] = 0;
			}
        }
    }

    if (stringId)
        PrepareStringBattle(stringId, gBankAttacker);

    gBattlescriptCurrInstr++;
}

void atk12_waitmessage(void)
{
    if (gBattleExecBuffer == 0)
    {
        if (!gBattleCommunication[MSG_DISPLAY])
        {
            gBattlescriptCurrInstr += 3;
        }
        else
        {
            u16 toWait = T2_READ_16(gBattlescriptCurrInstr + 1);
            if (++gPauseCounterBattle >= toWait || gMain.newKeys & A_BUTTON)
            {
                gPauseCounterBattle = 0;
                gBattlescriptCurrInstr += 3;
                gBattleCommunication[MSG_DISPLAY] = 0;
            }
        }
    }
}

void atk19_tryfaintmon(void)
{
    u8* BS_ptr;

    if (gBattlescriptCurrInstr[2] != 0)
    {
        gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
        if (gHitMarker & HITMARKER_FAINTED(gActiveBattler))
        {
            BS_ptr = T1_READ_PTR(gBattlescriptCurrInstr + 3);

            BattleScriptPop();
            gBattlescriptCurrInstr = BS_ptr;
            gSideAffecting[SIDE(gActiveBattler)] &= ~(SIDE_STATUS_SPIKES_DAMAGED);
        }
        else
        {
            gBattlescriptCurrInstr += 7;
        }
    }
    else
    {
        u8 bank;

        if (gBattlescriptCurrInstr[1] == BS_GET_ATTACKER)
        {
            gActiveBattler = gBankAttacker;
            bank = gBankTarget;
            BS_ptr = BattleScript_FaintAttacker;
        }
        else if (gBattlescriptCurrInstr[1] == BS_GET_SCRIPTING_BANK)
        {
            gActiveBattler = gBattleScripting->bank;
            bank = gBattleScripting->bank;
            BS_ptr = BattleScript_FaintScriptingBank;
        }
        else
        {
            gActiveBattler = gBankTarget;
            bank = gBankAttacker;
            BS_ptr = BattleScript_FaintTarget;
        }
        if (!(gAbsentBattlerFlags & gBitTable[gActiveBattler])
        && gBattleMons[gActiveBattler].hp == 0)
        {
            gHitMarker |= HITMARKER_FAINTED(gActiveBattler);
            BattleScriptPush(gBattlescriptCurrInstr + 7);
            gBattlescriptCurrInstr = BS_ptr;
            if (SIDE(gActiveBattler) == B_SIDE_PLAYER)
            {
                gHitMarker |= HITMARKER_x400000;
                if (gBattleResults->playerFaintCounter < 0xFF)
                    gBattleResults->playerFaintCounter++;
                AdjustFriendshipOnBattleFaint(gActiveBattler);
            }
            else
            {
                if (gBattleResults->opponentFaintCounter < 0xFF)
                    gBattleResults->opponentFaintCounter++;
                gBattleResults->lastOpponentSpecies = GetMonData(&gEnemyParty[gBattlerPartyIndexes[gActiveBattler]], MON_DATA_SPECIES, NULL);
            }

			gNewBS->RetaliateCounters[SIDE(gActiveBattler)] = 2;

            if ((gHitMarker & HITMARKER_DESTINYBOND)
			&& gBattleMons[gBankAttacker].hp != 0
			&& gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
            {
                gHitMarker &= ~(HITMARKER_DESTINYBOND);
				const u8* backupScript = gBattlescriptCurrInstr;
				gBattlescriptCurrInstr = BattleScript_DestinyBondTakesLife;
                BattleScriptPushCursor();
                gBattleMoveDamage = gBattleMons[bank].hp;
                gBattlescriptCurrInstr = backupScript;
            }

            if ((gStatuses3[gBankTarget] & STATUS3_GRUDGE)
             && !(gHitMarker & HITMARKER_GRUDGE)
             && SIDE(gBankAttacker) != SIDE(gBankTarget)
             && gBattleMons[gBankAttacker].hp != 0
             && gCurrentMove != MOVE_STRUGGLE
			 && gBattlescriptCurrInstr[1] != BS_GET_SCRIPTING_BANK)
            {
                u8 moveIndex = gBattleStruct->chosenMovePositions[gBankAttacker];

                gBattleMons[gBankAttacker].pp[moveIndex] = 0;
                BattleScriptPush(gBattlescriptCurrInstr);
                gBattlescriptCurrInstr = BattleScript_GrudgeTakesPp;
                gActiveBattler = gBankAttacker;
                EmitSetMonData(0, moveIndex + REQUEST_PPMOVE1_BATTLE, 0, 1, &gBattleMons[gActiveBattler].pp[moveIndex]);
                MarkBufferBankForExecution(gActiveBattler);

                PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[gBankAttacker].moves[moveIndex])
            }
        }
        else
        {
            gBattlescriptCurrInstr += 7;
        }
    }
}

void atk1B_cleareffectsonfaint(void) {
	int i;
	gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 partner = PARTNER(gActiveBattler);
	pokemon_t* mon = GetBankPartyData(gActiveBattler);

    if (!gBattleExecBuffer) {
		switch (gNewBS->FaintEffectsTracker) {
			case Faint_ClearEffects:
				gBattleMons[gActiveBattler].status1 = 0;
				EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 0x4, &gBattleMons[gActiveBattler].status1);
				MarkBufferBankForExecution(gActiveBattler);
				UndoEffectsAfterFainting();

				ClearSwitchBytes(gActiveBattler);
				ClearSwitchBits(gActiveBattler);

				gProtectStructs[gActiveBattler].KingsShield = 0;	//Necessary because could be sent away with Roar
				gProtectStructs[gActiveBattler].SpikyShield = 0;
				gProtectStructs[gActiveBattler].BanefulBunker = 0;
				gProtectStructs[gActiveBattler].enduredSturdy = 0;

				gNewBS->ZMoveData->toBeUsed[gActiveBattler] = 0; //Because you died before you could use the Z-Move

				gBattleMons[gActiveBattler].type3 = TYPE_BLANK;
				*SeedHelper = 0;
				++gNewBS->FaintEffectsTracker;
			__attribute__ ((fallthrough));

			case Faint_SoulHeart:
				for (; *SeedHelper < gBattlersCount; ++*SeedHelper)
				{
					u8 bank = *SeedHelper;
					if (ABILITY(bank) == ABILITY_SOULHEART
					&&  bank != gActiveBattler
					&&  gBattleMons[bank].hp != 0
					&&  STAT_CAN_RISE(bank, STAT_SPATK)
					&&  ViableMonCountFromBank(FOE(bank)) > 0) //Foe still has mons
					{
						++gBattleMons[bank].statStages[STAT_SPATK - 1];
						gBattleScripting->bank = gEffectBank = bank;

						PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_STAGE_SPATK);
						PREPARE_STAT_ROSE(gBattleTextBuff2);
						gBattleScripting->animArg1 = 0xE + STAT_STAGE_SPATK;
						gBattleScripting->animArg2 = 0;

						gLastUsedAbility = ABILITY_SOULHEART;

						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_SoulHeart;
						++*SeedHelper;
						return;
					}
				}
				++gNewBS->FaintEffectsTracker;
			__attribute__ ((fallthrough));

			case Faint_ReceiverActivate:
				gNewBS->ReceiverActivated = FALSE;
				u8 partnerAbility = ABILITY(partner);

				if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
				&& (partnerAbility == ABILITY_RECEIVER || partnerAbility == ABILITY_POWEROFALCHEMY)
				&& !CheckTableForAbility(CopyAbility(gActiveBattler), ReceiverBanTable))
				{
					//Put Ability Activation Here Eventually
					gLastUsedAbility = partnerAbility;
					gBattleMons[partner].ability = CopyAbility(gActiveBattler);
					gEffectBank = gActiveBattler;
					gBattleScripting->bank = partner;
					gNewBS->ReceiverActivated = TRUE;
					BattleScriptPushCursor();
					gBattlescriptCurrInstr = BattleScript_Receiver;
					
					AbilityPopUpHelper = gLastUsedAbility;
					EmitDataTransfer(0, &AbilityPopUpHelper, 1, &AbilityPopUpHelper);
					MarkBufferBankForExecution(gActiveBattler);
	
					++gNewBS->FaintEffectsTracker;
					return;
				}
				++gNewBS->FaintEffectsTracker;
			__attribute__ ((fallthrough));

			case Faint_SwitchInAbilities: //Now null b/c handled in BS
				++gNewBS->FaintEffectsTracker;
			__attribute__ ((fallthrough));

			case Faint_PrimalWeather:	;
				bool8 DoWeatherLoop = FALSE;

				switch (ABILITY(gActiveBattler)) {
				case ABILITY_PRIMORDIALSEA:
					if (gBattleWeather & WEATHER_RAIN_PRIMAL) {
						BattleStringLoader = PrimalRainEndString;
						DoWeatherLoop = TRUE;
					}
					break;
				case ABILITY_DESOLATELAND:
					if (gBattleWeather & WEATHER_SUN_PRIMAL) {
						BattleStringLoader = PrimalSunEndString;
						DoWeatherLoop = TRUE;
					}
					break;
				case ABILITY_DELTASTREAM:
					if (gBattleWeather & WEATHER_AIR_CURRENT_PRIMAL) {
						BattleStringLoader = PrimalAirCurrentEndString;
						DoWeatherLoop = TRUE;
					}
				}

				if (DoWeatherLoop) {

					for (i = 0; i < gBattlersCount; ++i) {
						if (i == gActiveBattler) continue;
						if (ABILITY(i) == ABILITY(gActiveBattler)) break;
					}

					if (i == gBattlersCount) {
						gBattleWeather = 0;
						gWeatherCounter = 0;
						BattleScriptPushCursor();
						gBattlescriptCurrInstr = BattleScript_PrimalWeatherEnd;
						return;
					}
				}
				++gNewBS->FaintEffectsTracker;
			__attribute__ ((fallthrough));

			case Faint_FormsRevert:
				if (mon->backupSpecies)
				{
					EmitSetMonData(0, REQUEST_SPECIES_BATTLE, 0, 2, &mon->backupSpecies);
					MarkBufferBankForExecution(gActiveBattler);
					mon->backupSpecies = 0;
					++gNewBS->FaintEffectsTracker;
					return;
				}
				break; //No form change means skip the next two states

			case Faint_FormsStats:
				CalculateMonStats(mon);
				EmitSetRawMonData(0, offsetof(pokemon_t, attack), 2 /*Atk*/ + 2 /*Def*/ + 2 /*Spd*/ + 2 */*Sp Atk*/ + 2 /*Sp Def*/, &mon->attack); //Reload all stats
				MarkBufferBankForExecution(gActiveBattler);
				++gNewBS->FaintEffectsTracker;
				return;

			case Faint_FormsHP: ;
				u16 oldHP, newHP;
				oldHP = mon->hp;

				if (mon->species == SPECIES_ZYGARDE || mon->species == SPECIES_ZYGARDE_10)
				{
					newHP = MathMin(mon->maxHP, oldHP);
					EmitSetMonData(0, REQUEST_HP_BATTLE, 0, 2, &newHP);
					MarkBufferBankForExecution(gActiveBattler);
					++gNewBS->FaintEffectsTracker;
					return;
				}
		}

		gNewBS->FaintEffectsTracker = 0;
        gBattlescriptCurrInstr += 2;
    }
}

void atk1D_jumpifstatus2(void) {
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
    u32 flags = T2_READ_32(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET && !MoveBlockedBySubstitute(gCurrentMove, gBankAttacker, bank))
		flags &= ~(STATUS2_SUBSTITUTE);

    if (gBattleMons[bank].status2 & flags && gBattleMons[bank].hp)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 10;
}

void atk1F_jumpifsideaffecting(void) {
    u8 side;
    u16 flags = T2_READ_16(gBattlescriptCurrInstr + 2);
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 4);

    if (gBattlescriptCurrInstr[1] == 1)
        side = GetBattlerPosition(gBankAttacker) & 1;

    else {
        side = GetBattlerPosition(gBankTarget) & 1;
		if (ABILITY(gBankAttacker) == ABILITY_INFILTRATOR)
			side &= ~(SIDE_STATUS_SAFEGUARD | SIDE_STATUS_MIST);
	}

    if (gSideAffecting[side] & flags)
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 8;
}

void atk22_jumpiftype(void) //u8 bank, u8 type, *ptr
{
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
    u8 type = gBattlescriptCurrInstr[2];
    void* jump_loc = T2_READ_PTR(gBattlescriptCurrInstr + 3);

    if (IsOfType(bank, type))
        gBattlescriptCurrInstr = jump_loc;
    else
        gBattlescriptCurrInstr += 7;
}

void MoveValuesCleanUp(void)
{
    gMoveResultFlags = 0;
    gBattleScripting->dmgMultiplier = 1;
    gCritMultiplier = 100;
    gBattleCommunication[MOVE_EFFECT_BYTE] = 0;
	gNewBS->backupMoveEffect = 0;
    gBattleCommunication[6] = 0;
    gHitMarker &= ~(HITMARKER_DESTINYBOND);
    gHitMarker &= ~(HITMARKER_SYNCHRONISE_EFFECT);
}

void atk27_decrementmultihit(void)
{
    if (gMultiHitCounter == 0 || --gMultiHitCounter == 0)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atk40_jumpifaffectedbyprotect(void)
{
    if (ProtectAffects(gCurrentMove, gBankAttacker, gBankTarget, FALSE))
    {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        JumpIfMoveFailed(5, 0);
        gBattleCommunication[6] = 1;
    }
    else
        gBattlescriptCurrInstr += 5;
}

void atk42_jumpiftype2(void) //u8 bank, u8 type, *ptr
{
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);

    if (IsOfType(bank, gBattlescriptCurrInstr[2]))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 3);
    else
        gBattlescriptCurrInstr += 7;
}

static void TryContraryChangeStatAnim(u8 bank, u16* argumentPtr)
{
	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		u8 value = 0;
		switch (GET_STAT_BUFF_VALUE2(gBattleScripting->statChanger)) {
			case SET_STAT_BUFF_VALUE(1): // +1
				value = STAT_ANIM_MINUS1;
				break;
			case SET_STAT_BUFF_VALUE(2): // +2
				value = STAT_ANIM_MINUS2;
				break;
			case SET_STAT_BUFF_VALUE(1) | STAT_BUFF_NEGATIVE: // -1
				value = STAT_ANIM_PLUS1;
				break;
			case SET_STAT_BUFF_VALUE(2) | STAT_BUFF_NEGATIVE: // -2
				value = STAT_ANIM_PLUS2;
				break;
		}
		*argumentPtr = GET_STAT_BUFF_ID(gBattleScripting->statChanger) + value - 1;
	}
}

void atk45_playanimation(void)
{
    u16* argumentPtr;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 3);

	if (gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (gBattlescriptCurrInstr[2] == B_ANIM_CASTFORM_CHANGE
	||	gBattlescriptCurrInstr[2] == B_ANIM_STATS_CHANGE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_SNATCH_MOVE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_SUBSTITUTE_FADE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_WISHIWASHI_FISH
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_RED_PRIMAL_REVERSION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ZMOVE_ACTIVATE
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_MEGA_EVOLUTION
	|| 	gBattlescriptCurrInstr[2] == B_ANIM_ULTRA_BURST
	||  gBattlescriptCurrInstr[2] == B_ANIM_LOAD_DEAFUALT_BG
	||  gBattlescriptCurrInstr[2] == B_ANIM_LOAD_ABILITY_POP_UP
	||  gBattlescriptCurrInstr[2] == B_ANIM_DESTROY_ABILITY_POP_UP)
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
	else if (gBattlescriptCurrInstr[2] == B_ANIM_TRANSFORM_MOVE)
	{
		EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 7;
	}
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        BattleScriptPush(gBattlescriptCurrInstr + 7);
        gBattlescriptCurrInstr = BattleScript_Pausex20;
    }
    else if (gBattlescriptCurrInstr[2] == B_ANIM_RAIN_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_SUN_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_SANDSTORM_CONTINUES
          || gBattlescriptCurrInstr[2] == B_ANIM_HAIL_CONTINUES
	      || gBattlescriptCurrInstr[2] == B_ANIM_STRONG_WINDS_CONTINUE
	      || gBattlescriptCurrInstr[2] == B_ANIM_FOG_CONTINUES)
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 7;
    }
    else
    {
        EmitBattleAnimation(0, gBattlescriptCurrInstr[2], *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 7;
    }
}

void atk46_playanimation2(void) // animation Id is stored in the first pointer
{
    u16* argumentPtr;
    const u8* animationIdPtr;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    animationIdPtr = T2_READ_PTR(gBattlescriptCurrInstr + 2);
    argumentPtr = T2_READ_PTR(gBattlescriptCurrInstr + 6);

	if (*animationIdPtr == B_ANIM_STATS_CHANGE)
		TryContraryChangeStatAnim(gActiveBattler, argumentPtr);

	if (*animationIdPtr == B_ANIM_CASTFORM_CHANGE
	||	*animationIdPtr == B_ANIM_STATS_CHANGE
	|| 	*animationIdPtr == B_ANIM_SNATCH_MOVE
	|| 	*animationIdPtr == B_ANIM_SUBSTITUTE_FADE
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_WISHIWASHI_FISH
	|| 	*animationIdPtr == B_ANIM_ZYGARDE_CELL_SWIRL
	|| 	*animationIdPtr == B_ANIM_BLUE_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_RED_PRIMAL_REVERSION
	|| 	*animationIdPtr == B_ANIM_TRANSFORM
	|| 	*animationIdPtr == B_ANIM_ZMOVE_ACTIVATE
	|| 	*animationIdPtr == B_ANIM_MEGA_EVOLUTION
	|| 	*animationIdPtr == B_ANIM_ULTRA_BURST
	||  *animationIdPtr == B_ANIM_LOAD_DEAFUALT_BG
	||  *animationIdPtr == B_ANIM_LOAD_ABILITY_POP_UP
	||  *animationIdPtr == B_ANIM_DESTROY_ABILITY_POP_UP)
	{
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
	else if (*animationIdPtr == B_ANIM_TRANSFORM_MOVE)
	{
		EmitMoveAnimation(0, MOVE_TRANSFORM, 0, 1, 1, 0xFF, &gDisableStructs[gActiveBattler], 0);
		MarkBufferBankForExecution(gActiveBattler);
		gBattlescriptCurrInstr += 10;
	}
    else if (gHitMarker & HITMARKER_NO_ANIMATIONS)
    {
        gBattlescriptCurrInstr += 10;
    }
    else if (*animationIdPtr == B_ANIM_RAIN_CONTINUES
             || *animationIdPtr == B_ANIM_SUN_CONTINUES
             || *animationIdPtr == B_ANIM_SANDSTORM_CONTINUES
             || *animationIdPtr == B_ANIM_HAIL_CONTINUES
			 || *animationIdPtr == B_ANIM_STRONG_WINDS_CONTINUE
			 || *animationIdPtr == B_ANIM_FOG_CONTINUES)
    {
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
    else if (gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE)
    {
        gBattlescriptCurrInstr += 10;
    }
    else
    {
        EmitBattleAnimation(0, *animationIdPtr, *argumentPtr);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 10;
    }
}

void atk63_jumptocalledmove(void)
{
    if (gBattlescriptCurrInstr[1])
        gCurrentMove = gRandomMove;
    else
        gChosenMove = gCurrentMove = gRandomMove;

	if (gBattlescriptCurrInstr[1] != 0xFF)
		gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
    gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
}

void atk64_statusanimation(void) {
    if (gBattleExecBuffer) return;

    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
            EmitStatusAnimation(0, 0, gBattleMons[gActiveBattler].status1);
            MarkBufferBankForExecution(gActiveBattler);
    }
    gBattlescriptCurrInstr += 2;
}

void atk66_chosenstatusanimation(void) {
    if (gBattleExecBuffer) return;

    u32 status;
    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    status = T1_READ_32(gBattlescriptCurrInstr + 3);

	if (!(gStatuses3[gActiveBattler] & STATUS3_SEMI_INVULNERABLE) && gDisableStructs[gActiveBattler].substituteHP == 0 && !(gHitMarker & HITMARKER_NO_ANIMATIONS)) {
            EmitStatusAnimation(0, gBattlescriptCurrInstr[2], status);
            MarkBufferBankForExecution(gActiveBattler);
    }
    gBattlescriptCurrInstr += 7;
}

void atk6A_removeitem(void) {
    u8 bank = gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
	u8 oldItemEffect = ITEM_EFFECT(bank);
	gLastUsedItem = gBattleMons[bank].item;

	if (!gNewBS->IncinerateCounters[bank] && gLastUsedItem) {
		CONSUMED_ITEMS(bank) = gLastUsedItem;
		SAVED_CONSUMED_ITEMS(bank) = gLastUsedItem;
		gNewBS->UnburdenBoosts |= 1 << bank;
		AddBankToPickupStack(bank);
	}

    gBattleMons[bank].item = 0;
    EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
    MarkBufferBankForExecution(bank);
    gBattlescriptCurrInstr += 2;

	u8 partner = PARTNER(bank);
	u16 partnerItem = gBattleMons[partner].item;

	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&& gNewBS->NoSymbiosisByte == FALSE
	&& ABILITY(partner) == ABILITY_SYMBIOSIS
	&& (!gNewBS->IncinerateCounters[bank] || oldItemEffect == ITEM_EFFECT_AIR_BALLOON) //Air Balloons can't be restored by Recycle, but they trigger Symbiosis
	&& !(gWishFutureKnock->knockedOffPokes[SIDE(bank)] & gBitTable[gBattlerPartyIndexes[bank]])
	&& gBattleMons[partner].item
	&& CanTransferItem(gBattleMons[bank].species, partnerItem, GetBankPartyData(bank))
	&& CanTransferItem(gBattleMons[partner].species, partnerItem, GetBankPartyData(bank))) {
		gBattleMons[bank].item = partnerItem;
		gBattleMons[partner].item = 0;

		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[bank].item);
		MarkBufferBankForExecution(bank);

		EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[partner].item);
		MarkBufferBankForExecution(partner);

		gEffectBank = bank;
		gBattleScripting->bank = partner;

		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_Symbiosis;
	}

	gNewBS->NoSymbiosisByte = FALSE;
	gNewBS->IncinerateCounters[bank] = 0;
	gWishFutureKnock->knockedOffPokes[SIDE(bank)] &= ~(gBitTable[gBattlerPartyIndexes[bank]]);
}

void atk70_recordlastability(void) {
    gActiveBattler = GetBattleBank(gBattlescriptCurrInstr[1]);
    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
    gBattlescriptCurrInstr += 2;
}

void atk77_setprotect(void) {
    bool8 not_last_turn = TRUE;
    u16 last_move = gLastResultingMoves[gBankAttacker];
	u16 rate = 0xFFFF;
	u16 divisor = 1;
	int i;

	switch (last_move) {
		case MOVE_PROTECT:
		case MOVE_DETECT:
		case MOVE_ENDURE:
		case MOVE_KINGSSHIELD:
		case MOVE_SPIKYSHIELD:
		case MOVE_BANEFULBUNKER:
		case MOVE_CRAFTYSHIELD:
		case MOVE_MATBLOCK:
		case MOVE_QUICKGUARD:
		case MOVE_WIDEGUARD:
			break;
		default:
			gDisableStructs[gBankAttacker].protectUses = 0;
	}

    if (gCurrentTurnActionNumber == (gBattlersCount - 1)) {
        not_last_turn = FALSE;
	}

	if (gCurrentMove == MOVE_WIDEGUARD || gCurrentMove == MOVE_QUICKGUARD || gCurrentMove == MOVE_CRAFTYSHIELD || gCurrentMove ==MOVE_MATBLOCK)
		divisor = 1; //These moves increase the fail rate, but can't fail themselves
	else {
		for (i = 0; i < gDisableStructs[gBankAttacker].protectUses && i <= 6; ++i)
			divisor *= 3;
	}

    if (udivsi(rate, divisor) >= Random() && not_last_turn) {

        u8 atkSide = SIDE(gBankAttacker);
		switch (gCurrentMove) {
			case MOVE_KINGSSHIELD:
				gProtectStructs[gBankAttacker].KingsShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_SPIKYSHIELD:
				gProtectStructs[gBankAttacker].SpikyShield = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_BANEFULBUNKER:
				gProtectStructs[gBankAttacker].BanefulBunker = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				break;

			case MOVE_CRAFTYSHIELD:
				gSideAffecting[atkSide] |= SIDE_STATUS_CRAFTY_SHIELD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				BattleStringLoader = CraftyShieldSetString;
				break;

			case MOVE_MATBLOCK:
				if (!gDisableStructs[gBankAttacker].isFirstTurn)
					goto PROTECT_FAILED;

				gSideAffecting[atkSide] |= SIDE_STATUS_MAT_BLOCK;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				BattleStringLoader = MatBlockSetString;
				break;

			case MOVE_QUICKGUARD:
				gSideAffecting[atkSide] |= SIDE_STATUS_QUICK_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				BattleStringLoader = QuickGuardSetString;
				break;

			case MOVE_WIDEGUARD:
				gSideAffecting[atkSide] |= SIDE_STATUS_WIDE_GUARD;
				gBattleCommunication[MULTISTRING_CHOOSER] = 3;
				BattleStringLoader = WideGuardSetString;
				break;

			case MOVE_ENDURE:
				gProtectStructs[gBankAttacker].endured = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				break;

			default:
				gProtectStructs[gBankAttacker].protected = 1;
				gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		}

        ++gDisableStructs[gBankAttacker].protectUses;
    }

    else {
	PROTECT_FAILED:
        gDisableStructs[gBankAttacker].protectUses = 0;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gMoveResultFlags |= MOVE_RESULT_MISSED;
    }

    gBattlescriptCurrInstr++;
}

//The Damp check has been moved to attackcanceler
void atk78_faintifabilitynotdamp(void)
{
    if (gBattleExecBuffer) return;

    gActiveBattler = gBankAttacker;
    gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
    EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
    MarkBufferBankForExecution(gActiveBattler);
    gBattlescriptCurrInstr++;
}

void atk7A_jumpifnexttargetvalid(void) {
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE) {

		if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
		&& gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)) {
			gBattlescriptCurrInstr += 5;
			return;
		}

        for (gBankTarget++; gBankTarget < gBattlersCount; gBankTarget++) {
            if (gBankTarget == gBankAttacker && !(gBattleMoves[gCurrentMove].target & MOVE_TARGET_USER))
                continue;
            if (!(gAbsentBattlerFlags & gBitTable[gBankTarget]))
                break;
        }

        if (gBankTarget >= gBattlersCount)
			if (gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
			&&  !(gAbsentBattlerFlags & gBitTable[GetBattlerAtPosition(B_POSITION_PLAYER_LEFT)]))
			{
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
				gBattlescriptCurrInstr = jump_loc;
			}
			else
				gBattlescriptCurrInstr += 5;
        else
			gBattlescriptCurrInstr = jump_loc;
    }
    else
        gBattlescriptCurrInstr += 5;
}

void atk7C_trymirrormove(void)
{
    s32 validMovesCount;
    s32 i;
    u16 move;
    u16 movesArray[4] = {0};

    for (validMovesCount = 0, i = 0; i < gBattlersCount; i++)
    {
        if (i != gBankAttacker)
        {
            move = gBattleStruct->lastTakenMoveFrom[gBankAttacker][i];

            if (move != 0 && move != 0xFFFF)
                movesArray[validMovesCount++] = move;
        }
    }

    move = gBattleStruct->lastTakenMove[gBankAttacker];

    if (move != 0 && move != 0xFFFF)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gCurrentMove = move;
        gBankTarget = GetMoveTarget(gCurrentMove, 0);
		gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else if (validMovesCount)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        i = umodsi(Random(), validMovesCount);
        gCurrentMove = movesArray[i];
        gBankTarget = GetMoveTarget(gCurrentMove, 0);
		gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
    }
    else
    {
        gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr++;
    }
}

void atk7D_setrain(void) {
    if (gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }
    else {
        gBattleWeather = WEATHER_RAIN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_DAMP_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atk7E_setreflect(void) {
    if (gSideAffecting[SIDE(gBankAttacker)] & SIDE_STATUS_REFLECT) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else {
        gSideAffecting[SIDE(gBankAttacker)] |= SIDE_STATUS_REFLECT;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].reflectTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_DEF);
		BattleStringLoader = gText_ScreenRaisedStat;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    gBattlescriptCurrInstr++;
}

const u16 gReflectLightScreenSafeguardStringIds[] =
{
    STRINGID_BUTITFAILED, 0x184
};

void atk7F_setseeded(void) {
    if ((gMoveResultFlags & MOVE_RESULT_NO_EFFECT) || (gStatuses3[gBankTarget] & STATUS3_LEECHSEED)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }

    else if (IsOfType(gBankTarget, TYPE_GRASS)) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }

    else {
        gStatuses3[gBankTarget] |= gBankAttacker;
        gStatuses3[gBankTarget] |= STATUS3_LEECHSEED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }

    gBattlescriptCurrInstr++;
}

void atk81_trysetrest(void)
{
	bool8 fail = FALSE;
    gActiveBattler = gBankTarget = gBankAttacker;
    gBattleMoveDamage = gBattleMons[gBankTarget].maxHP * (-1);

	if (CheckGrounding(gActiveBattler) && (TerrainType == MISTY_TERRAIN || TerrainType == ELECTRIC_TERRAIN))
	{
		gBattlescriptCurrInstr = BattleScript_ButItFailed;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && ABILITY(gActiveBattler) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting->bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (IsOfType(gActiveBattler, TYPE_GRASS) && gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL)
	{
		gBattleScripting->bank = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedByFlowerVeil;
		fail = TRUE;
	}
	else if (ABILITY(gActiveBattler) == ABILITY_SWEETVEIL)
	{
		gBattleScripting->bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE && ABILITY(PARTNER(gActiveBattler)) == ABILITY_SWEETVEIL)
	{
		gBattleScripting->bank = PARTNER(gActiveBattler);
		gBattlescriptCurrInstr = BattleScript_TeamProtectedBySweetVeil;
		fail = TRUE;
	}
	else if (IsUproarBeingMade())
	{
		gBattleScripting->bank = gActiveBattler;
		gBattleCommunication[MULTISTRING_CHOOSER] = 0;
		gBattlescriptCurrInstr = BattleScript_RestCantSleep;
		fail = TRUE;
	}
	else if (BATTLER_MAX_HP(gActiveBattler))
	{
		gBattleScripting->bank = gActiveBattler;
		gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
		fail = TRUE;
	}

	if (!fail)
	{
		switch (ABILITY(gActiveBattler)) {
			case ABILITY_INSOMNIA:
			case ABILITY_VITALSPIRIT:
				gBattlescriptCurrInstr = BattleScript_TargetStayedAwakeUsingAbility;
				fail = TRUE;
				break;
			case ABILITY_LEAFGUARD:
				if (WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
				{
					gBattlescriptCurrInstr = BattleScript_ProtectedByAbility;
					fail = TRUE;
				}
				break;
			case ABILITY_COMATOSE:
				gBattlescriptCurrInstr = BattleScript_ButItFailed;
				fail = TRUE;
				break;
			case ABILITY_SHIELDSDOWN:
				if (SPECIES(gBankAttacker) == SPECIES_MINIOR_SHIELD)
				{
					gBattlescriptCurrInstr = BattleScript_ButItFailed;
					fail = TRUE;
				}
		}
	}

	if (!fail)
	{
        if (gBattleMons[gActiveBattler].status1 & ((u8)(~STATUS1_SLEEP)))
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;

        gBattleMons[gBankTarget].status1 = 3;
        EmitSetMonData(0, REQUEST_STATUS_BATTLE, 0, 4, &gBattleMons[gActiveBattler].status1);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

void atk82_jumpifnotfirstturn(void)
{
    u8* failJump = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBankAttacker].isFirstTurn
	&& !gNewBS->InstructInProgress)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = failJump;
}

void atk84_jumpifcantmakeasleep(void) {
	u8 bankDef = gBankTarget;
	u8 defPartner = PARTNER(bankDef);
	u8 defAbility = ABILITY(bankDef);
	u8 defPartnerAbility = ABILITY(defPartner);

    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (UproarWakeUpCheck(bankDef) || gBattleMons[bankDef].species == SPECIES_MINIOR_SHIELD)
        gBattlescriptCurrInstr = jump_loc;

    else if (defAbility == ABILITY_INSOMNIA || defAbility == ABILITY_VITALSPIRIT || defAbility == ABILITY_COMATOSE || defAbility == ABILITY_SWEETVEIL
	|| (defAbility == ABILITY_LEAFGUARD && WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
	|| (defAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
        gLastUsedAbility = defAbility;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        gBattlescriptCurrInstr = jump_loc;
        RecordAbilityBattle(bankDef, gLastUsedAbility);
    }

	else if (defPartnerAbility == ABILITY_SWEETVEIL
	|| (defPartnerAbility == ABILITY_FLOWERVEIL && IsOfType(bankDef, TYPE_GRASS) && gCurrentMove != MOVE_REST)) {
		gLastUsedAbility = defPartnerAbility;
		gBattlescriptCurrInstr = jump_loc;
		RecordAbilityBattle(defPartner, gLastUsedAbility);
	}

	else if (CheckGrounding(bankDef) && (TerrainType == ELECTRIC_TERRAIN || TerrainType == MISTY_TERRAIN))
		gBattlescriptCurrInstr = jump_loc;

    else
        gBattlescriptCurrInstr += 5;
}

//This function used to calculate damage, but now all it does is check if Spit Up can be used
void atk86_stockpiletobasedamage(void) {
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBankAttacker].stockpileCounter == 0)
        gBattlescriptCurrInstr = jump_loc;
    else
	{
		gBattleScripting->animTurn = gDisableStructs[gBankAttacker].stockpileCounter;
        gBattlescriptCurrInstr += 5;
	}
}

void atk87_stockpiletohpheal(void)
{
    u8* jumpPtr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    if (gDisableStructs[gBankAttacker].stockpileCounter == 0)
    {
        gBattlescriptCurrInstr = jumpPtr;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }
    else if (gBattleMons[gBankAttacker].maxHP == gBattleMons[gBankAttacker].hp)
    {
        gDisableStructs[gBankAttacker].stockpileCounter = 0;
        gBattlescriptCurrInstr = jumpPtr;
        gBankTarget = gBankAttacker;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
        gBattleMoveDamage = MathMax(1, udivsi(gBattleMons[gBankAttacker].maxHP, (1 << (3 - gDisableStructs[gBankAttacker].stockpileCounter))));
        gBattleMoveDamage *= -1;

        gBattleScripting->animTurn = gDisableStructs[gBankAttacker].stockpileCounter;
        gBattlescriptCurrInstr += 5;
        gBankTarget = gBankAttacker;
    }
}

void atk88_negativedamage(void) {
	if (gCurrentMove == MOVE_OBLIVIONWING)
		gBattleMoveDamage = udivsi(75 * gHpDealt, 100);
	else
		gBattleMoveDamage = (gHpDealt / 2);

	if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_BIG_ROOT)
		gBattleMoveDamage = udivsi(130 * gHpDealt, 100);

	gBattleMoveDamage *= -1;

    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = -1;

    gBattlescriptCurrInstr++;
}

void atk8A_normalisebuffs(void) //Haze
{
    int i, j;

    for (i = 0; i < gBattlersCount; ++i)
    {
        for (j = 0; j < BATTLE_STATS_NO-1; ++j)
            gBattleMons[i].statStages[j] = 6;
    }

    ++gBattlescriptCurrInstr;
}

void atk8D_setmultihitcounter(void) {
    if (gBattlescriptCurrInstr[1])
        gMultiHitCounter = gBattlescriptCurrInstr[1];

	if (ABILITY(gBankAttacker) == ABILITY_SKILLLINK)
		gMultiHitCounter = 5;

	else if (ABILITY(gBankAttacker) == ABILITY_BATTLEBOND
	&& gCurrentMove == MOVE_WATERSHURIKEN
	&& gBattleMons[gBankAttacker].species == SPECIES_ASHGRENINJA)
	{
		gMultiHitCounter = 3;
	}
	else {
        gMultiHitCounter = Random() & 3;
        if (gMultiHitCounter > 1)
            gMultiHitCounter = (Random() & 3) + 2;
        else
            gMultiHitCounter += 2;
    }
    gBattlescriptCurrInstr += 2;
}

void atk90_tryconversiontypechange(void) {
	u8* fail_script = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	if (gCurrentMove != MOVE_REFLECTTYPE) { //Conversion
		u8 moveType = gBattleMoves[gBattleMons[gBankAttacker].moves[0]].type;

		if (IS_BLANK_TYPE(moveType))
			gBattlescriptCurrInstr = fail_script;

		else if (
			moveType == gBattleMons[gBankAttacker].type1
		||  moveType == gBattleMons[gBankAttacker].type2
		||  moveType == gBattleMons[gBankAttacker].type3)
			gBattlescriptCurrInstr = fail_script;

		else {
			gBattleMons[gBankAttacker].type1 = moveType;
			gBattleMons[gBankAttacker].type2 = moveType;
			gBattleMons[gBankAttacker].type3 = TYPE_BLANK;

			PREPARE_TYPE_BUFFER(gBattleTextBuff1, moveType);

			gBattlescriptCurrInstr += 5;
		}
	}

	else { //Reflect Type
		u8 defType1 = gBattleMons[gBankTarget].type1;
		u8 defType2 = gBattleMons[gBankTarget].type2;
		u8 defType3 = gBattleMons[gBankTarget].type3;

		//If target has no type
		if((IS_BLANK_TYPE(defType1))
		&& (IS_BLANK_TYPE(defType2))
		&& (IS_BLANK_TYPE(defType3)))
			gBattlescriptCurrInstr = fail_script;

		else {
			//If target has no main types, but has a third type
			if ((IS_BLANK_TYPE(defType1))
			&&  (IS_BLANK_TYPE(defType2))
			&& !(IS_BLANK_TYPE(defType3))) {
				defType1 = TYPE_NORMAL;
				defType2 = TYPE_NORMAL;
			}

			else { //Target Has Main Type
				if (IS_BLANK_TYPE(defType1))
					defType1 = defType2;
				else if (IS_BLANK_TYPE(defType2))
					defType2 = defType1;

				if (IS_BLANK_TYPE(defType3)) //Just in case it has a burned out Fire type
					defType3 = TYPE_BLANK;
			}

			gBattleMons[gBankAttacker].type1 = defType1;
			gBattleMons[gBankAttacker].type2 = defType2;
			gBattleMons[gBankAttacker].type3 = defType3;

			gBattlescriptCurrInstr += 5;
		}
	}
}

void atk91_givepaydaymoney(void)
{
	int i;
	u32 money = 0;

    if (!(gBattleTypeFlags & (BATTLE_TYPE_LINK | BATTLE_TYPE_FRONTIER | BATTLE_TYPE_TRAINER_TOWER)))
	{
		for (i = 0; i < 6; ++i)
			money += (gPlayerParty[i].level * 5) * gNewBS->PayDayByPartyIndices[i];

		money *= gBattleStruct->moneyMultiplier;
		money = MathMin(money, 0xFFFF);

		if (money)
		{
			AddMoney(&gSaveBlock1->money, money);
			PREPARE_HWORD_NUMBER_BUFFER(gBattleTextBuff1, 5, money);
			BattleScriptPush(gBattlescriptCurrInstr + 1);
			gBattlescriptCurrInstr = BattleScript_PrintPayDayMoneyString;
			return;
		}
    }
	gBattlescriptCurrInstr++;
}

void atk92_setlightscreen(void) {
    if (gSideAffecting[SIDE(gBankAttacker)] & SIDE_STATUS_LIGHTSCREEN) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 0;
    }

    else {
        gSideAffecting[SIDE(gBankAttacker)] |= SIDE_STATUS_LIGHTSCREEN;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_LIGHT_CLAY)
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 8;
		else
			gSideTimers[SIDE(gBankAttacker)].lightscreenTimer = 5;

		PREPARE_STAT_BUFFER(gBattleTextBuff1, STAT_SPDEF);
        BattleStringLoader = gText_ScreenRaisedStat;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    gBattlescriptCurrInstr++;
}

void atk93_tryKO(void) {
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;

	u8 atkAbility = ABILITY(bankAtk);
	u8 defAbility = ABILITY(bankDef);
	u8 defEffect = ITEM_EFFECT(bankDef);
    u8 defQuality = ITEM_QUALITY(bankDef);
	u8 mystery = ItemId_GetMystery2(ITEM(bankDef));

    gStringBank = bankDef;

    if (atkAbility == ABILITY_STURDY) {
        gMoveResultFlags |= MOVE_RESULT_MISSED;
        gLastUsedAbility = ABILITY_STURDY;
        gBattlescriptCurrInstr = BattleScript_SturdyPreventsOHKO;
        RecordAbilityBattle(bankDef, ABILITY_STURDY);
    }

    else {
		u16 chance = FALSE;
		u8 baseAcc;
		#ifdef NO_SHEER_COLD_NERF
			baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#else
			if (gCurrentMove == MOVE_SHEERCOLD && !IsOfType(bankAtk, TYPE_ICE))
				baseAcc = 20;
			else
				baseAcc = gBattleMoves[gCurrentMove].accuracy;
		#endif

		if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level) {
			if (((gStatuses3[bankDef] & STATUS3_ALWAYS_HITS && gDisableStructs[bankDef].bankWithSureHit == bankAtk)
			|| atkAbility == ABILITY_NOGUARD || defAbility == ABILITY_NOGUARD))
				chance = TRUE;

			#ifndef NO_SHEER_COLD_NERF
			else if (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE))
				chance = FALSE;
			#endif

			else {
				chance = baseAcc + (gBattleMons[bankAtk].level - gBattleMons[bankDef].level);
				if (umodsi(Random(), 100) + 1 < chance)
					chance = TRUE;
				else
					chance = FALSE;
			}
		}
		else
			chance = FALSE;

        if (chance) {
		    if (defEffect == ITEM_EFFECT_FOCUS_BAND && !mystery && umodsi(Random(), 100) < defQuality) {
				RecordItemBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
			}
			//Focus Sash
			else if (defEffect == ITEM_EFFECT_FOCUS_BAND && mystery && (gBattleMons[bankDef].hp == gBattleMons[bankDef].maxHP)) {
				RecordItemBattle(bankDef, defEffect);
				gSpecialStatuses[bankDef].focusBanded = 1;
				gNewBS->EnduranceHelper = ENDURE_FOCUS_SASH;
			}

            if (gProtectStructs[bankDef].endured) {
                gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_ENDURED;
            }
            else if (gSpecialStatuses[bankDef].focusBanded) {
                gBattleMoveDamage = gBattleMons[bankDef].hp - 1;
                gMoveResultFlags |= MOVE_RESULT_FOE_HUNG_ON;
                gLastUsedItem = gBattleMons[bankDef].item;
            }
            else {
                gBattleMoveDamage = gBattleMons[bankDef].hp;
                gMoveResultFlags |= MOVE_RESULT_ONE_HIT_KO;
            }
            gBattlescriptCurrInstr += 5;
        }

        else {
            gMoveResultFlags |= MOVE_RESULT_MISSED;
			#ifdef NO_SHEER_COLD_NERF
				if (gBattleMons[bankAtk].level >= gBattleMons[bankDef].level)
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
			#else
				if (gBattleMons[bankAtk].level < gBattleMons[bankDef].level
				|| (gCurrentMove == MOVE_SHEERCOLD && IsOfType(bankDef, TYPE_ICE)))
					gBattleCommunication[MULTISTRING_CHOOSER] = 1;
				else
					gBattleCommunication[MULTISTRING_CHOOSER] = 0;
			#endif

            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
    }
}

void atk94_damagetohalftargethp(void) { //Super Fang
	switch (gCurrentMove) {
		case MOVE_GUARDIAN_OF_ALOLA:
			gBattleMoveDamage = udivsi(gBattleMons[gBankTarget].hp * 75, 100);
			if (ProtectsAgainstZMoves(gCurrentMove, gBankAttacker, gBankTarget))
				gBattleMoveDamage = udivsi(gBattleMoveDamage * 25, 100);
			break;

		default:
			gBattleMoveDamage = gBattleMons[gBankTarget].hp / 2;
	}

    if (gBattleMoveDamage == 0)
        gBattleMoveDamage = 1;
    gBattlescriptCurrInstr++;
}

void atk95_setsandstorm(void) {
    if (gBattleWeather & (WEATHER_SANDSTORM_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }

    else {
        gBattleWeather = WEATHER_SANDSTORM_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 3;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_SMOOTH_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atk96_weatherdamage(void) {
	u8 bank = gBankAttacker;
	u8 ability = ABILITY(bank);
	u8 effect = ITEM_EFFECT(bank);

	gBattleMoveDamage = 0;

	if (gAbsentBattlerFlags & gBitTable[bank])
        gBattleMoveDamage = 0;

	#ifndef NO_GHOST_BATTLES //Ghosts can't take damage from Sand Stream or Snow Warning
	else if ((gBattleTypeFlags & (BATTLE_TYPE_SCRIPTED_WILD_1 | BATTLE_TYPE_GHOST)) == BATTLE_TYPE_GHOST
	&&  SIDE(bank) == B_SIDE_OPPONENT)
		gBattleMoveDamage = 0;
	#endif

	else if (WEATHER_HAS_EFFECT
		&& 	ability != ABILITY_MAGICGUARD
		&&	ability != ABILITY_OVERCOAT
		&&	effect  != ITEM_EFFECT_SAFETY_GOGGLES
		&& !(gStatuses3[bank] & (STATUS3_UNDERGROUND | STATUS3_UNDERWATER | STATUS3_DISAPPEARED)))
	{

        if (gBattleWeather & WEATHER_SANDSTORM_ANY)
		{
            if (!IsOfType(bank, TYPE_ROCK) && !IsOfType(bank, TYPE_GROUND) && !IsOfType(bank, TYPE_STEEL)
            && ability != ABILITY_SANDVEIL && ability != ABILITY_SANDRUSH && ability != ABILITY_SANDFORCE)
                gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
            else
			{
                gBattleMoveDamage = 0;
			}
        }
        else if (gBattleWeather & WEATHER_HAIL_ANY)
		{
            if (!IsOfType(bank, TYPE_ICE) && ability != ABILITY_ICEBODY && ability != ABILITY_SNOWCLOAK && ability != ABILITY_SLUSHRUSH)
				gBattleMoveDamage = MathMax(1, gBattleMons[bank].maxHP / 16);
            else
                gBattleMoveDamage = 0;
        }
    }

    else
        gBattleMoveDamage = 0;

    gBattlescriptCurrInstr++;
}

void atk97_tryinfatuating(void)
{
	u8 bankAtk, bankDef;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
	{
		bankAtk = gBankAttacker;
		bankDef = gBankTarget;
	}
	else
	{
		bankAtk = gBankTarget;
		bankDef = gBankAttacker;
	}

    struct Pokemon *monAttacker, *monTarget;
    u16 speciesAttacker, speciesTarget;
    u32 personalityAttacker, personalityTarget;

	monAttacker = GetBankPartyData(bankAtk);
	monTarget = GetBankPartyData(bankDef);

    speciesAttacker = monAttacker->species;
    personalityAttacker = monAttacker->personality;

    speciesTarget = monTarget->species;
    personalityTarget = monTarget->personality;

    if (ABILITY(bankDef) == ABILITY_OBLIVIOUS)
    {
        gBattlescriptCurrInstr = BattleScript_ObliviousPrevents;
    }
    else
    {
        if (GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) == GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget)
        || gBattleMons[bankDef].status2 & STATUS2_INFATUATION
        || GetGenderFromSpeciesAndPersonality(speciesAttacker, personalityAttacker) == MON_GENDERLESS
        || GetGenderFromSpeciesAndPersonality(speciesTarget, personalityTarget) == MON_GENDERLESS)
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
        }
        else
        {
            gBattleMons[bankDef].status2 |= STATUS2_INFATUATED_WITH(bankAtk);
            gBattlescriptCurrInstr += 6;
        }
    }
}

void atk9B_transformdataexecution(void)
{
    gChosenMove = 0xFFFF;
    gBattlescriptCurrInstr++;
    if (gBattleMons[gBankTarget].status2 & STATUS2_TRANSFORMED
    || gStatuses3[gBankTarget] & (STATUS3_SEMI_INVULNERABLE | STATUS3_ILLUSION))
    {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 1;
    }
    else
    {
		TransformPokemon(gBankAttacker, gBankTarget);
        gActiveBattler = gBankAttacker;
        EmitResetActionMoveSelection(0, RESET_MOVE_SELECTION);
        MarkBufferBankForExecution(gActiveBattler);
    }
}

void TransformPokemon(u8 bankAtk, u8 bankDef)
{
    u32 i;
    u8 *battleMonAttacker, *battleMonTarget;

    gBattleMons[bankAtk].status2 |= STATUS2_TRANSFORMED;
    gDisableStructs[bankAtk].disabledMove = 0;
    gDisableStructs[bankAtk].disableTimer1 = 0;
    gDisableStructs[bankAtk].transformedMonPersonality = gBattleMons[bankDef].personality;
    gDisableStructs[bankAtk].mimickedMoves = 0;

    PREPARE_SPECIES_BUFFER(gBattleTextBuff1, SPECIES(bankDef))

    battleMonAttacker = (u8*)(&gBattleMons[bankAtk]);
    battleMonTarget = (u8*)(&gBattleMons[bankDef]);

    for (i = 0; i < offsetof(struct BattlePokemon, pp); i++)
	{
        battleMonAttacker[i] = battleMonTarget[i];
	}

	if (IS_BLANK_TYPE(gBattleMons[bankDef].type1)
	&&  IS_BLANK_TYPE(gBattleMons[bankDef].type2))
	{
		gBattleMons[bankAtk].type1 = TYPE_NORMAL;
		gBattleMons[bankAtk].type2 = TYPE_NORMAL;
	}

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMoves[gBattleMons[bankAtk].moves[i]].pp < 5)
            gBattleMons[bankAtk].pp[i] = gBattleMoves[gBattleMons[bankAtk].moves[i]].pp;
        else
            gBattleMons[bankAtk].pp[i] = 5;
    }
	
    gBattleCommunication[MULTISTRING_CHOOSER] = 0;
	gBattleSpritesDataPtr->bankData[bankAtk].transformSpecies = SPECIES(bankDef);
}

void atk9D_mimicattackcopy(void)
{
    gChosenMove = 0xFFFF;

    if (gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED
    || gLastUsedMoves[gBankTarget] == 0
    || gLastUsedMoves[gBankTarget] == 0xFFFF
	|| CheckTableForMove(gLastUsedMoves[gBankTarget], MimicBanTable)
	|| IsZMove(gLastUsedMoves[gBankTarget]))
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        int i;

        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (gBattleMons[gBankAttacker].moves[i] == gLastUsedMoves[gBankTarget])
                break;
        }

        if (i == MAX_MON_MOVES)
        {
            gBattleMons[gBankAttacker].moves[gCurrMovePos] = gLastUsedMoves[gBankTarget];
            gBattleMons[gBankAttacker].pp[gCurrMovePos] = gBattleMoves[gLastUsedMoves[gBankTarget]].pp;

            PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastUsedMoves[gBankTarget])

            gDisableStructs[gBankAttacker].mimickedMoves |= gBitTable[gCurrMovePos];
            gBattlescriptCurrInstr += 5;
        }
        else
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
    }
}

void atk9E_metronome(void)
{
    while (TRUE)
    {
        gCurrentMove = umodsi(Random(), LAST_MOVE_INDEX) + 1;
        if (IsZMove(gCurrentMove))
            continue;

        if (CheckTableForMove(gCurrentMove, MetronomeBanTable))
			continue;

        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
		gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
        gBattlescriptCurrInstr = gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect];
        gBankTarget = GetMoveTarget(gCurrentMove, 0);
        return;
    }
}

void atkA0_psywavedamageeffect(void)
{
    gBattleMoveDamage = GetPsywaveDamage(Random() % 101);
    ++gBattlescriptCurrInstr;
}

s32 GetPsywaveDamage(u8 randDamage)
{
	return MathMax(1, (gBattleMons[gBankAttacker].level * (randDamage + 50)) / 100);
}

void atkA1_counterdamagecalculator(void) {
    u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].physicalBank);

    if (gProtectStructs[gBankAttacker].physicalDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].physicalBank].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBankAttacker].physicalDmg * 2;

        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].physicalBank;
        gBattlescriptCurrInstr += 5;
    }
    else 
	{
        gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA2_mirrorcoatdamagecalculator(void) {
    u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = SIDE(gProtectStructs[gBankAttacker].specialBank);

    if (gProtectStructs[gBankAttacker].specialDmg && atkSide != defSide && gBattleMons[gProtectStructs[gBankAttacker].specialBank].hp)
	{
		gBattleMoveDamage = gProtectStructs[gBankAttacker].specialDmg * 2;

        if (gSideTimers[defSide].followmeTimer && gBattleMons[gSideTimers[defSide].followmeTarget].hp)
            gBankTarget = gSideTimers[defSide].followmeTarget;
        else
            gBankTarget = gProtectStructs[gBankAttacker].specialBank;
        gBattlescriptCurrInstr += 5;
    }
    else
	{
        gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA3_disablelastusedattack(void)
{
    int i;
	u8 bankDef;

	if (gBattlescriptCurrInstr[1] == BS_GET_TARGET)
		bankDef = gBankTarget;
	else
		bankDef = gBankAttacker; //For Cursed Body

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMons[bankDef].moves[i] == gLastUsedMoves[bankDef])
            break;
    }

    if (gDisableStructs[bankDef].disabledMove == 0
    && i != MAX_MON_MOVES
	&& gBattleMons[bankDef].pp[i] != 0
	&& !AbilityBattleEffects(ABILITYEFFECT_CHECK_BANK_SIDE, bankDef, ABILITY_AROMAVEIL, 0, 0))
    {
        PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleMons[bankDef].moves[i])

        gDisableStructs[bankDef].disabledMove = gBattleMons[bankDef].moves[i];
        gDisableStructs[bankDef].disableTimer1 = 4;
        gDisableStructs[bankDef].disableTimer2 = gDisableStructs[bankDef].disableTimer1; // used to save the random amount of turns?
        gBattlescriptCurrInstr += 6;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    }
}

void atkA4_trysetencore(void)
{
    int i;

    for (i = 0; i < MAX_MON_MOVES; i++)
    {
        if (gBattleMons[gBankTarget].moves[i] == gLastUsedMoves[gBankTarget])
            break;
    }

    if (gLastUsedMoves[gBankTarget] == MOVE_STRUGGLE
    ||  gLastUsedMoves[gBankTarget] == MOVE_ENCORE
	||  gNewBS->playedShellTrapMessage & gBitTable[gBankTarget]
    ||  CheckTableForMove(gLastUsedMoves[gBankTarget], MovesThatCallOtherMovesTable)
	||  IS_Z_MOVE(gLastUsedMoves[gBankTarget]))
    {
        i = 4;
    }

    if (gDisableStructs[gBankTarget].encoredMove == 0
    && i != 4
	&& gBattleMons[gBankTarget].pp[i] != 0)
    {
        gDisableStructs[gBankTarget].encoredMove = gBattleMons[gBankTarget].moves[i];
        gDisableStructs[gBankTarget].encoredMovePos = i;
        gDisableStructs[gBankTarget].encoreTimer = 3;
        gDisableStructs[gBankTarget].encoreTimerStartValue = gDisableStructs[gBankTarget].encoreTimer;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA6_settypetorandomresistance(void) { //Conversion 2
	u8 bankAtk = gBankAttacker;
	u8 bankDef = gBankTarget;
	u32 i, j, rands;

    if (gLastUsedMoves[bankDef] == 0 || gLastUsedMoves[bankDef] == 0xFFFF) {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		return;
	}

    for (rands = 0; rands < 1000; ++rands)
	{
        while (((i = (Random() & 0x7F)) > udivsi(sizeof(gTypeEffectiveness), 3)));

        i *= 3;

		#ifdef INVERSE_BATTLES
			if (FlagGet(INVERSE_FLAG))
			{
				if (TYPE_EFFECT_ATK_TYPE(i) == gNewBS->LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(i) >= TYPE_MUL_SUPER_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(i));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(i));

					gBattlescriptCurrInstr += 5;
					return;
				}
			}

			else
		#endif
			{
				if (TYPE_EFFECT_ATK_TYPE(i) == gNewBS->LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(i) <= TYPE_MUL_NOT_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(i));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(i));

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
    }

    for (j = 0, rands = 0; rands < sizeof(gTypeEffectiveness); j += 3, rands += 3) {
        switch (TYPE_EFFECT_ATK_TYPE(j)) {
			case TYPE_ENDTABLE:
			case TYPE_FORESIGHT:
				break;
			default:
		#ifdef INVERSE_BATTLES
			if (FlagGet(INVERSE_FLAG)) {
				if (TYPE_EFFECT_ATK_TYPE(j) == gNewBS->LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(j) >= TYPE_MUL_SUPER_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(rands));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(rands))

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
			else
		#endif
			{
				if (TYPE_EFFECT_ATK_TYPE(j) == gNewBS->LastUsedTypes[bankDef]
				&& TYPE_EFFECT_MULTIPLIER(j) <= TYPE_MUL_NOT_EFFECTIVE
				&& !IsOfType(bankAtk, TYPE_EFFECT_DEF_TYPE(i)))
				{
					SET_BATTLER_TYPE(bankAtk, TYPE_EFFECT_DEF_TYPE(rands));
					PREPARE_TYPE_BUFFER(gBattleTextBuff1, TYPE_EFFECT_DEF_TYPE(rands))

					gBattlescriptCurrInstr += 5;
					return;
				}
			}
        }
    }

    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkA8_copymovepermanently(void) // sketch
{
    gChosenMove = 0xFFFF;

    if (!(gBattleMons[gBankAttacker].status2 & STATUS2_TRANSFORMED)
	&& !IsZMove(gLastPrintedMoves[gBankTarget])
    && gLastPrintedMoves[gBankTarget] != MOVE_STRUGGLE
	&& gLastPrintedMoves[gBankTarget] != MOVE_CHATTER
	&& gLastPrintedMoves[gBankTarget] != MOVE_SKETCH
    && gLastPrintedMoves[gBankTarget] != 0
    && gLastPrintedMoves[gBankTarget] != 0xFFFF)
    {
        int i;

        for (i = 0; i < MAX_MON_MOVES; i++)
        {
            if (gBattleMons[gBankAttacker].moves[i] == MOVE_SKETCH)
                continue;
            if (gBattleMons[gBankAttacker].moves[i] == gLastPrintedMoves[gBankTarget])
                break;
        }

        if (i != MAX_MON_MOVES)
        {
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
        }
        else // sketch worked
        {
            struct MovePpInfo movePpData;

            gBattleMons[gBankAttacker].moves[gCurrMovePos] = gLastPrintedMoves[gBankTarget];
            gBattleMons[gBankAttacker].pp[gCurrMovePos] = gBattleMoves[gLastPrintedMoves[gBankTarget]].pp;
            gActiveBattler = gBankAttacker;

            for (i = 0; i < MAX_MON_MOVES; i++)
            {
                movePpData.moves[i] = gBattleMons[gBankAttacker].moves[i];
                movePpData.pp[i] = gBattleMons[gBankAttacker].pp[i];
            }
            movePpData.ppBonuses = gBattleMons[gBankAttacker].ppBonuses;

            EmitSetMonData(0, REQUEST_MOVES_PP_BATTLE, 0, sizeof(struct MovePpInfo), &movePpData);
            MarkBufferBankForExecution(gActiveBattler);

            PREPARE_MOVE_BUFFER(gBattleTextBuff1, gLastPrintedMoves[gBankTarget])

            gBattlescriptCurrInstr += 5;
        }
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkA9_trychoosesleeptalkmove(void) {
    u8 unusable_moves = 0;
    int i;

    for (i = 0; i < 4; ++i) {
        if (CheckTableForMove(gBattleMons[gBankAttacker].moves[i], SleepTalkBanTable))
            unusable_moves |= gBitTable[i];
    }

    unusable_moves = CheckMoveLimitations(gBankAttacker, unusable_moves, 0xFF ^ MOVE_LIMITATION_PP ^ MOVE_LIMITATION_DISABLED ^ MOVE_LIMITATION_CHOICE);
    if (unusable_moves == 0xF) //all 4 moves cannot be chosen
        gBattlescriptCurrInstr += 5;

    else { //at least one move can be chosen
        u32 random_pos;
        do {
            random_pos = Random() & 3;
        } while ((gBitTable[random_pos] & unusable_moves));

        gRandomMove = gBattleMons[gBankAttacker].moves[random_pos];
        gCurrMovePos = random_pos;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gBankTarget = GetMoveTarget(gRandomMove, 0);
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
		gHitMarker |= HITMARKER_NO_PPDEDUCT;
    }
}

u8 CheckMoveLimitations(u8 bank, u8 unusableMoves, u8 check) {
    u8 holdEffect = ITEM_EFFECT(bank);
    u16 choicedMove = CHOICED_MOVE(bank);
    int i;

    gStringBank = bank;
    for (i = 0; i < 4; i++) {
		u16 move = gBattleMons[bank].moves[i];
        if (move == 0 && check & MOVE_LIMITATION_ZEROMOVE)
            unusableMoves |= gBitTable[i];
        else if (gBattleMons[bank].pp[i] == 0 && check & MOVE_LIMITATION_PP)
            unusableMoves |= gBitTable[i];
        else if (move == gDisableStructs[bank].disabledMove && check & MOVE_LIMITATION_DISABLED)
            unusableMoves |= gBitTable[i];
        else if (move == gLastUsedMoves[bank] && check & MOVE_LIMITATION_TORMENTED && gBattleMons[bank].status2 & STATUS2_TORMENT)
            unusableMoves |= gBitTable[i];
        else if (gDisableStructs[bank].tauntTimer && check & MOVE_LIMITATION_TAUNT && SPLIT(move) == SPLIT_STATUS)
            unusableMoves |= gBitTable[i];
        else if (IsImprisoned(bank, move) && check & MOVE_LIMITATION_IMPRISION)
            unusableMoves |= gBitTable[i];
        else if (gDisableStructs[bank].encoreTimer && gDisableStructs[bank].encoredMove != move)
            unusableMoves |= gBitTable[i];
        else if (holdEffect == ITEM_EFFECT_CHOICE_BAND && choicedMove != 0 && choicedMove != 0xFFFF && choicedMove != move && check & MOVE_LIMITATION_CHOICE)
            unusableMoves |= gBitTable[i];

		else if (holdEffect == ITEM_EFFECT_ASSAULT_VEST && SPLIT(move) == SPLIT_STATUS)
			unusableMoves |= gBitTable[i];
		else if (FlagGet(SKY_BATTLE_FLAG) && CheckTableForMove(move, SkyBattleBanTable))
			unusableMoves |= gBitTable[i];
		else if (gNewBS->GravityTimer && CheckTableForMove(move, GravityBanTable))
			unusableMoves |= gBitTable[i];
		else if (gNewBS->ThroatChopTimers[bank] && CheckSoundMove(move))
			unusableMoves |= gBitTable[i];
		else if (gNewBS->HealBlockTimers[bank] && CheckHealingMove(move))
			unusableMoves |= gBitTable[i];
    }
    return unusableMoves;
}

void atkAA_setdestinybond(void) {
	if (gNewBS->DestinyBondCounters[gBankAttacker])
		gBattlescriptCurrInstr = BattleScript_ButItFailed + 2;

	else {
		gNewBS->DestinyBondCounters[gBankAttacker] = 2;
		gBattleMons[gBankAttacker].status2 |= STATUS2_DESTINY_BOND;
		gBattlescriptCurrInstr++;
	}
}

void atkAD_tryspiteppreduce(void) {
    if (gLastUsedMoves[gBankTarget] != 0
	&&  gLastUsedMoves[gBankTarget] != 0xFFFF
	&& (!(gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE) || ABILITY(gBankAttacker) == ABILITY_NOGUARD || ABILITY(gBankTarget) == ABILITY_NOGUARD)) {
        int i;
        for (i = 0; i < 4; i++)
        {
            if (gLastUsedMoves[gBankTarget] == gBattleMons[gBankTarget].moves[i])
                break;
        }

        if (i != 4 && gBattleMons[gBankTarget].pp[i] > 0) {
            s32 lost_pp = MathMin(gBattleMons[gBankTarget].pp[i], 4);

            gBattleTextBuff1[0] = 0xFD;
            gBattleTextBuff1[1] = 2;
            gBattleTextBuff1[2] = gLastUsedMoves[gBankTarget];
            gBattleTextBuff1[3] = gLastUsedMoves[gBankTarget] >> 8;
            gBattleTextBuff1[4] = 0xFF;
            ConvertIntToDecimalStringN(gBattleTextBuff2, lost_pp, 0, 1);
            gBattleTextBuff2[0] = 0xFD;
            gBattleTextBuff2[1] = 1;
            gBattleTextBuff2[2] = 1;
            gBattleTextBuff2[3] = 1;
            gBattleTextBuff2[4] = lost_pp;
            gBattleTextBuff2[5] = 0xFF;

            gBattleMons[gBankTarget].pp[i] -= lost_pp;
            gActiveBattler = gBankTarget;
            if (!(gDisableStructs[gActiveBattler].mimickedMoves & gBitTable[i])
                && !(gBattleMons[gActiveBattler].status2 & STATUS2_TRANSFORMED))
            {
                EmitSetMonData(0, REQUEST_PPMOVE1_BATTLE + i, 0, 1, &gBattleMons[gActiveBattler].pp[i]);
                MarkBufferBankForExecution(gActiveBattler);
            }
            gBattlescriptCurrInstr += 5;
            if (gBattleMons[gBankTarget].pp[i] == 0)
                CancelMultiTurnMoves(gBankTarget);
            return;
        }
    }
    gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkAF_cursetarget(void) {
    if (gBattleMons[gBankTarget].status2 & STATUS2_CURSED
	|| (gStatuses3[gBankTarget] & STATUS3_SEMI_INVULNERABLE && ABILITY(gBankAttacker) != ABILITY_NOGUARD && ABILITY(gBankTarget) != ABILITY_NOGUARD))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else {
        gBattleMons[gBankTarget].status2 |= STATUS2_CURSED;
        gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
        gBattlescriptCurrInstr += 5;
    }
}

void atkB0_trysetspikes(void) {
	u8 atkSide = SIDE(gBankAttacker);
    u8 defSide = atkSide ^ BIT_SIDE;
	u8 stringcase = 0xFF;

	switch (gCurrentMove) {
		case MOVE_STEALTHROCK:
			if (gSideTimers[defSide].srAmount) {
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].srAmount = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 1;
			}
			break;

		case MOVE_TOXICSPIKES:
			if (gSideTimers[defSide].tspikesAmount >= 2) {
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].tspikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 2;
			}
			break;

		case MOVE_STICKYWEB:
			if (gSideTimers[defSide].stickyWeb) {
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].stickyWeb = 1;
				gBattlescriptCurrInstr += 5;
				stringcase = 3;
			}
			break;

		default:
			if (gSideTimers[defSide].spikesAmount >= 3) {
				gSpecialStatuses[gBankAttacker].ppNotAffectedByPressure = 1;
				gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
			}
			else {
				gSideAffecting[defSide] |= SIDE_STATUS_SPIKES;
				gSideTimers[defSide].spikesAmount++;
				gBattlescriptCurrInstr += 5;
				stringcase = 0;
			}
	}

	if (stringcase != 0xFF)
		BattleStringLoader = (u8*) EntryHazardsStrings[stringcase];
}

//Actual calc has been moved to GetBasePower function
void atkB3_rolloutdamagecalculation(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
    {
        CancelMultiTurnMoves(gBankAttacker);
        gBattlescriptCurrInstr = BattleScript_MoveMissedPause;
    }
    else
    {
        if (!(gBattleMons[gBankAttacker].status2 & STATUS2_MULTIPLETURNS)) // first hit
        {
            gDisableStructs[gBankAttacker].rolloutTimer = 5;
            gDisableStructs[gBankAttacker].rolloutTimerStartValue = 5;
            gBattleMons[gBankAttacker].status2 |= STATUS2_MULTIPLETURNS;
            gLockedMoves[gBankAttacker] = gCurrentMove;
        }
        if (--gDisableStructs[gBankAttacker].rolloutTimer == 0) // last hit
        {
            gBattleMons[gBankAttacker].status2 &= ~(STATUS2_MULTIPLETURNS);
        }

        gBattlescriptCurrInstr++;
    }
}

void atkB4_jumpifconfusedandstatmaxed(void)
{
    if (gBattleMons[gBankTarget].status2 & STATUS2_CONFUSION
    && !STAT_CAN_RISE(gBankTarget, gBattlescriptCurrInstr[1]))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    else
        gBattlescriptCurrInstr += 6;
}

void atkB5_furycuttercalc(void)
{
    if (gMoveResultFlags & MOVE_RESULT_NO_EFFECT
	|| gLastPrintedMoves[gBankAttacker] != gChosenMove)
    {
        gDisableStructs[gBankAttacker].furyCutterCounter = 0;
    }
    else
    {
        if (gDisableStructs[gBankAttacker].furyCutterCounter < 4)
            gDisableStructs[gBankAttacker].furyCutterCounter++;
    }

	gBattlescriptCurrInstr++;
}

void atkB7_presentdamagecalculation(void)
{
    int rand = Random() & 0xFF;

    if (rand < 102)
        gDynamicBasePower = 40;
    else if (rand < 178)
        gDynamicBasePower = 80;
    else if (rand < 204)
        gDynamicBasePower = 120;
    else
    {
        gBattleMoveDamage = MathMax(gBattleMons[gBankTarget].maxHP / 4, 1);
        gBattleMoveDamage *= -1;
    }

    if (rand < 204)
        gBattlescriptCurrInstr = BattleScript_HitFromCritCalc;
	else if (gNewBS->HealBlockTimers[gBankTarget])
		gBattlescriptCurrInstr = BattleScript_NoHealTargetAfterHealBlock;
    else if (gBattleMons[gBankTarget].maxHP == gBattleMons[gBankTarget].hp)
        gBattlescriptCurrInstr = BattleScript_AlreadyAtFullHp;
    else
    {
		gNewBS->ParentalBondOn = FALSE;
		gMultiHitCounter = 0;
        gMoveResultFlags &= ~(MOVE_RESULT_DOESNT_AFFECT_FOE);
        gBattlescriptCurrInstr = BattleScript_PresentHealTarget;
    }
}

void atkB9_magnitudedamagecalculation(void)
{
	if (gDynamicBasePower) //Power has already been calculated
	{
		gBattlescriptCurrInstr = (u8*) 0x81D6930;
		return;
	}

    int magnitude = umodsi(Random(), 100);

    if (magnitude < 5)
    {
        gDynamicBasePower = 10;
        magnitude = 4;
    }
    else if (magnitude < 15)
    {
        gDynamicBasePower = 30;
        magnitude = 5;
    }
    else if (magnitude < 35)
    {
        gDynamicBasePower = 50;
        magnitude = 6;
    }
    else if (magnitude < 65)
    {
        gDynamicBasePower = 70;
        magnitude = 7;
    }
    else if (magnitude < 85)
    {
        gDynamicBasePower = 90;
        magnitude = 8;
    }
    else if (magnitude < 95)
    {
        gDynamicBasePower = 110;
        magnitude = 9;
    }
    else
    {
        gDynamicBasePower = 150;
        magnitude = 10;
    }

    PREPARE_BYTE_NUMBER_BUFFER(gBattleTextBuff1, 2, magnitude)
    gBattlescriptCurrInstr++;
}


//In this function, the target attacks the attacker
void atkBA_jumpifnopursuitswitchdmg(void) {
	int i;

	switch (gMultiHitCounter) {
		case 1:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_LEFT);
			break;

		default:
			if (SIDE(gBankAttacker) == B_SIDE_PLAYER)
				gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
			else
				gBankTarget = GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT);
	}


    if (gActionForBanks[gBankTarget] == ACTION_USE_MOVE
	&& gChosenMovesByBanks[gBankTarget] == MOVE_PURSUIT
	//&& gBankAttacker == gBattleStruct->moveTarget[gBankTarget] //Used pre Gen 4
	&& !(gBattleMons[gBankTarget].status1 & (STATUS_SLEEP | STATUS_FREEZE))
    && gBattleMons[gBankAttacker].hp
	&& !gDisableStructs[gBankTarget].truantCounter) {
        for (i = 0; i < gBattlersCount; i++) {
            if (gBanksByTurnOrder[i] == gBankTarget)
                gActionsByTurnOrder[i] = 11;
        }

        gCurrentMove = MOVE_PURSUIT;
        gBattlescriptCurrInstr += 5;
        gBattleScripting->animTurn = 1;
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkBB_setsunny(void)
{
    if (gBattleWeather & (WEATHER_SUN_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }

    else {
        gBattleWeather = WEATHER_SUN_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 4;

		if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_HEAT_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atkBC_maxattackhalvehp(void)
{
    u32 halfHp = gBattleMons[gBankAttacker].maxHP / 2;

    if (gBattleMons[gBankAttacker].maxHP / 2 == 0)
	{
        halfHp = 1;
	}

	if (ABILITY(gBankAttacker) == ABILITY_CONTRARY
	&& gBattleMons[gBankAttacker].statStages[STAT_ATK-1] > 0
	&& gBattleMons[gBankAttacker].hp > halfHp)
	{
        gBattleMons[gBankAttacker].statStages[STAT_ATK-1] = 0;
        gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
        gBattlescriptCurrInstr += 5;
	}
    if (gBattleMons[gBankAttacker].statStages[STAT_ATK-1] < 12
    && gBattleMons[gBankAttacker].hp > halfHp)
    {
        gBattleMons[gBankAttacker].statStages[STAT_ATK-1] = 12;
        gBattleMoveDamage = MathMax(1, gBattleMons[gBankAttacker].maxHP / 2);
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkBE_rapidspinfree(void) {
	u8 bankAtk = gBankAttacker;
	u8 sideAtk = SIDE(bankAtk);
	u8 sideDef = SIDE(gBankTarget);

	if (gCurrentMove != MOVE_DEFOG) {
		if (gBattleMons[bankAtk].status2 & STATUS2_WRAPPED)
		{
			gBattleMons[bankAtk].status2 &= ~(STATUS2_WRAPPED);
			gBankTarget = gBattleStruct->wrappedBy[bankAtk];

			PREPARE_MOVE_BUFFER(gBattleTextBuff1, gBattleStruct->wrappedMove[bankAtk]);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_WrapFree;
		}
		else if (gStatuses3[bankAtk] & STATUS3_LEECHSEED)
		{
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED);
			gStatuses3[bankAtk] &= ~(STATUS3_LEECHSEED_BATTLER);
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_LeechSeedFree;
		}
		else if (gSideAffecting[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideAffecting[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_SpikesFree;
		}
		else
			gBattlescriptCurrInstr++;
	}
	else //Defog
	{
		if (gSideAffecting[sideAtk] & SIDE_STATUS_SPIKES)
		{
			gSideAffecting[sideAtk] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideAtk].spikesAmount = 0;
			gSideTimers[sideAtk].tspikesAmount = 0;
			gSideTimers[sideAtk].srAmount = 0;
			gSideTimers[sideAtk].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			BattleStringLoader = RemovedEntryHazardsString;
		}
		else if (gSideAffecting[sideDef] & SIDE_STATUS_SPIKES)
		{
			gSideAffecting[sideDef] &= ~(SIDE_STATUS_SPIKES);
			gSideTimers[sideDef].spikesAmount = 0;
			gSideTimers[sideDef].tspikesAmount = 0;
			gSideTimers[sideDef].srAmount = 0;
			gSideTimers[sideDef].stickyWeb = 0;
			BattleScriptPushCursor();
			gBattlescriptCurrInstr = BattleScript_PrintCustomString;
			BattleStringLoader = RemovedEntryHazardsTargetSideString;
		}
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_REFLECT))
		{
			gSideTimers[sideDef].reflectTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_REFLECT, SIDE_STATUS_REFLECT, sideDef);
		}
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_LIGHTSCREEN))
		{
			gSideTimers[sideDef].lightscreenTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_LIGHTSCREEN, SIDE_STATUS_LIGHTSCREEN, sideDef);
		}
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_SAFEGUARD))
		{
			gSideTimers[sideDef].safeguardTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_SAFEGUARD, SIDE_STATUS_SAFEGUARD, sideDef);
		}
		else if (gSideAffecting[sideDef] & (SIDE_STATUS_MIST))
		{
			gSideTimers[sideDef].mistTimer = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_MIST, SIDE_STATUS_MIST, sideDef);
		}
		else if (gNewBS->AuroraVeilTimers[sideDef])
		{
			gNewBS->AuroraVeilTimers[sideDef] = 0;
			TEXT_BUFFER_SIDE_STATUS(MOVE_AURORAVEIL, 0, sideDef);
		}
		else
			gBattlescriptCurrInstr++;
	}
}

void atkC0_recoverbasedonsunlight(void) {
    gBankTarget = gBankAttacker;
    if (gBattleMons[gBankAttacker].hp < gBattleMons[gBankAttacker].maxHP) {

		if (!gBattleWeather || !WEATHER_HAS_EFFECT)
			gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 2;

		else if (gCurrentMove == MOVE_SHOREUP && gBattleWeather & WEATHER_SANDSTORM_ANY)
			gBattleMoveDamage = udivsi(20 * gBattleMons[gBankAttacker].maxHP, 30);

		else if (gBattleWeather & WEATHER_SUN_ANY)
			gBattleMoveDamage = udivsi(20 * gBattleMons[gBankAttacker].maxHP, 30);

		else //Not Sunny Weather
			gBattleMoveDamage = gBattleMons[gBankAttacker].maxHP / 4;

		gBattleMoveDamage = MathMax(1, gBattleMoveDamage);
        gBattleMoveDamage *= -1;
        gBattlescriptCurrInstr += 5;
    }

    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkC2_selectfirstvalidtarget(void) {
    for (gBankTarget = 0; gBankTarget < gBattlersCount; ++gBankTarget) {
        if (gBankTarget == gBankAttacker) continue;

        if (!(gAbsentBattlerFlags & gBitTable[gBankTarget])) break;
    }

	//Stops certain animations from acting like they only target the partner
	if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
	&&  gBankAttacker == GetBattlerAtPosition(B_POSITION_PLAYER_RIGHT)
	&&  gBankTarget == GetBattlerAtPosition(B_POSITION_PLAYER_LEFT))
	{
		if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_LEFT);
		else if (IsBattlerAlive(GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT)))
			gBankTarget = GetBattlerAtPosition(B_POSITION_OPPONENT_RIGHT);
	}

    gBattlescriptCurrInstr++;
}

void atkC3_trysetfutureattack(void) {
    if (gWishFutureKnock->futureSightCounter[gBankTarget] != 0)
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else
    {
        gWishFutureKnock->futureSightMove[gBankTarget] = gCurrentMove;
        gWishFutureKnock->futureSightAttacker[gBankTarget] = gBankAttacker;
		gWishFutureKnock->futureSightPartyIndex[gBankTarget] = gBattlerPartyIndexes[gBankAttacker];
        gWishFutureKnock->futureSightCounter[gBankTarget] = 3;

        if (gCurrentMove == MOVE_DOOMDESIRE)
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;

        gBattlescriptCurrInstr += 5;
    }
}

void atkC5_setsemiinvulnerablebit(void) {
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] |= STATUS3_IN_AIR;
			break;

		case MOVE_DIG:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERGROUND;
			break;

		case MOVE_DIVE:
			gStatuses3[gBankAttacker] |= STATUS3_UNDERWATER;
			break;

		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] |= STATUS3_DISAPPEARED;
			break;

		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] |= (STATUS3_SKY_DROP_ATTACKER | STATUS3_IN_AIR);
			gStatuses3[gBankTarget] |= (STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);

			gNewBS->skyDropAttackersTarget[gBankAttacker] = gBankTarget;
			gNewBS->skyDropTargetsAttacker[gBankTarget] = gBankAttacker;

			if (gSideTimers[SIDE(gBankTarget)].followmeTarget == gBankTarget) //Removes Follow Me's effect
				gSideTimers[SIDE(gBankTarget)].followmeTimer = 0;
    }
    gBattlescriptCurrInstr++;
}

void atkC6_clearsemiinvulnerablebit(void) {
    switch (gCurrentMove) {
		case MOVE_FLY:
		case MOVE_BOUNCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_IN_AIR;
			break;
		case MOVE_DIG:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERGROUND;
			break;
		case MOVE_DIVE:
			gStatuses3[gBankAttacker] &= ~STATUS3_UNDERWATER;
			break;
		case MOVE_SHADOWFORCE:
		case MOVE_PHANTOMFORCE:
			gStatuses3[gBankAttacker] &= ~STATUS3_DISAPPEARED;
			break;
		case MOVE_SKYDROP:
			gStatuses3[gBankAttacker] &= ~(STATUS3_SKY_DROP_ATTACKER | STATUS3_IN_AIR);
			gStatuses3[gBankTarget] &= ~(STATUS3_SKY_DROP_TARGET | STATUS3_IN_AIR);
			gNewBS->skyDropAttackersTarget[gBankAttacker] = 0;
			gNewBS->skyDropTargetsAttacker[gBankTarget] = 0;
			break;
    }
    gBattlescriptCurrInstr++;
}

void atkC8_sethail(void) {
    if (gBattleWeather & (WEATHER_HAIL_ANY | WEATHER_PRIMAL_ANY)) {
        gMoveResultFlags |= MOVE_RESULT_FAILED;
        gBattleCommunication[MULTISTRING_CHOOSER] = 2;
    }

    else {
        gBattleWeather = WEATHER_HAIL_TEMPORARY;
        gBattleCommunication[MULTISTRING_CHOOSER] = 5;

        if (ITEM_EFFECT(gBankAttacker) == ITEM_EFFECT_ICY_ROCK)
			gWishFutureKnock->weatherDuration = 8;
		else
			gWishFutureKnock->weatherDuration = 5;
    }
    gBattlescriptCurrInstr++;
}

void atkC9_jumpifattackandspecialattackcannotfall(void) // memento
{
    if (!STAT_CAN_FALL(gBankTarget, STAT_ATK)
    && !STAT_CAN_FALL(gBankTarget, STAT_SPATK)
    && gBattleCommunication[6] != 1)
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
    else
    {
        gActiveBattler = gBankAttacker;
        gBattleMoveDamage = gBattleMons[gActiveBattler].hp;
        EmitHealthBarUpdate(0, INSTANT_HP_BAR_DROP);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
}

void atkCA_setforcedtarget(void) //Follow me
{
	if (gCurrentMove == MOVE_SPOTLIGHT)
	{
		gSideTimers[SIDE(gBankTarget)].followmeTimer = 1;
		gSideTimers[SIDE(gBankTarget)].followmeTarget = gBankTarget;
		gBattlescriptCurrInstr++;
	}
	else
	{
		gSideTimers[SIDE(gBankAttacker)].followmeTimer = 1;
		gSideTimers[SIDE(gBankAttacker)].followmeTarget = gBankAttacker;
		gBattlescriptCurrInstr++;
	}
}

void atkCC_callterrainattack(void) { //nature power
    gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);

	gCurrentMove = GetNaturePowerMove();

    gBankTarget = GetMoveTarget(gCurrentMove, 0);
	gBattleStruct->atkCancellerTracker = CANCELLER_GRAVITY_2;
    BattleScriptPush(gBattleScriptsForMoveEffects[gBattleMoves[gCurrentMove].effect]);
    gBattlescriptCurrInstr++;
}

u16 GetNaturePowerMove(void)
{
	u16 move;

	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			move = gTerrainTable[0].naturePowerMove;
			break;

		case GRASSY_TERRAIN:
			move = gTerrainTable[1].naturePowerMove;
			break;

		case MISTY_TERRAIN:
			move = gTerrainTable[2].naturePowerMove;
			break;

		case PSYCHIC_TERRAIN:
			move = gTerrainTable[3].naturePowerMove;
			break;

		default:
			if (IsTerrainMoveIndoors())
				move = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].naturePowerMove;
			else
				move = gTerrainTable[gBattleTerrain + 4].naturePowerMove;
	}
	
	return move;
}

void atkCF_jumpifnodamage(void)
{
    if (gProtectStructs[gBankAttacker].physicalDmg
	||  gProtectStructs[gBankAttacker].specialDmg
	||  !(gNewBS->playedFocusPunchMessage & gBitTable[gBankAttacker])) //Forced to use Focus Punch by Encore
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkD0_settaunt(void)
{
    if (gDisableStructs[gBankTarget].tauntTimer == 0)
    {
        gDisableStructs[gBankTarget].tauntTimer = 4;
        gDisableStructs[gBankTarget].tauntTimer2 = 4;
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkD2_tryswapitems(void) { //Trick
    // Wild Pokemon can't swap items with player
    if (SIDE(gBankAttacker) == B_SIDE_OPPONENT && !(gBattleTypeFlags & (BATTLE_TYPE_TRAINER)))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else {
        // can't swap if two pokemon don't have an item
        // or if either of them is a forbidden item
        if ((gBattleMons[gBankAttacker].item == 0 && gBattleMons[gBankTarget].item == 0)
        || !CanTransferItem(gBattleMons[gBankAttacker].species, gBattleMons[gBankAttacker].item, GetBankPartyData(gBankAttacker))
        || !CanTransferItem(gBattleMons[gBankAttacker].species, gBattleMons[gBankTarget].item, GetBankPartyData(gBankAttacker))
        || !CanTransferItem(gBattleMons[gBankTarget].species, gBattleMons[gBankTarget].item, GetBankPartyData(gBankTarget))
        || !CanTransferItem(gBattleMons[gBankTarget].species, gBattleMons[gBankAttacker].item, GetBankPartyData(gBankTarget)))
            gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

        // check if ability prevents swapping
        else if (ABILITY(gBankTarget) == ABILITY_STICKYHOLD)  {
            gBattlescriptCurrInstr = BattleScript_StickyHoldActivates;
            gLastUsedAbility = gBattleMons[gBankTarget].ability;
            RecordAbilityBattle(gBankTarget, gLastUsedAbility);
        }

        // took a while, but all checks passed and items can be safely swapped
        else {
            u16 oldItemAtk, *newItemAtk;

            newItemAtk = &gBattleStruct->changedItems[gBankAttacker];
            oldItemAtk = gBattleMons[gBankAttacker].item;
            *newItemAtk = gBattleMons[gBankTarget].item;

            gBattleMons[gBankAttacker].item = 0;
            gBattleMons[gBankTarget].item = oldItemAtk;

            gActiveBattler = gBankAttacker;
            EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, newItemAtk);
            MarkBufferBankForExecution(gBankAttacker);

            gActiveBattler = gBankTarget;
            EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gBankTarget].item);
            MarkBufferBankForExecution(gBankTarget);

			gBattleStruct->choicedMove[gBankTarget] = 0;
            gBattleStruct->choicedMove[gBankTarget] = 0;

            gBattleStruct->choicedMove[gBankAttacker] = 0;
            gBattleStruct->choicedMove[gBankAttacker] = 0;

            gBattlescriptCurrInstr += 5;

            PREPARE_ITEM_BUFFER(gBattleTextBuff1, *newItemAtk);
            PREPARE_ITEM_BUFFER(gBattleTextBuff2, oldItemAtk);

            if (oldItemAtk != 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2; // attacker's item -> <- target's item
            else if (oldItemAtk == 0 && *newItemAtk != 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 0; // nothing -> <- target's item
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = 1; // attacker's item -> <- nothing
        }
    }
}

void atkD3_trycopyability(void) //Role Play
{
	if (gBattleExecBuffer) return;

	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBankAttacker);
	defAbilityLoc = GetAbilityLocation(gBankTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == defAbility
	||  defAbility == ABILITY_NONE
	||  CheckTableForAbility(atkAbility, RolePlayAttackerBanTable)
	||  CheckTableForAbility(defAbility, RolePlayBanTable))
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

	else
	{
		*atkAbilityLoc = defAbility;
        gLastUsedAbility = atkAbility; //To display what changed
		TransferAbilityPopUp(gBankAttacker, gLastUsedAbility);

		gNewBS->SlowStartTimers[gBankAttacker] = 0;
		gStatuses3[gBankAttacker] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE);
        gBattlescriptCurrInstr += 5;
    }
}

void atkDA_tryswapabilities(void) { //Skill Swap
	u8* atkAbilityLoc, *defAbilityLoc;
	u8 atkAbility, defAbility;

	//Get correct location of ability
	atkAbilityLoc = GetAbilityLocation(gBankAttacker);
	defAbilityLoc = GetAbilityLocation(gBankTarget);

	atkAbility = *atkAbilityLoc;
	defAbility = *defAbilityLoc;

	if (atkAbility == ABILITY_NONE || defAbility == ABILITY_NONE
	|| CheckTableForAbility(atkAbility, SkillSwapBanTable) || CheckTableForAbility(defAbility, SkillSwapBanTable)
	|| gMoveResultFlags & MOVE_RESULT_NO_EFFECT)
	{
		gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
	}
    else
	{
        *atkAbilityLoc = defAbility;
        *defAbilityLoc = atkAbility;
		gNewBS->SlowStartTimers[gBankAttacker] = 0;
		gNewBS->SlowStartTimers[gBankTarget] = 0;
		gStatuses3[gBankAttacker] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE);
		gStatuses3[gBankTarget] &= ~(STATUS3_SWITCH_IN_ABILITY_DONE);
        gBattlescriptCurrInstr += 5;
    }
}

void atkDB_tryimprision(void) {
    if ((gStatuses3[gBankAttacker] & STATUS3_IMPRISONED))
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);

    else {
        PressurePPLoseOnUsingImprision(gBankAttacker);
        gStatuses3[gBankAttacker] |= STATUS3_IMPRISONED;
        gBattlescriptCurrInstr += 5;
    }
}

void atkDE_asistattackselect(void)
{
    s32 chooseableMovesNo = 0;
    struct Pokemon* party;
    s32 monId, moveId;
    u16* movesArray = gBattleStruct->assistPossibleMoves;
	u8 firstMonId, lastMonId;

    party = LoadPartyRange(gBankAttacker, &firstMonId, &lastMonId);

    for (monId = firstMonId; monId < lastMonId; ++monId)
    {
        if (monId == gBattlerPartyIndexes[gBankAttacker]
		|| party[monId].species == SPECIES_NONE
        || GetMonData(&party[monId], MON_DATA_IS_EGG, 0))
            continue;

        for (moveId = 0; moveId < MAX_MON_MOVES; moveId++)
        {
            u16 move = party[monId].moves[moveId];

            if (CheckTableForMove(move, AssistBanTable))
                continue;

            if (move == MOVE_NONE)
                continue;

            movesArray[chooseableMovesNo] = move;
            chooseableMovesNo++;
        }
    }
    if (chooseableMovesNo)
    {
        gHitMarker &= ~(HITMARKER_ATTACKSTRING_PRINTED);
        gRandomMove = movesArray[((Random() & 0xFF) * chooseableMovesNo) >> 8];
        gBankTarget = GetMoveTarget(gRandomMove, 0);
        gBattlescriptCurrInstr += 5;
    }
    else
    {
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
    }
}

void atkE4_getsecretpowereffect(void) {
	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[0].secretPowerEffect;
			break;

		case GRASSY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[1].secretPowerEffect;
			break;

		case MISTY_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[2].secretPowerEffect;
			break;

		case PSYCHIC_TERRAIN:
			gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[3].secretPowerEffect;
			break;

		default:
			if (IsTerrainMoveIndoors())
				gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].secretPowerEffect;
			else
				gBattleCommunication[MOVE_EFFECT_BYTE] = gTerrainTable[gBattleTerrain + 4].secretPowerEffect;
	}
    gBattlescriptCurrInstr++;
}

void atkE5_pickupitemcalculation(void) {
	u16 item = 0;
	u8 chance = 0;

    for (int i = 0; i < 6; ++i)
	{
        if (gPlayerParty[i].species == SPECIES_NONE) break;
		if (gPlayerParty[i].item != ITEM_NONE) continue;
        if (GetMonData(&gPlayerParty[i], MON_DATA_IS_EGG, 0)) continue;

		u8 level = gPlayerParty[i].level;

		switch (GetPartyAbility(&gPlayerParty[i])) {
			case ABILITY_PICKUP:
				chance = 10; // ~10% chance of pickup to activate
				item = ChoosePickupItem(level);
				break;

			case ABILITY_HONEYGATHER:
				chance = 5 + 5 * udivsi((level - 1), 10);
				item = ITEM_HONEY;
		}

		if (Random() % 100 < chance)
			gPlayerParty[i].item = item;
    }

    ++gBattlescriptCurrInstr;
}

static item_t ChoosePickupItem(u8 level)
{
    u32 row_num = (level - 1) / 10;
    const item_t* const common_row = sPickupCommonItems + row_num;
    const item_t* const rare_row = sPickupRareItems + row_num;

    u16 k = Random();

    for (u32 i = 0; i < PU_COMMON_PER_ROW; ++i)
        if (k < pickup_common_item_ceilings[i])
            return common_row[i];

    for (u32 i = 0; i < PU_RARE_PER_ROW; ++i)
        if (k < pickup_rare_item_ceilings[i])
            return rare_row[i];

    // will never be reached
    return ITEM_NONE;
}

void atkE7_trycastformdatachange(void)
{
	if (gBattleExecBuffer) return;

    u8 form;
	u8 bank = gBattleScripting->bank;

    gBattlescriptCurrInstr++;

	switch (SPECIES(bank)) { //Not ability b/c you can lose ability
		case SPECIES_CASTFORM:
			form = CastformDataTypeChange(bank);
			if (form)
			{
				BattleScriptPushCursorAndCallback(BattleScript_CastformChange);
				gBattleStruct->castformToChangeInto = form - 1;
			}
			return;

		case SPECIES_CHERRIM:
			if (ABILITY(bank) == ABILITY_FLOWERGIFT && !IS_TRANSFORMED(bank)
			&& WEATHER_HAS_EFFECT && gBattleWeather & WEATHER_SUN_ANY)
			{
				DoFormChange(bank, SPECIES_CHERRIM_SUN, FALSE, FALSE);
				BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
			}
			break;

		case SPECIES_CHERRIM_SUN:
			if (ABILITY(bank) != ABILITY_FLOWERGIFT
			|| !WEATHER_HAS_EFFECT || !(gBattleWeather & WEATHER_SUN_ANY))
			{
				DoFormChange(bank, SPECIES_CHERRIM, FALSE, FALSE);
				BattleScriptPushCursorAndCallback(BattleScript_FlowerGift);
			}
	}
}

u8 CastformDataTypeChange(u8 bank) {
    u8 formChange = 0;
    if (gBattleMons[bank].species != SPECIES_CASTFORM || gBattleMons[bank].hp == 0)
        return CASTFORM_NO_CHANGE;

    if ((!WEATHER_HAS_EFFECT || ABILITY(bank) != ABILITY_FORECAST) && gBattleMonForms[bank] != CASTFORM_NORMAL) {
        SET_BATTLER_TYPE(bank, TYPE_NORMAL);
        return CASTFORM_TO_NORMAL;
    }

	if (WEATHER_HAS_EFFECT && ABILITY(bank) == ABILITY_FORECAST) {

		if (!(gBattleWeather & (WEATHER_RAIN_ANY | WEATHER_SUN_ANY | WEATHER_HAIL_ANY)) && gBattleMonForms[bank] != CASTFORM_NORMAL) {
			SET_BATTLER_TYPE(bank, TYPE_NORMAL);
			formChange = CASTFORM_TO_NORMAL;
		}

		else if (gBattleWeather & WEATHER_SUN_ANY &&gBattleMonForms[bank] != CASTFORM_SUN) {
			SET_BATTLER_TYPE(bank, TYPE_FIRE);
			formChange = CASTFORM_TO_FIRE;
		}

		else if (gBattleWeather & WEATHER_RAIN_ANY && gBattleMonForms[bank] != CASTFORM_RAIN) {
			SET_BATTLER_TYPE(bank, TYPE_WATER);
			formChange = CASTFORM_TO_WATER;
		}

		else if (gBattleWeather & WEATHER_HAIL_ANY && gBattleMonForms[bank] != CASTFORM_HAIL) {
			SET_BATTLER_TYPE(bank, TYPE_ICE);
			formChange = CASTFORM_TO_ICE;
		}

		return formChange;
	}
    return CASTFORM_NO_CHANGE;
}

void atkE8_settypebasedhalvers(void) { //water/mud sport
    bool8 worked = FALSE;

    if (gBattleMoves[gCurrentMove].effect == EFFECT_MUD_SPORT)
    {
        if (!gNewBS->MudSportTimer) {
            gNewBS->MudSportTimer = 5;
            gBattleCommunication[MULTISTRING_CHOOSER] = 0;
            worked = TRUE;
        }
    }
    else { //Water Sport
        if (!gNewBS->WaterSportTimer) {
            gNewBS->WaterSportTimer = 5;
            gBattleCommunication[MULTISTRING_CHOOSER] = 1;
            worked = TRUE;
        }
    }

    if (worked)
        gBattlescriptCurrInstr += 5;
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEA_tryrecycleitem(void) {
    gActiveBattler = gBankAttacker;
	u16 item = SAVED_CONSUMED_ITEMS(gActiveBattler);

    if (item && gBattleMons[gActiveBattler].item == 0) {
        gLastUsedItem = item;
        SAVED_CONSUMED_ITEMS(gActiveBattler) = 0;
		CONSUMED_ITEMS(gActiveBattler) = 0;	//Remove the temporary item storage
		RemoveBankFromPickupStack(gActiveBattler);
        gBattleMons[gActiveBattler].item = gLastUsedItem;
        EmitSetMonData(0, REQUEST_HELDITEM_BATTLE, 0, 2, &gBattleMons[gActiveBattler].item);
        MarkBufferBankForExecution(gActiveBattler);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

void atkEB_settypetoterrain(void) {
	u8 type = GetCamouflageType();

    if (!IsOfType(gBankAttacker, type))
    {
        gBattleMons[gBankAttacker].type1 = type;
        gBattleMons[gBankAttacker].type2 = type;
		gBattleMons[gBankAttacker].type3 = TYPE_BLANK;
        PREPARE_TYPE_BUFFER(gBattleTextBuff1, type);
        gBattlescriptCurrInstr += 5;
    }
    else
        gBattlescriptCurrInstr = T1_READ_PTR(gBattlescriptCurrInstr + 1);
}

u8 GetCamouflageType(void)
{
	u8 type;

	switch (TerrainType) {
		case ELECTRIC_TERRAIN:
			type = gTerrainTable[0].camouflageType;
			break;

		case GRASSY_TERRAIN:
			type = gTerrainTable[1].camouflageType;
			break;

		case MISTY_TERRAIN:
			type = gTerrainTable[2].camouflageType;
			break;

		case PSYCHIC_TERRAIN:
			type = gTerrainTable[3].camouflageType;
			break;

		default:
			if (IsTerrainMoveIndoors())
				type = gTerrainTable[BATTLE_TERRAIN_INSIDE + 4].camouflageType;
			else
				type = gTerrainTable[gBattleTerrain + 4].camouflageType;
	}

	return type;
}

void atkEE_removelightscreenreflect(void) { //Brick Break
    u8 side = SIDE(gBankTarget);
    if (gSideTimers[side].reflectTimer
	||  gSideTimers[side].lightscreenTimer
	||  gSideAffecting[side] & SIDE_STATUS_REFLECT
	||  gSideAffecting[side] & SIDE_STATUS_LIGHTSCREEN
	||  gNewBS->AuroraVeilTimers[side])
	{
        //Now handled as a move effect
		/*gSideAffecting[side] &= ~(SIDE_STATUS_REFLECT);
        gSideAffecting[side] &= ~(SIDE_STATUS_LIGHTSCREEN);
        gSideTimers[side].reflectTimer = 0;
        gSideTimers[side].lightscreenTimer = 0;
		gNewBS->AuroraVeilTimers[side] = 0;*/
        gBattleScripting->animTurn = 1;
        gBattleScripting->animTargetsHit = 1;
    }
    else {
        gBattleScripting->animTurn = 0;
        gBattleScripting->animTargetsHit = 0;
    }
    gBattlescriptCurrInstr++;
}
