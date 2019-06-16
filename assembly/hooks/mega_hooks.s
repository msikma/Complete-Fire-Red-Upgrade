.thumb
.text
.align 2

.include "../defines"

.global MegaRetrieveDataHook

@;0x148C0 with r0
MegaRetrieveDataHook:
	push {r3}
	bl MegaRetrieveData
	pop {r3}
	ldr r0, =BATTLE_COMMUNICATION
	ldrb r1, [r3]
	ldr r2, =0x8014B30 | 1
	bx r2

LoadMegaGraphicsHook:
	push {r0}
	mov r0, r5
	bl LoadMegaGraphics
	pop {r0}

	pop {r4-r6}
	pop {r1}
	bx r1

@;0x80483A4 with r0
MegaLevelStringHook:
	lsl r4, #0x18
	lsr r4, #0x18
	
	@ Get owner of the health bar
	ldr r2, objects
	lsl r1, r5, #4
	add r1, r5
	lsl r1, r1, #2
	add r1, r2
	ldrh r0, [r1, #0x3A]
	bl HasMegaSymbol
	cmp r0, #0
	bne LoadSpecialMegaSymbol
	
	ldr r1, level_string
	mov r0, sp
	mov r2, #0x10
	bl MemCopy
	mov r0, sp
	add r0, #2
	
	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str
	
	mov r1, r0
	sub r1, #2
	b LevelStringReturn
	
LoadSpecialMegaSymbol:
	adr r1, special_string
	mov r0, sp
	mov r2, #0x10
	bl MemCopy
	mov r0, sp
	add r0, #0
	
	mov r1, r4
	mov r2, #0
	mov r3, #3
	bl int_to_str
	
	mov r1, r0
	sub r1, #2
	
	@
	mov r0, sp
	sub r1, r0
	mov r0, #3
	sub r0, r1
	lsl r1, r0, #2
	add r1, r1, r0
	sub r1, #2
	add r3, sp, #0x10
	mov r0, sp
	mov r2, #3
	ldr r6, =0x080483D6 | 1
	bx r6
	
LevelStringReturn:	
	ldr r6, =0x080483C4 | 1
	bx r6
	
int_to_str:
	ldr r6, =ConvertIntToDecimalStringN
	bx r6
	
MemCopy:
	ldr r3, =Memcpy
	bx r3
	
	.align 2
level_string: .word 0x0826051C
objects: .word 0x0202063C

special_string:
.byte 0xFF
.rept 16
.byte 0
.endr

.align 2
@;0804BE80 with r3
CreateShakerHook:
	lsl r0, #0x18
	lsr r0, #0x18
	
	@;Save Index
	ldr r3, .ShakerData
	strb r0, [r3, #1]
	
	@ Set boolean to true
	mov r2, #1
	strb r2, [r3]
	
	@ Return
	ldr r2, =0x0202063C
	lsl r1, r0, #4
	ldr r3, =0x0804BE88 | 1
	bx r3
	
@;0x804BEDC with r2
ObjcShakerHook:
	mov r2, #0
	asr r0, #0x10
	cmp r0, #0x15
	bne ObjcShakerHookReturn
	
	strh r2, [r1, #0x24]
	strh r2, [r1, #0x26]
	
	@;Set boolean to false
	ldr r1, .ShakerData
	strb r2, [r1]
	
	ldr r2, =(0x0804BEE6 + 1)
	bx r2
	
ObjcShakerHookReturn:
	pop {pc}

.align 2
.ShakerData: .word SHAKER_HELPER

@;0x803301C with r1
PlayerHandleStatusIconUpdateHook:
	ldr r0, [r0, #0x4]
	lsl r1, r2, #0x1
	add r1, r2
	lsl r1, #0x2
	add r1, r0
	push {r1}
	bl CreateMegaIndicatorAfterAnim
	pop {r1}
	ldr r0, =0x8033026 | 1
	bx r0

@;0x8038974 with r1
OpponentHandleStatusIconUpdateHook:
	ldr r0, [r0, #0x4]
	lsl r1, r2, #0x1
	add r1, r2
	lsl r1, #0x2
	add r1, r0
	push {r1}
	bl CreateMegaIndicatorAfterAnim
	pop {r1}
	ldr r0, =0x803897E | 1
	bx r0

@;0x802F858 with r0
LoadHealthBoxesIndicatorHook:
	push {r4-r6,lr}
	sub sp, #0x14
	mov r5, r0
	mov r4, r1
	lsl r5, #0x18
	lsr r5, #0x18
	bl CreateMegaIndicatorAfterAnim
	ldr r1, =0x80483A4 | 1
	bx r1
