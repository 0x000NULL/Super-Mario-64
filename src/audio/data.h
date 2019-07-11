#ifndef _AUDIO_DATA_H
#define _AUDIO_DATA_H

#include <ultra64.h>

#include "playback.h"

struct Struct80332190
{
    /*0x00*/ u32 frequency; // 32000, 27000
    /*0x04*/ u8 unk4; // stored in D_80226D70, 16 (mostly), 20, 14, 12, 10, 8 
    /*0x05*/ u8 unk5; // stored in D_802212A2, always 1
    /*0x06*/ u16 size; // sent as param to soundAlloc
    /*0x08*/ u16 unk8; // unknown, stored to D_802211B0.4
    /*0x0A*/ u16 unkA; // stored in D_802212A0
    /*0x0C*/ u32 unkC; // added with unk10
    /*0x10*/ u32 unk10;
    /*0x14*/ u32 unk14; // added with unk18
    /*0x18*/ u32 unk18;
}; // size = 0x1C

extern struct Struct80332190 D_80332190[18];
extern u16 D_80332388[128]; // unused

extern f32 D_80332488[255];
extern f32 D_80332884[128];

extern u8 D_80332A84[16];
extern u8 D_80332A94[16];
extern s8 D_80332AA4[16];
extern struct SubstructInstrumentSomething D_80332AB4[3];

extern s16 *gWaveSamples[4];

extern u16 D_80332CD0[10];
extern f32 D_80332CE4[128];
extern f32 D_80332EE4[128];
extern f32 D_803330E4[128];

extern f32 D_803332E4[128];
extern f32 D_803334E4[128];
extern f32 D_803336E4[128];
extern f32 D_803338E4[128];
extern f32 D_80333AE4[128];
extern f32 D_80333CE4[128];

extern s16 D_80333EE4;
extern s8 D_80333EE8;
extern s32 D_80333EEC;
extern s32 D_80333EF0;
extern volatile s32 D_80333EF4;

#endif /* _AUDIO_DATA_H */
