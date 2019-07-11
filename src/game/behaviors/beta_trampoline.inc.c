// beta_trampoline.c.inc

void bhv_beta_trampoline_spawn_loop(void)
{
    f32 yScale;
    f32 yDisplacement;
    copy_object_pos_and_angle(o,o->parentObj);
    copy_object_graph_y_offset(o,o->parentObj);
    o->oPosY -= 75.0f;
    if((yDisplacement = o->oPosY - o->oHomeY) >= 0)
        yScale = yDisplacement/10.0 + 1.0;
    else
    {
        yDisplacement = -yDisplacement;
        yScale = 1.0 - yDisplacement/500.0;
    }
    scale_object_xyz(o,1.0f,yScale,1.0f);
}

void bhv_beta_trampoline_loop(void)
{
    struct Object* sp1C;
    obj_set_model(MODEL_TRAMPOLINE);
    if(o->oTimer == 0)
    {
        sp1C = spawn_object(o,MODEL_TRAMPOLINE_CENTER,bhvBetaTrampolineSpawn);
        sp1C->oPosY -= 75.0f;
        sp1C = spawn_object(o,MODEL_TRAMPOLINE_BASE,bhvStaticObject);
        sp1C->oPosY -= 150.0f;
    }
    if(gMarioObject->platform == o)
        o->oBetaTrampolineUnk110 = 1;
    else
    {
        o->oBetaTrampolineUnk110 = 0;
        o->oPosY = o->oHomeY;
    }
    nop_80254E50();
}
