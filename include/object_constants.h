#ifndef _OBJECT_CONSTANTS_H
#define _OBJECT_CONSTANTS_H

// This file contains macros that provide descriptive names for
// field-specific and object-specific constants, e.g. actions.

/* activeFlags */
#define ACTIVE_FLAG_ACTIVE                 (1 <<  0) // 0x0001
#define ACTIVE_FLAG_FAR_AWAY               (1 <<  1) // 0x0002
#define ACTIVE_FLAG_UNK2                   (1 <<  2) // 0x0004
#define ACTIVE_FLAG_IN_DIFFERENT_ROOM      (1 <<  3) // 0x0008
#define ACTIVE_FLAG_UNIMPORTANT            (1 <<  4) // 0x0010
#define ACTIVE_FLAG_INITIATED_TIME_STOP    (1 <<  5) // 0x0020
#define ACTIVE_FLAG_MOVE_THROUGH_GRATE     (1 <<  6) // 0x0040
#define ACTIVE_FLAG_UNK7                   (1 <<  7) // 0x0080
#define ACTIVE_FLAG_UNK8                   (1 <<  8) // 0x0100
#define ACTIVE_FLAG_UNK9                   (1 <<  9) // 0x0200
#define ACTIVE_FLAG_UNK10                  (1 << 10) // 0x0400

#define ACTIVE_FLAGS_DEACTIVATED 0

/* respawnInfoType */
#define RESPAWN_INFO_TYPE_NULL 0
#define RESPAWN_INFO_TYPE_32   1
#define RESPAWN_INFO_TYPE_16   2

/* respawnInfo */
#define RESPAWN_INFO_DONT_RESPAWN 0xFF

/* oFlags */
#define OBJ_FLAG_UPDATE_GFX_POS_AND_ANGLE         (1 <<  0) // 0x00000001
#define OBJ_FLAG_MOVE_XZ_USING_FVEL               (1 <<  1) // 0x00000002
#define OBJ_FLAG_MOVE_Y_WITH_TERMINAL_VEL         (1 <<  2) // 0x00000004
#define OBJ_FLAG_SET_FACE_YAW_TO_MOVE_YAW         (1 <<  3) // 0x00000008
#define OBJ_FLAG_0010                             (1 <<  4) // 0x00000010
#define OBJ_FLAG_0020                             (1 <<  5) // 0x00000020
#define OBJ_FLAG_COMPUTE_DIST_TO_MARIO            (1 <<  6) // 0x00000040
#define OBJ_FLAG_ACTIVE_FROM_AFAR                 (1 <<  7) // 0x00000080
#define OBJ_FLAG_0100                             (1 <<  8) // 0x00000100
#define OBJ_FLAG_TRANSFORM_RELATIVE_TO_PARENT     (1 <<  9) // 0x00000200
#define OBJ_FLAG_HOLDABLE                         (1 << 10) // 0x00000400
#define OBJ_FLAG_0800                             (1 << 11) // 0x00000800
#define OBJ_FLAG_1000                             (1 << 12) // 0x00001000
#define OBJ_FLAG_COMPUTE_ANGLE_TO_MARIO           (1 << 13) // 0x00002000
#define OBJ_FLAG_PERSISTENT_RESPAWN               (1 << 14) // 0x00004000
#define OBJ_FLAG_8000                             (1 << 15) // 0x00008000
#define OBJ_FLAG_30                               (1 << 30) // 0x40000000

/* oHeldState */
#define HELD_FREE 0
#define HELD_HELD 1
#define HELD_THROWN 2
#define HELD_DROPPED 3

/* oDialogueState */
#define DIALOGUE_UNK1_ENABLE_TIME_STOP 0
#define DIALOGUE_UNK1_INTERRUPT_MARIO_ACTION 1
#define DIALOGUE_UNK1_BEGIN_DIALOGUE 2
#define DIALOGUE_UNK1_AWAIT_DIALOGUE 3
#define DIALOGUE_UNK1_DISABLE_TIME_STOP 4

#define DIALOGUE_UNK1_FLAG_1 (1 << 1) // 0x02
#define DIALOGUE_UNK1_FLAG_2 (1 << 2) // 0x04
#define DIALOGUE_UNK1_FLAG_4 (1 << 4) // 0x10

#define DIALOGUE_UNK2_ENABLE_TIME_STOP 0
#define DIALOGUE_UNK2_TURN_AND_INTERRUPT_MARIO_ACTION 1
#define DIALOGUE_UNK2_AWAIT_DIALOGUE 2
#define DIALOGUE_UNK2_END_DIALOGUE 3

#define DIALOGUE_UNK2_FLAG_0 (1 << 0) // 0x01
#define DIALOGUE_UNK2_LEAVE_TIME_STOP_ENABLED (1 << 4) // 0x10

/* oMoveFlags */
#define OBJ_MOVE_LANDED                (1 <<  0) // 0x0001
#define OBJ_MOVE_ON_GROUND             (1 <<  1) // 0x0002  // mutually exclusive to OBJ_MOVE_LANDED
#define OBJ_MOVE_LEFT_GROUND           (1 <<  2) // 0x0004
#define OBJ_MOVE_ENTERED_WATER         (1 <<  3) // 0x0008
#define OBJ_MOVE_AT_WATER_SURFACE      (1 <<  4) // 0x0010
#define OBJ_MOVE_UNDERWATER_OFF_GROUND (1 <<  5) // 0x0020
#define OBJ_MOVE_UNDERWATER_ON_GROUND  (1 <<  6) // 0x0040
#define OBJ_MOVE_IN_AIR                (1 <<  7) // 0x0080
#define OBJ_MOVE_8                     (1 <<  8) // 0x0100
#define OBJ_MOVE_HIT_WALL              (1 <<  9) // 0x0200
#define OBJ_MOVE_HIT_EDGE              (1 << 10) // 0x0400
#define OBJ_MOVE_ABOVE_LAVA            (1 << 11) // 0x0800
#define OBJ_MOVE_12                    (1 << 12) // 0x1000
#define OBJ_MOVE_13                    (1 << 13) // 0x2000
#ifndef VERSION_JP
#define OBJ_MOVE_ABOVE_DEATH_BARRIER   (1 << 14) // 0x4000
#endif

#define OBJ_MOVE_MASK_ON_GROUND (OBJ_MOVE_LANDED | OBJ_MOVE_ON_GROUND)
#define OBJ_MOVE_MASK_33 0x33
#define OBJ_MOVE_MASK_IN_WATER (\
    OBJ_MOVE_ENTERED_WATER |\
    OBJ_MOVE_AT_WATER_SURFACE |\
    OBJ_MOVE_UNDERWATER_OFF_GROUND |\
    OBJ_MOVE_UNDERWATER_ON_GROUND)
#define OBJ_MOVE_MASK_HIT_WALL_OR_IN_WATER \
    (OBJ_MOVE_HIT_WALL | OBJ_MOVE_MASK_IN_WATER)

/* oActiveParticleFlags */
#define ACTIVE_PARTICLE_0    0x00000001
#define ACTIVE_PARTICLE_3    0x00000008
#define ACTIVE_PARTICLE_4    0x00000010
#define ACTIVE_PARTICLE_5    0x00000020
#define ACTIVE_PARTICLE_6    0x00000040
#define ACTIVE_PARTICLE_7    0x00000080
#define ACTIVE_PARTICLE_8    0x00000100
#define ACTIVE_PARTICLE_9    0x00000200
#define ACTIVE_PARTICLE_10   0x00000400
#define ACTIVE_PARTICLE_11   0x00000800
#define ACTIVE_PARTICLE_12   0x00001000
#define ACTIVE_PARTICLE_13   0x00002000
#define ACTIVE_PARTICLE_14   0x00004000
#define ACTIVE_PARTICLE_15   0x00008000
#define ACTIVE_PARTICLE_16   0x00010000
#define ACTIVE_PARTICLE_17   0x00020000
#define ACTIVE_PARTICLE_18   0x00040000
#define ACTIVE_PARTICLE_19   0x00080000

/* oAction */
#define OBJ_ACT_LAVA_DEATH 100
#define OBJ_ACT_DEATH_PLANE_DEATH 101

#define OBJ_ACT_HORIZONTAL_KNOCKBACK 100
#define OBJ_ACT_VERTICAL_KNOCKBACK 101
#define OBJ_ACT_SQUISHED 102

/* Bob-omb */
    /* oBehParams2ndByte */
    #define BOBOMB_BP_STYPE_GENERIC 0
    #define BOBOMB_BP_STYPE_STATIONARY 1
    /* oAction */
    #define BOBOMB_ACT_PATROL 0
    #define BOBOMB_ACT_LAUNCHED 1
    #define BOBOMB_ACT_CHASE_MARIO 2
    #define BOBOMB_ACT_EXPLODE 3
    #define BOBOMB_ACT_LAVA_DEATH 100
    #define BOBOMB_ACT_DEATH_PLANE_DEATH 101
    
/* Moving Blue Coin */
    /* oAction */
    #define MOV_BCOIN_ACT_STILL 0
    #define MOV_BCOIN_ACT_MOVING 1

/* Moving Yellow Coin */
    /* oAction */
    #define MOV_YCOIN_ACT_IDLE 0
    #define MOV_YCOIN_ACT_BLINKING 1
    #define MOV_YCOIN_ACT_LAVA_DEATH 100
    #define MOV_YCOIN_ACT_DEATH_PLANE_DEATH 101
    
/* Bob-omb Buddy */
    /* oBehParams2ndByte */
    #define BOBOMB_BUDDY_BP_STYPE_GENERIC 0
    #define BOBOMB_BUDDY_BP_STYPE_BOB_GRASS_KBB 1
    #define BOBOMB_BUDDY_BP_STYPE_BOB_CANNON_KBB 2
    #define BOBOMB_BUDDY_BP_STYPE_BOB_GRASS 3
    /* oAction */
    #define BOBOMB_BUDDY_ACT_IDLE 0
    #define BOBOMB_BUDDY_ACT_TURN_TO_TALK 2
    #define BOBOMB_BUDDY_ACT_TALK 3
    /* oBobombBuddyRole */
    #define BOBOMB_BUDDY_ROLE_ADVICE 0
    #define BOBOMB_BUDDY_ROLE_CANNON 1
    /* oBobombBuddyCannonStatus */
    #define BOBOMB_BUDDY_CANNON_UNOPENED 0
    #define BOBOMB_BUDDY_CANNON_OPENING 1
    #define BOBOMB_BUDDY_CANNON_OPENED 2
    #define BOBOMB_BUDDY_CANNON_STOP_TALKING 3
    /* oBobombBuddyHasTalkedToMario */
    #define BOBOMB_BUDDY_HAS_NOT_TALKED 0
    #define BOBOMB_BUDDY_HAS_TALKED 2
    
/* Cannon Trap Door */
    /* oAction */
    #define CANNON_TRAP_DOOR_ACT_CLOSED 0
    #define CANNON_TRAP_DOOR_ACT_CAM_ZOOM 1
    #define CANNON_TRAP_DOOR_ACT_OPENING 2
    #define CANNON_TRAP_DOOR_ACT_OPEN 3
    
/* Homing Amp */
    /* oAction */
    #define AMP_HOMING_ACT_INACTIVE 0
    #define AMP_HOMING_ACT_APPEAR 1
    #define AMP_HOMING_ACT_CHASE 2
    #define AMP_HOMING_ACT_GIVE_UP 3
    #define AMP_HOMING_ACT_ATTACK_COOLDOWN 4
    
/* Amp */
    /* oBehParams2ndByte */
    #define AMP_BP_ROT_RADIUS_200 0
    #define AMP_BP_ROT_RADIUS_300 1
    #define AMP_BP_ROT_RADIUS_400 2
    #define AMP_BP_ROT_RADIUS_0 3
    /* oAction */
    #define AMP_ACT_IDLE 2
    #define AMP_ATTACK_COOLDOWN 4
    
/* Butterfly */
    /* oAction */
    #define BUTTERFLY_ACT_RESTING 0
    #define BUTTERFLY_ACT_FOLLOW_MARIO 1
    #define BUTTERFLY_ACT_RETURN_HOME 2

/* Hoot */
    /* oHootAvailability */
    #define HOOT_AVAIL_ASLEEP_IN_TREE 0
    #define HOOT_AVAIL_WANTS_TO_TALK 1
    #define HOOT_AVAIL_READY_TO_FLY 2
    /* oAction */
    #define HOOT_ACT_ASCENT 0
    #define HOOT_ACT_CARRY 1
    #define HOOT_ACT_TIRED 2
    
/* Bully (all variants) */
    /* oBehParams2ndByte */
    #define BULLY_BP_SIZE_SMALL 0
    #define BULLY_BP_SIZE_BIG 1
    /* oAction */
    #define BULLY_ACT_PATROL 0
    #define BULLY_ACT_CHASE_MARIO 1
    #define BULLY_ACT_KNOCKBACK 2
    #define BULLY_ACT_BACK_UP 3
    #define BULLY_ACT_INACTIVE 4
    #define BULLY_ACT_ACTIVATE_AND_FALL 5
    #define BULLY_ACT_LAVA_DEATH 100
    #define BULLY_ACT_DEATH_PLANE_DEATH 101
    /* oBullySubtype */
    #define BULLY_STYPE_GENERIC 0
    #define BULLY_STYPE_MINION 1
    #define BULLY_STYPE_CHILL 16
    
/* Water Ring (all variants */
    /* oAction */
    #define WATER_RING_ACT_NOT_COLLECTED 0
    #define WATER_RING_ACT_COLLECTED 1
    
/* Jet Stream Water Ring Spawner */
    /* oAction */
    #define JS_RING_SPAWNER_ACT_ACTIVE 0
    #define JS_RING_SPAWNER_ACT_INACTIVE 1
    
/* Celebration Star */
    /* oAction */
    #define CELEB_STAR_ACT_SPIN_AROUND_MARIO 0
    #define CELEB_STAR_ACT_FACE_CAMERA 1
    
/* LLL Drawbridge */
    /* oAction */
    #define LLL_DRAWBRIDGE_ACT_LOWER 0
    #define LLL_DRAWBRIDGE_ACT_RAISE 1
    
/* Bomp (both variants) */
    /* oAction */
    #define BOMP_ACT_WAIT 0
    #define BOMP_ACT_POKE_OUT 1
    #define BOMP_ACT_EXTEND 2
    #define BOMP_ACT_RETRACT 3
    
/* WF Sliding Brick Platform */
    /* oBehParams2ndByte */
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_10 1
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_15 2
    #define WF_SLID_BRICK_PTFM_BP_MOV_VEL_20 3
    /* oAction */
    #define WF_SLID_BRICK_PTFM_ACT_WAIT 0
    #define WF_SLID_BRICK_PTFM_ACT_EXTEND 1
    #define WF_SLID_BRICK_PTFM_ACT_RETRACT 2
    
/* Fake Moneybag Coin */
    /* oAction */
    #define FAKE_MONEYBAG_COIN_ACT_IDLE 0
    #define FAKE_MONEYBAG_COIN_ACT_TRANSFORM 1
    
/* Moneybag */
    /* oAction */
    #define MONEYBAG_ACT_APPEAR 0
    #define MONEYBAG_ACT_UNUSED_APPEAR 1
    #define MONEYBAG_ACT_MOVE_AROUND 2
    #define MONEYBAG_ACT_RETURN_HOME 3
    #define MONEYBAG_ACT_DISAPPEAR 4
    #define MONEYBAG_ACT_DEATH 5
    /* oMoneybagJumpState */
    #define MONEYBAG_JUMP_LANDING 0
    #define MONEYBAG_JUMP_PREPARE 1
    #define MONEYBAG_JUMP_JUMP 2
    #define MONEYBAG_JUMP_JUMP_AND_BOUNCE 3
    #define MONEYBAG_JUMP_WALK_AROUND 4
    #define MONEYBAG_JUMP_WALK_HOME 5
    
/* Bowling Ball */
    /* oAction */
    #define BBALL_ACT_INITIALIZE 0
    #define BBALL_ACT_ROLL 1
    
/* Bowling Ball + Bowling Ball Spawner (all variants) */
    /* oBehParams2ndByte */
    #define BBALL_BP_STYPE_BOB_UPPER 0
    #define BBALL_BP_STYPE_TTM 1
    #define BBALL_BP_STYPE_BOB_LOWER 2
    #define BBALL_BP_STYPE_THI_LARGE 3
    #define BBALL_BP_STYPE_THI_SMALL 4
    
/* Bowling Ball (Free) */
    /* oAction */
    #define FREE_BBALL_ACT_IDLE 0
    #define FREE_BBALL_ACT_ROLL 1
    #define FREE_BBALL_ACT_RESET 2

/* Yoshi */
    /* oAction */
    #define YOSHI_ACT_IDLE 0
    #define YOSHI_ACT_WALK 1
    #define YOSHI_ACT_TALK 2
    #define YOSHI_ACT_WALK_JUMP_OFF_ROOF 3
    #define YOSHI_ACT_FINISH_JUMPING_AND_DESPAWN 4
    #define YOSHI_ACT_GIVE_PRESENT 5
    #define YOSHI_ACT_CREDITS 10

/* Koopa */
    /* oAction */
    #define KOOPA_UNSHELLED_ACT_RUN 0
    #define KOOPA_UNSHELLED_ACT_DIVE 1
    #define KOOPA_UNSHELLED_ACT_LYING 2
    #define KOOPA_UNSHELLED_ACT_UNUSED3 3

    #define KOOPA_SHELLED_ACT_STOPPED 0
    #define KOOPA_SHELLED_ACT_WALK 1
    #define KOOPA_SHELLED_ACT_RUN_FROM_MARIO 2
    #define KOOPA_SHELLED_ACT_LYING 3
    #define KOOPA_SHELLED_ACT_DIE 4

    #define KOOPA_THE_QUICK_ACT_WAIT_BEFORE_RACE 0
    #define KOOPA_THE_QUICK_ACT_UNUSED1 1
    #define KOOPA_THE_QUICK_ACT_SHOW_INIT_TEXT 2
    #define KOOPA_THE_QUICK_ACT_RACE 3
    #define KOOPA_THE_QUICK_ACT_DECELERATE 4
    #define KOOPA_THE_QUICK_ACT_STOP 5
    #define KOOPA_THE_QUICK_ACT_AFTER_RACE 6

    /* oSubAction */
    #define KOOPA_SHELLED_SUB_ACT_START_WALK 0
    #define KOOPA_SHELLED_SUB_ACT_WALK 1
    #define KOOPA_SHELLED_SUB_ACT_STOP_WALK 2

    #define KOOPA_THE_QUICK_SUB_ACT_START_RUN 0
    #define KOOPA_THE_QUICK_SUB_ACT_RUN 1
    #define KOOPA_THE_QUICK_SUB_ACT_JUMP 2

    /* oKoopaTheQuickRaceIndex */
    #define KOOPA_THE_QUICK_BOB_INDEX 0
    #define KOOPA_THE_QUICK_THI_INDEX 1

    /* oBehParams2ndByte */
    #define KOOPA_BP_UNSHELLED 0
    #define KOOPA_BP_NORMAL 1
    #define KOOPA_BP_KOOPA_THE_QUICK_BASE 2
    #define KOOPA_BP_KOOPA_THE_QUICK_BOB (KOOPA_BP_KOOPA_THE_QUICK_BASE + KOOPA_THE_QUICK_BOB_INDEX)
    #define KOOPA_BP_KOOPA_THE_QUICK_THI (KOOPA_BP_KOOPA_THE_QUICK_BASE + KOOPA_THE_QUICK_THI_INDEX)
    #define KOOPA_BP_TINY 4

/* Pokey */
    /* oAction */
    #define POKEY_ACT_UNINITIALIZED 0
    #define POKEY_ACT_WANDER 1
    #define POKEY_ACT_UNLOAD_PARTS 2

/* Swoop */
    /* oAction */
    #define SWOOP_ACT_IDLE 0
    #define SWOOP_ACT_MOVE 1

/* Fly guy */
    /* oAction */
    #define FLY_GUY_ACT_IDLE 0
    #define FLY_GUY_ACT_APPROACH_MARIO 1
    #define FLY_GUY_ACT_LUNGE 2
    #define FLY_GUY_ACT_SHOOT_FIRE 3

/* Goomba triplet spawner */
    /* oBehParams2ndByte */
    #define GOOMBA_TRIPLET_SPAWNER_BP_SIZE_MASK 0x00000003
    #define GOOMBA_TRIPLET_SPAWNER_BP_EXTRA_GOOMBAS_MASK 0x000000FC
    /* oAction */
    #define GOOMBA_TRIPLET_SPAWNER_ACT_UNLOADED 0
    #define GOOMBA_TRIPLET_SPAWNER_ACT_LOADED 1

/* Goomba */
    /* oBehParams2ndByte */
    #define GOOMBA_BP_SIZE_MASK 0x00000003
    #define GOOMBA_SIZE_REGULAR 0
    #define GOOMBA_SIZE_HUGE 1
    #define GOOMBA_SIZE_TINY 2
    #define GOOMBA_BP_TRIPLET_FLAG_MASK 0x000000FC
    /* oAction */
    #define GOOMBA_ACT_WALK 0
    #define GOOMBA_ACT_ATTACKED_MARIO 1
    #define GOOMBA_ACT_JUMP 2

/* Chain chomp */
    /* oAction */
    #define CHAIN_CHOMP_ACT_UNINITIALIZED 0
    #define CHAIN_CHOMP_ACT_MOVE 1
    #define CHAIN_CHOMP_ACT_UNLOAD_CHAIN 2

    /* oSubAction */
    #define CHAIN_CHOMP_SUB_ACT_TURN 0
    #define CHAIN_CHOMP_SUB_ACT_LUNGE 1

    /* oChainChompReleaseStatus */
    #define CHAIN_CHOMP_NOT_RELEASED 0
    #define CHAIN_CHOMP_RELEASED_TRIGGER_CUTSCENE 1
    #define CHAIN_CHOMP_RELEASED_LUNGE_AROUND 2
    #define CHAIN_CHOMP_RELEASED_BREAK_GATE 3
    #define CHAIN_CHOMP_RELEASED_JUMP_AWAY 4
    #define CHAIN_CHOMP_RELEASED_END_CUTSCENE 5

/* Chain chomp chain part */
    /* oBehParams2ndByte */
    #define CHAIN_CHOMP_CHAIN_PART_BP_PIVOT 0

/* Wooden post */
    /* oBehParams */
    #define WOODEN_POST_BP_NO_COINS_MASK 0x0000FF00

/* Wiggler */
    /* oAction */
    #define WIGGLER_ACT_UNINITIALIZED 0
    #define WIGGLER_ACT_WALK 1
    #define WIGGLER_ACT_KNOCKBACK 2
    #define WIGGLER_ACT_JUMPED_ON 3
    #define WIGGLER_ACT_SHRINK 4
    #define WIGGLER_ACT_FALL_THROUGH_FLOOR 5

    /* oWigglerTextStatus */
    #define WIGGLER_TEXT_STATUS_AWAIT_DIALOGUE 0
    #define WIGGLER_TEXT_STATUS_SHOWING_DIALOGUE 1
    #define WIGGLER_TEXT_STATUS_COMPLETED_DIALOGUE 2

/* Spiny */
    /* oAction */
    #define SPINY_ACT_WALK 0
    #define SPINY_ACT_HELD_BY_LAKITU 1
    #define SPINY_ACT_THROWN_BY_LAKITU 2
    #define SPINY_ACT_ATTACKED_MARIO 3

/* Evil lakitu */
    /* oAction */
    #define ENEMY_LAKITU_ACT_UNINITIALIZED 0
    #define ENEMY_LAKITU_ACT_MAIN 1

    /* oSubAction */
    #define ENEMY_LAKITU_SUB_ACT_NO_SPINY 0
    #define ENEMY_LAKITU_SUB_ACT_HOLD_SPINY 1
    #define ENEMY_LAKITU_SUB_ACT_THROW_SPINY 2

/* Cloud */
    /* oAction */
    #define CLOUD_ACT_SPAWN_PARTS 0
    #define CLOUD_ACT_MAIN 1
    #define CLOUD_ACT_UNLOAD 2
    #define CLOUD_ACT_FWOOSH_HIDDEN 3

    /* oBehParams2ndByte */
    #define CLOUD_BP_FWOOSH 0
    #define CLOUD_BP_LAKITU_CLOUD 1

/* Camera lakitu */
    /* oAction */
    #define CAMERA_LAKITU_INTRO_ACT_TRIGGER_CUTSCENE 0
    #define CAMERA_LAKITU_INTRO_ACT_SPAWN_CLOUD 1
    #define CAMERA_LAKITU_INTRO_ACT_UNK2 2

    /* oBehParams2ndByte */
    #define CAMERA_LAKITU_BP_FOLLOW_CAMERA 0
    #define CAMERA_LAKITU_BP_INTRO 1

/* Monty mole */
    /* oAction */
    #define MONTY_MOLE_ACT_SELECT_HOLE 0
    #define MONTY_MOLE_ACT_RISE_FROM_HOLE 1
    #define MONTY_MOLE_ACT_SPAWN_ROCK 2
    #define MONTY_MOLE_ACT_BEGIN_JUMP_INTO_HOLE 3
    #define MONTY_MOLE_ACT_THROW_ROCK 4
    #define MONTY_MOLE_ACT_JUMP_INTO_HOLE 5
    #define MONTY_MOLE_ACT_HIDE 6
    #define MONTY_MOLE_ACT_JUMP_OUT_OF_HOLE 7

    /* oBehParams2ndByte */
    #define MONTY_MOLE_BP_NO_ROCK 0

/* Monty mole rock */
    /* oAction */
    #define MONTY_MOLE_ROCK_ACT_HELD 0
    #define MONTY_MOLE_ROCK_ACT_MOVE 1

/* Platform on track */
    /* oAction */
    #define PLATFORM_ON_TRACK_ACT_INIT 0
    #define PLATFORM_ON_TRACK_ACT_WAIT_FOR_MARIO 1
    #define PLATFORM_ON_TRACK_ACT_MOVE_ALONG_TRACK 2
    #define PLATFORM_ON_TRACK_ACT_PAUSE_BRIEFLY 3
    #define PLATFORM_ON_TRACK_ACT_FALL 4

    /* oBehParams >> 16 */
    #define PLATFORM_ON_TRACK_BP_MASK_PATH 0xF
    #define PLATFORM_ON_TRACK_BP_MASK_TYPE (0x7 << 4)
    #define PLATFORM_ON_TRACK_BP_RETURN_TO_START (1 << 8)
    #define PLATFORM_ON_TRACK_BP_DONT_DISAPPEAR (1 << 9)
    #define PLATFORM_ON_TRACK_BP_DONT_TURN_YAW (1 << 10)
    #define PLATFORM_ON_TRACK_BP_DONT_TURN_ROLL (1 << 11)

    #define PLATFORM_ON_TRACK_TYPE_CARPET    0
    #define PLATFORM_ON_TRACK_TYPE_SKI_LIFT  1
    #define PLATFORM_ON_TRACK_TYPE_CHECKERED 2
    #define PLATFORM_ON_TRACK_TYPE_GRATE     3

/* Water bomb */
    /* oAction */
    #define WATER_BOMB_ACT_SHOT_FROM_CANNON 0
    #define WATER_BOMB_ACT_INIT 1
    #define WATER_BOMB_ACT_DROP 2
    #define WATER_BOMB_ACT_EXPLODE 3

/* TTC rotating solid */
    /* oBehParams2ndByte */
    #define TTC_ROTATING_SOLID_BP_CUBE 0
    #define TTC_ROTATING_SOLID_BP_TRIANGULAR_PRISM 1

/* TTC moving bar */
    /* oAction */
    #define TTC_MOVING_BAR_ACT_WAIT 0
    #define TTC_MOVING_BAR_ACT_PULL_BACK 1
    #define TTC_MOVING_BAR_ACT_EXTEND 2
    #define TTC_MOVING_BAR_ACT_RETRACT 3

/* TTC cog */
    /* oBehParams2ndByte */
    #define TTC_COG_BP_SHAPE_MASK 0x00000002
    #define TTC_COG_BP_SHAPE_HEXAGON (0 << 1)
    #define TTC_COG_BP_SHAPE_TRIANGLE (1 << 1)
    #define TTC_COG_BP_DIR_MASK 0x00000001
    #define TTC_COG_BP_DIR_CCW (0 << 0) // TODO: Check these
    #define TTC_COG_BP_DIR_CW (1 << 0)

/* TTC 2D Rotator */
    /* oBehParams2ndByte */
    #define TTC_2D_ROTATOR_BP_HAND 0
    #define TTC_2D_ROTATOR_BP_2D_COG 1

/* Dorrie */
    /* oAction */
    #define DORRIE_ACT_MOVE 0
    #define DORRIE_ACT_LOWER_HEAD 1
    #define DORRIE_ACT_RAISE_HEAD 2

/* Mad piano */
    /* oAction */
    #define MAD_PIANO_ACT_WAIT 0
    #define MAD_PIANO_ACT_ATTACK 1

/* Fire piranha plant */
    /* oAction */
    #define FIRE_PIRANHA_PLANT_ACT_HIDE 0
    #define FIRE_PIRANHA_PLANT_ACT_GROW 1

/* Fire spitter */
    /* oAction */
    #define FIRE_SPITTER_ACT_IDLE 0
    #define FIRE_SPITTER_ACT_SPIT_FIRE 1

/* Eyerok boss */
    /* oAction */
    #define EYEROK_BOSS_ACT_SLEEP 0
    #define EYEROK_BOSS_ACT_WAKE_UP 1
    #define EYEROK_BOSS_ACT_SHOW_INTRO_TEXT 2
    #define EYEROK_BOSS_ACT_FIGHT 3
    #define EYEROK_BOSS_ACT_DIE 4

/* Eyerok hand */
    /* oAction */
    #define EYEROK_HAND_ACT_SLEEP 0
    #define EYEROK_HAND_ACT_IDLE 1
    #define EYEROK_HAND_ACT_OPEN 2
    #define EYEROK_HAND_ACT_SHOW_EYE 3
    #define EYEROK_HAND_ACT_CLOSE 4
    #define EYEROK_HAND_ACT_RETREAT 5
    #define EYEROK_HAND_ACT_TARGET_MARIO 6
    #define EYEROK_HAND_ACT_SMASH 7
    #define EYEROK_HAND_ACT_FIST_PUSH 8
    #define EYEROK_HAND_ACT_FIST_SWEEP 9
    #define EYEROK_HAND_ACT_BEGIN_DOUBLE_POUND 10 // raising for double smash
    #define EYEROK_HAND_ACT_DOUBLE_POUND 11 // double smashing
    #define EYEROK_HAND_ACT_ATTACKED 12
    #define EYEROK_HAND_ACT_RECOVER 13
    #define EYEROK_HAND_ACT_BECOME_ACTIVE 14
    #define EYEROK_HAND_ACT_DIE 15

/* Klepto */
    /* oAction */
    #define KLEPTO_ACT_CIRCLE_TARGET_HOLDING 0
    #define KLEPTO_ACT_APPROACH_TARGET_HOLDING 1
    #define KLEPTO_ACT_WAIT_FOR_MARIO 2
    #define KLEPTO_ACT_TURN_TOWARD_MARIO 3
    #define KLEPTO_ACT_DIVE_AT_MARIO 4
    #define KLEPTO_ACT_RESET_POSITION 5
    #define KLEPTO_ACT_STRUCK_BY_MARIO 6
    #define KLEPTO_ACT_RETREAT 7

    /* oAnimState */
    #define KLEPTO_ANIM_STATE_HOLDING_NOTHING 0
    #define KLEPTO_ANIM_STATE_HOLDING_CAP 1
    #define KLEPTO_ANIM_STATE_HOLDING_STAR 2

/* Bird */
    /* oAction */
    #define BIRD_ACT_INIT 0
    #define BIRD_ACT_FLY 1

    /* oBehParams2ndByte */
    #define BIRD_BP_SPAWNED 0
    #define BIRD_BP_SPAWNER 1

/* Racing penguin */
    /* oAction */
    #define RACING_PENGUIN_ACT_WAIT_FOR_MARIO 0
    #define RACING_PENGUIN_ACT_SHOW_INIT_TEXT 1
    #define RACING_PENGUIN_ACT_PREPARE_FOR_RACE 2
    #define RACING_PENGUIN_ACT_RACE 3
    #define RACING_PENGUIN_ACT_FINISH_RACE 4
    #define RACING_PENGUIN_ACT_SHOW_FINAL_TEXT 5

/* Skeeter */
    /* oAction */
    #define SKEETER_ACT_IDLE 0
    #define SKEETER_ACT_LUNGE 1
    #define SKEETER_ACT_WALK 2

/* Triplet butterfly */
    /* oAction */
    #define TRIPLET_BUTTERFLY_ACT_INIT 0
    #define TRIPLET_BUTTERFLY_ACT_WANDER 1
    #define TRIPLET_BUTTERFLY_ACT_ACTIVATE 2
    #define TRIPLET_BUTTERFLY_ACT_EXPLODE 3

    /* oBehParams2ndByte */
    #define TRIPLET_BUTTERFLY_BP_BUTTERFLY_NUM 0x00000003
    #define TRIPLET_BUTTERFLY_BP_NO_BOMBS 0x00000004

    /* oTripletButterflyType */
    #define TRIPLET_BUTTERFLY_TYPE_NORMAL -1
    #define TRIPLET_BUTTERFLY_TYPE_EXPLODES 0
    #define TRIPLET_BUTTERFLY_TYPE_SPAWN_1UP 1

/* Water level diamond */
    /* oAction */
    // Loading
    #define WATER_LEVEL_DIAMOND_ACT_INIT 0
    // Idling when Mario isn't inside its hitbox
    #define WATER_LEVEL_DIAMOND_ACT_IDLE 1
    // While the water level is changing
    #define WATER_LEVEL_DIAMOND_ACT_CHANGE_WATER_LEVEL 2
    // After the water level has changed but Mario hasn't left its hitbox yet
    #define WATER_LEVEL_DIAMOND_ACT_IDLE_SPINNING 3

#endif
