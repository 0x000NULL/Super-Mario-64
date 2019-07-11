#ifndef _AUDIO_INTERFACE_2_H
#define _AUDIO_INTERFACE_2_H

#include "types.h"

extern s32 D_80331D40;
extern f32 D_803320E0[6];

extern u64 gAudioSPTaskYieldBuffer[]; // ucode yield data ptr; only used in JP

struct SPTask *func_8031D924(void);
void SetSound(s32 arg0, f32 *arg1);
void func_8031EED0(void);
void func_8031F7CC(u8 arg0, u16 arg1);
void func_8031F810(u8 arg0, u8 arg1, u16 arg2);
void func_8031FFB4(u8 arg0, u16 arg1, u8 arg2);
void func_80320040(u8 arg0, u16 arg1);
void func_80320248(u8 arg0);
void func_803202A0(void);
void func_803205E8(u32 arg0, f32 *arg1);
void func_803206F8(f32 *arg0);
void func_80320890(void);
void func_803208C0(u8 arg0, u16 arg1);
void func_80320980(u8 arg0, u16 arg1);
void func_80320A4C(u8 arg0, u8 arg1);
void func_80320A68(u8 dialogId);
void func_80320AE8(u8 arg0, u16 arg1, s16 arg2);
void func_80320CE8(u16 arg0);
void func_80320E20(u16 arg0, u16 arg1);
void func_80320E74(void);
u16 func_80320E98(void);
void func_80320F84(u8 arg0, u8 arg1, u8 arg2, u16 arg3);
void func_80321080(u16 arg0);
void func_803210D4(u16 arg0);
void play_course_clear(void);
void play_peachs_jingle(void);
void play_puzzle_jingle(void);
void play_star_fanfare(void);
void play_power_star_jingle(u8 arg0);
void play_race_fanfare(void);
void play_toads_jingle(void);
void func_80321368(u8 arg0);
void func_80321434(u8 arg0);

#endif /* _AUDIO_INTERFACE_2_H */
