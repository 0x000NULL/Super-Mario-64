#include <ultra64.h>

#include "sm64.h"
#include "area.h"
#include "audio/interface_2.h"
#include "camera.h"
#include "mario_misc.h"
#include "behavior_actions.h"
#include "behavior_data.h"
#include "engine/behavior_script.h"
#include "game.h"
#include "engine/graph_node.h"
#include "envfx_snow.h"
#include "level_update.h"
#include "engine/math_util.h"
#include "memory.h"
#include "object_helpers.h"
#include "object_helpers2.h"
#include "goddard/mario_head_6.h"
#include "rendering_graph_node.h"
#include "save_file.h"
#include "sound_init.h"
#include "skybox.h"
#include "interaction.h"
#include "object_list_processor.h"

static s8 D_8032CDF0[7] = {0x01, 0x02, 0x01, 0x00, 0x01, 0x02, 0x01};
static s8 D_8032CDF8[] = {0x0a, 0x0c, 0x10, 0x18, 0x0a, 0x0a, 0x0a, 0x0e, 0x14, 0x1e, 0x0a, 0x0a, 0x0a, 0x10, 0x14, 0x1a, 0x1a, 0x14, 0x00, 0x00};
static s16 D_8032CE0C = 0;

struct GraphNodeObject D_80339FE0;
struct MarioBodyState D_8033A040[2]; // 2nd is never accessed in practice, most likely Luigi related

// This whole file is weirdly organized. It has to be the same file due
// to rodata boundries and function aligns, which means the programmer
// treated this like a "misc" file for vaguely mario related things
// (message NPC related things, the mario head geo, and mario geo
// functions)


// mario head geo
Gfx *Geo18_802764B0(s32 run, struct GraphNode *node, Mat4 *c)
{
    Gfx *sp24 = NULL;
    s16 sp22 = 0;
    struct GraphNode12A *sp1C = (struct GraphNode12A *)node;
    UNUSED Mat4 *sp18 = c;

    if (run == TRUE)
    {
        if (gPlayer1Controller->controllerData != NULL && gWarpTransition.isActive == 0)
            gd_copy_p1_contpad(gPlayer1Controller->controllerData);
        sp24 = (Gfx *)PHYSICAL_TO_VIRTUAL(gdm_gettestdl(sp1C->unk18));
        D_8032C6A0 = gd_vblank;
        sp22 = gd_sfx_to_play();
        func_80248E24(sp22);
    }
    return sp24;
}

static void bhvToadMessage_faded(void)
{
    if (gCurrentObject->oDistanceToMario > 700.0f)
        gCurrentObject->oToadMessageRecentlyTalked = 0;
    if (gCurrentObject->oToadMessageRecentlyTalked == 0 && gCurrentObject->oDistanceToMario < 600.0f)
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_OPACIFYING;
}

static void bhvToadMessage_opaque(void)
{
    if (gCurrentObject->oDistanceToMario > 700.0f)
    {
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADING;
    }
    else
    {
        if (gCurrentObject->oToadMessageRecentlyTalked == 0)
        {
            gCurrentObject->oUnk190 = (1 << 14);
            if (gCurrentObject->oInteractStatus & INT_STATUS_INTERACTED)
            {
                gCurrentObject->oInteractStatus = 0;
                gCurrentObject->oToadMessageState = TOAD_MESSAGE_TALKING;
                play_toads_jingle();
            }
        }
    }
}

static void bhvToadMessage_talking(void)
{
    if (obj_update_dialogue_unk2(3, 1, 162, gCurrentObject->oToadMessageDialogNum) != 0)
    {
        gCurrentObject->oToadMessageRecentlyTalked = 1;
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADING;
        switch (gCurrentObject->oToadMessageDialogNum)
        {
        case TOAD_STAR_1_DIALOG:
            gCurrentObject->oToadMessageDialogNum = TOAD_STAR_1_DIALOG_AFTER;
            bhv_spawn_star_objects(0);
            break;
        case TOAD_STAR_2_DIALOG:
            gCurrentObject->oToadMessageDialogNum = TOAD_STAR_2_DIALOG_AFTER;
            bhv_spawn_star_objects(1);
            break;
        case TOAD_STAR_3_DIALOG:
            gCurrentObject->oToadMessageDialogNum = TOAD_STAR_3_DIALOG_AFTER;
            bhv_spawn_star_objects(2);
            break;
        }
    }
}

static void bhvToadMessage_opacifying(void)
{
    if ((gCurrentObject->oOpacity += 6) == 255)
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_OPAQUE;
}

static void bhvToadMessage_fading(void)
{
    if ((gCurrentObject->oOpacity -= 6) == 81)
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADED;
}

void bhvToadMessage_loop(void)
{
    if (gCurrentObject->header.gfx.node.flags & 1)
    {
        gCurrentObject->oUnk190 = 0;
        switch (gCurrentObject->oToadMessageState)
        {
        case TOAD_MESSAGE_FADED:
            bhvToadMessage_faded();
            break;
        case TOAD_MESSAGE_OPAQUE:
            bhvToadMessage_opaque();
            break;
        case TOAD_MESSAGE_OPACIFYING:
            bhvToadMessage_opacifying();
            break;
        case TOAD_MESSAGE_FADING:
            bhvToadMessage_fading();
            break;
        case TOAD_MESSAGE_TALKING:
            bhvToadMessage_talking();
            break;
        }
    }
}

void bhvToadMessage_init(void)
{
    s32 saveFlags = save_file_get_flags();
    s32 starCount = save_file_get_total_star_count(gCurrSaveFileNum - 1, 0, 24);
    s32 dialogNum = (gCurrentObject->oBehParams >> 24) & 0xFF;
    s32 enoughStars = TRUE;

    switch (dialogNum)
    {
    case TOAD_STAR_1_DIALOG:
        enoughStars = (starCount >= TOAD_STAR_1_REQUIREMENT);
        if (saveFlags & (1 << 24))
            dialogNum = TOAD_STAR_1_DIALOG_AFTER;
        break;
    case TOAD_STAR_2_DIALOG:
        enoughStars = (starCount >= TOAD_STAR_2_REQUIREMENT);
        if (saveFlags & (1 << 25))
            dialogNum = TOAD_STAR_2_DIALOG_AFTER;
        break;
    case TOAD_STAR_3_DIALOG:
        enoughStars = (starCount >= TOAD_STAR_3_REQUIREMENT);
        if (saveFlags & (1 << 26))
            dialogNum = TOAD_STAR_3_DIALOG_AFTER;
        break;
    }
    if (enoughStars)
    {
        gCurrentObject->oToadMessageDialogNum = dialogNum;
        gCurrentObject->oToadMessageRecentlyTalked = 0;
        gCurrentObject->oToadMessageState = TOAD_MESSAGE_FADED;
        gCurrentObject->oOpacity = 81;
    }
    else
    {
        mark_object_for_deletion(gCurrentObject);
    }
}

static void bhvUnlockDoorStar_spawn_particle(s16 angleOffset)
{
    struct Object *sparkleParticle = spawn_object(gCurrentObject, 0, bhvPowerupSparkles2);

    sparkleParticle->oPosX += 100.0f * sins((gCurrentObject->oUnlockDoorStarTimer * 0x2800) + angleOffset);
    sparkleParticle->oPosZ += 100.0f * coss((gCurrentObject->oUnlockDoorStarTimer * 0x2800) + angleOffset);
    // Particles are spawned lower each frame
    sparkleParticle->oPosY -= gCurrentObject->oUnlockDoorStarTimer * 10.0f;
}

void bhvUnlockDoorStar_init(void)
{
    gCurrentObject->oUnlockDoorStarState = UNLOCK_DOOR_STAR_RISING;
    gCurrentObject->oUnlockDoorStarTimer = 0;
    gCurrentObject->oUnlockDoorStarYawVel = 0x1000;
    gCurrentObject->oPosX += 30.0f * sins(gMarioState->faceAngle[1] - 0x4000);
    gCurrentObject->oPosY += 160.0f;
    gCurrentObject->oPosZ += 30.0f * coss(gMarioState->faceAngle[1] - 0x4000);
    gCurrentObject->oMoveAngleYaw = 0x7800;
    scale_object(gCurrentObject, 0.5f);
}

void bhvUnlockDoorStar_loop(void)
{
    UNUSED u8 unused1[4];
    s16 prevYaw = gCurrentObject->oMoveAngleYaw;
    UNUSED u8 unused2[4];

    // Speed up the star every frame
    if (gCurrentObject->oUnlockDoorStarYawVel < 0x2400)
        gCurrentObject->oUnlockDoorStarYawVel += 0x60;
    switch (gCurrentObject->oUnlockDoorStarState)
    {
    case UNLOCK_DOOR_STAR_RISING:
        gCurrentObject->oPosY += 3.4f; // Raise the star up in the air
        gCurrentObject->oMoveAngleYaw += gCurrentObject->oUnlockDoorStarYawVel; // Apply yaw velocity
        scale_object(gCurrentObject, gCurrentObject->oUnlockDoorStarTimer / 50.0f + 0.5f); // Scale the star to be bigger
        if (++gCurrentObject->oUnlockDoorStarTimer == 30)
        {
            gCurrentObject->oUnlockDoorStarTimer = 0;
            gCurrentObject->oUnlockDoorStarState++; // Sets state to UNLOCK_DOOR_STAR_WAITING
        }
        break;
    case UNLOCK_DOOR_STAR_WAITING:
        gCurrentObject->oMoveAngleYaw += gCurrentObject->oUnlockDoorStarYawVel; // Apply yaw velocity
        if (++gCurrentObject->oUnlockDoorStarTimer == 30)
        {
            SetSound(SOUND_MENU_STARSOUND, gCurrentObject->header.gfx.cameraToObject); // Play final sound
            obj_hide(); // Hide the object
            gCurrentObject->oUnlockDoorStarTimer = 0;
            gCurrentObject->oUnlockDoorStarState++; // Sets state to UNLOCK_DOOR_STAR_SPAWNING_PARTICLES
        }
        break;
    case UNLOCK_DOOR_STAR_SPAWNING_PARTICLES:
        // Spawn two particles, opposite sides of the star.
        bhvUnlockDoorStar_spawn_particle(0);
        bhvUnlockDoorStar_spawn_particle(0x8000);
        if (gCurrentObject->oUnlockDoorStarTimer++ == 20)
        {
            gCurrentObject->oUnlockDoorStarTimer = 0;
            gCurrentObject->oUnlockDoorStarState++; // Sets state to UNLOCK_DOOR_STAR_DONE
        }
        break;
    case UNLOCK_DOOR_STAR_DONE: // The object stays loaded for an additional 50 frames so that the sound doesn't immediately stop.
        if (gCurrentObject->oUnlockDoorStarTimer++ == 50)
            mark_object_for_deletion(gCurrentObject);
        break;
    }
    // Checks if the angle has cycled back to 0.
    // This means that the code will execute when the star completes a full revolution.
    if (prevYaw > (s16)gCurrentObject->oMoveAngleYaw)
        SetSound(SOUND_GENERAL_SHORTSTAR, gCurrentObject->header.gfx.cameraToObject); // Play a sound every time the star spins once
}


static Gfx *func_802769E0(struct GraphNode12A *node, s16 b)
{
    Gfx *sp2C;
    Gfx *sp28 = NULL;

    if (b == 255)
    {
        node->fnNode.node.flags = (node->fnNode.node.flags & 0xFF) | 0x100;
        sp28 = alloc_display_list(2 * sizeof(*sp28));
        sp2C = sp28;
    }
    else
    {
        node->fnNode.node.flags = (node->fnNode.node.flags & 0xFF) | 0x500;
        sp28 = alloc_display_list(3 * sizeof(*sp28));
        sp2C = sp28;
        gDPSetAlphaCompare(sp2C++, G_AC_DITHER);
    }
    gDPSetEnvColor(sp2C++, 255, 255, 255, b);
    gSPEndDisplayList(sp2C);
    return sp28;
}

Gfx *Geo18_802770A4(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    UNUSED u8 unused1[4];
    Gfx *sp28 = NULL;
    struct GraphNode12A *sp24 = (struct GraphNode12A *)node;
    struct MarioBodyState *sp20 = &D_8033A040[sp24->unk18];
    s16 sp1E;
    UNUSED u8 unused2[4];

    if (run == TRUE)
    {
        sp1E = (sp20->unk08 & 0x100) ? (sp20->unk08 & 0xFF) : 255;
        sp28 = func_802769E0(sp24, sp1E);
    }
    return sp28;
}

s32 geo_switch_mario_stand_run(s32 run, struct GraphNode *node, UNUSED Mat4 *mtx)
{
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    struct MarioBodyState *sp0 = &D_8033A040[switchCase->numCases];

    if (run == TRUE)
        // assign result. 0 if moving, 1 if stationary.
        switchCase->unk1E = ((sp0->action & ACT_FLAG_STATIONARY) == FALSE);
    return 0;
}

s32 geo_switch_mario_eyes(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    struct MarioBodyState *sp8 = &D_8033A040[switchCase->numCases];
    s16 sp6;

    if (run == TRUE)
    {
        if (sp8->eyeState == 0)
        {
            sp6 = ((switchCase->numCases * 32 + gAreaUpdateCounter) >> 1) & 0x1F;
            if (sp6 < 7)
                switchCase->unk1E = D_8032CDF0[sp6];
            else
                switchCase->unk1E = 0;
        }
        else
        {
            switchCase->unk1E = sp8->eyeState - 1;
        }
    }
    return 0;
}

Gfx *Geo18_80277294(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNode12A *sp24 = (struct GraphNode12A *)node;
    struct MarioBodyState *sp20 = &D_8033A040[sp24->unk18];
    s32 action = sp20->action;

    if (run == TRUE)
    {
        struct GraphNode017 *sp18 = (struct GraphNode017 *)node->next;

        if (action != 0x00840452 && action != 0x00840454 && action != 0x04000440 && action != 0x20810446)
            vec3s_copy(sp20->unkC, D_80385FDC);
        sp18->unk18[0] = sp20->unkC[1];
        sp18->unk18[1] = sp20->unkC[2];
        sp18->unk18[2] = sp20->unkC[0];
    }
    return NULL;
}

Gfx *Geo18_802773A4(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNode12A *sp2C = (struct GraphNode12A *)node;
    struct MarioBodyState *sp28 = &D_8033A040[sp2C->unk18];
    s32 action = sp28->action;

    if (run == TRUE)
    {
        struct GraphNode017 *sp20 = (struct GraphNode017 *)node->next;
        u8 *sp1C = (u8 *)D_8032CF9C->unk18;

        if (*sp1C == 6)
        {
            sp20->unk18[0] = gPlayerStatusForCamera->unk16[1];
            sp20->unk18[2] = gPlayerStatusForCamera->unk16[0];
        }
        else if (action & 0x20000000)
        {
            sp20->unk18[0] = sp28->unk12[1];
            sp20->unk18[1] = sp28->unk12[2];
            sp20->unk18[2] = sp28->unk12[0];
        }
        else
        {
            vec3s_set(sp28->unk12, 0, 0, 0);
            vec3s_set(sp20->unk18, 0, 0, 0);
        }
    }
    return NULL;
}

s32 geo_switch_mario_hand(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    struct MarioBodyState *sp0 = &D_8033A040[0];

    if (run == TRUE)
    {
        if (sp0->handState == 0)
        {
            switchCase->unk1E = ((sp0->action & ACT_FLAG_SWIMMING_OR_FLYING) != 0);
        }
        else
        {
            if (switchCase->numCases == 0)
                switchCase->unk1E = (sp0->handState < 5) ? sp0->handState : 1;
            else
                switchCase->unk1E = (sp0->handState < 2) ? sp0->handState : 0;
        }
    }
    return 0;
}

Gfx *Geo18_802775CC(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNode12A *spC = (struct GraphNode12A *)node;
    struct GraphNodeScaleOptionalDisplayList *sp8 = (struct GraphNodeScaleOptionalDisplayList *)node->next;
    struct MarioBodyState *sp4 = &D_8033A040[0];

    if (run == TRUE)
    {
        sp8->scale = 1.0f;
        if (spC->unk18 == sp4->unk0B >> 6)
        {
            if (D_8032CE0C != gAreaUpdateCounter && (sp4->unk0B & 0x3F) > 0)
            {
                sp4->unk0B--;
                D_8032CE0C = gAreaUpdateCounter;
            }
            sp8->scale = D_8032CDF8[spC->unk18 * 6 + (sp4->unk0B & 0x3F)] / 10.0f;
        }
    }
    return NULL;
}

s32 geo_switch_mario_cap_effect(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    struct MarioBodyState *sp0 = &D_8033A040[switchCase->numCases];

    if (run == TRUE)
        switchCase->unk1E = sp0->unk08 >> 8;
    return 0;
}

s32 geo_switch_mario_cap_on_off(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNode *next = node->next;
    struct GraphNodeSwitchCase *switchCase = (struct GraphNodeSwitchCase *)node;
    struct MarioBodyState *sp4 = &D_8033A040[switchCase->numCases];

    if (run == TRUE)
    {
        switchCase->unk1E = sp4->capState & 1;
        while (next != node)
        {
            if (next->type == 21)
            {
                if (sp4->capState & 2)
                    next->flags |= 1;
                else
                    next->flags &= ~1;
            }
            next = next->next;
        }
    }
    return 0;
}

Gfx *Geo18_80277824(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    s16 spE;
    struct GraphNode12A *sp8 = (struct GraphNode12A *)node;

    if (run == TRUE)
    {
        struct GraphNode017 *sp4 = (struct GraphNode017 *)node->next;

        if (D_8033A040[sp8->unk18 >> 1].unk07 == 0)
            spE = (coss((gAreaUpdateCounter & 0xF) << 12) + 1.0f) * 4096.0f;
        else
            spE = (coss((gAreaUpdateCounter & 7) << 13) + 1.0f) * 6144.0f;
        if (!(sp8->unk18 & 1))
            sp4->unk18[0] = -spE;
        else
            sp4->unk18[0] = spE;
    }
    return NULL;
}

Gfx *Geo1C_8027795C(s32 a, struct GraphNode *b, Mat4 *c)
{
    struct Struct8027795C *sp2C = (struct Struct8027795C *)b;
    Mat4 *sp28 = c;
    struct MarioState *sp24 = &gMarioStates[sp2C->unk18];

    if (a == 1)
    {
        sp2C->unk1C = 0;
        if (sp24->heldObj != NULL)
        {
            sp2C->unk1C = sp24->heldObj;
            switch (sp24->unk98->unk0A)
            {
            case 1:
                if (sp24->action & ACT_FLAG_THROWING)
                    vec3s_set(sp2C->unk20, 50, 0, 0);
                else
                    vec3s_set(sp2C->unk20, 50, 0, 110);
                break;
            case 2:
                vec3s_set(sp2C->unk20, 145, -173, 180);
                break;
            case 3:
                vec3s_set(sp2C->unk20, 80, -270, 1260);
                break;
            }
        }
    }
    else if (a == 5)
    {
        func_8037A550(sp24->unk98->unk18, *sp28, D_8032CF9C->unk34);
    }
    return NULL;
}

Gfx *Geo18_80277B14(s32 run, struct GraphNode *node, UNUSED Mat4 *c)
{
    f32 sp34;
    struct Object *sp30 = gMarioStates->marioObj;

    switch (run)
    {
    case 0:
        init_graph_node_object(NULL, &D_80339FE0, NULL, D_80385FD0, D_80385FDC, D_80385FE4);
        break;
    case 3:
        func_8037C044(node, &D_80339FE0.node);
        break;
    case 2:
        func_8037C0BC(&D_80339FE0.node);
        break;
    case 1:
        if (sp30->header.gfx.pos[0] > 1700.0f)
        {
            // TODO: Is this a geo layout copy or a graph node copy?
            D_80339FE0.asGraphNode = sp30->header.gfx.asGraphNode;
            D_80339FE0.unk18 = sp30->header.gfx.unk18;
            vec3s_copy(D_80339FE0.angle, sp30->header.gfx.angle);
            vec3f_copy(D_80339FE0.pos, sp30->header.gfx.pos);
            vec3f_copy(D_80339FE0.scale, sp30->header.gfx.scale);
            // FIXME: why does this set unk38, an inline struct, to a ptr to another one? wrong
            // GraphNode types again?
            D_80339FE0.unk38 = *(struct GraphNodeObject_sub *)&sp30->header.gfx.unk38.animID;
            sp34 = 4331.53 - D_80339FE0.pos[0];
            D_80339FE0.pos[0] = sp34 + 4331.53;
            D_80339FE0.angle[1] = -D_80339FE0.angle[1];
            D_80339FE0.scale[0] *= -1.0f;
            // FIXME: Why doesn't this match?
            //D_80339FE0.node.flags |= 1;
            ((s16 *)&D_80339FE0)[1] |= 1;
        }
        else
        {
            // FIXME: Why doesn't this match?
            //D_80339FE0.node.flags &= ~1;
            ((s16 *)&D_80339FE0)[1] &= ~1;
        }
        break;
    }
    return NULL;
}

Gfx *Geo18_80277D6C(s32 a, struct GraphNode *node, UNUSED Mat4 *c)
{
    struct GraphNode12A *sp34 = (struct GraphNode12A *)node;
    Gfx *sp30 = NULL;

    if (a == 1 && D_8032CFA0 == &D_80339FE0)
    {
        sp30 = alloc_display_list(3 * sizeof(*sp30));

        if (sp34->unk18 == 0)
        {
            gSPClearGeometryMode(&sp30[0], G_CULL_BACK);
            gSPSetGeometryMode(&sp30[1], G_CULL_FRONT);
            gSPEndDisplayList(&sp30[2]);
        }
        else
        {
            gSPClearGeometryMode(&sp30[0], G_CULL_FRONT);
            gSPSetGeometryMode(&sp30[1], G_CULL_BACK);
            gSPEndDisplayList(&sp30[2]);
        }
        sp34->fnNode.node.flags = (sp34->fnNode.node.flags & 0xFF) | 0x100;
    }
    return sp30;
}
