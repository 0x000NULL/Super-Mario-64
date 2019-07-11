#include <ultra64.h>

#include "sm64.h"
#include "data.h"
#include "playback.h"
#include "interface_1.h"

#ifdef VERSION_JP
#define US_FLOAT(x) x
#else
#define US_FLOAT(x) x ## f
#endif

#ifdef VERSION_JP
#define US_FLOAT2(x) x ## .0
#else
#define US_FLOAT2(x) x
#endif

void func_80319E70(void)
{
}

void func_80319E78(struct Struct80222A18 *a0) {
    s32 i;

    if (a0->unkE != 0) {
        a0->unk18 += a0->unk1C;

        if (a0->unk18 > US_FLOAT2(1)) a0->unk18 = US_FLOAT2(1);
        if (a0->unk18 < 0) a0->unk18 = 0;

        if (--a0->unkE == 0) {
            switch (a0->unk2) {
                case 1:
                    func_8031AE24(a0);
                    return;

                case 2:
                case 3:
                    a0->unk2 = 0;
                    break;

                case 4:
                    break;
            }
        }
    }

    for (i = 0; i < 16; i++) {
        if (IS_SPECIAL(a0->unk2C[i]) == 1) {
            if (a0->unk2C[i]->unk0b80 == 1) {
                f32 f0;
                f32 f12;
                f32 f2;
                s32 v1;
                struct Struct80225DD8 *v0 = a0->unk2C[i];

                f0 = v0->unk40->unk18 * (v0->unk20 * v0->unk1C);
                if (v0->unk40->unk0b20 && (v0->unk2 & 0x20) != 0)
                    f0 *= v0->unk40->unk24;

                f2 = v0->unk28 * v0->unk24;
                f12 = US_FLOAT(1.0) - v0->unk28;

                for (v1 = 0; v1 < 4; v1++) {
                    struct SubStruct_func_80318870 *v0_2 = v0->unk44[v1];
                    if (v0_2 != NULL && v0_2->unk0b80 && v0_2->unk44 != NULL) {
                        v0_2->unk34 = v0->unk2C * v0_2->unk20;
                        v0_2->unk2C = v0_2->unk24 * f0;
                        v0_2->unk30 = (v0_2->unk28 * f12) + f2;
                    }
                }
            }
        }
    }
}

f32 func_8031A078(struct struct8031A078 *a0) {
    u32 v0;
    f32 result;
    if (!a0->unk00)
        return 1.0f;

    a0->unk04 += a0->unk08;
    v0 = a0->unk04;

    if (v0 >= 127)
        v0 = 127;

    result = US_FLOAT(1.0) + (D_80332488[v0 + 127] - US_FLOAT(1.0)) * a0->unk0C;
    return result;
}

s8 func_8031A17C(struct Struct8031A17C *a0) {
    s32 v1;
    a0->unk4 += a0->unkE;

    v1 = (a0->unk4 >> 10) & 0x3F;

    switch (v1 & 0x30) {
        case 0x10:
            v1 = 31 - v1;

        case 0x00:
            return a0->unk8[v1];

        case 0x20:
            v1 -= 0x20;
            break;

        case 0x30:
            v1 = 63 - v1;
            break;
    }

    return -a0->unk8[v1];
}

f32 func_8031A204(struct Struct8031A17C *a0) {
    s8 v0;
    f32 f2;
    f32 result;

    if (a0->unk16 != 0) {
        a0->unk16--;
        return 1;
    }

    if (a0->unk14) {
        if (a0->unk14 == 1) {
            a0->unk10 = a0->unk0->unkE;
        }
        else {
            a0->unk10 += (a0->unk0->unkE - a0->unk10) / a0->unk14;
        }

        a0->unk14--;
    }
    else {
        if (a0->unk10 != a0->unk0->unkE) {
            a0->unk14 = a0->unk0->unk12;
            if (a0->unk14 == 0)
                a0->unk10 = a0->unk0->unkE;
        }
    }

    if (a0->unk12) {
        if (a0->unk12 == 1) {
            a0->unkE = a0->unk0->unkC;
        }
        else {
            a0->unkE = a0->unkE + (a0->unk0->unkC - a0->unkE) / a0->unk12;
        }

        a0->unk12--;
    }
    else {
        if (a0->unkE != a0->unk0->unkC) {
            a0->unk12 = a0->unk0->unk10;
            if (a0->unk12 == 0)
                a0->unkE = a0->unk0->unkC;
        }
    }

    if (a0->unk10 == 0) {
        return 1.0f;
    }
    
    v0 = func_8031A17C(a0);
    f2 = (f32) a0->unk10 /  US_FLOAT(4096.0);

    result = f2 * (D_80332488[v0 + 127] - US_FLOAT(1.0)) + US_FLOAT(1.0);
    return result;
}

void func_8031A418(struct Struct_func_80318870 *a0) {
    if (a0->unk84.unkC != 0) {
        a0->unk18 = func_8031A078(&a0->unk74);
        if (a0->unk2C != MINUS_ONE) {
            a0->unk1C = func_8031A204(&a0->unk84);
        }
    }
}

void func_8031A478(struct Struct_func_80318870 *a0) {
    struct Struct8031A17C *v0; 
    struct Struct80225DD8 *v1;
    a0->unk1C = 1.0f;
    a0->unk18 = 1.0f;

    v0 = &a0->unk84;

    if (a0->unk2C->unk50->unkA == 0
        && a0->unk2C->unk50->unkE == 0
        && a0->unk2C->unk4.unk00 == 0) {
        v0->unkC = 0;
        return;
    }


    v0->unkC = 1;
    v0->unk4 = 0;
    v0->unk8 = D_80332AA4;
    v0->unk0 = a0->unk2C->unk50;

    v1 = v0->unk0;

    v0->unk14 = v1->unk12;
    if (v0->unk14 == 0) {
        v0->unk10 = v1->unkE;
    }
    else {
        v0->unk10 = v1->unkA;
    }


    v0->unk12 = v1->unk10;
    if (v0->unk12 == 0) {
        v0->unkE = v1->unkC;
    }
    else {
        v0->unkE = v1->unk8;
    }

    v0->unk16 = v1->unk14;
    a0->unk74 = a0->unk2C->unk4;
}

void func_8031A564(struct SubStruct_func_8031A584 *a0, struct SubstructInstrumentSomething *a1, s16 *a2) {
    a0->unk00 = 0;
    a0->unk01 = 0;
    a0->unk02 = 0;
    a0->unk0A = 0;
    a0->unk10 = 0;
    a0->unk1C = a1;
    a0->unk18 = a2;
}

s32 func_8031A584(struct SubStruct_func_8031A584 *a0) {
    u8 flags = a0->unk00;
    switch (a0->unk01)
    {
        case 0:
            return 0;

        case 1: {
            a0->unk06 = a0->unk02;
            a0->unk04 = a0->unk02;
            if ((flags & 0x40) != 0) {
                a0->unk01 = 5;
                goto CONT;
            }
            // fallthrough
        }

        case 2:
            a0->unk08 = 0;
            a0->unk14 = a0->unk06 << 0x10;
            a0->unk01 = 3;
            // fallthrough

        case 3:
            a0->unk0A = a0->unk1C[a0->unk08].unk00;
            switch (a0->unk0A)
            {
                case 0:
                    a0->unk01 = 0;
                    break;
                case -1:
                    a0->unk01 = 5;
                    break;
                case -2:
                    a0->unk08 = a0->unk1C[a0->unk08].unk02;
                    break;
                case -3:
                    a0->unk01 = 1;
                    break;

                default:
                    a0->unk04 = a0->unk1C[a0->unk08].unk02;
                    a0->unk10 = ((a0->unk04 -a0->unk06) << 0x10) / a0->unk0A;
                    a0->unk01 = 4;
                    a0->unk08++;
                    break;
            }
            if (a0->unk01 != 4) {
                goto CONT;
            }

        case 4:
            a0->unk14 += a0->unk10;
            a0->unk06 = (a0->unk14) >> 0x10;
            if (--a0->unk0A <= 0) {
                a0->unk01 = 3;
            }

        case 5:
            goto CONT;

        case 6:
        case 7: {
            a0->unk06 -= a0->unk0E;
            if (a0->unk0C != 0) {
                if (a0->unk01 == 6) {
                    if (a0->unk06 < a0->unk0C) {
                        a0->unk06 = a0->unk0C;
                        a0->unk0A = a0->unk0C / 16;
                        a0->unk01 = 8;
                    }
                    goto CONT;
                }
            }

            if (a0->unk06 < 0x64) {
                a0->unk06 = 0;
                a0->unk01 = 0;
            }
            goto CONT;
        }

        case 8:
            a0->unk0A -= 1;
            if (a0->unk0A == 0) {
                a0->unk01 = 7;
            }
            goto CONT;
    }

CONT:

    if ((flags & 0x20)) {
        a0->unk01 = 6;
        a0->unk00 = flags & ~0x20;
    }

    if ((flags & 0x10)) {
        a0->unk01 = 7;
        a0->unk00 = flags & ~0x30;
    }

    *a0->unk18 = a0->unk06;
    return 0;
}
