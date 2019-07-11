#ifndef _PRINT_H
#define _PRINT_H

#include "types.h"

// extern ? D_8035FF90;
// extern ? D_803303FC;
// extern ? D_803303F8;
// extern ? D_8035FFA0;
// extern ? D_8035FFA4;
// extern ? D_80330400;
extern struct PaintingData *D_8035FFA8;

// extern ? func_802D1EE0(?);
// extern ? func_802D1F94(?);
// extern ? func_802D2028(?);
// extern ? func_802D20A4(?);
// extern ? func_802D2160(?);
// extern ? func_802D22A0(?);
// extern ? func_802D231C(?);
// extern ? func_802D23D8(?);
// extern ? func_802D251C(?);
// extern ? func_802D26BC(?);
// extern ? func_802D279C(?);
// extern ? func_802D293C(?);
// extern ? func_802D2A1C(?);
// extern ? func_802D2BCC(?);
// extern ? func_802D2CBC(?);
// extern ? func_802D2E5C(?);
// extern ? func_802D2F4C(?);
// extern ? func_802D310C(?);
// extern ? func_802D320C(?);
// extern ? func_802D338C(?);
// extern ? Print1(?);
// extern ? Print2(?);
// extern ? func_802D3918(?);
// extern ? func_802D39DC(?);
// extern ? func_802D3CF0(?);
// extern ? func_802D43FC(?);
// extern ? func_802D45FC(?);
// extern ? func_802D4874(?);
// extern ? func_802D4A8C(?);
// extern ? func_802D4BAC(?);
// extern ? func_802D4C98(?);
// extern ? func_802D4CC8(?);
// extern ? func_802D4E04(?);
// extern ? func_802D4E5C(?);
// extern ? func_802D4EC8(?);
// extern ? func_802D4FC0(?);
// extern ? Geo18_802D5B98(?);
// extern ? Geo18_802D5D0C(?);
// extern ? func_802D5320(?);
// extern ? func_802D5374(?);
// extern ? func_802D5664(?);
extern void print_text_fmt_int(s32, s32, const char *, s32);
extern void print_text(s32, s32, char *);
extern void print_text_centered(s32, s32, const char *);
// extern ? func_802D5D78(?);
// extern ? addchar_displist(?);
// extern ? func_802D5FEC(?);
// extern ? func_802D605C(?);
extern void func_802D61A8(void);

#endif /* _PRINT_H */
