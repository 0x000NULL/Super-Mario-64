#include <ultra64.h>

#include "sm64.h"
#include "dma.h"
#include "dac.h"
#include "data.h"
#include "interface_1.h"
#include "interface_2.h"

#define ALIGN16(val) (((val) + 0xF) & ~0xF)

void BlockDmaCopy(u32 devAddr, void *vAddr, u32 nbytes)
{
    osInvalDCache(vAddr, (s32)nbytes);
    osPiStartDma(&D_80226520, OS_MESG_PRI_HIGH, OS_READ, devAddr, vAddr, nbytes, &D_80226500);
    osRecvMesg(&D_80226500, NULL, OS_MESG_BLOCK);
}

void func_80317034(u32 devAddr, void *vAddr, u32 nbytes, OSMesgQueue *queue, OSIoMesg *mesg)
{
    osInvalDCache(vAddr, (s32)nbytes);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, devAddr, vAddr, nbytes, queue);
}

void func_803170A0(u32 *devAddr, u8 **vAddr, s32 *size, OSMesgQueue *queue, OSIoMesg *mesg)
{
    s32 transfer = (*size < 0x1000 ? *size : 0x1000);
    *size -= transfer;
    osInvalDCache(*vAddr, transfer);
    osPiStartDma(mesg, OS_MESG_PRI_NORMAL, OS_READ, *devAddr, *vAddr, transfer, queue);
    *devAddr += transfer;
    *vAddr += transfer;
}

void func_8031715C()
{
    // (uses too much stack if declared as '(void)')
    u32 i;

    for (i = 0; i < D_80226B3C; i++)
    {
        struct Struct80226538 *temp = D_80226538 + i;
        if (temp->unkE != 0)
        {
            temp->unkE--;
            if (temp->unkE == 0)
            {
                temp->unkD = D_80226D4A;
                D_80226B48[D_80226D4A++] = (u8)i;
            }
        }
    }

    for (i = D_80226B3C; i < D_80226B38; i++)
    {
        struct Struct80226538 *temp = D_80226538 + i;
        if (temp->unkE != 0)
        {
            temp->unkE--;
            if (temp->unkE == 0)
            {
                temp->unkD = D_80226D4B;
                D_80226C48[D_80226D4B++] = (u8)i;
            }
        }
    }

    D_80226B40 = 0;
}

#ifdef NON_MATCHING

void *func_80317270(u8 *arg0, u32 arg1, s32 arg2, u8 *arg3)
{
    struct Struct80226538 *temp; // v1
    struct Struct80226538 *dma; // sp58, t0
    u32 transfer; // v0
    u32 devAddr; // s0
    u32 i; // a0
    u32 dmaIndex; // sp48, t2
    s32 hasDma = 0; // t4

    if (arg2 != 0 || *arg3 >= D_80226B3C)
    {
        for (i = D_80226B3C; i < D_80226B38; i++)
        {
            temp = D_80226538 + i;
            transfer = (u32) arg0 - temp->unk4;
            if ((s32)transfer >= 0 && temp->unkA - arg1 >= transfer)
            {
                if (temp->unkE == 0 && D_80226D4B != D_80226D49)
                {
                    if (temp->unkD != D_80226D49)
                    {
                        D_80226C48[temp->unkD] = D_80226C48[D_80226D49];
                        D_80226538[D_80226C48[D_80226D49]].unkD = temp->unkD;
                    }
                    D_80226D49++;
                }
                temp->unkE = 60;
                *arg3 = (u8)i;
                transfer = (u32) arg0 - temp->unk4;
                return temp->unk0 + transfer;
            }
            dma = temp;
        }

        if (D_80226D4B != D_80226D49 && arg2 != 0)
        {
            dmaIndex = D_80226C48[D_80226D49++];
            dma = &D_80226538[dmaIndex];
            hasDma = 1;
        }
    }
    else
    {
        dma = &D_80226538[*arg3];
        transfer = (u32) arg0 - dma->unk4;
        if ((s32)transfer >= 0 && dma->unkA - arg1 >= transfer)
        {
            if (dma->unkE == 0)
            {
                if (dma->unkD != D_80226D48)
                {
                    D_80226B48[dma->unkD] = D_80226B48[D_80226D48];
                    D_80226538[D_80226B48[D_80226D48]].unkD = dma->unkD;
                }
                D_80226D48++;
                transfer = (u32) arg0 - dma->unk4;
            }
            dma->unkE = 2;
            return dma->unk0 + transfer;
        }
    }

    if (!hasDma)
    {
        dmaIndex = D_80226B48[D_80226D48++];
        dma = &D_80226538[dmaIndex];
        hasDma = 1;
    }

    transfer = dma->unkA;
    devAddr = (u32) arg0 & ~0xF;
    dma->unkE = 2;
    dma->unk4 = devAddr;
    dma->unk8 = transfer;
#ifndef VERSION_JP
    osInvalDCache(dma->unk0, transfer);
#endif
    D_80226D84++;
    osPiStartDma(&D_80226000[D_80226D84 - 1], OS_MESG_PRI_NORMAL, OS_READ,
            devAddr, dma->unk0, transfer, &D_80225EE8);
    *arg3 = dmaIndex;
    return dma->unk0 + (u32) arg0 - devAddr;
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_80317270_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_80317270_us.s")
#endif

void func_8031758C(UNUSED s32 arg0)
{
    s32 i;
    s32 j;

    D_80226D68 = 0x510;
    for (i = 0; i < D_80226D70 * 3; i++)
    {
        D_80226538[D_80226B38].unk0 = soundAlloc(&D_802212C8, D_80226D68);
        if (D_80226538[D_80226B38].unk0 == NULL)
        {
            goto out1;
        }
        D_80226538[D_80226B38].unk4 = 0;
        D_80226538[D_80226B38].unk8 = 0;
        D_80226538[D_80226B38].unkC = 0;
        D_80226538[D_80226B38].unkE = 0;
        D_80226538[D_80226B38].unkA = D_80226D68;
        D_80226B38++;
    }
out1:

    for (i = 0; (u32) i < D_80226B38; i++)
    {
        D_80226B48[i] = (u8) i;
        D_80226538[i].unkD = (u8) i;
    }

    for (j = D_80226B38; j < 0x100; j++)
    {
        D_80226B48[j] = 0;
    }

    D_80226D48 = 0;
    D_80226D4A = (u8) D_80226B38;
    D_80226B3C = D_80226B38;
    D_80226D68 = 0x5a0;

    for (i = 0; i < D_80226D70; i++)
    {
        D_80226538[D_80226B38].unk0 = soundAlloc(&D_802212C8, D_80226D68);
        if (D_80226538[D_80226B38].unk0 == NULL)
        {
            goto out2;
        }
        D_80226538[D_80226B38].unk4 = 0;
        D_80226538[D_80226B38].unk8 = 0;
        D_80226538[D_80226B38].unkC = 0;
        D_80226538[D_80226B38].unkE = 0;
        D_80226538[D_80226B38].unkA = D_80226D68;
        D_80226B38++;
    }
out2:

    for (i = D_80226B3C; (u32) i < D_80226B38; i++)
    {
        D_80226C48[i - D_80226B3C] = (u8) i;
        D_80226538[i].unkD = (u8) (i - D_80226B3C);
    }

    for (j = D_80226B38; j < 0x100; j++)
    {
        D_80226C48[j] = D_80226B3C;
    }

    D_80226D49 = 0;
    D_80226D4B = D_80226B38 - D_80226B3C;
}

#ifndef static
static void unused_80317844(void)
{
    // With -O2 -framepointer, this never-invoked static function gets *almost*
    // optimized out, regardless of contents, leaving only "jr $ra, nop".
    // If not declared as static, it unnecessarily moves the stack pointer up
    // and down by 8.
}
#else
// Keep supporting the good old "#define static" hack.
#undef static
static void unused_80317844(void) {}
#define static
#endif

#ifdef NON_MATCHING
void func_8031784C(struct Struct_func_8031784C *mem, u8 *offset, u32 arg2, u32 arg3)
{
    // Make pointers into real pointers rather than indices
    struct InstrumentSomething *subB;
    struct InstrumentSomething **itSubB;
    u32 i;
    u32 memBase = (u32)mem;
    u32 offsetBase = (u32)offset;

#define INIT_SUB(subAndF32) \
    { \
        struct SubEntry **itSubEntry = &subAndF32.unk0; \
        if ((*itSubEntry) != 0) \
        { \
            /* Making these volatile gives correct codegen further down; it makes
             * lw/addiu/sw's happen in source order, and uses two registers...
             * It looks odd, though, so maybe they should not be volatile.
             * It might also be causing the extra register use.
             * Presumably subEntry and subEntry2 ought to have different types,
             * but that doesn't matter for codegen. */ \
            volatile struct SubEntry *subEntry, *subEntry2; \
            *itSubEntry = (void *)(memBase + (u32)(*itSubEntry)); \
            subEntry = *itSubEntry; \
            subEntry2 = *itSubEntry; \
            if (subEntry2->unk1 == 0) \
            { \
                void *a = subEntry2->unk4; \
                void *b = subEntry->unk8; \
                void *c = subEntry->unkC; \
                subEntry->unk4 = (void *) (offsetBase + (u32)a); \
                subEntry->unk8 = (void *) (memBase + (u32)b); \
                subEntry->unkC = (void *) (memBase + (u32)c); \
                subEntry->unk1 = 1; \
            } \
        } \
    }

    if (mem->header != 0)
    {
        if (arg3 != 0)
        {
            mem->header = (struct SubA **)(memBase + (u32)mem->header);
            if (arg3 != 0)
            {
                for (i = 0; i < arg3; i++)
                {
#if 0
                    // This doesn't work: Taking the address to mem->header[i]
                    // does an sll to figure out the lower loop limit.
                    volatile struct SubA *subA, *subA2;
                    struct SubA **h = &mem->header[i];
                    if (*h == 0) continue;
                    {
                        *h = (void *)(memBase + (u32)*h);
                        subA = *h;
                        subA2 = *h;
                        if (subA2->unk2 == 0)
                        {
                            void *d;
                            INIT_SUB(((struct SubA *)subA2)->unk4);
                            d = subA2->unkC;
                            subA->unk2 = 1;
                            subA->unkC = (void *) (memBase + (u32)d);
                        }
                    }
#else
                    // Neither does this: Using mem->header[i] directly
                    // deduplicates it -- subA and subA2 end up in the same
                    // register.
                    struct SubA **header = mem->header;
                    u32 h = (u32)header[i];
                    if (h != 0)
                    {
                        volatile struct SubA *subA, *subA2;
                        header[i] = (struct SubA *)(memBase + h);
                        subA = header[i];
                        subA2 = header[i];
                        if (subA->unk2 == 0)
                        {
                            void *d;
                            INIT_SUB(((struct SubA *)subA)->unk4);
                            d = subA->unkC;
                            subA2->unk2 = 1;
                            subA2->unkC = (void *) (memBase + (u32)d);
                        }
                    }
#endif
                }
            }
        }
    }

    if ((arg2 >= 1) && (arg2 >= 1) != 0)
    {
        itSubB = mem->subB;
        do
        {
            if (*itSubB)
            {
                *itSubB = (void *)(memBase + (u32)*itSubB);
                subB = *itSubB;

                if (subB->unk0 == 0)
                {
                    INIT_SUB(subB->unk8);
                    INIT_SUB(subB->unk10);
                    INIT_SUB(subB->unk18);

                    subB->unk0 = 1;
                    subB->unk4 = (void *) (memBase + (u32)subB->unk4);
                }
            }
            itSubB++;
        }
        while (itSubB != &mem->subB[arg2]);
    }
#undef INIT_SUB
}

#else
GLOBAL_ASM("asm/non_matchings/func_8031784C.s")
#endif

void *func_80317A88(s32 arg0, s32 arg1)
{
    UNUSED u32 pad1[4];
    u32 buf[4];
    u32 unk1, unk2;
    void *ret;
    u8 *data;
    s32 alloc;

    alloc = D_80226D50->seqArray[arg0].len + 0xf;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    data = D_80226D50->seqArray[arg0].offset;
    ret = func_803163DC(&D_802214F8, 1, alloc, arg1, arg0);
    if (ret == 0)
    {
        return 0;
    }

    BlockDmaCopy((u32)data, buf, 0x10);
    unk1 = buf[0];
    unk2 = buf[1];
    BlockDmaCopy((u32)(data + 0x10), ret, alloc);
    func_8031784C(ret, D_80226D54->seqArray[arg0].offset, unk1, unk2);
    D_80226D60[arg0].unk1 = (u8)unk1;
    D_80226D60[arg0].unk2 = (u8)unk2;
    // (Guessing at type)
    D_80226D60[arg0].unk4 = ((struct Struct_func_8031784C *)ret)->subB;
    D_80226D60[arg0].unk8 = ((struct Struct_func_8031784C *)ret)->header;
    D_802218D0[arg0] = 2;
    return ret;
}

void *func_80317BE4(s32 arg0, s32 arg1, struct Struct80222A18 *arg2)
{
    u32 unk8, unk9;
    UNUSED u32 pad1[2];
    u32 buf[4];
    UNUSED u32 pad2;
    s32 alloc;
    void *ret;
    u8 *data;
    OSMesgQueue *mesgQueue;

    alloc = D_80226D50->seqArray[arg0].len + 0xf;
    alloc = ALIGN16(alloc);
    alloc -= 0x10;
    data = D_80226D50->seqArray[arg0].offset;
    ret = func_803163DC(&D_802214F8, 1, alloc, arg1, arg0);
    if (ret == 0)
    {
        return 0;
    }

    BlockDmaCopy((u32)data, buf, 0x10);
    unk8 = buf[0];
    unk9 = buf[1];
    arg2->unk7 = (u8)arg0;
    arg2->unk8 = unk8;
    arg2->unk9 = unk9;
    arg2->currentMemAddr = ret;
    arg2->mem = ret;
    arg2->currentDevAddr = (u32)(data + 0x10);
    arg2->size = alloc;
    mesgQueue = &arg2->mesgQueue2;
    osCreateMesgQueue(mesgQueue, &arg2->mesg2, 1);
    arg2->mesg2 = NULL;
    arg2->unk0b8 = 1;
    func_803170A0(&arg2->currentDevAddr, &arg2->currentMemAddr, &arg2->size,
            mesgQueue, &arg2->ioMesg2);
    D_802218D0[arg0] = 1;
    return ret;
}

void *func_80317D1C(s32 arg0, s32 arg1)
{
    s32 alloc;
    void *ret;
    u8 *data;

    alloc = ALIGN16(D_80226D4C->seqArray[arg0].len + 0xf);
    data = D_80226D4C->seqArray[arg0].offset;
    ret = func_803163DC(&D_80221328, 1, alloc, arg1, arg0);
    if (ret == NULL)
    {
        return NULL;
    }
    BlockDmaCopy((u32)data, ret, alloc);
    D_80221910[arg0] = 2;
    return ret;
}

void *func_80317DC8(s32 arg0, s32 arg1, struct Struct80222A18 *arg2)
{
    s32 alloc;
    void *ret;
    u8 *data;
    OSMesgQueue *mesgQueue;

    alloc = D_80226D4C->seqArray[arg0].len + 0xf;
    alloc = ALIGN16(alloc);
    data = D_80226D4C->seqArray[arg0].offset;
    ret = func_803163DC(&D_80221328, 1, alloc, arg1, arg0);
    if (ret == NULL)
    {
        return NULL;
    }

    if (alloc <= 0x40)
    {
        BlockDmaCopy((u32)data, ret, alloc);
        D_80221910[arg0] = 2;
    }
    else
    {
        BlockDmaCopy((u32)data, ret, 0x40);
        mesgQueue = &arg2->mesgQueue1;
        osCreateMesgQueue(mesgQueue, &arg2->mesg1, 1);
        arg2->mesg1 = 0;
        arg2->unk0b10 = 1;
        func_80317034((u32) (data + 0x40), (u8*)ret + 0x40, alloc - 0x40,
                mesgQueue, &arg2->ioMesg1);
        D_80221910[arg0] = 1;
    }
    return ret;
}

u8 func_80317F04(u32 arg0, s32 *arg1, s32 *arg2)
{
    void *temp;
    u32 v0;
    u16 offset;
    u8 i;
    u8 ret;

    *arg2 = 0;
    *arg1 = 0;
    offset = ((u16 *)D_80226D58)[arg0] + 1;
    for (i = D_80226D58[offset - 1], ret = 0; i; i--)
    {
        offset++;
        v0 = D_80226D58[offset - 1];

        if ((D_802218D0[v0] >= 2) == 1)
        {
            temp = func_8031680C(&D_802214F8, 2, D_80226D58[offset - 1]);
        }
        else
        {
            temp = 0;
        }

        if (temp == 0)
        {
            (*arg2)++;
            ret = v0;
        }
        else
        {
            (*arg1)++;
        }
    }

    return ret;
}

void *func_8031804C(s32 arg0, u8 *arg1)
{
    void *ret;
    u32 v0;
    u16 offset;
    u8 i;

    offset = ((u16 *)D_80226D58)[arg0] + 1;
    for (i = D_80226D58[offset - 1]; i; i--)
    {
        offset++;
        v0 = D_80226D58[offset - 1];

        if ((D_802218D0[v0] >= 2) == 1)
        {
            ret = func_8031680C(&D_802214F8, 2, D_80226D58[offset - 1]);
        }
        else
        {
            ret = 0;
        }

        if (!ret)
        {
            ret = func_80317A88(v0, 2);
        }
    }
    *arg1 = v0;
    return ret;
}

void func_80318178(u32 arg0, u8 arg1)
{
    void *cond;
    u8 temp;

    if (arg0 >= D_80226D5C)
    {
        return;
    }

    D_80333EF4 = 0x19710515;
    if (arg1 & 2)
    {
        func_8031804C(arg0, &temp);
    }

    if (arg1 & 1)
    {
        if ((D_802218D0[arg0] >= 2) == 1)
        {
            cond = func_8031680C(&D_80221328, 2, arg0);
        }
        else
        {
            cond = 0;
        }

        if (!cond && func_80317D1C(arg0, 2) == 0)
        {
            D_80333EF4 = 0x76557364;
            return;
        }
    }

    D_80333EF4 = 0x76557364;
}

void func_80318280(u32 arg0, u32 arg1, s32 arg2)
{
    if (arg2 == 0)
    {
        D_80333EF4 = 0x19710515;
    }
    func_803182E0(arg0, arg1, arg2);
    if (arg2 == 0)
    {
        D_80333EF4 = 0x76557364;
    }
}

void func_803182E0(u32 arg0, u32 arg1, s32 arg2)
{
    void *ret;
    struct Struct80222A18 *temp = &D_80222A18[arg0];
    UNUSED u32 padding[2];
    s32 sp3C;
    s32 sp38;
    s32 sp34;

    if (arg1 >= D_80226D5C)
    {
        return;
    }

    func_8031AE24(temp);
    if (arg2 != 0)
    {
        sp3C = 0;
        sp38 = 0;
        sp34 = func_80317F04(arg1, &sp38, &sp3C);
        if (sp3C == 1)
        {
            if (func_80317BE4(sp34, 2, temp) == NULL)
            {
                return;
            }
            temp->unk6[0] = sp34;
        }
        else if (func_8031804C(arg1, temp->unk6) == NULL)
        {
            return;
        }
    }
    else if (func_8031804C(arg1, temp->unk6) == NULL)
    {
        return;
    }

    temp->unk5 = arg1;
    ret = func_8031680C(&D_80221328, 2, arg1);
    if (ret == NULL)
    {
        if (temp->unk0b10)
        {
            return;
        }

        if (arg2 != 0)
        {
            ret = func_80317DC8(arg1, 2, temp);
        }
        else
        {
            ret = func_80317D1C(arg1, 2);
        }

        if (ret == NULL)
        {
            return;
        }
    }

    func_8031D42C(arg0);
    temp->unk6C.unk18 = 0;
    temp->unk12 = 0;
    temp->unk0b80 = 1;
    temp->unk14 = ret;
    temp->unk6C.unk0 = ret;
}

void InitAudioSystem()
{
    UNUSED s8 pad[32];
    u8 buf[0x10];
    s32 i, j, k;
    s32 lim1, lim2, lim3;
    u32 alloc;
    u64 *ptr64;
    void *data;
    UNUSED s32 pad2;

    D_80333EF4 = 0;

    lim1 = D_80333EE8;
    for (i = 0; i < lim1; i++)
    {
        D_80226E58[i] = 0;
        D_80226E98[i] = 0;
    }

    lim2 = D_80333EEC;
    for (i = 0; i <= lim2 / 8 - 1; i++)
    {
        ((u64 *) gAudioHeap)[i] = 0;
    }

    i = 0;
    lim3 = ((u32) &D_80226EC0 - (u32) &D_802211A0) / 8;
    ptr64 = &D_802211A0 - 1;
    for (k = lim3; k >= 0; k--)
    {
        i++;
        ptr64[i] = 0;
    }

    for (i = 0; i < 3; i++)
    {
        D_80226E4C[i] = 0x00a0;
    }

    D_80226D80 = 0;
    D_80226D88 = 0;
    D_80226D8C = 0;
    D_80226D7F = 0;
    D_80226D9C = NULL;
    D_80226DA0[0].task.t.data_size = 0;
    D_80226DA0[1].task.t.data_size = 0;
    osCreateMesgQueue(&D_80226500, D_80226518, 1);
    osCreateMesgQueue(&D_80225EE8, D_80225F00, ARRAY_COUNT(D_80225F00));
    D_80226D84 = 0;
    D_80226B38 = 0;

    func_80316108(D_80333EF0);

    for (i = 0; i < 3; i++)
    {
        D_80226E40[i] = soundAlloc(&D_802212B8, 0xa00);

        for (j = 0; j < 0x500; j++)
        {
            D_80226E40[i][j] = 0;
        }
    }

    func_80316928(&D_80332190[0]);

    D_80226D4C = (ALSeqFile*)buf;
    data = gMusicData;
    BlockDmaCopy((u32)data, D_80226D4C, 0x10);
    D_80226D5C = D_80226D4C->seqCount;
    alloc = ALIGN16(D_80226D5C * sizeof(ALSeqData) + 4);
    D_80226D4C = soundAlloc(&D_802212B8, alloc);
    BlockDmaCopy((u32)data, D_80226D4C, alloc);
    alSeqFileNew(D_80226D4C, data);

    D_80226D50 = (ALSeqFile*)buf;
    data = gSoundDataADSR;
    BlockDmaCopy((u32)data, D_80226D50, 0x10);
    alloc = D_80226D50->seqCount * sizeof(ALSeqData) + 4;
    alloc = ALIGN16(alloc);
    D_80226D60 = soundAlloc(&D_802212B8, D_80226D50->seqCount * sizeof(struct Struct_80226D60));
    D_80226D50 = soundAlloc(&D_802212B8, alloc);
    BlockDmaCopy((u32)data, D_80226D50, alloc);
    alSeqFileNew(D_80226D50, data);

    D_80226D54 = (ALSeqFile*)buf;
    BlockDmaCopy((u32)data, D_80226D54, 0x10);
    alloc = D_80226D54->seqCount * sizeof(ALSeqData) + 4;
    alloc = ALIGN16(alloc);
    D_80226D54 = soundAlloc(&D_802212B8, alloc);
    BlockDmaCopy((u32)gSoundDataRaw, D_80226D54, alloc);
    alSeqFileNew(D_80226D54, gSoundDataRaw);

    D_80226D58 = soundAlloc(&D_802212B8, 0x100);
    BlockDmaCopy((u32)gInstrumentSets, D_80226D58, 0x100);

    func_8031D4B8();
    D_80333EF4 = 0x76557364;
}
