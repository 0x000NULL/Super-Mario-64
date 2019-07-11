#include <ultra64.h>

#include "sm64.h"
#include "dac.h"
#include "dma.h"
#include "interface_1.h"
#include "interface_2.h"
#include "playback.h"
#include "something.h"
#include "game/mario.h"
#include "game/level_update.h"
#include "game/area.h"
#include "game/room.h"
#include "seq_ids.h"

#ifdef VERSION_JP
#define US_FLOAT(x) x
#else
#define US_FLOAT(x) x ## f
#endif

struct Sound
{
    s32 unk0;
    f32 *unk4;
}; // size = 8

struct Struct80360928
{
    f32 unk0;
    u8 unk4;
    f32 unk8;
    u16 unkC;
}; // size = 0x10

struct Struct80360C48
{
    f32 *unk0;
    f32 *unk4;
    f32 *unk8;
    f32 unkC;
    u32 unk10; // priority?
    u32 unk14; // packed bits, same as first arg to SetSound
    u8 unk18; // 0, 1, or 2-15
    u8 unk19; // sometimes set to 10
    u8 unk1A; // prev
    u8 unk1B; // next (or vice versa)
}; // size = 0x1C

struct Struct80363818
{
    u8 unk0;
    u8 unk1;
}; // size = 2

struct Struct80331F50
{
    s16 unk0;
    u8 unk2;
    u8 unk3;
    s16 unk4;
    s16 unk6;
    u8 unk8;
    u8 unk9;
    s16 unkA;
}; // size = 0xC

// data
s32 D_80331D40 = 0;
s32 D_80331D44 = 0;

// Dialog sounds
// The US difference is the sound for Dialog037 ("I win! You lose! Ha ha ha ha!
// You're no slouch, but I'm a better sledder! Better luck next time!")
#define _ 0xff
u8 D_80331D48[170] = {
//     0  1  2  3  4  5  6  7  8  9
/* 0*/ _, 6, 6, 6, 6, 3, 3, 3, _, 3,
/* 1*/ _, _, _, _, _, _, _, 4, _, _,
/* 2*/ _, 2, 2, 2, 2, 2, 2, 2, 2, 2,
/* 3*/ _, _, _, _, _, _, _, JP_US_DEF(3 , 1), _, _,
/* 4*/ _, 3, _, _, _, _, _, 6, _, _,
/* 5*/ _, _, _, _, _, 1, 1, 1, 1, 1,
/* 6*/ _, _, _, _, _, _, _, 7, _, _,
/* 7*/ _, _, _, _, _, _, _, _, _, 0,
/* 8*/ 0, _, _, _, _, 5, _, _, _, _,
/* 9*/ 7, _, 7, 7, _, _, _, _, 5, 5,
/*10*/ 0, 0, _, _, _, 6, 6, 5, 5, _,
/*11*/ _, _, _, _, 8, 8, 4, 8, 8, _,
/*12*/ _, _, _, _, _, _, _, _, 4, _,
/*13*/ _, _, 1, _, _, _, _, _, _, _,
/*14*/ _, _, _, _, _, _, _, _, _, _,
/*15*/ 9, 9, 9, _, _, _, _, _, _, _,
/*16*/ _, 10, _, _, _, _, _, _, 9, _
};
#undef _

s32 D_80331DF4[15] = {
    SOUND_OBJECT_MONKEY1,
    SOUND_OBJECT_BIGPENGUIN,
    SOUND_OBJECT_BOWSERINTROLAUGH,
    SOUND_OBJECT_KOOPA,
    SOUND_OBJECT_KINGBOBOMB,
    SOUND_OBJECT_BOOLAUGH2,
    SOUND_OBJECT_BOBOMBBUDDY,
    SOUND_OBJECT_BOWSERLAUGH,
    SOUND_CH9_UNK69,
    SOUND_OBJECT_WIGGLER2,
    SOUND_GENERAL_YOSHI,
    NO_SOUND,
    NO_SOUND,
    NO_SOUND,
    NO_SOUND
};

u8 D_80331E30 = 0;
u8 D_80331E34 = 0;

// Format:
// - only play if D_80331EB0 equals this number
// - repeated until mask == 0:
//   - mask << 8 | index, where index points into D_80331F50
//   - one number for each bit set in mask
s16 D_80331E38[] = // BBH
{SEQ_LEVEL_SPOOKY,
    0x01 << 8 | 6, 13,
    0x01 << 8 | 6, 10,
    5};
s16 D_80331E44[] = // DDD
{SEQ_LEVEL_WATER,
    0x12 << 8 | 0, -800, 1,
    0x52 << 8 | 0, -2000, 470, 1,
    0x42 << 8 | 2, 100, 2,
    1};
s16 D_80331E5C[] = // JRB
{SEQ_LEVEL_WATER,
    0x50 << 8 | 0, 945, -5260,
    0x02 << 8 | 0, 2,
    0x40 << 8 | 0, 1000,
    0x44 << 8 | 2, -3100, -900,
    1, 5};
s16 D_80331E78[] = // WDW
{SEQ_LEVEL_UNDERGROUND,
    0x0a << 8 | 4, -670, 1,
    0x02 << 8 | 4, 2,
    3};
s16 D_80331E88[] = // HMC
{SEQ_LEVEL_UNDERGROUND,
    0x88 << 8 | 4, 0, -203,
    0x18 << 8 | 4, 0, -2400,
    3};
s16 D_80331E98[] = // LEVEL_UNKNOWN_38
{SEQ_LEVEL_UNDERGROUND,
    0x02 << 8 | 3, 1,
    0x02 << 8 | 4, 2,
    0x02 << 8 | 7, 3,
    0};
s16 D_80331EA8[] = {0, 0};

u8 D_80331EAC = 0xff; // current area music?
u8 D_80331EB0 = 0xff; // current music?

#define _ D_80331EA8
s16 *D_80331EB4[LEVEL_MAX+1] = {
    _, _, _, _, D_80331E38, _, _, D_80331E88, _, _, _, D_80331E78, D_80331E5C,
    _, _, _, _, _, _, _, _, _, _, D_80331E44, _, _, _, _, _, _, _, _, _, _, _,
    _, _, _, D_80331E98
};
#undef _

struct Struct80331F50 D_80331F50[8] = {
    {0,      0, 0x7f, 100, 0xe43, 0, 0, 100},
    {3,      0, 0x7f, 100, 0xe40, 0, 0, 100},
    {0xe43,  0, 0x7f, 200, 0,     0, 0, 200},
    {0x2ff,  0, 0x7f, 100, 0x100, 0, 0, 100},
    {0x3f7,  0, 0x7f, 100, 8,     0, 0, 100},
    {0x70,   0, 0x7f, 10,  0,     0, 0, 100},
    {0,      0, 0x7f, 100, 0x70,  0, 0, 10},
    {0xffff, 0, 0x7f, 100, 0,     0, 0, 100}
};

u8 D_80331FB0[40][3] = {
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x28, 0x28, 0x28},
    {0x10, 0x38, 0x38},
    {0x20, 0x20, 0x30},
    {0x28, 0x28, 0x28},
    {0x08, 0x30, 0x30},
    {0x08, 0x08, 0x08},
    {0x10, 0x28, 0x28},
    {0x10, 0x18, 0x18},
    {0x10, 0x18, 0x18},
    {0x0c, 0x0c, 0x20},
    {0x18, 0x18, 0x18},
    {0x20, 0x20, 0x20},
    {0x08, 0x08, 0x08},
    {0x28, 0x28, 0x28},
    {0x28, 0x28, 0x28},
    {0x28, 0x28, 0x28},
    {0x10, 0x10, 0x10},
    {0x28, 0x28, 0x28},
    {0x08, 0x30, 0x30},
    {0x10, 0x20, 0x20},
    {0x08, 0x08, 0x08},
    {0x00, 0x00, 0x00},
    {0x08, 0x08, 0x08},
    {0x28, 0x28, 0x28},
    {0x28, 0x28, 0x28},
    {0x20, 0x20, 0x20},
    {0x40, 0x40, 0x40},
    {0x28, 0x28, 0x28},
    {0x70, 0x00, 0x00},
    {0x40, 0x40, 0x40},
    {0x40, 0x40, 0x40},
    {0x00, 0x00, 0x00},
    {0x08, 0x08, 0x08},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00},
    {0x00, 0x00, 0x00}
};

u16 D_80332028[LEVEL_MAX+1] = {
    20000, 20000, 20000, 20000, 28000, 17000, 20000, 16000,
    15000, 15000, 14000, 17000, 20000, 20000, 18000, 20000,
    25000, 16000, 30000, 16000, 20000, 16000, 22000, 17000,
    13000, 20000, 20000, 20000, 18000, 20000, JP_US_DEF(25000 , 60000),
    20000, 20000, JP_US_DEF(25000 , 60000), JP_US_DEF(25000 , 60000),
    20000, 15000, 20000, 20000
};

u8 D_80332078[36] = {
    127, 80, 80, 75, 70, 75, 75, 75, 70, 65, 80, 65, 85, 75, 65, 70, 65, 70,
    70, 65, 80, 70, 85, 75, 75, 85, 70, 80, 80, 70, 75, 80, 70, 65, 0, 0
};

u8 D_8033209C = 0xff;
u8 D_803320A0 = 0;
u8 D_803320A4[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // pointers to head of list
u8 D_803320B0[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // pointers to head of list
u8 D_803320BC[10] = {0};
u8 D_803320C8[10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // sizes of D_80360C38
u8 D_803320D4[10] = {0x70, 0x30, JP_US_DEF(0x30 , 0x40), 0x80, 0x20,
                     0x80, 0x20, JP_US_DEF(0x30 , 0x40), 0x80, 0x80};
f32 D_803320E0[6] = {0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f}; // exported
u8 D_803320F8[16] = {0}; // (size 10 + file split/unused var?)
u8 D_80332108 = 0;
u8 D_8033210C = 0;
u16 D_80332110 = 0;
u8 D_80332114 = 0;
u16 D_80332118 = 0;
u8 D_8033211C = 0;
u8 D_80332120 = 0;
u8 D_80332124 = 0;
u8 D_80332128 = 0;
#ifndef VERSION_JP
u8 D_U_8033323C = 0;
#endif

// bss
u16 *D_80360120;
struct Sound D_80360128[0x100];
struct Struct80360928 D_80360928[3][16];
u8 D_80360C28[10];
u8 D_80360C38[10][1];
struct Struct80360C48 D_80360C48[10][40]; // list item memory for D_803320A4 and D_803320B0
u8 D_80363808[10];
u8 D_80363812;
u8 D_80363813;
struct Struct80363818 D_80363818[6];

#ifdef VERSION_JP
typedef u16 Arg1T;
typedef u8 Arg2T;
#else
typedef s32 Arg1T;
typedef s32 Arg2T;
#endif

static void func_8031E0E4(u8 listIndex, u8 item);

// Local functions that could be static but are defined in/called from GLOBAL_ASM blocks,
// or not part of the large block of static functions.
void func_8031EEF8(void);
void SetMusic(u8 arg0, u8 arg1, u16 arg2);
void func_8031F888(u8 arg0, u8 arg1, u8 arg2, u16 arg3);
void func_8031F96C(u8 arg0);
void func_8031FA4C(void);
u8 func_803200E4(u16 arg0);
void func_80320ED8(void);

#ifdef VERSION_US
void unused_8031E4F0(void)
{
    // This is some debug function which is almost entirely optimized away,
    // except for loops, string literals (the original source likely did
    // "#define printf /* nothing */"), and a read of a volatile variable.
    s32 i;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
    "AUTOSEQ ";
    "%2x %2x <%5x : %5x / %5x>\n";
    "AUTOBNK ";
    "%2x %3x <%5x : %5x / %5x>\n";
    "STAYSEQ ";
    "[%2x] <%5x / %5x>\n";
    "%2x ";
    "\n";
    "STAYBNK ";
    "[%2x] <%5x / %5x>\n";
    "%2x ";
    "\n\n";
    "    0123456789ABCDEF0123456789ABCDEF01234567\n";
    "--------------------------------------------\n";
    "SEQ ";
    "%1x";
    "\n";
    "BNK ";
    "%1x";
    "\n";
    "FIXHEAP ";
    "%4x / %4x\n";
    "DRVHEAP ";
    "%5x / %5x\n";
    "DMACACHE  %4d Blocks\n";
    "CHANNELS  %2d / MAX %3d \n";
    "TEMPOMAX  %d\n";
    "TEMPO G0  %d\n";
    "TEMPO G1  %d\n";
    "TEMPO G2  %d\n";
    "DEBUGFLAG  %8x\n";
    "COUNT %8d\n";
#pragma GCC diagnostic pop

    for (i = 0; (u32)i < D_80221328.first.unk0; i++) {}
    for (i = 0; (u32)i < D_802214F8.first.unk0; i++) {}
    for (i = 0; i < 40; i++) {}
    for (i = 0; i < 40; i += 4) {}
}

void unused_8031E568(void)
{
    D_80226D80;
}
#endif

static void func_8031D630(s32 arg0, Arg1T arg1)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];
#ifndef VERSION_JP
    if (arg1 == 0)
    {
        arg1++;
    }
#endif

    temp->unk1C = -(temp->unk18 / arg1);
    temp->unk2 = 1;
    temp->unkE = arg1;
}

static void func_8031D690(s32 arg0, Arg1T arg1)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];

    // TODO: move this to the correct function of this file (it matches
    // regardless of where it is placed).
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-value"
    "DAC:Lost 1 Frame.\n";
    "DMA: Request queue over.( %d )\n";
#pragma GCC diagnostic pop

    if (arg1 == 0 || temp->unk2 == 1)
    {
        return;
    }

    temp->unk2 = 2;
    temp->unkE = arg1;
    temp->unk18 = 0.0f;
    temp->unk1C = 0.0f;
}

static void func_8031D6E4(s32 arg0, Arg1T arg1, u8 arg2)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];
    Arg2T arg2i = arg2;
    f32 f0;

    if (temp->unk2 == 1)
    {
        return;
    }

    f0 = ((f32) arg2i / 100.0 * temp->unk18);
    temp->unk20 = temp->unk18;
    temp->unkE = 0;
    if (arg1 == 0)
    {
        temp->unk18 = f0;
        return;
    }
    temp->unk1C = (f0 - temp->unk18) / arg1;
    temp->unk2 = 4;
    temp->unkE = arg1;
}

static void func_8031D7B0(s32 arg0, Arg1T arg1)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];

    if (temp->unk2 == 1)
    {
        return;
    }

    temp->unkE = 0;
    if (arg1 == 0)
    {
        temp->unk18 = temp->unk20;
        return;
    }
    temp->unk1C = (temp->unk20 - temp->unk18) / arg1;
    temp->unk2 = 2;
    temp->unkE = arg1;
}

static void func_8031D838(s32 arg0, Arg1T arg1, u8 arg2)
{
    struct Struct80222A18 *temp = &D_80222A18[arg0];
    Arg2T arg2i = arg2;

    if (temp->unk2 == 1)
    {
        return;
    }

    temp->unkE = 0;
    if (arg1 == 0)
    {
        temp->unk18 = ((f32) arg2i / 127.0);
        return;
    }
    temp->unk1C = (((f32) ((f32) arg2i / 127.0) - temp->unk18) / (f32) arg1);
    temp->unk2 = 4;
    temp->unkE = arg1;
}

#ifdef NON_MATCHING

struct SPTask *func_8031D924(void)
{
    s32 sp40;
    s32 sp3C;
    u32 t2;
    s32 v0;
    s32 old;
    UNUSED s32 dummy;
    s32 dataIndex;
    OSTask_t *task;

    D_80226D80++;
    if (D_80333EF4 != 0x76557364)
    {
        return 0;
    }

    D_80226D88 ^= 1;
    D_80226D8C++;
    D_80226D8C %= 3;
    sp3C = (D_80226D8C + 1) % 3;
    t2 = osAiGetLength() >> 2;
    if (D_80226E4C[sp3C] != 0)
    {
        osAiSetNextBuffer(D_80226E40[sp3C], D_80226E4C[sp3C] * 4);
    }

    // there is a pointless volatile load here, that loads into a temporary
    // register rather than a zero one...
    D_80226D84; D_80226D84 = 0;
    // dummy = D_80226D84; D_80226D84 = dummy & 0;
    // dummy = D_80226D84; D_80226D84 = dummy * 0;
    D_80226D9C = &D_80226DA0[D_80226D88];
    D_80226D98 = D_80226D90[D_80226D88];

    old = D_80226D8C;
    D_80360120 = D_80226E40[old];
    D_80226E4C[old] = (((D_80226D74 - t2) + 0x40) & 0xfff0) + 0x10;
    v0 = D_80226D74 + 0x10;
    if (D_80226E4C[old] < D_80226D78)
    {
        D_80226E4C[old] = D_80226D78;
    }
    if (D_80226E4C[old] > v0)
    {
        D_80226E4C[old] = v0;
    }

    if (D_80331D44 != 0)
    {
        func_8031EEF8();
        D_80331D44 = 0;
    }

    D_80226D98 = func_80313CD4(D_80226D98, &sp40, D_80360120, D_80226E4C[old]);
    D_80226EB8 = ((D_80226D80 + D_80226EB8) * D_80226D80);

    dataIndex = D_80226D88;
    D_80226D9C->msgqueue = NULL;
    D_80226D9C->msg = NULL;

    task = &D_80226D9C->task.t;
    task->type = M_AUDTASK;
    task->flags = 0;
    task->ucode_boot = rspF3DBootStart;
    task->ucode_boot_size = (u32) rspF3DBootEnd - (u32) rspF3DBootStart;
    task->ucode = rspAspMainStart;
    task->ucode_size = 0x800;
    task->ucode_data = rspAspMainDataStart;
    task->ucode_data_size = (rspAspMainDataEnd - rspAspMainDataStart) * sizeof(u64);
    task->dram_stack = NULL;
    task->dram_stack_size = 0;
    task->output_buff = NULL;
    task->output_buff_size = NULL;
    task->data_ptr = D_80226D90[dataIndex];
    task->data_size = sp40 * 8;
#ifdef VERSION_JP
    task->yield_data_ptr = gAudioSPTaskYieldBuffer;
    task->yield_data_size = 0x400;
#else
    task->yield_data_ptr = NULL;
    task->yield_data_size = 0;
#endif

    func_8031715C();
    return D_80226D9C;
}

#elif defined(VERSION_JP)
GLOBAL_ASM("asm/non_matchings/func_8031D924_jp.s")
#else
GLOBAL_ASM("asm/non_matchings/func_8031D924_us.s")
#endif

void SetSound(s32 arg0, f32 *arg1)
{
    D_80360128[D_80331E34].unk0 = arg0;
    D_80360128[D_80331E34].unk4 = arg1;
    D_80331E34++;
}

static void func_8031DCA8(u32 arg0, f32 *arg1)
{
    u8 listIndex;
    u8 index;
    u8 counter = 0;
    f32 dist;
    const f32 one = 1.0f;

    listIndex = (arg0 & 0xf0000000) >> 0x1c;
    if ((u8)((arg0 & 0xff0000) >> 0x10) >= D_803320D4[listIndex] ||
            D_803320F8[listIndex] != 0)
    {
        return;
    }

    index = D_80360C48[listIndex][0].unk1B;
    while (index != 0xff && index != 0)
    {
        if (D_80360C48[listIndex][index].unk0 == arg1)
        {
            if ((D_80360C48[listIndex][index].unk14 & 0xff00) <= (arg0 & 0xff00))
            {
                if ((D_80360C48[listIndex][index].unk14 & 0x80) != 0 ||
                        (arg0 & 0xff0000) != (D_80360C48[listIndex][index].unk14 & 0xff0000))
                {
                    func_8031E0E4(listIndex, index);
                    D_80360C48[listIndex][index].unk14 = arg0;
                    D_80360C48[listIndex][index].unk18 = arg0 & 0xf;
                }
                D_80360C48[listIndex][index].unk19 = 10;
            }
            index = 0;
        }
        else
        {
            index = D_80360C48[listIndex][index].unk1B;
        }
        counter++;
    }

    if (counter == 0)
    {
        D_80363808[listIndex] = 32;
    }

    if (D_80360C48[listIndex][D_803320B0[listIndex]].unk1B != 0xff && index != 0)
    {
        index = D_803320B0[listIndex];
        dist = sqrtf(arg1[0]*arg1[0] + arg1[1]*arg1[1] + arg1[2]*arg1[2]) * one;
        D_80360C48[listIndex][index].unk0 = &arg1[0];
        D_80360C48[listIndex][index].unk4 = &arg1[1];
        D_80360C48[listIndex][index].unk8 = &arg1[2];
        D_80360C48[listIndex][index].unkC = dist;
        D_80360C48[listIndex][index].unk14 = arg0;
        D_80360C48[listIndex][index].unk18 = arg0 & 0xf;
        D_80360C48[listIndex][index].unk19 = 10;
        D_80360C48[listIndex][index].unk1A = D_803320A4[listIndex];
        D_80360C48[listIndex][D_803320A4[listIndex]].unk1B = D_803320B0[listIndex];
        D_803320A4[listIndex] = D_803320B0[listIndex];
        D_803320B0[listIndex] = D_80360C48[listIndex][D_803320B0[listIndex]].unk1B;
        D_80360C48[listIndex][D_803320B0[listIndex]].unk1A = 0xff;
        D_80360C48[listIndex][index].unk1B = 0xff;
    }
}

static void func_8031DF64(void)
{
    struct Sound *temp;

    while (D_80331E34 != D_80331E30)
    {
        temp = &D_80360128[D_80331E30];
        func_8031DCA8(temp->unk0, temp->unk4);
        D_80331E30++;
    }
}

static void func_8031DFE8(u8 listIndex, u8 item)
{
    // move item from list D_803320A4 to list D_803320B0
    if (D_803320A4[listIndex] == item)
    {
        D_803320A4[listIndex] = D_80360C48[listIndex][item].unk1A;
    }
    else
    {
        D_80360C48[listIndex][D_80360C48[listIndex][item].unk1B].unk1A =
            D_80360C48[listIndex][item].unk1A;
    }

    D_80360C48[listIndex][D_80360C48[listIndex][item].unk1A].unk1B =
        D_80360C48[listIndex][item].unk1B;
    D_80360C48[listIndex][item].unk1B = D_803320B0[listIndex];
    D_80360C48[listIndex][item].unk1A = -1;
    D_80360C48[listIndex][D_803320B0[listIndex]].unk1A = item;
    D_803320B0[listIndex] = item;
}

static void func_8031E0E4(u8 listIndex, u8 item)
{
    if (D_80360C48[listIndex][item].unk14 & 0x10)
    {
        D_80332110 &= (1 << listIndex) ^ 0xffff;
        func_803200E4(50);
    }
}

static void func_8031E16C(u8 listIndex)
{
    u32 val2;
    u8 spDB;
    u8 i;
    u8 j;
    u8 index;
    u32 sp98[16] = {
        0x10000000, 0x10000000, 0x10000000, 0x10000000,
        0x10000000, 0x10000000, 0x10000000, 0x10000000,
        0x10000000, 0x10000000, 0x10000000, 0x10000000,
        0x10000000, 0x10000000, 0x10000000, 0x10000000
    };
    u8 sp88[16] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    u8 sp78[16] = {
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
    };
    u8 sp77 = 0;
    u8 val;

    index = D_80360C48[listIndex][0].unk1B;
    while (index != 0xff)
    {
        spDB = index;

        if ((D_80360C48[listIndex][index].unk14 & 0x8f) == 0x81)
        {
            if (!D_80360C48[listIndex][index].unk19--)
            {
                D_80360C48[listIndex][index].unk14 = 0;
            }
        }
        else if ((D_80360C48[listIndex][index].unk14 & 0x80) == 0)
        {
            if (D_80360C48[listIndex][index].unk19-- == 8)
            {
                func_8031E0E4(listIndex, index);
                D_80360C48[listIndex][index].unk14 = 0;
            }
        }

        if (D_80360C48[listIndex][index].unk14 == 0 && D_80360C48[listIndex][index].unk18 == 1)
        {
            spDB = D_80360C48[listIndex][index].unk1A;
            D_80360C48[listIndex][index].unk18 = 0;
            func_8031DFE8(listIndex, index);
        }

        if (D_80360C48[listIndex][index].unk18 != 0 && index == spDB)
        {
            D_80360C48[listIndex][index].unkC = sqrtf(
                    (*D_80360C48[listIndex][index].unk0 * *D_80360C48[listIndex][index].unk0) +
                    (*D_80360C48[listIndex][index].unk4 * *D_80360C48[listIndex][index].unk4) +
                    (*D_80360C48[listIndex][index].unk8 * *D_80360C48[listIndex][index].unk8)) * 1;

            val = (D_80360C48[listIndex][index].unk14 & 0xff00) >> 8;
            if (D_80360C48[listIndex][index].unk14 & 0x4000000)
            {
                D_80360C48[listIndex][index].unk10 =
                    0x4c * (0xff - val);
            }
            else if (*D_80360C48[listIndex][index].unk8 > 0.0f)
            {
                D_80360C48[listIndex][index].unk10 =
                    (u32) D_80360C48[listIndex][index].unkC +
                    (u32) (*D_80360C48[listIndex][index].unk8 / US_FLOAT(6.0)) +
                    0x4c * (0xff - val);
            }
            else
            {
                D_80360C48[listIndex][index].unk10 =
                    (u32) D_80360C48[listIndex][index].unkC +
                    0x4c * (0xff - val);
            }

            for (i = 0; i < D_803320C8[listIndex]; i++)
            {
                if (sp98[i] >= D_80360C48[listIndex][index].unk10)
                {
                    for (j = D_803320C8[listIndex] - 1; j > i; j--)
                    {
                        sp98[j] = sp98[j-1];
                        sp88[j] = sp88[j-1];
                        sp78[j] = sp78[j-1];
                    }
                    sp98[i] = D_80360C48[listIndex][index].unk10;
                    sp88[i] = index;
                    sp78[i] = D_80360C48[listIndex][index].unk18;
                    i = D_803320C8[listIndex];
                }
            }
            sp77++;
        }
        index = D_80360C48[listIndex][spDB].unk1B;
    }

    D_803320BC[listIndex] = sp77;
    D_80360C28[listIndex] = D_803320C8[listIndex];

    for (i = 0; i < D_80360C28[listIndex]; i++)
    {
        for (index = 0; index < D_80360C28[listIndex]; index++)
        {
            if (sp88[index] != 0xff && D_80360C38[listIndex][i] == sp88[index])
            {
                sp88[index] = 0xff;
                index = 0xfe;
            }
        }

        if (index != 0xff)
        {
            if (D_80360C38[listIndex][i] != 0xff)
            {
                if (D_80360C48[listIndex][D_80360C38[listIndex][i]].unk14 == 0)
                {
                    if (D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 == 2)
                    {
                        D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 = 0;
                        func_8031DFE8(listIndex, D_80360C38[listIndex][i]);
                    }
                }
                val2 = D_80360C48[listIndex][D_80360C38[listIndex][i]].unk14 & 0x8f;
                if (val2 >= 130 && D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 != 0)
                {
#ifndef VERSION_JP
                    func_8031E0E4(listIndex, D_80360C38[listIndex][i]);
#endif
                    D_80360C48[listIndex][D_80360C38[listIndex][i]].unk14 = 0;
                    D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 = 0;
                    func_8031DFE8(listIndex, D_80360C38[listIndex][i]);
                }
                else
                {
                    if (val2 == 2 && D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 != 0)
                    {
                        D_80360C48[listIndex][D_80360C38[listIndex][i]].unk18 = 1;
                    }
                }
            }
            D_80360C38[listIndex][i] = 0xff;
        }
    }

    for (index = 0; index < D_80360C28[listIndex]; index++)
    {
        if (sp88[index] != 0xff)
        {
            for (i = 0; i < D_80360C28[listIndex]; i++)
            {
                if (D_80360C38[listIndex][i] == 0xff)
                {
                    D_80360C38[listIndex][i] = sp88[index];
                    D_80360C48[listIndex][sp88[index]].unk14 =
                        (D_80360C48[listIndex][sp88[index]].unk14 & ~0xF) + 1;
                    sp88[i] = 0xff;
                    i = 0xfe;
                }
            }
        }
    }
}

static f32 func_8031E97C(f32 arg0, f32 arg1)
{
    f32 abs0;
    f32 abs1;
    f32 ret;

    abs0 = (arg0 < 0 ? -arg0 : arg0);

    if (abs0 > US_FLOAT(22000.0))
    {
        abs0 = US_FLOAT(22000.0);
    }

    abs1 = (arg1 < 0 ? -arg1 : arg1);

    if (abs1 > US_FLOAT(22000.0))
    {
        abs1 = US_FLOAT(22000.0);
    }

    if (arg0 == US_FLOAT(0.0) && arg1 == US_FLOAT(0.0))
    {
        ret = US_FLOAT(0.5);
    }
    else if (US_FLOAT(0.0) <= arg0 && abs1 <= abs0)
    {
        ret = US_FLOAT(1.0) -
            (US_FLOAT(44000.0) - abs0) /
            (US_FLOAT(3.0) * (US_FLOAT(44000.0) - abs1));
    }
    else if (arg0 < 0 && abs1 < abs0)
    {
        ret = (US_FLOAT(44000.0) - abs0) /
            (US_FLOAT(3.0) * (US_FLOAT(44000.0) - abs1));
    }
    else
    {
        ret = 0.5 + arg0 / (US_FLOAT(6.0) * abs1);
    }

    return ret;
}

static f32 func_8031EB24(u8 listIndex, u8 item, f32 arg2)
{
    f32 f0;
    f32 f12;
#ifndef VERSION_JP
    s32 div = listIndex < 3 ? 2 : 3;
#endif

    if (!(D_80360C48[listIndex][item].unk14 & 0x1000000))
    {
#ifdef VERSION_JP
        f0 = D_80332028[gCurrLevelNum];
        if (f0 < D_80360C48[listIndex][item].unkC)
        {
            f12 = 0.0f;
        }
        else
        {
            f12 = 1.0 - D_80360C48[listIndex][item].unkC / f0;
        }
#else
        if (D_80360C48[listIndex][item].unkC > 22000.0f)
        {
            f12 = 0.0f;
        }
        else
        {
            f0 = D_80332028[gCurrLevelNum] / div;
            if (f0 < D_80360C48[listIndex][item].unkC)
            {
                f12 = ((22000.0f - D_80360C48[listIndex][item].unkC) / (22000.0f - f0)) * (1.0f - arg2);
            }
            else
            {
                f12 = 1.0f - D_80360C48[listIndex][item].unkC / f0 * arg2;
            }
        }
#endif

        if (D_80360C48[listIndex][item].unk14 & 0x2000000)
        {
#ifdef VERSION_JP
            if (f12 != 0.0)
#else
            if (f12 >= 0.08f)
#endif
            {
                f12 -= (f32) (D_80226EB8 & 0xf) / US_FLOAT(192.0);
            }
        }
    }
    else
    {
        f12 = 1.0f;
    }

    return arg2 * f12 * f12 + 1.0f - arg2;
}

static f32 func_8031EC7C(u8 listIndex, u8 item)
{
    f32 f2;

    if (!(D_80360C48[listIndex][item].unk14 & 0x8000000))
    {
        f2 = D_80360C48[listIndex][item].unkC / US_FLOAT(22000.0);
        if (D_80360C48[listIndex][item].unk14 & 0x2000000)
        {
            f2 += (f32) (D_80226EB8 & 0xff) * US_FLOAT(0.015625);
        }
    }
    else
    {
        f2 = 0.0f;
    }
    return f2 / US_FLOAT(15.0) + US_FLOAT(1.0);
}

static u8 func_8031ED70(UNUSED u8 listIndex, UNUSED u8 item, u8 arg2)
{
    u8 area;
    u8 level;
    u8 ret;

#ifndef VERSION_JP
    if (D_80360C48[listIndex][item].unk14 & 0x20)
    {
        level = 0;
        area = 0;
    }
    else
    {
#endif
        level = (gCurrLevelNum > LEVEL_MAX ? LEVEL_MAX : gCurrLevelNum);
        area = gCurrAreaIndex - 1;
        if (area > 2)
            area = 2;
#ifndef VERSION_JP
    }
#endif

    ret = (u8) ((u8) D_80222A18[2].unk2C[arg2]->unk54[5] + D_80331FB0[level][area] +
            (US_FLOAT(1.0) - D_80222A18[2].unk2C[arg2]->unk20) * JP_US_DEF(48.0 , 40.0f));

    if (ret > 0x7f)
    {
        ret = 0x7f;
    }
    return ret;
}

static void func_8031EEC8(void)
{
}

void func_8031EED0(void)
{
    D_80331D44 = 1;
    func_8031EEC8();
}

void func_8031EEF8(void)
{
    u8 unk18;
    u8 unk58;
    u8 listIndex;
    u8 j;
    u8 k;
    u8 index;
    f32 ret;

    k = 0;
    func_8031DF64();
    func_8031FA4C();
    if (D_80222A18[2].unk2C[0] == &D_80225DD8)
    {
        return;
    }

    for (listIndex = 0; listIndex < 10; listIndex++)
    {
        func_8031E16C(listIndex);
        for (j = 0; j < 1; j++)
        {
            index = D_80360C38[listIndex][j];
            if (index < 0xff && D_80360C48[listIndex][index].unk18 != 0)
            {
                unk18 = D_80360C48[listIndex][index].unk14 & 0xf;
                unk58 = (D_80360C48[listIndex][index].unk14 >> 0x10);
                D_80360C48[listIndex][index].unk18 = unk18;
                if (unk18 == 1)
                {
                    if (D_80360C48[listIndex][index].unk14 & 0x10)
                    {
                        D_80332110 |= 1 << listIndex;
                        func_803200E4(50);
                    }

                    D_80360C48[listIndex][index].unk14++;
                    D_80360C48[listIndex][index].unk18 = 2;
                    D_80222A18[2].unk2C[k]->unk54[4] = unk58;
                    D_80222A18[2].unk2C[k]->unk54[0] = 1;

                    switch (listIndex)
                    {
                    case 1:
                        if (!(D_80360C48[listIndex][index].unk14 & 0x8000000))
                        {
                            if (D_80363808[listIndex] >= 9)
                            {
                                ret = func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                                D_80222A18[2].unk2C[k]->unk20 = ret;
                            }
                            else
                            {
                                ret = func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                                D_80222A18[2].unk2C[k]->unk20 =
                                    (D_80363808[listIndex] + 8.0f) / 16 * ret;
                            }
                            D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                                        *D_80360C48[listIndex][index].unk0,
                                        *D_80360C48[listIndex][index].unk8);

                            if ((D_80360C48[listIndex][index].unk14 & 0xff0000) == 0x170000)
                            {
                                ret = func_8031EC7C(listIndex, index);
                                D_80222A18[2].unk2C[k]->unk2C =
                                    ((f32) D_80363808[listIndex] / US_FLOAT(80.0)) + ret;
                            }
                            else
                            {
                                ret = func_8031EC7C(listIndex, index);
                                D_80222A18[2].unk2C[k]->unk2C =
                                    ((f32) D_80363808[listIndex] / US_FLOAT(400.0)) + ret;
                            }
                            D_80222A18[2].unk2C[k]->unk3 =
                                func_8031ED70(listIndex, index, k);

                            break;
                        }
                    // fallthrough
                    case 7:
                        D_80222A18[2].unk2C[k]->unk20 = 1.0f;
                        D_80222A18[2].unk2C[k]->unk24 = 0.5f;
                        D_80222A18[2].unk2C[k]->unk2C = 1.0f;
                        break;
                    case 0:
                    case 2:
                        D_80222A18[2].unk2C[k]->unk20 =
                            func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                        D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                            *D_80360C48[listIndex][index].unk0,
                            *D_80360C48[listIndex][index].unk8);
                        D_80222A18[2].unk2C[k]->unk2C = func_8031EC7C(listIndex, index);
                        D_80222A18[2].unk2C[k]->unk3 = func_8031ED70(listIndex, index, k);
                        break;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                        D_80222A18[2].unk2C[k]->unk3 =
                            func_8031ED70(listIndex, index, k);
                        D_80222A18[2].unk2C[k]->unk20 =
                            func_8031EB24(listIndex, index, JP_US_DEF(1.0f , 0.8f));
                        D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                                *D_80360C48[listIndex][index].unk0,
                                *D_80360C48[listIndex][index].unk8);
                        D_80222A18[2].unk2C[k]->unk2C =
                            func_8031EC7C(listIndex, index);
                        break;
                    }
                }
#ifdef VERSION_JP
                else if (unk18 == 0)
                {
                    func_8031E0E4(listIndex, index);
                    D_80222A18[2].unk2C[k]->unk54[0] = 0;
                    func_8031DFE8(listIndex, index);
                }
#else
                else if (D_80222A18[2].unk2C[k]->unk44[0] == 0)
                {
                    func_8031E0E4(listIndex, index);
                    D_80360C48[listIndex][index].unk18 = 0;
                    func_8031DFE8(listIndex, index);
                }
                else if (unk18 == 0 && D_80222A18[2].unk2C[k]->unk44[0]->unk0b40 == 0)
                {
                    func_8031E0E4(listIndex, index);
                    D_80222A18[2].unk2C[k]->unk54[0] = 0;
                    func_8031DFE8(listIndex, index);
                }
#endif
                else if (D_80222A18[2].unk2C[k]->unk44[0]->unk0b80 == 0)
                {
                    func_8031E0E4(listIndex, index);
                    D_80360C48[listIndex][index].unk18 = 0;
                    func_8031DFE8(listIndex, index);
                }
                else
                {
                    // Exactly the same code as before. Unfortunately we can't
                    // make a macro out of this, because then everything ends up
                    // on the same line after preprocessing, and the compiler,
                    // somehow caring about line numbers, makes it not match (it
                    // computes function arguments in the wrong order).
                    switch (listIndex)
                    {
                    case 1:
                        if (!(D_80360C48[listIndex][index].unk14 & 0x8000000))
                        {
                            if (D_80363808[listIndex] >= 9)
                            {
                                ret = func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                                D_80222A18[2].unk2C[k]->unk20 = ret;
                            }
                            else
                            {
                                ret = func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                                D_80222A18[2].unk2C[k]->unk20 =
                                    (D_80363808[listIndex] + 8.0f) / 16 * ret;
                            }
                            D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                                        *D_80360C48[listIndex][index].unk0,
                                        *D_80360C48[listIndex][index].unk8);

                            if ((D_80360C48[listIndex][index].unk14 & 0xff0000) == 0x170000)
                            {
                                ret = func_8031EC7C(listIndex, index);
                                D_80222A18[2].unk2C[k]->unk2C =
                                    ((f32) D_80363808[listIndex] / US_FLOAT(80.0)) + ret;
                            }
                            else
                            {
                                ret = func_8031EC7C(listIndex, index);
                                D_80222A18[2].unk2C[k]->unk2C =
                                    ((f32) D_80363808[listIndex] / US_FLOAT(400.0)) + ret;
                            }
                            D_80222A18[2].unk2C[k]->unk3 =
                                func_8031ED70(listIndex, index, k);

                            break;
                        }
                    // fallthrough
                    case 7:
                        D_80222A18[2].unk2C[k]->unk20 = 1.0f;
                        D_80222A18[2].unk2C[k]->unk24 = 0.5f;
                        D_80222A18[2].unk2C[k]->unk2C = 1.0f;
                        break;
                    case 0:
                    case 2:
                        D_80222A18[2].unk2C[k]->unk20 =
                            func_8031EB24(listIndex, index, JP_US_DEF(0.8f , 0.9f));
                        D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                            *D_80360C48[listIndex][index].unk0,
                            *D_80360C48[listIndex][index].unk8);
                        D_80222A18[2].unk2C[k]->unk2C = func_8031EC7C(listIndex, index);
                        D_80222A18[2].unk2C[k]->unk3 = func_8031ED70(listIndex, index, k);
                        break;
                    case 3:
                    case 4:
                    case 5:
                    case 6:
                    case 8:
                    case 9:
                        D_80222A18[2].unk2C[k]->unk3 =
                            func_8031ED70(listIndex, index, k);
                        D_80222A18[2].unk2C[k]->unk20 =
                            func_8031EB24(listIndex, index, JP_US_DEF(1.0f , 0.8f));
                        D_80222A18[2].unk2C[k]->unk24 = func_8031E97C(
                                *D_80360C48[listIndex][index].unk0,
                                *D_80360C48[listIndex][index].unk8);
                        D_80222A18[2].unk2C[k]->unk2C =
                            func_8031EC7C(listIndex, index);
                        break;
                    }
                }
            }
            k++;
        }

        k += D_803320C8[listIndex] - D_80360C28[listIndex];
    }
}

void SetMusic(u8 arg0, u8 arg1, u16 arg2)
{
    u8 temp_ret;
    u8 i;

    if (arg0 == 0)
    {
        D_8033209C = arg1 & 0x7f;
        D_80331EB0 = 0xff;
        D_80331EAC = 0xff;
        D_803320A0 = 2;
    }

    for (i = 0; i < 0x10; i++)
    {
        D_80360928[arg0][i].unkC = 0;
    }

    D_80222A18[arg0].unk1 = arg1 & 0x80;
    func_80318280(arg0, arg1 & 0x7f, 0);
    if (arg0 == 0)
    {
        temp_ret = func_803200E4(0);
        if (temp_ret != 0xff)
        {
            D_80222A18[0].unk2 = 4;
            D_80222A18[0].unk18 = (f32) temp_ret / US_FLOAT(127.0);
        }
    }
    else
    {
        func_8031D690(arg0, arg2);
    }
}

void func_8031F7CC(u8 arg0, u16 arg1)
{
    if (arg0 == 0)
    {
        D_8033209C = 0xff;
    }
    func_8031D630(arg0, arg1);
}

void func_8031F810(u8 arg0, u8 arg1, u16 arg2)
{
    u8 i;
    for (i = 0; i < 0x10; i++)
    {
        func_8031F888(arg0, i, arg1, arg2);
    }
}

void func_8031F888(u8 arg0, u8 arg1, u8 arg2, u16 arg3)
{
    struct Struct80360928 *temp;

    if (D_80222A18[arg0].unk2C[arg1] != &D_80225DD8)
    {
        temp = &D_80360928[arg0][arg1];
        temp->unkC = arg3;
        temp->unk0 = ((f32) (arg2 / US_FLOAT(127.0)) - D_80222A18[arg0].unk2C[arg1]->unk1C) / arg3;
        temp->unk4 = arg2;
        temp->unk8 = D_80222A18[arg0].unk2C[arg1]->unk1C;
    }
}

void func_8031F96C(u8 arg0)
{
    u8 i;

    for (i = 0; i < 16; i++)
    {
        if (D_80222A18[arg0].unk2C[i] != &D_80225DD8 && D_80360928[arg0][i].unkC != 0)
        {
            D_80360928[arg0][i].unk8 += D_80360928[arg0][i].unk0;
            D_80222A18[arg0].unk2C[i]->unk1C = D_80360928[arg0][i].unk8;
            D_80360928[arg0][i].unkC--;
            if (D_80360928[arg0][i].unkC == 0)
            {
                D_80222A18[arg0].unk2C[i]->unk1C = D_80360928[arg0][i].unk4 / 127.0f;
            }
        }
    }
}

#ifdef NON_MATCHING
void func_8031FA4C(void)
{
    u8 music; // sp57
    s16 conditionValues[8]; // sp44
    u8 conditionTypes[8]; // sp3C
    s16 sp3A;
    s16 sp38;
    u32 conditionBits; // s0
    u32 tempBits; // v1
    s32 numConditions; // v1
    u16 bit; // a1
    u8 condIndex; // a0 (same as numConditions?)
    u8 i; // s1
    u8 j; // v0
    u16 bit2; // s0, v1

    func_8031F96C(0);
    func_8031F96C(2);
    func_80320ED8();
    if (D_803320A0 != 0)
    {
        D_803320A0--;
    }
    else
    {
        D_80331EB0 = D_8033209C;
    }

    if (D_80331EB0 != D_80331EB4[gCurrLevelNum][0])
    {
        return;
    }

    conditionBits = D_80331EB4[gCurrLevelNum][1] & 0xff00;
    music = D_80331EB4[gCurrLevelNum][1] & 0xff;
    i = 2;
    while (conditionBits & 0xff00)
    {
        condIndex = 0;
        for (j = 0, bit = 0x8000; j < 8; j++, bit = bit >> 1)
        {
            if (conditionBits & bit)
            {
                s16 val = D_80331EB4[gCurrLevelNum][i];
                conditionValues[condIndex] = val;
                i++;
                conditionTypes[condIndex] = j;
                condIndex++;
            }
        }

        numConditions = condIndex;
        for (j = 0; j < numConditions; j++)
        {
            // (having all 'temp' share a single variable affects regalloc)
            UNUSED s16 temp;
            switch (conditionTypes[j])
            {
            case 0: {
                s16 temp = gMarioStates[0].pos[0];
                if (temp < conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 1: {
                s16 temp = gMarioStates[0].pos[1];
                if (temp < conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 2: {
                s16 temp = gMarioStates[0].pos[2];
                if (temp < conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 3: {
                s16 temp = gMarioStates[0].pos[0];
                if (temp >= conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 4: {
                s16 temp = gMarioStates[0].pos[1];
                if (temp >= conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 5: {
                s16 temp = gMarioStates[0].pos[2];
                if (temp >= conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 6: {
                s16 temp = gCurrAreaIndex;
                if (temp != conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            case 7: {
                s16 temp = gMarioCurrentRoom;
                if (temp != conditionValues[j])
                    j = numConditions + 1;
                break;
            }
            }
        }

        if (j == numConditions)
        {
            // The area matches. Break out of the loop.
            tempBits = 0;
        }
        else
        {
            tempBits = D_80331EB4[gCurrLevelNum][i++];
            music = tempBits & 0xff,
            tempBits &= 0xff00;
        }

        conditionBits = tempBits;
    }

    if (music != D_80331EAC)
    {
        bit2 = 1;
        if (D_80331EAC == 0xff)
        {
            sp3A = 1;
            sp38 = 1;
        }
        else
        {
            sp3A = D_80331F50[music].unk4;
            sp38 = D_80331F50[music].unkA;
        }

        for (i = 0; i < 16; i++)
        {
            if (D_80331F50[music].unk0 & bit2)
            {
                func_8031F888(0, i, D_80331F50[music].unk3, sp3A);
            }
            if (D_80331F50[music].unk6 & bit2)
            {
                func_8031F888(0, i, D_80331F50[music].unk9, sp38);
            }
            bit2 <<= 1;
        }

        D_80331EAC = music;
    }
}

#else
GLOBAL_ASM("asm/non_matchings/func_8031FA4C.s")
#endif

void Unknown8031FED0(u8 arg0, u32 arg1, s8 arg2)
{
    u8 i;

    if (arg2 < 0)
    {
        arg2 = -arg2;
    }

    for (i = 0; i < 16; i++)
    {
        if (D_80222A18[arg0].unk2C[i] != &D_80225DD8)
        {
            if ((arg1 & 3) == 0)
            {
                D_80222A18[arg0].unk2C[i]->unk1C = 1.0f;
            }
            else if ((arg1 & 1) != 0)
            {
                D_80222A18[arg0].unk2C[i]->unk1C = (f32) arg2 / US_FLOAT(127.0);
            }
            else
            {
                D_80222A18[arg0].unk2C[i]->unk1C = US_FLOAT(1.0) - (f32) arg2 / US_FLOAT(127.0);
            }
        }
        arg1 >>= 2;
    }
}

void func_8031FFB4(u8 arg0, u16 arg1, u8 arg2)
{
    if (arg0 == 0)
    {
        D_80363813 = 1;
        func_803200E4(arg1);
    }
    else if (D_80222A18[arg0].unk0b80 == 1)
    {
        func_8031D6E4(arg0, arg1, arg2);
    }
}

void func_80320040(u8 arg0, u16 arg1)
{
    D_80363813 = 0;
    if (arg0 == 0)
    {
        if (D_80222A18[arg0].unk2 != 1)
        {
            func_803200E4(arg1);
        }
    }
    else
    {
        if (D_80222A18[arg0].unk0b80 == 1)
        {
            func_8031D7B0(arg0, arg1);
        }
    }
}

u8 func_803200E4(u16 arg0)
{
    u8 a2 = 0xff;
    u8 temp;

    if (D_8033209C == 0xff || D_8033209C == 0x1a)
    {
        return 0xff;
    }

    if (D_80222A18[0].unk20 == 0.0f && arg0)
    {
        D_80222A18[0].unk20 = D_80222A18[0].unk18;
    }

    if (D_80363812 != 0)
    {
        a2 = (D_80363812 & 0x7f);
    }

    if (D_8033211C != 0)
    {
        temp = (D_8033211C & 0x7f);
        if (temp < a2)
        {
            a2 = temp;
        }
    }

    if (D_80363813 != 0 && 40 < a2)
    {
        a2 = 40;
    }

    if (D_80332110 != 0 && 20 < a2)
    {
        a2 = 20;
    }

    if (D_80222A18[0].unk0b80 == 1)
    {
        if (a2 != 0xff)
        {
            func_8031D838(0, arg0, a2);
        }
        else
        {
            D_80222A18[0].unk20 = D_80332078[D_8033209C] / 127.0f;
            func_8031D7B0(0, arg0);
        }
    }
    return a2;
}

void func_80320248(u8 arg0)
{
    u8 i;

    for (i = 0; i < 3; i++)
    {
        D_80222A18[i].unk0b20 = arg0;
    }
}

void func_803202A0(void)
{
    u8 i;
    u8 j;

    for (i = 0; i < 10; i++)
    {
        for (j = 0; j < 40; j++)
        {
            D_80360C48[i][j].unk18 = 0;
        }

        for (j = 0; j < 1; j++)
        {
            D_80360C38[i][j] = 0xff;
        }

        D_803320A4[i] = 0;
        D_803320B0[i] = 1;
        D_803320BC[i] = 0;
    }

    for (i = 0; i < 10; i++)
    {
        D_80360C48[i][0].unk1A = 0xff;
        D_80360C48[i][0].unk1B = 0xff;

        for (j = 1; j < 40 - 1; j++)
        {
            D_80360C48[i][j].unk1A = j - 1;
            D_80360C48[i][j].unk1B = j + 1;
        }

        D_80360C48[i][j].unk1A = j - 1;
        D_80360C48[i][j].unk1B = 0xff;
    }

    for (j = 0; j < 3; j++)
    {
        for (i = 0; i < 16; i++)
        {
            D_80360928[j][i].unkC = 0;
        }
    }

    for (i = 0; i < 6; i++)
    {
        D_80363818[i].unk1 = 0;
    }

    func_80320980(2, 0xffff);
    D_80332118 = 0;
    D_80363812 = 0;
    D_80363813 = 0;
    D_80332110 = 0;
    D_80332114 = 0;
    D_8033209C = 0xff;
    D_80226D7F = 0;
    D_80332128 = 0;
    D_8033211C = 0;
    D_80332120 = 0;
    D_80332124 = 0;
    D_80331E30 = 0;
    D_80331E34 = 0;
}

void Unknown8032050C(u8 arg0, u8 *arg1, u8 *arg2, u8 *arg3)
{
    u8 i;
    u8 counter = 0;

    for (i = 0; i < D_803320C8[arg0]; i++)
    {
        if (D_80360C38[arg0][i] != 0xff)
        {
            counter++;
        }
    }

    *arg1 = counter;
    *arg2 = D_803320BC[arg0];
    if (D_80360C38[arg0][0] != 0xff)
    {
        *arg3 = (u8)(D_80360C48[arg0][D_80360C38[arg0][0]].unk14 >> 0x10);
    }
    else
    {
        *arg3 = 0xff;
    }
}

void func_803205E8(u32 arg0, f32 *arg1)
{
    u8 listIndex;
    u8 item;

    listIndex = (arg0 & 0xf0000000) >> 0x1c;
    item = D_80360C48[listIndex][0].unk1B;
    while (item != 0xff)
    {
        if ((u16)(arg0 >> 0x10) == (u16)(D_80360C48[listIndex][item].unk14 >> 0x10) &&
                D_80360C48[listIndex][item].unk0 == arg1)
        {
            func_8031E0E4(listIndex, item);
            D_80360C48[listIndex][item].unk14 = 0;
            item = 0xff;
        }
        else
        {
            item = D_80360C48[listIndex][item].unk1B;
        }
    }
}

void func_803206F8(f32 *arg0)
{
    u8 listIndex;
    u8 item;

    for (listIndex = 0; listIndex < 10; listIndex++)
    {
        item = D_80360C48[listIndex][0].unk1B;
        while (item != 0xff)
        {
            if (D_80360C48[listIndex][item].unk0 == arg0)
            {
                func_8031E0E4(listIndex, item);
                D_80360C48[listIndex][item].unk14 = 0;
            }
            item = D_80360C48[listIndex][item].unk1B;
        }
    }
}

static void func_803207DC(u8 listIndex)
{
    u8 item = D_80360C48[listIndex][0].unk1B;

    while (item != 0xff)
    {
        func_8031E0E4(listIndex, item);
        D_80360C48[listIndex][item].unk14 = 0;
        item = D_80360C48[listIndex][item].unk1B;
    }
}

void func_80320890(void)
{
    func_803207DC(1);
    func_803207DC(4);
    func_803207DC(6);
}

void func_803208C0(UNUSED u8 arg0, u16 arg1)
{
    u8 i;

    for (i = 0; i < 10; i++)
    {
        if (arg1 & 1)
        {
            D_803320F8[i] = 1;
        }
        arg1 = arg1 >> 1;
    }
}

static void func_8032091C(void)
{
    u8 i;

    for (i = 0; i < 3; i++)
    {
        func_8031AE24(&D_80222A18[i]);
    }
}

void func_80320980(UNUSED u8 arg0, u16 arg1)
{
    u8 i;

    for (i = 0; i < 10; i++)
    {
        if (arg1 & 1)
        {
            D_803320F8[i] = 0;
        }
        arg1 = arg1 >> 1;
    }
}

u8 Unknown803209D8(u8 arg0, u8 arg1, u8 arg2)
{
    u8 ret = 0;
    if (D_80222A18[arg0].unk2C[arg1] != &D_80225DD8)
    {
        D_80222A18[arg0].unk2C[arg1]->unk0b10 = arg2;
        ret = arg2;
    }
    return ret;
}

void func_80320A4C(u8 arg0, u8 arg1)
{
    D_80363808[arg0] = arg1;
}

void func_80320A68(u8 dialogId)
{
    u8 soundId;

    if (dialogId >= 170)
    {
        dialogId = 0;
    }

    soundId = D_80331D48[dialogId];
    if (soundId != 0xff)
    {
        SetSound(D_80331DF4[soundId], D_803320E0);
        if (soundId == 2)
        {
            SetMusic(1, SEQ_EVENT_KOOPA_MESSAGE, 0);
        }
    }

#ifndef VERSION_JP
    // "You've stepped on the (Wing|Metal|Vanish) Cap Switch"
    if (dialogId == 10 || dialogId == 11 || dialogId == 12)
    {
        play_puzzle_jingle();
    }
#endif
}

void func_80320AE8(u8 arg0, u16 arg1, s16 arg2)
{
    u8 loBits = arg1 & 0xff;
    u8 hiBits = arg1 >> 8;
    u8 i;
    u8 foundIndex = 0;

    if (arg0 != 0)
    {
        SetMusic(arg0, loBits, arg2);
        return;
    }

    if (D_80332128 == 6)
    {
        return;
    }

    for (i = 0; i < D_80332128; i++)
    {
        if (D_80363818[i].unk0 == loBits)
        {
            if (i == 0)
            {
                SetMusic(0, loBits, arg2);
            }
            else if (!D_80222A18[0].unk0b80)
            {
                func_80320CE8(D_80363818[0].unk0);
            }
            return;
        }
    }

    for (i = 0; i < D_80332128; i++)
    {
        if (D_80363818[i].unk1 <= hiBits)
        {
            foundIndex = i;
            i = D_80332128;
        }
    }

    if (foundIndex == 0)
    {
        SetMusic(0, loBits, arg2);
        D_80332128++;
    }

    for (i = D_80332128 - 1; i > foundIndex; i--)
    {
        D_80363818[i].unk1 = D_80363818[i - 1].unk1;
        D_80363818[i].unk0 = D_80363818[i - 1].unk0;
    }

    D_80363818[foundIndex].unk1 = hiBits;
    D_80363818[foundIndex].unk0 = loBits;
}

void func_80320CE8(u16 arg0)
{
    u8 foundIndex;
    u8 i;

    if (D_80332128 == 0)
    {
        return;
    }

    foundIndex = D_80332128;

    for (i = 0; i < D_80332128; i++)
    {
        if (D_80363818[i].unk0 == (u8)(arg0 & 0xff))
        {
            D_80332128--;
            if (i == 0)
            {
                if (D_80332128 != 0)
                {
                    SetMusic(0, D_80363818[1].unk0, 0);
                }
                else
                {
                    func_8031F7CC(0, 0x14);
                }
            }
            foundIndex = i;
            i = D_80332128;
        }
    }

    for (i = foundIndex; i < D_80332128; i++)
    {
        D_80363818[i].unk1 = D_80363818[i+1].unk1;
        D_80363818[i].unk0 = D_80363818[i+1].unk0;
    }
    D_80363818[i].unk1 = 0;
}

void func_80320E20(u16 arg0, u16 arg1)
{
    if (D_80332128 != 0 && D_80363818[0].unk0 == (u8)(arg0 & 0xff))
    {
        func_8031F7CC(0, arg1);
    }
}

void func_80320E74(void)
{
    if (D_80332128 != 0)
    {
        D_80332128 = 1;
    }
}

u16 func_80320E98(void)
{
    if (D_80332128 != 0)
    {
        return (D_80363818[0].unk1 << 8) + D_80363818[0].unk0;
    }
    return -1;
}

void func_80320ED8(void)
{
    if (D_80222A18[1].unk0b80 || D_8033211C == 0)
    {
        return;
    }

    D_8033211C = 0;
    func_803200E4(50);

    if (D_80363812 != 0 && (D_80332120 == 0x13 || D_80332120 == 0xb))
    {
        SetMusic(1, D_80332120, 1);
        if (D_80332124 != 0xff)
        {
            func_8031D838(1, 1, D_80332124);
        }
    }
}

void func_80320F84(u8 arg0, u8 arg1, u8 arg2, u16 arg3)
{
    UNUSED u32 dummy;

    D_80332118 = 0;
    if (D_8033209C == 0xff || D_8033209C == 2)
    {
        return;
    }

    if (D_80363812 == 0)
    {
        D_80363812 = arg1 + 0x80;
        func_803200E4(arg3);
        SetMusic(1, arg0, arg3 >> 1);
        if (arg2 < 0x80)
        {
            func_8031D838(1, arg3, arg2);
        }
        D_80332124 = arg2;
        D_80332120 = arg0;
    }
    else if (arg2 != 0xff)
    {
        D_80363812 = arg1 + 0x80;
        func_803200E4(arg3);
        func_8031D838(1, arg3, arg2);
        D_80332124 = arg2;
    }
}

void func_80321080(u16 arg0)
{
    if (D_80363812 != 0)
    {
        D_80363812 = 0;
        D_80332120 = 0;
        D_80332124 = 0;
        func_803200E4(arg0);
        func_8031F7CC(1, arg0);
    }
}

void func_803210D4(u16 arg0)
{
    u8 i;

    if (D_8033210C != 0)
    {
        return;
    }

    if (D_80222A18[0].unk0b80 == 1)
    {
        func_8031D630(0, arg0);
    }
    if (D_80222A18[1].unk0b80 == 1)
    {
        func_8031D630(1, arg0);
    }

    for (i = 0; i < 10; i++)
    {
        if (i != 7)
        {
            func_8031F888(2, i, 0, arg0 / 16);
        }
    }
    D_8033210C = 1;
}

void play_course_clear(void)
{
    SetMusic(1, SEQ_EVENT_CUTSCENE_COLLECT_STAR, 0);
    D_8033211C = 0x80;
    func_803200E4(50);
}

void play_peachs_jingle(void)
{
    SetMusic(1, SEQ_EVENT_PEACH_MESSAGE, 0);
    D_8033211C = 0x80;
    func_803200E4(50);
}

/**
    Plays the puzzle jingle. Plays the dadada dadada *dadada* jingle
    that usually plays when you solve a "puzzle", like chests, talking to
    yoshi, releasing chain chomp, opening the pyramid top, etc.
*/
void play_puzzle_jingle(void)
{
    SetMusic(1, SEQ_EVENT_SOLVE_PUZZLE, 0);
    D_8033211C = 0x94;
    func_803200E4(50);
}

void play_star_fanfare(void)
{
    SetMusic(1, SEQ_EVENT_HIGH_SCORE, 0);
    D_8033211C = 0x94;
    func_803200E4(50);
}

void play_power_star_jingle(u8 arg0)
{
    if (arg0 == 0)
    {
        D_80363812 = 0;
    }
    SetMusic(1, SEQ_EVENT_CUTSCENE_STAR_SPAWN, 0);
    D_8033211C = 0x94;
    func_803200E4(50);
}

void play_race_fanfare(void)
{
    SetMusic(1, SEQ_EVENT_RACE, 0);
    D_8033211C = 0x94;
    func_803200E4(50);
}

void play_toads_jingle(void)
{
    SetMusic(1, SEQ_EVENT_TOAD_MESSAGE, 0);
    D_8033211C = 0x94;
    func_803200E4(50);
}

void func_80321368(u8 arg0)
{
#ifndef VERSION_JP
    if (arg0 >= 8)
    {
        arg0 = 0;
        D_U_8033323C = 0;
    }
#endif
    D_80331D44 = 0;
    func_8032091C();
    func_803202A0();
    func_80316928(&D_80332190[arg0]);
    osWritebackDCacheAll();
    if (arg0 != 7)
    {
        func_80318178(0x1b, 3);
        func_80318178(0x1d, 3);
        func_80318178(0x15, 3);
    }
    SetMusic(2, 0, 0);
    D_80332108 = (D_80332108 & 0xf0) + arg0;
    D_80226D7F = D_80332108 >> 4;
    D_8033210C = 0;
}

void func_80321434(u8 arg0)
{
    D_80332108 = (D_80332108 & 0xf) + (arg0 << 4);
    D_80226D7F = arg0;
}

void Unknown80321460(UNUSED s32 arg0, UNUSED s32 arg1, UNUSED s32 arg2, UNUSED s32 arg3)
{
}

void Unknown80321474(UNUSED s32 arg0)
{
}
