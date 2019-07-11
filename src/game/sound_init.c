#include <ultra64.h>

#include "sm64.h"
#include "level_update.h"
#include "main.h"
#include "engine/math_util.h"
#include "area.h"
#include "profiler.h"
#include "audio/dma.h"
#include "audio/interface_2.h"
#include "print.h"
#include "save_file.h"
#include "sound_init.h"

Vec3f D_80339DC0;
OSMesgQueue gSoundMesgQueue;
OSMesg gSoundMesgBuf[1];
struct VblankHandler gSoundVblankHandler;

u8 D_8032C6C0 = 0;
u8 D_8032C6C4 = 0;
u16 D_8032C6C8 = 0xFFFF;
u16 D_8032C6CC = 0xFFFF;
u16 D_8032C6D0 = 0xFFFF;
u8 D_8032C6D4 = 0;
u8 unused8032C6D8[16] = {0};
s16 D_8032C6E8[] = {0, 3, 1, 0};
// I'm not sure what this is. Only the 20th array element is used.
u32 D_8032C6F0[] = {
    SOUND_UNKNOWN_UNK1400, SOUND_UNKNOWN_UNK1401, SOUND_UNKNOWN_UNK1402,
    SOUND_UNKNOWN_UNK1403, SOUND_UNKNOWN_UNK1404, SOUND_UNKNOWN_UNK1405,
    SOUND_UNKNOWN_UNK1406, SOUND_UNKNOWN_UNK1410, SOUND_UNKNOWN_UNK1412,
    SOUND_UNKNOWN_UNK1411, SOUND_UNKNOWN_UNK1414, SOUND_UNKNOWN_UNK1420,
    NO_SOUND, SOUND_ENVIRONMENT_BOATROCKING1, SOUND_ENVIRONMENT_ELEVATOR3,
    SOUND_ENVIRONMENT_UNKNOWN2, SOUND_ENVIRONMENT_WATERFALL1, SOUND_ENVIRONMENT_WATERFALL2,
    SOUND_ENVIRONMENT_ELEVATOR1, SOUND_ENVIRONMENT_DRONING1, SOUND_ENVIRONMENT_DRONING2,
    SOUND_ENVIRONMENT_ELEVATOR2, SOUND_ENVIRONMENT_WIND1, SOUND_ENVIRONMENT_WATER,
    SOUND_CH6_UNKNOWN000, SOUND_MOVING_UNKNOWN19, SOUND_CH6_UNKNOWN002,
    SOUND_CH6_UNKNOWN003, SOUND_CH6_UNKNOWN010, SOUND_CH9_UNK52,
    SOUND_CH8_UNK50, SOUND_OBJECT_BIRDS2, SOUND_ENVIRONMENT_ELEVATOR2,
    SOUND_CH6_UNKNOWN004_2, SOUND_CH6_UNKNOWN004, SOUND_ENVIRONMENT_ELEVATOR4
};
s8 D_8032C780 = 0;

void func_80248C10(void)
{
    D_8032C6C0 = 0;
}

void func_80248C28(s32 a)
{
    switch (a)
    {
    case 1:
        func_80320248(1);
        break;
    case 2:
        func_8031FFB4(0, 60, 40);
        break;
    }
    D_8032C6C0 |= a;
}

void func_80248CB8(s32 a)
{
    switch (a)
    {
    case 1:
        func_80320248(0);
        break;
    case 2:
        func_80320040(0, 60);
        break;
    }
    D_8032C6C0 &= ~a;
}

void func_80248D48(void)
{
    if (D_8032C6C4 == 0)
    {
        D_8032C6C4 = 1;
        func_803208C0(2, 890);
    }
}

void func_80248D90(void)
{
    if (D_8032C6C4 == 1)
    {
        D_8032C6C4 = 0;
        func_80320980(2, 890);
    }
}

void set_sound_mode(u16 mode)
{
    if (mode < 3)
        func_80321434(D_8032C6E8[mode]);
}

void func_80248E24(s16 a)
{
    if (a & 0x1)
        SetSound(SOUND_MENU_HANDAPPEAR, D_803320E0);
    else if (a & 0x2)
        SetSound(SOUND_MENU_HANDDISAPPEAR, D_803320E0);
    else if (a & 0x4)
        SetSound(SOUND_MENU_UNKNOWN1, D_803320E0);
    else if (a & 0x8)
        SetSound(SOUND_MENU_PINCHMARIOFACE, D_803320E0);
    else if (a & 0x10)
        SetSound(SOUND_MENU_PINCHMARIOFACE, D_803320E0);
    else if (a & 0x20)
        SetSound(SOUND_MENU_LETGOMARIOFACE, D_803320E0);
    else if (a & 0x40)
        SetSound(SOUND_MENU_CAMERAZOOMIN, D_803320E0);
    else if (a & 0x80)
        SetSound(SOUND_MENU_CAMERAZOOMOUT, D_803320E0);
    
    if (a & 0x100)
        func_80249464(20, NULL);
}

void func_80248FBC(void)
{
    if (D_8035FFA8 != NULL && D_8035FFA8->unk7 == 2)
    {
        if (D_8032C780 == 0)
            SetSound(SOUND_UNKNOWN_UNK3828, gMarioStates[0].marioObj->header.gfx.cameraToObject);
        D_8032C780 = 1;
    }
    else
    {
        D_8032C780 = 0;
    }
}

void func_80249040(void)
{
    u8 sp1f = FALSE;
    
    if (gCurrLevelNum == LEVEL_CASTLE && gCurrAreaIndex == 2 && gMarioState->numStars < 70)
    {
        if (gMarioState->floor != NULL && gMarioState->floor->room == 6)
        {
            if (gMarioState->pos[2] < 2540.0f)
                sp1f = TRUE;
        }
    }

    if (D_8032C6D4 ^ sp1f)
    {
        D_8032C6D4 = sp1f;
        if (sp1f)
            func_80320F84(24, 0, 255, 1000);
        else
            func_80321080(500);
    }
}

void func_80249148(u16 a, u16 b, s16 c)
{
    if (gResetTimer == 0 && b != D_8032C6C8)
    {
        if (gCurrCreditsEntry != 0)
            func_80321368(7);
        else
            func_80321368(a);
        
        if (gSaveFileDoesNotExist == 0 || b != 4)
        {
            func_80320AE8(0, b, c);
            D_8032C6C8 = b;
        }
    }
}

void func_802491FC(s16 a)
{
    func_803210D4(a);
    D_8032C6C8 = 0xFFFF;
    D_8032C6CC = 0xFFFF;
    D_8032C6D0 = 0xFFFF;
}

void func_8024924C(s16 a)
{
    func_8031F7CC(0, a);
    D_8032C6C8 = 0xFFFF;
    D_8032C6CC = 0xFFFF;
    D_8032C6D0 = 0xFFFF;
}

void func_802492A0(u16 a)
{
    func_80320AE8(0, a, 0);
    D_8032C6C8 = a;
}

void func_802492E0(void)
{
    func_80320AE8(0, (4 << 8) | 142, 0);
    D_8032C6CC = (4 << 8) | 142;
}

void func_8024931C(void)
{
    if (D_8032C6CC != 0xFFFF)
    {
        func_80320CE8(D_8032C6CC);
        D_8032C6CC = 0xFFFF;
    }
}

void func_80249368(u16 a)
{
    func_80320AE8(0, a, 0);
    if (D_8032C6D0 != 0xFFFF && D_8032C6D0 != a)
        func_80320CE8(D_8032C6D0);
    D_8032C6D0 = a;
}

void func_802493D4(void)
{
    if (D_8032C6D0 != 0xFFFF)
        func_80320E20(D_8032C6D0, 600);
}

void func_80249418(void)
{
    if (D_8032C6D0 != 0xFFFF)
    {
        func_80320CE8(D_8032C6D0);
        D_8032C6D0 = 0xFFFF;
    }
}

void func_80249464(s32 a, void *b)
{
    SetSound(D_8032C6F0[a], b);
}

void func_802494A8(void)
{
    func_8031EED0();
}

void thread4_sound(UNUSED void *arg)
{
    InitAudioSystem();
    func_803202A0();
    vec3f_copy(D_80339DC0, D_80385FD0);
    osCreateMesgQueue(&gSoundMesgQueue, gSoundMesgBuf, ARRAY_COUNT(gSoundMesgBuf));
    set_vblank_handler(1, &gSoundVblankHandler, &gSoundMesgQueue, (OSMesg)512);
    
    while (1)
    {
        OSMesg msg;

        osRecvMesg(&gSoundMesgQueue, &msg, OS_MESG_BLOCK);
        if (gResetTimer < 25)
        {
            struct SPTask *spTask;

            profiler_log_thread4_time();
            spTask = func_8031D924();
            if (spTask != NULL)
                dispatch_audio_sptask(spTask);
            profiler_log_thread4_time();
        }
    }
}
