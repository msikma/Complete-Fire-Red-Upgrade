.thumb
.text
.align 2

.include "../Anim_Defines.asm"
/*.include "../asm_defines.s" Don't Uncomment This!*/

.global gBattleAnims_General	

.equ RED_PRIMAL_TASK, AnimTask_ReloadAttackerSprite
.equ RED_PRIMAL_TASK_2, AnimTask_PlayAttackerCry

gBattleAnims_General:
.word 0x81d5b88	@ ANIM_NOTHING		original 0x1c6ea8
.word 0x81d5bbe	@ ANIM_STAT_BUFF
.word 0x81d5bc7	@ ANIM_SUBSTITUTE_REMOVAL
.word 0x81d5c04	@ ANIM_CREATE_SUBSTITUTE
.word 0x81d5c0c	@ ANIM_POKEBLOCK_TRANSFORM
.word 0x81d5c54	@ ANIM_KNOCK_OFF_ITEM
.word ANIM_WRAPPED_END_TURN	@ ANIM_WRAP
.word ANIM_USEITEM	@ ANIM_ITEM_USE
.word 0x81d5e66	@ ANIM_SMOKE_BALL
.word 0x81d5f42	@ ANIM_CHARGE
.word 0x81d5f8f	@ ANIM_RAIN
.word 0x81d5fd8	@ ANIM_SUN
.word 0x81d5fdd	@ ANIM_SANDSTORM
.word 0x81d5fe2	@ ANIM_HAIL
.word 0x81d5fe7	@ ANIM_LEECH_SEED_HEAL
.word 0x81d5ff5	@ ANIM_HIT
.word 0x81d6026	@ ANIM_STEAL_ITEM
.word 0x81d604b	@ ANIM_WIGGLE
.word 0x81d60a9	@ ANIM_FUTURE_SIGHT_HIT
.word 0x81d6108	@ ANIM_DOOM_DESIRE_HIT
.word 0x81d61cd	@ ANIM_FOCUS_PUNCH_PUMP
.word 0x81d620d	@ ANIM_INGRAIN_RECOVER
.word 0x81d6250	@ ANIM_DARK_SCREEN_RECOVER
.word 0x81d628a	@ ANIM_ASTONISH_DROPS
.word 0x81d6301	@ ANIM_SCARY_FACE_ASTONISH
.word 0x81d637b	@ ANIM_TRANSFORM_FRONT
.word 0x81d6394	@ ANIM_TURN_INTO_ROCK
.word 0x81d63dc	@ ANIM_WAITING_WAGGLE
.word 0x81d64b6	@ ANIM_LEVEL_UP_SHINE
.word 0x81d64d7	@ ANIM_CALL_BACK_POKEMON
.word 0x81d64e8	@ ANIM_CALL_BACK_POKEMON_2
.word 0x81d64f9	@ ANIM_CAPTURE_POKEMON
.word 0x81d657b	@ ANIM_TURN_INTO_POKEBALL
.word 0x81d6594	@ ANIM_SWITCH
.word 0x81d659e	@ ANIM_CREATE_SUBSTITUTE_2
.word 0x81d2811 @ ANIM_TRANSFORM
.word ANIM_WISHIWASHI_FISH
.word ANIM_ZYGARDE_CELL_SWIRL
.word ANIM_DELTA_STREAM
.word ANIM_ELECTRIC_SURGE
.word ANIM_GRASSY_SURGE
.word ANIM_MISTY_SURGE
.word ANIM_PSYCHIC_SURGE
.word ANIM_SEA_OF_FIRE
.word ANIM_HEALING_SPARKLES
.word ANIM_LUNAR_DANCE_HEAL
.word ANIM_HEALING_WISH_HEAL
.word ANIM_RED_PRIMAL_REVERSION
.word ANIM_BLUE_PRIMAL_REVERSION
.word ANIM_GRASSY_TERRAIN_HEAL
.word ANIM_POWDER_EXPLOSION
.word ANIM_BEAK_BLAST_WARM_UP
.word ANIM_SHELL_TRAP_SET
.word ANIM_BERRY_EAT
.word ANIM_FOG_CONTINUES
.word ANIM_AQUA_RING_HEAL
.word ELECTRIC_TERRAIN_ACTIVE_ANIM
.word GRASSY_TERRAIN_ACTIVE_ANIM
.word MISTY_TERRAIN_ACTIVE_ANIM
.word PSYCHIC_TERRAIN_ACTIVE_ANIM
.word BATON_PASS_ANIM
.word DRAGON_TAIL_BLOW_AWAY_ANIM
.word ANIM_ZMOVE_ACTIVATE
.word ANIM_MEGA_EVOLUTION
.word ANIM_ULTRA_BURST
.word ANIM_LOAD_DEFAULT_BG
.word ANIM_LOAD_ABILITY_POP_UP
.word ANIM_REMOVE_ABILITY_POP_UP

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_USEITEM:
	loadparticle ANIM_TAG_THIN_RING 
	loadparticle ANIM_TAG_SPARKLE_2 
	pause 0x0
	launchtask AnimTask_pal_fade 0xa 0x5 PAL_DEF 0x2 0x0 0x9 0x7FFF
	playsound2 0x91 0xc0 
	launchtask 0x8099A79 0x2 0x4 0x10 0x80 bank_attacker 0x2  
	waitanimation
	playsound2 0x91 0xc0 
	launchtask 0x8099A79 0x2 0x4 0x10 0x80 bank_attacker 0x2  
	waitanimation
	playsound2 0x5F 0xc0 
	launchtemplate 0x83E7B24 0x2 0x5 0x2 0x3 0x7 0x0 0x67f1  
	launchtemplate 0x83E4094 0x3 0x4 0x0 0x0 0x0 0x0	 
	waitanimation 
	endanimation

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
ANIM_WRAPPED_END_TURN:
	launchtask AnimTask_GetTrappedMoveAnimId 0x5 0x0 
	jumpifargmatches 0x0 0x1 0x81d5cd3 	@Fire Spin
	jumpifargmatches 0x0 0x2 0x81d5cf8 	@Whirlpool
	jumpifargmatches 0x0 0x3 ANIM_CLAMP 	@Clamp
	jumpifargmatches 0x0 0x4 0x81d5da9 	@Sand Tomb
	jumpifargmatches 0x0 0x5 0x81d5cd3 	@Magma Storm
	jumpifargmatches 0x0 0x6 ANIM_INFESTATION
	goto 0x81d5c8b 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_WISHIWASHI_FISH:
	loadparticle ANIM_TAG_WISHIWASHI_FISH @Fish
	loadparticle ANIM_TAG_SMALL_BUBBLES @Bubbles
	soundcomplex 0x75 0x3F 0x3 0x10
	pokespritetoBG bank_attacker
	leftbankBG_over_partnerBG bank_attacker
	call WISHIWASHI_SWIRL_PARTICLES
	launchtask AnimTask_sprite_transform 0x2 0x1 0x0
	call WISHIWASHI_SWIRL_PARTICLES
	call WISHIWASHI_SWIRL_PARTICLES
	waitanimation
	launchtask RED_PRIMAL_TASK_2 0x1 0x2 0x0 0xff
	waitanimation
	pokespritefromBG bank_attacker
	endanimation

WISHIWASHI_SWIRL_PARTICLES:
	launchtemplate WISHIWASHI_FISH 0x82 0x7 0x0 0x1c 0x210 0x1e 0xd 0x32 0x1  
	pause 0x2 
	launchtemplate WISHIWASHI_BUBBLES 0x82 0x7 0x0 0x20 0x1e0 0x14 0x10 0xffd2 0x1  
	pause 0x2 
	launchtemplate WISHIWASHI_FISH 0x82 0x7 0x0 0x21 0x240 0x14 0x8 0x2a 0x1  
	pause 0x2 
	launchtemplate WISHIWASHI_BUBBLES 0x82 0x7 0x0 0x1f 0x190 0x19 0xb 0xffd6 0x1  
	pause 0x2 
	launchtemplate WISHIWASHI_FISH 0x82 0x7 0x0 0x1c 0x200 0x19 0x10 0x2e 0x1  
	pause 0x2 
	launchtemplate WISHIWASHI_BUBBLES 0x82 0x7 0x0 0x21 0x1d0 0x1e 0xf 0xffce 0x1  
	pause 0x2 
	return

.align 2
WISHIWASHI_FISH: objtemplate ANIM_TAG_WISHIWASHI_FISH ANIM_TAG_WISHIWASHI_FISH 0x83AC9D0 0x83E2C00 0x0 0x8231CFC 0x80B477D
WISHIWASHI_BUBBLES: objtemplate ANIM_TAG_SMALL_BUBBLES ANIM_TAG_SMALL_BUBBLES 0x83ACB50 0x83E5A78 0x0 0x83E6AB4 0x80B477D

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ZYGARDE_CELL_SWIRL:
	loadparticle ANIM_TAG_ZYGARDE_HEXES 
	loadparticle ANIM_TAG_BUBBLE 
	loadparticle ANIM_TAG_IMPACT 
	loadparticle ANIM_TAG_ROCKS 
	pokespritetoBG bank_target 
	leftbankBG_over_partnerBG bank_target 
	launchtemplate Template_Pal_Fade 0x2 0x5 0x1 0x0 0x0 0xf 0xc0 
	pause 0xa 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x78 0x46 0x5 0x46 0x1e 
	pause 0x1 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x73 0x37 0x6 0x3c 0x19 
	pause 0x1 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x73 0x3c 0x7 0x3c 0x1e 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x73 0x37 0xa 0x3c 0x1e 
	pause 0x3 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x64 0x32 0x4 0x32 0x1a 
	pause 0x1 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x69 0x19 0x8 0x3c 0x14 
	pause 0x1 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x73 0x28 0xa 0x30 0x1e 
	pause 0x3 
	playsound2 0xe3 0x0 
	launchtask AnimTask_sprite_transform 0x2 0x1 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x78 0x1e 0x6 0x2d 0x19 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x73 0x23 0xa 0x3c 0x1e 
	pause 0x3 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x69 0x14 0x8 0x28 0x0 
	pause 0x3 
	playsound2 0xe3 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x14 0xff 0xf 0x20 0x0 
	launchtemplate ZYGARDE_CELL 0x82 0x5 0x6e 0xa 0x8 0x20 0x14 
	waitanimation 
	launchtemplate Template_Pal_Fade 0x2 0x5 0x1 0x1 0xf 0x0 0xc0 
	waitanimation 
	pokespritefromBG bank_target 
	launchtask RED_PRIMAL_TASK_2 0x1 0x2 0x0 0xff 
	waitanimation 
	resetblends 
	endanimation 

.align 2
ZYGARDE_CELL: objtemplate ANIM_TAG_ZYGARDE_HEXES ANIM_TAG_ZYGARDE_HEXES 0x83AC9D0 0x83E2C00 0x0 0x8231CFC 0x80A2D11 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_DELTA_STREAM:
	loadparticle ANIM_TAG_FLYING_DIRT 
	launchtask AnimTask_pal_fade_particle 0x5 0x5 ANIM_TAG_FLYING_DIRT 0x0 0x6 0x6 0x7fff 
	launchtask 0x80b4811 0x5 0x1 0x1 
	launchtask 0x80adaa5 0x6 0x2 0x6 0x7fff 
	playsound2 0x7d 0x3f 
	pause 0x44 
	playsound2 0x7e 0x3f 
	pause 0x38 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ELECTRIC_SURGE:
	loadparticle ANIM_TAG_ELECTRIC_ORBS 
	playsound2 0xce 0xc0 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0x0 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0x2a 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0x54 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0x7e 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0xa8 
	launchtemplate ELECTRIC_SURGE_1 0x2 0x2 0x1a 0xd2 
	pause 0x34 
	setarg 0x7 0xffff 
	playsound2 0xdb 0xc0 
	launchtask 0x80998b1 0x5 0x5 0xfff9 0xfff9 0xb 0x0 0x0 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0x0 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0x20 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0x40 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0x60 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0x80 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0xa0 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0xc0 
	launchtemplate ELECTRIC_SURGE_2 0x82 0x1 0xe0 
	waitanimation
ELECTRIC_TERRAIN_ACTIVE_ANIM:
	loaddefaultBG @At this point the new BG is in effect
	waitfortransparentBG 
	playsound2 0x70 0x3f 
	pause 0x40 
	playsound2 0x70 0x3f 
	pause 0x40
	endanimation 

.align 2
ELECTRIC_SURGE_1: objtemplate ANIM_TAG_ELECTRIC_ORBS ANIM_TAG_ELECTRIC_ORBS 0x83AC9C8 0x83E6118 0x0 0x8231CFC 0x80AA175 
ELECTRIC_SURGE_2: objtemplate ANIM_TAG_ELECTRIC_ORBS ANIM_TAG_ELECTRIC_ORBS 0x83AC9C8 0x83E6118 0x0 0x8231CFC 0x80AA2B1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_GRASSY_SURGE:
	loadparticle ANIM_TAG_ORBS 
	loadparticle ANIM_TAG_GREEN_SPARKLE 
	playsound2 0x91 0xc0 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0x0 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0x2a 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0x54 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0x7e 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0xa8 
	launchtemplate GRASSY_TERRAIN_2 0x2 0x2 0x1a 0xd2 
	pause 0x34 
	setarg 0x7 0xffff 
	playsound2 0xbc 0xc0 
	launchtask 0x80998b1 0x5 0x5 0xfff9 0xfff9 0xb 0x0 0x0 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0x0 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0x20 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0x40 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0x60 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0x80 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0xa0 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0xc0 
	launchtemplate GRASSY_TERRAIN_1 0x82 0x1 0xe0 
	waitanimation 
GRASSY_TERRAIN_ACTIVE_ANIM:
	loaddefaultBG @At this point the new BG is in effect
	waitfortransparentBG
	playsoundpanchange2 0xc3 0xc0 0x3f 0x1 0x0 
	waitforsound
	endanimation 

.align 2
GRASSY_TERRAIN_1: objtemplate ANIM_TAG_GREEN_SPARKLE ANIM_TAG_GREEN_SPARKLE 0x83AC9D0 0x83E3760 0x0 0x8231CFC 0x80AA2B1 
GRASSY_TERRAIN_2: objtemplate ANIM_TAG_ORBS ANIM_TAG_ORBS 0x83ACB50 0x83E2A40 0x0 0x83E2A54 0x80AA175 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_MISTY_SURGE:
	loadparticle ANIM_TAG_ORBS 
	loadparticle ANIM_TAG_GREEN_SPARKLE 
	loadparticle ANIM_TAG_WATER_GUN 
	playsound2 0xbc 0xc0 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0x0 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0x2a 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0x54 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0x7e 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0xa8 
	launchtemplate MISTY_TERRAIN_2 0x2 0x2 0x1a 0xd2 
	pause 0x34 
	setarg 0x7 0xffff 
	playsound2 0xdb 0xc0 
	launchtask 0x80998b1 0x5 0x5 0xfff9 0xfff9 0xb 0x0 0x0 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0x0 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0x20 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0x40 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0x60 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0x80 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0xa0 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0xc0 
	launchtemplate MISTY_TERRAIN_1 0x82 0x1 0xe0 
	waitanimation 
MISTY_TERRAIN_ACTIVE_ANIM:
	loaddefaultBG @At this point the new BG is in effect
	waitfortransparentBG
	playsound2 0xec 0xc0 
	pause 0x40 
	pause 0x40 
	endanimation 

.align 2
MISTY_TERRAIN_1: objtemplate ANIM_TAG_GREEN_SPARKLE ANIM_TAG_WATER_GUN 0x83AC9D0 0x83E3760 0x0 0x8231CFC 0x80AA2B1 
MISTY_TERRAIN_2: objtemplate ANIM_TAG_ORBS ANIM_TAG_WATER_GUN 0x83ACB50 0x83E2A40 0x0 0x83E2A54 0x80AA175 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_PSYCHIC_SURGE:
	loadparticle ANIM_TAG_ORBS 
	loadparticle ANIM_TAG_GREEN_SPARKLE 
	loadparticle ANIM_TAG_POISON_BUBBLE 
	playsound2 0xbc 0xc0 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0x0 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0x2a 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0x54 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0x7e 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0xa8 
	launchtemplate PSYCHIC_TERRAIN_2 0x2 0x2 0x1a 0xd2 
	pause 0x34 
	setarg 0x7 0xffff 
	playsound2 0xdb 0xc0 
	launchtask 0x80998b1 0x5 0x5 0xfff9 0xfff9 0xb 0x0 0x0 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0x0 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0x20 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0x40 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0x60 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0x80 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0xa0 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0xc0 
	launchtemplate PSYCHIC_TERRAIN_1 0x82 0x1 0xe0 
	waitanimation
PSYCHIC_TERRAIN_ACTIVE_ANIM:
	loaddefaultBG @At this point the new BG is in effect
	waitfortransparentBG
	soundcomplex 0xb1 0x3f 0xa 0x3 
	waitforsound
	endanimation 

.align 2
PSYCHIC_TERRAIN_1: objtemplate ANIM_TAG_GREEN_SPARKLE ANIM_TAG_POISON_BUBBLE 0x83AC9D0 0x83E3760 0x0 0x8231CFC 0x80AA2B1
PSYCHIC_TERRAIN_2: objtemplate ANIM_TAG_ORBS ANIM_TAG_WATER_GUN 0x83ACB50 0x83E2A40 0x0 0x83E2A54 0x80AA175 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_SEA_OF_FIRE:
	loadparticle ANIM_TAG_SMALL_EMBER
	launchtemplate 0x83E7B24 0x2 0x5 0x1 0x3 0x0 0x7 0x043D
	pause 0x20
	pokespritetoBG bank_target
	setblends 0x80c
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xF0 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xC0 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0x90 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0x60 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0x30 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0x0 0x10 0x28
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xffD0 0x10 0x28  
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xffA0 0x10 0x28  
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xff70 0x10 0x28  
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xff40 0x10 0x28  
	launchtemplate 0x83E5BF8 0x81 0x5 0x0 0xa 0xff10 0x10 0x28 
	pause 0x4
	playsound2 0x8C 0x3f 
	waitanimation
	pokespritefromBG bank_target 
	launchtemplate 0x83E7B24 0x2 0x5 0x1 0x3 0x7 0x0 0x043D
	waitanimation
	resetblends
	endanimation

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_HEALING_SPARKLES:
	loadparticle ANIM_TAG_BLUE_STAR 
	call HEALING_ANIM 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_LUNAR_DANCE_HEAL:
	loadparticle ANIM_TAG_SPARKLE_2 
	loadparticle ANIM_TAG_GUARD_RING 
	loadparticle ANIM_TAG_SMALL_EMBER 
	pokespritetoBG side_attacker 
	setblends 0x808 
	soundcomplex 0xbc 0xc0 0x10 0x3 
	call HEALING_STARS 
	launchtemplate LUNARDANCE_RING 0x2 0x0 
	pause 0x4 
	launchtemplate LUNARDANCE_RING 0x2 0x0 
	pause 0x4 
	launchtemplate LUNARDANCE_RING 0x2 0x0 
	pause 0x40 
	waitanimation 
	pokespritefromBG side_attacker 
	resetblends 
	endanimation 

.align 2
LUNARDANCE_RING: objtemplate ANIM_TAG_GUARD_RING ANIM_TAG_SMALL_EMBER 0x83ACBE0 0x8231CF0 0x0 0x83E44D4 0x80AAAE5 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_HEALING_WISH_HEAL:
	loadparticle ANIM_TAG_GOLD_STARS 
	launchtemplate Template_Pal_Fade 0x2 0x5 0x1 0x0 0x0 0x10 0x0 
	waitanimation 
	playsoundpanchange3 0xc8 0x3f 0xc0 0xfd 0x0 
	launchtemplate 0x83ff168 0x28 0x0 
	waitanimation 
	launchtemplate Template_Pal_Fade 0x2 0x5 0x1 0x0 0x10 0x0 0x0 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_RED_PRIMAL_REVERSION:
	loadparticle ANIM_TAG_OMEGA_STONE 
	pokespritefromBG side_attacker 
	resetblends 
	pause 0x0 
	waitfortransparentBG 
	pokespritetoBG bank_attacker 
	setblends 0x80c 
	playsound2 0xc2 0xc0 
	launchtemplate RED_PRIMAL_OMEGA 0x29 0x4 0x0 0x0 0x0 0x0 
	pause 0x14 
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x0 0x10 0x7fff 
	launchtask AnimTask_pal_fade_particle 0x5 0x5 ANIM_TAG_OMEGA_STONE 0x2 0x0 0x10 0x7fff 
	goto ANIM_PRIMAL_REVERSION_JOIN

.align 2
RED_PRIMAL_OMEGA: objtemplate ANIM_TAG_OMEGA_STONE ANIM_TAG_OMEGA_STONE 0x83ACBC0 0x8231CF0 0x0 0x83E7144 0x8075D9D 
RED_PRIMAL_ALPHA: objtemplate ANIM_TAG_ALPHA_STONE ANIM_TAG_ALPHA_STONE 0x83ACBC0 0x8231CF0 0x0 0x83E7144 0x8075D9D 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BLUE_PRIMAL_REVERSION:
	loadparticle ANIM_TAG_ALPHA_STONE 
	pokespritefromBG side_attacker 
	pause 0x0 
	waitfortransparentBG 
	pokespritetoBG bank_attacker 
	setblends 0x80c 
	playsound2 0xc2 0xc0 
	launchtemplate BLUE_PRIMAL_ALPHA 0x29 0x4 0x0 0x0 0x0 0x0 
	pause 0x14 
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x0 0x10 0x7fff 
	launchtask AnimTask_pal_fade_particle 0x5 0x5 ANIM_TAG_ALPHA_STONE 0x2 0x0 0x10 0x7fff 
	
ANIM_PRIMAL_REVERSION_JOIN:
	waitanimation 
	launchtask AnimTask_pal_fade_particle 0x5 0x0 
	launchtask RED_PRIMAL_TASK 0x2 0x0 
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x10 0x0 0x7fff 
	waitanimation 
	launchtask AnimTask_screen_shake 0x5 0x3 0x1 0x5 0xe 
	launchtask RED_PRIMAL_TASK_2 0x1 0x2 0x0 0xff 
	waitanimation 
	pokespritefromBG side_attacker 
	pokespritefromBG 0xd 
	endanimation 

.align 2
BLUE_PRIMAL_ALPHA: objtemplate ANIM_TAG_ALPHA_STONE ANIM_TAG_ALPHA_STONE 0x83ACBC0 0x8231CF0 0x0 0x83E7144 0x8075D9D 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_GRASSY_TERRAIN_HEAL:
	loadparticle ANIM_TAG_ORBS 
	loadparticle ANIM_TAG_BLUE_STAR 
	pokespritetoBG side_target 
	setblends 0x80c 
	waitanimation 
	pause 0x3 
	call 0x81cec44 
	waitanimation 
	pause 0xf 
	call HEALING_ANIM 
	waitanimation 
	launchtemplate Template_Pal_Fade 0x2 0x5 0x1 0x1 0x4 0x0 0x33ed 
	waitanimation 
	pokespritefromBG side_target 
	resetblends
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_POWDER_EXPLOSION:
	loadparticle ANIM_TAG_EXPLOSION 
	launchtask AnimTask_move_bank 0x2 0x5 0x0 0x0 0x5 0x20 0x1 
	call 0x81c8eb2 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BEAK_BLAST_WARM_UP:
	loadparticle ANIM_TAG_SMALL_EMBER 
	playsound2 0xa4 0xc0 
	pause 0x3 
	launchtask AnimTask_pal_fade_complex 0x2 0x6 0x2 0x2 0x2 0x0 0xb 0x1f 
	launchtemplate 0x83e5dfc 0x3 0x4 0x0 0x0 0x38 0x0 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_SHELL_TRAP_SET:
	loadparticle ANIM_TAG_SMALL_EMBER 
	loadparticle ANIM_TAG_IMPACT 
	pokespritetoBG bank_target 
	setblends 0x80c 
	call BURN_CHANCE_ANIM 
	pause 0x4 
	playsound2 0x8c 0x3f 
	pause 0x15 
	launchtemplate Template_Hit 0x2 0x4 0x0 0x0 0x1 0x2 
	launchtemplate Template_Hit 0x2 0x4 0x0 0xa 0x1 0x2 
	launchtemplate Template_Hit 0x2 0x4 0xa 0x0 0x1 0x2 
	launchtemplate Template_Hit 0x2 0x4 0xa 0xa 0x1 0x2 
	waitanimation 
	pokespritefromBG bank_target 
	resetblends 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_BERRY_EAT:
	loadparticle ANIM_TAG_BERRY_NORMAL
	loadparticle ANIM_TAG_SHARP_TEETH
	loadparticle ANIM_TAG_THIN_RING 
	loadparticle ANIM_TAG_SPARKLE_2
	playsound2 0xb3 0xc0
	launchtemplate BERRYFLOAT 0xc 0x1 0x0
	pause 0x45
	launchtask AnimTask_arg7_is_target_player 0x2 0x0
	jumpifargmatches 0x7 0x1 BERRYEAT_ON_PLAYER
BERRYEAT_ON_OPPONENT:
	call BITE_FOE
	pause 0x10
	call BITE_FOE
	pause 0xA
	goto POST_BERRY_EAT
BERRYEAT_ON_PLAYER:
	call BITE_PLAYER
	pause 0x10
	call BITE_PLAYER
	pause 0xA
POST_BERRY_EAT:
	playsound2 0x5F 0xc0 
	launchtemplate 0x83E7B24 0x2 0x5 0x2 0x3 0x7 0x0 0x67f1  
	launchtemplate 0x83E4094 0x3 0x4 0x0 0x0 0x0 0x0	 
	waitanimation 
	endanimation
	
BITE_PLAYER:
	playsound2 0x9a 0x3f 
	launchtemplate 0x83E7930 0x2 0x6 0x27 0xffD0 0x0 0x0 0x333 0xa  
	launchtemplate 0x83E7930 0x2 0x6 0x27 0x10 0x4 0x0 0xfccd 0xa
	return

BITE_FOE:
	playsound2 0x9a 0x3f 
	launchtemplate 0x83E7930 0x2 0x6 0xffDF 0xffD0 0x0 0x0 0x333 0xa  
	launchtemplate 0x83E7930 0x2 0x6 0xffDF 0x10 0x4 0x0 0xfccd 0xa
	return

.align 2
BERRYFLOAT: objtemplate ANIM_TAG_BERRY_NORMAL ANIM_TAG_BERRY_NORMAL 0x83ACA98 0x8231CF0 0x0 0x83E3A2C 0x80A6C09

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_FOG_CONTINUES:
	waitanimation 
	playsound2 0xef 0x0 
	launchtask AnimTask_create_haze 0x5 0x0 
	pause 0x1e 
	launchtask AnimTask_pal_fade 0xa 0x5 0x780 0x2 0x0 0xc 0xff7f 
	pause 0x5a 
	launchtask AnimTask_pal_fade 0xa 0x5 0x780 0x1 0xc 0x0 0xff7f 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_AQUA_RING_HEAL:
	loadparticle ANIM_TAG_GUARD_RING 
	loadparticle ANIM_TAG_WATER_ORB 
	loadparticle ANIM_TAG_SMALL_BUBBLES 
	loadparticle ANIM_TAG_BLUE_STAR 
	pokespritetoBG bank_attacker 
	setblends 0x808 
	playsound2 0xda 0xc0 
	launchtask AnimTask_pal_fade_complex 0x2 0x6 0x2 0x0 0x2 0x0 0xa 0x7f6e 
	launchtemplate AQUARING_RING 0x2 0x0 
	launchtemplate AQUA_RING_BUBBLES 0x2 0x4 0x0 0xfff6 0x19 0x0 
	pause 0x4 
	launchtemplate AQUARING_RING 0x2 0x0 
	launchtemplate AQUA_RING_BUBBLES 0x2 0x4 0xfff1 0x0 0x19 0x0 
	pause 0x4 
	launchtemplate AQUARING_RING 0x2 0x0 
	launchtemplate AQUA_RING_BUBBLES 0x2 0x4 0x14 0xa 0x19 0x0 
	waitanimation 
	call HEALING_ANIM 
	pokespritefromBG bank_attacker 
	resetblends 
	endanimation 
	
.align 2
AQUARING_RING: objtemplate ANIM_TAG_GUARD_RING ANIM_TAG_WATER_ORB 0x83ACBE0 0x8231CF0 0x0 0x83E44D4 0x80AAAE5 
AQUA_RING_BUBBLES: objtemplate ANIM_TAG_SMALL_BUBBLES ANIM_TAG_SMALL_BUBBLES 0x83ACB50 0x83E5A78 0x0 0x8231CFC 0x80AB309 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
BATON_PASS_ANIM:
	loadparticle ANIM_TAG_POKEBALL 
	playsound2 0xd9 0xc0 
	launchtask AnimTask_pal_fade_complex 0x2 0x6 0x1f 0x1 0x2 0x0 0xb 0x7adf 
	launchtemplate 0x83ff150 0x2 0x0 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
DRAGON_TAIL_BLOW_AWAY_ANIM:
	playsound2 0x7a 0x3f 
	launchtask 0x80995fd 0x5 0x2 0x1 0x8 
	waitanimation 
	endanimation 

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ZMOVE_ACTIVATE:
	loadparticle ANIM_TAG_FOCUS_ENERGY @focus energy
	loadparticle ANIM_TAG_Z_MOVE_SYMBOL @Z-Move Symbol
	loadparticle ANIM_TAG_WHIP_HIT @green color
	loadparticle ANIM_TAG_SWEAT_BEAD @blue color
	loadparticle ANIM_TAG_PAW_PRINT @yellow color
	pokespritetoBG bank_attacker
	setblends 0x80c
	loadBG1 BG_ZMOVE_ACTIVATE
	waitfortransparentbg
	launchtask 0x80BB82D 0x5 0x4 0x0 0x0 0x0 0xFFFF

	playsound2 0xC2 0xc0
	launchtask AnimTask_pal_fade_complex 0x2 0x6 PAL_ATK 0x0 0x6 0x0 0xb 0x76BC
	call RAINBOW_BUFF
	call RAINBOW_BUFF
	call RAINBOW_BUFF
	launchtemplate ZSYMBOL 0x29 0x4 0x0 0x0 0x0 0x0
	call RAINBOW_BUFF
	call RAINBOW_BUFF
	waitanimation
	call UNSET_SCROLLING_BG

	resetblends
	pokespritefrombg bank_attacker
	endanimation

RAINBOW_BUFF:
	launchtemplate BLUEBUFF 0x2 0x4 0x0 0xffe8 0x1a 0x2 
	pause 0x3
	launchtemplate 0x83E3604 0x2 0x4 0x0 0xe 0x1c 0x1 @Red Buff
	pause 0x3 
	launchtemplate GREENBUFF 0x2 0x4 0x0 0xfffb 0xa 0x2 
	pause 0x3 
	launchtemplate YELLOWBUFF 0x2 0x4 0x0 0x1c 0x1a 0x3 
	pause 0x3
	return

.align 2
BLUEBUFF: objtemplate ANIM_TAG_FOCUS_ENERGY ANIM_TAG_SWEAT_BEAD 0x83ACA18 0x83E3600 0x0 0x8231CFC 0x80A5AD9
GREENBUFF: objtemplate ANIM_TAG_FOCUS_ENERGY ANIM_TAG_WHIP_HIT 0x83ACA18 0x83E3600 0x0 0x8231CFC 0x80A5AD9
YELLOWBUFF: objtemplate ANIM_TAG_FOCUS_ENERGY ANIM_TAG_PAW_PRINT 0x83ACA18 0x83E3600 0x0 0x8231CFC 0x80A5AD9
ZSYMBOL: objtemplate ANIM_TAG_Z_MOVE_SYMBOL ANIM_TAG_Z_MOVE_SYMBOL 0x83ACBC0 0x8231CF0 0x0 0x83E7144 0x8075D9D

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_MEGA_EVOLUTION:
	loadparticle ANIM_TAG_MEGA_RAINBOW
	loadparticle ANIM_TAG_MEGA_STONE
	pokespritetoBG side_attacker 
	setblends 0x80c 
	soundcomplex 0x85 0xc0 0xd 0x3 
	launchtask AnimTask_pal_fade_complex 0x2 0x6 0x2 0x0 0x6 0x0 0xb 0x2fff 
	call BRING_IN_RAINBOWS_BALLS
	call BRING_IN_RAINBOWS_BALLS
	call BRING_IN_RAINBOWS_BALLS
	waitanimation 
	pokespritefromBG side_attacker 
	resetblends 
	pause 0x0 
	waitfortransparentBG 
	pokespritetoBG bank_attacker 
	setblends 0x80c 
	playsound2 0xc2 0xc0 
	launchtemplate MEGA_STONE 0x29 0x4 0x0 0x0 0x0 0x0 
	pause 0x14 
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x0 0x10 0xffff 
	launchtask AnimTask_pal_fade_particle 0x5 0x5 ANIM_TAG_MEGA_STONE 0x2 0x0 0x10 0xffff 
	waitanimation
	unloadparticle ANIM_TAG_MEGA_RAINBOW
	unloadparticle ANIM_TAG_MEGA_STONE
	loadparticle ANIM_TAG_MEGA_SYMBOL
	launchtask RED_PRIMAL_TASK 0x2 0x0 
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x10 0x0 0xffff 
	waitanimation 
	launchtask AnimTask_screen_shake 0x5 0x3 0x1 0x5 0xe 
	launchtask RED_PRIMAL_TASK_2 0x1 0x2 0x0 0xff 
	waitanimation 
	pokespritefromBG side_attacker 
	launchtemplate MEGA_SYMBOL 0x82 0x0 
	waitanimation 
	pokespritefromBG 0xd 
	endanimation

BRING_IN_RAINBOWS_BALLS:
	launchtemplate RAINBOW_BALL 0x2 0x3 0x28 0xfff6 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0xffdd 0xfff6 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0xf 0xffd8 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0xfff6 0xffe0 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0x19 0xffec 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0xffd8 0xffec 0xd 
	pause 0x3 
	launchtemplate RAINBOW_BALL 0x2 0x3 0x5 0xffd8 0xd 
	pause 0x3 
	return

.align 2
RAINBOW_BALL: objtemplate ANIM_TAG_MEGA_RAINBOW ANIM_TAG_MEGA_RAINBOW 0x83ACB50 0x83E2A40 0x0 0x83E2A54 0x80A2389 
MEGA_STONE: objtemplate ANIM_TAG_MEGA_STONE ANIM_TAG_MEGA_STONE 0x83ACBC0 0x8231CF0 0x0 0x83E7144 0x8075D9D
MEGA_SYMBOL: objtemplate ANIM_TAG_MEGA_SYMBOL ANIM_TAG_MEGA_SYMBOL 0x83ACAF8 0x8231CF0 0x0 0x8231CFC 0x80B67D5

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_ULTRA_BURST:
	loadparticle ANIM_TAG_ULTRA_BURST_SYMBOL
	loadparticle ANIM_TAG_WHITE_CIRCLE_OF_LIGHT
	loadparticle ANIM_TAG_ORBS @Recover
	pokespritetoBG side_attacker
	setblends 0x80c

@Recover:
	soundcomplex 0x85 0xC0 0xD 0x3
	launchtask AnimTask_pal_fade_complex 0x2 0x6 PAL_ATK 0x0 0x6 0x0 0xb 0x7fff
	call RECOVER_LOAD_PARTICLES 
	call RECOVER_LOAD_PARTICLES 
	call RECOVER_LOAD_PARTICLES
	waitanimation	

@CircleThing:
	playsound2 0xc2 0xc0 
	launchtemplate 0x83e7148 0x29 0x4 0x0 0x0 0x0 0x0 
	pause 0x14 

@WhiteOutScreen:
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x0 0x10 0xffff 
	launchtask AnimTask_pal_fade_particle 0x5 0x5 ANIM_TAG_WHITE_CIRCLE_OF_LIGHT 0x2 0x0 0x10 0xffff 
	waitanimation
	launchtemplate ULTRABURSTSYMBOL 0x82 0x0
	waitanimation
	launchtask AnimTask_pal_fade_particle 0x5 0x0
	launchtask RED_PRIMAL_TASK 0x2 0x0
	launchtask 0x80ba83d 0x5 0x5 0x5 0x2 0x10 0x0 0xffff 
	waitanimation
	launchtask AnimTask_screen_shake 0x5 0x3 0x0 0x5 0xe
	launchtask RED_PRIMAL_TASK_2 0x1 0x2 0x0 0xFF
	waitanimation

End:
	resetblends
	pokespritefrombg side_attacker
	endanimation

.align 2
ULTRABURSTSYMBOL: objtemplate ANIM_TAG_ULTRA_BURST_SYMBOL ANIM_TAG_ULTRA_BURST_SYMBOL 0x83ACAF8 0x8231CF0 0x0 0x231CFC 0x80B67D5

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_LOAD_DEFAULT_BG:
	loaddefaultBG
	waitfortransparentBG
	endanimation
	
@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_LOAD_ABILITY_POP_UP:
	@loadparticle ANIM_TAG_ABILITY_POP_UP @;Done by the task b\c not compressed
	pokespritetoBG side_attacker
	launchtask AnimTask_LoadAbilityPopUp 0x5 0x0
	waitanimation
	pause 0x20
	pokespritefromBG side_attacker
	endanimation

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
ANIM_REMOVE_ABILITY_POP_UP:
	launchtask AnimTask_DestroyAbilityPopUp 0x5 0x0
	pause 0x20
	endanimation
