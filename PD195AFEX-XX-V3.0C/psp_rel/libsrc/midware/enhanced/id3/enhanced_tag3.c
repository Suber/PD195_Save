/********************************************************************************
 *                              USDK 5102
 *                            Module: POWER MANAGER
 *                 Copyright(c) 2001-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *    wuyueqian     2009-9-07 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * file
 * brief
 * Author   wuyueqian
 * par
 *
 *      Copyright(c) 2001-2007 Actions Semiconductor, All Rights Reserved.
 *
 * version 1.0
 * date  2008/6/12
 *******************************************************************************/

#include <actos.h>
#include <enhanced.h>
#include <stringH.h>

#pragma name(MW_EH_ID33)

#define ID3_BUFFER 0x0600//0x1800
uint8 far reserveFarData3;
const uint8 reserveHCData3 = 0;

#pragma renamecode(MW_EH_ID33)
void reserveFunID33(void)
{
}


  
const uint16 TabAddr[] = 
{
    0, 5, 16, 23, 28, 33, 37, 43, 50, 54, 
    59, 65, 71, 76, 79, 82, 85, 91, 95, 101, 
    111, 122, 125, 135, 141, 151, 162, 169, 177, 182, 
    191, 197, 203, 212, 224, 228, 233, 237, 246, 252, 
    257, 267, 271, 275, 279, 284, 294, 309, 325, 331, 
    337, 345, 362, 372, 380, 389, 394, 406, 412, 416, 
    423, 428, 440, 448, 454, 468, 475, 482, 493, 497, 
    506, 513, 518, 524, 532, 540, 545, 550, 557, 566, 
    574, 578, 587, 599, 604, 614, 619, 624, 631, 637, 
    646, 656, 666, 680, 695, 708, 716, 723, 729, 742, 
    750, 756, 762, 769, 774, 786, 792, 800, 809, 815, 
    825, 831, 838, 842, 847, 852, 860, 866, 877, 889, 
    898, 902, 910, 918, 926, 936, 945, 948, 967, 975, 
    981, 986, 993, 1002, 1011, 1015, 1034, 1044, 1054, 1063, 
    1084, 1097, 1105, 1110, 1120, 1125, 1129, 1137
};
const uint8 genre_tab[]="BluesClassicRockCountryDanceDiscoFunkGrungeHip-HopJazz\
MetalNewAgeOldiesOtherPopR&BRapReggaeRockTechno\
IndustrialAlternativeSkaDeathMetalPranksSoundtrackEuro-TechnoAmbientTrip-HopVocal\
Jazz+FunkFusionTranceClassicalInstrumentalAcidHouseGameSoundClipGospel\
NoiseAlternRockBassSoulPunkSpaceMeditativeInstrumentalPopInstrumentalRockEthnic\
GothicDarkwaveTechno-IndustrialElectronicPop-FolkEurodanceDreamSouthernRockComedyCult\
GangstaTop40ChristianRapPop/FunkJungleNativeAmericanCabaretNewWavePsychadelicRave\
ShowtunesTrailerLo-FiTribalAcidPunkAcidJazzPolkaRetroMusicalRock&Roll\
HardRockFolkFolk-RockNationalFolkSwingFastFusionBebobLatinRevivalCeltic\
BluegrassAvantgardeGothicRockProgessiveRockPsychedelicRockSymphonicRockSlowRockBigBandChorusEasyListening\
AcousticHumourSpeechChansonOperaChamberMusicSonataSymphonyBootyBassPrimus\
PornGrooveSatireSlowJamClubTangoSambaFolkloreBalladPowerBalladRhythmicSoul\
FreestyleDuetPunkRockDrumSoloAcapellaEuro-HouseDanceHallGoaDrum&BassClub-HouseHardcore\
TerrorIndieBritPopNegerpunkPolskPunkBeatChristianGangstaRapHeavyMetalBlackMetalCrossover\
ContemporaryChristianChristianRockMerengueSalsaTrashMetalAnimeJPopSynthpop";


bool SelectGENRE(ID3Info_t *ID3Info,BYTE GENREindex)
{
    uint8 *pSaveaddr;
    int savelen;
    if(GENREindex>147)
    {
        ID3Info->GENRE_buffer[0]=0x0;
        return FALSE;
    }
    
    pSaveaddr = (uint8 *)(genre_tab+TabAddr[GENREindex]);
    savelen = TabAddr[GENREindex+1]-TabAddr[GENREindex];
    if(savelen >(ID3Info->GENRE_length-1))
    {
        savelen = ID3Info->GENRE_length-1;
    }
    memcpy(ID3Info->GENRE_buffer,pSaveaddr,savelen);
    ID3Info->GENRE_buffer[savelen] = '\0';     //ÕâÀï²¹×Ö·û´®½áÊø
    return TRUE;
}

