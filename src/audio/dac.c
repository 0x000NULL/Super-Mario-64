#include <ultra64.h>

#include "sm64.h"
#include "dac.h"
#include "data.h"
#include "dma.h"
#include "playback.h"
#include "something.h"
#include "interface_1.h"
#include "interface_2.h"

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

struct Struct803161E0
{
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkC;
}; // size = 0x10

struct U32Pair
{
    u32 first;
    u32 second;
}; // size = 0x8

extern struct SoundAllocPool D_802212A8;
extern struct SoundAllocPool D_802212F8;
extern struct SoundAllocPool D_80221308;
extern struct SoundAllocPool D_80221318;

extern struct SoundMultiPool D_802216C8;

extern s32 D_80226D6C;

extern struct Struct803161E0 D_80221898;
extern struct U32Pair D_802218A8;
extern struct Struct803161E0 D_802218B0;
extern struct Struct803161E0 D_802218C0;

void func_80315E60(void)
{
    s32 i;

    for (i = 0; i < 64; i++)
    {
        D_802218D0[i] = 0;
    }

    for (i = 0; i < 256; i++)
    {
        D_80221910[i] = 0;
    }
}


void func_80315EA4(s32 arg0)
{
    s32 i;

    for (i = 0; i < D_80226D70; i++)
    {
        struct Struct_func_80318870 *s0 = &D_80222A10[i];

        if (s0->unk7 == arg0)
        {
            if (s0->unk4 > 1)
            {
                s0->unk2C->unk0b80 = 0;
                s0->unk2C->unk0b40 = 1;
            }
            func_80315DE0(s0);
            func_803195A4(&s0->unkA4);
            func_8031AEF4(&D_80225EA8[0], &s0->unkA4);
        }
    }
}


void func_80315F94(s32 arg0)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        if (D_80222A18[i].unk0b80 && D_80222A18[i].unk5 == arg0)
            func_8031AE24(D_80222A18 + i);
    }
}

#ifdef NON_MATCHING
void *soundAlloc(struct SoundAllocPool *pool, u32 size)
{
    // Has a bunch of register swaps: v1 -> a2/a0, a1 -> v1, a2 -> a1
    s32 last; // a2
    s32 i; // a3
    u8 *ret; // a1, v0
    s32 aligned; // t6

    ret = pool->unk4;
    aligned = ALIGN16(size);
    if (pool->unk4 + aligned <= pool->unk8 + pool->unk0)
    {
        pool->unk4 += aligned;
        last = pool->unk4 - ret - 1;
        for (i = 0; i <= last; i++)
        {
            ret[i] = 0;
        }
    }
    else
    {
        return NULL;
    }

    return ret;
}

#else
GLOBAL_ASM("asm/non_matchings/soundAlloc.s")
#endif

void func_80316094(struct SoundAllocPool *arg0, void *arg1, u32 arg2)
{
    arg0->unk4 = arg0->unk0 = (u8 *)(((u32) arg1 + 0xf) & -0x10);
    arg0->unk8 = arg2;
    arg0->unkC = 0;
}

void func_803160B4(struct SoundPoolHolder *arg0)
{
    arg0->pool.unkC = 0;
    arg0->unk0 = 0;
    arg0->pool.unk4 = arg0->pool.unk0;
}

void func_803160C8(struct SoundPoolHolder2 *arg0)
{
    arg0->pool.unkC = 0;
    arg0->unk0 = 0;
    arg0->arr[1].unk0 = arg0->pool.unk8 + arg0->pool.unk0;
    arg0->arr[0].unk8 = -1;
    arg0->arr[1].unk8 = -1;
    arg0->pool.unk4 = arg0->pool.unk0;
    arg0->arr[0].unk0 = arg0->pool.unk0;
}

void unused_803160F8(struct SoundAllocPool *arg0)
{
    arg0->unkC = 0;
    arg0->unk4 = arg0->unk0;
}

void func_80316108(s32 arg0)
{
    func_80316094(&D_802212B8, gAudioHeap, arg0);
    func_80316094(&D_802212A8, gAudioHeap + arg0, D_80333EEC - arg0);
}

void func_80316164(struct Struct803161E0 *a)
{
    D_802212A8.unk4 = D_802212A8.unk0;
    func_80316094(&D_802212C8, soundAlloc(&D_802212A8, a->unk0), a->unk0);
    func_80316094(&D_802212F8, soundAlloc(&D_802212A8, a->unkC), a->unkC);
}

void func_803161E0(struct U32Pair *a)
{
    D_802212F8.unk4 = D_802212F8.unk0;
    func_80316094(&D_80221308, soundAlloc(&D_802212F8, a->first), a->first);
    func_80316094(&D_80221318, soundAlloc(&D_802212F8, a->second), a->second);
}

void func_8031625C(struct Struct803161E0 *a)
{
    D_80221308.unk4 = D_80221308.unk0;
    func_80316094(&D_80221328.first.pool, soundAlloc(&D_80221308, a->unk0), a->unk0);
    func_80316094(&D_802214F8.first.pool, soundAlloc(&D_80221308, a->unk4), a->unk4);
    func_80316094(&D_802216C8.first.pool, soundAlloc(&D_80221308, a->unk8), a->unk8);
    func_803160B4(&D_80221328.first);
    func_803160B4(&D_802214F8.first);
    func_803160B4(&D_802216C8.first);
}

void func_80316318(struct Struct803161E0 *a)
{
    D_80221318.unk4 = D_80221318.unk0;
    func_80316094(&D_80221328.second.pool, soundAlloc(&D_80221318, a->unk0), a->unk0);
    func_80316094(&D_802214F8.second.pool, soundAlloc(&D_80221318, a->unk4), a->unk4);
    func_80316094(&D_802216C8.second.pool, soundAlloc(&D_80221318, a->unk8), a->unk8);
    func_803160C8(&D_80221328.second);
    func_803160C8(&D_802214F8.second);
    func_803160C8(&D_802216C8.second);
}

static void unused_803163D4()
{
}

#ifdef NON_MATCHING
void *func_803163DC(struct SoundMultiPool *arg0, s32 count, s32 size, s32 arg3, s32 arg4)
{
    // arg3 = 0, 1 or 2?
    // arg4 = some s32?

    u8 *table; // sp5C
    u8 index; // sp5B
    struct SoundAllocPool *pool;
    void *ret;
    u32 firstVal;
    u32 secondVal;
    u32 both3;
    u32 first3, second3;
    u32 first0, second0;
    u32 firstN1, secondN1;
    UNUSED s32 temp;
    struct SoundPoolHolder2 *v1; // sp30
    struct SoundPoolHolder *first = &arg0->first;

    if (arg3 == 0)
    {
        v1 = &arg0->second;
        if (arg0 == &D_80221328)
        {
            table = D_80221910;
            index = 0;
        }
        else if (arg0 == &D_802214F8)
        {
            table = D_802218D0;
            index = 1;
        }

        firstVal = (v1->arr[0].unk8 == -1 ? 0 : table[v1->arr[0].unk8]); // a3, a2
        secondVal = (v1->arr[1].unk8 == -1 ? 0 : table[v1->arr[1].unk8]); // a1
        first0 = (firstVal == 0);
        first3 = (firstVal == 3); // t0
        firstN1 = (firstVal != 1);
        second0 = (secondVal == 0);
        second3 = (secondVal == 3);
        secondN1 = (secondVal != 1);
        both3 = (first3 && second3); // a0

        if (first0)
        {
            v1->unk0 = 0;
        }
        else if (second0)
        {
            v1->unk0 = 1;
        }
        else if (both3)
        {
            // either is fine, maybe?
        }
        else if (first3)
        {
            v1->unk0 = 0;
        }
        else if (second3)
        {
            v1->unk0 = 1;
        }
        else if (firstN1)
        {
            v1->unk0 = 0;
        }
        else if (secondN1)
        {
            v1->unk0 = 1;
        }
        else
        {
            return NULL;
        }

        if (v1->arr[v1->unk0].unk8 != -1)
        {
            table[v1->arr[v1->unk0].unk8] = 0;
            if (index == 1)
            {
                func_80315EA4(v1->arr[v1->unk0].unk8);
            }
        }

        pool = &arg0->second.pool; // a1
        switch (v1->unk0)
        {
        case 0:
            v1->arr[0].unk0 = pool->unk0;
            v1->arr[0].unk8 = arg4;
            v1->arr[0].unk4 = size;

            pool->unk4 = pool->unk0 + size;

            if (v1->arr[1].unk0 < pool->unk4)
            {
                table[v1->arr[1].unk8] = 0;

                switch (index)
                {
                case 0:
                    func_80315F94(v1->arr[1].unk8);
                    break;
                case 1:
                    func_80315EA4(v1->arr[1].unk8);
                    break;
                }

                v1->arr[1].unk8 = -1;
                v1->arr[1].unk0 = pool->unk8 + pool->unk0;
            }

            ret = v1->arr[0].unk0;
            break;

        case 1:
            v1->arr[1].unk0 = pool->unk8 + pool->unk0 - size - 0x10;
            v1->arr[1].unk8 = arg4;
            v1->arr[1].unk4 = size;

            if (v1->arr[1].unk0 < pool->unk4)
            {
                table[v1->arr[0].unk8] = 0;

                switch (index)
                {
                case 0:
                    func_80315F94(v1->arr[0].unk8);
                    break;
                case 1:
                    func_80315EA4(v1->arr[0].unk8);
                    break;
                }

                v1->arr[0].unk8 = -1;
                pool->unk4 = pool->unk0;
            }

            ret = v1->arr[1].unk0;
            break;

        default:
            return NULL;
        }

        v1->unk0 ^= 1;
        return ret;
    }

    arg0->arr[first->unk0].unk0 = soundAlloc(&first->pool, count * size);

    if (arg0->arr[first->unk0].unk0 == 0)
    {
        if (arg3 != 1)
        {
            if (arg3 == 2)
            {
                // Prevent tail call optimization.
                ret = func_803163DC(arg0, count, size, 0, arg4);
                return ret;
            }
        }
        else
        {
            return NULL;
        }
    }

    arg0->arr[first->unk0].unk8 = arg4;
    arg0->arr[first->unk0].unk4 = size;
    first->unk0++; return arg0->arr[first->unk0 - 1].unk0;
}

#else
GLOBAL_ASM("asm/non_matchings/func_803163DC.s")
#endif

void *func_8031680C(struct SoundMultiPool *arg0, s32 arg1, s32 arg2)
{
    u32 i;
    void *ret;
    struct SoundPoolHolder2 *second = &arg0->second;

    if (arg1 == 0)
    {
        if (second->arr[0].unk8 == arg2)
        {
            second->unk0 = 1;
            return second->arr[0].unk0;
        }
        else if (second->arr[1].unk8 == arg2)
        {
            second->unk0 = 0;
            return second->arr[1].unk0;
        }
        return NULL;
    }
    else
    {
        struct SoundPoolHolder *first = &arg0->first;
        for (i = 0; i < first->unk0; i++)
        {
            if (arg2 == arg0->arr[i].unk8)
            {
                return arg0->arr[i].unk0;
            }
        }

        if (arg1 == 2)
        {
            // Prevent tail call optimization by using a temporary.
            // (Did they compile with -Wo,-notail?)
            ret = func_8031680C(arg0, 0, arg2);
            return ret;
        }
        return NULL;
    }
}

void func_803168CC(void)
{
    D_802211B0.unk4 -= D_802211B0.unk4 / 4;
}

void func_803168F4(s32 frames)
{
    // Sleep for a number of frames.
    D_80226D80 = 0;
    while (D_80226D80 < frames) { /* spin */ }
}

#ifdef NON_MATCHING
void func_80316928(struct Struct80332190 *arg0)
{
    // Wrong regalloc, and a lui which is too far up.
    s32 sp2C;
    s16 *mem;
    s32 i; // s0
    s32 j;
    s32 k;
    s32 sum1;
    s32 sum2;
    s32 sum3;
    s32 sum4;
    s32 s1;
    s8 temp8;
    s32 size;
    UNUSED s32 temp;

    if (D_80333EF4 != 0)
    {
        func_803168CC();
        for (i = 0; i < D_80226D70; i++)
        {
            if (D_80222A10[i].unk0b80 && D_80222A10[i].unk54.unk01)
            {
                D_80222A10[i].unk54.unk0E = 0x8000 / D_80226D7E;
                D_80222A10[i].unk54.unk00 |= 0x10;
            }
        }

        s1 = 0;
        for (;;)
        {
            func_803168F4(1);
            s1++;
            if (s1 > 8 * 30)
            {
                // Break after 8 seconds
                break;
            }

            for (i = 0; i < D_80226D70; i++)
            {
                if (D_80222A10[i].unk0b80)
                    break;
            }

            if (i == D_80226D70)
            {
                // All zero, break early
                break;
            }
        }

        func_803168CC();
        func_803168F4(3);
        D_80333EF4 = 0x19710515;
        func_803168F4(3);

        s1 = D_80226D84;
        while (s1 > 0)
        {
            for (i = 0; i < D_80226D84; i++)
            {
                if (osRecvMesg(&D_80225EE8, NULL, OS_MESG_NOBLOCK) == 0)
                    s1--;
            }
        }
        D_80226D84 = 0;

        for (j = 0; j < 3; j++)
        {
            for (k = 0; k < 0x500; k++)
            {
                D_80226E40[j][k] = 0;
            }
        }
    }

    D_80226B38 = 0;
    sp2C = arg0->size;
    D_80226D64 = osAiSetFrequency(arg0->frequency);
    D_80226D70 = arg0->unk4;
    size = D_80226D64 / 60;
    D_80226D74 = ALIGN16(size);
    D_802212A2 = arg0->unk5;

    switch (D_802212A2)
    {
    case 1:
        D_802212A3 = 0;
        break;
    case 2:
        D_802212A3 = 1;
        break;
    case 4:
        D_802212A3 = 2;
        break;
    case 8:
        D_802212A3 = 3;
        break;
    case 16:
        D_802212A3 = 4;
        break;
    default:
        D_802212A3 = 0;
    }

    D_802212A2 = arg0->unk5;
    D_802212A0 = arg0->unkA;
    D_80226D78 = D_80226D74 - 0x10;
    temp8 = D_80226D74 / 160 + 1;
    D_80226D7E = temp8;
#ifdef VERSION_JP
    D_80226D7C = temp8 * 3600 / D_80333EE4;
#else
    D_80226D7C = (u32) (temp8 * 2880000.0f / D_80333EE4 / 16.713f);
#endif
    D_80226D6C = D_80226D70 * 20 * temp8 + 320;
    sum1 = arg0->unk10 + arg0->unkC;
    sum2 = arg0->unk18 + arg0->unk14;
    sum3 = sum1 + sum2;
    // (the address of D_802212A8.unk8 is lui'd too far up)
    sum4 = D_802212A8.unk8 - sum3 - 0x100;
    D_80221898.unk0 = sum4;
    D_80221898.unkC = sum3;
    func_80316164(&D_80221898);
    D_802218A8.first = sum1;
    D_802218A8.second = sum2;
    func_803161E0(&D_802218A8);
    D_802218B0.unk0 = arg0->unkC;
    D_802218B0.unk4 = arg0->unk10;
    D_802218B0.unk8 = 0;
    func_8031625C(&D_802218B0);
    D_802218C0.unk0 = arg0->unk14;
    D_802218C0.unk4 = arg0->unk18;
    D_802218C0.unk8 = 0;
    func_80316318(&D_802218C0);
    func_80315E60();

    for (j = 0; j < 2; j++)
    {
        D_80226D90[j] = soundAlloc(&D_802212C8, D_80226D6C * 8);
    }

    D_80222A10 = soundAlloc(&D_802212C8,
            D_80226D70 * sizeof(struct Struct_func_80318870));
    func_80319D40();
    func_80319248();

    if (sp2C == 0)
    {
        D_802211B0.unk1 = 0;
    }
    else
    {
        D_802211B0.unk1 = 8;
        D_802211B0.unk14.unk00 = soundAlloc(&D_802212C8, sp2C * 2);
        D_802211B0.unk14.unk04 = soundAlloc(&D_802212C8, sp2C * 2);
        D_802211B0.unk8 = 0;
        D_802211B0.unkC = 0;
        D_802211B0.unk3 = 0;
        D_802211B0.unk10 = sp2C;
        D_802211B0.unk4 = arg0->unk8;
        D_802211B0.unk2 = 2;
        if (D_802212A2 != 1)
        {
            D_802211B0.unk0 = 1;
            D_802211B0.unk6 = 0x8000 / D_802212A2;
            D_802211B0.unk1C = soundAlloc(&D_802212C8, 32);
            D_802211B0.unk20 = soundAlloc(&D_802212C8, 32);
            D_802211B0.unk24 = soundAlloc(&D_802212C8, 32);
            D_802211B0.unk28 = soundAlloc(&D_802212C8, 32);
            for (i = 0; i < D_80226D7E; i++)
            {
                mem = soundAlloc(&D_802212C8, 0x280);
                D_802211B0.unk2C[0][i].unk4 = mem;
                D_802211B0.unk2C[0][i].unk8 = mem + 0xA0;
                mem = soundAlloc(&D_802212C8, 0x280);
                D_802211B0.unk2C[1][i].unk4 = mem;
                D_802211B0.unk2C[1][i].unk8 = mem + 0xA0;
            }
        }
    }

    func_8031758C(D_80226D70);
    osWritebackDCacheAll();
    if (D_80333EF4 != 0)
    {
        D_80333EF4 = 0x76557364;
    }
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_80316928_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_80316928_us.s")
#endif
