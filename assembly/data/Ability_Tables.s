.thumb
.text
.align 2

.include "../defines"

.equ ABILITY_TABLES_TERMIN, 0xFF

.global RolePlayBanTable
.global RolePlayAttackerBanTable
.global SkillSwapBanTable
.global WorrySeedGastroAcidBanTable
.global EntrainmentBanTableAttacker
.global EntrainmentBanTableTarget
.global SimpleBeamBanTable
.global ReceiverBanTable
.global TraceBanTable
.global MoldBreakerIgnoreAbilities

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RolePlayBanTable:
.byte ABILITY_TRACE
.byte ABILITY_WONDERGUARD
.byte ABILITY_FORECAST
.byte ABILITY_FLOWERGIFT
.byte ABILITY_MULTITYPE
.byte ABILITY_ILLUSION
.byte ABILITY_ZENMODE
.byte ABILITY_IMPOSTER
.byte ABILITY_STANCECHANGE
.byte ABILITY_POWEROFALCHEMY
.byte ABILITY_RECEIVER
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RolePlayAttackerBanTable:
.byte ABILITY_MULTITYPE
.byte ABILITY_ZENMODE
.byte ABILITY_STANCECHANGE
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SkillSwapBanTable:
.byte ABILITY_WONDERGUARD
.byte ABILITY_MULTITYPE
.byte ABILITY_ILLUSION
.byte ABILITY_STANCECHANGE
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

WorrySeedGastroAcidBanTable:
.byte ABILITY_MULTITYPE
.byte ABILITY_STANCECHANGE
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

EntrainmentBanTableAttacker:
.byte ABILITY_TRACE
.byte ABILITY_FORECAST
.byte ABILITY_FLOWERGIFT
.byte ABILITY_ZENMODE
.byte ABILITY_ILLUSION
.byte ABILITY_IMPOSTER
.byte ABILITY_POWEROFALCHEMY
.byte ABILITY_RECEIVER
.byte ABILITY_DISGUISE
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

SimpleBeamBanTable:
EntrainmentBanTableTarget:
.byte ABILITY_TRUANT
.byte ABILITY_MULTITYPE
.byte ABILITY_STANCECHANGE
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ReceiverBanTable:
.byte ABILITY_TRACE
.byte ABILITY_FORECAST
.byte ABILITY_FLOWERGIFT
.byte ABILITY_MULTITYPE
.byte ABILITY_ILLUSION
.byte ABILITY_ZENMODE
.byte ABILITY_IMPOSTER
.byte ABILITY_STANCECHANGE
.byte ABILITY_POWEROFALCHEMY
.byte ABILITY_RECEIVER
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_WONDERGUARD
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TraceBanTable:
.byte ABILITY_TRACE
.byte ABILITY_FORECAST
.byte ABILITY_FLOWERGIFT
.byte ABILITY_MULTITYPE
.byte ABILITY_ILLUSION
.byte ABILITY_ZENMODE
.byte ABILITY_IMPOSTER
.byte ABILITY_STANCECHANGE
.byte ABILITY_POWEROFALCHEMY
.byte ABILITY_RECEIVER
.byte ABILITY_SCHOOLING
.byte ABILITY_COMATOSE
.byte ABILITY_SHIELDSDOWN
.byte ABILITY_DISGUISE
.byte ABILITY_RKSSYSTEM
.byte ABILITY_BATTLEBOND
.byte ABILITY_POWERCONSTRUCT
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MoldBreakerIgnoreAbilities:
.byte ABILITY_BATTLEARMOR
.byte ABILITY_CLEARBODY
.byte ABILITY_DAMP
.byte ABILITY_DRYSKIN
.byte ABILITY_FILTER
.byte ABILITY_FLASHFIRE
.byte ABILITY_FLOWERGIFT
.byte ABILITY_HEATPROOF
.byte ABILITY_HYPERCUTTER
.byte ABILITY_IMMUNITY
.byte ABILITY_INNERFOCUS
.byte ABILITY_INSOMNIA
.byte ABILITY_KEENEYE
.byte ABILITY_LEAFGUARD
.byte ABILITY_LEVITATE
.byte ABILITY_LIGHTNINGROD
.byte ABILITY_LIMBER
.byte ABILITY_MAGMAARMOR
.byte ABILITY_MARVELSCALE
.byte ABILITY_MOTORDRIVE
.byte ABILITY_OBLIVIOUS
.byte ABILITY_OWNTEMPO
.byte ABILITY_SANDVEIL
.byte ABILITY_SHELLARMOR
.byte ABILITY_SHIELDDUST
.byte ABILITY_SIMPLE
.byte ABILITY_SNOWCLOAK
.byte ABILITY_SOLIDROCK
.byte ABILITY_SOUNDPROOF
.byte ABILITY_STICKYHOLD
.byte ABILITY_STORMDRAIN
.byte ABILITY_STURDY
.byte ABILITY_SUCTIONCUPS
.byte ABILITY_TANGLEDFEET
.byte ABILITY_THICKFAT
.byte ABILITY_UNAWARE
.byte ABILITY_VITALSPIRIT
.byte ABILITY_VOLTABSORB
.byte ABILITY_WATERABSORB
.byte ABILITY_WATERVEIL
.byte ABILITY_WHITESMOKE
.byte ABILITY_WONDERGUARD
.byte ABILITY_BIGPECKS
.byte ABILITY_CONTRARY
.byte ABILITY_FRIENDGUARD
.byte ABILITY_HEAVYMETAL
.byte ABILITY_LIGHTMETAL
.byte ABILITY_MAGICBOUNCE
.byte ABILITY_MULTISCALE
.byte ABILITY_SAPSIPPER
.byte ABILITY_TELEPATHY
.byte ABILITY_WONDERSKIN
.byte ABILITY_AROMAVEIL
.byte ABILITY_BULLETPROOF
.byte ABILITY_FLOWERVEIL
.byte ABILITY_FURCOAT
.byte ABILITY_OVERCOAT
.byte ABILITY_SWEETVEIL
.byte ABILITY_DAZZLING
.byte ABILITY_DISGUISE
.byte ABILITY_FLUFFY
.byte ABILITY_QUEENLYMAJESTY
.byte ABILITY_WATERBUBBLE
.byte ABILITY_PORTALPOWER
.byte ABILITY_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
