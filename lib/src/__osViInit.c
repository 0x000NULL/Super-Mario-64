#include "libultra_internal.h"
#include "hardware.h"

#ifdef VERSION_US
extern u32 osTvType;
#endif 

OSViContext D_803348B0[2] = {0};
OSViContext *D_80334910 = &D_803348B0[0];
OSViContext *D_80334914 = &D_803348B0[1];
u32 D_80334918 = TV_TYPE_NTSC;
u32 D_8033491C = 0x02E6D354;

extern OSViMode D_80334990;
extern OSViMode D_803349E0;
void __osViInit(void)
{
//#ifdef VERSION_JP
#ifdef VERSION_US
    D_80334918 = osTvType;
#endif
    bzero(D_803348B0, sizeof(D_803348B0));
    D_80334910 = &D_803348B0[0];
    D_80334914 = &D_803348B0[1];
    D_80334914->retraceCount = 1;
    D_80334910->retraceCount = 1;
#ifdef VERSION_JP
    if (D_80334918 != TV_TYPE_PAL)
#else
    if (D_80334918 == TV_TYPE_NTSC)
#endif
    {
        D_80334914->unk08 = &D_80334990;
        D_8033491C = 0x02E6D354;
    }
    else
    {
        D_80334914->unk08 = &D_803349E0;
#ifdef VERSION_JP
        D_8033491C = 0x02F5B2D2;
#else
        D_8033491C = 0x02E6025C;
#endif
    }
    D_80334914->unk00 = 32;
    D_80334914->features = D_80334914->unk08->comRegs.ctrl;
#ifdef VERSION_US
    while (HW_REG(VI_CURRENT_REG, u32) > 0xa)
        ;
    HW_REG(VI_STATUS_REG, u32) = 0;
#endif
    __osViSwapContext();
}
