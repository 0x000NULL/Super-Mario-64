#ifndef _AUDIO_DMA_H
#define _AUDIO_DMA_H

#include "types.h"
#include "playback.h"

struct Struct80226538
{
    u8 *unk0; // virtual address
    u32 unk4; // device address
    u16 unk8; // dma transfer size
    u16 unkA; // also dma transfer size?
    u8 unkC;
    u8 unkD; // index into D_80226C48
    u8 unkE; // counter, gets decremented
}; // size = 0x10

struct SubSubEntry
{
    s32 unk0;
    s32 unk4;
    s32 unk8; // compared against 0
};

struct SubEntry
{
    u8 unk0;
    u8 unk1;
    u8 *unk4; // points into offset
    struct SubSubEntry *unk8; // points into mem
    ALADPCMBook *unkC; // points into mem
};

struct SubEntryAndF32
{
    struct SubEntry *unk0;
    f32 unk4;
};

struct SubA
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    struct SubEntryAndF32 unk4;
    struct SubstructInstrumentSomething *unkC; // points into mem
};

struct InstrumentSomething
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 unk3;
    struct SubstructInstrumentSomething *unk4;
    struct SubEntryAndF32 unk8;
    struct SubEntryAndF32 unk10;
    struct SubEntryAndF32 unk18;
};

struct Struct_func_8031784C
{
    struct SubA **header;
    struct InstrumentSomething *subB[1];
};

struct Struct_80226D60
{
    u8 unk0;
    u8 unk1;
    u8 unk2;
    struct InstrumentSomething **unk4;
    struct SubA **unk8;
}; // size = 0xC

extern u64 D_802211A0; // audio .stack data start marker
extern OSMesgQueue D_80225EE8;
extern OSMesg D_80225F00[0x40];
extern OSIoMesg D_80226000[0x40];
extern OSMesgQueue D_80226500;
extern OSMesg D_80226518[1];
extern OSIoMesg D_80226520;
extern struct Struct80226538 D_80226538[96];
extern u32 D_80226B3C;
extern u32 D_80226B40;
extern u8 D_80226B48[256];
extern u8 D_80226C48[256];
extern u8 D_80226D48;
extern u8 D_80226D49;
extern u8 D_80226D4A;
extern u8 D_80226D4B;
extern ALSeqFile *D_80226D4C;
extern ALSeqFile *D_80226D50;
extern ALSeqFile *D_80226D54;
extern u8 *D_80226D58;
extern u16 D_80226D5C;
extern struct Struct_80226D60 *D_80226D60; // array of size D_80226D50->seqCount
extern u32 D_80226D68;
extern volatile s32 D_80226D80;
extern s32 D_80226D88;
extern s32 D_80226D8C;
// volatile according to func_8031D924, but not to InitAudioSystem.
// Maybe incorrectly extern'd in dma.c?
extern struct SPTask *D_80226D9C;
extern struct SPTask D_80226DA0[2];
extern u16 *D_80226E40[3];
extern s16 D_80226E4C[3];
extern u16 D_80226E52[];
extern u32 D_80226E58[];
extern u16 D_80226E98[];
extern u64 D_80226EC0; // audio .stack data end marker

extern s8 D_80333EE8;
extern s32 D_80333EF0; // amount of heap designated to D_802212B8, 0x2500
extern volatile s32 D_80333EF4;

extern u8 gMusicData[];
extern u8 gSoundDataADSR[];
extern u8 gSoundDataRaw[];
extern u8 gInstrumentSets[];

void BlockDmaCopy(u32 devAddr, void *vAddr, u32 nbytes);
void func_80317034(u32 devAddr, void *vAddr, u32 nbytes, OSMesgQueue *queue, OSIoMesg *mesg);
void func_803170A0(u32 *devAddr, u8 **vAddr, s32 *size, OSMesgQueue *queue, OSIoMesg *mesg);
void func_8031715C(void);
void *func_80317270(u8 *arg0, u32 arg1, s32 arg2, u8 *arg3);
void func_8031758C(s32 arg0);
void func_8031784C(struct Struct_func_8031784C *arg0, u8 *offset, u32 arg2, u32 arg3);
void *func_80317A88(s32 arg0, s32 arg1);
void *func_80317BE4(s32 arg0, s32 arg1, struct Struct80222A18 *arg2);
void *func_80317D1C(s32 arg0, s32 arg1);
void *func_80317DC8(s32 arg0, s32 arg1, struct Struct80222A18 *arg2);
u8 func_80317F04(u32 arg0, s32 *arg1, s32 *arg2);
void *func_8031804C(s32 arg0, u8 *arg1);
void func_80318178(u32 arg0, u8 arg1);
void func_80318280(u32 arg0, u32 arg1, s32 arg2);
void func_803182E0(u32 arg0, u32 arg1, s32 arg2);
void InitAudioSystem(void);

#endif /* _AUDIO_DMA_H */
