#include "defines.h"
#include "../include/save.h"
#include "../include/constants/vars.h"

#include "../include/new/save.h"

// Each 4 KiB flash sector contains 3968 bytes of actual data followed by a 128 byte footer
#define SECTOR_DATA_SIZE 0xFF0
#define SECTOR_FOOTER_SIZE 128
#define SECTOR_SAVE_SLOT_LENGTH 14
#define PLAYER_NAME_LENGTH 8
#define FILE_SIGNATURE 0x08012025
#define gSaveBlockParasite 0x0203B174
#define parasiteSize 0xEC4

// old 0x080DA23C table changes
const struct SaveSectionOffset SaveSectionOffsets[] =
{
    {SECTOR_DATA_SIZE * 0, 0xF24}, // saveblock2
    // 0xCC byes saved

    {SECTOR_DATA_SIZE * 0, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 1, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 2, SECTOR_DATA_SIZE}, // saveblock 1
    {SECTOR_DATA_SIZE * 3, 0xD98}, // saveblock 1
    // 0x258 bytes saved

    {SECTOR_DATA_SIZE * 0, SECTOR_DATA_SIZE}, // saveblock Pokemonstorage
    {SECTOR_DATA_SIZE * 1, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 2, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 3, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 4, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 5, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 6, SECTOR_DATA_SIZE},
    {SECTOR_DATA_SIZE * 7, SECTOR_DATA_SIZE},
    {0x7F80, 0x450},
    // 0xBA0 saved
};

/* Any save sector that isn't full, we'll plop our data of these sizes in there */
static const u16 SaveBlockParasiteSizes[3] =
{
    SECTOR_DATA_SIZE - 0xF24, // 0xCC
    SECTOR_DATA_SIZE - 0xD98, // 0x258
    SECTOR_DATA_SIZE - 0x450, // 0xBA0
};


//This file's functions:
static void LoadSector30And31();
static void SaveSector30And31();
static void SaveParasite();
static void LoadParasite();
static void CallSomething(u16 arg, EraseFlash func);

/* Saving and loading for sector 30 and 31. Could potentially add the Hall of fame sectors too */
static void LoadSector30And31()
{
    struct SaveSection* saveBuffer = (struct SaveSection*)0x02039A38;
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    DoReadFlashWholeSection(30, saveBuffer);
    u32 startLoc = gSaveBlockParasite + parasiteSize;
    Memcpy((void*)(startLoc), saveBuffer, SECTOR_DATA_SIZE);

    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    DoReadFlashWholeSection(31, saveBuffer);
    startLoc += SECTOR_DATA_SIZE;
	#ifdef UNBOUND
    Memcpy((void*) startLoc, saveBuffer, 0xBD8);
	#else
	Memcpy((void*) startLoc, saveBuffer, SECTOR_DATA_SIZE);
	#endif
}

static void SaveSector30And31()
{
    struct SaveSection* saveBuffer = (struct SaveSection*)0x02039A38;
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    u32 startLoc = gSaveBlockParasite + parasiteSize;
    Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
    TryWriteSector(30, saveBuffer->data);

    /* Write sector 31 */
    Memset(saveBuffer, 0, sizeof(struct SaveSection));
    startLoc += SECTOR_DATA_SIZE;
    Memcpy(saveBuffer->data, (void*)(startLoc), SECTOR_DATA_SIZE);
    TryWriteSector(31, saveBuffer->data);
}


/* This parasitic saveblock idea originated from JPAN's work. Frees up 0xEC4 bytes - almost a sector */
static void SaveParasite()
{
    struct SaveSection* s = gFastSaveSection;
    u32 size = 0;
    u32* data = NULL;
    u32* parasiteP1 = (u32*)gSaveBlockParasite;
    u32* parasiteP2 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0]);
    u32* parasiteP3 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0] + SaveBlockParasiteSizes[1]);

    switch (s->id) {
        case 0:
            data = parasiteP1;
            size = SaveBlockParasiteSizes[0];
            break;
        case 4:
            data = parasiteP2;
            size = SaveBlockParasiteSizes[1];
            break;
        case 13:
            data = parasiteP3;
            size = SaveBlockParasiteSizes[2];
            break;
        default:
            return;
    };
    u16 index = SECTOR_DATA_SIZE - size;
    Memcpy(&(s->data[index]), (u32*)data, size);
}


static void LoadParasite()
{
    struct SaveSection* s = gFastSaveSection;
    u32 size = 0;
    u32* data = NULL;
    u32* parasiteP1 = (u32*)gSaveBlockParasite;
    u32* parasiteP2 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0]); //b240
    u32* parasiteP3 = (u32*)(gSaveBlockParasite + SaveBlockParasiteSizes[0] + SaveBlockParasiteSizes[1]); //b496
    
	switch (s->id) {
        case 0:
            data = parasiteP1;
            size = SaveBlockParasiteSizes[0];
            break;
        case 4:
            data = parasiteP2;
            size = SaveBlockParasiteSizes[1];
            break;
        case 13:
            data = parasiteP3;
            size = SaveBlockParasiteSizes[2];
            break;
        default:
            return;
    }

    u16 index = SECTOR_DATA_SIZE - size;
    Memcpy(data, &(s->data[index]), size);
}

// 080D9E54
u8 HandleLoadSector(unusedArg u16 a1, const struct SaveSectionLocation* location)
{
	u16 id;
    u16 sector = SECTOR_SAVE_SLOT_LENGTH * (gSaveCounter % 2);
    bool8 checksumStatus = FALSE;

    for (u8 i = 0; i < SECTOR_SAVE_SLOT_LENGTH; ++i)
	{
        DoReadFlashWholeSection(i + sector, gFastSaveSection);

        id = gFastSaveSection->id;
        if (id == 0)
            gLastWrittenSector = i;

        u16 checksum = CalculateSaveChecksum(gFastSaveSection->data, location[id].size);
        if (gFastSaveSection->security == FILE_SIGNATURE
		&&  gFastSaveSection->checksum == checksum)
		{
            Memcpy(location[id].data, gFastSaveSection->data, location[id].size);
            LoadParasite();
			checksumStatus = TRUE;
        }
    }

    //Start of the game don't load jibberish into block
    if (checksumStatus)
        LoadSector30And31();

    return 1;
}

// 080D9870
u8 HandleWriteSector(u16 chunkId, const struct SaveSectionLocation* location)
{
    u16 sector;
    u8* data;
    u16 size;

    //Get nonbackup sector slot
    sector = chunkId + gLastWrittenSector;
    sector %= SECTOR_SAVE_SLOT_LENGTH;
    sector += SECTOR_SAVE_SLOT_LENGTH * (gSaveCounter % 2);
	
    data = location[chunkId].data;
    size = location[chunkId].size;

	//Clear save section
	Memset(gFastSaveSection, 0, sizeof(struct SaveSection));

    gFastSaveSection->id = chunkId;
    gFastSaveSection->security = FILE_SIGNATURE;
    gFastSaveSection->counter = gSaveCounter;

    Memcpy(gFastSaveSection->data, data, size);

    gFastSaveSection->checksum = CalculateSaveChecksum(data, size);

    //Write data to leftover save section
    SaveParasite();
    u8 retVal = TryWriteSector(sector, gFastSaveSection->data);
    SaveSector30And31();
    return retVal;
}

static void CallSomething(u16 arg, EraseFlash func)
{
    func(arg);
}


u8 HandleSavingData(u8 saveType)
{
    u32* backupPtr = gMain.vblankCounter1;
    //u8 *tempAddr;
    gMain.vblankCounter1 = NULL;
    UpdateSaveAddresses();
    switch (saveType) {
        case SAVE_NORMAL: // normal save. also called by overwriting your own save.
        default:
        {
            SaveSerializedGame();
            SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
            break;
        }
        case SAVE_LINK: // _081532C4
        {
			SaveSerializedGame();
			for(u8 i = 0; i < 5; i++)
				SaveWriteToFlash(i, gRamSaveSectionLocations);
			break;
        }
		//case SAVE_EREADER:
			//Removed for more save space
		case SAVE_HALL_OF_FAME: // hall of fame.
			if (GetGameStat(GAME_STAT_ENTERED_HOF) < 999)
				IncrementGameStat(GAME_STAT_ENTERED_HOF);
			SaveSerializedGame();
			SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
			HandleWriteSectorNBytes(SECTOR_ID_HOF_1, gDecompressionBuffer, 0xF80);
			HandleWriteSectorNBytes(SECTOR_ID_HOF_2, gDecompressionBuffer + 0xF80, 0xF80);
			break;
	
        case SAVE_OVERWRITE_DIFFERENT_FILE:
        {
            for (u8 i = SECTOR_ID_HOF_1; i < SECTORS_COUNT; i++)
			{
                u32 *t = (u32*)0x03007430;
                EraseFlash EraseFlashSector = (EraseFlash)(*t);
                CallSomething(i, EraseFlashSector);
            }
            SaveSerializedGame();
            SaveWriteToFlash(0xFFFF, gRamSaveSectionLocations);
            break;
        }
    };
    gMain.vblankCounter1 = backupPtr;
    return 0;
}

void NewGameWipeNewSaveData(void)
{
	#ifdef UNBOUND
		Memset((void*) gSaveBlockParasite, 0, 0x2A8C);
	#else
		Memset((void*) gSaveBlockParasite, 0, 0x2EA4);
	#endif
}

u8* GetExpandedFlagPointer(u16 id)
{
	#ifdef SAVE_BLOCK_EXPANSION
		if (id >= 0x900 && id < 0x1900)
			return &gExpandedFlags[(id - 0x900) / 8];
		else if (id >= 0x1900)
			return (u8*) &Var8000; //Returns Var8000 to prevent bad memory access
		else
			return NULL;
	#else
		if (id >= 0x1900)
			return (u8*) &Var8000; //Returns Var8000 to prevent bad memory access
		else
			return NULL;
	#endif
}

u16* GetExpandedVarPointer(u16 id)
{
	#ifdef SAVE_BLOCK_EXPANSION
		if (id >= 0x5000 && id < 0x5200)
			return &gExpandedVars[id - 0x5000];
		else if (id >= VARS_START + 0x100 && id < SPECIAL_VARS_START)
			return (u16*) 1; //Indicates to return NULL
		else
			return NULL;
	#else
		if (id >= VARS_START + 0x100 && id < SPECIAL_VARS_START)
			return (u16*) 1; //Indicates to return NULL
		else
			return NULL;
	#endif	
}