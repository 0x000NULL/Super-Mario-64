// bowser_puzzle_piece.c.inc

void func_802BDC60(s16 a0,void* a1,f32 a2,f32 a3,s8 a4,s8* a5)
{
    struct Object* sp1C = spawn_object(o,a0,a1); // bowser_puzzle_piece
    sp1C->oPosX += a2;
    sp1C->oPosY += 50.0f;
    sp1C->oPosZ += a3;
    sp1C->oAction = a4;
    sp1C->oBowserPuzzlePieceUnk10C = a5;
    sp1C->oBowserPuzzlePieceUnk110 = a5;
}

void func_802BDD0C(f32 a0)
{
    s32 i;
    for(i=0;i<14;i++)
        func_802BDC60(D_8032FB4C[i].unk0,bhvLllBowserPuzzlePiece,D_8032FB4C[i].unk1*a0/10.0f,D_8032FB4C[i].unk2*a0/10.0f,D_8032FB4C[i].unk3,D_8032FB4C[i].unk4);
    o->oAction++;
}

void bhv_lll_bowser_puzzle_loop(void)
{
    s32 i;
    UNUSED struct Object* sp28;
    switch(o->oAction)
    {
    case 0:
        func_802BDD0C(480.0f);
        break;
    case 1:
        if(o->oBowserPuzzleUnkF4 == 3 && o->oDistanceToMario < 1000.0f)
        {
            for(i=0;i<5;i++)
                sp28 = spawn_object(o,MODEL_YELLOW_COIN,bhvSingleCoinGetsSpawned);
            o->oBowserPuzzleUnkF4 = 0;
            o->oAction++;
        }
        break;
    case 2:
        break;
    }
}

void ActionBowserPuzzlePiece0(void) {}

void ActionBowserPuzzlePiece1(void)
{
    o->oPosY += 50.0f;
    o->oAction = 3;
}

void func_802BDF2C(void)
{
    s8* sp1C = o->oBowserPuzzlePieceUnk110;
    if(gMarioObject->platform == o)
        o->parentObj->oBowserPuzzleUnkF4 = 1;
    if(o->oBowserPuzzlePieceUnk108 == 0)
    {
        obj_change_action(*sp1C);
        sp1C++;
        o->oBowserPuzzlePieceUnk110 = sp1C;
        if(*sp1C == -1)
        {
            o->parentObj->oBowserPuzzleUnkF4 |= 2;
            o->oBowserPuzzlePieceUnk110 = o->oBowserPuzzlePieceUnk10C;
        }
        o->oBowserPuzzlePieceUnk108 = 1;
    }
}

void func_802BE014(f32 a0,f32 a1,s32 a2,UNUSED s32 a3)
{
    if(o->oTimer < 20)
    {
        if(o->oTimer%2)
            o->oBowserPuzzlePieceUnk100 = 0.0f;
        else
            o->oBowserPuzzlePieceUnk100 = -6.0f;
    }
    else
    {
        if(o->oTimer == 20)
            PlaySound2(SOUND_CH9_UNK19);
        if(o->oTimer < a2 + 20)
        {
            o->oBowserPuzzlePieceUnkFC += a0;
            o->oBowserPuzzlePieceUnk104 += a1;
        }
        else
        {
            o->oAction = 2;
            o->oBowserPuzzlePieceUnk108 = 0;
        }
    }
}

void ActionBowserPuzzlePiece2(void)
{
    UNUSED s32 sp4;
    if(o->oTimer < 24)
        sp4 = 0;
    else
        o->oBowserPuzzlePieceUnk108 = 0;
}

void ActionBowserPuzzlePiece3(void)
{
    func_802BE014(-120.0f,0.0f,4,4);
}

void ActionBowserPuzzlePiece4(void)
{
    func_802BE014(120.0f,0.0f,4,5);
}

void ActionBowserPuzzlePiece5(void)
{
    func_802BE014(0.0f,-120.0f,4,6);
}

void ActionBowserPuzzlePiece6(void)
{
    func_802BE014(0.0f,120.0f,4,3);
}

void (*TableBowserPuzzlePieceActions[])(void) = {ActionBowserPuzzlePiece0,ActionBowserPuzzlePiece1,ActionBowserPuzzlePiece2,ActionBowserPuzzlePiece3,ActionBowserPuzzlePiece4,ActionBowserPuzzlePiece5,ActionBowserPuzzlePiece6};

void bhv_lll_bowser_puzzle_piece_loop(void)
{
    func_802BDF2C();
    obj_call_action_function(TableBowserPuzzlePieceActions);
    o->oPosX = o->oBowserPuzzlePieceUnkFC + o->oHomeX;
    o->oPosY = o->oBowserPuzzlePieceUnk100 + o->oHomeY;
    o->oPosZ = o->oBowserPuzzlePieceUnk104 + o->oHomeZ;
}
