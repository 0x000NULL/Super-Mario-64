#include <ultra64.h>

#include "sm64.h"
#include "dac.h"
#include "dma.h"
#include "interface_1.h"
#include "interface_2.h"
#include "playback.h"
#include "playback2.h"

#ifdef VERSION_JP
#define US_FLOAT(x) x
#else
#define US_FLOAT(x) x ## f
#endif

// Convert u8 or u16 to f32. On JP, this uses a u32->f32 conversion,
// resulting in more bloated codegen, while on US it goes through s32.
// Since u8 and u16 fit losslessly in both, behavior is the same.
#ifdef VERSION_JP
#define FLOAT_CAST(x) (f32) (x)
#else
#define FLOAT_CAST(x) (f32) (s32) (x)
#endif

void func_8031B0A4(struct SubStruct_func_80318870 *arg0);
void func_8031C104(struct Struct80225DD8 *arg0);

void func_8031A810(struct Struct80225DD8 *arg0)
{
    s32 i;

    arg0->unk0b80 = 0;
    arg0->unk0b40 = 0;
    arg0->unk0b20 = 0;
    arg0->unk0b10 = 0;
    arg0->unk0b8 = 0;
    arg0->unk0b4 = 0;
    arg0->unk1A = 0;
    arg0->unk0b2 = 0;
    arg0->unk5C.unk18 = 0;
    arg0->unk20 = 1.0f;
    arg0->unk1C = 1.0f;
    arg0->unk2C = 1.0f;
    arg0->unk24 = 0.5f;
    arg0->unk28 = 1.0f;
    arg0->unk34 = 0;
    arg0->unk3 = 0;
    arg0->unk4 = 3;
    arg0->unk16 = 0;
    arg0->unk78.unk4 = D_80332AB4;
    arg0->unk78.unk0 = 0x20;
    arg0->unk78.unk2 = 0;
    arg0->unk6 = D_80226D7E;
    arg0->unkC = 0x800;
    arg0->unk8 = 0x800;
    arg0->unkE = 0;
    arg0->unkA = 0;
    arg0->unk10 = 0;
    arg0->unk12 = 0;
    arg0->unk14 = 0;

    for (i = 0; i < 8; i++)
    {
        arg0->unk54[i] = -1;
    }

    arg0->unk0b1 = 0;
    func_803191F8(arg0->unk80);
}

s32 func_8031A920(struct Struct80225DD8 *arg0, s32 arg1)
{
    struct SubStruct_func_80318870 *sub;

    if (arg0->unk44[arg1] == NULL)
    {
        sub = func_8031AF34(&D_80225E98);
        arg0->unk44[arg1] = sub;
        if (sub == NULL)
        {
            arg0->unk44[arg1] = NULL;
            return -1;
        }
    }
    else
    {
        func_80318EC4(arg0->unk44[arg1]);
    }

    sub = arg0->unk44[arg1];
    sub->unk50 = arg0;
    sub->unk14 = arg0->unk78;
    sub->unk14.unk0 = 0;
    sub->unk0b80 = 1;
    sub->unk0b20 = 0;
    sub->unk0b10 = 0;
    sub->unk0b40 = 0;
    sub->unk4.unk00 = 0;
    sub->unk54.unk18 = 0;
    sub->unk1 = 0;
    sub->unk2 = 0x80;
    sub->unk1E = 0;
    sub->unk3C = 0;
    sub->unk3E = 0;
    sub->unk40 = 0;
    sub->unk44 = NULL;
    sub->unk48 = NULL;
    sub->unk24 = 0.0f;
    sub->unk28 = 0.5f;
    return 0;
}

void func_8031AA10(struct SubStruct_func_80318870 *arg0)
{
    if (arg0 != NULL)
    {
        func_80318EC4(arg0);
        arg0->unk0b80 = 0;
        arg0->unk0b40 = 1;
    }
}

void func_8031AA4C(struct Struct80225DD8 *arg0, s32 arg1)
{
    struct SubStruct_func_80318870 *unk44 = arg0->unk44[arg1];
    struct PlaybackListItem *item;

    if (unk44 != NULL)
    {
        // push to end of list
        item = &unk44->unk70;
        if (item->prev == NULL)
        {
            D_80225E98.prev->next = item;
            item->prev = D_80225E98.prev;
            item->next = &D_80225E98;
            D_80225E98.prev = item;
            D_80225E98.u.count++;
            item->unkC = D_80225E98.unkC;
        }
        func_8031AA10(unk44);
        arg0->unk44[arg1] = NULL;
    }
}

void func_8031AAD0(struct Struct80225DD8 *arg0)
{
    s32 i;
    for (i = 0; i < 4; i++)
    {
        func_8031AA4C(arg0, i);
    }

    func_803192FC(arg0->unk80);
    arg0->unk0b80 = 0;
    arg0->unk0b40 = 1;
}

struct Struct80225DD8 *func_8031AB3C(void)
{
    s32 i;
    for (i = 0; i < ARRAY_COUNT(D_80222DD8); i++)
    {
        if (D_80222DD8[i].unk40 == NULL)
        {
            return D_80222DD8 + i;
        }
    }
    return &D_80225DD8;
}

void func_8031AB84(struct Struct80222A18 *arg0, u16 bits)
{
    struct Struct80225DD8 *temp;
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        if (bits & 1)
        {
            temp = arg0->unk2C[i];
            if (IS_SPECIAL(temp) == 1 && temp->unk40 == arg0)
            {
                func_8031AAD0(temp);
                temp->unk40 = 0;
            }
            temp = func_8031AB3C();
            if (IS_SPECIAL(temp) == 0)
            {
                D_80331D40 = i + 0x10000;
                arg0->unk2C[i] = temp;
            }
            else
            {
                func_8031A810(temp);
                arg0->unk2C[i] = temp;
                temp->unk40 = arg0;
                temp->unk5 = arg0->unk6[0];
                temp->unk2 = arg0->unk4;
                temp->unk1 = arg0->unk3;
            }
        }
        bits >>= 1;
    }
}

void func_8031ACB0(struct Struct80222A18 *arg0, u16 bits)
{
    struct Struct80225DD8 *temp;
    s32 i;

    for (i = 0; i < 0x10; i++)
    {
        if (bits & 1)
        {
            temp = arg0->unk2C[i];
            if (IS_SPECIAL(temp) == 1)
            {
                if (temp->unk40 == arg0)
                {
                    func_8031AAD0(temp);
                    temp->unk40 = 0;
                }
                arg0->unk2C[i] = &D_80225DD8;
            }
        }
        bits >>= 1;
    }
}

void func_8031AD80(struct Struct80222A18 *arg0, u8 arg1, void *arg2)
{
    struct Struct80225DD8 *temp = arg0->unk2C[arg1];
    s32 i;

    if (IS_SPECIAL(temp) != 0)
    {
        temp->unk0b80 = 1;
        temp->unk0b40 = 0;
        temp->unk5C.unk18 = 0;
        temp->unk5C.unk0 = arg2;
        temp->unk16 = 0;
        for (i = 0; i < 4; i++)
        {
            if (temp->unk44[i] != 0)
            {
                func_8031AA4C(temp, i);
            }
        }
    }
}

void func_8031AE24(struct Struct80222A18 *arg0)
{
    func_8031ACB0(arg0, 0xffff);
    func_803192FC(arg0->unk90);
    arg0->unk0b40 = 1;
    arg0->unk0b80 = 0;

    if (D_80221910[arg0->unk5] >= 2)
    {
        D_80221910[arg0->unk5] = 3;
    }

    if (D_802218D0[arg0->unk6[0]] >= 2)
    {
        D_802218D0[arg0->unk6[0]] = 3;
    }

    if (D_802214F8.second.arr[0].unk8 == arg0->unk6[0])
    {
        D_802214F8.second.unk0 = 1;
    }
    else if (D_802214F8.second.arr[1].unk8 == arg0->unk6[0])
    {
        D_802214F8.second.unk0 = 0;
    }
}

void func_8031AEF4(struct PlaybackListItem *head, struct PlaybackListItem *item)
{
    // add 'item' to the end of the list given by 'head', if it's not in any list
    if (item->prev == NULL)
    {
        head->prev->next = item;
        item->prev = head->prev;
        item->next = head;
        head->prev = item;
        head->u.count++;
        item->unkC = head->unkC;
    }
}

void *func_8031AF34(struct PlaybackListItem *head)
{
    // remove the last item from a list, and return it (or NULL if empty)
    struct PlaybackListItem *item = head->prev;
    if (item == head)
    {
        return NULL;
    }
    item->prev->next = head;
    head->prev = item->prev;
    item->prev = NULL;
    head->u.count--;
    return item->u.value;
}

void func_8031AF74(void)
{
    s32 i;

    D_80225E98.prev = &D_80225E98;
    D_80225E98.next = &D_80225E98;
    D_80225E98.u.count = 0;
    D_80225E98.unkC = NULL;

    for (i = 0; i < ARRAY_COUNT(D_802245D8); i++)
    {
        D_802245D8[i].unk70.u.value2 = D_802245D8 + i;
        D_802245D8[i].unk70.prev = NULL;
        func_8031AEF4(&D_80225E98, &D_802245D8[i].unk70);
    }
}

u8 func_8031B01C(struct Interface1Buffer *arg0)
{
    u8 *old = arg0->unk0++;
    return *old;
}

s16 func_8031B030(struct Interface1Buffer *arg0)
{
    s16 ret = *(arg0->unk0++) << 8;
    ret = *(arg0->unk0++) | ret;
    return ret;
}

u16 func_8031B060(struct Interface1Buffer *arg0)
{
    u16 ret = *(arg0->unk0++);
    if (ret & 0x80)
    {
        ret = (ret << 8) & 0x7f00;
        ret = *(arg0->unk0++) | ret;
    }
    return ret;
}

#ifdef NON_MATCHING

void func_8031B0A4(struct SubStruct_func_80318870 *arg0)
{
    struct Struct80222A18 *sp5C; // t4
    struct Struct80225DD8 *sp58; // t5
    struct Interface1Buffer *buf; // v0
    struct Substruct80225DD8 *temp_v0_11;
    struct struct8031A078 *temp_v0_22;
    struct InstrumentSomething **temp_a1_3;
    struct InstrumentSomething *phi_a0;
    struct SubEntryAndF32 *phi_v0_5;
    struct SubEntryAndF32 *phi_v0_6;
    struct InstrumentSomething *phi_v1_2;
    struct SubA *subA;
    u8 sp3F;
    u8 instr; // a0
    u8 sp3D; // t0
    u8 loBits;
    u16 sp3A; // t2, a0, a1
    s32 sp30; // t3
    f32 sp28; // f0
    f32 sp24;
    u8 temp8;
    u8 *old;
    u8 *old2;
    u8 temp_v0_20;
    u8 phi_a1_3;
    f32 temp_f12;
    f32 temp_f2;
    s32 temp_a0_5;
    u8 temp_t0;
    s32 temp_t1;
    s32 temp_t1_2;
    u8 temp_a0_6;
    u8 temp_t7_4;
    s32 temp_a3;
    u8 phi_v0_2;
    s32 phi_v1;
    f32 phi_f0;

    sp3F = 1;
    if (arg0->unk0b80 == 0)
    {
        return;
    }

    if (arg0->unk3C >= 2)
    {
        arg0->unk3C--;
        if (!arg0->unk0b20 && arg0->unk3E >= arg0->unk3C)
        {
            func_80318EC4(arg0);
            arg0->unk0b20 = 1;
        }
        return;
    }

    if (!arg0->unk0b10)
    {
        func_80318EC4(arg0);
    }

    if ((arg0->unk4.unk00 & ~0x80) == 1 || (arg0->unk4.unk00 & ~0x80) == 2)
    {
        arg0->unk4.unk00 = 0;
    }

    sp58 = arg0->unk50;
    sp5C = sp58->unk40;

    for (;;)
    {
        // (Moving buf outside the loop improves initial regalloc, but is wrong)
        buf = &arg0->unk54;
        old2 = buf->unk0++;
        instr = *old2;
        if (instr <= 0xc0)
        {
            break;
        }

        switch (instr)
        {
        case 0xff:
            if (buf->unk18 == 0)
            {
                func_8031AA10(arg0);
                return;
            }
            buf->unk18--,
            buf->unk0 = buf->unk4[buf->unk18];
            break;

        case 0xfc:
            // Something is wrong with the codegen here... It almost looks like
            // it's inlining func_8031B030, but it lacks a s16 cast.
            sp3A = *(buf->unk0++) << 8;
            sp3A = *(buf->unk0++) | sp3A;
            buf->unk18++;
            buf->unk4[buf->unk18 - 1] = buf->unk0;
            buf->unk0 = sp5C->unk14 + sp3A;
            break;

        case 0xf8:
            old = buf->unk0++;
            buf->unk14[buf->unk18] = *old;
            buf->unk18++;
            buf->unk4[buf->unk18 - 1] = buf->unk0;
            break;

        case 0xf7:
            buf->unk14[buf->unk18 - 1]--;
            if (buf->unk14[buf->unk18 - 1] != 0)
            {
                buf->unk0 = buf->unk4[buf->unk18 - 1];
            }
            else
            {
                buf->unk18--;
            }
            break;

        case 0xfb:
            sp3A = *(buf->unk0++) << 8;
            sp3A = *(buf->unk0++) | sp3A;
            buf->unk0 = sp5C->unk14 + sp3A;
            break;

        case 0xc1:
        case 0xca:
            temp_a0_5 = *(buf->unk0++);
            if (instr == 0xc1)
            {
                arg0->unk24 = (f32) (temp_a0_5 * temp_a0_5);
            }
            else
            {
                arg0->unk28 = (f32) temp_a0_5 / US_FLOAT(128.0);
            }
            break;

        case 0xc2:
        case 0xc9:
            temp_a0_6 = *(buf->unk0++);
            if (instr == 0xc9)
            {
                arg0->unk2 = temp_a0_6;
            }
            else
            {
                arg0->unk1E = temp_a0_6;
            }
            break;

        case 0xc4:
        case 0xc5:
            if (instr == 0xc4)
            {
                temp8 = 1;
            }
            else
            {
                temp8 = 0;
            }
            arg0->unk0b10 = temp8;
            func_80318EC4(arg0);
            break;

        case 0xc3:
            // This doesn't match very well... sp3A is definitely set here
            // (it's falsely preserved until after the loop), but maybe there's
            // also inlining going on, with sp3A as a temp variable being used
            // for no good reason? Or it could just be a macro.
            sp3A = *(buf->unk0++);
            if (sp3A & 0x80)
            {
                sp3A = (sp3A << 8) & 0x7f00;
                sp3A = *(buf->unk0++) | sp3A;
            }
            arg0->unk38 = sp3A;
            break;

        case 0xc6:
            old = buf->unk0++;
            phi_v0_2 = *old;
            if (phi_v0_2 >= 0x7f)
            {
                break;
            }
            temp_a3 = sp58->unk5;
            if (phi_v0_2 >= D_80226D60[temp_a3].unk1)
            {
                phi_v0_2 = D_80226D60[temp_a3].unk1;
                if (phi_v0_2 == 0)
                {
                    break;
                }
                phi_v0_2--;
            }

            temp_a1_3 = &arg0->unk48;
            phi_a0 = D_80226D60[temp_a3].unk4[phi_v0_2];
            if (phi_a0 == NULL)
            {
                while (phi_v0_2 != 0xff)
                {
                    phi_a0 = D_80226D60[temp_a3].unk4[phi_v0_2];
                    if (phi_a0 != NULL)
                    {
                        break;
                    }
                    phi_v0_2--;
                }
            }

            temp_v0_11 = &arg0->unk14;
            if ((D_802214F8.first.pool.unk0 <= (u8 *) phi_a0 &&
                    (u8 *) phi_a0 <= D_802214F8.first.pool.unk0 + D_802214F8.first.pool.unk8) ||
                (D_802214F8.second.pool.unk0 <= (u8 *) phi_a0 &&
                    (u8 *) phi_a0 <= D_802214F8.second.pool.unk0 + D_802214F8.second.pool.unk8))
            {
                temp_v0_11->unk4 = phi_a0->unk4;
                temp_v0_11->unk0 = phi_a0->unk3;
                *temp_a1_3 = phi_a0;
            }
            else
            {
                D_80331D40 = phi_v0_2 + 0x20000;
                *temp_a1_3 = NULL;
            }
            break;

        case 0xc7:
            old = buf->unk0++;
            arg0->unk4.unk00 = *old;
            old = buf->unk0++;
            temp_t7_4 = sp58->unk1A + *old + arg0->unk1E + sp5C->unk10;
            if (temp_t7_4 >= 0x80)
            {
                temp_t7_4 = 0;
            }
            arg0->unk3 = temp_t7_4;

            if (arg0->unk4.unk00 & 0x80)
            {
                arg0->unk1C = *(buf->unk0++);
                break;
            }

            sp3A = *(buf->unk0++);
            if (sp3A & 0x80)
            {
                sp3A = (sp3A << 8) & 0x7f00;
                sp3A = *(buf->unk0++) | sp3A;
            }
            arg0->unk1C = sp3A;
            break;

        case 0xc8:
            arg0->unk4.unk00 = 0;
            break;

        default:
            loBits = instr & 0xf;
            switch (instr & 0xf0)
            {
            case 0xd0:
                sp3A = sp5C->unk88[loBits];
                arg0->unk24 = (f32) (sp3A * sp3A);
                break;
            case 0xe0:
                arg0->unk2 = sp5C->unk8C[loBits];
                break;
            }
        }
    }

    buf = &arg0->unk54;
    if (instr == 0xc0)
    {
        sp3A = *(buf->unk0++);
        if (sp3A & 0x80)
        {
            sp3A = (sp3A << 8) & 0x7f00;
            sp3A = *(buf->unk0++) | sp3A;
        }
        arg0->unk3C = sp3A;
        arg0->unk0b20 = 1;
    }
    else
    {
        arg0->unk0b20 = 0;

        if (sp58->unk0b2 == 1)
        {
            temp_t1_2 = instr & 0xc0;

            // phi_a0_3 = sp3A; // real assignment, or same vars?
            buf = &arg0->unk54;
            switch (temp_t1_2)
            {
            case 0x00:
                sp3A = *(buf->unk0++);
                if (sp3A & 0x80)
                {
                    sp3A = (sp3A << 8) & 0x7f00;
                    sp3A = *(buf->unk0++) | sp3A;
                }
                sp30 = *(buf->unk0++);
                arg0->unk2 = *(buf->unk0++);
                arg0->unk3A = sp3A;
                break;

            case 0x40:
                sp3A = *(buf->unk0++);
                if (sp3A & 0x80)
                {
                    sp3A = (sp3A << 8) & 0x7f00;
                    sp3A = *(buf->unk0++) | sp3A;
                }
                sp30 = *(buf->unk0++);
                arg0->unk2 = 0;
                arg0->unk3A = sp3A;
                break;

            case 0x80:
                sp30 = *(buf->unk0++);
                arg0->unk2 = *(buf->unk0++);
                sp3A = arg0->unk3A;
                break;
            }

            arg0->unk24 = sp30 * sp30;
            phi_v1 = instr - temp_t1_2;
        }
        else
        {
            temp_t1 = instr & 0xc0;

            buf = &arg0->unk54;

            // phi_a0_3 = sp3A;
            switch (temp_t1)
            {
            case 0x00:
                sp3A = *(buf->unk0++);
                if (sp3A & 0x80)
                {
                    sp3A = (sp3A << 8) & 0x7f00;
                    sp3A = *(buf->unk0++) | sp3A;
                }
                arg0->unk3A = sp3A;
                break;

            case 0x40:
                sp3A = arg0->unk38;
                break;

            case 0x80:
                sp3A = arg0->unk3A;
                break;
            }

            phi_v1 = instr - temp_t1;
        }

        arg0->unk3C = sp3A;
        arg0->unk3E = arg0->unk2 * sp3A / 256;
        if ((sp5C->unk0b20 && (sp58->unk2 & 0x40) != 0) ||
            sp58->unk0b10 || !sp58->unk0b8)
        {
            arg0->unk0b20 = 1;
        }
        else
        {
            if (sp58->unk18 == 0)
            {
                temp_t0 = phi_v1 + sp58->unk1A + arg0->unk1E;
                if (temp_t0 >= D_80226D60[sp58->unk5].unk2)
                {
                    temp_t0 = D_80226D60[sp58->unk5].unk2;
                    if (temp_t0 == 0)
                    {
                        arg0->unk0b20 = 1;
                        goto skip;
                    }
                    temp_t0--;
                }

                subA = D_80226D60[sp58->unk5].unk8[temp_t0];
                if (subA == NULL)
                {
                    arg0->unk0b20 = 1;
                }
                else
                {
                    arg0->unk14.unk4 = subA->unkC;
                    arg0->unk14.unk0 = subA->unk0;

                    arg0->unk28 = FLOAT_CAST(subA->unk1) / US_FLOAT(128.0);
                    arg0->unk4C = &subA->unk4;
                    arg0->unk20 = arg0->unk4C->unk4;
                }
skip:;
            }
            else
            {
                temp_v0_20 = phi_v1 + sp5C->unk10 + sp58->unk1A + arg0->unk1E;
                if (temp_v0_20 >= 0x80)
                {
                    arg0->unk0b20 = 1;
                }
                else
                {
                    phi_v1_2 = arg0->unk48;
                    if (arg0->unk48 == NULL)
                    {
                        phi_v1_2 = sp58->unk3C;
                    }

                    if (arg0->unk4.unk00 != 0)
                    {
                        phi_a1_3 = arg0->unk3;
                        if (phi_a1_3 < temp_v0_20)
                        {
                            phi_a1_3 = temp_v0_20;
                        }
                        if (phi_v1_2 != NULL)
                        {
                            if (phi_a1_3 < phi_v1_2->unk1)
                            {
                                phi_v0_5 = &phi_v1_2->unk8;
                            }
                            else if (phi_a1_3 <= phi_v1_2->unk2)
                            {
                                phi_v0_5 = &phi_v1_2->unk10;
                            }
                            else
                            {
                                phi_v0_5 = &phi_v1_2->unk18;
                            }
                            sp3F = (phi_v0_5 == arg0->unk4C);
                            arg0->unk4C = phi_v0_5;
                            phi_f0 = phi_v0_5->unk4;
                        }
                        else
                        {
                            arg0->unk4C = NULL;
                            phi_f0 = 1.0f;
                        }

                        temp_f2 = D_80332884[temp_v0_20] * phi_f0;
                        temp_f12 = D_80332884[arg0->unk3] * phi_f0;

                        temp_v0_22 = &arg0->unk4;
                        switch (arg0->unk4.unk00 & ~0x80)
                        {
                        case 1:
                        case 3:
                        case 5:
                            sp24 = temp_f2;
                            sp28 = temp_f12;
                            break;
                        case 2:
                        case 4:
                            sp24 = temp_f12;
                            sp28 = temp_f2;
                            break;
                        }

                        temp_v0_22->unk0C = sp24 / sp28 - US_FLOAT(1.0);
                        if (arg0->unk4.unk00 & 0x80)
                        {
                            temp_v0_22->unk08 = US_FLOAT(32512.0) * FLOAT_CAST(sp5C->unkA) /
                                ((f32) arg0->unk3C * (f32) D_80226D7C * FLOAT_CAST(arg0->unk1C));
                        }
                        else
                        {
                            temp_v0_22->unk08 = US_FLOAT(127.0) / FLOAT_CAST(arg0->unk1C);
                        }
                        temp_v0_22->unk04 = 0.0f;
                        arg0->unk20 = sp28;
                        if ((arg0->unk4.unk00 & ~0x80) == 5)
                        {
                            arg0->unk3 = temp_v0_20;
                        }
                    }
                    else if (phi_v1_2 != NULL)
                    {
                        if (temp_v0_20 < phi_v1_2->unk1)
                        {
                            phi_v0_6 = &phi_v1_2->unk8;
                        }
                        else if (temp_v0_20 <= phi_v1_2->unk2)
                        {
                            phi_v0_6 = &phi_v1_2->unk10;
                        }
                        else
                        {
                            phi_v0_6 = &phi_v1_2->unk18;
                        }
                        sp3F = (phi_v0_6 == arg0->unk4C);
                        arg0->unk4C = phi_v0_6;
                        arg0->unk20 = phi_v0_6->unk4 * D_80332884[temp_v0_20];
                    }
                    else
                    {
                        arg0->unk4C = NULL;
                        arg0->unk20 = D_80332884[temp_v0_20];
                    }
                }
            }
            arg0->unk40 = arg0->unk3C;
        }
    }

    if (arg0->unk0b20 == 1)
    {
        if (arg0->unk44 != NULL || arg0->unk0b10)
        {
            func_80318EC4(arg0);
        }
        return;
    }

    if (!arg0->unk0b10)
    {
        sp3D = 1;
    }
    else if (arg0->unk44 == NULL || arg0->unk1 == 0)
    {
        sp3D = 1;
    }
    else if (sp3F == 0)
    {
        func_80318EC4(arg0);
        sp3D = 1;
    }
    else
    {
        sp3D = 0;
        if (arg0->unk4C == NULL)
        {
            func_80319164(arg0->unk44, arg0);
        }
    }

    if (sp3D != 0)
    {
        arg0->unk44 = func_803198E0(arg0);
    }

    if (arg0->unk44 != NULL && arg0->unk44->unk2C == arg0)
    {
        func_8031A478(arg0->unk44);
    }
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_8031B0A4_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_8031B0A4_us.s")
#endif

u8 func_8031BEB0(struct Struct80225DD8 *arg0, u8 arg1, struct InstrumentSomething **arg2, struct Substruct80225DD8 *arg3)
{
    struct InstrumentSomething *a2;
    UNUSED u32 pad;

    if (arg1 >= D_80226D60[arg0->unk5].unk1)
    {
        arg1 = D_80226D60[arg0->unk5].unk1;
        if (arg1 == 0)
        {
            return 0;
        }
        arg1--;
    }

    a2 = D_80226D60[arg0->unk5].unk4[arg1];
    if (a2 == NULL)
    {
        struct Struct80225DD8 sp10 = *arg0;

        while (arg1 != 0xff)
        {
            a2 = D_80226D60[sp10.unk5].unk4[arg1];
            if (a2 != NULL)
                break;
            arg1--;
        }
    }

    if (((u32)D_802214F8.first.pool.unk0 <= (u32)a2 &&
            (u32)a2 <= (u32)(D_802214F8.first.pool.unk0 + D_802214F8.first.pool.unk8)) ||
        ((u32)D_802214F8.second.pool.unk0 <= (u32)a2 &&
            (u32)a2 <= (u32)(D_802214F8.second.pool.unk0 + D_802214F8.second.pool.unk8)))
    {
        arg3->unk4 = a2->unk4;
        arg3->unk0 = a2->unk3;
        *arg2 = a2;
        arg1++;
        return arg1;
    }

    D_80331D40 = arg1 + 0x20000;
    *arg2 = NULL;
    return 0;
}

void SetInstrument(struct Struct80225DD8 *arg0, u8 arg1)
{
    if (arg1 >= 0x80)
    {
        arg0->unk18 = arg1;
        arg0->unk3C = NULL;
    }
    else if (arg1 == 0x7f)
    {
        arg0->unk18 = 0;
        arg0->unk3C = INSTRUMENT_SENTINEL;
    }
    else
    {
        arg0->unk18 = func_8031BEB0(arg0, arg1, &arg0->unk3C, &arg0->unk78);
        if (arg0->unk18 == 0)
        {
            arg0->unk0b8 = 0;
            return;
        }
    }
    arg0->unk0b8 = 1;
}

void func_8031C0C4(struct Struct80225DD8 *arg0, u8 arg1)
{
    arg0->unk20 = FLOAT_CAST(arg1) / US_FLOAT(127.0);
}

#ifdef NON_MATCHING
void func_8031C104(struct Struct80225DD8 *arg0)
{
    u16 sp5A;
    s8 value; // sp53
    u8 sp38;
    u8 instr; // v1, s1
    u8 loBits; // t0, a0
    struct Interface1Buffer *buf;
    struct Struct80222A18 *unk40;
    u8 temp;
    s8 tempSigned;
    s32 temp32;
    s32 i;
    u8 temp2;

    if (!arg0->unk0b80)
    {
        return;
    }

    if (arg0->unk0b20)
    {
        for (i = 0; i < 4; i++)
        {
            if (arg0->unk44[i] != 0)
            {
                func_8031B0A4(arg0->unk44[i]);
            }
        }
        return;
    }

    unk40 = arg0->unk40;
    if (unk40->unk0b20 && (arg0->unk2 & 0x80) != 0)
    {
        return;
    }

    if (arg0->unk16 != 0)
    {
        arg0->unk16--;
    }

    buf = &arg0->unk5C;
    if (arg0->unk16 == 0)
    {
        for (;;)
        {
            instr = func_8031B01C(buf);
            if (instr == 0xff)
            {
                // This fixes a reordering in 'case 0x90', somehow
                sp5A = buf->unk18;
                if (sp5A == 0)
                {
                    func_8031AAD0(arg0);
                    break;
                }
                buf->unk18--,
                buf->unk0 = buf->unk4[buf->unk18];
            }
            if (instr == 0xfe)
            {
                break;
            }
            if (instr == 0xfd)
            {
                arg0->unk16 = func_8031B060(buf);
                break;
            }
            if (instr == 0xf3)
            {
                arg0->unk0b20 = 1;
                break;
            }

            // (new_var = instr fixes order of s1/s2, but causes a reordering
            // towards the bottom of the function)
            if (instr > 0xc0)
            {
                switch (instr)
                {
                case 0xfc:
                    sp5A = func_8031B030(buf);
                    buf->unk18++,
                    buf->unk4[buf->unk18 - 1] = buf->unk0;
                    buf->unk0 = unk40->unk14 + sp5A;
                    break;
                case 0xf8:
                    buf->unk14[buf->unk18] = func_8031B01C(buf);
                    buf->unk18++,
                    buf->unk4[buf->unk18 - 1] = buf->unk0;
                    break;
                case 0xf7:
                    buf->unk14[buf->unk18 - 1]--;
                    if (buf->unk14[buf->unk18 - 1] != 0)
                    {
                        buf->unk0 = buf->unk4[buf->unk18 - 1];
                    }
                    else
                    {
                        buf->unk18--;
                    }
                    break;
                case 0xf6:
                    buf->unk18--;
                    break;
                case 0xf5:
                case 0xf9:
                case 0xfa:
                case 0xfb:
                    sp5A = func_8031B030(buf);
                    if (instr == 0xfa && value != 0) break;
                    if (instr == 0xf9 && value >= 0) break;
                    if (instr == 0xf5 && value < 0) break;
                    buf->unk0 = unk40->unk14 + sp5A;
                    break;
                case 0xf2:
                    // arg0->unk80 should live in a saved register
                    func_803192FC(arg0->unk80);
                    temp = func_8031B01C(buf);
                    func_80319428(arg0->unk80, temp);
                    break;
                case 0xf1:
                    func_803192FC(arg0->unk80);
                    break;
                case 0xc2:
                    sp5A = func_8031B030(buf);
                    arg0->unk30 = (void *)(unk40->unk14 + sp5A);
                    break;
                case 0xc5:
                    if (value != -1)
                    {
                        sp5A = (*arg0->unk30)[value][1] + ((*arg0->unk30)[value][0] << 8);
                        arg0->unk30 = (void *)(unk40->unk14 + sp5A);
                    }
                    break;
                case 0xc1:
                    SetInstrument(arg0, func_8031B01C(buf));
                    break;
                case 0xc3:
                    arg0->unk0b2 = 0;
                    break;
                case 0xc4:
                    arg0->unk0b2 = 1;
                    break;
                case 0xdf:
                    func_8031C0C4(arg0, func_8031B01C(buf));
                    break;
                case 0xe0:
                    arg0->unk1C = FLOAT_CAST(func_8031B01C(buf)) / US_FLOAT(128.0);
                    break;
                case 0xde:
                    sp5A = func_8031B030(buf);
                    arg0->unk2C = FLOAT_CAST(sp5A) / US_FLOAT(32768.0);
                    break;
                case 0xd3:
                    temp = func_8031B01C(buf) + 0x7f;
                    arg0->unk2C = D_80332488[temp];
                    break;
                case 0xdd:
                    arg0->unk24 = FLOAT_CAST(func_8031B01C(buf)) / US_FLOAT(128.0);
                    break;
                case 0xdc:
                    arg0->unk28 = FLOAT_CAST(func_8031B01C(buf)) / US_FLOAT(128.0);
                    break;
                case 0xdb:
                    tempSigned = *buf->unk0;
                    buf->unk0++;
                    arg0->unk1A = tempSigned;
                    break;
                case 0xda:
                    sp5A = func_8031B030(buf);
                    arg0->unk78.unk4 = (struct SubstructInstrumentSomething *)
                        (unk40->unk14 + sp5A);
                    break;
                case 0xd9:
                    arg0->unk78.unk0 = func_8031B01C(buf);
                    break;
                case 0xd8:
                    arg0->unkE = func_8031B01C(buf) * 8;
                    arg0->unkA = 0;
                    arg0->unk12 = 0;
                    break;
                case 0xd7:
                    arg0->unk8 = arg0->unkC = func_8031B01C(buf) * 32;
                    arg0->unk10 = 0;
                    break;
                case 0xe2:
                    arg0->unkA = func_8031B01C(buf) * 8;
                    arg0->unkE = func_8031B01C(buf) * 8;
                    arg0->unk12 = func_8031B01C(buf) * 16;
                    break;
                case 0xe1:
                    arg0->unk8 = func_8031B01C(buf) * 32;
                    arg0->unkC = func_8031B01C(buf) * 32;
                    arg0->unk10 = func_8031B01C(buf) * 16;
                    break;
                case 0xe3:
                    arg0->unk14 = func_8031B01C(buf) * 16;
                    break;
                case 0xd6:
                    temp = func_8031B01C(buf);
                    if (temp == 0)
                    {
                        temp = D_80226D7E;
                    }
                    arg0->unk6 = temp;
                    break;
                case 0xd4:
                    arg0->unk3 = func_8031B01C(buf);
                    break;
                case 0xc6:
                    temp = func_8031B01C(buf);
                    temp32 = ((u16 *)D_80226D58)[unk40->unk5];
                    temp = D_80226D58[temp32 + D_80226D58[temp32] - temp];
                    // temp should be in a saved register across this call
                    if (func_8031680C(&D_802214F8, 2, temp) != 0)
                    {
                        arg0->unk5 = temp;
                    }
                    break;
                case 0xc7:
                    // sp38 doesn't go on the stack
                    sp38 = value;
                    temp2 = func_8031B01C(buf);
                    sp5A = func_8031B030(buf);
                    unk40->unk14[sp5A] = sp38 + temp2;
                    break;
                case 0xc8:
                case 0xc9:
                case 0xcc:
                    temp = func_8031B01C(buf);
                    if (instr == 0xc8)
                    {
                        value -= temp;
                    }
                    else if (instr == 0xcc)
                    {
                        value = temp;
                    }
                    else
                    {
                        value &= temp;
                    }
                    break;
                case 0xca:
                    arg0->unk2 = func_8031B01C(buf);
                    break;
                case 0xcb:
                    sp5A = func_8031B030(buf);
                    value = unk40->unk14[sp5A + value];
                    break;
                case 0xd0:
                    arg0->unk0b4 = func_8031B01C(buf);
                    break;
                case 0xd1:
                    arg0->unk1 = func_8031B01C(buf);
                    break;
                case 0xd2:
                    arg0->unk78.unk2 = func_8031B01C(buf) << 8;
                    break;
                case 0xe4:
                    if (value != -1)
                    {
                        u8 (*thingy)[2] = *arg0->unk30;
                        buf->unk18++,
                        buf->unk4[buf->unk18 - 1] = buf->unk0;
                        sp5A = thingy[value][1] + (thingy[value][0] << 8);
                        buf->unk0 = unk40->unk14 + sp5A;
                    }
                    break;
                }
            }
            else
            {
                // loBits is recomputed a lot
                loBits = instr & 0xf;
                // #define loBits (instr & 0xf)
                switch (instr & 0xf0)
                {
                case 0x00:
                    if (arg0->unk44[loBits] != 0)
                    {
                        value = arg0->unk44[loBits]->unk0b40;
                    }
                    break;
                case 0x70:
                    arg0->unk54[loBits] = value;
                    break;
                case 0x80:
                    value = arg0->unk54[loBits];
                    if (loBits < 4)
                    {
                        arg0->unk54[loBits] = -1;
                    }
                    break;
                case 0x50:
                    value -= arg0->unk54[loBits];
                    break;
                case 0x90:
                    sp5A = func_8031B030(buf);
                    if (func_8031A920(arg0, loBits) == 0)
                    {
                        arg0->unk44[loBits]->unk54.unk0 = unk40->unk14 + sp5A;
                    }
                    break;
                case 0xa0:
                    func_8031AA4C(arg0, loBits);
                    break;
                case 0xb0:
                    if (value != -1 && func_8031A920(arg0, loBits) != -1)
                    {
                        temp = (*arg0->unk30)[value][0] + ((*arg0->unk30)[value][1] << 8);
                        arg0->unk44[loBits]->unk54.unk0 = unk40->unk14 + temp;
                    }
                    break;
                case 0x60:
                    arg0->unk4 = loBits;
                    break;
                case 0x10:
                    sp5A = func_8031B030(buf);
                    func_8031AD80(unk40, loBits, unk40->unk14 + sp5A);
                    break;
                case 0x20:
                    func_8031AAD0(unk40->unk2C[loBits]);
                    break;
                case 0x30:
                    unk40->unk2C[loBits]->unk54[func_8031B01C(buf)] = value;
                    break;
                case 0x40:
                    value = unk40->unk2C[loBits]->unk54[func_8031B01C(buf)];
                    break;
                }
            }
        }
    }

    for (i = 0; i < 4; i++)
    {
        if (arg0->unk44[i] != 0)
        {
            func_8031B0A4(arg0->unk44[i]);
        }
    }
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_8031C104_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_8031C104_us.s")
#endif

void func_8031CAD4(struct Struct80222A18 *arg0)
{
    u8 instr;
    u8 loBits;
    u8 temp;
    s8 tempSigned;
    s32 value;
    s32 i;
    u16 u16v;
    u8 *tempPtr;
    struct Interface1Buffer *buf;

    if (arg0->unk0b80 == 0)
    {
        return;
    }

    if (arg0->unk0b8 == 1)
    {
        if (arg0->mesg2 == NULL)
        {
            return;
        }
        if (arg0->size == 0)
        {
            arg0->unk0b8 = 0;
            func_8031784C(arg0->mem, D_80226D54->seqArray[arg0->unk7].offset, arg0->unk8, arg0->unk9);
            D_80226D60[arg0->unk7].unk1 = arg0->unk8;
            D_80226D60[arg0->unk7].unk2 = arg0->unk9;
            D_80226D60[arg0->unk7].unk4 = arg0->mem->subB;
            D_80226D60[arg0->unk7].unk8 = arg0->mem->header;
            D_802218D0[arg0->unk7] = 2;
        }
        else
        {
            osCreateMesgQueue(&arg0->mesgQueue2, &arg0->mesg2, 1);
            arg0->mesg2 = NULL;
            func_803170A0(&arg0->currentDevAddr, &arg0->currentMemAddr, &arg0->size, &arg0->mesgQueue2, &arg0->ioMesg2);
        }
        return;
    }

    if (arg0->unk0b10 == 1)
    {
        if (arg0->mesg1 == NULL)
        {
            return;
        }
        arg0->unk0b10 = 0;
        D_80221910[arg0->unk5] = 2;
    }

    if ((D_80221910[arg0->unk5] < 2) != 0 || (D_802218D0[arg0->unk6[0]] < 2) != 0)
    {
        func_8031AE24(arg0);
        return;
    }

    D_80221910[arg0->unk5] = 2;
    D_802218D0[arg0->unk6[0]] = 2;
    if (arg0->unk0b20 && (arg0->unk4 & 0x80) != 0)
    {
        return;
    }

    arg0->unkC += arg0->unkA;
    if (arg0->unkC < D_80226D7C)
    {
        return;
    }
    arg0->unkC -= (u16) D_80226D7C;

    buf = &arg0->unk6C;
    if (arg0->unk12 >= 2)
    {
        arg0->unk12--;
    }
    else
    {
        for (;;)
        {
            instr = func_8031B01C(buf);
            if (instr == 0xff)
            {
                if (buf->unk18 == 0)
                {
                    func_8031AE24(arg0);
                    break;
                }
                buf->unk18--,
                buf->unk0 = buf->unk4[buf->unk18];
            }

            if (instr == 0xfd)
            {
                arg0->unk12 = func_8031B060(buf);
                break;
            }

            if (instr == 0xfe)
            {
                arg0->unk12 = 1;
                break;
            }

            if (instr >= 0xc0)
            {
                switch (instr)
                {
                case 0xff:
                    break;

                case 0xfc:
                    u16v = func_8031B030(buf);
                    buf->unk18++,
                    buf->unk4[buf->unk18 - 1] = buf->unk0;
                    buf->unk0 = arg0->unk14 + u16v;
                    break;

                case 0xf8:
                    buf->unk14[buf->unk18] = func_8031B01C(buf);
                    buf->unk18++,
                    buf->unk4[buf->unk18 - 1] = buf->unk0;
                    break;

                case 0xf7:
                    buf->unk14[buf->unk18 - 1]--;
                    if (buf->unk14[buf->unk18 - 1] != 0)
                    {
                        buf->unk0 = buf->unk4[buf->unk18 -  1];
                    }
                    else
                    {
                        buf->unk18--;
                    }
                    break;

                case 0xf5:
                case 0xf9:
                case 0xfa:
                case 0xfb:
                    u16v = func_8031B030(buf);
                    if (instr == 0xfa && value != 0) break;
                    if (instr == 0xf9 && value >= 0) break;
                    if (instr == 0xf5 && value < 0) break;
                    buf->unk0 = arg0->unk14 + u16v;
                    break;

                case 0xf2:
                    func_803192FC(arg0->unk90);
                    func_80319428(arg0->unk90, func_8031B01C(buf));
                    break;

                case 0xf1:
                    func_803192FC(arg0->unk90);
                    break;

                case 0xdf:
                    arg0->unk10 = 0;
                    // fallthrough

                case 0xde:
                    arg0->unk10 += (s8) func_8031B01C(buf);
                    break;

                case 0xdd:
                case 0xdc:
                    temp = func_8031B01C(buf);
                    if (instr == 0xdd)
                    {
#ifdef VERSION_JP
                        arg0->unkA = temp;
#else
                        arg0->unkA = temp * 0x30;
#endif
                    }
                    else
                    {
#ifdef VERSION_JP
                        arg0->unkA += (s8) temp;
#else
                        arg0->unkA += (s8) temp * 0x30;
#endif
                    }

                    if (arg0->unkA > D_80226D7C)
                    {
                        arg0->unkA = D_80226D7C;
                    }

                    if ((s16) arg0->unkA <= 0)
                    {
                        arg0->unkA = 1;
                    }
                    break;

                case 0xdb:
                    temp = func_8031B01C(buf);
                    switch (arg0->unk2)
                    {
                    case 2:
                        if (arg0->unkE != 0)
                        {
                            f32 scaled = FLOAT_CAST(temp) / US_FLOAT(127.0);
                            arg0->unk1C = (scaled - arg0->unk18) / FLOAT_CAST(arg0->unkE);
                            break;
                        }
                        // fallthrough
                    case 0:
                        arg0->unk18 = FLOAT_CAST(temp) / US_FLOAT(127.0);
                        break;
                    case 1:
                    case 4:
                        arg0->unk20 = FLOAT_CAST(temp) / US_FLOAT(127.0);
                        break;
                    }
                    break;

                case 0xda:
                    tempSigned = func_8031B01C(buf);
                    arg0->unk18 = arg0->unk18 + (f32) tempSigned / US_FLOAT(127.0);
                    break;

                case 0xd7:
                    u16v = func_8031B030(buf);
                    func_8031AB84(arg0, u16v);
                    break;

                case 0xd6:
                    u16v = func_8031B030(buf);
                    func_8031ACB0(arg0, u16v);
                    break;

                case 0xd5:
                    tempSigned = func_8031B01C(buf);
                    arg0->unk24 = (f32) tempSigned / US_FLOAT(127.0);
                    break;

                case 0xd4:
                    arg0->unk0b20 = 1;
                    break;

                case 0xd3:
                    arg0->unk4 = func_8031B01C(buf);
                    break;

                case 0xd2:
                case 0xd1:
                    u16v = func_8031B030(buf);
                    tempPtr = arg0->unk14 + u16v;
                    if (instr == 0xd2)
                    {
                        arg0->unk88 = tempPtr;
                    }
                    else
                    {
                        arg0->unk8C = tempPtr;
                    }
                    break;

                case 0xd0:
                    arg0->unk3 = func_8031B01C(buf);
                    break;

                case 0xcc:
                    value = func_8031B01C(buf);
                    break;

                case 0xc9:
                    value = func_8031B01C(buf) & value;
                    break;

                case 0xc8:
                    value = value - func_8031B01C(buf);
                    break;
                }
            }
            else
            {
                loBits = instr & 0xf;
                switch (instr & 0xf0)
                {
                case 0x00:
                    if (IS_SPECIAL(arg0->unk2C[loBits]) == 1)
                    {
                        value = arg0->unk2C[loBits]->unk0b40;
                    }
                    break;
                case 0x10:
                    break;
                case 0x20:
                    break;
                case 0x40:
                    break;
                case 0x50:
                    value -= arg0->unk1;
                    break;
                case 0x60:
                    break;
                case 0x70:
                    arg0->unk1 = value;
                    break;
                case 0x80:
                    value = arg0->unk1;
                    break;
                case 0x90:
                    u16v = func_8031B030(buf);
                    func_8031AD80(arg0, loBits, arg0->unk14 + u16v);
                    break;
                case 0xa0:
                    break;
                case 0xd8: // (this makes no sense)
                    break;
                case 0xd9:
                    break;
                }
            }
        }
    }

    for (i = 0; i < 0x10; i++)
    {
        if (arg0->unk2C[i] != &D_80225DD8)
        {
            func_8031C104(arg0->unk2C[i]);
        }
    }
}

void func_8031D384(UNUSED s32 arg0)
{
    s32 i;
    for (i = 0; i < 3; i++)
    {
        if (D_80222A18[i].unk0b80 == 1)
        {
            func_8031CAD4(D_80222A18 + i);
            func_80319E78(D_80222A18 + i);
        }
    }
    func_80319BC8();
    func_80318908();
}

void func_8031D42C(u32 arg0)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];
    temp->unk0b20 = 0;
    temp->unk12 = 0;
    temp->unk2 = 0;
    temp->unkE = 0;
    temp->unkC = 0;
#ifdef VERSION_JP
    temp->unkA = 0x78;
#else
    temp->unkA = 0x1680;
#endif
    temp->unk10 = 0;
    temp->unk4 = 0xe0;
    temp->unk3 = 0;
    temp->unk88 = D_80332A84;
    temp->unk8C = D_80332A94;
    temp->unk18 = 1.0f;
    temp->unk1C = 0.0f;
    temp->unk20 = 0.0f;
    temp->unk24 = 0.5f;
}

void func_8031D4B8(void)
{
    // Initialization function, called from InitAudioSystem
    s32 i, j;

    for (i = 0; i < 32; i++)
    {
        D_80222DD8[i].unk40 = 0;
        D_80222DD8[i].unk0b80 = 0;
    }

    for (i = 0; i < 32; i++)
    {
        //! Size of wrong array. Zeroes out second half of D_80222DD8[0],
        // all of D_80222DD8[1..31], and part of D_802245D8[0].
        // However, this is only called at startup, so it's pretty harmless.
        for (j = 0; j < ARRAY_COUNT(D_802245D8); j++)
        {
            D_80222DD8[i].unk44[j] = NULL;
        }
    }

    func_8031AF74();

    for (i = 0; i < ARRAY_COUNT(D_802245D8); i++)
    {
        D_802245D8[i].unk50 = 0;
        D_802245D8[i].unk0b80 = 0;
    }

    for (i = 0; i < 3; i++)
    {
        for (j = 0; j < 0x10; j++)
        {
            D_80222A18[i].unk2C[j] = &D_80225DD8;
        }

        D_80222A18[i].unk1 = -1;
        D_80222A18[i].unk0b8 = 0;
        D_80222A18[i].unk0b10 = 0;
        func_803191F8(D_80222A18[i].unk90);
        func_8031D42C(i);
    }
}
