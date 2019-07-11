#include <ultra64.h>

#include "sm64.h"
#include "game.h"
#include "sound_init.h"
#include "level_update.h"
#include "interaction.h"
#include "mario.h"
#include "mario_step.h"
#include "mario_actions_moving.h"
#include "save_file.h"
#include "area.h"
#include "camera.h"
#include "object_helpers.h"
#include "object_helpers2.h"
#include "moving_texture.h"
#include "ingame_menu.h"
#include "audio/interface_2.h"
#include "engine/math_util.h"
#include "engine/graph_node.h"
#include "engine/surface_collision.h"
#include "engine/behavior_script.h"
#include "behavior_data.h"
#include "object_list_processor.h"

// TODO: put this elsewhere
enum SaveOption
{
    SAVE_OPT_SAVE_AND_CONTINUE = 1,
    SAVE_OPT_SAVE_AND_QUIT,
    SAVE_OPT_CONTINUE_DONT_SAVE
};


static struct Object *sIntroWarpPipeObj;
static struct Object *sEndPeachObj;
static struct Object *sEndRightToadObj;
static struct Object *sEndLeftToadObj;
static struct Object *sEndJumboStarObj;
static UNUSED s32 sUnused;
static s16 sEndPeachAnimation;
static s16 sEndToadAnims[2];

static Vp sEndCutsceneVp = {{ {640, 480, 511, 0}, {640, 480, 511, 0} }};
static struct CreditsEntry *sDispCreditsEntry = NULL;

// related to peach gfx?
static s8 D_8032CBE4 = 0;
static s8 D_8032CBE8 = 0;
static s8 D_8032CBEC[7] = { 2, 3, 2, 1, 2, 3, 2 };

static u8 sStarsNeededForDialogue[6] = { 1, 3, 8, 30, 50, 70 };

// data for the jumbo star cutscene
static Vec4s sJumboStarKeyframes[27] =
{
    {     20,      0,    678,  -2916 },
    {     30,      0,    680,  -3500 },
    {     40,   1000,    700,  -4000 },
    {     50,   2500,    750,  -3500 },
    {     50,   3500,    800,  -2000 },
    {     50,   4000,    850,      0 },
    {     50,   3500,    900,   2000 },
    {     50,   2000,    950,   3500 },
    {     50,      0,   1000,   4000 },
    {     50,  -2000,   1050,   3500 },
    {     50,  -3500,   1100,   2000 },
    {     50,  -4000,   1150,      0 },
    {     50,  -3500,   1200,  -2000 },
    {     50,  -2000,   1250,  -3500 },
    {     50,      0,   1300,  -4000 },
    {     50,   2000,   1350,  -3500 },
    {     50,   3500,   1400,  -2000 },
    {     50,   4000,   1450,      0 },
    {     50,   3500,   1500,   2000 },
    {     50,   2000,   1600,   3500 },
    {     50,      0,   1700,   4000 },
    {     50,  -2000,   1800,   3500 },
    {     50,  -3500,   1900,   2000 },
    {     30,  -4000,   2000,      0 },
    {      0,  -3500,   2100,  -2000 },
    {      0,  -2000,   2200,  -3500 },
    {      0,      0,   2300,  -4000 },
};

static s32 sSparkleGenTheta = 0;
static s32 sSparkleGenPhi   = 0;

// blink twice then have half-shut eyes (see end_peach_cutscene_kiss_from_peach)
static u8 sMarioBlinkOverride[20] = {
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_CLOSED,      MARIO_EYES_CLOSED,
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_OPEN,        MARIO_EYES_OPEN,
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_CLOSED,      MARIO_EYES_CLOSED,
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_OPEN,        MARIO_EYES_OPEN,
    MARIO_EYES_HALF_CLOSED, MARIO_EYES_HALF_CLOSED,
    MARIO_EYES_CLOSED,      MARIO_EYES_CLOSED,
};


/** get_credits_str_width: Calculate width of a Credits String
 * Loop over each character in a credits string and increment the length. If the
 * character is a space, increment by 4; otherwise increment by 7. Once the next
 * character is a null character (equal to 0), stop counting the length since
 * that's the end of the string.
 */
static s32 get_credits_str_width(char *str)
{
    u32 c;
    s32 length = 0;

    while ((c = *str++) != 0)
        length += (c == ' ' ? 4 : 7);

    return length;
}

/** print_displaying_credits_entry: Print the current displaying Credits Entry
 * Called in render_game. This function checks if sDispCreditsEntry points to a
 * credits entry (see act_credits_cutscene), and if so, display it. The reason
 * this is called every frame in render_game is because the credits need to
 * display on top of everything else.
 * To print a credits entry, we take the first character of the first string,
 * subtract the value of the 0 character, and use that as the number of lines to
 * print, excluding the title. Then, we print the title (after advancing the
 * pointer by 1) at X 28, Y either 28 or 172, depending on the print at bottom
 * flag. Finally, we print each line using the number of lines previously stored
 * as a counter, and increase the Y value by either the constant 16 (JP only) or
 * by the value of lineHeight.
 */
void print_displaying_credits_entry(void)
{
    char **currStrPtr;
    char  *titleStr;
    s16 numLines;
    s16 strY;
#ifndef VERSION_JP
    s16 lineHeight;
#endif

    if (sDispCreditsEntry != NULL)
    {
        currStrPtr = (char **)sDispCreditsEntry->unk0C;
        titleStr = *currStrPtr++;
        numLines = *titleStr++ - '0';

        strY = (sDispCreditsEntry->unk02 & 0x20 ? 28 : 172) \
        + (numLines == 1) * 16;
    #ifndef VERSION_JP
        lineHeight = 16;
    #endif

        // init printing credits text?
        func_802D9A60();
        print_credits_str(28, strY, titleStr);

    #ifndef VERSION_JP
        switch (numLines)
        {
            case 4:
                print_credits_str(28, strY + 24, *currStrPtr++);
                numLines   = 2;
                lineHeight = 24;
                break;
            case 5:
                print_credits_str(28, strY + 16, *currStrPtr++);
                numLines = 3;
                break;
        }
    #endif

        // smart dev here, thinking ahead for when the cosmic ray hits the rdram
        // chips 23 years later and nearly causes upwarp 2
        while (numLines-- > 0)
        {
            print_credits_str(
                (320 - 28) - get_credits_str_width(*currStrPtr), strY,
                *currStrPtr
            );

        #ifdef VERSION_JP
            strY += 16;
        #else
            strY += lineHeight;
        #endif

            currStrPtr++;
        }

        // update credits fade?
        func_802D9AE8();
        sDispCreditsEntry = NULL;
    }
}

void BehEndPeachLoop(void)
{
    func_8029ED38(sEndPeachAnimation);
    if (func_8029F788())
    {
        // anims: 0-3, 4, 5, 6-8, 9, 10, 11
        if (sEndPeachAnimation < 3 || sEndPeachAnimation == 6 || sEndPeachAnimation == 7)
            sEndPeachAnimation++;
    }
}

void BehEndToadLoop(void)
{
    s32 toadAnimIndex = (gCurrentObject->oPosX >= 0.0f);

    func_8029ED38(sEndToadAnims[toadAnimIndex]);
    if (func_8029F788())
    {
        // 0-1, 2-3, 4, 5, 6, 7
        if (sEndToadAnims[toadAnimIndex] == 0 ||
            sEndToadAnims[toadAnimIndex] == 2)
            sEndToadAnims[toadAnimIndex]++;
    }
}

// Geo switch case function for controlling Peach's eye state.
s32 geo_switch_peach_eyes(s32 run, struct GraphNode *node, UNUSED s32 a2)
{
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    s16 timer;

    if (run == TRUE)
    {
        if (D_8032CBE4 == 0)
        {
            timer = (gAreaUpdateCounter + 0x20) >> 1 & 0x1F;
            if (timer < 7)
                switchCase->unk1E = D_8032CBE8 * 4 + D_8032CBEC[timer];
            else
                switchCase->unk1E = D_8032CBE8 * 4 + 1;
        }
        else
            switchCase->unk1E = D_8032CBE8 * 4 + D_8032CBE4 - 1;
    }

    return 0;
}

// unused
void Unknown80256FF8(u16 *a0)
{
    if (get_dialog_id() == -1)
        *a0 = 0;
}

/** get_star_collection_dialogue: Determine what dialogue should show when Mario
 ** collects a star.
 * Determines if Mario has collected enough stars to get a dialogue for it, and
 * if so, return the dialogue ID. Otherwise, return 0
 */
static s32 get_star_collection_dialogue(struct MarioState *m)
{
    s32 i;
    s32 dialogueID = 0;
    s32 numStarsRequired;

    for (i = 0; i < 6; i++)
    {
        numStarsRequired = sStarsNeededForDialogue[i];
        if (m->unkB8 < numStarsRequired && m->numStars >= numStarsRequired)
        {
            dialogueID = i + 0x8D;
            break;
        }
    }

    m->unkB8 = m->numStars;
    return dialogueID;
}

// save menu handler
static void handle_save_menu(struct MarioState *m)
{
    s32 dialogueID;
    // wait for the menu to show up
    // mario_finished_animation(m) ? (not my file, not my problem)
    if (func_802507AC(m) && D_8033A760 != 0)
    {
        // save and continue / save and quit
        if (D_8033A760 == SAVE_OPT_SAVE_AND_CONTINUE ||
            D_8033A760 == SAVE_OPT_SAVE_AND_QUIT)
        {
            save_file_do_save(gCurrSaveFileNum - 1);

            if (D_8033A760 == SAVE_OPT_SAVE_AND_QUIT)
                func_80249788(-2, 0); // reset game
        }

        // not quitting
        if (D_8033A760 != SAVE_OPT_SAVE_AND_QUIT)
        {
            disable_time_stop();
            m->faceAngle[1] += 0x8000;
            // figure out what dialogue to show, if we should
            dialogueID = get_star_collection_dialogue(m);
            if (dialogueID != 0)
            {
                play_peachs_jingle();
                // look up for dialogue
                set_mario_action(m, ACT_READING_AUTOMATIC_DIALOGUE, dialogueID);
            }
            else
            {
                set_mario_action(m, ACT_IDLE, 0);
            }
        }
    }
}

/** spawn_obj_at_mario_rel_yaw: Spawns object at Mario with relative yaw.
 * Spawns object with given behavior and model and copies over Mario's position
 * and yaw plus relative yaw.
 */
static struct Object *spawn_obj_at_mario_rel_yaw(
    struct MarioState *m,
    s32 model,
    void *behavior,
    s16 relYaw
)
{
    struct Object *o = spawn_object(m->marioObj, model, behavior);

    o->oFaceAngleYaw = m->faceAngle[1] + relYaw;
    o->oPosX = m->pos[0];
    o->oPosY = m->pos[1];
    o->oPosZ = m->pos[2];

    return o;
}

/** cutscene_take_cap_off: Put Mario's cap on.
 * Clears "cap on head" flag, sets "cap in hand" flag, plays sound
 * SOUND_ACTION_UNKNOWN43D.
 */
static void cutscene_take_cap_off(struct MarioState *m)
{
    m->flags &= ~MARIO_CAP_ON_HEAD;
    m->flags |=  MARIO_CAP_IN_HAND;
    SetSound(SOUND_ACTION_UNKNOWN43D, m->marioObj->header.gfx.cameraToObject);
}

/** cutscene_put_cap_on: Put Mario's cap on.
 * Clears "cap in hand" flag, sets "cap on head" flag, plays sound
 * SOUND_ACTION_UNKNOWN43E.
 */
static void cutscene_put_cap_on(struct MarioState *m)
{
    m->flags &= ~MARIO_CAP_IN_HAND;
    m->flags |=  MARIO_CAP_ON_HEAD;
    SetSound(SOUND_ACTION_UNKNOWN43E, m->marioObj->header.gfx.cameraToObject);
}

/** mario_ready_to_speak: Determine if Mario is able to speak to a NPC
 * The following conditions must be met in order for Mario to be considered
 * ready to speak.
 * 1: Mario's action must be in the stationary or moving action groups, or if
 *    not, he must be in the "waiting for dialogue" state.
 * 2: Mario mat not be riding a shell or be invulnerable.
 * 3: Mario must not be in first person mode.
 */
s32 mario_ready_to_speak(void)
{
    u32 actionGroup = gMarioState->action & ACT_GROUP_MASK;
    s32 isReadyToSpeak = FALSE;

    if (
        (
            gMarioState->action == ACT_WAITING_FOR_DIALOGUE ||
            actionGroup == ACT_GROUP_STATIONARY ||
            actionGroup == ACT_GROUP_MOVING
        ) &&
        (
            !(
                gMarioState->action &
                (ACT_FLAG_RIDING_SHELL | ACT_FLAG_INVULNERABLE)
            ) &&
            gMarioState->action != ACT_FIRST_PERSON
        )
    )
            isReadyToSpeak = TRUE;

    return isReadyToSpeak;
}

// (can) place mario in dialogue?
// initiate dialogue?
// return values:
// 0 = not in dialogue
// 1 = starting dialogue
// 2 = speaking
s32 set_mario_npc_dialogue(s32 actionArg)
{
    s32 dialogueState = 0;

    // in dialogue
    if (gMarioState->action == ACT_READING_NPC_DIALOGUE)
    {
        if (gMarioState->actionState < 8)
            dialogueState = 1; // starting dialogue
        if (gMarioState->actionState == 8)
        {
            if (actionArg == 0)
                gMarioState->actionState++; // exit dialogue
            else
                dialogueState = 2;
        }
    }
    else if (actionArg != 0 && mario_ready_to_speak())
    {
        gMarioState->usedObj = gCurrentObject;
        set_mario_action(gMarioState, ACT_READING_NPC_DIALOGUE, actionArg);
        dialogueState = 1; // starting dialogue
    }

    return dialogueState;
}

// actionargs:
// 1 : no head turn
// 2 : look up
// 3 : look down
// actionstate values:
// 0 - 7: looking toward npc
// 8: in dialogue
// 9 - 22: looking away from npc
// 23: end
static s32 act_reading_npc_dialogue(struct MarioState *m)
{
    s32 headTurnAmount = 0;
    s16 angleToNPC;

    if (m->actionArg == 2)
        headTurnAmount = -1024;
    if (m->actionArg == 3)
        headTurnAmount = 384;

    if (m->actionState < 8)
    {
        // turn to NPC
        angleToNPC = mario_angle_to_object(m, m->usedObj);
        m->faceAngle[1] = angleToNPC - approach_s32(
            (angleToNPC - m->faceAngle[1]) << 16 >> 16, 0, 2048, 2048
        );
        // turn head to npc
        m->actionTimer += headTurnAmount;
        // set animation
        set_mario_animation(
            m, m->heldObj == NULL ? \
            MARIO_ANIM_FIRST_PERSON : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ
        );
    }
    else if (m->actionState >= 9 && m->actionState < 17)
    {
        // look back from facing NPC
        m->actionTimer -= headTurnAmount;
    }
    else if (m->actionState == 23)
    {
        if (m->flags & MARIO_CAP_IN_HAND)
            set_mario_action(m, ACT_PUTTING_ON_CAP, 0);
        else
            set_mario_action(
                m, m->heldObj == NULL ? ACT_IDLE : ACT_UNKNOWN_007, 0
            );
    }
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    vec3s_set(m->unk98->unk12, m->actionTimer, 0, 0);

    if (m->actionState != 8)
        m->actionState++;

    return FALSE;
}

// puts mario in a state where he's waiting for (npc) dialogue; doesn't do much
static s32 act_waiting_for_dialogue(struct MarioState *m)
{
    set_mario_animation(
        m, m->heldObj == NULL ? \
        MARIO_ANIM_FIRST_PERSON : MARIO_ANIM_IDLE_WITH_LIGHT_OBJ
    );
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    return FALSE;
}

// makes mario disappear and triggers warp
static s32 act_disappeared(struct MarioState *m)
{
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    stop_and_set_height_to_floor(m);
    m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
    if (m->actionArg)
    {
        m->actionArg--;
        if ((m->actionArg & 0xFFFF) == 0)
            level_trigger_warp(m, m->actionArg >> 16);
    }
    return FALSE;
}

static s32 act_reading_automatic_dialogue(struct MarioState *m)
{
    u32 actionArg;

    m->actionState++;
    if (m->actionState == 2)
        enable_time_stop();
    if (m->actionState < 9)
    {
        set_mario_animation(
            m, m->prevAction == ACT_STAR_DANCE_WATER ? \
            MARIO_ANIM_WATER_IDLE : MARIO_ANIM_FIRST_PERSON
        );
        // always look up for automatic dialogues
        m->actionTimer -= 1024;
    }
    else
    {
        // set mario dialogue
        if (m->actionState == 9)
        {
            actionArg = m->actionArg;
            if (GET_HIGH_U16_OF_32(actionArg) == 0)
                func_802D7F90(GET_LOW_U16_OF_32(actionArg));
            else
                func_802D7FCC(GET_HIGH_U16_OF_32(actionArg), GET_LOW_U16_OF_32(actionArg));
        }
        // wait until dialogue is done
        else if (m->actionState == 10)
        {
            if (get_dialog_id() >= 0)
                m->actionState--;
        }
        // look back down
        else if (m->actionState < 19)
        {
            m->actionTimer += 1024;
        }
        // finished action
        else if (m->actionState == 25)
        {
            disable_time_stop();
            if (gSaveFileDoesNotExist)
            {
                gSaveFileDoesNotExist = FALSE;
                func_802492A0((0 << 8) | 4);
            }
            if (m->prevAction == ACT_STAR_DANCE_WATER)
                set_mario_action(m, ACT_WATER_IDLE, 0); // 100c star?
            else
                // make mario walk into door after star dialogue
                set_mario_action(m,
                    m->prevAction == ACT_UNLOCKING_STAR_DOOR ? \
                    ACT_WALKING : ACT_IDLE,
                    0
                );
        }
    }
    // apply head turn
    vec3s_set(m->unk98->unk12, m->actionTimer, 0, 0);
    return FALSE;
}

static s32 act_reading_sign(struct MarioState *m)
{
    struct Object *marioObj = m->marioObj;

    func_80250F50(m, SOUND_ACTION_UNKNOWN45B, MARIO_UNKNOWN_16);

    switch (m->actionState)
    {
        // start dialogue
        case 0:
            stop_mario(1);
            enable_time_stop();
            // reading sign
            set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
            m->actionState = 1;
            // intentional fall through
        // turn toward sign
        case 1:
            m->faceAngle[1] += marioObj->oMarioPoleUnk108 / 11;
            m->pos[0] += marioObj->oMarioReadingSignDPosX / 11.0f;
            m->pos[2] += marioObj->oMarioReadingSignDPosZ / 11.0f;
            // create the text box
            if (m->actionTimer++ == 10)
            {
                CreateTextBox(m->usedObj->oBehParams2ndByte);
                m->actionState = 2;
            }
            break;
        // in dialogue
        case 2:
            // dialogue finished
            if (gCurrLevelCamera->cutscene == 0)
            {
                disable_time_stop();
                set_mario_action(m, ACT_IDLE, 0);
            }
            break;
    }

    vec3f_copy(marioObj->header.gfx.pos, m->pos);
    vec3s_set(marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    return FALSE;
}

// debug free move action
static s32 act_debug_free_move(struct MarioState *m)
{
    struct Surface *surf;
    f32   floorHeight;
    Vec3f pos;
    f32   speed;
    u32   action;

    // integer immediates, generates convert instructions for some reason
    speed = gPlayer1Controller->buttonDown & B_BUTTON ? 4 : 1;
    if (gPlayer1Controller->buttonDown & L_TRIG)
        speed = 0.01f;

    set_mario_animation(m, MARIO_ANIM_A_POSE);
    vec3f_copy(pos, m->pos);

    if (gPlayer1Controller->buttonDown & U_JPAD)
        pos[1] += 16.0f * speed;
    if (gPlayer1Controller->buttonDown & D_JPAD)
        pos[1] -= 16.0f * speed;

    if (m->intendedMag > 0)
    {
        pos[0] += 32.0f * speed * sins(m->intendedYaw);
        pos[2] += 32.0f * speed * coss(m->intendedYaw);
    }

    func_8025181C(pos, 60.0f, 50.0f);

    floorHeight = find_floor(pos[0], pos[1], pos[2], &surf);
    if (surf != NULL)
    {
        if (pos[1] < floorHeight)
            pos[1] = floorHeight;
        vec3f_copy(m->pos, pos);
    }

    m->faceAngle[1] = m->intendedYaw;
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);

    if (gPlayer1Controller->buttonPressed == A_BUTTON)
    {
        if (m->pos[1] <= m->waterLevel - 100)
            action = ACT_WATER_IDLE;
        else
            action = ACT_IDLE;
        set_mario_action(m, action, 0);
    }

    return FALSE;
}

// star dance handler
static void general_star_dance_handler(struct MarioState *m, s32 isInWater)
{
    s32 dialogueID;
    if (m->actionState == 0)
    {
        switch (++m->actionTimer)
        {
            case 1:
                spawn_object(m->marioObj, MODEL_STAR, bhvCelebrationStar);
                func_80248D48();
                if (m->actionArg & 1)
                {
                    play_course_clear();
                }
                else
                {
                    if (gCurrLevelNum == LEVEL_BOWSER_1 ||
                        gCurrLevelNum == LEVEL_BOWSER_2)
                        func_80320AE8(1, (15 << 8) | 23, 0);
                    else
                        func_80320AE8(1, (15 << 8) | 1, 0);
                }
                break;

            case 42:
                SetSound(SOUND_MARIO_HEREWEGO, m->marioObj->header.gfx.cameraToObject);
                break;

            case 80:
                if ((m->actionArg & 1) == 0)
                {
                    level_trigger_warp(m, WARP_OP_STAR_EXIT);
                }
                else
                {
                    enable_time_stop();
                    func_802D8050(gLastCompletedStarNum == 7 ? 13 : 14);
                    m->actionState = 1;
                }
                break;
        }
    }
    else if (m->actionState == 1 && gDialogueResponse)
    {
        if (gDialogueResponse == 1)
            save_file_do_save(gCurrSaveFileNum - 1);
        m->actionState = 2;
    }
    else if (m->actionState == 2 && func_80250770(m))
    {
        disable_time_stop();
        func_80248D90();
        dialogueID = get_star_collection_dialogue(m);
        if (dialogueID != 0)
            // look up for dialogue
            set_mario_action(m, ACT_READING_AUTOMATIC_DIALOGUE, dialogueID);
        else
            set_mario_action(m, isInWater ? ACT_WATER_IDLE : ACT_IDLE, 0);
    }
}

static s32 act_star_dance(struct MarioState *m)
{
    m->faceAngle[1] = m->area->camera->trueYaw;
    set_mario_animation(
        m, m->actionState == 2 ? \
        MARIO_ANIM_RETURN_FROM_STAR_DANCE : MARIO_ANIM_STAR_DANCE
    );
    general_star_dance_handler(m, 0);
    if (m->actionState != 2 && m->actionTimer >= 40)
        m->unk98->handState = MARIO_HAND_PEACE_SIGN;
    stop_and_set_height_to_floor(m);
    return FALSE;
}

static s32 act_star_dance_water(struct MarioState *m)
{
    m->faceAngle[1] = m->area->camera->trueYaw;
    set_mario_animation(
        m, m->actionState == 2 ? \
        MARIO_ANIM_RETURN_FROM_WATER_STAR_DANCE : MARIO_ANIM_WATER_STAR_DANCE
    );
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    vec3s_set(m->marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);
    general_star_dance_handler(m, 1);
    if (m->actionState != 2 && m->actionTimer >= 62)
        m->unk98->handState = MARIO_HAND_PEACE_SIGN;
    return FALSE;
}

static s32 act_fall_after_star_grab(struct MarioState *m)
{
    if (m->pos[1] < m->waterLevel - 130)
    {
        SetSound(SOUND_ACTION_UNKNOWN430, m->marioObj->header.gfx.cameraToObject);
        m->particleFlags |= PARTICLE_6;
        return set_mario_action(m, ACT_STAR_DANCE_WATER, m->actionArg);
    }
    if (perform_air_step(m, 1) == AIR_STEP_LANDED)
    {
        func_80251280(m, SOUND_TERRAIN_2);
        set_mario_action(
            m,
            m->actionArg & 1 ? ACT_STAR_DANCE_NO_EXIT : ACT_STAR_DANCE_EXIT,
            m->actionArg
        );
    }
    set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    return FALSE;
}

// general death hander
static s32 common_death_handler(
    struct MarioState *m, s32 animation, s32 frameToDeathWarp
)
{
    s32 animFrame = set_mario_animation(m, animation);
    if (animFrame == frameToDeathWarp)
        level_trigger_warp(m, WARP_OP_DEATH);
    m->unk98->eyeState = MARIO_EYES_DEAD;
    stop_and_set_height_to_floor(m);
    return animFrame;
}

static s32 act_standing_death(struct MarioState *m)
{
    if (m->input & INPUT_UNKNOWN_8)
        return set_mario_action(m, ACT_SUFFOCATION, 0);

    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    common_death_handler(m, MARIO_ANIM_DYING_FALL_OVER, 80);
    if (m->marioObj->header.gfx.unk38.animFrame == 77)
        func_80251280(m, SOUND_TERRAIN_4);
    return FALSE;
}

static s32 act_electrocution(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    common_death_handler(m, MARIO_ANIM_ELECTROCUTION, 43);
    return FALSE;
}

static s32 act_suffocation(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    common_death_handler(m, MARIO_ANIM_SUFFOCATING, 86);
    return FALSE;
}

static s32 act_death_on_back(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    if (common_death_handler(m, MARIO_ANIM_DYING_ON_BACK, 54) == 40)
        func_80251348(m, SOUND_TERRAIN_4);
    return FALSE;
}

static s32 act_death_on_stomach(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    if (common_death_handler(m, MARIO_ANIM_DYING_ON_STOMACH, 37) == 37)
        func_80251348(m, SOUND_TERRAIN_4);
    return FALSE;
}

static s32 act_quicksand_death(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        set_mario_animation(m, MARIO_ANIM_DYING_IN_QUICKSAND);
        func_80250AAC(m, 60);
        m->actionState = 1;
    }
    if (m->actionState == 1)
    {
        if (m->quicksandDepth >= 100.0f)
        {
            func_80250F50(m, SOUND_MARIO_WAAAOOOW, MARIO_UNKNOWN_16);
        }
        if ((m->quicksandDepth += 5.0f) >= 180.0f)
        {
            level_trigger_warp(m, WARP_OP_DEATH);
            m->actionState = 2;
        }
    }
    stationary_ground_step(m);
    SetSound(SOUND_UNKNOWN_UNK1414, m->marioObj->header.gfx.cameraToObject);
    return FALSE;
}

static s32 act_eaten_by_bubba(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_DYING, MARIO_UNKNOWN_16);
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
    m->health = 0xFF;
    if (m->actionTimer++ == 60)
        level_trigger_warp(m, WARP_OP_DEATH);
    return FALSE;
}

// set animation and forwardVel; when perform_air_step returns AIR_STEP_LANDED,
// set the new action
static s32 launch_mario_until_land(
    struct MarioState *m,
    s32 endAction,
    s32 animation,
    f32 forwardVel
)
{
    s32 airStepLanded;
    mario_set_forward_vel(m, forwardVel);
    set_mario_animation(m, animation);
    airStepLanded = (perform_air_step(m, 0) == AIR_STEP_LANDED);
    if (airStepLanded)
        set_mario_action(m, endAction, 0);
    return airStepLanded;
}

static s32 act_unlocking_key_door(struct MarioState *m)
{
    m->faceAngle[1] = m->usedObj->oMoveAngleYaw;

    m->pos[0] = m->usedObj->oPosX + coss(m->faceAngle[1]) * 75.0f;
    m->pos[2] = m->usedObj->oPosZ + sins(m->faceAngle[1]) * 75.0f;

    if (m->actionArg & 2)
        m->faceAngle[1] += 0x8000;

    if (m->actionTimer == 0)
    {
        spawn_obj_at_mario_rel_yaw(m, MODEL_BOWSER_KEY, bhvDoorKey1, 0);
        set_mario_animation(m, MARIO_ANIM_UNLOCK_DOOR);
    }

    switch (m->marioObj->header.gfx.unk38.animFrame)
    {
        case 79:
            SetSound(SOUND_GENERAL_SWITCH4, m->marioObj->header.gfx.cameraToObject);
            break;
        case 111:
            SetSound(SOUND_GENERAL_SWITCH2, m->marioObj->header.gfx.cameraToObject);
            break;
    }

    func_80250E50(m);
    stop_and_set_height_to_floor(m);

    if (func_80250770(m))
    {
        if (m->usedObj->oBehParams >> 24 == 1)
        {
            save_file_set_flags(SAVE_FLAG_UNLOCKED_UPSTAIRS_DOOR);
            save_file_clear_flags(SAVE_FLAG_HAVE_KEY_2);
        }
        else
        {
            save_file_set_flags(SAVE_FLAG_UNLOCKED_BASEMENT_DOOR);
            save_file_clear_flags(SAVE_FLAG_HAVE_KEY_1);
        }
        set_mario_action(m, ACT_WALKING, 0);
    }

    m->actionTimer++;
    return FALSE;
}

static s32 act_unlocking_star_door(struct MarioState *m)
{
    switch (m->actionState)
    {
        case 0:
            m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
            if (m->actionArg & 2)
                m->faceAngle[1] += 0x8000;
            m->marioObj->oMarioReadingSignDPosX = m->pos[0];
            m->marioObj->oMarioReadingSignDPosZ = m->pos[2];
            set_mario_animation(m, MARIO_ANIM_SUMMON_STAR);
            m->actionState++;
            break;
        case 1:
            if (func_80250770(m))
            {
                spawn_object(m->marioObj, MODEL_STAR, bhvUnlockDoorStar);
                m->actionState++;
            }
            break;
        case 2:
            if (m->actionTimer++ == 70)
            {
                set_mario_animation(m, MARIO_ANIM_RETURN_STAR_APPROACH_DOOR);
                m->actionState++;
            }
            break;
        case 3:
            if (func_80250770(m))
            {
                save_file_set_flags(get_door_save_file_flag(m->usedObj));
                set_mario_action(m, ACT_READING_AUTOMATIC_DIALOGUE, 38);
            }
            break;
    }

    m->pos[0] = m->marioObj->oMarioReadingSignDPosX;
    m->pos[2] = m->marioObj->oMarioReadingSignDPosZ;

    func_80250E50(m);
    stop_and_set_height_to_floor(m);

    return FALSE;
}

// not sure what kind of door this is
static s32 act_entering_star_door(struct MarioState *m)
{
    f32 targetDX;
    f32 targetDZ;
    s16 targetAngle;

    if (m->actionTimer++ == 0)
    {
        m->interactObj->oInteractStatus = 0x00010000;

        // ~30 degrees / 1/12 rot
        targetAngle = m->usedObj->oMoveAngleYaw + 0x1555;
        if (m->actionArg & 2)
            targetAngle += 0x5556; // ~120 degrees / 1/3 rot (total 150d / 5/12)

        // targetDX and targetDZ are the offsets to add to mario's position to
        // have mario stand 150 units in front of the door

        targetDX = m->usedObj->oPosX + 150.0f * sins(targetAngle) - m->pos[0];
        targetDZ = m->usedObj->oPosZ + 150.0f * coss(targetAngle) - m->pos[2];

        m->marioObj->oMarioReadingSignDPosX = targetDX / 20.0f;
        m->marioObj->oMarioReadingSignDPosZ = targetDZ / 20.0f;

        m->faceAngle[1] = atan2s(targetDZ, targetDX);
    }

    // set mario's animation
    if (m->actionTimer < 15)
    {
        set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    }

    // go through door? for 20 frames
    else if (m->actionTimer < 35)
    {
        m->pos[0] += m->marioObj->oMarioReadingSignDPosX;
        m->pos[2] += m->marioObj->oMarioReadingSignDPosZ;

        func_80250934(m, MARIO_ANIM_WALKING, 0x00028000);
    }

    else
    {
        m->faceAngle[1] = m->usedObj->oMoveAngleYaw;

        if (m->actionArg & 2)
            m->faceAngle[1] += 0x8000;

        m->pos[0] += 12.0f * sins(m->faceAngle[1]);
        m->pos[2] += 12.0f * coss(m->faceAngle[1]);

        func_80250934(m, MARIO_ANIM_WALKING, 0x00028000);
    }

    stop_and_set_height_to_floor(m);

    if (m->actionTimer == 48)
        set_mario_action(m, ACT_IDLE, 0);

    return FALSE;
}

static s32 act_going_through_door(struct MarioState *m)
{
    if (m->actionTimer == 0)
    {
        if (m->actionArg & 1)
        {
            m->interactObj->oInteractStatus = 0x00010000;
            set_mario_animation(m, MARIO_ANIM_PULL_DOOR_WALK_IN);
        }
        else
        {
            m->interactObj->oInteractStatus = 0x00020000;
            set_mario_animation(m, MARIO_ANIM_PUSH_DOOR_WALK_IN);
        }
    }
    m->faceAngle[1] = m->usedObj->oMoveAngleYaw;
    m->pos[0] = m->usedObj->oPosX;
    m->pos[2] = m->usedObj->oPosZ;

    func_80250E50(m);
    stop_and_set_height_to_floor(m);

    if (m->actionArg & 4)
    {
        if (m->actionTimer == 16)
            level_trigger_warp(m, WARP_OP_WARP_DOOR);
    }
    else
    {
        if (func_80250770(m))
        {
            if (m->actionArg & 2)
                m->faceAngle[1] += 0x8000;
            set_mario_action(m, ACT_IDLE, 0);
        }
    }

    m->actionTimer++;
    return FALSE;
}

s32 act_warp_door_spawn(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        m->actionState = 1;
        if (m->actionArg & 1)
            m->usedObj->oInteractStatus = 0x00040000;
        else
            m->usedObj->oInteractStatus = 0x00080000;
    }
    else if (m->usedObj->oAction == 0)
    {
        if (gSaveFileDoesNotExist == TRUE && gCurrLevelNum == LEVEL_CASTLE)
            set_mario_action(m, ACT_READING_AUTOMATIC_DIALOGUE, 21);
        else
            set_mario_action(m, ACT_IDLE, 0);
    }
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    stop_and_set_height_to_floor(m);
    return FALSE;
}

static s32 act_emerge_from_pipe(struct MarioState *m)
{
    struct Object *marioObj = m->marioObj;

    if (m->actionTimer++ < 11)
    {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
        return FALSE;
    }

    marioObj->header.gfx.node.flags |= GRAPH_RENDER_01;

    func_80250F50(m, SOUND_MARIO_YAHOO, MARIO_UNKNOWN_17);

    if (gCurrLevelNum == 13)
    {
        if (gCurrAreaIndex == 2)
            func_80250F50(m, SOUND_MENU_EXITPIPE, MARIO_UNKNOWN_16);
        else
            func_80250F50(m, SOUND_MENU_ENTERPIPE, MARIO_UNKNOWN_16);
    }

    if (launch_mario_until_land(m, ACT_JUMP_LAND_STOP, MARIO_ANIM_SINGLE_JUMP, 8.0f))
    {
        mario_set_forward_vel(m, 0.0f);
        func_80251280(m, SOUND_TERRAIN_2);
    }
    return FALSE;
}

static s32 act_spawn_spin_airborne(struct MarioState *m)
{
    // entered water, exit action
    if (m->pos[1] < m->waterLevel - 100)
    {
        func_8024980C(0);
        return func_8025325C(m);
    }

    // updates all velocity variables based on m->forwardVel
    mario_set_forward_vel(m, m->forwardVel);

    // landed on floor, play spawn land animation
    if (perform_air_step(m, 0.0) == AIR_STEP_LANDED)
    {
        func_80251280(m, SOUND_TERRAIN_2);
        set_mario_action(m, ACT_SPAWN_SPIN_LANDING, 0);
    }

    // is 300 units above floor, spin and play woosh sounds
    if (m->actionState == 0 && m->pos[1] - m->floorHeight > 300.0f)
    {
        if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 0) // first anim frame
            SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);
    }

    // under 300 units above floor, enter freefall animation
    else
    {
        m->actionState = 1;
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    }

    return FALSE;
}

static s32 act_spawn_spin_landing(struct MarioState *m)
{
    stop_and_set_height_to_floor(m);
    set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
    if (func_80250770(m))
    {
        func_8024980C(0);
        set_mario_action(m, ACT_IDLE, 0);
    }
    return FALSE;
}

/** act_exit_airborne: Jump out of a level after collecting a Power Star (no
 ** sparkles)
 * Mario always faces a level entrance when he launches out of it, whether he
 * died or he collected a star/key. Because of that, we need him to move away
 * from the painting by setting his speed to -32.0f and have him face away from
 * the painting by adding 0x8000 (180 deg) to his graphics angle. We also set
 * his heal counter to 31 to restore 7.75 units of his health, and enable the
 * particle flag that generates sparkles.
 */
static s32 act_exit_airborne(struct MarioState *m)
{
    if (15 < m->actionTimer++ && \
        launch_mario_until_land(
            m, ACT_EXIT_LAND_SAVE_DIALOGUE, MARIO_ANIM_GENERAL_FALL, -32.0f
        )
    )
        // heal mario
        m->healCounter = 31;
    // rotate him to face away from the entrance
    m->marioObj->header.gfx.angle[1] += 0x8000;
    m->particleFlags |= PARTICLE_SPARKLES;
    return FALSE;
}

static s32 act_falling_exit_airborne(struct MarioState *m)
{
    if (
        launch_mario_until_land(
            m, ACT_EXIT_LAND_SAVE_DIALOGUE, MARIO_ANIM_GENERAL_FALL, 0.0f
        )
    )
        // heal mario
        m->healCounter = 31;
    // rotate mario to face away from the entrance
    m->marioObj->header.gfx.angle[1] += 0x8000;
    m->particleFlags |= PARTICLE_SPARKLES;
    return FALSE;
}

static s32 act_exit_land_save_dialogue(struct MarioState *m)
{
    s32 animFrame;
    stationary_ground_step(m);
    func_802512E4(m, SOUND_TERRAIN_2);
    switch (m->actionState)
    {
        // determine type of exit
        case 0:
            set_mario_animation(
                m, m->actionArg == 0 ? \
                MARIO_ANIM_GENERAL_LAND : MARIO_ANIM_LAND_FROM_SINGLE_JUMP
            );
            if (func_802507AC(m))
            {
                if (gLastCompletedCourseNum != COURSE_BITDW && gLastCompletedCourseNum != COURSE_BITFS)
                    enable_time_stop();

                func_802D9A14(2);
                D_8033A760 = 0;

                m->actionState = 3; // star exit with cap
                if (!(m->flags & MARIO_CAP_ON_HEAD))
                    m->actionState = 2; // star exit without cap
                if (gLastCompletedCourseNum == COURSE_BITDW || gLastCompletedCourseNum == COURSE_BITFS)
                    m->actionState = 1; // key exit
            }
            break;
        // key exit
        case 1:
            animFrame = set_mario_animation(m, MARIO_ANIM_THROW_CATCH_KEY);
            switch (animFrame)
            {
                case -1:
                    spawn_obj_at_mario_rel_yaw(m, 200, bhvDoorKey2, -32768);
                    //! fall through
                case 67:
                    SetSound(SOUND_ACTION_SWISH1, m->marioObj->header.gfx.cameraToObject);
                    //! fall through
                case 83:
                    SetSound(SOUND_ACTION_PATBACK, m->marioObj->header.gfx.cameraToObject);
                    //! fall through
                case 111:
                    SetSound(SOUND_ACTION_UNKNOWN45C, m->marioObj->header.gfx.cameraToObject);
                    // no break
            }
            handle_save_menu(m);
            break;
        // exit without cap
        case 2:
            animFrame = set_mario_animation(m, MARIO_ANIM_MISSING_CAP);
            if ((animFrame >= 18 && animFrame < 55) || (animFrame >= 112 && animFrame < 134))
                m->unk98->handState = MARIO_HAND_OPEN;
            if (!(animFrame < 109) && animFrame < 154)
                m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;

            handle_save_menu(m);
            break;
        // exit with cap
        case 3:
            animFrame = set_mario_animation(m, MARIO_ANIM_TAKE_CAP_OFF_THEN_ON);
            switch (animFrame)
            {
                case 12:
                    cutscene_take_cap_off(m);
                    break;
                case 37:
                // fall through
                case 53:
                    SetSound(SOUND_ACTION_BRUSHHAIR, m->marioObj->header.gfx.cameraToObject);
                    break;
                case 82:
                    cutscene_put_cap_on(m);
                    break;
            }
            handle_save_menu(m);
            break;
    }

    m->marioObj->header.gfx.angle[1] += 0x8000;
    return FALSE;
}

static s32 act_death_exit(struct MarioState *m)
{
    if (15 < m->actionTimer++ && \
        launch_mario_until_land(
            m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, -32.0f
        )
    )
    {
        #ifdef VERSION_JP
        SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
        #else
        SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
        #endif
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

static s32 act_unused_death_exit(struct MarioState *m)
{
    if (
        launch_mario_until_land(
            m, ACT_FREEFALL_LAND_STOP, MARIO_ANIM_GENERAL_FALL, 0.0f
        )
    )
    {
        #ifdef VERSION_JP
        SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
        #else
        SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
        #endif
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

static s32 act_falling_death_exit(struct MarioState *m)
{
    if (
        launch_mario_until_land(
            m, ACT_DEATH_EXIT_LAND, MARIO_ANIM_GENERAL_FALL, 0.0f
        )
    )
    {
        #ifdef VERSION_JP
        SetSound(SOUND_MARIO_OOOF, m->marioObj->header.gfx.cameraToObject);
        #else
        SetSound(SOUND_MARIO_OOOF2, m->marioObj->header.gfx.cameraToObject);
        #endif
        m->numLives--;
        // restore 7.75 units of health
        m->healCounter = 31;
    }
    // one unit of health
    m->health = 0x0100;
    return FALSE;
}

// waits 11 frames before actually executing, also has reduced fvel
static s32 act_special_exit_airborne(struct MarioState *m)
{
    struct Object *marioObj = m->marioObj;

    func_80250F50(m, SOUND_MARIO_YAHOO, MARIO_UNKNOWN_17);

    if (m->actionTimer++ < 11)
    {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
        return FALSE;
    }

    if (
        launch_mario_until_land(
            m, ACT_EXIT_LAND_SAVE_DIALOGUE, MARIO_ANIM_SINGLE_JUMP, -24.0f
        )
    )
    {
        // heal mario
        m->healCounter = 31;
        m->actionArg = 1;
    }

    m->particleFlags |= PARTICLE_SPARKLES;
    // rotate mario to face away from the entrance
    marioObj->header.gfx.angle[1] += 0x8000;
    // show mario
    marioObj->header.gfx.node.flags |= GRAPH_RENDER_01;

    return FALSE;
}

static s32 act_special_death_exit(struct MarioState *m)
{
    struct Object *marioObj = m->marioObj;

    if (m->actionTimer++ < 11)
    {
        marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
        return FALSE;
    }

    if (launch_mario_until_land(
        m,
        ACT_HARD_BACKWARD_GROUND_KB,
        MARIO_ANIM_BACKWARD_AIR_KB,
        -24.0f
    ))
    {
        m->numLives--;
        m->healCounter = 31;
    }
    // show mario
    marioObj->header.gfx.node.flags |= GRAPH_RENDER_01;
    // one unit of health
    m->health = 0x0100;

    return FALSE;
}

static s32 act_spawn_no_spin_airborne(struct MarioState *m)
{
    launch_mario_until_land(
        m, ACT_SPAWN_NO_SPIN_LANDING, MARIO_ANIM_GENERAL_FALL, 0.0f
    );
    if (m->pos[1] < m->waterLevel - 100)
        func_8025325C(m);
    return FALSE;
}

static s32 act_spawn_no_spin_landing(struct MarioState *m)
{
    func_802512E4(m, SOUND_TERRAIN_2);
    set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
    stop_and_set_height_to_floor(m);
    if (func_80250770(m))
    {
        func_8024980C(0);
        set_mario_action(m, ACT_IDLE, 0);
    }
    return FALSE;
}

static s32 act_bbh_enter_spin(struct MarioState *m)
{
    f32 floorDist;
    f32 scale;
    f32 cageDX;
    f32 cageDZ;
    f32 cageDist;
    f32 forwardVel;

    cageDX = m->usedObj->oPosX - m->pos[0];
    cageDZ = m->usedObj->oPosZ - m->pos[2];
    cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

    if (cageDist > 20.0f)
        forwardVel = 10.0f;
    else
        forwardVel = cageDist / 2.0f;
    if (forwardVel < 0.5f)
        forwardVel = 0.0f;

    switch (m->actionState)
    {
        case 0:
            floorDist = 512.0f - (m->pos[1] - m->floorHeight);
            m->vel[1] = floorDist > 0 ? \
                sqrtf(4.0f * floorDist + 1.0f) - 1.0f : 2.0f;

            m->actionState = 1;
            m->actionTimer = 100;
            // fall through

        case 1:
            m->faceAngle[1] = atan2s(cageDZ, cageDX);
            mario_set_forward_vel(m, forwardVel);

            if (set_mario_animation(m, MARIO_ANIM_FORWARD_SPINNING) == 0)
                SetSound(SOUND_ACTION_SWISH2, m->marioObj->header.gfx.cameraToObject);

            m->flags &= ~MARIO_UNKNOWN_08;
            perform_air_step(m, 0);
            if (m->vel[1] <= 0)
                m->actionState = 2;
            break;

        case 2:
            // fall through
        case 3:
            m->faceAngle[1] = atan2s(cageDZ, cageDX);
            mario_set_forward_vel(m, forwardVel);
            m->flags &= ~MARIO_UNKNOWN_08;
            if (perform_air_step(m, 0) == AIR_STEP_LANDED)
            {
                level_trigger_warp(m, WARP_OP_UNKNOWN_02);
                m->actionState = 4;
            }
            if (m->actionState == 2)
            {
                if (m->marioObj->header.gfx.unk38.animFrame == 0)
                    m->actionState = 3;
            }
            else
            {
                func_80250F50(m, SOUND_ACTION_UNKNOWN446, MARIO_UNKNOWN_16);
                set_mario_animation(m, MARIO_ANIM_DIVE);
                m->marioObj->header.gfx.angle[0] = atan2s(
                    m->forwardVel,
                    -m->vel[1]
                );
            }
            m->squishTimer = 0xFF;
            if (m->actionTimer >= 11)
            {
                m->actionTimer -= 6;
                scale = m->actionTimer / 100.0f;
                vec3f_set(m->marioObj->header.gfx.scale, scale, scale, scale);
            }
            break;

        case 4:
            stop_and_set_height_to_floor(m);
            m->marioObj->header.gfx.node.flags |= GRAPH_RENDER_10;
            break;
    }

    return FALSE;
}

static s32 act_bbh_enter_jump(struct MarioState *m)
{
    f32 cageDX;
    f32 cageDZ;
    f32 cageDist;

    func_80251218(m, m->flags & MARIO_METAL_CAP ? SOUND_ACTION_UNKNOWN428 : SOUND_TERRAIN_1, 1);
    func_80250FBC(m);

    if (m->actionState == 0)
    {
        cageDX = m->usedObj->oPosX - m->pos[0];
        cageDZ = m->usedObj->oPosZ - m->pos[2];
        cageDist = sqrtf(cageDX * cageDX + cageDZ * cageDZ);

        m->vel[1] = 60.0f;
        m->faceAngle[1] = atan2s(cageDZ, cageDX);
        mario_set_forward_vel(m, cageDist / 20.0f);

        m->flags &= ~MARIO_UNKNOWN_08;
        m->actionState = 1;
    }

    set_mario_animation(m, MARIO_ANIM_DOUBLE_JUMP_RISE);
    perform_air_step(m, 0);

    if (m->vel[1] <= 0.0f)
        set_mario_action(m, ACT_BBH_ENTER_SPIN, 0);

    return FALSE;
}

static s32 act_teleport_fade_out(struct MarioState *m)
{
    func_80250F50(m, SOUND_ACTION_UNKNOWN457, MARIO_UNKNOWN_16);
    set_mario_animation(
        m, m->prevAction == ACT_CROUCHING ? \
        MARIO_ANIM_CROUCHING : MARIO_ANIM_FIRST_PERSON
    );
    m->flags |= MARIO_TELEPORTING;

    if (m->actionTimer < 32)
        m->fadeWarpOpacity = (-m->actionTimer << 3) + 0xF8;

    if (m->actionTimer++ == 20)
        level_trigger_warp(m, WARP_OP_TELEPORT);

    stop_and_set_height_to_floor(m);

    return FALSE;
}

static s32 act_teleport_fade_in(struct MarioState *m)
{
    func_80250F50(m, SOUND_ACTION_UNKNOWN457, MARIO_UNKNOWN_16);
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);

    if (m->actionTimer < 32)
    {
        m->flags |= MARIO_TELEPORTING;
        m->fadeWarpOpacity = m->actionTimer << 3;
    }
    else
    {
        m->flags &= ~MARIO_TELEPORTING;
    }

    if (m->actionTimer++ == 32)
    {
        if (m->pos[1] < m->waterLevel - 100)
        {
            // Check if the camera is not underwater.
            if (m->area->camera->currPreset != CAMERA_PRESET_WATER_SURFACE)
                // camera related function?
                func_80285BD8(m->area->camera, 8, 1);
            set_mario_action(m, ACT_WATER_IDLE, 0);
        }
        else
        {
            set_mario_action(m, ACT_IDLE, 0);
        }
    }

    stop_and_set_height_to_floor(m);

    return FALSE;
}

// act shocked :omega:
static s32 act_shocked(struct MarioState *m)
{
    func_80250F50(m, SOUND_MARIO_WAAAOOOW, MARIO_UNKNOWN_16);
    SetSound(SOUND_UNKNOWN_UNK1416, m->marioObj->header.gfx.cameraToObject);
    set_camera_shake(SHAKE_SHOCK);

    if (set_mario_animation(m, MARIO_ANIM_SHOCKED) == 0)
    {
        m->actionTimer++;
        m->flags |= MARIO_UNKNOWN_06;
    }

    if (m->actionArg == 0)
    {
        mario_set_forward_vel(m, 0.0f);
        if (perform_air_step(m, 1) == AIR_STEP_LANDED)
        {
            func_80251280(m, SOUND_TERRAIN_2);
            m->actionArg = 1;
        }
    }
    else
    {
        if (m->actionTimer >= 6)
        {
            m->invincTimer = 30;
            set_mario_action(
                m,
                m->health < 0x0100 ? ACT_ELECTROCUTION : ACT_IDLE,
                0
            );
        }
        stop_and_set_height_to_floor(m);
    }

    return FALSE;
}

// no one will get the joke behind this so just ignore it

// ok this has to be squish
//
// ...:squish:
static s32 act_squished(struct MarioState *m)
{
    UNUSED s32 pad;
    f32 squishAmount;
    f32 spaceUnderCeil;
    s16 surfAngle;
    s32 underSteepSurf = FALSE; // seems to be responsible for setting velocity?

    if ((spaceUnderCeil = m->ceilHeight - m->floorHeight) < 0)
        spaceUnderCeil = 0;

    switch (m->actionState)
    {
        case 0:
            if (spaceUnderCeil > 160.0f)
            {
                m->squishTimer = 0;
                return set_mario_action(m, ACT_IDLE, 0);
            }

            m->squishTimer = 0xFF;

            if (spaceUnderCeil >= 10.1f)
            {
                // mario becomes a pancake
                squishAmount = spaceUnderCeil / 160.0f;
                vec3f_set(
                    m->marioObj->header.gfx.scale,
                    2.0f - squishAmount,
                    squishAmount,
                    2.0f - squishAmount
                );
            }
            else
            {
                if (!(m->flags & MARIO_METAL_CAP) && m->invincTimer == 0)
                {
                    // cap on: 3 units; cap off: 4.5 units
                    m->hurtCounter += m->flags & MARIO_CAP_ON_HEAD ? 12 : 18;
                    func_80250F50(m, SOUND_MARIO_ATTACKED, MARIO_UNKNOWN_17);
                }

                vec3f_set(m->marioObj->header.gfx.scale, 1.8f, 0.05f, 1.8f);
                m->actionState = 1;
            }
            break;
        case 1:
            if (spaceUnderCeil >= 30.0f)
                m->actionState = 2;
            break;
        case 2:
            m->actionTimer++;
            if (m->actionTimer >= 15)
            {
                // 1 unit of health
                if (m->health < 0x0100)
                {
                    level_trigger_warp(m, WARP_OP_DEATH);
                    // woosh, he's gone!
                    set_mario_action(m, ACT_DISAPPEARED, 0);
                }
                else if (m->hurtCounter == 0)
                {
                    // un-squish animation
                    m->squishTimer = 30;
                    set_mario_action(m, ACT_IDLE, 0);
                }
            }
            break;
    }

    // steep floor
    if (m->floor != NULL && m->floor->normal.y < 0.5f)
    {
        surfAngle = atan2s(m->floor->normal.z, m->floor->normal.x);
        underSteepSurf = TRUE;
    }
    // steep ceiling
    if (m->ceil != NULL && -0.5f < m->ceil->normal.y)
    {
        surfAngle = atan2s(m->ceil->normal.z, m->ceil->normal.x);
        underSteepSurf = TRUE;
    }

    if (underSteepSurf)
    {
        m->vel[0] = sins(surfAngle) * 10.0f;
        m->vel[2] = coss(surfAngle) * 10.0f;
        m->vel[1] = 0;

        // check if there's no floor 10 units away from the surface
        if (perform_ground_step(m) == GROUND_STEP_LEFT_GROUND)
        {
            // instant un-squish
            m->squishTimer = 0;
            set_mario_action(m, ACT_IDLE, 0);
            return FALSE;
        }
    }

    // squished for more than 10 seconds, so kill mario
    if (m->actionArg++ > 300)
    {
        // 0 units of health
        m->health = 0x00FF;
        m->hurtCounter = 0;
        level_trigger_warp(m, WARP_OP_DEATH);
        // woosh, he's gone!
        set_mario_action(m, ACT_DISAPPEARED, 0);
    }
    stop_and_set_height_to_floor(m);
    set_mario_animation(m, MARIO_ANIM_A_POSE);
    return FALSE;
}

static s32 act_putting_on_cap(struct MarioState *m)
{
    s32 animFrame = set_mario_animation(m, MARIO_ANIM_PUT_CAP_ON);

    if (animFrame == 0)
        enable_time_stop();

    if (animFrame == 28)
        cutscene_put_cap_on(m);

    if (func_80250770(m))
    {
        set_mario_action(m, ACT_IDLE, 0);
        disable_time_stop();
    }

    stationary_ground_step(m);
    return FALSE;
}

static void stuck_in_ground_handler(
    struct MarioState *m, s32 animation,
    s32 unstuckFrame, s32 target2, s32 target3,
    s32 endAction
)
{
    s32 animFrame = set_mario_animation(m, animation);

    if (m->input & INPUT_A_PRESSED)
    {
        m->actionTimer++;
        if (m->actionTimer >= 5 && animFrame < unstuckFrame - 1)
        {
            animFrame = unstuckFrame - 1;
            func_80250AAC(m, animFrame);
        }
    }

    stop_and_set_height_to_floor(m);

    if (animFrame == -1)
        func_802510E4(m, SOUND_TERRAIN_6, 1);

    else if (animFrame == unstuckFrame)
        func_802510E4(m, SOUND_ACTION_UNKNOWN443, 1);

    else if (animFrame == target2 || animFrame == target3)
        func_80251280(m, SOUND_TERRAIN_2);

    if (func_80250770(m))
        set_mario_action(m, endAction, 0);
}

static s32 act_head_stuck_in_ground(struct MarioState *m)
{
    stuck_in_ground_handler(
        m, MARIO_ANIM_HEAD_STUCK_IN_GROUND, 96, 105, 135, ACT_IDLE
    );
    return FALSE;
}

static s32 act_butt_stuck_in_ground(struct MarioState *m)
{
    stuck_in_ground_handler(
        m, MARIO_ANIM_BOTTOM_STUCK_IN_GROUND, 127, 136, -2, ACT_GROUND_POUND_LAND
    );
    return FALSE;
}

static s32 act_feet_stuck_in_ground(struct MarioState *m)
{
    stuck_in_ground_handler(
        m, MARIO_ANIM_LEGS_STUCK_IN_GROUND, 116, 129, -2, ACT_IDLE
    );
    return FALSE;
}

/** advance_cutscene_step: Advances the current step in the current cutscene.
 * Resets action state and action timer, adds 1 to the action arg (responsible
 * for keeping track of what step of the cutscene Mario is in.)
 */
static void advance_cutscene_step(struct MarioState *m)
{
    m->actionState = 0;
    m->actionTimer = 0;
    m->actionArg++;
}

static void intro_cutscene_hide_hud_and_mario(struct MarioState *m)
{
    gHudDisplayFlags = HUD_DISPLAY_NONE;
    m->unk94->unk1C[1] = 9;
    m->marioObj->header.gfx.node.flags &= ~GRAPH_RENDER_01;
    advance_cutscene_step(m);
}

static void intro_cutscene_peach_lakitu_scene(struct MarioState *m)
{
    if ((s16)m->unk94->unk1C[1] != 9)
    {
        if (m->actionTimer++ == 37)
        {
            sIntroWarpPipeObj = spawn_object_abs_with_rot(
                gCurrentObject, 0, MODEL_CASTLE_GROUNDS_WARP_PIPE, bhvStaticObject,
                -1328, 60, 4664,
                0, 180, 0
            );
            advance_cutscene_step(m);
        }
    }
}

static void intro_cutscene_raise_pipe(struct MarioState *m)
{
    sIntroWarpPipeObj->oPosY = camera_approach_f32_symmetric(sIntroWarpPipeObj->oPosY, 260.0f, 10.0f);

    if (m->actionTimer == 0)
        SetSound(SOUND_MENU_EXITPIPE, sIntroWarpPipeObj->header.gfx.cameraToObject);

    if (m->actionTimer++ == 38)
    {
        m->vel[1] = 60.0f;
        advance_cutscene_step(m);
    }
}

static void intro_cutscene_jump_out_of_pipe(struct MarioState *m)
{
    if (m->actionTimer == 25)
        gHudDisplayFlags = HUD_DISPLAY_DEFAULT;

    if (m->actionTimer++ >= 118)
    {
        m->marioObj->header.gfx.node.flags |= GRAPH_RENDER_01;

        func_80250F50(m, SOUND_MARIO_YAHOO, MARIO_UNKNOWN_17);
        #ifndef VERSION_JP
        func_80250F50(m, SOUND_ACTION_UNKNOWN444_3, MARIO_UNKNOWN_16);
        #endif

        set_mario_animation(m, MARIO_ANIM_SINGLE_JUMP);
        mario_set_forward_vel(m, 10.0f);
        if (perform_air_step(m, 0) == AIR_STEP_LANDED)
        {
            func_80320980(2, 816);
            func_80251280(m, SOUND_TERRAIN_2);
            #ifndef VERSION_JP
            SetSound(SOUND_MARIO_HAHA, m->marioObj->header.gfx.cameraToObject);
            #endif
            advance_cutscene_step(m);
        }
    }
}

static void intro_cutscene_land_outside_pipe(struct MarioState *m)
{
    set_mario_animation(m, MARIO_ANIM_LAND_FROM_SINGLE_JUMP);

    if (func_80250770(m))
        advance_cutscene_step(m);

    stop_and_set_height_to_floor(m);
}

static void intro_cutscene_lower_pipe(struct MarioState *m)
{
    if (m->actionTimer++ == 0)
    {
        SetSound(SOUND_MENU_ENTERPIPE, sIntroWarpPipeObj->header.gfx.cameraToObject);
        set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
    }

    sIntroWarpPipeObj->oPosY -= 5.0f;
    if (sIntroWarpPipeObj->oPosY <= 50.0f)
    {
        mark_object_for_deletion(sIntroWarpPipeObj);
        advance_cutscene_step(m);
    }

    stop_and_set_height_to_floor(m);
}

static void intro_cutscene_set_mario_to_idle(struct MarioState *m)
{
    if (gCurrLevelCamera->cutscene == 0)
    {
        gCameraMovementFlags &= ~CAM_MOVE_C_UP_MODE;
        set_mario_action(m, ACT_IDLE, 0);
    }

    stop_and_set_height_to_floor(m);
}

enum
{
    INTRO_CUTSCENE_HIDE_HUD_AND_MARIO,
    INTRO_CUTSCENE_PEACH_LAKITU_SCENE,
    INTRO_CUTSCENE_RAISE_PIPE,
    INTRO_CUTSCENE_JUMP_OUT_OF_PIPE,
    INTRO_CUTSCENE_LAND_OUTSIDE_PIPE,
    INTRO_CUTSCENE_LOWER_PIPE,
    INTRO_CUTSCENE_SET_MARIO_TO_IDLE
};

static s32 act_intro_cutscene(struct MarioState *m)
{
    switch (m->actionArg)
    {
        case INTRO_CUTSCENE_HIDE_HUD_AND_MARIO: intro_cutscene_hide_hud_and_mario(m); break;
        case INTRO_CUTSCENE_PEACH_LAKITU_SCENE: intro_cutscene_peach_lakitu_scene(m); break;
        case INTRO_CUTSCENE_RAISE_PIPE:         intro_cutscene_raise_pipe(m);         break;
        case INTRO_CUTSCENE_JUMP_OUT_OF_PIPE:   intro_cutscene_jump_out_of_pipe(m);   break;
        case INTRO_CUTSCENE_LAND_OUTSIDE_PIPE:  intro_cutscene_land_outside_pipe(m);  break;
        case INTRO_CUTSCENE_LOWER_PIPE:         intro_cutscene_lower_pipe(m);         break;
        case INTRO_CUTSCENE_SET_MARIO_TO_IDLE:  intro_cutscene_set_mario_to_idle(m);  break;
    }
    return FALSE;
}

// jumbo star cutscene: mario lands after grabbing the jumbo star
static void jumbo_star_cutscene_falling(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        m->input |= INPUT_A_DOWN;
        m->flags |= (MARIO_WING_CAP | MARIO_CAP_ON_HEAD);

        m->faceAngle[1] = -0x8000;
        m->pos[0] = 0.0f;
        m->pos[2] = 0.0f;

        mario_set_forward_vel(m, 0.0f);
        set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);

        if (perform_air_step(m, 1) == AIR_STEP_LANDED)
        {
            func_802492A0((15 << 8) | 31);
            func_80251280(m, SOUND_TERRAIN_2);
            m->actionState++;
        }
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
        if (func_80250770(m))
        {
            m->unk94->unk1C[1] = 10;
            advance_cutscene_step(m);
        }
    }
}

// jumbo star cutscene: mario takes off
static s32 jumbo_star_cutscene_taking_off(struct MarioState *m)
{
    struct Object *marioObj = m->marioObj;
    s32 animFrame;

    if (m->actionState == 0)
    {
        set_mario_animation(m, MARIO_ANIM_FINAL_BOWSER_RAISE_HAND_SPIN);
        marioObj->rawData.asF32[0x22] = 0.0f;

        if (func_802507AC(m))
        {
            func_80251280(m, SOUND_TERRAIN_2);
            m->actionState++;
        }
    }
    else
    {
        animFrame = set_mario_animation(m, MARIO_ANIM_FINAL_BOWSER_WING_CAP_TAKE_OFF);
        if (animFrame == 3 || animFrame == 28 || animFrame == 60)
            func_802510E4(m, SOUND_TERRAIN_1, 1);
        if (animFrame >= 3)
            marioObj->rawData.asF32[0x22] -= 32.0f;

        switch (animFrame)
        {
            case 3:
                SetSound(
                    SOUND_MARIO_YAH + (D_80226EB8 % 3 << 16),
                    m->marioObj->header.gfx.cameraToObject
                );
                break;

            case 28:
                SetSound(SOUND_MARIO_HOOHOO, m->marioObj->header.gfx.cameraToObject);
                break;

            case 60:
                SetSound(SOUND_MARIO_YAHOO, m->marioObj->header.gfx.cameraToObject);
                break;
        }
        m->particleFlags |= PARTICLE_SPARKLES;

        if (func_802507AC(m))
            advance_cutscene_step(m);
    }

    vec3f_set(m->pos, 0.0f, 307.0, marioObj->rawData.asF32[0x22]);
    func_80250E50(m);
    vec3f_copy(marioObj->header.gfx.pos, m->pos);
    vec3s_set(marioObj->header.gfx.angle, 0, m->faceAngle[1], 0);

    // not sure why they did this, probably was from being used to action
    // functions
    return FALSE;
}

// jumbo star cutscene: mario flying
static s32 jumbo_star_cutscene_flying(struct MarioState *m)
{
    Vec3f targetPos;
    UNUSED struct Object *marioObj = m->marioObj;
    f32 targetDX;
    f32 targetDY;
    f32 targetDZ;
    f32 targetHyp;
    s16 targetAngle;

    switch (m->actionState)
    {
        case 0:
            set_mario_animation(m, MARIO_ANIM_WING_CAP_FLY);
            func_8037AFB8(sJumboStarKeyframes);
            m->actionState++;
            // fall through
        case 1:
            if (func_8037AFE8(targetPos))
            {
                // lol does this twice
                set_mario_action(m, ACT_FREEFALL, 0);
                m->actionState++;
            }
            else
            {
                targetDX = targetPos[0] - m->pos[0];
                targetDY = targetPos[1] - m->pos[1];
                targetDZ = targetPos[2] - m->pos[2];
                targetHyp = sqrtf(targetDX * targetDX + targetDZ * targetDZ);
                targetAngle = atan2s(targetDZ, targetDX);

                vec3f_copy(m->pos, targetPos);
                m->marioObj->header.gfx.angle[0] = -atan2s(targetHyp, targetDY);
                m->marioObj->header.gfx.angle[1] = targetAngle;
                m->marioObj->header.gfx.angle[2] = \
                    ((m->faceAngle[1] - targetAngle) << 16 >> 16) * 20;
                m->faceAngle[1] = targetAngle;
            }
            break;
        case 2:
            set_mario_action(m, ACT_FREEFALL, 0);
            break;
    }

    m->unk98->handState = MARIO_HAND_RIGHT_OPEN;
    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    m->particleFlags |= PARTICLE_SPARKLES;

    if (m->actionTimer++ == 500)
        level_trigger_warp(m, WARP_OP_CREDITS_START);

    return FALSE;
}

enum
{
    JUMBO_STAR_CUTSCENE_FALLING,
    JUMBO_STAR_CUTSCENE_TAKING_OFF,
    JUMBO_STAR_CUTSCENE_FLYING
};

static s32 act_jumbo_star_cutscene(struct MarioState *m)
{
    switch (m->actionArg)
    {
        case JUMBO_STAR_CUTSCENE_FALLING:    jumbo_star_cutscene_falling(m);    break;
        case JUMBO_STAR_CUTSCENE_TAKING_OFF: jumbo_star_cutscene_taking_off(m); break;
        case JUMBO_STAR_CUTSCENE_FLYING:     jumbo_star_cutscene_flying(m);     break;
    }
    return FALSE;
}

static void generate_yellow_sparkles(s16 x, s16 y, s16 z, f32 radius)
{
    s16 offsetX = radius * coss(sSparkleGenTheta) * sins(sSparkleGenPhi);
    s16 offsetY = radius * sins(sSparkleGenTheta);
    s16 offsetZ = radius * coss(sSparkleGenTheta) * coss(sSparkleGenPhi);

    spawn_object_abs_with_rot(
        gCurrentObject, 0, MODEL_NONE, bhvPowerupSparkles2,
        x + offsetX, y + offsetY, z + offsetZ,
        0, 0, 0
    );

    //! copy paste error
    offsetX = offsetX * 4 / 3;
    offsetX = offsetY * 4 / 3;
    offsetX = offsetZ * 4 / 3;

    spawn_object_abs_with_rot(
        gCurrentObject, 0, MODEL_NONE, bhvPowerupSparkles2,
        x - offsetX, y - offsetY, z - offsetZ,
        0, 0, 0
    );

    sSparkleGenTheta += 0x3800;
    sSparkleGenPhi   += 0x6000;
}

// not sure what this does, returns the height of the floor, but idk about the
// other stuff (animation related?)
static f32 func_8025BC14(struct Object *o)
{
    struct Surface *surf;
    Vec3s sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;

    func_80250C84(o, o->header.gfx.angle[1], sp24);

    sp20 = o->header.gfx.pos[0] + sp24[0];
    sp1C = o->header.gfx.pos[1] + 10.0f;
    sp18 = o->header.gfx.pos[2] + sp24[2];

    return find_floor(sp20, sp1C, sp18, &surf);
}

// make mario fall and soften wing cap gravity
static void end_peach_cutscene_mario_falling(struct MarioState *m)
{
    if (m->actionTimer == 1)
        m->unk94->unk1C[1] = 11;

    m->input |= INPUT_A_DOWN;
    m->flags |= (MARIO_WING_CAP | MARIO_CAP_ON_HEAD);

    set_mario_animation(m, MARIO_ANIM_GENERAL_FALL);
    mario_set_forward_vel(m, 0.0f);

    if (perform_air_step(m, 0) == AIR_STEP_LANDED)
    {
        func_80251280(m, SOUND_TERRAIN_2);
        advance_cutscene_step(m);
    }
}

// set mario on the ground, wait and spawn the jumbo star outside the castle.
static void end_peach_cutscene_mario_landing(struct MarioState *m)
{
    set_mario_animation(m, MARIO_ANIM_GENERAL_LAND);
    stop_and_set_height_to_floor(m);

    if (func_80250770(m))
    {
        // make wing cap run out
        m->capTimer = 60;

        sEndJumboStarObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_STAR, bhvStaticObject,
            0, 2528, -1800,
            0, 0, 0
        );
        scale_object(sEndJumboStarObj, 3.0);
        advance_cutscene_step(m);
    }
}

// raise hand animation, lower hand animation, do some special effects
static void end_peach_cutscene_summon_jumbo_star(struct MarioState *m)
{
    set_mario_animation(
        m, m->actionState == 0 ? \
        MARIO_ANIM_CREDITS_RAISE_HAND : MARIO_ANIM_CREDITS_LOWER_HAND
    );

    if (m->actionState == 0 && func_802507AC(m))
        m->actionState++;
    if (m->actionTimer == 90)
        func_802492A0((0 << 8) | 32);
    if (m->actionTimer == 255)
        advance_cutscene_step(m);

    sEndJumboStarObj->oFaceAngleYaw += 0x0400;
    generate_yellow_sparkles(0, 2528, -1800, 250.0f);
    SetSound(SOUND_CH6_UNKNOWN00B, sEndJumboStarObj->header.gfx.cameraToObject);
}

// free peach from the stained glass window
static void end_peach_cutscene_spawn_peach(struct MarioState *m)
{
    if (m->actionTimer == 1)
    {
        play_transition(WARP_TRANSITION_FADE_INTO_COLOR, 14, 255, 255, 255);
    }
    if (m->actionTimer == 2)
    {
        SetSound(SOUND_MENU_STARSOUND, D_803320E0);
    }
    if (m->actionTimer == 44)
    {
        play_transition(WARP_TRANSITION_FADE_FROM_COLOR, 192, 255, 255, 255);
    }
    if (m->actionTimer == 40)
    {
        mark_object_for_deletion(sEndJumboStarObj);

        sEndPeachObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_PEACH, bhvEndPeach,
            0, 2428, -1300, 0, 0, 0
        );
        gCutsceneFocus = sEndPeachObj;

        sEndRightToadObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_TOAD, bhvEndToad,
            200, 906, -1290, 0, 0, 0
        );

        sEndLeftToadObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_TOAD, bhvEndToad,
            -200, 906, -1290, 0, 0, 0
        );

        sEndPeachObj->oOpacity     = 127;
        sEndRightToadObj->oOpacity = 255;
        sEndLeftToadObj->oOpacity  = 255;

        D_8032CBE4 = 4;
        sEndPeachAnimation = 4;

        sEndToadAnims[0] = 4;
        sEndToadAnims[1] = 5;
    }
    if (m->actionTimer >= 276)
    {
        sEndPeachObj->oOpacity = camera_approach_f32_symmetric(
            sEndPeachObj->oOpacity, 255.0f, 2.0f
        );
    }
    if (m->actionTimer >= 40)
    {
        generate_yellow_sparkles(0, 2628, -1300, 150.0f);
    }
    if (m->actionTimer == 355)
    {
        advance_cutscene_step(m);
    }
    // probably added sounds later and missed the previous >= 40 check
    if (m->actionTimer >= 40)
    {
        SetSound(SOUND_CH6_UNKNOWN00B, sEndPeachObj->header.gfx.cameraToObject);
    }
}

// descend peach
static void end_peach_cutscene_descend_peach(struct MarioState *m)
{
    generate_yellow_sparkles(0, sEndPeachObj->oPosY, -1300, 150.0f);

    if (sEndPeachObj->oPosY >= 1300.0f)
    {
        if (m->actionState < 60)
            m->actionState += 5;
    }
    else
    {
        if (m->actionState >= 27)
            m->actionState -= 2;
        set_mario_animation(m, MARIO_ANIM_CREDITS_RETURN_FROM_LOOK_UP);
    }

    if ((sEndPeachObj->oPosY -= m->actionState / 10) <= 907.0f)
        sEndPeachObj->oPosY = 906.0f;

    SetSound(SOUND_CH6_UNKNOWN00B, sEndPeachObj->header.gfx.cameraToObject);

    if (m->actionTimer >= 584)
        advance_cutscene_step(m);
}

// mario runs to peach
static void end_peach_cutscene_run_to_peach(struct MarioState *m)
{
    struct Surface *surf;

    if (m->actionTimer == 22)
        sEndPeachAnimation = 5;

    if ((m->pos[2] -= 20.0f) <= -1181.0f)
    {
        m->pos[2] = -1180.0f;
        advance_cutscene_step(m);
    }

    m->pos[1] = find_floor(m->pos[0], m->pos[1], m->pos[2], &surf);

    func_80250934(m, MARIO_ANIM_RUNNING, 0x00080000);
    func_80263AD4(m, 9, 45);

    vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
    m->particleFlags |= PARTICLE_DUST;
}

// dialogue 1
// "Mario!"
// "The power of the Stars is restored to the castle..."
static void end_peach_cutscene_dialogue_1(struct MarioState *m)
{
    s32 animFrame = set_mario_animation(
        m, m->actionState == 0 ? \
        MARIO_ANIM_CREDITS_TAKE_OFF_CAP : MARIO_ANIM_CREDITS_LOOK_UP
    );

    if (m->actionState == 0)
    {
        if (animFrame == 8)
            cutscene_take_cap_off(m);

        if (func_80250770(m))
            m->actionState++;
    }

    switch (m->actionTimer)
    {
        case 80:
            sEndPeachAnimation = 6;
            break;

        case 81:
            D_8032CBE4 = 3;
            break;

        case 145:
            D_8032CBE4 = 2;
            break;

        case 228:
            D_8032CBE4 = 1;
            D_8032CBE8 = 1;
            break;

        case 230:
            set_cutscene_message(160, 227, 0, 30);
        #ifndef VERSION_JP
            func_8031FFB4(0, 60, 40);
            SetSound(SOUND_PEACH_MARIO, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 275:
            D_8032CBE4 = 0;
            D_8032CBE8 = 0;
            break;

        case 290:
            set_cutscene_message(160, 227, 1, 60);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_POWEROFTHESTARS, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 480:
            advance_cutscene_step(m);
            break;
    }
}

// dialogue 2
// "...and it's all thanks to you!"
// "Thank you Mario!"
// "We have to do something special for you..."
static void end_peach_cutscene_dialogue_2(struct MarioState *m)
{
    sEndPeachAnimation = 9;

    switch (m->actionTimer)
    {
        case 29:
            set_cutscene_message(160, 227, 2, 30);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_THANKSTOYOU, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 45:
            D_8032CBE8 = 1;
            break;

        case 75:
            set_cutscene_message(160, 227, 3, 30);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_THANKYOUMARIO, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 130:
            set_cutscene_message(160, 227, 4, 40);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_SOMETHINGSPECIAL, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 200:
            advance_cutscene_step(m);
            break;
    }
}

static void end_peach_cutscene_kiss_from_peach(struct MarioState *m)
{
    sEndPeachAnimation = 10;

    if (m->actionTimer >= 90)
        m->unk98->eyeState = m->actionTimer < 110 ? \
            sMarioBlinkOverride[m->actionTimer - 90] : MARIO_EYES_HALF_CLOSED;

    switch (m->actionTimer)
    {
        case 8:
            D_8032CBE8 = 0;
            break;

        case 10:
            D_8032CBE4 = 3;
            break;

        case 50:
            D_8032CBE4 = 4;
            break;

        case 75:
            m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;
            break;

        case 76:
            m->unk98->eyeState = MARIO_EYES_CLOSED;
            break;

        case 100:
            D_8032CBE4 = 3;
            break;

        case 136:
            D_8032CBE4 = 0;
            break;

        case 140:
            advance_cutscene_step(m);
            break;
    }
}

static void end_peach_cutscene_star_dance(struct MarioState *m)
{
    s32 animFrame = set_mario_animation(m, MARIO_ANIM_CREDITS_PEACE_SIGN);

    if (animFrame == 77)
        cutscene_put_cap_on(m);
    if (animFrame == 88)
        SetSound(SOUND_MARIO_HEREWEGO, m->marioObj->header.gfx.cameraToObject);
    if (animFrame >= 98)
        m->unk98->handState = MARIO_HAND_PEACE_SIGN;

    if (m->actionTimer < 52)
        m->unk98->eyeState = MARIO_EYES_HALF_CLOSED;

    switch (m->actionTimer)
    {
        case 70:
            D_8032CBE4 = 1;
            break;

        case 86:
            D_8032CBE4 = 2;
            break;

        case 90:
            D_8032CBE4 = 3;
            break;

        case 120:
            D_8032CBE4 = 0;
            break;

        case 140:
        #ifndef VERSION_JP
            func_80320040(0, 60);
        #endif
            func_802492A0((15 << 8) | 26);
            break;

        case 142:
            advance_cutscene_step(m);
            break;
    }
}

// dialogue 3
// "Listen everybody"
// "let's bake a delicious cake..."
// "...for Mario..."
static void end_peach_cutscene_dialogue_3(struct MarioState *m)
{
    set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);

    sEndPeachObj->oPosY     = func_8025BC14(sEndPeachObj);
    sEndRightToadObj->oPosY = func_8025BC14(sEndRightToadObj);
    sEndLeftToadObj->oPosY  = func_8025BC14(sEndLeftToadObj);

    switch (m->actionTimer)
    {
        case 1:
            sEndPeachAnimation = 0;
            sEndToadAnims[0] = 0;
            sEndToadAnims[1] = 2;
            D_8032CBE8 = 1;
            set_cutscene_message(160, 227, 5, 30);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_BAKEACAKE, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;

        case 55:
            set_cutscene_message(160, 227, 6, 40);
            break;

        case 130:
            set_cutscene_message(160, 227, 7, 50);
        #ifndef VERSION_JP
            SetSound(SOUND_PEACH_FORMARIO, sEndPeachObj->header.gfx.cameraToObject);
        #endif
            break;
    }

    if (m->actionTimer == 350)
        advance_cutscene_step(m);
}

// "Mario!"
static void end_peach_cutscene_run_to_castle(struct MarioState *m)
{
    set_mario_animation(
        m, m->actionState == 0 ? \
        MARIO_ANIM_CREDITS_START_WALK_LOOK_UP : MARIO_ANIM_CREDITS_LOOK_BACK_THEN_RUN
    );

    m->marioObj->header.gfx.pos[1] = func_8025BC14(m->marioObj);

    if (m->actionState == 0 && func_802507AC(m))
        m->actionState = 1;

    if (m->actionTimer == 95)
    {
        set_cutscene_message(160, 227, 0, 40);
    #ifndef VERSION_JP
        SetSound(SOUND_PEACH_MARIO2, sEndPeachObj->header.gfx.cameraToObject);
    #endif
    }
    if (m->actionTimer == 389)
    {
        advance_cutscene_step(m);
    }
}

static void end_peach_cutscene_fade_out(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        level_trigger_warp(m, WARP_OP_CREDITS_NEXT);
        D_8032FFDC = 1500.0f;
        m->actionState = 1;
    }
}

enum
{
    END_PEACH_CUTSCENE_MARIO_FALLING,
    END_PEACH_CUTSCENE_MARIO_LANDING,
    END_PEACH_CUTSCENE_SUMMON_JUMBO_STAR,
    END_PEACH_CUTSCENE_SPAWN_PEACH,
    END_PEACH_CUTSCENE_DESCEND_PEACH,
    END_PEACH_CUTSCENE_RUN_TO_PEACH,
    END_PEACH_CUTSCENE_DIALOGUE_1,
    END_PEACH_CUTSCENE_DIALOGUE_2,
    END_PEACH_CUTSCENE_KISS_FROM_PEACH,
    END_PEACH_CUTSCENE_STAR_DANCE,
    END_PEACH_CUTSCENE_DIALOGUE_3,
    END_PEACH_CUTSCENE_RUN_TO_CASTLE,
    END_PEACH_CUTSCENE_FADE_OUT
};

static s32 act_end_peach_cutscene(struct MarioState *m)
{
    switch (m->actionArg)
    {
        case END_PEACH_CUTSCENE_MARIO_FALLING:     end_peach_cutscene_mario_falling(m);     break;
        case END_PEACH_CUTSCENE_MARIO_LANDING:     end_peach_cutscene_mario_landing(m);     break;
        case END_PEACH_CUTSCENE_SUMMON_JUMBO_STAR: end_peach_cutscene_summon_jumbo_star(m); break;
        case END_PEACH_CUTSCENE_SPAWN_PEACH:       end_peach_cutscene_spawn_peach(m);       break;
        case END_PEACH_CUTSCENE_DESCEND_PEACH:     end_peach_cutscene_descend_peach(m);     break;
        case END_PEACH_CUTSCENE_RUN_TO_PEACH:      end_peach_cutscene_run_to_peach(m);      break;
        case END_PEACH_CUTSCENE_DIALOGUE_1:        end_peach_cutscene_dialogue_1(m);        break;
        case END_PEACH_CUTSCENE_DIALOGUE_2:        end_peach_cutscene_dialogue_2(m);        break;
        case END_PEACH_CUTSCENE_KISS_FROM_PEACH:   end_peach_cutscene_kiss_from_peach(m);   break;
        case END_PEACH_CUTSCENE_STAR_DANCE:        end_peach_cutscene_star_dance(m);        break;
        case END_PEACH_CUTSCENE_DIALOGUE_3:        end_peach_cutscene_dialogue_3(m);        break;
        case END_PEACH_CUTSCENE_RUN_TO_CASTLE:     end_peach_cutscene_run_to_castle(m);     break;
        case END_PEACH_CUTSCENE_FADE_OUT:          end_peach_cutscene_fade_out(m);          break;
    }

    m->actionTimer++;

    sEndCutsceneVp.vp.vscale[0] = 640;
    sEndCutsceneVp.vp.vscale[1] = 360;
    sEndCutsceneVp.vp.vtrans[0] = 640;
    sEndCutsceneVp.vp.vtrans[1] = 480;
    func_8027A220(NULL, &sEndCutsceneVp, 0, 0, 0);

    return FALSE;
}

static s32 act_credits_cutscene(struct MarioState *m)
{
    s32 width;
    s32 height;

    m->unk94->unk1C[1] = 13;
    // checks if mario is underwater (JRB, DDD, SA, etc.)
    if (m->pos[1] < m->waterLevel - 100)
    {
        if (m->area->camera->currPreset != CAMERA_PRESET_BEHIND_MARIO)
            func_80285BD8(m->area->camera, 3, 1);
        set_mario_animation(m, MARIO_ANIM_WATER_IDLE);
        vec3f_copy(m->marioObj->header.gfx.pos, m->pos);
        // will copy over roll and pitch, if set
        vec3s_copy(m->marioObj->header.gfx.angle, m->faceAngle);
        m->particleFlags |= PARTICLE_5;
    }
    else
    {
        set_mario_animation(m, MARIO_ANIM_FIRST_PERSON);
        if (m->actionTimer > 0)
            stop_and_set_height_to_floor(m);
    }

    if (m->actionTimer >= 61)
    {
        if (m->actionState < 40)
            m->actionState += 2;

        width  = m->actionState * 640 / 100;
        height = m->actionState * 480 / 100;

        sEndCutsceneVp.vp.vscale[0] = 640 - width;
        sEndCutsceneVp.vp.vscale[1] = 480 - height;
        sEndCutsceneVp.vp.vtrans[0] = \
            (gCurrCreditsEntry->unk02 & 0x10 ? width  : -width) \
            * 56 / 100 + 640;
        sEndCutsceneVp.vp.vtrans[1] = \
            (gCurrCreditsEntry->unk02 & 0x20 ? height : -height) \
            * 66 / 100 + 480;

        func_8027A220(&sEndCutsceneVp, 0, 0, 0, 0);
    }

    if (m->actionTimer == 90)
        reset_cutscene_msg_fade();

    if (m->actionTimer >= 90)
        sDispCreditsEntry = gCurrCreditsEntry;

    if (m->actionTimer++ == 200)
        level_trigger_warp(m, 24);

    m->marioObj->header.gfx.angle[1] += (gCurrCreditsEntry->unk02 & 0xC0) << 8;

    return FALSE;
}

static s32 act_end_waving_cutscene(struct MarioState *m)
{
    if (m->actionState == 0)
    {
        m->unk94->unk1C[1] = 12;

        sEndPeachObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_PEACH, bhvEndPeach,
            60, 906, -1180, 0, 0, 0
        );

        sEndRightToadObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_TOAD, bhvEndToad,
            180, 906, -1170, 0, 0, 0
        );

        sEndLeftToadObj = spawn_object_abs_with_rot(
            gCurrentObject, 0, MODEL_TOAD, bhvEndToad,
            -180, 906, -1170, 0, 0, 0
        );

        sEndPeachObj->oOpacity     = 255;
        sEndRightToadObj->oOpacity = 255;
        sEndLeftToadObj->oOpacity  = 255;

        sEndPeachAnimation = 11;
        sEndToadAnims[0] = 6;
        sEndToadAnims[1] = 7;

        m->actionState = 1;
    }

    set_mario_animation(m, MARIO_ANIM_CREDITS_WAVING);
    stop_and_set_height_to_floor(m);

    m->marioObj->header.gfx.angle[1] += 0x8000;
    m->marioObj->header.gfx.pos[0] -= 60.0f;
    m->unk98->handState = MARIO_HAND_RIGHT_OPEN;

    if (m->actionTimer++ == 300)
        level_trigger_warp(m, WARP_OP_CREDITS_END);

    return FALSE;
}

static s32 check_for_instant_quicksand(struct MarioState *m)
{
    if (m->floor->type == SURFACE_INSTANT_QUICKSAND && \
        m->action & ACT_FLAG_INVULNERABLE && \
        m->action != ACT_QUICKSAND_DEATH)
    {
        func_80251F74(m);
        return drop_and_set_mario_action(m, ACT_QUICKSAND_DEATH, 0);
    }
    return FALSE;
}

s32 mario_execute_cutscene_action(struct MarioState *m)
{
    s32 cancel;

    if (check_for_instant_quicksand(m))
        return TRUE;

    switch (m->action)
    {
        case ACT_DISAPPEARED:                cancel = act_disappeared(m);                break;
        case ACT_INTRO_CUTSCENE:             cancel = act_intro_cutscene(m);             break;
        case ACT_STAR_DANCE_EXIT:            cancel = act_star_dance(m);                 break;
        case ACT_STAR_DANCE_NO_EXIT:         cancel = act_star_dance(m);                 break;
        case ACT_STAR_DANCE_WATER:           cancel = act_star_dance_water(m);           break;
        case ACT_FALL_AFTER_STAR_GRAB:       cancel = act_fall_after_star_grab(m);       break;
        case ACT_READING_AUTOMATIC_DIALOGUE: cancel = act_reading_automatic_dialogue(m); break;
        case ACT_READING_NPC_DIALOGUE:       cancel = act_reading_npc_dialogue(m);       break;
        case ACT_DEBUG_FREE_MOVE:            cancel = act_debug_free_move(m);            break;
        case ACT_READING_SIGN:               cancel = act_reading_sign(m);               break;
        case ACT_JUMBO_STAR_CUTSCENE:        cancel = act_jumbo_star_cutscene(m);        break;
        case ACT_WAITING_FOR_DIALOGUE:       cancel = act_waiting_for_dialogue(m);       break;
        case ACT_STANDING_DEATH:             cancel = act_standing_death(m);             break;
        case ACT_QUICKSAND_DEATH:            cancel = act_quicksand_death(m);            break;
        case ACT_ELECTROCUTION:              cancel = act_electrocution(m);              break;
        case ACT_SUFFOCATION:                cancel = act_suffocation(m);                break;
        case ACT_DEATH_ON_STOMACH:           cancel = act_death_on_stomach(m);           break;
        case ACT_DEATH_ON_BACK:              cancel = act_death_on_back(m);              break;
        case ACT_EATEN_BY_BUBBA:             cancel = act_eaten_by_bubba(m);             break;
        case ACT_END_PEACH_CUTSCENE:         cancel = act_end_peach_cutscene(m);         break;
        case ACT_CREDITS_CUTSCENE:           cancel = act_credits_cutscene(m);           break;
        case ACT_END_WAVING_CUTSCENE:        cancel = act_end_waving_cutscene(m);        break;
        case ACT_PULLING_DOOR:
        case ACT_PUSHING_DOOR:               cancel = act_going_through_door(m);         break;
        case ACT_WARP_DOOR_SPAWN:            cancel = act_warp_door_spawn(m);            break;
        case ACT_EMERGE_FROM_PIPE:           cancel = act_emerge_from_pipe(m);           break;
        case ACT_SPAWN_SPIN_AIRBORNE:        cancel = act_spawn_spin_airborne(m);        break;
        case ACT_SPAWN_SPIN_LANDING:         cancel = act_spawn_spin_landing(m);         break;
        case ACT_EXIT_AIRBORNE:              cancel = act_exit_airborne(m);              break;
        case ACT_EXIT_LAND_SAVE_DIALOGUE:    cancel = act_exit_land_save_dialogue(m);    break;
        case ACT_DEATH_EXIT:                 cancel = act_death_exit(m);                 break;
        case ACT_UNUSED_DEATH_EXIT:          cancel = act_unused_death_exit(m);          break;
        case ACT_FALLING_DEATH_EXIT:         cancel = act_falling_death_exit(m);         break;
        case ACT_SPECIAL_EXIT_AIRBORNE:      cancel = act_special_exit_airborne(m);      break;
        case ACT_SPECIAL_DEATH_EXIT:         cancel = act_special_death_exit(m);         break;
        case ACT_FALLING_EXIT_AIRBORNE:      cancel = act_falling_exit_airborne(m);      break;
        case ACT_UNLOCKING_KEY_DOOR:         cancel = act_unlocking_key_door(m);         break;
        case ACT_UNLOCKING_STAR_DOOR:        cancel = act_unlocking_star_door(m);        break;
        case ACT_ENTERING_STAR_DOOR:         cancel = act_entering_star_door(m);         break;
        case ACT_SPAWN_NO_SPIN_AIRBORNE:     cancel = act_spawn_no_spin_airborne(m);     break;
        case ACT_SPAWN_NO_SPIN_LANDING:      cancel = act_spawn_no_spin_landing(m);      break;
        case ACT_BBH_ENTER_JUMP:             cancel = act_bbh_enter_jump(m);             break;
        case ACT_BBH_ENTER_SPIN:             cancel = act_bbh_enter_spin(m);             break;
        case ACT_TELEPORT_FADE_OUT:          cancel = act_teleport_fade_out(m);          break;
        case ACT_TELEPORT_FADE_IN:           cancel = act_teleport_fade_in(m);           break;
        case ACT_SHOCKED:                    cancel = act_shocked(m);                    break;
        case ACT_SQUISHED:                   cancel = act_squished(m);                   break;
        case ACT_HEAD_STUCK_IN_GROUND:       cancel = act_head_stuck_in_ground(m);       break;
        case ACT_BUTT_STUCK_IN_GROUND:       cancel = act_butt_stuck_in_ground(m);       break;
        case ACT_FEET_STUCK_IN_GROUND:       cancel = act_feet_stuck_in_ground(m);       break;
        case ACT_PUTTING_ON_CAP:             cancel = act_putting_on_cap(m);             break;
    }

    if (!cancel)
    {
        if (m->input & INPUT_IN_WATER)
            m->particleFlags |= PARTICLE_7;
    }

    return cancel;
}
