#include <ultra64.h>
#include <unused.h>
#include "dac.h"
#include "data.h"
#include "dma.h"
#include "interface_1.h"
#include "interface_2.h"
#include "playback.h"
#include "something.h"

#define aSetLoadBufferPair(pkt, c, off)                    \
    aSetBuffer(pkt++, 0, c + 0x740, 0, 320 - c);           \
    aLoadBuffer(pkt++, FIX(&D_802211B0.unk14.unk00[off])); \
    aSetBuffer(pkt++, 0, c + 0x880, 0, 320 - c);           \
    aLoadBuffer(pkt++, FIX(&D_802211B0.unk14.unk04[off]));

#define ALIGN(val, amnt) (((val) + (1 << amnt) - 1) & ~((1 << amnt) - 1))
#define FIX(a) (u16 *)((u8 *)(a) + 0x80000000U)

#ifdef VERSION_JP
#define US_FLOAT(x) x
#else
#define US_FLOAT(x) x##f
#endif

#ifdef __GNUC__
#define GLOBAL_ASM(...)
#endif

u64 *func_80313E54(u16 *arg0, s32 arg1, u64 *arg2, u32 arg3);
u64 *func_80314480(u16 *arg0, s32 arg1, u64 *arg2);
u64 *func_80314F08(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2);
u64 *func_80314FD4(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, u16 arg4, u32 arg5);
u64 *func_80315030(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, s32 arg4, u32 arg5);
u64 *func_80315094(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, s32 arg4, u16 *arg5);
u64 *func_803155F4(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, s32 arg3, s32 arg4);

static void func_80313920(s32 arg0, u32 arg1)
{
    struct struct_3920_sp1c *sp1c;
    s32 a1;
    s32 v1;
    s32 v0;
    s32 v1_2;
    s32 a0;
    if (D_802212A2 != 1)
    {
        if (D_802211B0.unk2 == 0)
        {
            sp1c = &D_802211B0.unk2C[D_802211B0.unk3][arg1];
            osInvalDCache(sp1c->unk4, 640);
            for (a1 = 0, v1 = 0; v1 < sp1c->unk10 / 2; a1 += D_802212A2, v1++)
            {
                D_802211B0.unk14.unk00[v1 + sp1c->unkC] = sp1c->unk4[a1];
                D_802211B0.unk14.unk04[v1 + sp1c->unkC] = sp1c->unk8[a1];
            }
            for (v1 = 0; v1 < sp1c->unk12 / 2; a1 += D_802212A2, v1++)
            {
                D_802211B0.unk14.unk00[v1] = sp1c->unk4[a1];
                D_802211B0.unk14.unk04[v1] = sp1c->unk8[a1];
            }
        }
    }
    sp1c = &D_802211B0.unk2C[D_802211B0.unk3][arg1];

    v1_2 = v0 = arg0 / D_802212A2;
    if (((v0 + D_802211B0.unk8) - D_802211B0.unk10) < 0)
    {
        sp1c->unk10 = v0 * 2;
        sp1c->unk12 = 0;
        sp1c->unkC = (s32)D_802211B0.unk8;
        D_802211B0.unk8 += v0;
    }
    else
    {
        a0 = (v0 + D_802211B0.unk8) - D_802211B0.unk10;
        v0 = v1_2 - a0;
        sp1c->unk10 = (v0)*2;
        sp1c->unk12 = a0 * 2;
        sp1c->unkC = D_802211B0.unk8;
        D_802211B0.unk8 = a0;
    }
    sp1c->unk00 = v1_2;
    sp1c->unk02 = arg0;
}

// volume related
static s32 func_80313BB0(u16 arg0, u16 arg1, s32 arg2)
{
    f32 ret;
    switch (arg2)
    {
    default:
        ret = D_803332E4[arg1 >> 8] * D_803334E4[arg0 >> 8];
        break;
    case 128:
        ret = D_80333AE4[arg1 >> 8] * D_80333CE4[arg0 >> 8];
        break;
    case 136:
        ret = D_803332E4[arg1 >> 8] * D_803334E4[arg0 >> 8];
        break;
    case 144:
        ret = D_803336E4[arg1 >> 8] * D_803338E4[arg0 >> 8];
        break;
    }
    return ret;
}

u64 *func_80313CD4(u64 *arg0, s32 *arg1, u16 *arg2, s32 arg3)
{
    s32 s0;
    s32 i;
    s32 s2 = arg3;
    s32 s3;
    u32 *s4 = (u32 *)arg2;
    u64 *s6 = arg0;
    s32 v0;

    aSegment(s6++, 0, 0);

    for (i = D_80226D7E; i > 0; i--)
    {
        s3 = i - 1;
        if (i == 1)
        {
            s0 = s2;
        }
        else
        {
            v0 = s2 / i;
            s0 = v0 - (v0 & 7);

            if ((v0 & 7) >= 4)
                s0 += 8;
        }
        func_8031D384(s3);
        if (D_802211B0.unk1 != 0)
        {
            func_80313920(s0, D_80226D7E - i);
        }
        s6 = func_80313E54((u16 *)s4, s0, s6, D_80226D7E - i);
        s2 -= s0;
        s4 += s0;
    }
    if (D_802211B0.unk2 != 0)
        D_802211B0.unk2--;
    D_802211B0.unk3 ^= 1;
    arg1[0] = s6 - arg0;
    return s6;
}

u64 *func_80313E54(u16 *arg0, s32 arg1, u64 *arg2, u32 arg3)
{
    UNUSED s32 pad1[1];
    s16 ra;
    s16 t4;
    UNUSED s32 pad[2];
    struct struct_3920_sp1c *v1;
    UNUSED s32 pad2[2];

    v1 = &D_802211B0.unk2C[D_802211B0.unk3][arg3];

    if (D_802211B0.unk1 == 0)
    {

        aClearBuffer(arg2++, 0x4c0, 640);
        arg2 = func_80314480(arg0, arg1, arg2);
    }
    else
    {
        if (D_802212A2 == 1)
        {
            aSetLoadBufferPair(arg2, 0, v1->unkC);
            if (v1->unk12 != 0)
            {
                aSetLoadBufferPair(arg2, v1->unk10, 0);
            }
            aDMEMMove(arg2++, 0x740, 0x4c0, 0x280);
            aSetBuffer(arg2++, 0, 0, 0, 640);
            aMix(arg2++, 0, D_802211B0.unk4 + 0x8000, 0x740, 0x740);
        }
        else
        {
            t4 = (s16)((v1->unkC & 7) * 2);
            ra = (s16)ALIGN(v1->unk10 + t4, 4);
            aSetLoadBufferPair(arg2, 0, v1->unkC - t4 / 2);
            if (v1->unk12 != 0)
            {
                aSetLoadBufferPair(arg2, ra, 0);
            }
            aSetBuffer(arg2++, 0, t4 + 0x740, 0x4c0, arg1 << 1);
            aResample(arg2++, D_802211B0.unk0, (u16)D_802211B0.unk6, FIX(D_802211B0.unk1C));
            aSetBuffer(arg2++, 0, t4 + 0x880, 0x600, arg1 << 1);
            aResample(arg2++, D_802211B0.unk0, (u16)D_802211B0.unk6, FIX(D_802211B0.unk20));
            aSetBuffer(arg2++, 0, 0, 0, 640);
            aMix(arg2++, 0, D_802211B0.unk4 + 32768, 0x4c0, 0x4c0);
            aDMEMMove(arg2++, 0x4c0, 0x740, 0x280);
        }
        arg2 = func_80314480(arg0, arg1, arg2);
        if (D_802212A2 == 1)
        {
            //maybe also macro?
            aSetBuffer(arg2++, 0, 0, 0x740, v1->unk10);
            aSaveBuffer(arg2++, FIX(&D_802211B0.unk14.unk00[v1->unkC]));
            aSetBuffer(arg2++, 0, 0, 0x880, v1->unk10);
            aSaveBuffer(arg2++, FIX(&D_802211B0.unk14.unk04[v1->unkC]));
            if (v1->unk12 != 0)
            {
                aSetBuffer2(arg2++, 0, 0, v1->unk10 + 0x740, v1->unk12);
                aSaveBuffer(arg2++, FIX(D_802211B0.unk14.unk00));
                aSetBuffer2(arg2++, 0, 0, v1->unk10 + 0x880, v1->unk12);
                aSaveBuffer(arg2++, FIX(D_802211B0.unk14.unk04));
            }
        }
        else
        {
            aSetBuffer(arg2++, 0, 0, 0x740, 0x280);
            aSaveBuffer(arg2++, FIX(D_802211B0.unk2C[D_802211B0.unk3][arg3].unk4));
            D_802211B0.unk0 = 0;
        }
    }
    return arg2;
}

#ifdef NON_MATCHING
u64 *func_80314480(u16 *arg0, s32 arg1, u64 *arg2)
{
    s32 sp174;
    struct Struct_func_80318870 *s7;
    struct SubEntry *sp164;
    struct SubSubEntry *sp160;
    s16 *sp15C;
    s32 t2; //150
    s32 t3; //14c
    s32 sp148; //audio flags?
    UNUSED u8 pad8[0x14];
    s32 sp130;
    UNUSED u8 pad7[0xC];
    u8 *sp120;
    u32 t5; //108, definitely unsigned
    //UNUSED u8 pad6[4];
    s32 sp110;
    s32 a0; // 10c
    //UNUSED u8 pad5[0x10c - 0xe8 - 4];
    s32 spE8;
    s32 spE4;
    u32 fp; //probably unsigned
    s32 t0;
    s32 spD8;
    u16 spD6;

    // sp6c is a temporary!

    u16 sp5c; 
    s32 sp58;
    s32 sp54;
    s32 s6;
    s32 s6_2;
    s32 s2;

    s32 s0;
    s32 s3;
    s32 s5;
    //s32 v0;
    u32 v1_1;
    s32 v1_2;
    u32 a3;
    s32 t9;
    u8 *v0_2;

    f32 f12;
    UNUSED s32 temp;

    for (sp174 = 0, sp15C = NULL; sp174 < D_80226D70; sp174++)
    {
        s7 = &D_80222A10[sp174];
        // probably a macro; this weird condition is in other places as well
        if ((D_802218D0[s7->unk7] < 2) != 0)
        {
            D_80331D40 = (s7->unk7 << 8) + sp174 + 0x1000000;
        }
        else if (s7->unk0b80)
        {
            // This matches much much better if unk0b80 is volatile... but that
            // breaks other functions (e.g. func_80315D94). Can we achieve the
            // volatile effect in some other way?
            sp148 = 0;

            if (s7->unk0b40 == 1)
            {
                sp148 = 1;
                s7->unk14 = 0;
                s7->unk20 = 0;
            }

            if (s7->unk38 < US_FLOAT(2.0))
            {
                spE8 = 1;
                if (s7->unk38 > US_FLOAT(1.99996))
                {
                    s7->unk38 = US_FLOAT(1.99996);
                }
                f12 = s7->unk38;
            }
            else
            {
                spE8 = 2;
                if (s7->unk38 >= US_FLOAT(3.99993))
                {
                    s7->unk38 = US_FLOAT(3.99993);
                }
                f12 = s7->unk38 * US_FLOAT(.5);
            }

            sp5c = (u16)(s32)(f12 * 32768.0f);
            v1_1 = s7->unk20 + (sp5c * arg1) * 2;
            s7->unk20 = v1_1; // 16-bit store, can't reuse

            if (s7->unk24 == NULL)
            {
                // v1_1 >> 0x10 stored in s0
                arg2 = func_80314F08(arg2, s7, v1_1 >> 0x10);
                spD6 = s7->unk14 * 2 + 0x180;
                s7->unk14 += (v1_1 >> 0x10);
                sp148 = 0;
            }
            else
            {
                sp164 = s7->unk24->unk0;

                // sp58 is a low-numbered register, so possibly a temporary.
                // Should it be used for v1_1 >> 0x10 above as well? But then
                // the asm matches worse. This variable seems to highly involved
                // in causing this function not to match...
                sp58 = v1_1 >> 0x10; // v0

                sp160 = sp164->unk8;
                sp110 = sp160->unk4;
                sp120 = sp164->unk4;

                sp54 = (spE8 == 1);

                spD8 = 0;
                for (spE4 = 0; spE4 < spE8; spE4++)
                {
                    fp = 0;
                    s5 = 0;

                    // This whole if-else if chain is weird. First it uses sp54
                    // instead of spE8 == 1, and it needs a weird if to not
                    // induce non-matchings all over the rest of the function.
                    // Then it induces a bunch of stack-relative loads that
                    // shouldn't be there. Finally, it relates to sp58, which
                    // behaves very oddly...
                    if (sp54) // spE8 == 1
                    {
                        if (1) // shouldn't be here, but it makes things line up better...
                            t5 = sp58;
                    }
                    else if (sp58 & 1)
                    {
                        t5 = (sp58 & ~1) + (spE4 * 2);
                    }
                    else
                    {
                        t5 = sp58;
                    }

                    if (sp15C != sp164->unkC->book)
                    {
                        u32 v1;
                        sp15C = sp164->unkC->book;
                        v1 = sp164->unkC->order * sp164->unkC->npredictors;
                        aLoadADPCM(arg2++, v1 * 16, FIX(sp15C));
                    }

                    while (fp != t5)
                    {
                        s32 v1;
                        s32 s0;
                        // sp58 = sp58; here, doesn't happen
                        t2 = 0;
                        t3 = 0;
                        a0 = t5 - fp;
                        s2 = s7->unk14 & 0xf;
                        v1 = sp110 - s7->unk14;

                        if (s2 == 0 && !s7->unk0b20)
                        {
                            s2 = 16;
                        }
                        s6 = 16 - s2; // a1

                        if (a0 < v1)
                        {
                            t0 = (a0 - s6 + 0xf) / 16;
                            s0 = t0 * 16;
                            s3 = s6 + s0 - a0;
                        }
                        else
                        {
                            s0 = v1 + s2 - 0x10;
                            s3 = 0;
                            if (s0 <= 0)
                            {
                                s0 = 0;
                                s6 = v1;
                            }
                            t0 = (s0 + 0xf) / 16;
                            if (sp160->unk8 != 0)
                            {
                                t3 = 1;
                            }
                            else
                            {
                                t2 = 1;
                            }
                        }

                        // Improve regalloc for saved registers. Probably
                        // shouldn't be here, but it gives nicer diffs for now.
                        s6_2 = s6;

                        if (t0 != 0)
                        {
                            // maybe keep a var for t0 * 9?
                            v0_2 = func_80317270(sp120 + (s7->unk14 - s2 + 0x10) / 16 * 9,
                                    t0 * 9, sp148, &s7->unk3);
                            a3 = (u32)v0_2 & 0xf;
                            aSetBuffer(arg2++, 0, 0x3f0, 0, t0 * 9 + a3);
                            aLoadBuffer(arg2++, FIX(v0_2 - a3));
                        }
                        else
                        {
                            s0 = 0;
                            a3 = 0;
                        }

                        if (s7->unk0b20 != 0)
                        {
                            aSetLoop(arg2++, FIX(sp164->unk8 + 8));
                            sp148 = A_LOOP; // = 2
                            s7->unk0b20 = 0;
                        }

                        if (fp == 0)
                        {
                            aSetBuffer(arg2++, 0, a3 + 0x3f0, 0x180, s0 * 2);
                            aADPCMdec(arg2++, sp148, FIX(s7->unk34->unk00));
                            sp130 = s2 * 2;
                        }
                        else
                        {
                            aSetBuffer(arg2++, 0, a3 + 0x3f0, ALIGN(s5, 5) + 0x180, s0 * 2);
                            aADPCMdec(arg2++, sp148, FIX(s7->unk34->unk00));
                            aDMEMMove(arg2++, ALIGN(s5, 5) + (s2 * 2) + 0x180, s5 + 0x180, (s0 + s6_2 - s3) * 2);
                        }

                        fp = fp + s0 + s6_2 - s3;
                        v1_2 = s0 + s6_2 - s3;
                        switch (sp148)
                        {
                        case 1: // A_INIT?
                            sp130 = 0;
                            s5 += s0 * 2;
                            break;

                        case A_LOOP: // = 2
                            s5 += v1_2 * 2;
                            break;

                        default:
                            if (s5 != 0)
                            {
                                s5 += v1_2 * 2;
                            }
                            else
                            {
                                s5 = (v1_2 + s2) * 2;
                            }
                            break;
                        }
                        sp148 = 0;

                        if (t2)
                        {
                            aClearBuffer(arg2++, s5 + 0x180, (t5 - fp) * 2);
                            s7->unk14 = 0;
                            s7->unk0b10 = 1;
                            s7->unk0b80 = 0;
                            break; // goto? doesn't matter, though
                        }
                        if (t3)
                        {
                            s7->unk0b20 = 1;
                            s7->unk14 = sp160->unk0;
                        }
                        else
                        {
                            s7->unk14 += a0;
                        }
                    }

                    switch (spE8)
                    {
                    case 1:
                        spD6 = sp130 + 0x180;
                        break;

                    case 2:
                        switch (spE4)
                        {
                        case 0:
                            aSetBuffer(arg2++, 0, sp130 + 0x180, 0x20, t5 + 4);
                            aResample(arg2++, 0x1, 0xff60, FIX(s7->unk34->unkf0));
                            spD8 = t5 + 4;
                            spD6 = 36;
                            if (s7->unk0b10 != 0)
                            {
                                aClearBuffer(arg2++, t5 + 0x24, t5 + 0x10);
                            }
                            break;

                        case 1:
                            aSetBuffer(arg2++, 0, sp130 + 0x180, 0x160, t5 + 8);
                            aResample(arg2++, 0x1, 0xff60, FIX(s7->unk34->unkf0));
                            aDMEMMove(arg2++, 0x164, spD8 + 0x20, t5 + 4);
                            break;
                        }
                    }

                    if (s7->unk0b10 != 0)
                    {
                        // ("break;" doesn't match)
                        sp148 = 0;
                        goto out;
                    }
                }

                sp148 = 0;
            }
out:

            if (s7->unk0b40 == 1)
            {
                sp148 = 1;
                s7->unk0b40 = 0;
            }

            arg2 = func_80314FD4(arg2, s7, arg1 * 2, sp5c, spD6, sp148);

            if (s7->unkC != 0 || s7->unk10 != 0)
            {
                s0 = 1;
            }
            else if (s7->unkE != 0 || s7->unk12 != 0)
            {
                s0 = 2;
            }
            else
            {
                s0 = 0;
            }

            arg2 = func_80315030(arg2, s7, arg1, 0, s0, sp148);
            if (s7->unk1)
            {
                arg2 = func_803155F4(arg2, s7, arg1 * 2, sp148, s0);
            }
        }
    }

    t9 = arg1 * 2;
    aSetBuffer(arg2++, 0, 0, 0, t9);
    aInterleave(arg2++, 0x4c0, 0x600);
    t9 *= 2;
    aSetBuffer(arg2++, 0, 0, 0, t9);
    aSaveBuffer(arg2++, FIX(arg0));
    return arg2;
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_80314480_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_80314480_us.s")
#endif

u64 *func_80314F08(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2)
{
    s32 a3;
    s32 i;
    //                fl, dmemin, dmemout, count
    aSetBuffer(arg0++, 0, 0x180, 0, sizeof(arg1->unk34->unk110)); //interesting that it's 128...
    aLoadBuffer(arg0++, FIX(arg1->unk34->unk110));
    arg1->unk14 = (arg1->unk5 - 1) & arg1->unk14;
    a3 = 64 - arg1->unk14;
    if (a3 < arg2)
    {
        for (i = 0; i <= (arg2 - a3 + 0x3f) / (1 << 6) - 1; i++)
        {
            //                dmemin dmemout       count
            aDMEMMove(arg0++, 0x180, 512 + i * sizeof(arg1->unk34->unk110), sizeof(arg1->unk34->unk110))
        }
    }
    return arg0;
}

u64 *func_80314FD4(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, u16 arg4, u32 arg5)
{
    aSetBuffer(arg0++, 0, arg4, 0, arg2);
    aResample(arg0++, arg5, arg3, FIX(arg1->unk34->unk20));
    return arg0;
}

u64 *func_80315030(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, s32 arg4, UNUSED u32 arg5)
{
    UNUSED u8 pad[16];
    u16 sp20[4];
    sp20[0] = arg1->unk9C;
    sp20[1] = arg1->unk9E;
    sp20[2] = arg1->unk3C;
    sp20[3] = arg1->unk3E;
    arg1->unk9C = sp20[2];
    arg1->unk9E = sp20[3];
    return func_80315094(arg0, arg1, arg2, arg3, arg4, sp20);
}

u64 *func_80315094(u64 *audio_cmd, struct Struct_func_80318870 *arg1, s32 arg2, u16 arg3, s32 arg4, u16 *arg5)
{
    UNUSED u8 pad[3];
    u8 t1;
    UNUSED u8 pad2[8];
    s32 spac, v0;
    if (arg1->unk1)
    {
        aClearBuffer(audio_cmd++, 0x200, 320);

        switch (arg4)
        {
        case 1:
            aSetBuffer(audio_cmd++, 0, arg3, 0x200, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x600, 0x740, 0x880);
            break;
        case 2:
            aSetBuffer(audio_cmd++, 0, arg3, 0x4c0, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x200, 0x740, 0x880);
            break;
        default:
            aSetBuffer(audio_cmd++, 0, arg3, 0x4c0, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x600, 0x740, 0x880);
            break;
        }
    }
    else
    {
        if (arg1->unk0b4)
        {
            aClearBuffer(audio_cmd++, 0x200, 640);
            aSetBuffer(audio_cmd++, 0, arg3, 0x200, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x600, 0x340, 0x880);
        }
        else if (arg1->unk0b2)
        {
            aClearBuffer(audio_cmd++, 0x200, 640);
            aSetBuffer(audio_cmd++, 0, arg3, 0x4c0, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x200, 0x740, 0x340);
        }
        else
        {
            aSetBuffer(audio_cmd++, 0, arg3, 0x4c0, arg2 * 2);
            aSetBuffer(audio_cmd++, 8, 0x600, 0x740, 0x880);
        }
    }

    if (arg5[2] == arg5[0] && arg5[3] == arg5[1] && !arg1->unk0b8)
    {
        t1 = 0;
    }
    else
    {
        t1 = 1;
        spac = func_80313BB0(arg5[0], arg5[2], arg2);
        v0 = func_80313BB0(arg5[1], arg5[3], arg2);

        // arg5 is volume, so spac and v0 must be rate?
        aSetVolume(audio_cmd++, 6, arg5[0], 0, 0);
        aSetVolume(audio_cmd++, 4, arg5[1], 0, 0);
        aSetVolume32(audio_cmd++, 2, arg5[2], spac);
        aSetVolume32(audio_cmd++, 0, arg5[3], v0);
        aSetVolume(audio_cmd++, 8, D_802212A0, 0, arg1->unkA0);
    }
    if (D_802211B0.unk1 && arg1->unk40)
    {
        aEnvMixer(audio_cmd++, t1 | 8, FIX(arg1->unk34->unk40));
        if (arg1->unk0b4)
        {
            aSetBuffer(audio_cmd++, 0, 0, 0, arg2 * 2);
            aMix(audio_cmd++, 0, 0x8000, 0x200, 0x4c0);
            aMix(audio_cmd++, 0, 0x8000, 0x340, 0x740);
        }
        else if (arg1->unk0b2)
        {
            aSetBuffer(audio_cmd++, 0, 0, 0, arg2 * 2);
            aMix(audio_cmd++, 0, 0x8000, 0x200, 0x600);
            aMix(audio_cmd++, 0, 0x8000, 0x340, 0x880);
        }
    }
    else
    {
        aEnvMixer(audio_cmd++, t1, FIX(arg1->unk34->unk40));
        if (arg1->unk0b4)
        {
            aSetBuffer(audio_cmd++, 0, 0, 0, arg2 * 2);
            aMix(audio_cmd++, 0, 0x8000, 0x200, 0x4c0);
        }
        else if (arg1->unk0b2)
        {
            aSetBuffer(audio_cmd++, 0, 0, 0, arg2 * 2);
            aMix(audio_cmd++, 0, 0x8000, 0x200, 0x600);
        }
    }
    return audio_cmd;
}

u64 *func_803155F4(u64 *arg0, struct Struct_func_80318870 *arg1, s32 arg2, s32 arg3, s32 arg4)
{
    u16 t0;
    u16 a3;
    u16 a2;
    u16 pitch; // t2
    UNUSED s32 padding[11];

    switch (arg4)
    {
    case 1:
        t0 = 0x4c0;
        arg1->unk12 = 0;
        a2 = arg1->unkC;
        a3 = arg1->unk10;
        arg1->unk10 = a2;
        break;
    case 2:
        t0 = 0x600;
        arg1->unk10 = 0;
        a2 = arg1->unkE;
        a3 = arg1->unk12;
        arg1->unk12 = a2;
        break;
    default:
        return arg0;
    }

    if (arg3 != 1) //A_INIT?
    {
        if (a3 == 0)
        {
            aDMEMMove(arg0++, 0x200, 0, 8);
            aClearBuffer(arg0++, 8, 8);
            aDMEMMove(arg0++, 0x200, 0x10, 0x10);
            aSetBuffer(arg0++, 0, 0, 0, 32);
            aSaveBuffer(arg0++, FIX(arg1->unk34->unk90));
            pitch = (arg2 << 0xf) / (a2 + arg2 - a3 + 8);
            aSetBuffer(arg0++, 0, 0x208, 0, a2 + arg2 - a3);
            aResample(arg0++, 0, pitch, FIX(arg1->unk34->unk90));
        }
        else
        {
            pitch = (a2 == 0) ?
                (arg2 << 0xf) / (arg2 - a3 - 4) :
                (arg2 << 0xf) / (arg2 + a2 - a3);
            aSetBuffer(arg0++, 0, 0x200, 0, a2 + arg2 - a3);
            aResample(arg0++, 0, pitch, FIX(arg1->unk34->unk90));
        }

        if (a3 != 0)
        {
            aSetBuffer(arg0++, 0, 0x200, 0, a3);
            aLoadBuffer(arg0++, FIX(arg1->unk34->unkb0));
            aDMEMMove(arg0++, 0, a3 + 0x200, a2 + arg2 - a3);
        }
        else
        {
            aDMEMMove(arg0++, 0, 0x200, a2 + arg2 - a3);
        }
    }
    else
    {
        aDMEMMove(arg0++, 0x200, 0, arg2);
        aDMEMMove(arg0++, 0, a2 + 0x200, arg2);
        aClearBuffer(arg0++, 0x200, a2);
    }

    if (a2)
    {
        aSetBuffer(arg0++, 0, 0, arg2 + 0x200, a2);
        aSaveBuffer(arg0++, FIX(arg1->unk34->unkb0));
    }

    aSetBuffer(arg0++, 0, 0, 0, arg2);
    aMix(arg0++, 0, 0x7fff, 0x200, t0);

    return arg0;
}

void func_803159C0(struct Struct_func_80318870 *arg0)
{
    arg0->unk3C = 0;
    arg0->unk3E = 0;
    arg0->unk40 = 0;
    arg0->unkA0 = 0;
    arg0->unkA2 = 0;
    arg0->unk9C = 1;
    arg0->unk9E = 1;
    arg0->unk38 = 0.0f;
}

void func_803159EC(struct Struct_func_80318870 *arg0, f32 arg1, f32 arg2, u8 arg3)
{
    s32 v0;
    f32 f0;
    f32 f2;
#ifdef VERSION_JP
    v0 = MIN((s32)(arg2 * 127.5), 127);
#endif
    if (arg0->unk0b1 && D_80226D7F == 1)
    {
        s8 a1;
        a1 = MIN((s8)(arg2 * 10.0f), 9);
        arg0->unkE = D_80332CD0[a1];
        arg0->unkC = D_80332CD0[9 - a1];
        arg0->unk0b4 = 0;
        arg0->unk0b2 = 0;
        arg0->unk1 = 1;
#ifdef VERSION_US
        v0 = (s32)(arg2 * 127.5f) & 127;
#endif
        f0 = D_80332CE4[v0];
        f2 = D_80332CE4[127 - v0];
    }
    else if (arg0->unk0b1 && D_80226D7F == 0)
    {
        u8 v1 = 0;
        u8 a1 = 0;
        arg0->unkE = 0;
        arg0->unkC = 0;
        arg0->unk1 = 0;
#ifdef VERSION_US
        v0 = (s32)(arg2 * 127.5f) & 127;
#endif
        f0 = D_80332EE4[v0];
        f2 = D_80332EE4[127 - v0];
        if (v0 < 0x20)
        {
            v1 = 1;
        }
        else if (v0 > 0x60)
        {
            a1 = 1;
        }
        arg0->unk0b4 = a1;
        arg0->unk0b2 = v1;
    }
    else if (D_80226D7F == 3)
    {
        f0 = .707f;
        f2 = .707f;
    }
    else
    {
#ifdef VERSION_US
        v0 = (s32)(arg2 * 127.5f) & 127;
#endif
        f0 = D_803330E4[v0];
        f2 = D_803330E4[127 - v0];
    }

    arg1 = MAX(arg1, 0);
#ifdef VERSION_JP
    arg0->unk3C = ((u16)(arg1 * f0)) & -0x8100;
    arg0->unk3E = ((u16)(arg1 * f2)) & -0x8100;
#else
    arg0->unk3C = (u16)((s32)(arg1 * f0)) & -0x8100;
    arg0->unk3E = (u16)((s32)(arg1 * f2)) & -0x8100;
#endif
    if (arg0->unk3C == 0)
    {
        arg0->unk3C++;
    }
    if (arg0->unk3E == 0)
    {
        arg0->unk3E++;
    }
    if (arg0->unk40 != arg3)
    {
        arg0->unk40 = arg3;
        arg0->unkA0 = arg3 << 8;
        arg0->unk0b8 = 1;
        return;
    }
    if (arg0->unk0b40)
    {
        arg0->unk0b8 = 1;
        return;
    }
    arg0->unk0b8 = 0;
    return;
}

void func_80315D88(struct Struct_func_80318870 *arg0, f32 arg1)
{
    arg0->unk38 = arg1;
}

// init something?
void func_80315D94(struct Struct_func_80318870 *arg0)
{
    arg0->unk1 = 0;
    arg0->unkE = 0;
    arg0->unk0b80 = 1;
    arg0->unk0b40 = 1;
    arg0->unk0b20 = 0;
    arg0->unk0b10 = 0;
    arg0->unk0b4 = 0;
    arg0->unk0b2 = 0;
    arg0->unkC = 0;
    arg0->unk10 = 0;
    arg0->unk12 = 0;
}

void func_80315DE0(struct Struct_func_80318870 *arg0)
{
    if (arg0->unk0b40 == 1)
    {
        arg0->unk0b40 = 0;
    }
    else
    {
        func_803159EC(arg0, 0, .5, 0);
    }
    arg0->unk4 = 0;
    arg0->unk0b80 = 0;
    arg0->unk0b10 = 0;
    arg0->unk2C = MINUS_ONE;
    arg0->unk28 = MINUS_ONE;
}
