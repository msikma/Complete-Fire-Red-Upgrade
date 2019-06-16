.thumb
.text
.align 2

.include "../defines"

.equ MOVE_TABLES_TERMIN, 0xFEFE

.global SheerForceTable
.global RecklessTable
.global IronFistTable
.global MegaLauncherTable
.global StrongJawTable
.global BallBombMoveTable
.global DanceMoveTable
.global GravityBanTable
.global MeFirstBanTable
.global CopycatBanTable
.global InstructBanList
.global MetronomeBanTable
.global AssistBanTable
.global MimicBanTable
.global PowderTable
.global SoundMoveTable
.global SubstituteBypassTable
.global TypeChangeExceptionTable
.global SkyBattleBanTable
.global StatChangeIgnoreTable
.global HighCritTable
.global AlwaysCritTable
.global SleepTalkBanTable
.global MovesThatCallOtherMovesTable
.global MovesThatRequireRecharging
.global MinimizeHitTable
.global MoldBreakerMoves
.global FlinchMoveTable
.global ParentalBondBanList
.global MovesCanUnfreezeAttacker
.global MovesCanUnfreezeTarget
.global MovesThatChangePhysicality
.global TwoToFiveStrikesMoves
.global TwoStrikesMoves
.global ThreeStrikesMoves
.global Percent25RecoilMoves
.global Percent33RecoilMoves
.global Percent50RecoilMoves
.global Percent66RecoilMoves
.global Percent75RecoilMoves
.global Percent100RecoilMoves
.global IgnoreAirTable
.global IgnoreUndergoundTable
.global IgnoreUnderwaterTable
.global AlwaysHitRainTable
.global SpecialAttackPhysicalDamageMoves
.global SpecialWholeFieldMoveTable
.global AromaVeilTable

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SheerForceTable:
.hword MOVE_AIRSLASH
.hword MOVE_ANCIENTPOWER
.hword MOVE_ASTONISH
.hword MOVE_BITE
.hword MOVE_BLIZZARD
.hword MOVE_BODYSLAM
.hword MOVE_BUBBLE
.hword MOVE_BUBBLEBEAM
.hword MOVE_BULLDOZE
.hword MOVE_CHARGEBEAM
.hword MOVE_CONFUSION
.hword MOVE_CRUNCH
.hword MOVE_CRUSHCLAW
.hword MOVE_DARKPULSE
.hword MOVE_DRAGONRUSH
.hword MOVE_DRAGONBREATH
.hword MOVE_DOUBLEIRONBASH
.hword MOVE_DYNAMICPUNCH
.hword MOVE_EARTHPOWER
.hword MOVE_EMBER
.hword MOVE_EXTRASENSORY
.hword MOVE_FAKEOUT
.hword MOVE_FIREBLAST
.hword MOVE_FIREFANG
.hword MOVE_FIREPUNCH
.hword MOVE_FLAMECHARGE
.hword MOVE_FLAMEWHEEL
.hword MOVE_FLAMETHROWER
.hword MOVE_FLAREBLITZ
.hword MOVE_FLASHCANNON
.hword MOVE_FOCUSBLAST
.hword MOVE_FORCEPALM
.hword MOVE_GUNKSHOT
.hword MOVE_HEADBUTT
.hword MOVE_HEATWAVE
.hword MOVE_ICEBEAM
.hword MOVE_ICEFANG
.hword MOVE_ICEPUNCH
.hword MOVE_ICYWIND
.hword MOVE_IRONHEAD
.hword MOVE_IRONTAIL
.hword MOVE_LAVAPLUME
.hword MOVE_LIQUIDATION
.hword MOVE_LOWSWEEP
.hword MOVE_METALCLAW
.hword MOVE_MUDBOMB
.hword MOVE_MUDSHOT
.hword MOVE_MUDSLAP
.hword MOVE_PLAYROUGH
.hword MOVE_POISONFANG
.hword MOVE_POISONJAB
.hword MOVE_POISONSTING
.hword MOVE_POISONTAIL
.hword MOVE_POWERUPPUNCH
.hword MOVE_PSYCHIC
.hword MOVE_ROCKCLIMB
.hword MOVE_ROCKSLIDE
.hword MOVE_ROCKSMASH
.hword MOVE_ROCKTOMB
.hword MOVE_SCALD
.hword MOVE_SECRETPOWER
.hword MOVE_SHADOWBALL
.hword MOVE_SIGNALBEAM
.hword MOVE_SKYATTACK
.hword MOVE_SLUDGEBOMB
.hword MOVE_SLUDGEWAVE
.hword MOVE_SNARL
.hword MOVE_SNORE
.hword MOVE_STEELWING
.hword MOVE_STOMP
.hword MOVE_STRUGGLEBUG
.hword MOVE_THUNDER
.hword MOVE_THUNDERFANG
.hword MOVE_THUNDERBOLT
.hword MOVE_THUNDERPUNCH
.hword MOVE_TWISTER
.hword MOVE_WATERPULSE
.hword MOVE_WATERFALL
.hword MOVE_ZAPCANNON
.hword MOVE_ZENHEADBUTT
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

RecklessTable:
.hword MOVE_BRAVEBIRD
.hword MOVE_DOUBLEEDGE
.hword MOVE_FLAREBLITZ
.hword MOVE_HEADCHARGE
.hword MOVE_HEADSMASH
.hword MOVE_HIGHJUMPKICK
.hword MOVE_JUMPKICK
.hword MOVE_LIGHTOFRUIN
.hword MOVE_SUBMISSION
.hword MOVE_TAKEDOWN
.hword MOVE_VOLTTACKLE
.hword MOVE_WOODHAMMER
.hword MOVE_WILDCHARGE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IronFistTable:
.hword MOVE_BULLETPUNCH
.hword MOVE_COMETPUNCH
.hword MOVE_DIZZYPUNCH
.hword MOVE_DRAINPUNCH
.hword MOVE_DYNAMICPUNCH
.hword MOVE_FIREPUNCH
.hword MOVE_FOCUSPUNCH
.hword MOVE_HAMMERARM
.hword MOVE_ICEHAMMER
.hword MOVE_ICEPUNCH
.hword MOVE_MACHPUNCH
.hword MOVE_MEGAPUNCH
.hword MOVE_METEORMASH
.hword MOVE_PLASMAFISTS
.hword MOVE_POWERUPPUNCH
.hword MOVE_SHADOWPUNCH
.hword MOVE_SKYUPPERCUT
.hword MOVE_THUNDERPUNCH
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MegaLauncherTable:
.hword MOVE_AURASPHERE
.hword MOVE_DARKPULSE
.hword MOVE_DRAGONPULSE
.hword MOVE_HEALPULSE
.hword MOVE_ORIGINPULSE
.hword MOVE_WATERPULSE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

StrongJawTable:
.hword MOVE_BITE
.hword MOVE_CRUNCH
.hword MOVE_FIREFANG
.hword MOVE_HYPERFANG
.hword MOVE_ICEFANG
.hword MOVE_POISONFANG
.hword MOVE_PSYCHICFANGS
.hword MOVE_THUNDERFANG
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

BallBombMoveTable:
.hword MOVE_ACIDSPRAY
.hword MOVE_AURASPHERE
.hword MOVE_BARRAGE
.hword MOVE_BEAKBLAST
.hword MOVE_BULLETSEED
.hword MOVE_EGGBOMB
.hword MOVE_ELECTROBALL
.hword MOVE_ENERGYBALL
.hword MOVE_FOCUSBLAST
.hword MOVE_GYROBALL
.hword MOVE_ICEBALL
.hword MOVE_MAGNETBOMB
.hword MOVE_MISTBALL
.hword MOVE_MUDBOMB
.hword MOVE_OCTAZOOKA
.hword MOVE_POLLENPUFF
.hword MOVE_ROCKBLAST
.hword MOVE_ROCKWRECKER
.hword MOVE_SEARINGSHOT
.hword MOVE_SEEDBOMB
.hword MOVE_SHADOWBALL
.hword MOVE_SLUDGEBOMB
.hword MOVE_WEATHERBALL
.hword MOVE_ZAPCANNON
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

DanceMoveTable:
.hword MOVE_DRAGONDANCE
.hword MOVE_FEATHERDANCE
.hword MOVE_FIERYDANCE
.hword MOVE_LUNARDANCE
.hword MOVE_PETALDANCE
.hword MOVE_QUIVERDANCE
.hword MOVE_REVELATIONDANCE
.hword MOVE_SWORDSDANCE
.hword MOVE_TEETERDANCE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

GravityBanTable:
.hword MOVE_BOUNCE
.hword MOVE_FLY
.hword MOVE_SPLASH
.hword MOVE_JUMPKICK
.hword MOVE_HIGHJUMPKICK
.hword MOVE_MAGNETRISE
.hword MOVE_TELEKINESIS
.hword MOVE_FLYINGPRESS
.hword MOVE_SKYDROP
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MeFirstBanTable:
.hword MOVE_STRUGGLE
.hword MOVE_BELCH
.hword MOVE_CHATTER
.hword MOVE_COVET
.hword MOVE_THIEF
.hword MOVE_FOCUSPUNCH
.hword MOVE_SHELLTRAP
.hword MOVE_BEAKBLAST
.hword MOVE_COUNTER
.hword MOVE_MIRRORCOAT
.hword MOVE_METALBURST
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

CopycatBanTable:
.hword MOVE_ASSIST
.hword MOVE_BANEFULBUNKER
.hword MOVE_BEAKBLAST
.hword MOVE_BESTOW
.hword MOVE_CELEBRATE
.hword MOVE_CHATTER
.hword MOVE_CIRCLETHROW
.hword MOVE_COPYCAT
.hword MOVE_COUNTER
.hword MOVE_COVET
.hword MOVE_DESTINYBOND
.hword MOVE_DETECT
.hword MOVE_DRAGONTAIL
.hword MOVE_ENDURE
.hword MOVE_FEINT
.hword MOVE_FOCUSPUNCH
.hword MOVE_FOLLOWME
.hword MOVE_HELPINGHAND
.hword MOVE_HOLDHANDS
.hword MOVE_KINGSSHIELD
.hword MOVE_MATBLOCK
.hword MOVE_MEFIRST
.hword MOVE_METRONOME
.hword MOVE_MIMIC
.hword MOVE_MIRRORCOAT
.hword MOVE_MIRRORMOVE
.hword MOVE_PROTECT
.hword MOVE_RAGEPOWDER
.hword MOVE_ROAR
.hword MOVE_SHELLTRAP
.hword MOVE_SKETCH
.hword MOVE_SLEEPTALK
.hword MOVE_SNATCH
.hword MOVE_STRUGGLE
.hword MOVE_SPIKYSHIELD
.hword MOVE_SPOTLIGHT
.hword MOVE_SWITCHEROO
.hword MOVE_THIEF
.hword MOVE_TRANSFORM
.hword MOVE_TRICK
.hword MOVE_WHIRLWIND
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

InstructBanList:
.hword MOVE_INSTRUCT
.hword MOVE_BIDE
.hword MOVE_FOCUSPUNCH
.hword MOVE_BEAKBLAST
.hword MOVE_SHELLTRAP
.hword MOVE_SKETCH
.hword MOVE_TRANSFORM
.hword MOVE_MIMIC
.hword MOVE_KINGSSHIELD
.hword MOVE_STRUGGLE
.hword MOVE_BOUNCE
.hword MOVE_DIG
.hword MOVE_DIVE
.hword MOVE_FLY
.hword MOVE_FREEZESHOCK
.hword MOVE_GEOMANCY
.hword MOVE_ICEBURN
.hword MOVE_PHANTOMFORCE
.hword MOVE_RAZORWIND
.hword MOVE_SHADOWFORCE
.hword MOVE_SKULLBASH
.hword MOVE_SKYATTACK
.hword MOVE_SKYDROP
.hword MOVE_SOLARBEAM
.hword MOVE_SOLARBLADE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MetronomeBanTable:
.hword MOVE_AFTERYOU
.hword MOVE_ASSIST
.hword MOVE_BANEFULBUNKER
.hword MOVE_BEAKBLAST
.hword MOVE_BELCH
.hword MOVE_BESTOW
.hword MOVE_CELEBRATE
.hword MOVE_CHATTER
.hword MOVE_COPYCAT
.hword MOVE_COUNTER
.hword MOVE_COVET
.hword MOVE_CRAFTYSHIELD
.hword MOVE_DESTINYBOND
.hword MOVE_DETECT
.hword MOVE_DIAMONDSTORM
.hword MOVE_ENDURE
.hword MOVE_FEINT
.hword MOVE_FLEURCANNON
.hword MOVE_FOCUSPUNCH
.hword MOVE_FOLLOWME
.hword MOVE_FREEZESHOCK
.hword MOVE_HELPINGHAND
.hword MOVE_HOLDHANDS
.hword MOVE_HYPERSPACEFURY
.hword MOVE_HYPERSPACEHOLE
.hword MOVE_ICEBURN
.hword MOVE_INSTRUCT
.hword MOVE_KINGSSHIELD
.hword MOVE_LIGHTOFRUIN
.hword MOVE_MATBLOCK
.hword MOVE_MEFIRST
.hword MOVE_METRONOME
.hword MOVE_MIMIC
.hword MOVE_MINDBLOWN
.hword MOVE_MIRRORCOAT
.hword MOVE_MIRRORMOVE
.hword MOVE_NATUREPOWER
.hword MOVE_PHOTONGEYSER
.hword MOVE_PLASMAFISTS
.hword MOVE_PROTECT
.hword MOVE_QUASH
.hword MOVE_QUICKGUARD
.hword MOVE_RAGEPOWDER
.hword MOVE_RELICSONG
.hword MOVE_SECRETSWORD
.hword MOVE_SHELLTRAP
.hword MOVE_SKETCH
.hword MOVE_SLEEPTALK
.hword MOVE_SNARL
.hword MOVE_SNATCH
.hword MOVE_SNORE
.hword MOVE_SPECTRALTHIEF
.hword MOVE_SPIKYSHIELD
.hword MOVE_SPOTLIGHT
.hword MOVE_STEAMERUPTION
.hword MOVE_STRUGGLE
.hword MOVE_SWITCHEROO
.hword MOVE_TECHNOBLAST
.hword MOVE_THOUSANDARROWS
.hword MOVE_THOUSANDWAVES
.hword MOVE_THIEF
.hword MOVE_TRANSFORM
.hword MOVE_TRICK
.hword MOVE_VCREATE
.hword MOVE_WIDEGUARD
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

AssistBanTable:
.hword MOVE_ASSIST
.hword MOVE_BANEFULBUNKER
.hword MOVE_BEAKBLAST
.hword MOVE_BELCH
.hword MOVE_BESTOW
.hword MOVE_BOUNCE
.hword MOVE_CELEBRATE
.hword MOVE_CHATTER
.hword MOVE_CIRCLETHROW
.hword MOVE_COPYCAT
.hword MOVE_COUNTER
.hword MOVE_COVET
.hword MOVE_DESTINYBOND
.hword MOVE_DETECT
.hword MOVE_DIG
.hword MOVE_DIVE
.hword MOVE_DRAGONTAIL
.hword MOVE_ENDURE
.hword MOVE_FEINT
.hword MOVE_FLY
.hword MOVE_FOCUSPUNCH
.hword MOVE_FOLLOWME
.hword MOVE_HELPINGHAND
.hword MOVE_HOLDHANDS
.hword MOVE_KINGSSHIELD
.hword MOVE_MATBLOCK
.hword MOVE_MEFIRST
.hword MOVE_METRONOME
.hword MOVE_MIMIC
.hword MOVE_MIRRORCOAT
.hword MOVE_MIRRORMOVE
.hword MOVE_NATUREPOWER
.hword MOVE_PHANTOMFORCE
.hword MOVE_PROTECT
.hword MOVE_RAGEPOWDER
.hword MOVE_ROAR
.hword MOVE_SHADOWFORCE
.hword MOVE_SHELLTRAP
.hword MOVE_SKETCH
.hword MOVE_SKYDROP
.hword MOVE_SLEEPTALK
.hword MOVE_SNATCH
.hword MOVE_SPIKYSHIELD
.hword MOVE_SPOTLIGHT
.hword MOVE_STRUGGLE
.hword MOVE_SWITCHEROO
.hword MOVE_THIEF
.hword MOVE_TRANSFORM
.hword MOVE_TRICK
.hword MOVE_WHIRLWIND
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MimicBanTable:
.hword MOVE_METRONOME
.hword MOVE_STRUGGLE
.hword MOVE_SKETCH
.hword MOVE_MIMIC
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

PowderTable:
.hword MOVE_COTTONSPORE
.hword MOVE_POISONPOWDER
.hword MOVE_POWDER
.hword MOVE_RAGEPOWDER
.hword MOVE_SLEEPPOWDER
.hword MOVE_SPORE
.hword MOVE_STUNSPORE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SoundMoveTable:
.hword MOVE_BOOMBURST
.hword MOVE_BUGBUZZ
.hword MOVE_CHATTER
.hword MOVE_CLANGINGSCALES
.hword MOVE_CONFIDE
.hword MOVE_DISARMINGVOICE
.hword MOVE_ECHOEDVOICE
.hword MOVE_GRASSWHISTLE
.hword MOVE_GROWL
.hword MOVE_HEALBELL
.hword MOVE_HYPERVOICE
.hword MOVE_METALSOUND
.hword MOVE_NOBLEROAR
.hword MOVE_PARTINGSHOT
.hword MOVE_PERISHSONG
.hword MOVE_RELICSONG
.hword MOVE_ROAR
.hword MOVE_ROUND
.hword MOVE_SCREECH
.hword MOVE_SING
.hword MOVE_SNARL
.hword MOVE_SNORE
.hword MOVE_SPARKLINGARIA
.hword MOVE_SUPERSONIC
.hword MOVE_UPROAR
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SubstituteBypassTable:
.hword MOVE_AFTERYOU
.hword MOVE_AROMATICMIST
.hword MOVE_ATTRACT
.hword MOVE_BESTOW
.hword MOVE_CONVERSION2
.hword MOVE_CURSE
.hword MOVE_DESTINYBOND
.hword MOVE_DISABLE
.hword MOVE_ENCORE
.hword MOVE_FAIRYLOCK
.hword MOVE_FORESIGHT
.hword MOVE_GEARUP
.hword MOVE_GRUDGE
.hword MOVE_GUARDSWAP
.hword MOVE_HAZE
.hword MOVE_HEARTSWAP
.hword MOVE_HELPINGHAND
.hword MOVE_HYPERSPACEFURY
.hword MOVE_HYPERSPACEHOLE
.hword MOVE_IMPRISON
.hword MOVE_INSTRUCT
.hword MOVE_MAGNETICFLUX
.hword MOVE_MIRACLEEYE
.hword MOVE_ODORSLEUTH
.hword MOVE_PLAYNICE
.hword MOVE_POWDER
.hword MOVE_POWERSWAP
.hword MOVE_PSYCHUP
.hword MOVE_REFLECTTYPE
.hword MOVE_ROLEPLAY
.hword MOVE_SKILLSWAP
.hword MOVE_SPECTRALTHIEF
.hword MOVE_SPEEDSWAP
.hword MOVE_SPITE
.hword MOVE_TAUNT
.hword MOVE_TORMENT
.hword MOVE_WHIRLWIND
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TypeChangeExceptionTable:
.hword MOVE_HIDDENPOWER
.hword MOVE_JUDGMENT
.hword MOVE_NATURALGIFT
.hword MOVE_REVELATIONDANCE
.hword MOVE_STRUGGLE
.hword MOVE_TECHNOBLAST
.hword MOVE_WEATHERBALL
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

StatChangeIgnoreTable:
.hword MOVE_SACREDSWORD
.hword MOVE_CHIPAWAY
.hword MOVE_DARKESTLARIAT
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SkyBattleBanTable:
.hword MOVE_BODYSLAM
.hword MOVE_BULLDOZE
.hword MOVE_DIG
.hword MOVE_DIVE
.hword MOVE_EARTHPOWER
.hword MOVE_EARTHQUAKE
.hword MOVE_ELECTRICTERRAIN
.hword MOVE_FISSURE
.hword MOVE_FIREPLEDGE
.hword MOVE_FLYINGPRESS
.hword MOVE_FRENZYPLANT
.hword MOVE_GEOMANCY
.hword MOVE_GRASSKNOT
.hword MOVE_GRASSPLEDGE
.hword MOVE_GRASSYTERRAIN
.hword MOVE_GRAVITY
.hword MOVE_HEATCRASH
.hword MOVE_HEAVYSLAM
.hword MOVE_INGRAIN
.hword MOVE_LANDSWRATH
.hword MOVE_MAGNITUDE
.hword MOVE_MATBLOCK
.hword MOVE_MISTYTERRAIN
.hword MOVE_MUDSPORT
.hword MOVE_MUDDYWATER
.hword MOVE_ROTOTILLER
.hword MOVE_SEISMICTOSS
.hword MOVE_SLAM
.hword MOVE_SMACKDOWN
.hword MOVE_SPIKES
.hword MOVE_STOMP
.hword MOVE_SUBSTITUTE
.hword MOVE_SURF
.hword MOVE_TOXICSPIKES
.hword MOVE_THOUSANDARROWS
.hword MOVE_THOUSANDWAVES
.hword MOVE_WATERPLEDGE
.hword MOVE_WATERSPORT
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

HighCritTable:
.hword MOVE_AEROBLAST
.hword MOVE_AIRCUTTER
.hword MOVE_ATTACKORDER
.hword MOVE_BLAZEKICK
.hword MOVE_CRABHAMMER
.hword MOVE_CROSSCHOP
.hword MOVE_CROSSPOISON
.hword MOVE_DRILLRUN
.hword MOVE_KARATECHOP
.hword MOVE_LEAFBLADE
.hword MOVE_NIGHTSLASH
.hword MOVE_POISONTAIL
.hword MOVE_PSYCHOCUT
.hword MOVE_RAZORLEAF
.hword MOVE_RAZORWIND
.hword MOVE_SHADOWCLAW
.hword MOVE_SKYATTACK
.hword MOVE_SLASH
.hword MOVE_SPACIALREND
.hword MOVE_STONEEDGE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

AlwaysCritTable:
.hword MOVE_STORMTHROW
.hword MOVE_FROSTBREATH
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SleepTalkBanTable:
.hword MOVE_ASSIST
.hword MOVE_BIDE
.hword MOVE_BELCH
.hword MOVE_BEAKBLAST
.hword MOVE_BOUNCE
.hword MOVE_COPYCAT
.hword MOVE_DIG
.hword MOVE_DIVE
.hword MOVE_FREEZESHOCK
.hword MOVE_FLY
.hword MOVE_FOCUSPUNCH
.hword MOVE_GEOMANCY
.hword MOVE_ICEBURN
.hword MOVE_MEFIRST
.hword MOVE_METRONOME
.hword MOVE_MIRRORMOVE
.hword MOVE_MIMIC
.hword MOVE_PHANTOMFORCE
.hword MOVE_RAZORWIND
.hword MOVE_SHADOWFORCE
.hword MOVE_SHELLTRAP
.hword MOVE_SKETCH
.hword MOVE_SKULLBASH
.hword MOVE_SKYATTACK
.hword MOVE_SKYDROP
.hword MOVE_SLEEPTALK
.hword MOVE_SOLARBLADE
.hword MOVE_SOLARBEAM
.hword MOVE_UPROAR
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MovesThatCallOtherMovesTable:
.hword MOVE_ASSIST
.hword MOVE_COPYCAT
.hword MOVE_MEFIRST
.hword MOVE_METRONOME
.hword MOVE_MIRRORMOVE
.hword MOVE_NATUREPOWER
.hword MOVE_SLEEPTALK
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MovesThatRequireRecharging:
.hword MOVE_BLASTBURN
.hword MOVE_FRENZYPLANT
.hword MOVE_GIGAIMPACT
.hword MOVE_HYDROCANNON
.hword MOVE_HYPERBEAM
.hword MOVE_PRISMATICLASER
.hword MOVE_ROAROFTIME
.hword MOVE_ROCKWRECKER
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MinimizeHitTable:
.hword MOVE_BODYSLAM
.hword MOVE_DRAGONRUSH
.hword MOVE_FLYINGPRESS
.hword MOVE_HEATCRASH
.hword MOVE_STEAMROLLER
.hword MOVE_STOMP
.hword MOVE_HEAVYSLAM
.hword MOVE_MALICIOUS_MOONSAULT
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MoldBreakerMoves:
.hword MOVE_MOONGEISTBEAM
.hword MOVE_PHOTONGEYSER
.hword MOVE_SUNSTEELSTRIKE
.hword MOVE_LIGHT_THAT_BURNS_THE_SKY
.hword MOVE_MENACING_MOONRAZE_MAELSTROM

.hword MOVE_SEARING_SUNRAZE_SMASH
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

FlinchMoveTable:
.hword MOVE_AIRSLASH
.hword MOVE_ASTONISH
.hword MOVE_BITE
.hword MOVE_BONECLUB
.hword MOVE_DARKPULSE
.hword MOVE_DRAGONRUSH
.hword MOVE_EXTRASENSORY
.hword MOVE_FAKEOUT
.hword MOVE_FIREFANG
.hword MOVE_HEADBUTT
.hword MOVE_HEARTSTAMP
.hword MOVE_HYPERFANG
.hword MOVE_ICEFANG
.hword MOVE_ICICLECRASH
.hword MOVE_IRONHEAD
.hword MOVE_NEEDLEARM
.hword MOVE_ROCKSLIDE
.hword MOVE_ROLLINGKICK
.hword MOVE_SKYATTACK
.hword MOVE_SNORE
.hword MOVE_STEAMROLLER
.hword MOVE_STOMP
.hword MOVE_THUNDERFANG
.hword MOVE_TWISTER
.hword MOVE_WATERFALL
.hword MOVE_ZENHEADBUTT
.hword MOVE_ZINGZAP
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ParentalBondBanList:
.hword MOVE_FLING
.hword MOVE_SELFDESTRUCT
.hword MOVE_EXPLOSION
.hword MOVE_FINALGAMBIT
.hword MOVE_ENDEAVOR
.hword MOVE_UPROAR
.hword MOVE_ICEBALL
.hword MOVE_ROLLOUT
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MovesCanUnfreezeAttacker:
.hword MOVE_FLAMEWHEEL
.hword MOVE_SACREDFIRE
.hword MOVE_FLAREBLITZ
.hword MOVE_FUSIONFLARE
.hword MOVE_SCALD
.hword MOVE_STEAMERUPTION
.hword MOVE_BURNUP
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MovesCanUnfreezeTarget:
.hword MOVE_SCALD
.hword MOVE_STEAMERUPTION
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

MovesThatChangePhysicality:
.hword MOVE_PHOTONGEYSER
.hword MOVE_LIGHT_THAT_BURNS_THE_SKY
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TwoToFiveStrikesMoves:
.hword MOVE_ARMTHRUST
.hword MOVE_BARRAGE
.hword MOVE_BONERUSH
.hword MOVE_BULLETSEED
.hword MOVE_COMETPUNCH
.hword MOVE_DOUBLESLAP
.hword MOVE_FURYATTACK
.hword MOVE_FURYSWIPES
.hword MOVE_ICICLESPEAR
.hword MOVE_PINMISSILE
.hword MOVE_ROCKBLAST
.hword MOVE_SPIKECANNON
.hword MOVE_TAILSLAP
.hword MOVE_WATERSHURIKEN
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TwoStrikesMoves:
.hword MOVE_BONEMERANG
.hword MOVE_DOUBLEHIT
.hword MOVE_DOUBLEKICK
.hword MOVE_DUALCHOP
.hword MOVE_GEARGRIND
.hword MOVE_TWINEEDLE
.hword MOVE_DOUBLEIRONBASH
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

ThreeStrikesMoves:
.hword MOVE_TRIPLEKICK
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

Percent25RecoilMoves:
.hword MOVE_TAKEDOWN
.hword MOVE_SUBMISSION
.hword MOVE_WILDCHARGE
.hword MOVE_HEADCHARGE
.hword MOVE_TABLES_TERMIN

Percent33RecoilMoves:
.hword MOVE_DOUBLEEDGE
.hword MOVE_VOLTTACKLE
.hword MOVE_FLAREBLITZ
.hword MOVE_BRAVEBIRD
.hword MOVE_WOODHAMMER
.hword MOVE_TABLES_TERMIN

Percent50RecoilMoves:
.hword MOVE_HEADSMASH
.hword MOVE_LIGHTOFRUIN
.hword MOVE_TABLES_TERMIN

Percent66RecoilMoves:
.hword MOVE_TABLES_TERMIN

Percent75RecoilMoves:
.hword MOVE_TABLES_TERMIN

Percent100RecoilMoves:
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

IgnoreAirTable:
.hword MOVE_GUST
.hword MOVE_TWISTER
.hword MOVE_THUNDER
.hword MOVE_SKYUPPERCUT
.hword MOVE_HURRICANE
.hword MOVE_SMACKDOWN
.hword MOVE_THOUSANDARROWS
.hword MOVE_TABLES_TERMIN

IgnoreUndergoundTable:
.hword MOVE_EARTHQUAKE
.hword MOVE_MAGNITUDE
.hword MOVE_FISSURE
.hword MOVE_TABLES_TERMIN

IgnoreUnderwaterTable:
.hword MOVE_SURF
.hword MOVE_WHIRLPOOL
.hword MOVE_TABLES_TERMIN

AlwaysHitRainTable:
.hword MOVE_THUNDER
.hword MOVE_HURRICANE
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SpecialAttackPhysicalDamageMoves:
.hword MOVE_PSYSHOCK
.hword MOVE_PSYSTRIKE
.hword MOVE_SECRETSWORD
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

SpecialWholeFieldMoveTable:
.hword MOVE_MAGNETICFLUX
.hword MOVE_GEARUP
.hword MOVE_FLOWERSHIELD
.hword MOVE_ROTOTILLER
.hword MOVE_GRAVITY
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

AromaVeilTable:
.hword MOVE_DISABLE
.hword MOVE_ATTRACT
.hword MOVE_ENCORE
.hword MOVE_TORMENT
.hword MOVE_TAUNT
.hword MOVE_HEALBLOCK
.hword MOVE_TABLES_TERMIN

@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
