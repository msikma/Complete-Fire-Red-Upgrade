#pragma once

#if (MUGSHOT_STYLE == MUGSHOT_BIG)
	extern const u16 Big_MugshotTiles[];
	extern const u16 Big_MugshotPal[];
	extern const u16 Big_MugshotMap[];

	#define sMugshotsTiles Big_MugshotTiles
	#define sMugshotsTilemap Big_MugshotMap
	#define sMugshotsDefaultPal Big_MugshotPal
#elif (MUGSHOT_STYLE == MUGSHOT_DP)
	extern const u16 DP_MugshotTiles[];
	extern const u16 DP_MugshotPal[];
	extern const u16 DP_MugshotMap[];

	#define sMugshotsTiles DP_MugshotTiles
	#define sMugshotsTilemap DP_MugshotMap
	#define sMugshotsDefaultPal DP_MugshotPal
#else //MUGSHOT_TWO_BARS
	#define sMugshotsTiles ((const u16*) 0x83F8F60)
	#define sMugshotsTilemap ((const u16*) 0x83FAC34)
	#define sMugshotsDefaultPal ((const u16*) 0x83FA660)
#endif


#define sOpponentMugshotsPals ((const u16**) 0x83FA740)
#define sPlayerMugshotsPals ((const u16**) 0x83FA754)
#define sMugshotsTrainerPicIDsTable ((const u8*) 0x3FA494)
extern const s16 sMugshotsOpponentCoords[][2];
extern const s16 sMugshotsOpponentRotationScales[][2];

extern const u8 VS_SpriteTiles[];
extern const u8 VS_SpritePal[];

extern const u8 Silver_MugshotTiles[];
extern const u8 Silver_MugshotPal[];

extern const u16 Big_Mugshot_Yellow_GrayPal[];