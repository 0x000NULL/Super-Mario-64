#ifndef _AUDIO_DAC_H
#define _AUDIO_DAC_H

#include "types.h"
#include "dma.h"
#include "data.h"

struct SoundAllocPool
{
    u8 *unk0; // start
    u8 *unk4; // current position
    u32 unk8; // size
    s32 unkC; // unknown type, set to zero
}; // size = 0x10

struct MultiPoolSub {
    u8 *unk0;
    u32 unk4; // size
    s32 unk8; // small integer
}; // size = 0xC

struct SoundPoolHolder
{
    u32 unk0;
    struct SoundAllocPool pool;
}; // size = 0x14

struct SoundPoolHolder2
{
    /*0x00*/ u32 unk0;
    /*0x04*/ struct SoundAllocPool pool;
    /*0x14*/ struct MultiPoolSub arr[2];
    /*0x2C*/ u32 pad2[4];
}; // size = 0x3C

struct SoundMultiPool
{
    /*0x000*/ struct SoundPoolHolder first;
    /*0x014*/ struct MultiPoolSub arr[32];
    /*0x194*/ struct SoundPoolHolder2 second;
}; // size = 0x1D0

// should probably live in something.h
struct struct_3920_sp1c
{
    s16 unk00;
    s16 unk02;
    s16 *unk4;
    s16 *unk8;
    s32 unkC;
    s16 unk10;
    s16 unk12;
}; // size = 0x14

struct Struct802211B0
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    u16 unk4;
    u16 unk6;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    struct
    {
        s16 *unk00;
        s16 *unk04;
    } unk14;
    void *unk1C;
    void *unk20;
    void *unk24;
    void *unk28;
    struct struct_3920_sp1c unk2C[2][4];
}; // 0xCC <= size <= 0x100
extern struct Struct802211B0 D_802211B0;

extern u8 gAudioHeap[];
extern s16 D_802212A0;
extern s8 D_802212A2;
extern u8 D_802212A3;
extern struct SoundAllocPool D_802212B8;
extern struct SoundAllocPool D_802212C8;
extern struct SoundMultiPool D_80221328;
extern struct SoundMultiPool D_802214F8;
extern u8 D_802218D0[64];
extern u8 D_80221910[256];
extern u32 D_80226B38;
extern s32 D_80226D64;
extern s32 D_80226D70;
extern s32 D_80226D74;
extern s32 D_80226D78;
extern s16 D_80226D7C;
extern s8 D_80226D7E;
extern s8 D_80226D7F;
extern volatile s32 D_80226D84; // counter of some sort
extern u64 *D_80226D90[2];
extern u64 *D_80226D98; // audio ucode data
extern s32 D_80333EEC; // size of audio heap

extern void *soundAlloc(struct SoundAllocPool *pool, u32 size);
extern void func_80316108(s32 arg0);
extern void *func_803163DC(struct SoundMultiPool *arg0, s32 arg1, s32 alloc, s32 arg3, s32 arg4);
extern void *func_8031680C(struct SoundMultiPool *arg0, s32 arg1, s32 arg2);
extern void func_80316928(struct Struct80332190 *arg0);

#endif /* _AUDIO_DAC_H */
