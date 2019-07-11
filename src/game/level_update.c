#include <ultra64.h>

#include "sm64.h"
#include "audio/interface_2.h"
#include "level_update.h"
#include "game.h"
#include "level_update.h"
#include "main.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "area.h"
#include "save_file.h"
#include "sound_init.h"
#include "mario.h"
#include "camera.h"
#include "object_list_processor.h"
#include "ingame_menu.h"
#include "obj_behaviors.h"
#include "display.h"
#include "save_file.h"
#include "debug_course.h"

#define PLAY_MODE_NORMAL        0
#define PLAY_MODE_PAUSED        2
#define PLAY_MODE_CHANGE_AREA   3
#define PLAY_MODE_CHANGE_LEVEL  4
#define PLAY_MODE_FRAME_ADVANCE 5

#define WARP_TYPE_NOT_WARPING  0
#define WARP_TYPE_CHANGE_LEVEL 1
#define WARP_TYPE_CHANGE_AREA  2
#define WARP_TYPE_SAME_AREA    3

#define WARP_NODE_F0            0xF0
#define WARP_NODE_DEATH         0xF1
#define WARP_NODE_F2            0xF2
#define WARP_NODE_WARP_FLOOR    0xF3
#define WARP_NODE_CREDITS_START 0xF8
#define WARP_NODE_CREDITS_NEXT  0xF9
#define WARP_NODE_CREDITS_END   0xFA

#define WARP_NODE_CREDITS_MIN 0xF8

#ifdef VERSION_JP
const char *credits01[] = { "1GAME DIRECTOR", "SHIGERU MIYAMOTO" };
const char *credits02[] = { "2ASSISTANT DIRECTORS", "YOSHIAKI KOIZUMI", "TAKASHI TEZUKA" };
const char *credits03[] = { "2SYSTEM PROGRAMMERS", "YASUNARI NISHIDA", "YOSHINORI TANIMOTO" };
const char *credits04[] = { "3PROGRAMMERS", "HAJIME YAJIMA", "DAIKI IWAMOTO", "TOSHIO IWAWAKI" };
const char *credits05[] = { "1CAMERA PROGRAMMER", "TAKUMI KAWAGOE" };
const char *credits06[] = { "1MARIO FACE PROGRAMMER", "GILES GODDARD" };
const char *credits07[] = { "2COURSE DIRECTORS", "YOICHI YAMADA", "YASUHISA YAMAMURA" };
const char *credits08[] = { "2COURSE DESIGNERS", "KENTA USUI", "NAOKI MORI" };
const char *credits09[] = { "3COURSE DESIGNERS", "YOSHIKI HARUHANA", "MAKOTO MIYANAGA", "KATSUHIKO KANNO" };
const char *credits10[] = { "1SOUND COMPOSER", "KOJI KONDO" };
const char *credits11[] = { "1SOUND EFFECTS", "YOJI INAGAKI" };
const char *credits12[] = { "1SOUND PROGRAMMER", "HIDEAKI SHIMIZU" };
const char *credits13[] = { "23D ANIMATORS", "YOSHIAKI KOIZUMI", "SATORU TAKIZAWA" };
const char *credits14[] = { "1CG DESIGNER", "MASANAO ARIMOTO" };
const char *credits15[] = { "3TECHNICAL SUPPORT", "TAKAO SAWANO", "HIROHITO YOSHIMOTO", "HIROTO YADA" };
const char *credits16[] = { "1TECHNICAL SUPPORT", "SGI. 64PROJECT STAFF" };
const char *credits17[] = { "2PROGRESS MANAGEMENT", "KIMIYOSHI FUKUI", "KEIZO KATO" };
const char *credits18[] = { "3SPECIAL THANKS TO", "JYOHO KAIHATUBU", "ALL NINTENDO", "MARIO CLUB STAFF" };
const char *credits19[] = { "1PRODUCER", "SHIGERU MIYAMOTO" };
const char *credits20[] = { "1EXECUTIVE PRODUCER", "HIROSHI YAMAUCHI" };
#else
const char *credits01[] = { "1GAME DIRECTOR", "SHIGERU MIYAMOTO" };
const char *credits02[] = { "2ASSISTANT DIRECTORS", "YOSHIAKI KOIZUMI", "TAKASHI TEZUKA" };
const char *credits03[] = { "2SYSTEM PROGRAMMERS", "YASUNARI NISHIDA", "YOSHINORI TANIMOTO" };
const char *credits04[] = { "3PROGRAMMERS", "HAJIME YAJIMA", "DAIKI IWAMOTO", "TOSHIO IWAWAKI" };
const char *credits05[] = { "4CAMERA PROGRAMMER", "MARIO FACE PROGRAMMER", "TAKUMI KAWAGOE", "GILES GODDARD" }; // US combines camera programmer and mario face programmer
const char *credits06[] = { "2COURSE DIRECTORS", "YOICHI YAMADA", "YASUHISA YAMAMURA" };
const char *credits07[] = { "2COURSE DESIGNERS", "KENTA USUI", "NAOKI MORI" };
const char *credits08[] = { "3COURSE DESIGNERS", "YOSHIKI HARUHANA", "MAKOTO MIYANAGA", "KATSUHIKO KANNO" };
const char *credits09[] = { "1SOUND COMPOSER", "KOJI KONDO" };
const char *credits10[] = { "4SOUND EFFECTS", "SOUND PROGRAMMER", "YOJI INAGAKI", "HIDEAKI SHIMIZU" }; // as well as sound effects and sound programmer
const char *credits11[] = { "23-D ANIMATORS", "YOSHIAKI KOIZUMI", "SATORU TAKIZAWA" };
const char *credits12[] = { "1ADDITIONAL GRAPHICS", "MASANAO ARIMOTO" };
const char *credits13[] = { "3TECHNICAL SUPPORT", "TAKAO SAWANO", "HIROHITO YOSHIMOTO", "HIROTO YADA" };
const char *credits14[] = { "1TECHNICAL SUPPORT", "SGI N64 PROJECT STAFF" };
const char *credits15[] = { "2PROGRESS MANAGEMENT", "KIMIYOSHI FUKUI", "KEIZO KATO" };
const char *credits16[] = { "5SCREEN TEXT WRITER", "TRANSLATION", "LESLIE SWAN", "MINA AKINO", "HIRO YAMADA" }; // ...in order to make room for these 2 new lines
const char *credits17[] = { "4MARIO VOICE", "PEACH VOICE", "CHARLES MARTINET", "LESLIE SWAN" };
const char *credits18[] = { "3SPECIAL THANKS TO", "EAD STAFF", "ALL NINTENDO PERSONNEL", "MARIO CLUB STAFF" };
const char *credits19[] = { "1PRODUCER", "SHIGERU MIYAMOTO" };
const char *credits20[] = { "1EXECUTIVE PRODUCER", "HIROSHI YAMAUCHI" };
#endif

struct CreditsEntry sCreditsSequence[] =
{
    { LEVEL_CASTLE_GROUNDS, 1,    1, -128, {    0,  8000,     0}, NULL      },
    { LEVEL_BOB,            1,    1,  117, {  713,  3918, -3889}, credits01 },
    { LEVEL_WF,             1,   50,   46, {  347,  5376,   326}, credits02 },
    { LEVEL_JRB,            1,   18,   22, { 3800, -4840,  2727}, credits03 },
    { LEVEL_CCM,            2,   34,   25, {-5464,  6656, -6575}, credits04 },
    { LEVEL_BBH,            1,    1,   60, {  257,  1922,  2580}, credits05 },
    { LEVEL_HMC,            1,  -15,  123, {-6469,  1616, -6054}, credits06 },
    { LEVEL_THI,            3,   17,  -32, {  508,  1024,  1942}, credits07 },
    { LEVEL_LLL,            2,   33,  124, {  -73,    82, -1467}, credits08 },
    { LEVEL_SSL,            1,   65,   98, {-5906,  1024, -2576}, credits09 },
    { LEVEL_DDD,            1,   50,   47, {-4884, -4607,  -272}, credits10 },
    { LEVEL_SL,             1,   17,  -34, { 1925,  3328,   563}, credits11 },
    { LEVEL_WDW,            1,   33,  105, { -537,  1850,  1818}, credits12 },
    { LEVEL_TTM,            1,    2,  -33, { 2613,   313,  1074}, credits13 },
    { LEVEL_THI,            1,   51,   54, {-2609,   512,   856}, credits14 },
    { LEVEL_TTC,            1,   17,  -72, {-1304,   -71,  -967}, credits15 },
    { LEVEL_RR,             1,   33,   64, { 1565,  1024,  -148}, credits16 },
    { LEVEL_SA,             1,    1,   24, {-1050, -1330, -1559}, credits17 },
    { LEVEL_COTMC,          1,   49,  -16, { -254,   415, -6045}, credits18 },
    { LEVEL_DDD,            2, -111,  -64, { 3948,  1185,  -104}, credits19 },
    { LEVEL_CCM,            1,   33,   31, { 3169, -4607,  5240}, credits20 },
    { LEVEL_CASTLE_GROUNDS, 1,    1, -128, {    0,   906, -1200}, NULL      },
    { 0,                    0,    1,    0, {    0,     0,     0}, NULL      },
};

struct MarioState gMarioStates[1];
struct MarioState *gMarioState = &gMarioStates[0];

u8 unused1[4] = {0};

s8 D_8032C9E0 = 0;

s16 sCurrPlayMode;
u16 D_80339ECA;

s16 sTransitionTimer;
void (*sTransitionUpdate)(s16 *);

u8 unused3[4];

u8 sCurrWarpType;
u8 sDestLevelNum;
u8 sDestAreaIndex;
u8 sDestWarpNodeId;
u32 sWarpArg;
s16 D_80339EE0;

s16 sDelayedWarpOp;
s16 sDelayedWarpTimer;
s16 sSourceWarpNodeId;
s32 sDelayedWarpArg;

u8 unused4[2];

s8 sTimerRunning;
s16 gDisplayedLives;
s16 gDisplayedCoins;
s16 gDisplayedStars;
s16 gDisplayedHealthWedges;
s16 gDisplayedKeys;
s16 gHudDisplayFlags;
u16 gTimerValueInFrames;
s8 gSaveFileDoesNotExist;


static void basic_update(s16 *arg);


u16 level_control_timer(u32 timerOp)
{
    switch (timerOp)
    {
    case TIMER_CONTROL_SHOW:
        gHudDisplayFlags |= HUD_DISPLAY_FLAG_TIMER;
        sTimerRunning = FALSE;
        gTimerValueInFrames = 0;
        break;

    case TIMER_CONTROL_START:
        sTimerRunning = TRUE;
        break;

    case TIMER_CONTROL_STOP:
        sTimerRunning = FALSE;
        break;

    case TIMER_CONTROL_HIDE:
        gHudDisplayFlags &= ~HUD_DISPLAY_FLAG_TIMER;
        sTimerRunning = FALSE;
        gTimerValueInFrames = 0;
        break;
    }

    return gTimerValueInFrames;
}

static u32 pressed_paused(void)
{
    u32 val4 = get_dialog_id() >= 0;
    u32 intangible = (gMarioState->action & ACT_FLAG_INTANGIBLE) != 0;

    if (!intangible && !val4 && !gWarpTransition.isActive && sDelayedWarpOp == WARP_OP_NONE &&
        (gPlayer1Controller->buttonPressed & START_BUTTON))
    {
        return TRUE;
    }

    return FALSE;
}

static void set_play_mode(s16 playMode)
{
    sCurrPlayMode = playMode;
    D_80339ECA = 0;
}

static void func_8024975C(s32 arg)
{
    sCurrPlayMode = PLAY_MODE_CHANGE_LEVEL;
    D_80339ECA = 0;
    D_80339EE0 = arg;
}

void func_80249788(u32 arg, u32 color)
{
    if (color != 0)
        color = 0xFF;

    func_802491FC(190);
    play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x10, color, color, color);
    level_set_transition(30, NULL);

    func_8024975C(arg);
}

static void nop_802497FC(void)
{
}

void func_8024980C(u32 arg)
{
    s32 gotAchievement;
    u32 val8 = gCurrentArea->dialog[arg];

    switch (val8)
    {
    case 0x81:
        gotAchievement = save_file_get_flags() & SAVE_FLAG_HAVE_VANISH_CAP;
        break;

    case 0x82:
        gotAchievement = save_file_get_flags() & SAVE_FLAG_HAVE_METAL_CAP;
        break;

    case 0x83:
        gotAchievement = save_file_get_flags() & SAVE_FLAG_HAVE_WING_CAP;
        break;

    case 0xFF:
        gotAchievement = TRUE;
        break;

    default:
        gotAchievement = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
        break;
    }

    if (!gotAchievement)
    {
        level_set_transition(-1, NULL);
        func_802D7F90(val8);
    }
}

static void func_8024992C(struct SpawnInfo *spawnInfo, u32 arg1)
{
    if (arg1 & 0x00000002)
        spawnInfo->startAngle[1] += 0x8000;

    spawnInfo->startPos[0] += 300.0f * sins(spawnInfo->startAngle[1]);
    spawnInfo->startPos[2] += 300.0f * coss(spawnInfo->startAngle[1]);
}

static void set_mario_initial_cap_powerup(struct MarioState *m)
{
    u32 capCourseIndex = gCurrCourseNum - COURSE_CAP_COURSES;

    switch (capCourseIndex)
    {
    case COURSE_COTMC - COURSE_CAP_COURSES:
        m->flags |= MARIO_METAL_CAP | MARIO_CAP_ON_HEAD;
        m->capTimer = 600;
        break;

    case COURSE_TOTWC - COURSE_CAP_COURSES:
        m->flags |= MARIO_WING_CAP | MARIO_CAP_ON_HEAD;
        m->capTimer = 1200;
        break;

    case COURSE_VCUTM - COURSE_CAP_COURSES:
        m->flags |= MARIO_VANISH_CAP | MARIO_CAP_ON_HEAD;
        m->capTimer = 600;
        break;
    }
}

static void set_mario_initial_action(struct MarioState *m, u32 spawnType, u32 actionArg)
{
    switch (spawnType)
    {
    case MARIO_SPAWN_UNKNOWN_01: set_mario_action(m, ACT_WARP_DOOR_SPAWN, actionArg); break;
    case MARIO_SPAWN_UNKNOWN_02: set_mario_action(m, ACT_IDLE, 0);                    break;
    case MARIO_SPAWN_UNKNOWN_03: set_mario_action(m, ACT_EMERGE_FROM_PIPE, 0);        break;
    case MARIO_SPAWN_UNKNOWN_04: set_mario_action(m, ACT_TELEPORT_FADE_IN, 0);        break;
    case MARIO_SPAWN_UNKNOWN_10: set_mario_action(m, ACT_IDLE, 0);                    break;
    case MARIO_SPAWN_UNKNOWN_12: set_mario_action(m, ACT_SPAWN_NO_SPIN_AIRBORNE, 0);  break;
    case MARIO_SPAWN_UNKNOWN_13: set_mario_action(m, ACT_HARD_BACKWARD_AIR_KB, 0);    break;
    case MARIO_SPAWN_UNKNOWN_14: set_mario_action(m, ACT_SPAWN_SPIN_AIRBORNE, 0);     break;
    case MARIO_SPAWN_UNKNOWN_15: set_mario_action(m, ACT_FALLING_DEATH_EXIT, 0);      break;
    case MARIO_SPAWN_UNKNOWN_16: set_mario_action(m, ACT_SPAWN_SPIN_AIRBORNE, 0);     break;
    case MARIO_SPAWN_UNKNOWN_17: set_mario_action(m, ACT_FLYING, 2);                  break;
    case MARIO_SPAWN_UNKNOWN_11: set_mario_action(m, ACT_WATER_IDLE, 1);              break;
    case MARIO_SPAWN_UNKNOWN_20: set_mario_action(m, ACT_EXIT_AIRBORNE, 0);           break;
    case MARIO_SPAWN_UNKNOWN_21: set_mario_action(m, ACT_DEATH_EXIT, 0);              break;
    case MARIO_SPAWN_UNKNOWN_22: set_mario_action(m, ACT_FALLING_EXIT_AIRBORNE, 0);   break;
    case MARIO_SPAWN_UNKNOWN_23: set_mario_action(m, ACT_UNUSED_DEATH_EXIT, 0);       break;
    case MARIO_SPAWN_UNKNOWN_24: set_mario_action(m, ACT_SPECIAL_EXIT_AIRBORNE, 0);   break;
    case MARIO_SPAWN_UNKNOWN_25: set_mario_action(m, ACT_SPECIAL_DEATH_EXIT, 0);      break;
    }

    set_mario_initial_cap_powerup(m);
}

static void init_mario_after_warp(void)
{
    struct ObjectWarpNode *spawnNode = area_get_warp_node(sDestWarpNodeId);
    u32 marioSpawnType = get_mario_spawn_type(spawnNode->object);

    if (gMarioState->action != ACT_UNINITIALIZED)
    {
        gPlayerSpawnInfos[0].startPos[0] = (s16) spawnNode->object->oPosX;
        gPlayerSpawnInfos[0].startPos[1] = (s16) spawnNode->object->oPosY;
        gPlayerSpawnInfos[0].startPos[2] = (s16) spawnNode->object->oPosZ;

        gPlayerSpawnInfos[0].startAngle[0] = 0;
        gPlayerSpawnInfos[0].startAngle[1] = spawnNode->object->oMoveAngleYaw;
        gPlayerSpawnInfos[0].startAngle[2] = 0;

        if (marioSpawnType == MARIO_SPAWN_UNKNOWN_01)
            func_8024992C(&gPlayerSpawnInfos[0], sWarpArg);

        if (sCurrWarpType == WARP_TYPE_CHANGE_LEVEL || sCurrWarpType == WARP_TYPE_CHANGE_AREA)
        {
            gPlayerSpawnInfos[0].areaIndex = sDestAreaIndex;
            load_mario_area();
        }

        func_802548BC();
        set_mario_initial_action(gMarioState, marioSpawnType, sWarpArg);

        gMarioState->interactObj = spawnNode->object;
        gMarioState->usedObj = spawnNode->object;
    }

    reset_camera(gCurrentArea->camera);
    sCurrWarpType = WARP_TYPE_NOT_WARPING;
    sDelayedWarpOp = WARP_OP_NONE;

    switch (marioSpawnType)
    {
    case MARIO_SPAWN_UNKNOWN_03: play_transition(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00); break;
    case MARIO_SPAWN_UNKNOWN_01: play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
    case MARIO_SPAWN_UNKNOWN_04: play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x14, 0xFF, 0xFF, 0xFF); break;
    case MARIO_SPAWN_UNKNOWN_16: play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x1A, 0xFF, 0xFF, 0xFF); break;
    case MARIO_SPAWN_UNKNOWN_14: play_transition(WARP_TRANSITION_FADE_FROM_CIRCLE, 0x10, 0x00, 0x00, 0x00); break;
    case MARIO_SPAWN_UNKNOWN_27: play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x10, 0x00, 0x00, 0x00); break;
    default:                     play_transition(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0x00, 0x00, 0x00); break;
    }

    if (gCurrDemoInput == NULL)
    {
        func_80249148(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);

        if (gMarioState->flags & MARIO_METAL_CAP)
            func_80249368(0x0000040F);

        if (gMarioState->flags & (MARIO_VANISH_CAP | MARIO_WING_CAP))
            func_80249368(0x0000040E);

#if VERSION_US
        if (gCurrLevelNum == LEVEL_BOB && func_80320E98() != 1033 && sTimerRunning != 0)
            func_80320AE8(0, (4 << 8) | 9, 0);
#endif

        if (sDestLevelNum == LEVEL_CASTLE
         && sDestAreaIndex == 1
#if VERSION_US
         && (sDestWarpNodeId == 31 || sDestWarpNodeId == 32)
#else
         && sDestWarpNodeId == 31
#endif
        )
        SetSound(SOUND_MENU_MARIOCASTLEWARP, D_803320E0);
#if VERSION_US
        if (sDestLevelNum == 16
         && sDestAreaIndex == 1
         && (sDestWarpNodeId == 7 || sDestWarpNodeId == 10
          || sDestWarpNodeId == 20 || sDestWarpNodeId == 30))
        {
            SetSound(SOUND_MENU_MARIOCASTLEWARP, D_803320E0);
        }
#endif
    }
}

// used for warps inside one level
static void func_8024A02C(void)
{
    if (sCurrWarpType != WARP_TYPE_NOT_WARPING)
    {
        if (sCurrWarpType == WARP_TYPE_CHANGE_AREA)
        {
            level_control_timer(TIMER_CONTROL_HIDE);
            func_8027AA88();
            load_area(sDestAreaIndex);
        }

        init_mario_after_warp();
    }
}

// used for warps between levels
static void func_8024A094(void)
{
    gCurrLevelNum = sDestLevelNum;

    level_control_timer(TIMER_CONTROL_HIDE);

    load_area(sDestAreaIndex);
    init_mario_after_warp();
}

static void func_8024A0E0(void)
{
    u32 marioAction;

    switch (sDestWarpNodeId)
    {
    case WARP_NODE_CREDITS_START:
        marioAction = ACT_END_PEACH_CUTSCENE;
        break;

    case WARP_NODE_CREDITS_NEXT:
        marioAction = ACT_CREDITS_CUTSCENE;
        break;

    case WARP_NODE_CREDITS_END:
        marioAction = ACT_END_WAVING_CUTSCENE;
        break;
    }

    gCurrLevelNum = sDestLevelNum;

    load_area(sDestAreaIndex);

    vec3s_set(
        gPlayerSpawnInfos[0].startPos,
        gCurrCreditsEntry->marioPos[0],
        gCurrCreditsEntry->marioPos[1],
        gCurrCreditsEntry->marioPos[2]);

    vec3s_set(
        gPlayerSpawnInfos[0].startAngle,
        0,
        0x100 * gCurrCreditsEntry->marioAngle,
        0);

    gPlayerSpawnInfos[0].areaIndex = sDestAreaIndex;

    load_mario_area();
    func_802548BC();

    set_mario_action(gMarioState, marioAction, 0);

    reset_camera(gCurrentArea->camera);

    sCurrWarpType = WARP_TYPE_NOT_WARPING;
    sDelayedWarpOp = WARP_OP_NONE;

    play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x14, 0x00, 0x00, 0x00);

    if (gCurrCreditsEntry == NULL || gCurrCreditsEntry == sCreditsSequence)
        func_80249148(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
}

static void check_instant_warp(void)
{
    s16 cameraAngle;
    struct Surface *floor;

    if (gCurrLevelNum == LEVEL_CASTLE &&
        save_file_get_total_star_count(gCurrSaveFileNum - 1, COURSE_MIN - 1, COURSE_MAX - 1) >= 70)
    {
        return;
    }

    if ((floor = gMarioState->floor) != NULL)
    {
        s32 index = floor->type - SURFACE_INSTANT_WARP_0;
        if (index >= 0 && index < 4 && gCurrentArea->instantWarps != NULL)
        {
            struct InstantWarp *warp = &gCurrentArea->instantWarps[index];

            if (warp->unk00 != 0)
            {
                gMarioState->pos[0] += warp->displacement[0];
                gMarioState->pos[1] += warp->displacement[1];
                gMarioState->pos[2] += warp->displacement[2];

                gMarioState->marioObj->oPosX = gMarioState->pos[0];
                gMarioState->marioObj->oPosY = gMarioState->pos[1];
                gMarioState->marioObj->oPosZ = gMarioState->pos[2];

                cameraAngle = gMarioState->area->camera->trueYaw;

                change_area(warp->area);
                gMarioState->area = gCurrentArea;

                instant_warp_camera_update(
                    warp->displacement[0],
                    warp->displacement[1],
                    warp->displacement[2]);

                gMarioState->area->camera->trueYaw = cameraAngle;
            }
        }
    }
}

static s16 func_8024A48C(s16 arg)
{
#if BUGFIX_KOOPA_RACE_MUSIC

    struct ObjectWarpNode *warpNode = area_get_warp_node(arg);
    s16 levelNum = warpNode->node.destLevel & 0x7F;
    s16 destArea = warpNode->node.destArea;
    s16 val4 = TRUE;
    s16 sp2C;

    if (levelNum == LEVEL_BOB && levelNum == gCurrLevelNum && destArea == gCurrAreaIndex)
    {
        sp2C = func_80320E98();
        if (sp2C == 1166 || sp2C == 1038)
            val4 = 0;
    }
    else
    {
        u16 val8 = gAreas[destArea].musicParam;
        u16 val6 = gAreas[destArea].musicParam2;

        val4 =
            levelNum == gCurrLevelNum &&
            val8 == gCurrentArea->musicParam &&
            val6 == gCurrentArea->musicParam2;

        if (func_80320E98() != val6)
            val4 = FALSE;
    }
    return val4;

#else

    struct ObjectWarpNode *warpNode = area_get_warp_node(arg);
    s16 levelNum = warpNode->node.destLevel & 0x7F;

    u16 val8 = gAreas[warpNode->node.destArea].musicParam;
    u16 val6 = gAreas[warpNode->node.destArea].musicParam2;

    s16 val4 =
        levelNum == gCurrLevelNum &&
        val8 == gCurrentArea->musicParam &&
        val6 == gCurrentArea->musicParam2;

    if (func_80320E98() != val6)
        val4 = FALSE;
    return val4;

#endif
}

/**
 * Set the current warp type and destination level/area/node.
 */
static void initiate_warp(s16 destLevel, s16 destArea, s16 destWarpNode, s32 arg3)
{
    if (destWarpNode >= WARP_NODE_CREDITS_MIN)
    {
        sCurrWarpType = WARP_TYPE_CHANGE_LEVEL;
    }
    else if (destLevel != gCurrLevelNum)
    {
        sCurrWarpType = WARP_TYPE_CHANGE_LEVEL;
    }
    else if (destArea != gCurrentArea->index)
    {
        sCurrWarpType = WARP_TYPE_CHANGE_AREA;
    }
    else
    {
        sCurrWarpType = WARP_TYPE_SAME_AREA;
    }

    sDestLevelNum = destLevel;
    sDestAreaIndex = destArea;
    sDestWarpNodeId = destWarpNode;
    sWarpArg = arg3;
}

/**
 * Check if mario is above and close to a painting warp floor, and return the
 * corresponding warp node.
 */
static struct WarpNode *get_painting_warp_node(void)
{
    struct WarpNode *warpNode = NULL;
    s32 paintingIndex = gMarioState->floor->type - SURFACE_PAINTING_WARP_0;

    if (paintingIndex >= 0 && paintingIndex < 0x2D)
    {
        if (paintingIndex < 0x2A ||
            gMarioState->pos[1] - gMarioState->floorHeight < 80.0f)
        {
            warpNode = &gCurrentArea->paintingWarpNodes[paintingIndex];
        }
    }

    return warpNode;
}

/**
 * Check is mario has entered a painting, and if so, initiate a warp.
 */
static void initiate_painting_warp(void)
{
    if (gCurrentArea->paintingWarpNodes != NULL && gMarioState->floor != NULL)
    {
        struct WarpNode warpNode;
        struct WarpNode *pWarpNode = get_painting_warp_node();

        if (pWarpNode != NULL)
        {
            if (gMarioState->action & ACT_FLAG_INTANGIBLE)
            {
                func_80248FBC();
            }
            else if (pWarpNode->id != 0)
            {
                warpNode = *pWarpNode;

                if (!(warpNode.destLevel & 0x80))
                    D_8032C9E0 = check_warp_checkpoint(&warpNode);

                initiate_warp(
                    warpNode.destLevel & 0x7F,
                    warpNode.destArea,
                    warpNode.destNode,
                    0);
                check_if_should_set_warp_checkpoint(&warpNode);

                play_transition_after_delay(WARP_TRANSITION_FADE_INTO_COLOR, 30, 255, 255, 255, 45);
                level_set_transition(74, basic_update);

                set_mario_action(gMarioState, ACT_DISAPPEARED, 0);

                gMarioState->marioObj->header.gfx.node.flags &= ~0x0001;

                SetSound(SOUND_MENU_STARSOUND, D_803320E0);
                func_802491FC(398);
            }
        }
    }
}

/**
 * If there is not already a delayed warp, schedule one. The source node is
 * based on the warp operation and sometimes mario's used object.
 * Return the time left until the delayed warp is initiated.
 */
s16 level_trigger_warp(struct MarioState *m, s32 warpOp)
{
    s32 val04 = TRUE;

    if (sDelayedWarpOp == WARP_OP_NONE)
    {
        m->invincTimer = -1;
        sDelayedWarpArg = 0;
        sDelayedWarpOp = warpOp;

        switch (warpOp)
        {
        case WARP_OP_DEMO_NEXT:
        case WARP_OP_DEMO_END:
            sDelayedWarpTimer = 20;
            sSourceWarpNodeId = WARP_NODE_F0;
            gSavedCourseNum = 0;
            val04 = FALSE;
            play_transition(WARP_TRANSITION_FADE_INTO_STAR, 0x14, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_CREDITS_END:
            sDelayedWarpTimer = 60;
            sSourceWarpNodeId = WARP_NODE_F0;
            val04 = FALSE;
            gSavedCourseNum = 0;
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x3C, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_STAR_EXIT:
            sDelayedWarpTimer = 32;
            sSourceWarpNodeId = WARP_NODE_F0;
            gSavedCourseNum = 0;
            play_transition(WARP_TRANSITION_FADE_INTO_MARIO, 0x20, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_DEATH:
            if (m->numLives == 0)
                sDelayedWarpOp = WARP_OP_GAME_OVER;
            sDelayedWarpTimer = 48;
            sSourceWarpNodeId = WARP_NODE_DEATH;
            play_transition(WARP_TRANSITION_FADE_INTO_BOWSER, 0x30, 0x00, 0x00, 0x00);
            SetSound(SOUND_MENU_BOWSERLAUGH, D_803320E0);
            break;

        case WARP_OP_WARP_FLOOR:
            sSourceWarpNodeId = WARP_NODE_WARP_FLOOR;
            if (area_get_warp_node(sSourceWarpNodeId) == NULL)
            {
                if (m->numLives == 0)
                    sDelayedWarpOp = WARP_OP_GAME_OVER;
                else
                    sSourceWarpNodeId = WARP_NODE_DEATH;
            }
            sDelayedWarpTimer = 20;
            play_transition(WARP_TRANSITION_FADE_INTO_CIRCLE, 0x14, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_UNKNOWN_01: // enter totwc
            sDelayedWarpTimer = 30;
            sSourceWarpNodeId = WARP_NODE_F2;
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0xFF, 0xFF, 0xFF);
#if VERSION_US
            SetSound(SOUND_MENU_STARSOUND, D_803320E0);
#endif
            break;

        case WARP_OP_UNKNOWN_02: // bbh enter
            sDelayedWarpTimer = 30;
            sSourceWarpNodeId = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0xFF, 0xFF, 0xFF);
            break;

        case WARP_OP_TELEPORT:
            sDelayedWarpTimer = 20;
            sSourceWarpNodeId = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
            val04 = !func_8024A48C(sSourceWarpNodeId);
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x14, 0xFF, 0xFF, 0xFF);
            break;

        case WARP_OP_WARP_DOOR:
            sDelayedWarpTimer = 20;
            sDelayedWarpArg = m->actionArg;
            sSourceWarpNodeId = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
            val04 = !func_8024A48C(sSourceWarpNodeId);
            play_transition(WARP_TRANSITION_FADE_INTO_CIRCLE, 0x14, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_WARP_OBJECT:
            sDelayedWarpTimer = 20;
            sSourceWarpNodeId = (m->usedObj->oBehParams & 0x00FF0000) >> 16;
            val04 = !func_8024A48C(sSourceWarpNodeId);
            play_transition(WARP_TRANSITION_FADE_INTO_STAR, 0x14, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_CREDITS_START:
            sDelayedWarpTimer = 30;
            play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x1E, 0x00, 0x00, 0x00);
            break;

        case WARP_OP_CREDITS_NEXT:
            if (gCurrCreditsEntry == &sCreditsSequence[0])
            {
                sDelayedWarpTimer = 60;
                play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x3C, 0x00, 0x00, 0x00);
            }
            else
            {
                sDelayedWarpTimer = 20;
                play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 0x14, 0x00, 0x00, 0x00);
            }
            val04 = FALSE;
            break;
        }

        if (val04 && gCurrDemoInput == NULL)
            func_802491FC((3 * sDelayedWarpTimer / 2) * 8 - 2);
    }

    return sDelayedWarpTimer;
}

/**
 * If a delayed warp is ready, initiate it.
 */
static void initiate_delayed_warp(void)
{
    struct ObjectWarpNode *warpNode;
    s32 destWarpNode;

    if (sDelayedWarpOp != WARP_OP_NONE && --sDelayedWarpTimer == 0)
    {
        func_802D8098();

        if (gDebugLevelSelect && (sDelayedWarpOp & WARP_OP_TRIGGERS_LEVEL_SELECT))
        {
            func_8024975C(-9);
        }
        else if (gCurrDemoInput != NULL)
        {
            if (sDelayedWarpOp == WARP_OP_DEMO_END)
                func_8024975C(-8);
            else
                func_8024975C(-2);
        }
        else
        {
            switch (sDelayedWarpOp)
            {
            case WARP_OP_GAME_OVER:
                save_file_reload();
                func_8024975C(-3);
                break;

            case WARP_OP_CREDITS_END:
                func_8024975C(-1);
                func_80320980(2, 0x3F0);
                break;

            case WARP_OP_DEMO_NEXT:
                func_8024975C(-2);
                break;

            case WARP_OP_CREDITS_START:
                gCurrCreditsEntry = &sCreditsSequence[0];
                initiate_warp(
                    gCurrCreditsEntry->levelNum,
                    gCurrCreditsEntry->areaIndex,
                    WARP_NODE_CREDITS_START,
                    0);
                break;

            case WARP_OP_CREDITS_NEXT:
                func_803208C0(2, 0x3FF);

                gCurrCreditsEntry += 1;
                gCurrActNum = gCurrCreditsEntry->unk02 & 0x07;
                if ((gCurrCreditsEntry + 1)->levelNum == 0)
                    destWarpNode = WARP_NODE_CREDITS_END;
                else
                    destWarpNode = WARP_NODE_CREDITS_NEXT;

                initiate_warp(
                    gCurrCreditsEntry->levelNum,
                    gCurrCreditsEntry->areaIndex,
                    destWarpNode,
                    0);
                break;

            default:
                warpNode = area_get_warp_node(sSourceWarpNodeId);

                initiate_warp(
                    warpNode->node.destLevel & 0x7F,
                    warpNode->node.destArea,
                    warpNode->node.destNode,
                    sDelayedWarpArg);

                check_if_should_set_warp_checkpoint(&warpNode->node);
                if (sCurrWarpType != WARP_TYPE_CHANGE_LEVEL)
                    level_set_transition(2, NULL);
                break;
            }
        }
    }
}

static void update_hud_values(void)
{
    if (gCurrCreditsEntry == NULL)
    {
        s16 numHealthWedges = gMarioState->health > 0 ? gMarioState->health >> 8 : 0;

        if (gCurrCourseNum > 0)
            gHudDisplayFlags |= HUD_DISPLAY_FLAG_COIN_COUNT;
        else
            gHudDisplayFlags &= ~HUD_DISPLAY_FLAG_COIN_COUNT;

        if (gDisplayedCoins < gMarioState->numCoins)
        {
            if (gGlobalTimer & 0x00000001)
            {
                u32 coinSound;
                if (gMarioState->action & (ACT_FLAG_SWIMMING | ACT_FLAG_METAL_WATER))
                    coinSound = SOUND_GENERAL_COINWATER1;
                else
                    coinSound = SOUND_GENERAL_COIN2;

                gDisplayedCoins += 1;
                SetSound(coinSound, gMarioState->marioObj->header.gfx.cameraToObject);
            }
        }

        if (gMarioState->numLives > 100)
            gMarioState->numLives = 100;

#if BUGFIX_MAX_LIVES
        if (gMarioState->numCoins > 999)
            gMarioState->numCoins = 999;

        if (gDisplayedCoins > 999)
            gDisplayedCoins = 999;
#else
        if (gMarioState->numCoins > 999)
            gMarioState->numLives = (s8) 999; //! Wrong variable
#endif

        gDisplayedStars = gMarioState->numStars;
        gDisplayedLives = gMarioState->numLives;
        gDisplayedKeys = gMarioState->numKeys;

        if (numHealthWedges > gDisplayedHealthWedges)
            SetSound(SOUND_MENU_POWERMETER, D_803320E0);
        gDisplayedHealthWedges = numHealthWedges;

        if (gMarioState->hurtCounter > 0)
            gHudDisplayFlags |= HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
        else
            gHudDisplayFlags &= ~HUD_DISPLAY_FLAG_EMPHASIZE_POWER;
    }
}

/**
 * Update objects, hud, and camera. This update function excludes things like
 * endless staircase, warps, pausing, etc. This is used when entering a painting,
 * presumably to allow painting and camera updating while avoiding triggering the
 * warp twice.
 */
static void basic_update(UNUSED s16 *arg)
{
    area_update_objects();
    update_hud_values();

    if (gCurrentArea != NULL)
        update_camera(gCurrentArea->camera);
}

static s32 play_mode_normal(void)
{
    if (gCurrDemoInput != NULL)
    {
        print_intro_text();
        if (gPlayer1Controller->buttonPressed & END_DEMO)
        {
            level_trigger_warp(
                gMarioState,
                gCurrLevelNum == LEVEL_PSS ? WARP_OP_DEMO_END : WARP_OP_DEMO_NEXT);
        }
        else if (!gWarpTransition.isActive && sDelayedWarpOp == WARP_OP_NONE &&
            (gPlayer1Controller->buttonPressed & START_BUTTON))
        {
            level_trigger_warp(gMarioState, WARP_OP_DEMO_NEXT);
        }
    }

    func_8024A02C();
    check_instant_warp();

    if (sTimerRunning && gTimerValueInFrames < 17999)
        gTimerValueInFrames += 1;

    area_update_objects();
    update_hud_values();

    if (gCurrentArea != NULL)
        update_camera(gCurrentArea->camera);

    initiate_painting_warp();
    initiate_delayed_warp();

    // If either initiate_painting_warp or initiate_delayed_warp initiated a
    // warp, change play mode accordingly.
    if (sCurrPlayMode == PLAY_MODE_NORMAL)
    {
        if (sCurrWarpType == WARP_TYPE_CHANGE_LEVEL)
        {
            set_play_mode(PLAY_MODE_CHANGE_LEVEL);
        }
        else if (sTransitionTimer != 0)
        {
            set_play_mode(PLAY_MODE_CHANGE_AREA);
        }
        else if (pressed_paused())
        {
            func_80248C28(1);
            gCameraMovementFlags |= CAM_MOVE_PAUSE_SCREEN;
            set_play_mode(PLAY_MODE_PAUSED);
        }
    }

    return 0;
}

static s32 play_mode_paused(void)
{
    if (D_8033A75E == 0)
    {
        func_802D9A14(1);
    }
    else if (D_8033A75E == 1)
    {
        func_80248CB8(1);
        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
        set_play_mode(PLAY_MODE_NORMAL);
    }
    else
    {
        // Exit level

        if (gDebugLevelSelect)
        {
            func_80249788(-9, 1);
        }
        else
        {
            initiate_warp(LEVEL_CASTLE, 1, 0x1F, 0);
            func_80249788(0, 0);
            gSavedCourseNum = 0;
        }

        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
    }

    return 0;
}

/**
 * Debug mode that lets you frame advance by pressing D-pad down. Unfortunately
 * it uses the pause camera, making it basically unusable in most levels.
 */
static s32 play_mode_frame_advance(void)
{
    if (gPlayer1Controller->buttonPressed & D_JPAD)
    {
        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
        play_mode_normal();
    }
    else if (gPlayer1Controller->buttonPressed & START_BUTTON)
    {
        gCameraMovementFlags &= ~CAM_MOVE_PAUSE_SCREEN;
        func_80248CB8(1);
        set_play_mode(PLAY_MODE_NORMAL);
    }
    else
    {
        gCameraMovementFlags |= CAM_MOVE_PAUSE_SCREEN;
    }

    return 0;
}

/**
 * Set the transition, which is a period of time after the warp is initiated
 * but before it actually occurs. If updateFunction is not NULL, it will be
 * called each frame during the transition.
 */
void level_set_transition(s16 length, void (*updateFunction)(s16 *))
{
    sTransitionTimer = length;
    sTransitionUpdate = updateFunction;
}

/**
 * Play the transition and then return to normal play mode.
 */
static s32 play_mode_change_area(void)
{
    //! This maybe was supposed to be sTransitionTimer == -1? sTransitionUpdate
    // is never set to -1.
    if (sTransitionUpdate == (void (*)(s16 *)) -1)
        update_camera(gCurrentArea->camera);
    else if (sTransitionUpdate != NULL)
        sTransitionUpdate(&sTransitionTimer);

    if (sTransitionTimer > 0)
        sTransitionTimer -= 1;

    //! If sTransitionTimer is -1, this will miss.
    if (sTransitionTimer == 0)
    {
        sTransitionUpdate = NULL;
        set_play_mode(PLAY_MODE_NORMAL);
    }

    return 0;
}

/**
 * Play the transition and then return to normal play mode.
 */
static s32 play_mode_change_level(void)
{
    if (sTransitionUpdate != NULL)
        sTransitionUpdate(&sTransitionTimer);

    //! If sTransitionTimer is -1, this will miss.
    if (--sTransitionTimer == -1)
    {
        gHudDisplayFlags = HUD_DISPLAY_NONE;
        sTransitionTimer = 0;
        sTransitionUpdate = NULL;

        if (sCurrWarpType != WARP_TYPE_NOT_WARPING)
            return sDestLevelNum;
        else
            return D_80339EE0;
    }

    return 0;
}

/**
 * Unused play mode. Doesn't call transition update and doesn't reset transition
 * at the end.
 */
static s32 play_mode_unused(void)
{
    if (--sTransitionTimer == -1)
    {
        gHudDisplayFlags = HUD_DISPLAY_NONE;

        if (sCurrWarpType != WARP_TYPE_NOT_WARPING)
            return sDestLevelNum;
        else
            return D_80339EE0;
    }

    return 0;
}

static s32 update_level(void)
{
    s32 val4;

    switch (sCurrPlayMode)
    {
    case PLAY_MODE_NORMAL:        val4 = play_mode_normal();        break;
    case PLAY_MODE_PAUSED:        val4 = play_mode_paused();        break;
    case PLAY_MODE_CHANGE_AREA:   val4 = play_mode_change_area();   break;
    case PLAY_MODE_CHANGE_LEVEL:  val4 = play_mode_change_level();  break;
    case PLAY_MODE_FRAME_ADVANCE: val4 = play_mode_frame_advance(); break;
    }

    if (val4 != 0)
    {
        func_80248C10();
        func_80248D90();
    }

    return val4;
}

static s32 init_level(void)
{
    s32 val4 = 0;

    set_play_mode(PLAY_MODE_NORMAL);

    sDelayedWarpOp = WARP_OP_NONE;
    sTransitionTimer = 0;
    D_80339EE0 = 0;

    if (gCurrCreditsEntry == NULL)
        gHudDisplayFlags = HUD_DISPLAY_DEFAULT;
    else
        gHudDisplayFlags = HUD_DISPLAY_NONE;

    sTimerRunning = 0;

    if (sCurrWarpType != WARP_TYPE_NOT_WARPING)
    {
        if (sDestWarpNodeId >= WARP_NODE_CREDITS_MIN)
            func_8024A0E0();
        else
            func_8024A094();
    }
    else
    {
        if (gPlayerSpawnInfos[0].areaIndex >= 0)
        {
            load_mario_area();
            func_802548BC();
        }

        if (gCurrentArea != NULL)
        {
            reset_camera(gCurrentArea->camera);

            if (gCurrDemoInput != NULL)
            {
                set_mario_action(gMarioState, ACT_IDLE, 0);
            }
            else if (gDebugLevelSelect == 0)
            {
                if (gMarioState->action != ACT_UNINITIALIZED)
                {
                    if (save_file_exists(gCurrSaveFileNum - 1))
                    {
                        set_mario_action(gMarioState, ACT_IDLE, 0);
                    }
                    else
                    {
                        set_mario_action(gMarioState, ACT_INTRO_CUTSCENE, 0);
                        val4 = 1;
                    }
                }
            }
        }

        if (val4 != 0)
            play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 0x5A, 0xFF, 0xFF, 0xFF);
        else
            play_transition(WARP_TRANSITION_FADE_FROM_STAR, 0x10, 0xFF, 0xFF, 0xFF);

        if (gCurrDemoInput == NULL)
        {
            func_80249148(gCurrentArea->musicParam, gCurrentArea->musicParam2, 0);
        }
    }

    if (gMarioState->action == ACT_INTRO_CUTSCENE)
        func_803208C0(2, 0x330);

    return 1;
}

/**
 * Initialize the current level if initOrUpdate is 0, or update the level if it
 * is 1.
 */
s32 lvl_init_or_update(s16 initOrUpdate, UNUSED s32 arg1)
{
    s32 result = 0;

    switch (initOrUpdate)
    {
    case 0: result = init_level(); break;
    case 1: result = update_level(); break;
    }

    return result;
}

s32 lvl_init_from_save_file(UNUSED s16 arg0, s32 levelNum)
{
    sCurrWarpType = WARP_TYPE_NOT_WARPING;
    sDelayedWarpOp = WARP_OP_NONE;
    gSaveFileDoesNotExist = !save_file_exists(gCurrSaveFileNum - 1);

    gCurrLevelNum = levelNum;
    gCurrCourseNum = COURSE_NONE;
    gSavedCourseNum = 0;
    gCurrCreditsEntry = NULL;
    gSpecialTripleJump = 0;

    func_80254CE0();
    disable_warp_checkpoint();
    save_file_move_cap_to_default_location();
    select_mario_cam_mode();
    func_802E2F40();

    return levelNum;
}

s32 lvl_set_current_level(UNUSED s16 arg0, s32 levelNum)
{
    s32 val4 = D_8032C9E0;

    D_8032C9E0 = 0;
    gCurrLevelNum = levelNum;
    gCurrCourseNum = gLevelToCourseNumTable[levelNum - 1];

    if (gCurrDemoInput != NULL ||
        gCurrCreditsEntry != NULL ||
        gCurrCourseNum == COURSE_NONE)
    {
        return 0;
    }

    if (gCurrLevelNum != LEVEL_BOWSER_1 &&
        gCurrLevelNum != LEVEL_BOWSER_2 &&
        gCurrLevelNum != LEVEL_BOWSER_3)
    {
        gMarioState->numCoins = 0;
        gDisplayedCoins = 0;
        gCurrCourseStarFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);
    }

    if (gSavedCourseNum != gCurrCourseNum)
    {
        gSavedCourseNum = gCurrCourseNum;
        nop_change_course();
        disable_warp_checkpoint();
    }

    if (gCurrCourseNum > COURSE_STAGES_MAX || val4 != 0)
        return 0;

    if (gDebugLevelSelect != 0 && gShowProfiler == 0)
        return 0;

    return 1;
}

/**
 * Play the "thank you so much for to playing my game" sound.
 */
s32 lvl_play_the_end_screen_sound(UNUSED s16 arg0, UNUSED s32 arg1)
{
    SetSound(SOUND_MENU_THANKYOUPLAYINGMYGAME, D_803320E0);
    return 1;
}
