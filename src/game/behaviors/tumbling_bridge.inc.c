// tumbling_bridge.c.inc

extern u8 wf_seg7_collision_tumbling_bridge[];
extern u8 bbh_seg7_collision_07026B1C[];
extern u8 lll_seg7_collision_0701D21C[];
extern u8 bitfs_seg7_collision_07015288[];

struct Struct8032F34C TableTumblingBridgeParams[] = {
    {9, -512, 0x80, MODEL_WF_TUMBLING_BRIDGE_PART, wf_seg7_collision_tumbling_bridge},
    {9, -412, 103, MODEL_BBH_TUMBLING_PLATFORM_PART, bbh_seg7_collision_07026B1C},
    {9, -512, 0x80, MODEL_LLL_FALLING_PLATFORM, lll_seg7_collision_0701D21C},
    {9, -512, 0x80, MODEL_BITFS_TUMBLING_PLATFORM_PART, bitfs_seg7_collision_07015288}
};

void bhv_tumbling_bridge_platform_loop(void)
{
    switch(o->oAction)
    {
    case 0:
        if(gMarioObject->platform == o)
        {
            o->oAction++;
            o->OBJECT_FIELD_S32(0x1B) = RandomSign() * 0x80;
        }
        break;
    case 1:
        obj_update_floor_height();
        if(o->oTimer > 5)
        {
            o->oAction++;
            PlaySound2(SOUND_GENERAL_PLATFORM);
        }
        break;
    case 2:
        if(o->oAngleVelPitch < 0x400)
            o->oAngleVelPitch += 0x80;
        if(o->oAngleVelRoll > -0x400 && o->oAngleVelRoll < 0x400)
            o->oAngleVelRoll+=o->OBJECT_FIELD_S32(0x1B); // acceleraration?
        o->oGravity = -3.0f;
        obj_rotate_face_angle_using_vel();
        obj_move_using_fvel_and_gravity();
        if(o->oPosY < o->oFloorHeight - 300.0f)
            o->oAction++;
        break;
    case 3:
        break;
    }
    if(o->parentObj->oAction == 3)
        mark_object_for_deletion(o);
}

void ActionTumblingBridge1(void)
{
    struct Object *platformObj;
    s32 i;
    s32 bridgeID = o->oBehParams2ndByte;
    s32 relativePlatformX;
    s32 relativePlatformZ;
    s32 relativePlatformY = 0;
    s32 relativeInitialPlatformY = 0;
    
    for (i = 0; i < TableTumblingBridgeParams[bridgeID].numBridgeSections; i++)
    {
        relativePlatformX = 0;
        relativePlatformZ = 0;
        
        if (bridgeID == 3)
            relativePlatformX =
                TableTumblingBridgeParams[bridgeID].bridgeRelativeStartingXorZ +
                TableTumblingBridgeParams[bridgeID].platformWidth * i;
        else
            relativePlatformZ =
                TableTumblingBridgeParams[bridgeID].bridgeRelativeStartingXorZ +
                TableTumblingBridgeParams[bridgeID].platformWidth * i;
        
        if (obj_has_behavior(bhvLllTumblingBridge))
        {
            if (i % 3 == 0)
                relativePlatformY -= 150;
            relativeInitialPlatformY = 450;
        }
        
        platformObj = spawn_object_relative(
            0,
            relativePlatformX,
            relativePlatformY + relativeInitialPlatformY,
            relativePlatformZ,
            o,
            TableTumblingBridgeParams[bridgeID].model,
            bhvTumblingBridgePlatform
        );
        
        set_object_collision_data(platformObj, TableTumblingBridgeParams[bridgeID].segAddr);
    }
    
    o->oAction = 2;
}

void ActionTumblingBridge2(void)
{
    obj_hide();
    if(obj_has_behavior(bhvLllTumblingBridge))
        obj_unhide();
    else if(o->oDistanceToMario > 1200.0f)
    {
        o->oAction = 3;
        obj_unhide();
    }
}

void ActionTumblingBridge3(void)
{
    obj_unhide();
    o->oAction = 0;
}

void ActionTumblingBridge0(void)
{
    if(obj_has_behavior(bhvLllTumblingBridge) || o->oDistanceToMario < 1000.0f)
        o->oAction = 1;
}

void (*TableTumblingBridgeActions[])(void) = {ActionTumblingBridge0,ActionTumblingBridge1,ActionTumblingBridge2,ActionTumblingBridge3};

s16 D_8032F38C[] = {-51,0,0,-461,0,0,-512,0,0,-2611,0,0,-2360,0,0,214,0,0,-50,1945,1,0};

void bhv_tumbling_bridge_loop(void)
{
    obj_call_action_function(TableTumblingBridgeActions);
}
