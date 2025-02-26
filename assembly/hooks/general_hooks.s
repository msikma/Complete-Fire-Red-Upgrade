.text
.thumb
.align 2

.include "../defines"

.global BuildTrainerPartyHook

BuildTrainerPartyHook:
	bl BuildTrainerPartySetup
	ldr r0, =0x800FF98 | 1
	bx r0

@0x800D2CC with r0
AIHandleItemUseHook:
	bl HandleNewBattleRamClearBeforeBattle
	bl SavePartyItems
	mov r0, #0xF
	bl BattleAI_HandleItemUseBeforeAISetup
	ldr r0, =0x2022B54
	str r4, [r0]
	ldr r0, =0x800D2D4 | 1
	bx r0
	
EmitChooseMoveHook:
	mov r3, r5
	bl EmitChooseMove
	pop {r4-r6,pc}

@0x802EF90 with r0
SwitchMoveDataHook:
	bl HandleMoveSwitchingUpdate
	ldr r0, =0x802F2C8 | 1
	bx r0

@0x80992E4 with r0
SlideMonToOffsetHook:
	mov r0, #0x0
	bl LoadBattleAnimTarget
	mov r2, r0
	ldr r0, =0x8099300 | 1
	bx r0

@0x80991BA with r0
SlideMonToOriginalPosHook:
	mov r0, #0x0
	bl LoadBattleAnimTarget
	ldr r1, =0x2023D44 @BattlerSpriteIds
	ldr r2, =0x80991DE | 1
	bx r2

@0x0804AA1E with r1
GetProperBallIdHook1:
	bl BallIdToItemId
	bl ItemIdToBallId
	lsl r0, #0x18
	lsr r0, #0x18
	mov r5, r0
	ldr r1, =0x804AA28 | 1
	bx r1

@0x080EF3A8 with r1
GetProperBallIdHook2:
	bl BallIdToItemId
	bl ItemIdToBallId
	lsl r0, #0x18
	lsr r0, #0x18
	ldr r1, =0x80EF3B0 | 1
	bx r1

@Summary Screen Poke Ball Hook
@0x08139CDA with r1
GetProperBallIdHook3:
	bl BallIdToItemId
	bl ItemIdToBallId
	mov r4, r0
	lsl r4, #0x18
	lsr r4, #0x18
	ldr r1, =0x8139CE4 | 1
	bx r1

@0x80F1792 with r0
ShinyAnimFixHook:
	bl ShinyAnimFix
	ldr r0, =0x80F179E | 1
	bx r0

@0x8014450 with r0
PartyMenuSwitchingHook:
	bl PartyMenuSwitchingUpdate
	ldr r0, =0x80146AC | 1
	bx r0

@0x80EFF0C with r2
CriticalCaptureAnimHook:
	push {r0-r1}
	bl CriticalCapturAnimUpdate
	cmp r0, #0x0
	pop {r0-r1}
	bne BallThrowSuccessReturn
	cmp r0, #0x4
	bne BallThrowBreakOutReturn
	cmp r1, #0x3
	bne BallThrowBreakOutReturn

BallThrowSuccessReturn:
	ldr r0, =0x80EFF14 | 1
	bx r0

BallThrowBreakOutReturn:
	ldr r0, =0x80EFF20 | 1
	bx r0

@0x80EF5F4 with r1
DoubleWildAnimBallThrowFixHook:
	strh r0, [r6, #0x2E]
	bl DoubleWildAnimBallThrowFix
	ldr r2, =ANIM_TARGET
	mov r8, r2
	ldrb r0, [r2]
	ldr r1, =0x80EF5FC | 1
	bx r1

@0x80A1E2C with r0
DoubleWildPokeBallItemUseFixHook:
	mov r0, r4
	bl DoubleWildPokeBallItemUseFix
	cmp r0, #0x0
	bne DoubleWildPokeBallItemUseFixEnd
	
	ldr r0, =0x203AD30 @Var 0x800E
	ldrh r0, [r0]
	mov r1, #0x1
	bl RemoveBagItem
	ldr r0, =0x80A1E36 | 1
	bx r0

DoubleWildPokeBallItemUseFixEnd:
	pop {r4, pc}

RemoveBagItem:
	ldr r3, =0x809A1D8 | 1
bxr3:
	bx r3

@0x802D95C with r0
DoubleWildDexHook1:
	push {r4-r5,lr}
	bl LoadTargetPartyData
	mov r4, r0
	mov r1, #0xB
	ldr r2, =0x802D964 | 1
	bx r2

@0x802D9D8 with r0
DoubleWildDexHook2:
	push {r4-r5,lr}
	sub sp, #0x18
	bl LoadTargetPartyData
	mov r1, #0xB
	ldr r2, =0x802D9E0 | 1
	bx r2

@0x8013D14 with r1
TrainerSlidingEndTurnHook:
	mov r1, r10
	str r0, [r1]
	bl CheckLastMonLowHPSlide
	bl Random
	ldr r1, =0x8013D1C | 1
	bx r1

Random:
	ldr r0, =0x8044EC8 | 1
	bx r0
	
@0x80813B8 with r0
LoadProperMusicForLinkBattlesHook:
	bl LoadProperMusicForLinkBattles
	ldr r1, =0x80813C6 | 1
	bx r1
	
@0x806E454 with r1
ExpandedVarsHook:
	push {r4-r6, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedVarPointer
	cmp r0, #0x1
	beq VarReturn0
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E45C | 1
	bx r0
	
ExpandedVarsPop:
	pop {r4-r6,pc}
	
VarReturn0:
	ldr r0, =0x806E462 | 1
	bx r0

.pool
@0x806E5C0 with r1
ExpandedFlagsHook:
	push {r4-r6, lr}
	lsl r5, r0, #0x10
	lsr r4, r5, #0x10
	mov r6, r4
	mov r0, r4
	bl GetExpandedFlagPointer
	cmp r0, #0x0
	bne ExpandedVarsPop
	ldr r0, =0x806E5C8 | 1
	bx r0

.pool
@0x80142D8 with r0
DoubleWildUseItemRunFixHook:
	ldr r5, =ACTIVE_BATTLER
	ldrb r4, [r5]
	bl CheckCantMoveThisTurn
	cmp r0, #0x0
	beq SkipEncoreReturn
	ldr r0, =0x801460E | 1
	bx r0

SkipEncoreReturn:
	ldr r0, =0x801432C | 1
	bx r0

.pool
@0x8054B44 with r0
NewGameSaveClearHook:
	bl NewGameWipeNewSaveData
	pop {r3}
	mov r8, r3
	pop {r4-r6}
	pop {r0}
	bx r0

.pool
@0x80CEDD4 with r1
EvolutionMovesHook:
	ldrb r1, [r5, #0x10]
	mov r0, r9
	bl MonTryLearningNewMoveAfterEvolution
	ldr r1, =0x80CEDDC | 1
	bx r1

.pool
@0x81058DC with r0
PokedexLoadAlternateHeightHook:
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x0 @PKDX_GET_HEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexHeight
	mov r0, r5
	ldr r1, =0x81058E6 | 1
	bx r1

ReturnPokedexHeight:
	mov r4, r0 @Store height in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x81058F6 | 1
	bx r1

.pool
@0x8105A58 with r0
PokedexLoadAlternateWeightHook:
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r5, r1 @Save species
	mov r0, r1
	mov r1, #0x1 @PKDX_GET_WEIGHT
	bl TryGetAlternateSpeciesSize
	cmp r0, #0x0
	bne ReturnPokedexWeight
	mov r0, r5
	ldr r1, =0x8105A60 | 1
	bx r1

ReturnPokedexWeight:
	mov r4, r0 @Store weight in r4
	mov r0, r5 @Store species in r0
	ldr r1, =SpeciesToNationalPokedexNum
	bl bxr1
	ldr r1, =0x8105A70 | 1
bxr1:
	bx r1

.pool
@0x80146AC with r0
ForfeitBattleTowerHook:
	bl HandleRunActionFrontier
	cmp r0, #0x0
	bne ReturnPrintForfeitBattleTower
	ldr r0, =BATTLE_TYPE
	ldr r0, [r0]
	mov r1, #BATTLE_TRAINER | BATTLE_WIRELESS
	and r0, r1
	ldr r1, =0x80146B4 | 1
	bx r1

ReturnPrintForfeitBattleTower:
	ldr r0, =0x8014C4E | 1
	bx r0

.pool
@0x811F3C8 with r2
LoadMaxNumPokemonDisplaySelection:
	and r0, r1
	cmp r0, #0x80
	bne LoadMaxNumPokemonDisplaySelectionMax
	mov r3, #0x2
	b LoadMaxNumPokemonDisplaySelectionReturn

LoadMaxNumPokemonDisplaySelectionMax:
	mov r1, #0x0
	bl ChoosePokemon_LoadMaxPKMNStr
	mov r3, r0

LoadMaxNumPokemonDisplaySelectionReturn:
	ldr r0, =0x811F3D2 | 1
	bx r0

.pool
@0x8124184 with r0
LoadMaxNumPokemonChooseBattleTowerStringHook:
	mov r0, sp
	mov r1, #0x1
	bl ChoosePokemon_LoadMaxPKMNStr
	mov r8, r0
	ldr r0, =0x812418C | 1
	bx r0

.pool
@0x805BA30 with r0
AutoRunHook:
	mov r0, #0x2 @B-Button
	and r5, r0
	cmp r5, #0x0
	bne CheckIfRunningAllowed
	bl IsAutoRunEnabled
	cmp r0, #0x0
	beq NoRunning

CheckIfRunningAllowed:
	ldr r2, .EventObjects
	ldrb r1, [r6, #0x5] @NPC Id
	lsl r0, r1, #0x3
	add r0, r1
	lsl r0, #0x2
	add r0, r2
	ldrb r0, [r0, #0x1E]
	bl IsRunningDisallowed
	cmp r0, #0x0
	bne NoRunning
	ldr r0, =0x805BA5A | 1
	bx r0

NoRunning:
	ldr r0, =0x805BA8C | 1
	bx r0

.align 2
.EventObjects: .word 0x2036E38

.pool
@0x8046390 with r1
FlameBodyMagmaArmorEggHook:
	mov r1, r4
	bl SubtractEggSteps
	ldr r0, =0x804639E | 1
	bx r0

.pool
@0x808BA60 with r2
SetBoxMonDataAtHook:
	mov r2, r5
	bl SetBoxMonDataAt
	pop {r4-r6,pc}

.pool 
@0x808BB70 with r2
GetAndCopyBoxMonDataAtHook:
	mov r2, r5
	bl GetAndCopyBoxMonDataAt
	pop {r4-r6, pc}

.pool
@0x808BDF0 with r0
sub_808BDE8Hook:
	mov r0, r8
	bl sub_808BDE8
	pop {r3}
	mov r8, r3
	pop {r4-r7,pc}

.pool
@0x8093768 with r1
CompressedStorageSummaryScreenUpdate1:
	lsl r0, #0x18
	lsr r0, #0x18
	mov r1, #0x0
	bl GetCompressedMonPtr
	ldr r1, =0x8093772 | 1
	bx r1

.pool
@0x8138BD2 with r1
CompressedStorageSummaryScreenUpdate2:
	bl SummaryScreenBoxMonMultiplier
	add r0, r4
	mov r1, r5
	bl CompressedMonToMon
	pop {r4-r5,pc}


.pool
@0x8139208 with r1
SwapBoxMonMovesUpdate1:
	lsr r0, #0x18
	bl SummaryScreenBoxMonMultiplier
	add r6, r0
	ldr r0, =DAMAGE_LOC
	str r6, [r0] @Use the damage loc as temporary storage

	ldr r0, =BATTLE_DATA @Battle data is being used as a temporary mon
	mov r1, r6
	bl CreateBoxMonFromCompressedMon
	ldr r6, =BATTLE_DATA
	ldr r0, =0x8139212 | 1
	bx r0

.pool
@0x8139304 with r0
SwapBoxMonMovesUpdate2:
	ldr r0, =BATTLE_DATA
	ldr r1, =DAMAGE_LOC
	ldr r1, [r1]
	bl CreateCompressedMonFromBoxMon @Update the mon in the PC
	add sp, #0xC
	pop {r3-r5}
	mov r8, r3
	mov r9, r4
	mov r10, r5
	pop {r4-r7,pc}

.pool
@0x80D860C with r0
ExpandedAbilityNamesBattle2:
	add r0, r5, r7
	ldrb r0, [r0, #0x1]
	bl GetAbilityName
	mov r1, r0
	ldr r0, =0x80D8618 | 1
	bx r0

.pool
@0x8136714 with r1
ExpandedAbilityNamesSummaryScreen:
	mov r1, r4
	bl CopyAbilityName
	ldr r0, =0x8136720 | 1
	bx r0


.pool
@0x8012850 with r0
BatonPassEffects:
	ldrb r1, [r3]
	lsl r1, #0x2
	add r1, r10
	ldr r0, [r1]
	ldr r2, =(STATUS3_SEEDED_RECOVERY | STATUS3_SEEDED | STATUS3_LOCKON | STATUS3_PERISH_SONG | STATUS3_ROOTED | STATUS3_LEVITATING | STATUS3_AQUA_RING | STATUS3_POWER_TRICK | STATUS3_ABILITY_SUPRESS)
	and r0, r2
	str r0, [r1]
BatonPassReturnReturn:
	ldr r0, =0x801285E | 1
	bx r0

.pool
@x80D8486 with r0
DisplayStatStageMessage:
	ldr r0, .SeverelyIndex
	cmp r0, r1
	beq BufferSeverely
	add r0, #0x1
	cmp r0, r1
	bne DisplayStatStageMessageNormal
	ldr r1, .DrasticallyStringPtr
	b DisplayStatStageMessageReturn

BufferSeverely:
	ldr r1, .SeverelyStringPtr
	b DisplayStatStageMessageReturn

DisplayStatStageMessageNormal:
	ldr r0, .gBattleStringsTable
	sub r1, #0xC
	lsl r1, r1, #0x2
	add r1, r1, r0
	ldr r1, [r1]

DisplayStatStageMessageReturn:
	ldr r0, =0x080D868E | 1
	bx r0
	
.align 2
.SeverelyIndex: .word 0x185
.gBattleStringsTable: .word 0x83FDF3C
.DrasticallyStringPtr: .word DrasticallyString
.SeverelyStringPtr: .word SeverelyString
	
.pool
@0x810BD64 with r1
CharacterCustomizationUpdateTrainerFrontPic2:
	mov r1, r4
	bl TryUpdateTrainerPicPalTrainerCard
	pop {r4-r5, pc}

.pool
@0x81090B8 with r0
SelectItemFromBagCheck:
	bl ShouldSelectItemFromBag
	cmp r0, #0x0
	bne SelectItemFromBagReturn
	ldr r1, =0x3005090 @;Tasks
	lsl r0, r6, #0x2
	add r0, r6
	lsl r0, #0x3
	ldr r2, =0x81090C0 | 1
	bx r2
	
SelectItemFromBagReturn:
	ldr r1, =0x8109024 | 1
	bx r1

@0x81323B0 with r0
SelectItemFromTMCaseCheck:
	bl ShouldSelectItemFromTMCase
	cmp r0, #0x0
	bne SelectItemFromTMCaseReturn
	ldr r0, =0x3005090 @;Tasks
	lsl r1, r7, #0x2
	add r1, r7
	lsl r1, #0x3
	ldr r2, =0x81323B8 | 1
bxr2:
	bx r2
	
SelectItemFromTMCaseReturn:
	ldr r0, =0x8132370 | 1
	bx r0

.pool
@0x80DB3C8 with r1
GrassFootstepNoiseHook:
	cmp r0, #0x0
	beq DoGrassFootstepNoise
	mov r0, r3
	mov r1, #0x4
DoSeekSpriteAnim:
	ldr r2, =SeekSpriteAnim
	bl bxr2
	b EndGrassFootstepNoiseCheck
	
DoGrassFootstepNoise:
	bl PlayGrassFootstepNoise
EndGrassFootstepNoiseCheck:
	mov r0, #0x0
	add sp, sp, #0x4
	pop {r4-r5, pc}

.pool
@0x80DB678 with r1
VeryTallGrassFootstepNoiseHook:
	cmp r0, #0x0
	beq DoGrassFootstepNoise
	mov r0, r3
	mov r1, #0x6
	b DoSeekSpriteAnim

.pool
@0x80DB9C4 with r1
SandFootstepNoiseHook:
	lsr r5, r0, #0x18
	bl PlaySandFootstepNoise
	mov r1, r5
	cmp r1, #0x40
	beq EndSandFootstepFieldEffect
	lsl r0, r1, #0x4
	ldr r3, =0x080DB9CC | 1
	bx r3
	
EndSandFootstepFieldEffect:
	ldr r0, =0x080DB9FE | 1
	bx r0

.pool
@0x807F542 with r0
WhiteoutScreenFix:
	mov r0, r1
	bl LoadProperWhiteoutString
	mov r1, r0
	mov r0, r6
	mov r2, #0x2
	mov r3, #0x8
	bl PrintWhiteoutTextScreen
	ldr r1, =0x807F54C | 1
	bx r1

PrintWhiteoutTextScreen:
	ldr r4, =0x807F3A4 | 1
	bx r4

.pool
@0x803401E with r4
TryHandleLaunchBattleTableAnimationHook:
	bl TryHandleLaunchBattleTableAnimation
	pop {r4-r7,pc}
