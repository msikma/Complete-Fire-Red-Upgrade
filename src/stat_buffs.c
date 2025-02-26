#include "defines.h"
#include "defines_battle.h"
#include "../include/battle_anim.h"
#include "../include/battle_string_ids.h"

#include "../include/new/ability_battle_scripts.h"
#include "../include/new/accuracy_calc.h"
#include "../include/new/battle_strings.h"
#include "../include/new/helper_functions.h"
#include "../include/new/stat_buffs.h"

#define BattleScript_MistProtected (u8*) 0x81D8C3E

extern u8 DrasticallyString[];
extern u8 SeverelyString[];

void atk13_printfromtable(void)
{
    if (gBattleExecBuffer) return;

    u16* ptr = (u16*)T1_READ_PTR(gBattlescriptCurrInstr + 1);
    ptr += gBattleCommunication[MULTISTRING_CHOOSER];
	u16 stringId = *(u16*)ptr;
    PrepareStringBattle(stringId, gBankAttacker);
    gBattlescriptCurrInstr += 5;
    gBattleCommunication[MSG_DISPLAY] = 1;
	
	if (stringId == STRINGID_PKMNSSTATCHANGED4 && SIDE(gBankTarget) != SIDE(gBankAttacker))
	{ //Stat Fell From Enemy
		switch (ABILITY(gBankTarget)) {
			case ABILITY_DEFIANT:
				gBattleScripting->statChanger = INCREASE_2 | STAT_STAGE_ATK;
				break;
			
			case ABILITY_COMPETITIVE:
				gBattleScripting->statChanger = INCREASE_2 | STAT_STAGE_SPATK;
				break;
			
			default:
				return;
		}
		BattleScriptPushCursor();
		gBattlescriptCurrInstr = BattleScript_DefiantCompetitive;
        gBattleScripting->bank = gBankTarget;
	}
}

void atk20_jumpifstat(void)
{
    u8 ret = 0;
    u8 bank = GetBattleBank(gBattlescriptCurrInstr[1]);
    u8 value = gBattleMons[bank].statStages[gBattlescriptCurrInstr[3] - 1];
	s8 comparison = gBattlescriptCurrInstr[4];
	
	if (ABILITY(bank) == ABILITY_CONTRARY)
	{
		comparison -= 12;
		comparison *= - 1;
	}
	
    switch (T2_READ_8(gBattlescriptCurrInstr + 2)) {
		case CMP_EQUAL:
			if (value == comparison)
				ret++;
			break;
		case CMP_NOT_EQUAL:
			if (value != comparison)
				ret++;
			break;
		case CMP_GREATER_THAN:
			if (value > comparison)
				ret++;
			break;
		case CMP_LESS_THAN:
			if (value < comparison)
				ret++;
			break;
		case CMP_COMMON_BITS:
			if (value & comparison)
				ret++;
			break;
		case CMP_NO_COMMON_BITS:
			if (!(value & comparison))
				ret++;
			break;
    }
	
    if (ret)
        gBattlescriptCurrInstr = T2_READ_PTR(gBattlescriptCurrInstr + 5);
    else
        gBattlescriptCurrInstr += 9;
}

void atk48_playstatchangeanimation(void)
{
    u32 currStat = 0;
    u16 statAnimId = 0;
    s32 changeableStatsCount = 0;
    u8 statsToCheck = 0;
	u8 flags = gBattlescriptCurrInstr[3];
	
    gActiveBattler = GetBattleBank(T2_READ_8(gBattlescriptCurrInstr + 1));
    statsToCheck = T2_READ_8(gBattlescriptCurrInstr + 2);
	
	if (ABILITY(gActiveBattler) == ABILITY_SIMPLE)
		flags |= ATK48_STAT_BY_TWO;

    if (flags & ATK48_STAT_NEGATIVE) // goes down
    {
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_UP;
	
	STAT_ANIM_DOWN:	;
        s16 startingStatAnimId;
		u8 ability = ABILITY(gActiveBattler);
		
        if (flags & ATK48_STAT_BY_TWO)
            startingStatAnimId = STAT_ANIM_MINUS2 - 1;
        else
            startingStatAnimId = STAT_ANIM_MINUS1 - 1;
		
        while (statsToCheck != 0) {
			
            if (statsToCheck & 1)
			{
				if (flags & ATK48_DONT_CHECK_LOWER)
				{
					if (gBattleMons[gActiveBattler].statStages[currStat - 1] > 0)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
				else if (!gSideTimers[SIDE(gActiveBattler)].mistTimer
						&& ability != ABILITY_CLEARBODY
						&& ability != ABILITY_WHITESMOKE
						&& ability != ABILITY_FULLMETALBODY
						&& !(ability == ABILITY_KEENEYE && currStat == STAT_STAGE_ACC)
						&& !(ability == ABILITY_HYPERCUTTER && currStat == STAT_STAGE_ATK)
						&& !(ability == ABILITY_BIGPECKS && currStat == STAT_STAGE_DEF))
				{
					if (gBattleMons[gActiveBattler].statStages[currStat - 1] > 0)
					{
						statAnimId = startingStatAnimId + currStat;
						changeableStatsCount++;
					}
				}
			}
            statsToCheck >>= 1, ++currStat;
        }

        if (changeableStatsCount > 1) // more than one stat, so the color is gray
        {
            if (flags & ATK48_STAT_BY_TWO)
                statAnimId = STAT_ANIM_MULTIPLE_MINUS2;
            else
                statAnimId = STAT_ANIM_MULTIPLE_MINUS1;
        }
    }
	
    else // goes up
    {
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY)
			goto STAT_ANIM_DOWN;
	
	STAT_ANIM_UP:	;
        s16 startingStatAnimId;
        if (flags & ATK48_STAT_BY_TWO)
            startingStatAnimId = STAT_ANIM_PLUS2 - 1;
        else
            startingStatAnimId = STAT_ANIM_PLUS1 - 1;

        while (statsToCheck != 0)
        {
            if (statsToCheck & 1 && gBattleMons[gActiveBattler].statStages[currStat - 1] < 12)
            {
                statAnimId = startingStatAnimId + currStat;
                changeableStatsCount++;
            }
            statsToCheck >>= 1, ++currStat;
        }

        if (changeableStatsCount > 1) // more than one stat, so the color is gray
        {
            if (flags & ATK48_STAT_BY_TWO)
                statAnimId = STAT_ANIM_MULTIPLE_PLUS2;
            else
                statAnimId = STAT_ANIM_MULTIPLE_PLUS1;
        }
    }

	if (gNewBS->SpectralThiefActive)
	{
		statAnimId = STAT_ANIM_MULTIPLE_MINUS1;
		changeableStatsCount = 0xFF;
	}
		
    if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount < 2)
	|| (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount < 3))
    {
        gBattlescriptCurrInstr += 4;
    }
    else if (changeableStatsCount != 0 && !gBattleScripting->statAnimPlayed)
    {
        EmitBattleAnimation(0, B_ANIM_STATS_CHANGE, statAnimId);
        MarkBufferBankForExecution(gActiveBattler);
        if ((T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_MULTIPLE && changeableStatsCount > 1)
		||  (T2_READ_8(gBattlescriptCurrInstr + 3) & ATK48_ONLY_TRIPLE && changeableStatsCount > 2))
            gBattleScripting->statAnimPlayed = TRUE;
        gBattlescriptCurrInstr += 4;
    }
    else
    {
        gBattlescriptCurrInstr += 4;
    }
}

void atk89_statbuffchange(void)
{
    u8* jump_loc = T1_READ_PTR(gBattlescriptCurrInstr + 2);
    if (ChangeStatBuffs(gBattleScripting->statChanger & 0xF0, gBattleScripting->statChanger & 0xF, T2_READ_8(gBattlescriptCurrInstr + 1), jump_loc) == 0)
        gBattlescriptCurrInstr += 6;
}

u8 ChangeStatBuffs(s8 statValue, u8 statId, u8 flags, u8* BS_ptr)
{
    bool8 certain = FALSE;
    bool8 notProtectAffected = FALSE;
    u32 index;

    if (flags & MOVE_EFFECT_AFFECTS_USER)
        gActiveBattler = gBankAttacker;
    else
        gActiveBattler = gBankTarget;
	
	u8 ability = ABILITY(gActiveBattler);
	
    flags &= ~(MOVE_EFFECT_AFFECTS_USER);

    if (flags & MOVE_EFFECT_CERTAIN)
        certain++;
    flags &= ~(MOVE_EFFECT_CERTAIN);

    if (flags & STAT_CHANGE_NOT_PROTECT_AFFECTED)
        notProtectAffected++;
    flags &= ~(STAT_CHANGE_NOT_PROTECT_AFFECTED);

    PREPARE_STAT_BUFFER(gBattleTextBuff1, statId)

    if ((statValue << 0x18) < 0) // stat decrease
    {
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gNewBS->ZMoveData->runningZEffect)
			goto RAISE_STAT_BUFF;
	
	LOWER_STAT_BUFF:
        if (gSideTimers[SIDE(gActiveBattler)].mistTimer
        && !certain 
		&& gCurrentMove != MOVE_CURSE 
		&& ABILITY(gBankAttacker) != ABILITY_INFILTRATOR)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                {
                    gBattlescriptCurrInstr = BS_ptr;
                }
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting->bank = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_MistProtected;
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
		
        else if (gCurrentMove != MOVE_CURSE
              && notProtectAffected != TRUE 
			  && JumpIfMoveAffectedByProtect(gCurrentMove, gBankAttacker, gBankTarget))
        {
            gBattlescriptCurrInstr = BattleScript_ButItFailed;
            return STAT_CHANGE_DIDNT_WORK;
        }
		
        else if ((ability == ABILITY_CLEARBODY 
			  ||  ability == ABILITY_WHITESMOKE 
			  ||  ability == ABILITY_FULLMETALBODY 
			  || (ability == ABILITY_FLOWERVEIL && IsOfType(gActiveBattler, TYPE_GRASS)))
		&& !certain && gCurrentMove != MOVE_CURSE)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                {
                    gBattlescriptCurrInstr = BS_ptr;
                }
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting->bank = gActiveBattler;
					gBattleCommunication[0] = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
                    gLastUsedAbility = gBattleMons[gActiveBattler].ability;
                    RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
		
		else if (gBattleTypeFlags & BATTLE_TYPE_DOUBLE
		&& IsOfType(gActiveBattler, TYPE_GRASS) 
		&& ABILITY(PARTNER(gActiveBattler)) == ABILITY_FLOWERVEIL
		&& !certain && gCurrentMove != MOVE_CURSE)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                if (gSpecialStatuses[gActiveBattler].statLowered)
                {
                    gBattlescriptCurrInstr = BS_ptr;
                }
                else
                {
                    BattleScriptPush(BS_ptr);
                    gBattleScripting->bank = PARTNER(gActiveBattler);
					gBattleCommunication[0] = gActiveBattler;
                    gBattlescriptCurrInstr = BattleScript_AbilityNoStatLoss;
                    gLastUsedAbility = ABILITY(PARTNER(gActiveBattler));
                    RecordAbilityBattle(PARTNER(gActiveBattler), gLastUsedAbility);
                    gSpecialStatuses[gActiveBattler].statLowered = 1;
                }
            }
            return STAT_CHANGE_DIDNT_WORK;
        }
		
        else if (((ability == ABILITY_KEENEYE && statId == STAT_STAGE_ACC)
			  ||  (ability == ABILITY_HYPERCUTTER && statId == STAT_STAGE_ATK)
			  ||  (ability == ABILITY_BIGPECKS && statId == STAT_STAGE_DEF))
        && !certain)
        {
            if (flags == STAT_CHANGE_BS_PTR)
            {
                BattleScriptPush(BS_ptr);
                gBattleScripting->bank = gActiveBattler;
                gBattlescriptCurrInstr = BattleScript_AbilityNoSpecificStatLoss;
                gLastUsedAbility = ability;
                RecordAbilityBattle(gActiveBattler, gLastUsedAbility);
            }
            return STAT_CHANGE_DIDNT_WORK;
        }

        else if ((ability == ABILITY_SHIELDDUST || SheerForceCheck()) && flags == 0)
            return STAT_CHANGE_DIDNT_WORK;
        
        else // try to decrease
        {
            statValue = -GET_STAT_BUFF_VALUE(statValue);
			
			if (ability == ABILITY_SIMPLE && !gNewBS->ZMoveData->runningZEffect)
				statValue *= 2;
			
            gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
            index = 1;
            if (statValue == -2)
            {
                gBattleTextBuff2[1] = B_BUFF_STRING;
                gBattleTextBuff2[2] = STRINGID_STATHARSHLY;
                gBattleTextBuff2[3] = STRINGID_STATHARSHLY >> 8;
                index = 4;
            }
			else if (statValue <= -3)
			{
				gBattleTextBuff2[1] = B_BUFF_STRING;
				gBattleTextBuff2[2] = 0x85;
				gBattleTextBuff2[3] = 0x1;
				index = 4;
            }
			if (!gNewBS->ZMoveData->runningZEffect) {
				gBattleTextBuff2[index] = B_BUFF_STRING;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL;
				index++;
				gBattleTextBuff2[index] = STRINGID_STATFELL >> 8;
				index++;
			}
            gBattleTextBuff2[index] = B_BUFF_EOS;

            if (gBattleMons[gActiveBattler].statStages[statId - 1] == 0)
                gBattleCommunication[MULTISTRING_CHOOSER] = 2;
            else
                gBattleCommunication[MULTISTRING_CHOOSER] = (gBankTarget == gActiveBattler);

        }
    }
	
	//Stat Increase
    else
    {
		if (ABILITY(gActiveBattler) == ABILITY_CONTRARY && !gNewBS->ZMoveData->runningZEffect)
			goto LOWER_STAT_BUFF;
		
	RAISE_STAT_BUFF:
        statValue = GET_STAT_BUFF_VALUE(statValue);
		
		if (ability == ABILITY_SIMPLE && !gNewBS->ZMoveData->runningZEffect)
			statValue *= 2;
				
        gBattleTextBuff2[0] = B_BUFF_PLACEHOLDER_BEGIN;
        index = 1;
        if (statValue == 2)
        {
            gBattleTextBuff2[1] = B_BUFF_STRING;
            gBattleTextBuff2[2] = STRINGID_STATSHARPLY;
            gBattleTextBuff2[3] = STRINGID_STATSHARPLY >> 8;
            index = 4;
        }
        if (statValue >= 3)
        {
            gBattleTextBuff2[1] = B_BUFF_STRING;
            gBattleTextBuff2[2] = 0x86;
            gBattleTextBuff2[3] = 0x1;
            index = 4;
        }
		if (!gNewBS->ZMoveData->runningZEffect) {
			gBattleTextBuff2[index] = B_BUFF_STRING;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE;
			index++;
			gBattleTextBuff2[index] = STRINGID_STATROSE >> 8;
			index++;
		}
        gBattleTextBuff2[index] = B_BUFF_EOS;

        if (gBattleMons[gActiveBattler].statStages[statId - 1] == 12)
            gBattleCommunication[MULTISTRING_CHOOSER] = 2;
        else
            gBattleCommunication[MULTISTRING_CHOOSER] = (gBankTarget == gActiveBattler);
    }

    gBattleMons[gActiveBattler].statStages[statId - 1] += statValue;
    if (gBattleMons[gActiveBattler].statStages[statId - 1] < 0)
        gBattleMons[gActiveBattler].statStages[statId - 1] = 0;
    if (gBattleMons[gActiveBattler].statStages[statId - 1] > 12)
        gBattleMons[gActiveBattler].statStages[statId - 1] = 12;

    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && flags & STAT_CHANGE_BS_PTR)
        gMoveResultFlags |= MOVE_RESULT_MISSED;

    if (gBattleCommunication[MULTISTRING_CHOOSER] == 2 && !(flags & STAT_CHANGE_BS_PTR))
        return STAT_CHANGE_DIDNT_WORK;

    return STAT_CHANGE_WORKED;
}