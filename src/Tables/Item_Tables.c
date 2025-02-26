#include "../defines.h"
#include "../../include/battle.h"
#include "../../include/constants/items.h"
#include "../../include/new/pickup_items.h"

const item_t sPickupCommonItems[PU_NUM_COMMON_ITEMS] =
{
	ITEM_POTION,
	ITEM_ANTIDOTE,
	ITEM_SUPER_POTION,
	ITEM_GREAT_BALL,
	ITEM_REPEL,
	ITEM_ESCAPE_ROPE,
	ITEM_FULL_HEAL,
	ITEM_HYPER_POTION,
	ITEM_ULTRA_BALL,
	ITEM_REVIVE,
	ITEM_RARE_CANDY,
	ITEM_SUN_STONE,
	ITEM_MOON_STONE,
	ITEM_HEART_SCALE,
	ITEM_FULL_RESTORE,
	ITEM_MAX_REVIVE,
	ITEM_PP_UP,
	ITEM_MAX_ELIXIR,
};

const item_t sPickupRareItems[PU_NUM_RARE_ITEMS] =
{
	ITEM_HYPER_POTION,
	ITEM_NUGGET,
	ITEM_KINGS_ROCK,
	ITEM_FULL_RESTORE,
	ITEM_ETHER,
	ITEM_IRON_BALL,
	ITEM_DESTINY_KNOT,
	ITEM_ELIXIR,
	ITEM_DESTINY_KNOT,
	ITEM_LEFTOVERS,
	ITEM_DESTINY_KNOT,
};

const u32 pickup_common_item_ceilings[PU_COMMON_PER_ROW] =
{
	19661, 26214, 32768, 39322, 45875, 52429, 58982, 61604, 64225
};

const u32 pickup_rare_item_ceilings[PU_RARE_PER_ROW] =
{ 
	64881, 65536
};

//Any values not listed default to BP 30
const struct FlingStruct gFlingTable[ITEMS_COUNT] =
{
	[ITEM_CHERI_BERRY] = {10, 0},
	[ITEM_CHESTO_BERRY] = {10, 0},
	[ITEM_PECHA_BERRY] = {10, 0},
	[ITEM_RAWST_BERRY] = {10, 0},
	[ITEM_ASPEAR_BERRY] = {10, 0},
	[ITEM_LEPPA_BERRY] = {10, 0},
	[ITEM_ORAN_BERRY] = {10, 0},
	[ITEM_PERSIM_BERRY] = {10, 0},
	[ITEM_LUM_BERRY] = {10, 0},
	[ITEM_SITRUS_BERRY] = {10, 0},
	[ITEM_FIGY_BERRY] = {10, 0},
	[ITEM_WIKI_BERRY] = {10, 0},
	[ITEM_MAGO_BERRY] = {10, 0},
	[ITEM_AGUAV_BERRY] = {10, 0},
	[ITEM_IAPAPA_BERRY] = {10, 0},
	[ITEM_RAZZ_BERRY] = {10, 0},
	[ITEM_BLUK_BERRY] = {10, 0},
	[ITEM_NANAB_BERRY] = {10, 0},
	[ITEM_WEPEAR_BERRY] = {10, 0},
	[ITEM_PINAP_BERRY] = {10, 0},
	[ITEM_POMEG_BERRY] = {10, 0},
	[ITEM_KELPSY_BERRY] = {10, 0},
	[ITEM_QUALOT_BERRY] = {10, 0},
	[ITEM_HONDEW_BERRY] = {10, 0},
	[ITEM_GREPA_BERRY] = {10, 0},
	[ITEM_TAMATO_BERRY] = {10, 0},
	[ITEM_CORNN_BERRY] = {10, 0},
	[ITEM_MAGOST_BERRY] = {10, 0},
	[ITEM_RABUTA_BERRY] = {10, 0},
	[ITEM_NOMEL_BERRY] = {10, 0},
	[ITEM_SPELON_BERRY] = {10, 0},
	[ITEM_PAMTRE_BERRY] = {10, 0},
	[ITEM_WATMEL_BERRY] = {10, 0},
	[ITEM_DURIN_BERRY] = {10, 0},
	[ITEM_BELUE_BERRY] = {10, 0},
	[ITEM_LIECHI_BERRY] = {10, 0},
	[ITEM_GANLON_BERRY] = {10, 0},
	[ITEM_SALAC_BERRY] = {10, 0},
	[ITEM_PETAYA_BERRY] = {10, 0},
	[ITEM_APICOT_BERRY] = {10, 0},
	[ITEM_LANSAT_BERRY] = {10, 0},
	[ITEM_STARF_BERRY] = {10, 0},
	[ITEM_ENIGMA_BERRY] = {10, 0},
	[ITEM_OCCA_BERRY] = {10, 0},
	[ITEM_PASSHO_BERRY] = {10, 0},
	[ITEM_WACAN_BERRY] = {10, 0},
	[ITEM_RINDO_BERRY] = {10, 0},
	[ITEM_YACHE_BERRY] = {10, 0},
	[ITEM_CHOPLE_BERRY] = {10, 0},
	[ITEM_KEBIA_BERRY] = {10, 0},
	[ITEM_SHUCA_BERRY] = {10, 0},
	[ITEM_COBA_BERRY] = {10, 0},
	[ITEM_PAYAPA_BERRY] = {10, 0},
	[ITEM_TANGA_BERRY] = {10, 0},
	[ITEM_CHARTI_BERRY] = {10, 0},
	[ITEM_KASIB_BERRY] = {10, 0},
	[ITEM_HABAN_BERRY] = {10, 0},
	[ITEM_COLBUR_BERRY] = {10, 0},
	[ITEM_BABIRI_BERRY] = {10, 0},
	[ITEM_CHILAN_BERRY] = {10, 0},
	[ITEM_MICLE_BERRY] = {10, 0},
	[ITEM_CUSTAP_BERRY] = {10, 0},
	[ITEM_JABOCA_BERRY] = {10, 0},
	[ITEM_ROWAP_BERRY] = {10, 0},
	[ITEM_ROSELI_BERRY] = {10, 0},
	[ITEM_KEE_BERRY] = {10, 0},
	[ITEM_MARANGA_BERRY] = {10, 0},
	[ITEM_SEA_INCENSE] = {10, 0},
	[ITEM_LAX_INCENSE] = {10, 0},
	[ITEM_LUCK_INCENSE] = {10, 0},
	[ITEM_FULL_INCENSE] = {10, 0},
	[ITEM_ODD_INCENSE] = {10, 0},
	[ITEM_PURE_INCENSE] = {10, 0},
	[ITEM_ROCK_INCENSE] = {10, 0},
	[ITEM_ROSE_INCENSE] = {10, 0},
	[ITEM_WAVE_INCENSE] = {10, 0},
	[ITEM_AIR_BALLOON] = {10, 0},
	[ITEM_BIG_ROOT] = {10, 0},
	[ITEM_BRIGHT_POWDER] = {10, 0},
	[ITEM_CHOICE_BAND] = {10, 0},
	[ITEM_CHOICE_SCARF] = {10, 0},
	[ITEM_CHOICE_SPECS] = {10, 0},
	[ITEM_DESTINY_KNOT] = {10, 0},
	[ITEM_ELECTRIC_SEED] = {10, 0},
	[ITEM_EXPERT_BELT] = {10, 0},
	[ITEM_FOCUS_BAND] = {10, 0},
	[ITEM_FOCUS_SASH] = {10, 0},
	[ITEM_GRASSY_SEED] = {10, 0},
	[ITEM_LAGGING_TAIL] = {10, 0},
	[ITEM_LEFTOVERS] = {10, 0},
	[ITEM_MENTAL_HERB] = {10, 0},
	[ITEM_METAL_POWDER] = {10, 0},
	[ITEM_MISTY_SEED] = {10, 0},
	[ITEM_MUSCLE_BAND] = {10, 0},
	[ITEM_PINK_NECTAR] = {10, 0},
	[ITEM_POWER_HERB] = {10, 0},
	[ITEM_PSYCHIC_SEED] = {10, 0},
	[ITEM_PURPLE_NECTAR] = {10, 0},
	[ITEM_QUICK_POWDER] = {10, 0},
	[ITEM_REAPER_CLOTH] = {10, 0},
	[ITEM_RED_CARD] = {10, 0},
	[ITEM_RED_NECTAR] = {10, 0},
	[ITEM_RING_TARGET] = {10, 0},
	[ITEM_SHED_SHELL] = {10, 0},
	[ITEM_SILK_SCARF] = {10, 0},
	[ITEM_SILVER_POWDER] = {10, 0},
	[ITEM_SMOOTH_ROCK] = {10, 0},
	[ITEM_SOFT_SAND] = {10, 0},
	[ITEM_SOOTHE_BELL] = {10, 0},
	[ITEM_WHITE_HERB] = {10, 0},
	[ITEM_WIDE_LENS] = {10, 0},
	[ITEM_WISE_GLASSES] = {10, 0},
	[ITEM_YELLOW_NECTAR] = {10, 0},
	[ITEM_ZOOM_LENS] = {10, 0},
	[ITEM_HEALTH_WING] = {20, 0},
	[ITEM_MUSCLE_WING] = {20, 0},
	[ITEM_RESIST_WING] = {20, 0},
	[ITEM_GENIUS_WING] = {20, 0},
	[ITEM_CLEVER_WING] = {20, 0},
	[ITEM_SWIFT_WING] = {20, 0},
	[ITEM_PRETTY_WING] = {20, 0},
	[ITEM_ETHER] = {30, 0},
	[ITEM_ELIXIR] = {30, 0},
	[ITEM_MAX_ETHER] = {30, 0},
	[ITEM_MAX_ELIXIR] = {30, 0},
	[ITEM_ABSORB_BULB] = {30, 0},
	[ITEM_ADRENALINE_ORB] = {30, 0},
	[ITEM_AMULET_COIN] = {30, 0},
	[ITEM_BALM_MUSHROOM] = {30, 0},
	[ITEM_BERRY_JUICE] = {30, 0},
	[ITEM_BIG_MALASADA] = {30, 0},
	[ITEM_BIG_MUSHROOM] = {30, 0},
	[ITEM_BIG_NUGGET] = {30, 0},
	[ITEM_BIG_PEARL] = {30, 0},
	[ITEM_BINDING_BAND] = {30, 0},
	[ITEM_BLACK_BELT] = {30, 0},
	[ITEM_BLACK_GLASSES] = {30, 0},
	[ITEM_BLACK_SLUDGE] = {30, 0},
	[ITEM_BOTTLE_CAP] = {30, 0},
	[ITEM_CASTELIACONE] = {30, 0},
	[ITEM_CELL_BATTERY] = {30, 0},
	[ITEM_CHARCOAL] = {30, 0},
	[ITEM_CLEANSE_TAG] = {30, 0},
	[ITEM_COMET_SHARD] = {30, 0},
	[ITEM_DEEP_SEA_SCALE] = {30, 0},
	[ITEM_DRAGON_SCALE] = {30, 0},
	[ITEM_EJECT_BUTTON] = {30, 0},
	[ITEM_ESCAPE_ROPE] = {30, 0},
	[ITEM_EVERSTONE] = {30, 0},
	[ITEM_EXP_SHARE] = {30, 0},
	[ITEM_FIRE_STONE] = {30, 0},
	[ITEM_FLAME_ORB] = {30, MOVE_EFFECT_BURN},
	[ITEM_FLOAT_STONE] = {30, 0},
	[ITEM_FLUFFY_TAIL] = {30, 0},
	[ITEM_GOLD_BOTTLE_CAP] = {30, 0},
	[ITEM_HEART_SCALE] = {30, 0},
	[ITEM_HONEY] = {30, 0},
	[ITEM_ICE_STONE] = {30, 0},
	[ITEM_KINGS_ROCK] = {30, MOVE_EFFECT_FLINCH},
	[ITEM_LAVA_COOKIE] = {30, 0},
	[ITEM_LEAF_STONE] = {30, 0},
	[ITEM_LIFE_ORB] = {30, 0},
	[ITEM_LIGHT_BALL] = {30, MOVE_EFFECT_PARALYSIS},
	[ITEM_LIGHT_CLAY] = {30, 0},
	[ITEM_LUCKY_EGG] = {30, 0},
	[ITEM_LUMINOUS_MOSS] = {30, 0},
	[ITEM_LUMIOSE_GALETTE] = {30, 0},
	[ITEM_MAGNET] = {30, 0},
	[ITEM_MAX_REVIVE] = {30, 0},
	[ITEM_METAL_COAT] = {30, 0},
	[ITEM_METRONOME] = {30, 0},
	[ITEM_MIRACLE_SEED] = {30, 0},
	[ITEM_MOON_STONE] = {30, 0},
	[ITEM_MYSTIC_WATER] = {30, 0},
	[ITEM_NEVER_MELT_ICE] = {30, 0},
	[ITEM_NUGGET] = {30, 0},
	[ITEM_OLD_GATEAU] = {30, 0},
	[ITEM_PEARL_STRING] = {30, 0},
	[ITEM_PEARL] = {30, 0},
	[ITEM_POKE_DOLL] = {30, 0},
	[ITEM_PRISM_SCALE] = {30, 0},
	[ITEM_PROTECTIVE_PADS] = {30, 0},
	[ITEM_RAZOR_FANG] = {30, MOVE_EFFECT_FLINCH},
	[ITEM_RELIC_BAND] = {30, 0},
	[ITEM_RELIC_COPPER] = {30, 0},
	[ITEM_RELIC_CROWN] = {30, 0},
	[ITEM_RELIC_GOLD] = {30, 0},
	[ITEM_RELIC_SILVER] = {30, 0},
	[ITEM_RELIC_STATUE] = {30, 0},
	[ITEM_RELIC_VASE] = {30, 0},
	[ITEM_REVIVE] = {30, 0},
	[ITEM_SACRED_ASH] = {30, 0},
	[ITEM_SCOPE_LENS] = {30, 0},
	[ITEM_SHALOUR_SABLE] = {30, 0},
	[ITEM_SHELL_BELL] = {30, 0},
	[ITEM_SHOAL_SALT] = {30, 0},
	[ITEM_SHOAL_SHELL] = {30, 0},
	[ITEM_SMOKE_BALL] = {30, 0},
#ifdef UNBOUND
	[ITEM_SNOWBALL] = {30, MOVE_EFFECT_FREEZE},
#else
	[ITEM_SNOWBALL] = {30, 0},
#endif
	[ITEM_SOUL_DEW] = {30, 0},
	[ITEM_SPELL_TAG] = {30, 0},
	[ITEM_STAR_PIECE] = {30, 0},
	[ITEM_STARDUST] = {30, 0},
	[ITEM_SUN_STONE] = {30, 0},
	[ITEM_THUNDER_STONE] = {30, 0},
	[ITEM_TINY_MUSHROOM] = {30, 0},
	[ITEM_TOXIC_ORB] = {30, MOVE_EFFECT_TOXIC},
	[ITEM_TWISTED_SPOON] = {30, 0},
	[ITEM_UPGRADE] = {30, 0},
	[ITEM_WATER_STONE] = {30, 0},
	[ITEM_EVIOLITE] = {40, 0},
	[ITEM_ICY_ROCK] = {40, 0},
	[ITEM_LUCKY_PUNCH] = {40, 0},
	[ITEM_FIGHTING_MEMORY] = {50, 0},
	[ITEM_FLYING_MEMORY] = {50, 0},
	[ITEM_POISON_MEMORY] = {50, 0},
	[ITEM_GROUND_MEMORY] = {50, 0},
	[ITEM_ROCK_MEMORY] = {50, 0},
	[ITEM_BUG_MEMORY] = {50, 0},
	[ITEM_GHOST_MEMORY] = {50, 0},
	[ITEM_STEEL_MEMORY] = {50, 0},
	[ITEM_FIRE_MEMORY] = {50, 0},
	[ITEM_WATER_MEMORY] = {50, 0},
	[ITEM_GRASS_MEMORY] = {50, 0},
	[ITEM_ELECTRIC_MEMORY] = {50, 0},
	[ITEM_PSYCHIC_MEMORY] = {50, 0},
	[ITEM_ICE_MEMORY] = {50, 0},
	[ITEM_DRAGON_MEMORY] = {50, 0},
	[ITEM_DARK_MEMORY] = {50, 0},
	[ITEM_FAIRY_MEMORY] = {50, 0},
	[ITEM_DUBIOUS_DISC] = {50, 0},
	[ITEM_SHARP_BEAK] = {50, 0},
	[ITEM_ADAMANT_ORB] = {60, 0},
	[ITEM_DAMP_ROCK] = {60, 0},
	[ITEM_GRISEOUS_ORB] = {60, 0},
	[ITEM_HEAT_ROCK] = {60, 0},
	[ITEM_LUSTROUS_ORB] = {60, 0},
	[ITEM_MACHO_BRACE] = {60, 0},
	[ITEM_ROCKY_HELMET] = {60, 0},
	[ITEM_STICK] = {60, 0},
	[ITEM_TERRAIN_EXTENDER] = {60, 0},
	[ITEM_BURN_DRIVE] = {70, 0},
	[ITEM_DOUSE_DRIVE] = {70, 0},
	[ITEM_SHOCK_DRIVE] = {70, 0},
	[ITEM_CHILL_DRIVE] = {70, 0},
	[ITEM_DRAGON_FANG] = {70, 0},
	[ITEM_POISON_BARB] = {70, MOVE_EFFECT_POISON},
	[ITEM_POWER_ANKLET] = {70, 0},
	[ITEM_POWER_BAND] = {70, 0},
	[ITEM_POWER_BELT] = {70, 0},
	[ITEM_POWER_BRACER] = {70, 0},
	[ITEM_POWER_LENS] = {70, 0},
	[ITEM_POWER_WEIGHT] = {70, 0},
	[ITEM_ULTRA_NECROZIUM_Z] = {80, 0},
	[ITEM_VENUSAURITE] = {80, 0},
	[ITEM_CHARIZARDITE_X] = {80, 0},
	[ITEM_CHARIZARDITE_Y] = {80, 0},
	[ITEM_BLASTOISINITE] = {80, 0},
	[ITEM_BEEDRILLITE] = {80, 0},
	[ITEM_PIDGEOTITE] = {80, 0},
	[ITEM_ALAKAZITE] = {80, 0},
	[ITEM_SLOWBRONITE] = {80, 0},
	[ITEM_GENGARITE] = {80, 0},
	[ITEM_KANGASKHANITE] = {80, 0},
	[ITEM_PINSIRITE] = {80, 0},
	[ITEM_GYARADOSITE] = {80, 0},
	[ITEM_AERODACTYLITE] = {80, 0},
	[ITEM_MEWTWONITE_X] = {80, 0},
	[ITEM_MEWTWONITE_Y] = {80, 0},
	[ITEM_AMPHAROSITE] = {80, 0},
	[ITEM_STEELIXITE] = {80, 0},
	[ITEM_SCIZORITE] = {80, 0},
	[ITEM_HERACRONITE] = {80, 0},
	[ITEM_HOUNDOOMINITE] = {80, 0},
	[ITEM_TYRANITARITE] = {80, 0},
	[ITEM_SCEPTILITE] = {80, 0},
	[ITEM_BLAZIKENITE] = {80, 0},
	[ITEM_SWAMPERTITE] = {80, 0},
	[ITEM_GARDEVOIRITE] = {80, 0},
	[ITEM_SABLENITE] = {80, 0},
	[ITEM_MAWILITE] = {80, 0},
	[ITEM_AGGRONITE] = {80, 0},
	[ITEM_MEDICHAMITE] = {80, 0},
	[ITEM_MANECTITE] = {80, 0},
	[ITEM_SHARPEDONITE] = {80, 0},
	[ITEM_CAMERUPTITE] = {80, 0},
	[ITEM_ALTARIANITE] = {80, 0},
	[ITEM_BANETTITE] = {80, 0},
	[ITEM_ABSOLITE] = {80, 0},
	[ITEM_GLALITITE] = {80, 0},
	[ITEM_SALAMENCITE] = {80, 0},
	[ITEM_METAGROSSITE] = {80, 0},
	[ITEM_LATIASITE] = {80, 0},
	[ITEM_LATIOSITE] = {80, 0},
	[ITEM_LOPUNNITE] = {80, 0},
	[ITEM_GARCHOMPITE] = {80, 0},
	[ITEM_LUCARIONITE] = {80, 0},
	[ITEM_ABOMASITE] = {80, 0},
	[ITEM_GALLADITE] = {80, 0},
	[ITEM_AUDINITE] = {80, 0},
	[ITEM_DIANCITE] = {80, 0},
	[ITEM_ASSAULT_VEST] = {80, 0},
	[ITEM_DAWN_STONE] = {80, 0},
	[ITEM_DUSK_STONE] = {80, 0},
	[ITEM_ELECTIRIZER] = {80, 0},
	[ITEM_MAGMARIZER] = {80, 0},
	[ITEM_ODD_KEYSTONE] = {80, 0},
	[ITEM_OVAL_STONE] = {80, 0},
	[ITEM_PROTECTOR] = {80, 0},
	[ITEM_QUICK_CLAW] = {80, 0},
	[ITEM_RAZOR_CLAW] = {80, 0},
	[ITEM_SACHET] = {80, 0},
	[ITEM_SAFETY_GOGGLES] = {80, 0},
	[ITEM_SHINY_STONE] = {80, 0},
	[ITEM_STICKY_BARB] = {80, 0},
	[ITEM_WEAKNESS_POLICY] = {80, 0},
	[ITEM_WHIPPED_DREAM] = {80, 0},
	[ITEM_FIST_PLATE] = {90, 0},
	[ITEM_SKY_PLATE] = {90, 0},
	[ITEM_TOXIC_PLATE] = {90, 0},
	[ITEM_EARTH_PLATE] = {90, 0},
	[ITEM_STONE_PLATE] = {90, 0},
	[ITEM_INSECT_PLATE] = {90, 0},
	[ITEM_SPOOKY_PLATE] = {90, 0},
	[ITEM_IRON_PLATE] = {90, 0},
	[ITEM_FLAME_PLATE] = {90, 0},
	[ITEM_SPLASH_PLATE] = {90, 0},
	[ITEM_MEADOW_PLATE] = {90, 0},
	[ITEM_ZAP_PLATE] = {90, 0},
	[ITEM_MIND_PLATE] = {90, 0},
	[ITEM_ICICLE_PLATE] = {90, 0},
	[ITEM_DRACO_PLATE] = {90, 0},
	[ITEM_DREAD_PLATE] = {90, 0},
	[ITEM_PIXIE_PLATE] = {90, 0},
	[ITEM_DEEP_SEA_TOOTH] = {90, 0},
	[ITEM_GRIP_CLAW] = {90, 0},
	[ITEM_THICK_CLUB] = {90, 0},
	[ITEM_HELIX_FOSSIL] = {100, 0},
	[ITEM_DOME_FOSSIL] = {100, 0},
	[ITEM_ROOT_FOSSIL] = {100, 0},
	[ITEM_CLAW_FOSSIL] = {100, 0},
	[ITEM_SKULL_FOSSIL] = {100, 0},
	[ITEM_ARMOR_FOSSIL] = {100, 0},
	[ITEM_COVER_FOSSIL] = {100, 0},
	[ITEM_PLUME_FOSSIL] = {100, 0},
	[ITEM_JAW_FOSSIL] = {100, 0},
	[ITEM_SAIL_FOSSIL] = {100, 0},
	[ITEM_HARD_STONE] = {100, 0},
	[ITEM_RARE_BONE] = {100, 0},
	[ITEM_IRON_BALL] = {130, 0},
};
