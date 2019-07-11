// pole.c.inc

void bhv_pole_init(void)
{
    s32 sp04;
    sp04 = o->oBehParams >> 0x10 & 0xFF;
    o->hitboxHeight = (f32)(sp04 * 10);
}

void bhv_giant_pole_loop(void)
{
    struct Object* topBall;
    if(o->oTimer == 0)
    {
        topBall = spawn_object(o,MODEL_YELLOW_SPHERE,bhvYellowBall);
        topBall->oPosY += o->hitboxHeight + 50.0f;
    }
    BehClimbDetectLoop();
}
