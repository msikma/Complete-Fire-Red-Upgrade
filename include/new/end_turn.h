#pragma once

#include "../global.h"

/**
 * \file end_turns.h
 * \brief Contains functions relating to the end of each turn after all Pokemon
 *		  on the field have acted. Also contains functions the implement the Gen 4+
 *		  switch-in after end turn feature (the end turn effects activate, and then
 *		  any fainted Pokemon are replaced). The order of the events found here are based on:
 *		  https://bulbapedia.bulbagarden.net/wiki/User:SnorlaxMonster/End-turn_resolution_order.
 */
 
//Exported Functions
u8 CountAliveMonsOnField(void);

//Functions Hooked In
u8 TurnBasedEffects(void);
bool8 HandleFaintedMonActions(void);
void RemoveSwitchInForFaintedBank(void);