#ifndef _SAVE_FILE_H_
#define _SAVE_FILE_H_

#include "types.h"
#include "area.h"

struct WarpNode;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;
extern s8 sUnusedGotGlobalCoinHiScore;
extern u8 gGotFileCoinHiScore;
extern u8 gCurrCourseStarFlags;
extern u8 gSpecialTripleJump;
extern s8 gLevelToCourseNumTable[LEVEL_MAX];

// game progress flags
#define SAVE_FLAG_FILE_EXISTS            /* 0x000001 */ (1 << 0)
#define SAVE_FLAG_HAVE_WING_CAP          /* 0x000002 */ (1 << 1)
#define SAVE_FLAG_HAVE_METAL_CAP         /* 0x000004 */ (1 << 2)
#define SAVE_FLAG_HAVE_VANISH_CAP        /* 0x000008 */ (1 << 3)
#define SAVE_FLAG_HAVE_KEY_1             /* 0x000010 */ (1 << 4)
#define SAVE_FLAG_HAVE_KEY_2             /* 0x000020 */ (1 << 5)
#define SAVE_FLAG_UNLOCKED_BASEMENT_DOOR /* 0x000040 */ (1 << 6)
#define SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR /* 0x000080 */ (1 << 7)
#define SAVE_FLAG_DDD_MOVED_BACK         /* 0x000100 */ (1 << 8)
#define SAVE_FLAG_MOAT_DRAINED           /* 0x000200 */ (1 << 9)
#define SAVE_FLAG_UNLOCKED_PSS_DOOR      /* 0x000400 */ (1 << 10)
#define SAVE_FLAG_UNLOCKED_WF_DOOR       /* 0x000800 */ (1 << 11)
#define SAVE_FLAG_UNLOCKED_CCM_DOOR      /* 0x001000 */ (1 << 12)
#define SAVE_FLAG_UNLOCKED_JRB_DOOR      /* 0x002000 */ (1 << 13)
#define SAVE_FLAG_UNLOCKED_BITDW_DOOR    /* 0x004000 */ (1 << 14)
#define SAVE_FLAG_UNLOCKED_BITFS_DOOR    /* 0x008000 */ (1 << 15)
#define SAVE_FLAG_CAP_ON_GROUND          /* 0x010000 */ (1 << 16)
#define SAVE_FLAG_CAP_ON_KLEPTO          /* 0x020000 */ (1 << 17)
#define SAVE_FLAG_CAP_ON_UKIKI           /* 0x040000 */ (1 << 18)
#define SAVE_FLAG_CAP_ON_MR_BLIZZARD     /* 0x080000 */ (1 << 19)
#define SAVE_FLAG_UNLOCKED_50_STAR_DOOR  /* 0x100000 */ (1 << 20)

// Variables for setting a warp checkpoint.
extern u8 gWarpCheckpointActNum;
extern u8 gWarpCheckpointCourseNum;
extern u8 gWarpCheckpointLevelID;
extern u8 gWarpCheckpointAreaNum;
extern u8 gWarpCheckpointWarpNode;

extern s8 gMainMenuDataModified;
extern s8 gSaveFileModified;

void save_file_do_save(s32 fileIndex);
void save_file_erase(s32 fileIndex);
void save_file_copy(s32 srcFileIndex, s32 destFileIndex);
void save_file_load_all(void);
void save_file_reload(void);
void save_file_collect_star_or_key(s16 coinScore, s16 starIndex);
s32 save_file_exists(s32 fileIndex);
u32 save_file_get_max_coin_score(s32 courseIndex);
s32 save_file_get_course_star_count(s32 fileIndex, s32 courseIndex);
s32 save_file_get_total_star_count(s32 fileIndex, s32 minCourse, s32 maxCourse);
void save_file_set_flags(s32 flags);
void save_file_clear_flags(s32 flags);
s32 save_file_get_flags(void);
s32 save_file_get_star_flags(s32 fileIndex, s32 courseIndex);
void save_file_set_star_flags(s32 fileIndex, s32 courseIndex, s32 starFlags);
s32 save_file_get_course_coin_score(s32 fileIndex, s32 courseIndex);
s32 save_file_is_cannon_unlocked(void);
void save_file_set_cannon_unlocked(void);
void save_file_set_cap_pos(s16 x, s16 y, s16 z);
s32 save_file_get_cap_pos(Vec3s capPos);
void save_file_set_sound_mode(u16 mode);
u16 save_file_get_sound_mode(void);
void save_file_move_cap_to_default_location(void);

void disable_warp_checkpoint(void);
void check_if_should_set_warp_checkpoint(struct WarpNode *a);
s32 check_warp_checkpoint(struct WarpNode *a);

#endif
