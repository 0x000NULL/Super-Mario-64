#include <ultra64.h>

#include "sm64.h"
#include "types.h"
#include "behavior_actions.h"
#include "game.h"
#include "main.h"
#include "mario.h"
#include "engine/behavior_script.h"
#include "engine/math_util.h"
#include "object_helpers.h"
#include "object_helpers2.h"
#include "behavior_data.h"
#include "obj_behaviors.h"
#include "engine/surface_collision.h"
#include "engine/surface_load.h"
#include "level_update.h"
#include "audio/interface_2.h"
#include "save_file.h"
#include "area.h"
#include "engine/graph_node.h"
#include "camera.h"
#include "display.h"
#include "spawn_object.h"
#include "mario_actions_cutscene.h"
#include "object_list_processor.h"
#include "spawn_sound.h"
#include "debug.h"
#include "object_constants.h"
#include "mario_step.h"
#include "obj_behaviors_2.h"
#include "platform_displacement.h"
#include "interaction.h"
#include "ingame_menu.h"
#include "room.h"
#include "rendering_graph_node.h"

#define o gCurrentObject

// BSS
s16 D_8035FF10;

struct Struct8032F24A {
    s16 pad;
    s16 unk0;
    void* unk1;
    s16 unk2;
};

struct Struct8032F34C {
    s16 numBridgeSections;
    s16 bridgeRelativeStartingXorZ;
    s16 platformWidth;
    s16 model;
    void* segAddr;
};

struct Struct8032FB4C {
    u8 unk0;
    s8 unk1;
    s8 unk2;
    s8 unk3;
    s8* unk4;
};

struct Struct8032F698 {
    void* unk0;
    s16 unk1;
    s16 unk2;
    s16 unk3;
    s16 unk4;
};

struct Struct802C0DF0 {
    u8 unk0;
    u8 unk1;
    u8 unk2;
    u8 model;
    void* behavior;
};

struct Struct8032FE4C {
    s32 unk0;
    s32 unk1;
    f32 unk2;
    f32 unk3;
};

struct Struct8032F754 {
    s32 unk0;
    Vec3f unk1;
    f32 unk2;
};

struct Struct8032FCE8 {
    s16 unk0;
    s16 unk1;
    void* unk2;
};

extern void BehClimbDetectLoop();
extern s16 gDebugInfo[][8];
extern s8 gDoorAdjacentRooms[][2];
extern u8 inside_castle_seg7_collision_ddd_warp_2[];
extern u8 inside_castle_seg7_collision_ddd_warp[];
extern s32 gDialogueResponse;
extern s32 gCutsceneActive;
extern u8 gCutsceneNumber;
extern s8* D_8032F96C[];
extern u32 bowser_seg6_unkmoveshorts_060576FC[];
extern u32 blue_fish_seg3_anims_0301C2B0[];
extern u32 cyan_fish_seg6_anims_0600E264[];
extern u32 blue_fish_seg3_anims_0301C2B0[];

void func_802A8D18(f32,f32,s32);

s32 func_802BDAD4(void);
void func_802C49E0(void);

s32 D_8032F0C0[] = {SAVE_FLAG_HAVE_WING_CAP,SAVE_FLAG_HAVE_METAL_CAP,SAVE_FLAG_HAVE_VANISH_CAP};

s16 D_8032F0CC[] = {6047,5664,5292,4934,4587,4254,3933,3624,3329,3046,2775,2517,2271,2039,1818,1611,1416,1233,1063,906,761,629,509,402,308,226,157,100,56,25,4,0};

#include "behaviors/star_door.inc.c"
#include "behaviors/mr_i.inc.c"
#include "behaviors/pole.inc.c"
#include "behaviors/thi_top.inc.c"
#include "behaviors/capswitch.inc.c"
#include "behaviors/king_bobomb.inc.c"
#include "behaviors/beta_chest.inc.c"
#include "behaviors/water_objs.inc.c"
#include "behaviors/cannon.inc.c"
#include "behaviors/chuckya.inc.c"
#include "behaviors/breakable_wall.inc.c"
#include "behaviors/kickable_board.inc.c"
#include "behaviors/tower_door.inc.c"
#include "behaviors/rotating_platform.inc.c"
#include "behaviors/koopa_shell_underwater.inc.c"
#include "behaviors/warp.inc.c"
#include "behaviors/white_puff_explode.inc.c"



// not in behavior file
struct SpawnParticlesInfo D_8032F270 = {2,20,MODEL_MIST,0,40,5,30,20,252,30,330.0f,10.0f};

void func_802AA618(s32 sp18,s32 sp1C,f32 sp20)
{
    D_8032F270.sizeBase = sp20;
    D_8032F270.sizeRange = sp20/20.0;
    D_8032F270.offsetY = sp1C;
    if(sp18 == 0)
        D_8032F270.count = 20;
    else if(sp18 > 20)
        D_8032F270.count = sp18;
    else
        D_8032F270.count = 4;
    obj_spawn_particles(&D_8032F270);
}

#include "behaviors/sparkle_spawn_star.inc.c"
#include "behaviors/coin.inc.c"
#include "behaviors/collide_particles.inc.c"
#include "behaviors/door.inc.c"
#include "behaviors/thwomp.inc.c"
#include "behaviors/tumbling_bridge.inc.c"
#include "behaviors/elevator.inc.c"
#include "behaviors/water_mist_particle.inc.c"
#include "behaviors/break_particles.inc.c"
#include "behaviors/water_mist.inc.c"
#include "behaviors/ground_particles.inc.c"
#include "behaviors/wind.inc.c"
#include "behaviors/piranha_particles.inc.c"
#include "behaviors/little_cage.inc.c"
#include "behaviors/falling_rising_platform.inc.c"
#include "behaviors/fishing_boo.inc.c"
#include "behaviors/flamethrower.inc.c"
#include "behaviors/bouncing_fireball.inc.c"
#include "behaviors/shock_wave.inc.c"
#include "behaviors/flame_mario.inc.c"
#include "behaviors/beta_fish_splash_spawner.inc.c"
#include "behaviors/spindrift.inc.c"
#include "behaviors/tower_platform.inc.c"
#include "behaviors/tree_particles.inc.c"
#include "behaviors/square_platform_cycle.inc.c"
#include "behaviors/piranha_bubbles.inc.c"
#include "behaviors/purple_switch.inc.c"
#include "behaviors/metal_box.inc.c"
#include "behaviors/switch_hidden_objects.inc.c"
#include "behaviors/breakable_box.inc.c"

// not sure what this is doing here. not in a behavior file.
s32 Geo18_802B1BB0(s32 run, UNUSED struct GraphNode *node, Mat4 mtx)
{
    Mat4 sp20;
    struct Object* sp1C;

    if(run == TRUE)
    {
        sp1C = (struct Object*)D_8032CFA0;
        if(sp1C == gMarioObject && sp1C->prevObj != NULL)
        {
            func_8029D704(sp20,mtx,D_8032CF9C->unk34);
            func_8029D558(sp20,sp1C->prevObj);
            func_8029EA0C(sp1C->prevObj);
        }
    }
    return 0;
}

#include "behaviors/heave_ho.inc.c"
#include "behaviors/spawn_star_exit.inc.c"
#include "behaviors/pound_explode.inc.c"
#include "behaviors/beta_trampoline.inc.c"
#include "behaviors/jumping_box.inc.c"
#include "behaviors/boo_cage.inc.c"

// not in behavior file
void func_802B2328(s32 n,s32 a1,s32 a2,s32 r) // n is the number of objects to spawn, r if the rate of change of phase (frequency?)
{
    s32 i;
    s16 seperation = 0x10000/n; // Evenly spread around a circle
    for(i=0;i<n;i++)
        spawn_object_relative(0,
                                    sins(D_8035FF10+i*seperation)*a1,
                                    (i+1)*a2,
                                    coss(D_8035FF10+i*seperation)*a1,
                                    o,MODEL_NONE,bhvPowerupSparkles2);

    D_8035FF10 += r*0x100;
}

#include "behaviors/beta_boo_key.inc.c"
#include "behaviors/grand_star.inc.c"
#include "behaviors/bowser_key.inc.c"
#include "behaviors/bullet_bill.inc.c"
#include "behaviors/bowser.inc.c"
#include "behaviors/blue_fish.inc.c"
#include "behaviors/checkerboard_platform.inc.c"
#include "behaviors/ddd_warp.inc.c"
#include "behaviors/water_pillar.inc.c"
#include "behaviors/moat_drainer.inc.c"
#include "behaviors/door_key.inc.c"
#include "behaviors/moat_grill.inc.c"
#include "behaviors/clock_arm.inc.c"
#include "behaviors/ukiki_cage.inc.c"
#include "behaviors/lll_octagonal_rotating_mesh.inc.c"
#include "behaviors/lll_sinking_rock_block.inc.c"
#include "behaviors/lll_rotating_hex_flame.inc.c"
#include "behaviors/lll_floating_wood_piece.inc.c"
#include "behaviors/lll_volcano_flames.inc.c"
#include "behaviors/lll_hexagonal_ring.inc.c"
#include "behaviors/lll_sinking_rectangle.inc.c"
#include "behaviors/lll_tilting_platform.inc.c"
#include "behaviors/koopa_shell.inc.c"
#include "behaviors/tox_box.inc.c"
#include "behaviors/piranha_plant.inc.c"
#include "behaviors/bowser_puzzle_piece.inc.c"
#include "behaviors/tuxie.inc.c"
#include "behaviors/fish.inc.c"
#include "behaviors/express_elevator.inc.c"
#include "behaviors/bub.inc.c"
#include "behaviors/exclamation_box.inc.c"
#include "behaviors/sound_spawner.inc.c"
#include "behaviors/ddd_sub.inc.c"
#include "behaviors/sushi.inc.c"
#include "behaviors/jrb_ship.inc.c"
#include "behaviors/white_puff.inc.c"
#include "behaviors/blue_coin.inc.c"
#include "behaviors/grill_door.inc.c"
#include "behaviors/wdw_water_level.inc.c"
#include "behaviors/tweester.inc.c"
#include "behaviors/boo.inc.c"
#include "behaviors/bbh_tilt_floor.inc.c"
#include "behaviors/bbh_tumbling_bookshelf.inc.c"
#include "behaviors/bbh_merry_go_round.inc.c"
#include "behaviors/static_checkered_platform.inc.c"
#include "behaviors/beta_following_circling_obj.inc.c"
#ifndef VERSION_JP
#include "behaviors/music_touch.inc.c"
#endif
#include "behaviors/castle_floor_trap.inc.c"

// not in behavior file
void BehClimbDetectLoop(void)
{
    if(o->oPosY - 10.0f < gMarioObject->oPosY && gMarioObject->oPosY < o->oPosY + o->hitboxHeight + 30.0f)
        if(o->oTimer > 10)
            if(!(gMarioStates->action & MARIO_PUNCHING))
                obj_push_mario_away(70.0f);
}

#include "behaviors/powerup_sparkles.inc.c"
#include "behaviors/scuttlebug.inc.c" // :scuttleeyes:
#include "behaviors/whomp.inc.c"
#include "behaviors/water_splash.inc.c"
#include "behaviors/wind_particle.inc.c"
#include "behaviors/snowman_wind.inc.c"
#include "behaviors/walking_penguin.inc.c"
