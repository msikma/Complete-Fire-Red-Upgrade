.thumb
.text
.align 2

.include "../defines"

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GetCurrentPocketItemAmountHook:
	push {r1-r3, lr}
	bl GetCurrentPocketItemAmount
	pop {r1-r3, pc}

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
CountItemsInBag:
	push {r4-r6,lr}
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	bl StoreBagItemCount
	lsl r1, r5, #0x3
	ldr r0, =0x8108DD0 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
GetSmallestItemAmount:
	push {r1-r2}
	mov r0, r5
	bl GetNumItemsInPocket
	pop {r1-r2}
	ldr r3, =0x8108E29
	bx r3

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
OpenBagBugFix1:
	push {r2-r3}
	mov r0, r4
	bl GetNumItemsInPocket
	mov r1, r0
	pop {r2-r3}
	add r0, #0x1
	cmp r2, r0
	ldr r0, =0x8108A26 | 1
	bx r0	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
OpenBagBugFix2:
	push {r1-r2}
	mov r0, r4
	bl GetNumItemsInPocket
	pop {r1-r2}
	add r3, r0, #0x1
	ldr r4, =0x8108A4C | 1
	bx r4

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
OpenBagBugFix3:
	push {r2-r3}
	mov r0, r6
	bl GetNumItemsInPocket
	add r0, #0x1
	pop {r2-r3}
	ldr r1, =0x8108AC8 | 1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
AllocateBerryPouchSpace:
	push {r4,lr}
	bl AllocateBerryPouchListBuffers
	ldr r1, =0x813D1CE | 1
	bx r1

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x806BDEC with r0
ExpandedBerriesFix1:
	cmp r6, #0x1
	bls BerryFix1NotBerry
	mov r0, r5
	bl IsBerry
	cmp r0, #0x0
	beq BerryFix1NotBerry
	ldr r0, =0x806BDFC | 1
	bx r0

BerryFix1NotBerry:
	ldr r0, =0x806BE22 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x814A490 with r0
ExpandedBerriesFix2: @Get Berry Number
	cmp r6, #0x0
	beq BerryFix2Return
	mov r0, r4
	bl BerryIdFromItemId

BerryFix2Return:
	ldr r0, =0x814A4C2 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
@0x813D33C with r0
GetBerryNameBerryIdFix:
	mov r0, r5
	bl BerryIdFromItemId
	mov r1, r0
	mov r0, r6
	mov r2, #0x2
	ldr r3, =0x813D344 | 1
	bx r3	

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeBackupRegularBagHook:
	bl PokeDudeBackupBag
	ldr r5, .PokeDudeItemBackupPtr
	ldr r4, .SaveBlock1
	ldr r0, =0x810AE22 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeBackupRegularBagClearHook:
	ldr r4, .SaveBlock1
	ldr r0, =0x810AEA4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeBackupBagRestoreHook:
	bl PokeDudeRestoreBag
	ldr r4, .SaveBlock1
	ldr r0, =0x810AF0C | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeBackupTMCaseHook:
	bl PokeDudeBackupKeyItemsTMs
	ldr r0, =0x8132E98 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeClearTMCaseHook:
	ldr r0, =0x8132EC4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
.pool
PokeDudeRestoreTMCaseHook:
	bl PokeDudeRestoreKeyItemsTMs
	ldr r0, =0x81331A4 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.pool
@0x80143D4 wirh r0
AreItemsDisabledHook:
	bl IsBagDisabled
	cmp r0, #0x0
	beq CanUseItemsReturn
	ldr r0, =0x80143E0 | 1
	bx r0

CanUseItemsReturn:
	ldr r0, =0x8014428 | 1
	bx r0

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

.align 2
.PokeDudeItemBackupPtr: .word 0x203AD2C
.SaveBlock1: .word 0x3005008
