// pyramid_elevator.c.inc

void bhv_pyramid_elevator_init(void) {
    s32 sp1C;
    struct Object *sp18;

    for (sp1C = 0; sp1C < 10; sp1C++)
    {
        sp18 = spawn_object(o, MODEL_BOWLING_BALL_2, bhvPyramidElevatorMetalBalls);
        sp18->oPosY = 4600 - sp1C * 460;
    }
}

void bhv_pyramid_elevator_loop(void) {
    switch (o->oAction) {
        case 0:
            if (gMarioObject->platform == o)
                o->oAction = 1;
            break;

        case 1:
            o->oPosY = o->oHomeY - sins(o->oTimer * 0x1000) * 10.0f;
            if (o->oTimer == 8)
                o->oAction = 2;
            break;

        case 2:
            o->oVelY = -10.0f;
            o->oPosY += o->oVelY;
            if (o->oPosY < 128.0f) {
                o->oPosY = 128.0f;
                o->oAction = 3;
            }
            break;

        case 3:
            o->oPosY = sins(o->oTimer * 0x1000) * 10.0f + 128.0f;
            if (o->oTimer >= 8) {
                o->oVelY = 0;
                o->oPosY = 128.0f;
            }
            break;
    }
}

void bhv_pyramid_elevator_metal_balls_loop(void) {
    struct Object *sp1C;

    obj_scale(0.15f);
    sp1C = obj_nearest_object_with_behavior(bhvPyramidElevator);
    
    if (sp1C != NULL) {
        if (sp1C->oAction != 0)
            o->activeFlags = 0;
    }
}
