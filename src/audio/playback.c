#include <ultra64.h>

#include "sm64.h"
#include "dac.h"
#include "data.h"
#include "interface_1.h"
#include "playback.h"
#include "something.h"
#include "playback2.h"

#ifdef VERSION_JP
#define US_FLOAT(x) x
#else
#define US_FLOAT(x) x ## f
#endif

void func_80318870(struct Struct_func_80318870 *arg0)
{
    if (arg0->unk2C->unk14.unk0 == 0)
    {
        func_8031A564(&arg0->unk54, arg0->unk2C->unk50->unk78.unk4, &arg0->unk8);
    }
    else
    {
        func_8031A564(&arg0->unk54, arg0->unk2C->unk14.unk4, &arg0->unk8);
    }
    arg0->unk54.unk01 = 1;
    func_803159C0(arg0);
    func_80315D94(arg0);
}

void func_803188E8(struct Struct_func_80318870 *arg0)
{
    func_80315DE0(arg0);
}

void func_80318908(void)
{
    f32 f2;
    f32 f0;
    u8 a3;
    f32 f20;
    f32 f12;
    f32 cap;
    struct Struct_func_80318870 *s0;
    struct SubStruct_func_80318870_2 *temp_v0;
    struct PlaybackListItem *it;
    s32 i;

    // Macro versions of func_80319564 and func_803195A4
    // (PREPEND does not actually need to be a macro, but it seems likely.)
#define PREPEND(item, head) \
    ((it = (item), it->prev != NULL) ? it : ( \
        it->prev = (head), \
        it->next = (head)->next, \
        (head)->next->prev = it, \
        (head)->next = it, \
        (head)->u.count++, \
        it->unkC = (head)->unkC, \
        it))
#define POP(item) \
    ((it = (item), it->prev == NULL) ? it : ( \
        it->prev->next = it->next, \
        it->next->prev = it->prev, \
        it->prev = NULL, \
        it))

    for (i = 0; i < D_80226D70; i++)
    {
        s0 = &D_80222A10[i];
        if (s0->unk4 != 0)
        {
            if (s0->unk4 == 1 || s0->unk0b10)
            {
                if (s0->unk8 == 0 || s0->unk0b10)
                {
                    if (s0->unk30 != MINUS_ONE)
                    {
                        func_803188E8(s0);
                        if (s0->unk30->unk50 != 0)
                        {
                            if (func_80319660(s0, s0->unk30) == 1)
                            {
                                func_803188E8(s0);
                                POP(&s0->unkA4);
                                PREPEND(&s0->unkA4, &D_80225EA8[0]);
                            }
                            else
                            {
                                func_8031A478(s0);
                                func_8031AEF4(&s0->unkA4.unkC[3], POP(&s0->unkA4));
                                s0->unk30 = MINUS_ONE;
                            }
                        }
                        else
                        {
                            func_803188E8(s0);
                            func_8031AEF4(&s0->unkA4.unkC[0], POP(&s0->unkA4));
                            s0->unk30 = MINUS_ONE;
                            continue;
                        }
                    }
                    else
                    {
                        func_803188E8(s0);
                        func_8031AEF4(&s0->unkA4.unkC[0], POP(&s0->unkA4));
                        continue;
                    }
                }
            }
            else
            {
                if (s0->unk54.unk01 == 0)
                {
                    func_803188E8(s0);
                    func_8031AEF4(&s0->unkA4.unkC[0], POP(&s0->unkA4));
                    continue;
                }
            }

            func_8031A584(&s0->unk54);
            func_8031A418(s0);
            temp_v0 = &s0->unk44;
            if (s0->unk4 == 1)
            {
                f0 = temp_v0->unk4;
                f20 = temp_v0->unk8;
                f12 = temp_v0->unkC;
                a3 = temp_v0->unk0;
            }
            else
            {
                f0 = s0->unk2C->unk34;
                f20 = s0->unk2C->unk2C;
                f12 = s0->unk2C->unk30;
                a3 = s0->unk2C->unk50->unk3;
            }

            f2 = s0->unk8;
            f0 *= s0->unk1C * s0->unk18;
            cap = 3.99992f;
            if (D_80226D64 != 32006)
            {
                f0 *= US_FLOAT(32000.0) / (f32) D_80226D64;
            }
            f0 = (f0 < cap ? f0 : cap);
            f2 *= 4.3498e-5f;
            f20 = f20 * f2 * f2;
            func_80315D88(s0, f0);
            func_803159EC(s0, f20, f12, a3);
            continue;
        }
    }
#undef PREPEND
#undef POP
}

void func_80318D18(struct SubStruct_func_80318870 *arg0, s32 arg1)
{
    struct Struct_func_80318870 *unk44;
    struct SubStruct_func_80318870_2 *sub;

    if (arg0 == MINUS_ONE || arg0->unk44 == NULL)
    {
        return;
    }

    unk44 = arg0->unk44;
    sub = &unk44->unk44;

    if (arg0->unk50 != 0 && arg0->unk50->unk1 == 0)
    {
        arg0->unk44 = NULL;
    }

    if (arg0 == unk44->unk30)
    {
        unk44->unk30 = MINUS_ONE;
    }

    if (arg0 != unk44->unk2C)
    {
        return;
    }

    arg0->unk1 = 0;
    if (unk44->unk54.unk01  != 6)
    {
        sub->unk4 = arg0->unk34;
        sub->unk8 = arg0->unk2C;
        sub->unkC = arg0->unk30;
        if (arg0->unk50 != NULL)
        {
            sub->unk0 = arg0->unk50->unk3;
        }
        unk44->unk4 = 1;
        unk44->unk28 = unk44->unk2C;
        unk44->unk2C = MINUS_ONE;
        if (arg1 == 7)
        {
            unk44->unk54.unk0E = 0x8000 / D_80226D7E;
            unk44->unk54.unk00 |= 0x10;
        }
        else
        {
            unk44->unk54.unk00 |= 0x20;
            if (arg0->unk14.unk0 == 0)
            {
                unk44->unk54.unk0E = arg0->unk50->unk78.unk0 * 0x18;
            }
            else
            {
                unk44->unk54.unk0E = arg0->unk14.unk0 * 0x18;
            }
            unk44->unk54.unk0C = (unk44->unk54.unk06 * arg0->unk50->unk78.unk2) / 0x10000;
        }
    }

    if (arg1 == 6)
    {
        func_803195A4(&unk44->unkA4);
        func_80319564(&unk44->unkA4.unkC[1], &unk44->unkA4);
    }
}

void func_80318EC4(struct SubStruct_func_80318870 *arg0)
{
    func_80318D18(arg0, 6);
}

void func_80318EE4(struct SubStruct_func_80318870 *arg0)
{
    func_80318D18(arg0, 7);
}

void func_80318F04(struct Struct_func_80318870 *arg0, struct SubStruct_func_80318870 *arg1)
{
    s32 i;
    s32 j;
    s32 pos;
    s32 stepSize;
    s32 offset;
    u8 lim;
    u8 origUnk5 = arg0->unk5;

    if (arg1->unk20 < US_FLOAT(1.0))
    {
        arg0->unk5 = 64;
        arg1->unk20 *= US_FLOAT(1.0465);
        stepSize = 1;
    }
    else if (arg1->unk20 < US_FLOAT(2.0))
    {
        arg0->unk5 = 32;
        arg1->unk20 *= US_FLOAT(0.52325);
        stepSize = 2;
    }
    else if (arg1->unk20 < US_FLOAT(4.0))
    {
        arg0->unk5 = 16;
        arg1->unk20 *= US_FLOAT(0.26263);
        stepSize = 4;
    }
    else
    {
        arg0->unk5 = 8;
        arg1->unk20 *= US_FLOAT(0.13081);
        stepSize = 8;
    }

    if (arg0->unk5 == origUnk5 && arg1->unk50->unk18 == arg0->unk6)
    {
        return;
    }

    arg0->unk6 = (u8) arg1->unk50->unk18;
    for (i = -1, pos = 0; pos < 0x40; pos += stepSize)
    {
        i++;
        arg0->unk34->unk110[i] = gWaveSamples[arg1->unk50->unk18 - 0x80][pos];
    }

    for (offset = arg0->unk5; offset < 0x40; offset += arg0->unk5)
    {
        lim = arg0->unk5;
        if (offset < 0 || offset > 0)
        {
            for (j = 0; j < lim; j++)
            {
                arg0->unk34->unk110[offset + j] = arg0->unk34->unk110[j];
            }
        }
        else
        {
            for (j = 0; j < lim; j++)
            {
                arg0->unk34->unk110[offset + j] = arg0->unk34->unk110[j];
            }
        }
    }

    osWritebackDCache(arg0->unk34->unk110, sizeof(arg0->unk34->unk110));
}

void func_80319164(struct Struct_func_80318870 *arg0, struct SubStruct_func_80318870 *arg1)
{
    s32 unk5 = arg0->unk5;
    func_80318F04(arg0, arg1);
    if (unk5 != 0)
    {
        arg0->unk14 *= arg0->unk5 / unk5;
    }
    else
    {
        arg0->unk14 = 0;
    }
}

void func_803191E8(struct PlaybackListItem *head)
{
    head->prev = head;
    head->next = head;
    head->u.count = 0;
}

void func_803191F8(struct PlaybackListItem *headList)
{
    func_803191E8(&headList[0]);
    func_803191E8(&headList[1]);
    func_803191E8(&headList[2]);
    func_803191E8(&headList[3]);
    headList[0].unkC = headList;
    headList[1].unkC = headList;
    headList[2].unkC = headList;
    headList[3].unkC = headList;
}

void func_80319248(void)
{
    s32 i;

    func_803191F8(D_80225EA8);
    for (i = 0; i < D_80226D70; i++)
    {
        D_80222A10[i].unkA4.u.value = &D_80222A10[i];
        D_80222A10[i].unkA4.prev = NULL;
        func_8031AEF4(&D_80225EA8[0], &D_80222A10[i].unkA4);
    }
}

void func_803192FC(struct PlaybackListItem *headList)
{
    struct PlaybackListItem *s0;
    struct PlaybackListItem *s2;
    s32 i;
    struct PlaybackListItem *s3;
    s32 j;

    for (i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            s2 = &headList[0];
            s3 = &D_80225EA8[0];
            break;

        case 1:
            s2 = &headList[1];
            s3 = &D_80225EA8[1];
            break;

        case 2:
            s2 = &headList[2];
            s3 = &D_80225EA8[2];
            break;

        case 3:
            s2 = &headList[3];
            s3 = &D_80225EA8[3];
            break;
        }

        j = 0;
        do
        {
            s0 = s2->next;
            if (s0 == s2)
                break;
            func_803195A4(s0);
            func_8031AEF4(s3, s0);
            j++;
        } while (j <= D_80226D70);
    }
}

void func_80319428(struct PlaybackListItem *headList, s32 count)
{
    s32 i;
    s32 j;
    struct Struct_func_80318870 *ret;
    struct PlaybackListItem *s1;
    struct PlaybackListItem *s2;

    func_803192FC(headList);

    for (i = 0, j = 0; j < count; i++)
    {
        if (i == 4)
            return;

        switch (i)
        {
        case 0:
            s1 = &D_80225EA8[0];
            s2 = &headList[0];
            break;

        case 1:
            s1 = &D_80225EA8[1];
            s2 = &headList[1];
            break;

        case 2:
            s1 = &D_80225EA8[2];
            s2 = &headList[2];
            break;

        case 3:
            s1 = &D_80225EA8[3];
            s2 = &headList[3];
            break;
        }

        while (j < count)
        {
            ret = func_8031AF34(s1);
            if (ret == 0)
                break;
            func_8031AEF4(s2, &ret->unkA4);
            j++;
        }
    }
}

void func_80319564(struct PlaybackListItem *head, struct PlaybackListItem *item)
{
    // add 'item' to the front of the list given by 'head', if it's not in any list
    if (item->prev == NULL)
    {
        item->prev = head;
        item->next = head->next;
        head->next->prev = item;
        head->next = item;
        head->u.count++;
        item->unkC = head->unkC;
    }
}

void func_803195A4(struct PlaybackListItem *item)
{
    // remove 'item' from the list it's in, if any
    if (item->prev != NULL)
    {
        item->prev->next = item->next;
        item->next->prev = item->prev;
        item->prev = NULL;
    }
}

struct Struct_func_80318870 *func_803195D0(struct PlaybackListItem *head, s32 arg1)
{
    struct PlaybackListItem *cur = head->next;
    struct PlaybackListItem *best;

    if (cur == head)
    {
        return NULL;
    }

    best = cur;
    for (; cur != head; cur = cur->next)
    {
        if (best->u.value->unk4 >= cur->u.value->unk4)
        {
            best = cur;
        }
    }

    if (arg1 < best->u.value->unk4)
    {
        return NULL;
    }

    func_803195A4(best);
    return best->u.value;
}

s32 func_80319660(struct Struct_func_80318870 *arg0, struct SubStruct_func_80318870 *arg1)
{
    arg0->unk28 = MINUS_ONE;
    arg0->unk2C = arg1;
    arg0->unk4 = arg1->unk50->unk4;
    if ((D_802218D0[arg1->unk50->unk5] < 2) != 0)
    {
        return 1;
    }

    arg0->unk7 = arg1->unk50->unk5;
    arg0->unk0b1 = arg1->unk50->unk0b4;
    arg0->unk24 = arg1->unk4C;
    arg1->unk1 = 3;
    arg1->unk44 = arg0;
    arg1->unk50->unk34 = arg0;
    arg1->unk50->unk38 = arg1;
    if (arg0->unk24 == NULL)
    {
        func_80318F04(arg0, arg1);
    }
    func_80318870(arg0);
    return 0;
}

void func_80319728(struct Struct_func_80318870 *arg0, struct SubStruct_func_80318870 *arg1)
{
    func_80318EE4(arg0->unk2C);
    arg0->unk30 = arg1;
}

void func_8031975C(struct Struct_func_80318870 *arg0, struct SubStruct_func_80318870 *arg1)
{
    arg0->unk30 = arg1;
    arg0->unk4 = 1;
    arg0->unk54.unk0E = 0x8000 / D_80226D7E;
    arg0->unk54.unk00 |= 0x10;
}

struct Struct_func_80318870 *func_803197B4(struct PlaybackListItem *headList, struct SubStruct_func_80318870 *arg1)
{
    struct Struct_func_80318870 *a2 = func_8031AF34(&headList[0]);
    if (a2 != NULL)
    {
        if (func_80319660(a2, arg1) == 1)
        {
            func_80319564(&D_80225EA8[0], &a2->unkA4);
            return NULL;
        }

        func_80319564(&headList[3], &a2->unkA4);
    }
    return a2;
}

struct Struct_func_80318870 *func_80319830(struct PlaybackListItem *headList, struct SubStruct_func_80318870 *arg1)
{
    struct Struct_func_80318870 *a2 = func_8031AF34(&headList[1]);
    if (a2 != NULL)
    {
        func_8031975C(a2, arg1);
        func_8031AEF4(&headList[2], &a2->unkA4);
    }
    return a2;
}

struct Struct_func_80318870 *func_80319884(struct PlaybackListItem *headList, struct SubStruct_func_80318870 *arg1)
{
    struct Struct_func_80318870 *a2 = func_803195D0(&headList[3], arg1->unk50->unk4);
    if (a2 != 0)
    {
        func_80319728(a2, arg1);
        func_8031AEF4(&headList[2], &a2->unkA4);
    }
    return a2;
}

struct Struct_func_80318870 *func_803198E0(struct SubStruct_func_80318870 *arg0)
{
    struct Struct_func_80318870 *ret;
    u32 mask = arg0->unk50->unk1;

    if (mask & 1)
    {
        ret = arg0->unk44;
        if (ret != NULL && ret->unk28 == arg0)
        {
            func_8031975C(ret, arg0);
            func_803195A4(&ret->unkA4);
            func_8031AEF4(&D_80225EA8[2], &ret->unkA4);
            return ret;
        }
    }

    if (mask & 2)
    {
        if (!(ret = func_803197B4(arg0->unk50->unk80, arg0)) &&
            !(ret = func_80319830(arg0->unk50->unk80, arg0)) &&
            !(ret = func_80319884(arg0->unk50->unk80, arg0)))
        {
            arg0->unk1 = 0;
            return 0;
        }
        return ret;
    }

    if (mask & 4)
    {
        if (!(ret = func_803197B4(arg0->unk50->unk80, arg0)) &&
            !(ret = func_803197B4(arg0->unk50->unk40->unk90, arg0)) &&
            !(ret = func_80319830(arg0->unk50->unk80, arg0)) &&
            !(ret = func_80319830(arg0->unk50->unk40->unk90, arg0)) &&
            !(ret = func_80319884(arg0->unk50->unk80, arg0)) &&
            !(ret = func_80319884(arg0->unk50->unk40->unk90, arg0)))
        {
            arg0->unk1 = 0;
            return 0;
        }
        return ret;
    }

    if (mask & 8)
    {
        if (!(ret = func_803197B4(D_80225EA8, arg0)) &&
            !(ret = func_80319830(D_80225EA8, arg0)) &&
            !(ret = func_80319884(D_80225EA8, arg0)))
        {
            arg0->unk1 = 0;
            return 0;
        }
        return ret;
    }

    if (!(ret = func_803197B4(arg0->unk50->unk80, arg0)) &&
        !(ret = func_803197B4(arg0->unk50->unk40->unk90, arg0)) &&
        !(ret = func_803197B4(D_80225EA8, arg0)) &&
        !(ret = func_80319830(arg0->unk50->unk80, arg0)) &&
        !(ret = func_80319830(arg0->unk50->unk40->unk90, arg0)) &&
        !(ret = func_80319830(D_80225EA8, arg0)) &&
        !(ret = func_80319884(arg0->unk50->unk80, arg0)) &&
        !(ret = func_80319884(arg0->unk50->unk40->unk90, arg0)) &&
        !(ret = func_80319884(D_80225EA8, arg0)))
    {
        arg0->unk1 = 0;
        return 0;
    }
    return ret;
}

void func_80319BC8(void)
{
    struct Struct_func_80318870 *s0;
    struct PlaybackListItem *item;
    s32 i;
    s32 cond;

    for (i = 0; i < D_80226D70; i++)
    {
        s0 = &D_80222A10[i];
        if (s0->unk2C != MINUS_ONE)
        {
            cond = FALSE;
            if (!s0->unk2C->unk0b80 && s0->unk4 >= 2)
            {
                cond = TRUE;
            }
            else
            {
                item = &s0->unk2C->unk70;
                if (s0->unk2C->unk50 == NULL)
                {
                    func_8031AEF4(&D_80225E98, item);
                    func_8031AA10(s0->unk2C);
                    s0->unk4 = 1;
                }
                else if (s0->unk2C->unk50->unk40 == 0)
                {
                    func_8031AAD0(s0->unk2C->unk50);
                    s0->unk4 = 1;
                }
                else if (s0->unk2C->unk50->unk40->unk0b20)
                {
                    if (s0->unk2C->unk50->unk2 & 0xc0)
                    {
                        cond = TRUE;
                    }
                }
                else
                {
                    cond = FALSE;
                }
            }

            if (cond)
            {
                func_80318EE4(s0->unk2C);
                func_803195A4(&s0->unkA4);
                func_80319564(s0->unkA4.unkC, &s0->unkA4);
                s0->unk4 = 1;
            }
        }
    }
}

void func_80319D40(void)
{
    struct Struct_func_80318870 *temp;
    s32 i;

    for (i = 0; i < D_80226D70; i++)
    {
        temp = &D_80222A10[i];
        temp->unk0b80 = 0;
        temp->unk0b4 = 0;
        temp->unk0b2 = 0;
        temp->unk0b1 = 0;
        temp->unk4 = 0;
        temp->unk2C = MINUS_ONE;
        temp->unk30 = MINUS_ONE;
        temp->unk28 = MINUS_ONE;
        temp->unk40 = 0;
        temp->unk1 = 0;
        temp->unk5 = 0;
        temp->unk6 = 0;
        temp->unk3C = 0;
        temp->unk3E = 0;
        temp->unk38 = 0.0f;
        temp->unk41 = 0x3f;
        temp->unk44.unk8 = 0.0f;
        temp->unk8 = 0;
        temp->unk54.unk01 = 0;
        temp->unk54.unk00 = 0;
        temp->unk84.unkC = 0;
        temp->unk74.unk04 = 0.0f;
        temp->unk74.unk08 = 0.0f;
        temp->unk34 = soundAlloc(&D_802212C8, 0x190);
    }
}
