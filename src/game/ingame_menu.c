#include <ultra64.h>

#include "sm64.h"
#include "memory.h"
#include "types.h"
#include "audio/interface_2.h"
#include "game.h"
#include "save_file.h"
#include "level_update.h"
#include "camera.h"
#include "text_strings.h"
#include "segment2.h"
#include "segment7.h"
#include "ingame_menu.h"
#include "engine/math_util.h"

extern Gfx *gDisplayListHead;
extern s32 gGlobalTimer;
extern s16 gCurrCourseNum;
extern s16 gCurrSaveFileNum;

extern Gfx coin_seg3_dl_03007940[];
extern Gfx coin_seg3_dl_03007968[];
extern Gfx coin_seg3_dl_03007990[];
extern Gfx coin_seg3_dl_030079B8[];

extern u8 main_menu_seg7_table_0700ABD0[];
extern Gfx castle_grounds_seg7_dl_0700EA58[];

u16 D_80360080;
s8 gLastDialogLineNum;
s32 D_80360084;
u16 D_80360088;
s16 gCutsceneMsgXOffset;
s16 gCutsceneMsgYOffset;
s8 D_8036008E;

extern u8 gLastCompletedCourseNum;
extern u8 gLastCompletedStarNum;

extern s32 select_or_activate_mario_cam(s32);

enum DiagBoxState {
    DIAG_STATE_OPENING,
    DIAG_STATE_WAITBUTTON,
    DIAG_STATE_SCROLLING,
    DIAG_STATE_CLOSING
};

enum DiagBoxType {
    DIAG_TYPE_ROTATE, // used in NPCs and level messages
    DIAG_TYPE_ZOOM    // used in signposts and wall signs and etc
};

#define DEFAULT_DIAGBOX_ANGLE      90.0f
#define DEFAULT_DIAGBOX_SCALE      19.0f

#ifndef VERSION_JP
u8 D_U_80331370[256] = { // TODO: Is there a way to auto generate this?
    7,  7,  7,  7,  7,  7,  7,  7,  7,  7,  6,  6,  6,  6,  6,  6,
    6,  6,  5,  6,  6,  5,  8,  8,  6,  6,  6,  6,  6,  5,  6,  6,
    8,  7,  6,  6,  6,  5,  5,  6,  5,  5,  6,  5,  4,  5,  5,  3,
    7,  5,  5,  5,  6,  5,  5,  5,  5,  5,  7,  7,  5,  5,  4,  4,
    8,  6,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    8,  8,  8,  8,  7,  7,  6,  7,  7,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  4,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  6,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    7,  5, 10,  5,  9,  8,  4,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  5,  7,  7,  6,  6,  8,  0,  8, 10,  6,  4, 10,  0,  0
};
#endif

s8 gDiagBoxState          = DIAG_STATE_OPENING;
f32 gDiagBoxOpenTimer     = DEFAULT_DIAGBOX_ANGLE;
f32 gDiagBoxScale         = DEFAULT_DIAGBOX_SCALE;
s16 D_8033041C = 0;
s8 gDiagBoxType           = DIAG_TYPE_ROTATE;
s16 gDialogID             = -1;
s16 gLastDialogPageStrPos = 0;
s16 D_8033042C = 0;
s8 D_80330430 = 1;
s8 D_80330434 = 0;
u8 D_80330438 = 0;
u8 D_8033043C = 0;
s32 gDialogueResponse = 0;

#ifdef VERSION_JP
#define MAX_STRING_WIDTH 18
#else
#define MAX_STRING_WIDTH 16
#endif

// dl_add_new_identity_matrix?
void func_802D6440(void)
{
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if(matrix == NULL)
        return;

    matrix->m[0][0] = 0x00010000;
    matrix->m[1][0] = 0x00000000;
    matrix->m[2][0] = 0x00000000;
    matrix->m[3][0] = 0x00000000;
    
    matrix->m[0][1] = 0x00000000;
    matrix->m[1][1] = 0x00010000;
    matrix->m[2][1] = 0x00000000;
    matrix->m[3][1] = 0x00000000;
    
    matrix->m[0][2] = 0x00000001;
    matrix->m[1][2] = 0x00000000;
    matrix->m[2][2] = 0x00000000;
    matrix->m[3][2] = 0x00000000;
    
    matrix->m[0][3] = 0x00000000;
    matrix->m[1][3] = 0x00000001;
    matrix->m[2][3] = 0x00000000;
    matrix->m[3][3] = 0x00000000;

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_LOAD);
    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_LOAD | G_MTX_PROJECTION);
}

void dl_add_new_translation_matrix(s8 pushOp, f32 x, f32 y, f32 z)
{
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if(matrix == NULL)
        return;

    guTranslate(matrix, x, y, z);

    if(pushOp == MENU_MTX_PUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PUSH);

    if(pushOp == MENU_MTX_NOPUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_NOPUSH);
}

void dl_add_new_rotation_matrix(s8 pushOp, f32 a, f32 x, f32 y, f32 z)
{
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if(matrix == NULL)
        return;

    guRotate(matrix, a, x, y, z);

    if(pushOp == MENU_MTX_PUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PUSH);

    if(pushOp == MENU_MTX_NOPUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_NOPUSH);
}

void dl_add_new_scale_matrix(s8 pushOp, f32 x, f32 y, f32 z)
{
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if(matrix == NULL)
        return;

    guScale(matrix, x, y, z);

    if(pushOp == MENU_MTX_PUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PUSH);

    if(pushOp == MENU_MTX_NOPUSH)
        gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_NOPUSH);
}

void dl_add_new_ortho_matrix(void)
{
    Mtx *matrix = (Mtx *) alloc_display_list(sizeof(Mtx));

    if(matrix == NULL)
        return;

    func_802D6440();

    guOrtho(matrix, 0.0f, 320.0f, 0.0f, 240.0f, -10.0f, 10.0f, 1.0f); 

    // Should produce G_RDPHALF_1 in Fast3D
    gSPPerspNormalize((Gfx *)(gDisplayListHead++), 0x0000FFFF);

    gSPMatrix(gDisplayListHead++, VIRTUAL_TO_PHYSICAL(matrix), G_MTX_PROJECTION)
}

// dl_add_new_ia8_tex_from_i1
u8* func_802D69A0(u16* in, s16 width, s16 height)
{
    s32 inPos;
    u16 bitMask;
    u8 *out;
    s16 outPos;
    
    outPos = 0;
    out = alloc_display_list(width * height);

    if(out == NULL)
        return NULL;

    for(inPos = 0; inPos < (width * height) / 16; inPos++)
    {
        bitMask = 0x8000;

        while(bitMask != 0)
        {
            if(in[inPos] & bitMask)
                out[outPos] = 0xFF;
            else
                out[outPos] = 0x00;

            bitMask /= 2;
            outPos++;
        }
    }

    return out;
}

// render_generic_char
void func_802D6AFC(u8 c)
{
    void **smallFontLUT;
    void *packedTexture;
#ifdef VERSION_JP
    void *unpackedTexture;
#endif

    smallFontLUT = segmented_to_virtual(seg2_small_font_lut);
    packedTexture = segmented_to_virtual(smallFontLUT[c]);

#ifdef VERSION_JP
    unpackedTexture = func_802D69A0(packedTexture, 8, 16);
    
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, VIRTUAL_TO_PHYSICAL(unpackedTexture));
#else
    gDPPipeSync(gDisplayListHead++);
    gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_16b, 1, VIRTUAL_TO_PHYSICAL(packedTexture));
#endif
    gSPDisplayList(gDisplayListHead++, dl_ia8_render_char);
}

#if defined(VERSION_US) || defined(VERSION_EU)
struct MultiTextEntry {
    u8 length;
    u8 str[4];
};

#define TEXT_THE_RAW 0x37, 0x2B, 0x28, 0x00
#define TEXT_YOU_RAW 0x3C, 0x32, 0x38, 0x00

enum MutliStringIDs {
    STRING_THE,
    STRING_YOU
};

/*
 * Place the multi-text string according to the ID passed. (US only)
 * 0: 'the'
 * 1: 'you'
 */
void put_multi_text_string(s8 multiTextID) // US: 802D76C8
{
    s8 i;
    struct MultiTextEntry textLengths[2] = {
        {3, {TEXT_THE_RAW}},
        {3, {TEXT_YOU_RAW}},
    };

    for (i = 0; i < textLengths[multiTextID].length; i++)
    {
        func_802D6AFC(textLengths[multiTextID].str[i]);
        dl_add_new_translation_matrix(2, (f32)(D_U_80331370[textLengths[multiTextID].str[i]]), 0.0f, 0.0f);
    }
}
#endif

void PrintGenericText(s16 x, s16 y, const u8 *str)
{
    s8 mark = DLG_MARK_NONE;
    s32 strPos = 0;
    u8 lineNum = 1;

    // create_new_translation_matrix
    dl_add_new_translation_matrix(MENU_MTX_PUSH, x, y, 0.0f);

    while(str[strPos] != 0xFF)
    {
        switch(str[strPos])
        {
        case 0xF0: // kana with dakuten
            mark = DLG_MARK_DAKUTEN;
            break;
        case 0xF1: // kana with handakuten
            mark = DLG_MARK_HANDAKUTEN;
            break;
        case 0xFE: // newline
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            dl_add_new_translation_matrix(MENU_MTX_PUSH, x, y - (lineNum * MAX_STRING_WIDTH), 0.0f);
            lineNum++;
            break;
        case 0x6E: // handakuten mark
            dl_add_new_translation_matrix(MENU_MTX_PUSH, -2.0f, -5.0f, 0.0f);
            func_802D6AFC(0xF1);
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            break;
#ifdef VERSION_US
        case 0xD0: // '/'
            dl_add_new_translation_matrix(MENU_MTX_NOPUSH, (f32)(D_U_80331370[0x9E] * 2), 0.0f, 0.0f);
            break;
        case 0xD1: // 'the'
            put_multi_text_string(STRING_THE);
            break;
        case 0xD2: // 'you'
            put_multi_text_string(STRING_YOU);
            break;
#endif
        case 0x9E: // space
#ifdef VERSION_JP
            dl_add_new_translation_matrix(MENU_MTX_NOPUSH, 5.0f, 0.0f, 0.0f);
            break;
#else
            dl_add_new_translation_matrix(MENU_MTX_NOPUSH, (f32)(D_U_80331370[0x9E]), 0.0f, 0.0f);
#endif
            break; // ? needed to match
        default:
            func_802D6AFC(str[strPos]);

            if(mark != DLG_MARK_NONE)
            {
                dl_add_new_translation_matrix(MENU_MTX_PUSH, 5.0f, 5.0f, 0.0f);
                func_802D6AFC(mark + 0xEF);
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                mark = DLG_MARK_NONE;
            }

#ifdef VERSION_JP
            dl_add_new_translation_matrix(MENU_MTX_NOPUSH, 10.0f, 0.0f, 0.0f);
#else
            dl_add_new_translation_matrix(MENU_MTX_NOPUSH, (f32)(D_U_80331370[str[strPos]]), 0.0f, 0.0f);
            break; // what an odd difference. US added a useless break here.
#endif
        }

        strPos++;
    }

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

// put_hud_menu_string
void PutString(s8 font, s16 x, s16 y, const u8 *str)
{
    s32 strPos = 0;
    void **fontLUT1 = segmented_to_virtual(main_menu_seg7_table_0700ABD0); // japanese color font
    void **fontLUT2 = segmented_to_virtual(seg2_hud_lut);                  // 0-9 A-Z Alphanumeric Font
    s32 curX = x;
    s32 curY = y;

    s32 xStride;

    if(font == 1)
        xStride = 16;
    else
        xStride = JP_US_DEF(14 , 12);

    while(str[strPos] != 0xFF)
    {
#ifdef VERSION_US
        if(str[strPos] == 0x9E)
        {
            if(0) //! dead code
            {
            }
            curX += 8;
            ; //! useless statement
        }
        else
        {
#endif
            gDPPipeSync(gDisplayListHead++);
            
            if(font == 1)
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT1[str[strPos]]);
            
            if(font == 2)
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT2[str[strPos]]);
            
            gSPDisplayList(gDisplayListHead++, dl_rgba16_load_tex_block);
            gSPTextureRectangle(gDisplayListHead++, curX<<2, curY<<2,
                (curX+16)<<2, (curY+16)<<2, 0, 0, 0, 0x400, 0x400);
            
            curX += xStride;
#ifdef VERSION_US
        }
#endif
        strPos++;
    }
}

void PrintRegularText(s16 x, s16 y, const u8 *str)
{
    s8 mark = DLG_MARK_NONE;
    s32 strPos = 0;
    s32 curX = x;
    s32 curY = y;
    void **fontLUT = segmented_to_virtual(main_menu_seg7_table_0700CD08);

    while(str[strPos] != 0xFF)
    {
        switch(str[strPos])
        {
        case 0xF0:
            mark = DLG_MARK_DAKUTEN;
            break;
        case 0xF1:
            mark = DLG_MARK_HANDAKUTEN;
            break;
        case 0x9E:
            curX += 4;
            break;
        default:
            gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[str[strPos]]);
            gDPLoadSync(gDisplayListHead++);
            gDPLoadBlock(gDisplayListHead++, 7, 0, 0, (0x40-1), 0x800);
            gSPTextureRectangle(gDisplayListHead++, curX<<2, curY<<2,
                (curX+8)<<2, (curY+8)<<2, 0, 0, 0, 0x400, 0x400);

            if(mark != DLG_MARK_NONE)
            {
                gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_IA, G_IM_SIZ_8b, 1, fontLUT[mark + 0xEF]);
                gDPLoadSync(gDisplayListHead++);
                gDPLoadBlock(gDisplayListHead++, 7, 0, 0, (0x40-1), 0x800);
                gSPTextureRectangle(gDisplayListHead++, (curX+6)<<2, (curY-7)<<2,
                    (curX+14)<<2, (curY+1)<<2, 0, 0, 0, 0x400, 0x400);

                mark = DLG_MARK_NONE;
            }
#ifdef VERSION_JP
            curX += 9;
#else
            curX += D_U_80331370[str[strPos]];
#endif
        }
        strPos++;
    }
}

void PutMiniString(s16 x, s16 y, const u8 *str)
{
    s32 strPos = 0;
    void **fontLUT = segmented_to_virtual(seg2_credits_font_lut);
    s32 curX = x;
    s32 curY = y;

    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0);
    gDPTileSync(gDisplayListHead++)
    gDPSetTile(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 2, 0, 0, 0, 2, 3, 0, 2, 3, 0);
    gDPSetTileSize(gDisplayListHead++, 0, 0, 0, (8-1)<<2, (8-1)<<2);

    while(str[strPos] != 0xFF)
    {
        switch(str[strPos])
        {
        case 0x9E:
            curX += 4;
            break;
        default:
            gDPPipeSync(gDisplayListHead++);
            gDPSetTextureImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, 1, fontLUT[str[strPos]]);
            gDPLoadSync(gDisplayListHead++);
            gDPLoadBlock(gDisplayListHead++, 7, 0, 0, (0x40-1), 0x400);
            gSPTextureRectangle(gDisplayListHead++, curX<<2, curY<<2,
                (curX+8)<<2, (curY+8)<<2, 0, 0, 0, 0x400, 0x400);
            curX += 7;
            break;
        }
        strPos++;
    }
}

void handleMenuScrolling(s8 scrollDirection, s8 *currentIndex, s8 minIndex, s8 maxIndex)
{
    u8 sp1f = 0;

    if (scrollDirection == MENU_SCROLL_VERTICAL)
    {
        if(gPlayer3Controller->rawStickY > 60)
            sp1f++;

        if(gPlayer3Controller->rawStickY < -60)
            sp1f += 2;
    }
    else if (scrollDirection == MENU_SCROLL_HORIZONTAL)
    {
        if(gPlayer3Controller->rawStickX > 60)
            sp1f += 2;

        if(gPlayer3Controller->rawStickX < -60)
            sp1f++;
    }

    if (((sp1f ^ D_80330438) & sp1f) == 2)
    {
        if(currentIndex[0] == maxIndex) //! Probably originally a >=, but later replaced with an == and an else statement.
        {
            currentIndex[0] = maxIndex; // Leftover from the >=
        }
        else
        {
            SetSound(SOUND_MENU_CHANGESELECT, D_803320E0);
            currentIndex[0]++;
        }
    }

    if(((sp1f ^ D_80330438) & sp1f) == 1)
    {
        if(currentIndex[0] == minIndex) // Same applies to here
        {
            // empty
        }
        else
        {
            SetSound(SOUND_MENU_CHANGESELECT, D_803320E0);
            currentIndex[0]--;
        }
    }

    if(D_8033043C == 10)
    {
        D_8033043C = 8;
        D_80330438 = 0;
    }
    else
    {
        D_8033043C++;
        D_80330438 = sp1f;
    }

    if((sp1f & 3) == 0)
    {
        D_8033043C = 0;
    }
}

s16 get_str_x_pos_from_center(s16 centerPos, u8 *str, UNUSED f32 scale) // scale is unused in US
{
    s16 strPos = 0;
#ifdef VERSION_JP
    f32 charsWidth = 0.0f;
#endif
    f32 spacesWidth = 0.0f;

    while(str[strPos] != 0xFF)
    {
#ifdef VERSION_JP
        if(str[strPos] == 0x9E)
            spacesWidth += 1.0;
        else if(str[strPos] != 0xF0 && str[strPos] != 0xF1)
            charsWidth += 1.0;
        strPos++;
#else
        spacesWidth += D_U_80331370[str[strPos]];
        strPos++;
#endif
    }
    // return the x position of where the string starts as half the string's
    // length from the position of the provided center.
#ifdef VERSION_JP
    return (f32)centerPos - (scale * (charsWidth / 2.0)) - ((scale / 2.0) * (spacesWidth / 2.0));
#else
    return (s16)(centerPos - (s16)(spacesWidth / 2.0));
#endif
}

#if defined(VERSION_US) || defined(VERSION_EU)
// get_str_width
s16 func_u_802D8934(u8 *str)
{
    s16 strPos = 0;
    s16 width = 0;

    while (str[strPos] != 0xFF)
    {
        width += D_U_80331370[str[strPos]];
        strPos++;
    }
    return width;
}
#endif

u8 D_80330444[] = {TEXT_UNK33};
u8 D_80330448[] = {TEXT_UNK32};

void ShowCoins(s32 useCourseCoinScore, s8 sp27, s8 sp2b, s16 x, s16 y)
{
    u8 strNumCoins[4];
    s16 numCoins;
    
    if(!useCourseCoinScore)
        numCoins = (u16)(save_file_get_max_coin_score(sp2b) & 0xFFFF);
    else
        numCoins = save_file_get_course_coin_score(sp27, sp2b);

    if(numCoins != 0)
    {
        PutString(2, x, y, D_80330444);
        PutString(2, x + 16, y, D_80330448);
        Int2Str(numCoins, strNumCoins);
        PutString(2, x + 32, y, strNumCoins);
    }
}

void ShowStars(s8 sp2b, s8 sp2f, s16 x, s16 y)
{
    u8 strStarCount[4];
    s16 starCount;
    u8 sp20[] = {TEXT_UNK35}; // D_8033044C star symbol?
    UNUSED u16 unused;
    u8 sp1c[] = {TEXT_UNK32}; // D_80330450 x symbol? 
    
    starCount = save_file_get_course_star_count(sp2b, sp2f);

    if(starCount != 0)
    {
        PutString(2, x, y, sp20);
        PutString(2, x + 16, y, sp1c);
        Int2Str(starCount, strStarCount);
        PutString(2, x + 32, y, strStarCount);
    }
}

void Int2Str(s32 num, u8 *dst)
{
    s32 digit1;
    s32 digit2;
    s32 digit3;

    s8 pos = 0;

    if(num > 999)
    {
        dst[0] = 0x00;
        dst[1] = 0xFF;
        return;
    }

    digit1 = num / 100;
    digit2 = (num - digit1 * 100) / 10;
    digit3 = (num - digit1 * 100) - (digit2 * 10);

    if(digit1 != 0)
        dst[pos++] = digit1;

    if(digit2 != 0 || digit1 != 0)
        dst[pos++] = digit2;

    dst[pos++] = digit3;
    dst[pos] = 0xFF;
}

s16 get_dialog_id(void)
{
    return gDialogID;
}

void func_802D7F90(s16 a0)
{
    if(gDialogID == -1)
    {
        gDialogID = a0;
        gDiagBoxType = DIAG_TYPE_ROTATE;
    }
}

void func_802D7FCC(s16 a0, s32 a1)
{
    if(gDialogID == -1)
    {
        gDialogID = a0;
        D_80360084 = a1;
        gDiagBoxType = DIAG_TYPE_ROTATE;
    }
}

void CreateTextBox(s16 a0)
{
    if(gDialogID == -1)
    {
        gDialogID = a0;
        gDiagBoxType = DIAG_TYPE_ZOOM;
    }
}

void func_802D8050(s16 a0)
{
    if(gDialogID == -1)
    {
        gDialogID = a0;
        gDiagBoxType = DIAG_TYPE_ROTATE;
        D_80330434 = 1;
    }
}

// reset_dialog_state?
void func_802D8098(void)
{
    level_set_transition(0, 0);

    if(gDiagBoxType == DIAG_TYPE_ZOOM)
        stop_mario(2);

    gDiagBoxScale = 19.0f;
    gDiagBoxOpenTimer = 90.0f;
    gDiagBoxState = DIAG_STATE_OPENING;
    gDialogID = -1;
    D_8033042C = 0;
    D_80330434 = 0;
    gLastDialogPageStrPos = 0;
    gDialogueResponse = 0;
}

void func_802D8134(struct DialogEntry *diagEntry, s8 sp47)
{
    UNUSED s32 unused;

    dl_add_new_translation_matrix(2, diagEntry->leftOffset, diagEntry->width, 0);

    switch(gDiagBoxType)
    {
    case DIAG_TYPE_ROTATE:
        if(gDiagBoxState == DIAG_STATE_OPENING || gDiagBoxState == DIAG_STATE_CLOSING)
        {
            dl_add_new_scale_matrix(2, 1.0 / gDiagBoxScale, 1.0 / gDiagBoxScale, 1.0f);
            dl_add_new_rotation_matrix(2, gDiagBoxOpenTimer * 4.0f, 0, 0, 1.0f); // convert the speed into angle
        }
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 0x96);
        break;
    case DIAG_TYPE_ZOOM:
        if(gDiagBoxState == DIAG_STATE_OPENING || gDiagBoxState == DIAG_STATE_CLOSING)
        {                
            dl_add_new_translation_matrix(2, 65.0 - (65.0 / gDiagBoxScale),
                (40.0 / gDiagBoxScale) - 40.0, 0);
            dl_add_new_scale_matrix(2, 1.0 / gDiagBoxScale, 1.0 / gDiagBoxScale, 1.0f);
        }
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 0x96);
        break;
    }

    dl_add_new_translation_matrix(1, JP_US_DEF(-5.0f , -7.0f), JP_US_DEF(2.0 , 5.0), 0);
    dl_add_new_scale_matrix(2, 1.1f, ((f32)sp47 / JP_US_DEF(4.0f , 5.0f)) + 0.1, 1.0f);

    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void func_802D8450(s8 a0, s8 a1)
{
    u8 sp17;

    if(a0 == 1)
    {
        if(a1 == 1)
        {
            gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
        }
        else
        {
            if(a1 == D_80330430)
            {
                sp17 = (gSineTable[D_80360080 >> 4] * 50.0f) + 200.0f;
                gDPSetEnvColor(gDisplayListHead++, sp17, sp17, sp17, 255);
            }
            else
            {
                gDPSetEnvColor(gDisplayListHead++, 200, 200, 200, 255);
            }
        }
    }
    else
    {
        switch(gDiagBoxType)
        {
        case DIAG_TYPE_ROTATE:
            break;
        case DIAG_TYPE_ZOOM:
            gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
            break;
        }
    }
}

void func_802D8690(s8 lineNum, s8 sp27, s8 *sp28, s8 *sp2c, s16 *sp30)
{
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    if(lineNum == sp27)
    {
        sp28[0] = 1;
        return;
    }
    dl_add_new_translation_matrix(1, JP_US_DEF(5.0f , 0.0f), 2 - (lineNum * JP_US_DEF(20 , 16)), 0);
    
    sp30[0] = 0;
    sp2c[0] = 1;
}

#ifdef VERSION_JP
void func_802D875C(s8 *sp20, s16 *sp24)
{
    if(sp24[0] != 0)
        dl_add_new_translation_matrix(2, sp20[0] * 10, 0, 0);

    dl_add_new_translation_matrix(1, -2.0f, -5.0f, 0);
    func_802D6AFC(0xF1);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    sp24[0]++;
    sp20[0] = 1;
}
#endif

void func_802D8830(s8 *sp20, s16 *sp24)
{
    s8 sp1f = D_80360084 / 10;
    s8 sp1e = D_80360084 - (sp1f * 10); // remainder

    if(sp1f != 0)
    {
#ifdef VERSION_JP
        dl_add_new_translation_matrix(2, sp20[0] * 10, 0, 0);
        func_802D6AFC(sp1f);
#else
        if (sp20[0] != 1) {
            dl_add_new_translation_matrix(2, (f32)(D_U_80331370[0x9E] * sp20[0]), 0, 0);
        }

        func_802D6AFC(sp1f);
        dl_add_new_translation_matrix(2, (f32)D_U_80331370[sp1f], 0, 0);
        sp20[0] = 1;
        sp24[0]++;
#endif
    }
    else
    {
#ifdef VERSION_JP
        sp20[0]++;
#endif
    }

#ifdef VERSION_JP
    dl_add_new_translation_matrix(2, sp20[0] * 10, 0, 0);
    func_802D6AFC(sp1e);
#else
    if(sp20[0] !=  1) {
        dl_add_new_translation_matrix(2, (f32)(D_U_80331370[0x9E] * (sp20[0] - 1)), 0, 0);
    }

    func_802D6AFC(sp1e);
    dl_add_new_translation_matrix(2, (f32)D_U_80331370[sp1e], 0, 0);
#endif

    sp24[0]++;
    sp20[0] = 1;
}

#if defined(VERSION_US) || defined(VERSION_EU)
void func_u_802D9634(s8 a0, s8 a1, s16 *a2, s8 a3, s8 sp10, s8 sp14)
{
    s8 i;
    struct MultiTextEntry textLengths[2] = {
        {3, {TEXT_THE_RAW}},
        {3, {TEXT_YOU_RAW}},
    };

    if(a1 >= sp14 && a1 <= (sp14 + a3))
    {
        if (a2[0] != 0 || (sp10 != 1))
        {
            dl_add_new_translation_matrix(2, (D_U_80331370[0x9E] * (sp10 - 1)), 0, 0);
        }
        for (i = 0; i < textLengths[a0].length; i++)
        {    
            func_802D6AFC(textLengths[a0].str[i]);
            dl_add_new_translation_matrix(2, (D_U_80331370[textLengths[a0].str[i]]), 0, 0);
        }
    }
    a2 += textLengths[a0].length;
}
#endif

u32 func_802D8954(s16 a0)
{
    if(a0 < 0)
        a0 = 0;

    return a0;
}

#ifdef VERSION_JP
void func_802D8980(s8 sp63, struct DialogEntry *diagEntry)
#else
void func_802D8980(s8 sp63, struct DialogEntry *diagEntry, s8 sp5B)
#endif
{
    UNUSED s32 u0, u1; // a guess?

    u8 strChar; // sp4F;

    u8 *str = (u8 *)segmented_to_virtual(diagEntry->str); // sp48
    s8 lineNum = 1; // sp47 in US

    s8 totalLines;

    s8 sp4d_45 = 0;
    UNUSED s8 sp4c_44 = 0; // only unused in US
    s8 sp4b_43 = 1;

    s8 linesPerBox = diagEntry->linesPerBox; // sp42

    s16 strIdx;  // sp40
    s16 linePos; // sp3E

    linePos = 0;

    if(gDiagBoxState == DIAG_STATE_SCROLLING)
        totalLines = (linesPerBox * 2) + 1; // if scrolling, consider the number of lines for both the current page and the page being scrolled to.
    else
        totalLines = linesPerBox + 1;

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    strIdx = D_8033042C;

    if(gDiagBoxState == DIAG_STATE_SCROLLING)
        dl_add_new_translation_matrix(2, 0, (f32)D_8033041C, 0);

    dl_add_new_translation_matrix(1, JP_US_DEF(5.0f , 0.0f), 2 - lineNum * JP_US_DEF(20 , 16), 0);

    while(sp4d_45 == 0)
    {
        func_802D8450(sp63, lineNum);
        strChar = str[strIdx];

        switch(strChar)
        {
        case 0xFF: // terminator
            sp4d_45 = 2;
            gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
            break;
        case 0xFE: // newline
            lineNum++;
            func_802D8690(lineNum, totalLines, &sp4d_45, &sp4b_43, &linePos);
            break;
        case 0xF0: // kana with dakuten
            sp4c_44 = 1;
            break;
        case 0xF1: // kana with handakuten
            sp4c_44 = 2;
            break;
        case 0x9E: // space
#ifdef VERSION_JP
            if(linePos != 0)
#endif
                sp4b_43++;
            linePos++;
            break;
#ifdef VERSION_JP
        case 0x6E: // handakuten
            func_802D875C(&sp4b_43, &linePos);
        break;
#else
        case 0xD0: // '/'
            sp4b_43 += 2;
            linePos += 2;
            break;
        case 0xD1: // 'the'
            func_u_802D9634(0, lineNum, &linePos, linesPerBox, sp4b_43, sp5B);
            sp4b_43 = 1;
            break;
        case 0xD2: // 'you'
            func_u_802D9634(1, lineNum, &linePos, linesPerBox, sp4b_43, sp5B);
            sp4b_43 = 1;
            break;
#endif
        case 0xE0: // star variable
            func_802D8830(&sp4b_43, &linePos);
            break;
        default:   // any other character
#ifdef VERSION_JP
            if(linePos != 0)
                dl_add_new_translation_matrix(2, sp4b_43 * 10, 0, 0);

            func_802D6AFC(strChar);
            sp4b_43 = 1;
            linePos++;

            if(sp4c_44 != 0)
            {
                dl_add_new_translation_matrix(1, 5.0f, 7.0f, 0);
                func_802D6AFC(sp4c_44 + 0xEF);
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                sp4c_44 = 0;
            }
#else
            if(lineNum >= sp5B && lineNum <= (sp5B + linesPerBox))
            {
                if(linePos || sp4b_43 != 1)
                    dl_add_new_translation_matrix(2, (f32)(D_U_80331370[0x9E] * (sp4b_43 - 1)), 0, 0);

                func_802D6AFC(strChar);
                dl_add_new_translation_matrix(2, (f32)(D_U_80331370[strChar]), 0, 0);
                sp4b_43 = 1;
                linePos++;
            }
#endif
        }

#ifdef VERSION_JP
        if(linePos == 12)
        {
            if(str[strIdx+1] == 0x6E) // handakuten
            {
                func_802D875C(&sp4b_43, &linePos);
                strIdx++;
            }

            if(str[strIdx+1] == 0x6F) // comma
            {
                dl_add_new_translation_matrix(2, sp4b_43 * 10, 0, 0);
                func_802D6AFC(0x6F);
                strIdx++;
            }

            if(str[strIdx+1] == 0xFE) // newline
                strIdx++;

            if(str[strIdx+1] == 0xFF) // terminator
            {
                sp4d_45 = 2;
                gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
                break; // exit loop
            }
            else
            {
                lineNum++;
                func_802D8690(lineNum, totalLines, &sp4d_45, &sp4b_43, &linePos);
            }
        }
#endif

        strIdx++;
    }
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);

    if(gDiagBoxState == DIAG_STATE_WAITBUTTON)
    {
        if(sp4d_45 == 2)
            gLastDialogPageStrPos = -1;
        else
            gLastDialogPageStrPos = strIdx;
    }

    gLastDialogLineNum = lineNum;
}

void func_802D8ED4(void)
{
    if(gDiagBoxState == DIAG_STATE_WAITBUTTON)
        handleMenuScrolling(MENU_SCROLL_HORIZONTAL, &D_80330430, 1, 2);

    dl_add_new_translation_matrix(2, (D_80330430 * JP_US_DEF(50 , 56)) - JP_US_DEF(25 , 47), JP_US_DEF(1 , 2) - (gLastDialogLineNum * JP_US_DEF(20 , 16)), 0);

    if(gDiagBoxType == DIAG_TYPE_ROTATE)
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    }
    else
    {
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    }

    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
}

void func_802D9030(s8 sp3b)
{
    s32 sp34 = gGlobalTimer;

    if(sp34 & 0x08)
        return;

    dl_add_new_translation_matrix(1, JP_US_DEF(123.0f , 118.0f), (sp3b * JP_US_DEF(-20 , -16)) + JP_US_DEF(2 , 5), 0);
    dl_add_new_scale_matrix(2, 0.8f, 0.8f, 1.0f);
    dl_add_new_rotation_matrix(2, -DEFAULT_DIAGBOX_ANGLE, 0, 0, 1.0f);

    if(gDiagBoxType == DIAG_TYPE_ROTATE)
    {
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    }
    else
    {
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 255);
    }

    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void func_802D91C0(s16 sp4a)
{
    s16 sp3c[] = { 0x0011, 0x0072, 0x0080, 0x0075, 0x0096 };
    s16 sp34[] = { 0x0005, 0x0009, 0x0037, 0x00A4 };
    s16 sp28[] = { 0x000A, 0x000B, 0x000C, 0x000D, 0x000E };
#ifdef VERSION_JP
    s16 sp20[] = { 0x0011, 0x0073, 0x0076, 0x0098 };
#else
    s16 sp20[] = { 0x0011, 0x0073, 0x0074, 0x0076, 0x0098 };
#endif
    s16 i;

    for(i = 0; i < (s16) ARRAY_COUNT(sp3c); i++)
    {
        if(sp3c[i] == sp4a)
        {
            func_80320040(0, 60);
            func_80320AE8(0, (4 << 8) | 22, 0);
            return;
        }
    }

    for(i = 0; i < (s16) ARRAY_COUNT(sp34); i++)
    {
        if(sp34[i] == sp4a && D_80330430 == 1)
        {
            play_race_fanfare();
            return;
        }
    }

    for(i = 0; i < (s16) ARRAY_COUNT(sp28); i++)
    {
        if(sp28[i] == sp4a && D_80330430 == 1)
        {
            SetSound(SOUND_MENU_STARSOUND, D_803320E0);
            return;
        }
    }

    for(i = 0; i < (s16) ARRAY_COUNT(sp20); i++)
    {
        if(sp20[i] == sp4a)
        {
            func_8031F7CC(0, 1);
            return;
        }
    }
}

s16 D_8033047C = -1;

u8 gEndCutsceneStr0[] = {TEXT_FILE_MARIO_EXCLAMATION};
u8 gEndCutsceneStr1[] = {TEXT_POWER_STARS_RESTORED};
u8 gEndCutsceneStr2[] = {TEXT_THANKS_TO_YOU};
u8 gEndCutsceneStr3[] = {TEXT_THANK_YOU_MARIO};
u8 gEndCutsceneStr4[] = {TEXT_SOMETHING_SPECIAL};
u8 gEndCutsceneStr5[] = {TEXT_COME_ON_EVERYBODY};
u8 gEndCutsceneStr6[] = {TEXT_LETS_HAVE_CAKE};
u8 gEndCutsceneStr7[] = {TEXT_FOR_MARIO};
u8 gEndCutsceneStr8[] = {TEXT_FILE_MARIO_QUESTION};

u8 *gEndCutsceneStrings[] = {
    gEndCutsceneStr0,
    gEndCutsceneStr1,
    gEndCutsceneStr2,
    gEndCutsceneStr3,
    gEndCutsceneStr4,
    gEndCutsceneStr5,
    gEndCutsceneStr6,
    gEndCutsceneStr7,
    // this [8] string is actually unused. in the
    // cutscene handler, the developers do not
    // set the 8th one, but use the first string
    // again at the very end, so Peach ends up
    // saying "Mario!" twice. It is likely that
    // she was originally meant to say "Mario?" at
    // the end but the developers changed their
    // mind, possibly because the line recorded
    // sounded more like an exclamation than a
    // question.
    gEndCutsceneStr8,
    NULL
};

u16 gCutsceneMsgFade = 0;
s16 gCutsceneMsgIndex = -1;
s16 gCutsceneMsgDuration = -1;
s16 gCutsceneMsgTimer = 0;
s8 D_80330530 = 1;
s8 D_80330534 = 1;

void func_802D93E0(void)
{
    void **diagTable = segmented_to_virtual(seg2_dialog_table);
    struct DialogEntry *diagEntry = segmented_to_virtual(diagTable[gDialogID]);
#ifndef VERSION_JP
    s8 sp2F;
#endif

    // if the dialog entry is invalid, set the ID to -1.
    if(segmented_to_virtual(NULL) == diagEntry)
    {
        gDialogID = -1;
        return;
    }

    switch(gDiagBoxState)
    {
    case DIAG_STATE_OPENING:
        if(gDiagBoxOpenTimer == DEFAULT_DIAGBOX_ANGLE)
        {
            func_80320A68(gDialogID);
            SetSound(SOUND_MENU_MESSAGEAPPEAR, D_803320E0);
        }

        if(gDiagBoxType == DIAG_TYPE_ROTATE)
        {
            gDiagBoxOpenTimer -= 7.5;
            gDiagBoxScale -= 1.5;
        }
        else
        {
            gDiagBoxOpenTimer -= 10.0;
            gDiagBoxScale -= 2.0;
        }

        if(gDiagBoxOpenTimer == 0.0f)
        {
            gDiagBoxState = DIAG_STATE_WAITBUTTON;
            D_80330430 = 1;
        }
#ifndef VERSION_JP
        sp2F = 1;
#endif
        break;
    case DIAG_STATE_WAITBUTTON:
        gDiagBoxOpenTimer = 0.0f;

        if((gPlayer3Controller->buttonPressed & A_BUTTON) ||
           (gPlayer3Controller->buttonPressed & B_BUTTON))
           {
               if(gLastDialogPageStrPos == -1)
               {
                   func_802D91C0(gDialogID);
                   gDiagBoxState = DIAG_STATE_CLOSING;
               }
               else
               {
                   gDiagBoxState = DIAG_STATE_SCROLLING;
                   SetSound(SOUND_MENU_MESSAGENEXTPAGE, D_803320E0);
               }
           }
#ifndef VERSION_JP
           sp2F = 1;
#endif
        break;
    case DIAG_STATE_SCROLLING:
        D_8033041C += diagEntry->linesPerBox * 2;

        if(D_8033041C >= diagEntry->linesPerBox * JP_US_DEF(20 , 16))
        {
            D_8033042C = gLastDialogPageStrPos;
            gDiagBoxState = DIAG_STATE_WAITBUTTON;
            D_8033041C = 0;
        }
#ifndef VERSION_JP
        sp2F = (D_8033041C / 16) + 1;
#endif
        break;
    case DIAG_STATE_CLOSING:
        if(gDiagBoxOpenTimer == 20.0f)
        {
            level_set_transition(0, 0);
            SetSound(SOUND_MENU_MESSAGEDISAPPEAR, D_803320E0);

            if(gDiagBoxType == DIAG_TYPE_ZOOM)
                stop_mario(2);

            gDialogueResponse = D_80330430;
        }

        gDiagBoxOpenTimer = gDiagBoxOpenTimer + 10.0f;
        gDiagBoxScale     = gDiagBoxScale + 2.0f;

        if(gDiagBoxOpenTimer == DEFAULT_DIAGBOX_ANGLE)
        {
            gDiagBoxState = DIAG_STATE_OPENING;
            gDialogID = -1;
            D_8033042C = 0;
            D_80330434 = 0;
            gLastDialogPageStrPos = 0;
            gDialogueResponse = 0;
        }
#ifndef VERSION_JP
        sp2F = 1;
#endif
        break;
    }

    func_802D8134(diagEntry, diagEntry->linesPerBox);

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE,
        func_802D8954(diagEntry->leftOffset),
        func_802D8954(240 - diagEntry->width),
        func_802D8954(JP_US_DEF(130 , 132) + diagEntry->leftOffset),
        func_802D8954(240 + ((diagEntry->linesPerBox*80)/JP_US_DEF(4 , 5)) - diagEntry->width));
#ifdef VERSION_JP
    func_802D8980(0, diagEntry);
#else
    func_802D8980(0, diagEntry, sp2F);
#endif

    if(gLastDialogPageStrPos == -1 && D_80330434 == 1)
        func_802D8ED4();

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 2, 2, 316, 236);

    if(gLastDialogPageStrPos != -1 && gDiagBoxState == DIAG_STATE_WAITBUTTON)
        func_802D9030(diagEntry->linesPerBox);
}

void func_802D9A14(s16 a0)
{
    if(D_8033047C == -1)
        D_8033047C = a0;
}

void reset_cutscene_msg_fade(void)
{
    gCutsceneMsgFade = 0;
}

void func_802D9A60(void)
{
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
}

void func_802D9AE8(void)
{
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    if(gCutsceneMsgFade < 250)
        gCutsceneMsgFade += 25;
    else
        gCutsceneMsgFade = 255;
}

u8 ascii_to_credits_char(u8 c)
{
    if(c >= 'A' && c <= 'Z')
        return (c - 0x37);

    if(c >= 'a' && c <= 'z')
        return (c - 0x57);

    if(c == ' ') return 0x9E;
    if(c == '.') return 0x24;
    if(c == '3') return 0x03;
    if(c == '4') return 0x04;
    if(c == '6') return 0x06;

    return 0x9E;
}

void print_credits_str(s16 x, s16 y, char *str)
{
    s32 pos = 0;
    u8 c = str[pos];
    u8 creditStr[100];

    while(c != 0)
    {
        creditStr[pos++] = ascii_to_credits_char(c);
        c = str[pos];
    }

    creditStr[pos] = 0xFF;

    PutMiniString(x, y, creditStr);
}

void set_cutscene_message(s16 xOffset, s16 yOffset, s16 msgIndex, s16 msgDuration)
{
    // is message done printing?
    if(gCutsceneMsgIndex == -1)
    {
        gCutsceneMsgIndex = msgIndex;
        gCutsceneMsgDuration = msgDuration;
        gCutsceneMsgTimer = 0;
        gCutsceneMsgXOffset = xOffset;
        gCutsceneMsgYOffset = yOffset;
        gCutsceneMsgFade = 0;
    }
}

void do_cutscene_handler(void)
{
    s16 x;

    // is a cutscene playing? do not perform this
    // handler's actions if so.
    if(gCutsceneMsgIndex == -1)
        return;

    dl_add_new_ortho_matrix();

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);

    // get the x coordinate of where the cutscene string starts.
    x = get_str_x_pos_from_center(gCutsceneMsgXOffset, gEndCutsceneStrings[gCutsceneMsgIndex], 10.0f);

    PrintGenericText(x, 240 - gCutsceneMsgYOffset, gEndCutsceneStrings[gCutsceneMsgIndex]);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);

    // if the timing variable is less than 5, increment
    // the fade until we are at full opacity.
    if(gCutsceneMsgTimer < 5)
        gCutsceneMsgFade += 50;

    // if the cutscene frame length + the fade-in counter is
    // less than the timer, it means we have exceeded the 
    // time that the message is supposed to remain on 
    // screen. if (message_duration = 50) and (msg_timer = 55)
    // then after the first 5 frames, the message will remain
    // on screen for another 50 frames until it starts fading.
    if(gCutsceneMsgDuration + 5 < gCutsceneMsgTimer)
        gCutsceneMsgFade -= 50;

    // like the first check, it takes 5 frames to fade out, so
    // perform a + 10 to account for the earlier check (10-5=5).
    if(gCutsceneMsgDuration + 10 < gCutsceneMsgTimer)
    {
        gCutsceneMsgIndex = -1;
        gCutsceneMsgFade = 0;
        gCutsceneMsgTimer = 0;
        return;
    }

    gCutsceneMsgTimer++;
}

#if defined(VERSION_US) || defined(VERSION_EU)
extern Gfx castle_grounds_seg7_us_dl_0700F2E8[];
#endif

#ifdef VERSION_JP
#define PEACH_MESSAGE_TIMER 170
#else
#define PEACH_MESSAGE_TIMER 250
#endif

// "Dear Mario" message handler
void print_peach_letter_message(void)
{
    void **diagTable = segmented_to_virtual(seg2_dialog_table);
    struct DialogEntry *diagEntry = segmented_to_virtual(diagTable[gDialogID]);

    u8 *str = segmented_to_virtual(diagEntry->str);

    dl_add_new_translation_matrix(1, 97.0f, 118.0f, 0);

    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, gCutsceneMsgFade);
    gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_dl_0700EA58);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 20, 20, 20, gCutsceneMsgFade);

    PrintGenericText(JP_US_DEF(53 , 38), JP_US_DEF(136 , 142), str);
#ifdef VERSION_JP
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
#else
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
    gDPSetEnvColor(gDisplayListHead++, 200, 80, 120, gCutsceneMsgFade);
    gSPDisplayList(gDisplayListHead++, castle_grounds_seg7_us_dl_0700F2E8);
#endif

    // at the start/end of message, reset the fade.
    if(gCutsceneMsgTimer == 0)
        gCutsceneMsgFade = 0;

    // we're less than 20 increments, so increase the fade.
    if(gCutsceneMsgTimer < 20)
        gCutsceneMsgFade += 10;

    // we're after PEACH_MESSAGE_TIMER increments, so decrease the fade.
    if(gCutsceneMsgTimer > PEACH_MESSAGE_TIMER)
        gCutsceneMsgFade -= 10;

    // 20 increments after the start of the decrease, we're
    // back where we are, so reset everything at the end.
    if(gCutsceneMsgTimer > (PEACH_MESSAGE_TIMER + 20))
    {
        gCutsceneMsgIndex = -1;
        gCutsceneMsgFade = 0; //! this is uselessly reset since
                              //  the next execution will just
                              //  set it to 0 again.
        gDialogID = -1;
        gCutsceneMsgTimer = 0;
        return; // return to avoid incrementing the timer
    }

    gCutsceneMsgTimer++;
}

void RenderHudCannonReticle(void)
{
    dl_add_new_translation_matrix(1, 160.0f, 120.0f, 0);

    gDPSetEnvColor(gDisplayListHead++, 50, 50, 50, 180);
    dl_add_new_translation_matrix(1, -20.0f, -8.0f, 0);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    dl_add_new_translation_matrix(1, 20.0f, 8.0f, 0);
    dl_add_new_rotation_matrix(2, 180.0f, 0, 0, 1.0f);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    dl_add_new_translation_matrix(1, 8.0f, -20.0f, 0);
    dl_add_new_rotation_matrix(2, DEFAULT_DIAGBOX_ANGLE, 0, 0, 1.0f);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    dl_add_new_translation_matrix(1, -8.0f, 20.0f, 0);
    dl_add_new_rotation_matrix(2, -DEFAULT_DIAGBOX_ANGLE, 0, 0, 1.0f);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void func_802DA4DC(void)
{
    D_8036008E = 0;
}

void func_802DA4F4(void)
{
    if(select_or_activate_mario_cam(0) == 1)
        D_80330530 = 1;
    else
        D_80330530 = 2;
}

void ShadeScreen(void)
{
    dl_add_new_translation_matrix(1, 0, 240.0f, 0);
    dl_add_new_scale_matrix(2, 2.6f, 3.4f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 110);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void PrintAnimatedRedCoin(s16 sp4a, s16 sp4e)
{
    s32 timer = gGlobalTimer;

    dl_add_new_translation_matrix(1, sp4a, sp4e, 0);
    dl_add_new_scale_matrix(2, 0.2f, 0.2f, 1.0f);
    gDPSetRenderMode(gDisplayListHead++, G_RM_TEX_EDGE, G_RM_TEX_EDGE2);

    switch(timer & 6)
    {
        case 0:
            gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007940);
            break;
        case 2:
            gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007968);
            break;
        case 4:
            gSPDisplayList(gDisplayListHead++, coin_seg3_dl_03007990);
            break;
        case 6:
            gSPDisplayList(gDisplayListHead++, coin_seg3_dl_030079B8);
            break;
    }

    gDPSetRenderMode(gDisplayListHead++, G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void func_802DA874(void)
{
    s8 sp1f;

    for(sp1f = 0; sp1f < D_8036008E; sp1f++)
    {
        PrintAnimatedRedCoin(290 - sp1f * 20,  16);
    }
}

void func_802DA8EC(void)
{
    u8 sp64[] = {TEXT_COURSE};
    u8 sp5c[] = {TEXT_MY_SCORE};
    u8 sp58[] = {TEXT_STAR};
    u8 sp54[] = {TEXT_UNFILLED_STAR};

    u8 strCourseNum[4];

    void **levelNameTbl = segmented_to_virtual(seg2_level_name_table);
    u8 *levelName;

    void **actNameTbl = segmented_to_virtual(seg2_act_name_table);
    u8 *actName;

    u8 courseIndex = gCurrCourseNum - 1;
    u8 starFlags = save_file_get_star_flags(gCurrSaveFileNum - 1, gCurrCourseNum - 1);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    if(courseIndex < 15)
    {
        ShowCoins(1, gCurrSaveFileNum - 1, courseIndex, 178, 103);
        ShowStars(gCurrSaveFileNum - 1, courseIndex, 118, 103);
    }

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);

    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    if(courseIndex < 15 &&
        save_file_get_course_star_count(gCurrSaveFileNum - 1, courseIndex) != 0)
    {
        PrintGenericText(62, 121, sp5c);
    }

    levelName = segmented_to_virtual(levelNameTbl[courseIndex]);

    if(courseIndex < 15)
    {
        PrintGenericText(63, 157, sp64);
        Int2Str(gCurrCourseNum, strCourseNum);
        PrintGenericText(JP_US_DEF(93 , 100), 157, strCourseNum);

        actName = segmented_to_virtual(actNameTbl[gCurrCourseNum * 6 + D_80330534 - 7]);

        if(starFlags & (1 << (D_80330534 + 31)))
            PrintGenericText(98, 140, sp58);
        else
            PrintGenericText(98, 140, sp54);

        PrintGenericText(116, 140, actName);
#ifndef VERSION_JP
        PrintGenericText(117, 157, &levelName[3]);
#endif
    }
#ifndef VERSION_JP
    else
    {
        PrintGenericText(94, 157, &levelName[3]);
    }
#else
    PrintGenericText(117, 157, &levelName[3]);
#endif
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
}

void PauseScreenCameraMenu(s16 sp72, s16 sp76, s8 *sp78, s16 sp7e)
{
    u8 sp64[] = {TEXT_LAKITU_MARIO}; //D_8033054C;
    u8 sp54[] = {TEXT_LAKITU_STOP}; //D_80330558;
    u8 sp48[] = {TEXT_NORMAL_UPCLOSE}; //D_80330568;
    u8 sp3c[] = {TEXT_NORMAL_FIXED}; //D_80330574;

    handleMenuScrolling(MENU_SCROLL_HORIZONTAL, sp78, 1, 2);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    PrintGenericText(sp72 + 14, sp76 + 2, sp64);
    PrintGenericText(sp72 + JP_US_DEF(4 , 3), sp76 - 13, sp48);
    PrintGenericText(sp72 + 124, sp76 + 2, sp54);
    PrintGenericText(sp72 + JP_US_DEF(116 , 119), sp76 - 13, sp3c);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
    dl_add_new_translation_matrix(1, ((sp78[0] - 1) * sp7e) + sp72, sp76 + JP_US_DEF(0 , 2), 0);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    switch(sp78[0])
    {
    case 1: select_or_activate_mario_cam(1); break;
    case 2: select_or_activate_mario_cam(2); break;
    }
}

void PauseScreen2(s16 sp62, s16 sp66, s8 *sp68, s16 sp6e)
{
    u8 sp50[] = {TEXT_CONTINUE}; //D_80330580;
    u8 sp44[] = {TEXT_EXIT_COURSE}; //D_80330590;
    u8 sp34[] = {TEXT_CAMERA_ANGLE_R}; //D_8033059C;

    handleMenuScrolling(MENU_SCROLL_VERTICAL, sp68, 1, 3);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    PrintGenericText(sp62 + 10, sp66 - 2, sp50);
    PrintGenericText(sp62 + 10, sp66 - 17, sp44);

    if(sp68[0] != 3)
    {
        PrintGenericText(sp62 + 10, sp66 - 33, sp34);
        gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);

        dl_add_new_translation_matrix(1, sp62 - JP_US_DEF(0 , 4), (sp66 - ((sp68[0] - 1) * sp6e)) - JP_US_DEF(4 , 2), 0);

        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
        gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
        gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    }

    if(sp68[0] == 3)
    {
        PauseScreenCameraMenu(sp62 - 42, sp66 - 42, &D_80330530, 110);
    }
}

void PauseCastleMenuBox(s16 sp42, s16 sp46)
{
    dl_add_new_translation_matrix(1, sp42-78, sp46-32, 0);
    dl_add_new_scale_matrix(2, 1.2f, 0.8f, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, 105);
    gSPDisplayList(gDisplayListHead++, dl_draw_text_bg_box);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);

    dl_add_new_translation_matrix(1, sp42+6, sp46-28, 0);
    dl_add_new_rotation_matrix(2, DEFAULT_DIAGBOX_ANGLE, 0, 0, 1.0f);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
    
    dl_add_new_translation_matrix(1, sp42-9, sp46-101, 0);
    dl_add_new_rotation_matrix(2, 270.0f, 0, 0, 1.0f);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);
    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

void func_802DB540(void)
{
    u8 sp07;

    if (gLastCompletedCourseNum == 0)
    {
        sp07 = 0;
    }
    else
    {
        sp07 = gLastCompletedCourseNum - 1;

        if(sp07 >= 15)
            sp07 = 15;
    }

    D_80330430 = sp07;
}

void PauseScreen1(void)
{
    u8 sp28[] = {TEXT_PAUSE};

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    PutString(2, 123, 81, sp28);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void func_802DB698(s16 sp4a, s16 sp4e, s16 sp52, s16 sp56)
{
    s16 sp46 = 0;
    
    UNUSED u8 sp28[30];

    u8 sp24[] = {TEXT_STAR}; //D_803305B4;

    u8 sp23 = save_file_get_star_flags(sp52, sp56);
    u16 sp20 = save_file_get_course_star_count(sp52, sp56);

    u16 sp1e = 0;

    if(sp23 & 0x40)
    {
        sp20--;
        PrintGenericText(sp4a+89, sp4e-5, sp24);
    }

    while(sp46 != sp20)
    {
        if(sp23 & (1 << sp1e))
        {
            sp28[sp1e*2] = 250;
            sp46++;
        }
        else
        {
            sp28[sp1e*2] = 253;
        }

        sp28[sp1e*2+1] = 158;
        sp1e++;
    }

    if(sp20 == sp1e && sp20 != 6)
    {
        sp28[sp1e*2] = 253;
        sp28[sp1e*2+1] = 158;
        sp1e++;
    }

    sp28[sp1e*2] = 0xFF;

    PrintGenericText(sp4a+14, sp4e+13, sp28);
}

void func_802DB840(s16 sp52, s16 sp56)
{
    void **sp4c = segmented_to_virtual(seg2_level_name_table);

    u8 sp48[] = {TEXT_COIN_X}; //D_803305B8;

    void *sp44;

    u8 sp3c[8];
    s16 sp3a = D_80330430;

    handleMenuScrolling(MENU_SCROLL_VERTICAL, &D_80330430, -1, 16);

    if(D_80330430 == 16)
        D_80330430 = 0;

    if(D_80330430 == -1)
        D_80330430 = 15;

    if(D_80330430 != 15)
    {
        while(save_file_get_course_star_count(gCurrSaveFileNum-1, D_80330430) == 0)
        {
            if(D_80330430 >= sp3a)
                D_80330430++;
            else
                D_80330430--;
            
            if(D_80330430 == 15 || D_80330430 == -1)
            {
                D_80330430 = 15;
                break;
            }
        }
    }

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    if(D_80330430 < 15)
    {
        sp44 = segmented_to_virtual(sp4c[D_80330430]);
        func_802DB698(sp52, sp56, gCurrSaveFileNum-1, D_80330430);
        PrintGenericText(sp52+34, sp56-5, sp48);
        Int2Str(save_file_get_course_coin_score(gCurrSaveFileNum-1, D_80330430), sp3c);
        PrintGenericText(sp52+54, sp56-5, sp3c);
    }
    else
    {
        u8 sp2c[] = {TEXT_STAR_X}; // D_803305BC
        sp44 = segmented_to_virtual(sp4c[25]);
        PrintGenericText(sp52+40, sp56+13, sp2c);
        Int2Str(save_file_get_total_star_count(gCurrSaveFileNum-1, 15, 24), sp3c);
        PrintGenericText(sp52+60, sp56+13, sp3c);
    }

    PrintGenericText(sp52-9, sp56+30, sp44);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
}

s8 D_803305C0 = 0;
s32 D_803305C4 = 0;
s32 D_803305C8 = 0; // gCourseCompleteMenuCoins
s8 D_803305CC = 0;

s16 func_802DBBB0(void)
{
    s16 sp26;

    switch(gDiagBoxState)
    {
    case DIAG_STATE_OPENING:
        D_80330430 = 1;
        D_80360088 = 0;
        level_set_transition(-1, 0);
#ifdef VERSION_JP
        SetSound(SOUND_MENU_PAUSE1, D_803320E0);
#else
        SetSound(SOUND_MENU_PAUSE1_HIGHPRIO, D_803320E0);
#endif

        if(gCurrCourseNum > 0 && gCurrCourseNum < 26)
        {
            func_802DA4F4();
            gDiagBoxState = DIAG_STATE_WAITBUTTON;
        }
        else
        {
            func_802DB540();
            gDiagBoxState = DIAG_STATE_SCROLLING;
        }
        break;
    case DIAG_STATE_WAITBUTTON:
        ShadeScreen();
        func_802DA8EC();
        func_802DA874();

        if(gMarioStates[0].action & ACT_FLAG_PAUSE_EXIT)
            PauseScreen2(99, 93, &D_80330430, 15);

        if(gPlayer3Controller->buttonPressed & A_BUTTON ||
           gPlayer3Controller->buttonPressed & START_BUTTON)
        {
            level_set_transition(0, 0);
            SetSound(SOUND_MENU_PAUSE2, D_803320E0);
            gDiagBoxState = DIAG_STATE_OPENING;
            D_8033047C = -1;

            if(D_80330430 == 2)
            {
                sp26 = D_80330430;
            }
            else
            {
                sp26 = 1;
            }

            return sp26;
        }
        break;
    case DIAG_STATE_SCROLLING:
        ShadeScreen();
        PauseScreen1();
        PauseCastleMenuBox(160, 143);
        func_802DB840(104, 60);

        if(gPlayer3Controller->buttonPressed & A_BUTTON ||
           gPlayer3Controller->buttonPressed & START_BUTTON)
        {
            level_set_transition(0, 0);
            SetSound(SOUND_MENU_PAUSE2, D_803320E0);
            D_8033047C = -1;
            gDiagBoxState = DIAG_STATE_OPENING;

            return 1;
        }
        break;
    }

    if(D_80360088 < 250)
        D_80360088 += 25;

    return 0;
}

void func_802DBE2C(s8 sp4b)
{
    u8 sp38[] = {TEXT_HISCORE_ENG}; //D_803305D0;
    u8 sp28[] = {TEXT_CONGRATULATIONS}; //D_803305D8;

    u8 sp27 = (gSineTable[D_80360080 >> 4] * 50.0f) + 200.0f;

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, sp27, sp27, sp27, 255);

    if(sp4b == 0)
        PutString(2, JP_US_DEF(112 , 109), JP_US_DEF(48 , 36), sp38);
    else
        PutString(2, JP_US_DEF(60 , 70), 67, sp28);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
}

void func_802DC050(s16 sp32, s16 sp36)
{
    u8 sp2c[4];
    u8 sp28[] = {TEXT_UNK33};// D_803305E8;
    u8 sp24[] = {TEXT_UNK32};// D_803305EC;

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, 255);

    PutString(2, sp32, sp36, sp28);
    PutString(2, sp32 + 16, sp36, sp24);

    Int2Str(D_803305C8, sp2c);
    PutString(2, sp32 + 32, sp36, sp2c);

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);

    if(D_803305C8 >= gDisplayedCoins)
    {
        D_803305C0 = 1;
        D_803305C8 = gDisplayedCoins;

        if (gGotFileCoinHiScore != 0)
        {
            func_802DBE2C(0);
        }
    }
    else
    {
        if((D_803305C4 & 1) || gDisplayedCoins > 70)
        {
            D_803305C8++;
            SetSound(SOUND_MENU_YOSHIGAINLIVES, D_803320E0);

            if(D_803305C8 ==  50 || D_803305C8 == 100 || D_803305C8 == 150)
            {
                SetSound(SOUND_GENERAL_1UP, D_803320E0);
                gMarioState[0].numLives++;
            }
        }

        if (gDisplayedCoins == D_803305C8 && gGotFileCoinHiScore != 0)
            SetSound(SOUND_MENU_MARIOCASTLEWARP2, D_803320E0);
    }
}

void func_802DC2B4(s32 sp18, u8 sp1f)
{
    if (gDisplayedCoins == D_803305C8 &&
        (gCurrCourseStarFlags & sp1f) == 0 &&
        D_803305CC == 0)
    {
        play_star_fanfare();
        D_803305CC = sp18;
    }
}

void func_802DC330(void)
{
#ifdef VERSION_JP
    u8 sp7c[] = {TEXT_UNK35}; //D_803305F0;
    u8 sp78[] = {TEXT_COURSE};//D_803305F4;
    u8 sp70[] = {TEXT_CATCH};//D_803305F8;
    u8 sp68[] = {TEXT_CLEAR};//D_80330600;
#else
    u8 sp78[] = {TEXT_COURSE};//D_803305F4;
    UNUSED u8 sp70[] = {TEXT_CATCH};//D_803305F8; unused in US
    UNUSED u8 sp68[] = {TEXT_CLEAR};//D_80330600; unused in US
    u8 sp7c[] = {TEXT_UNK35}; //D_803305F0;
#endif

    u8 **sp64 = segmented_to_virtual(seg2_act_name_table);
    u8 **sp60 = segmented_to_virtual(seg2_level_name_table);
    u8 *sp5c;

    u8 sp58[4];

    if (gLastCompletedCourseNum < 16)
    {
        func_802DC050(118, 103);
        func_802DC2B4(1, 1 << (gLastCompletedStarNum + 31));

        if (gLastCompletedStarNum == 7)
        {
            sp5c = segmented_to_virtual(sp64[91]);
        }
        else
        {
            sp5c = segmented_to_virtual(sp64[(gLastCompletedCourseNum * 6 + gLastCompletedStarNum) - 7]);
        }

        gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
        Int2Str(gLastCompletedCourseNum, sp58);
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, D_80360088);
        PrintGenericText(65, 165, sp78);
        PrintGenericText(JP_US_DEF(95 , 104), 165, sp58);
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
        PrintGenericText(63, 167, sp78);
        PrintGenericText(JP_US_DEF(93 , 102), 167, sp58);
        gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
    }
    else if (gLastCompletedCourseNum == 16 || gLastCompletedCourseNum == 17)
    {
        sp5c = segmented_to_virtual(sp60[gLastCompletedCourseNum - 1]);
        gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
        gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, D_80360088);
        PrintGenericText(71, 130, sp5c);
        PrintGenericText(JP_US_DEF(205 , func_u_802D8934(sp5c) + 81), 130, sp68);
        gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
        PrintGenericText(69, 132, sp5c);
        PrintGenericText(JP_US_DEF(203 , func_u_802D8934(sp5c) + 79), 132, sp68);
        gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
        func_802DBE2C(1);
        func_802DC050(118, 111);
        func_802DC2B4(2, 0);
        return;
    }
    else
    {
        sp5c = segmented_to_virtual(sp64[90]);
        func_802DC050(118, 103);
        func_802DC2B4(1, 1 << (gLastCompletedStarNum + 31));
    }

    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
    PutString(2, 55, 77, sp7c);
    gSPDisplayList(gDisplayListHead++, dl_rgba16_text_end);
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 0, 0, 0, D_80360088);
    PrintGenericText(76, 145, sp5c);
#ifdef VERSION_JP
    PrintGenericText(220, 145, sp70);
#endif
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
    PrintGenericText(74, 147, sp5c);
#ifdef VERSION_JP
    PrintGenericText(218, 147, sp70);
#endif
    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);
}

void Save(s16 sp62, s16 sp66, s8 *sp68, s16 sp6e)
{
    u8 sp50[] = {TEXT_SAVE_AND_CONTINUE}; //D_80330608;
    u8 sp44[] = {TEXT_SAVE_AND_QUIT}; //D_80330618;
    u8 sp34[] = {TEXT_CONTINUE_WITHOUT_SAVING}; //D_80330624;

    handleMenuScrolling(MENU_SCROLL_VERTICAL, sp68, 1, 3);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_begin);
    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);

    PrintGenericText(sp62 + JP_US_DEF(10 , 12), sp66 + JP_US_DEF(2 , 0), sp50);
    PrintGenericText(sp62 + JP_US_DEF(10 , 12), sp66 - JP_US_DEF(18 , 20), sp44);
    PrintGenericText(sp62 + JP_US_DEF(10 , 12), sp66 - JP_US_DEF(38 , 40), sp34);

    gSPDisplayList(gDisplayListHead++, dl_ia8_text_end);

    dl_add_new_translation_matrix(1, sp62, sp66 - ((sp68[0] - 1) * sp6e), 0);

    gDPSetEnvColor(gDisplayListHead++, 255, 255, 255, D_80360088);
    gSPDisplayList(gDisplayListHead++, dl_draw_triangle);

    gSPPopMatrix(gDisplayListHead++, G_MTX_MODELVIEW);
}

s16 func_802DCBD4(void)
{
    s16 sp26;

    switch(gDiagBoxState)
    {
    case DIAG_STATE_OPENING:
        func_802DC330();
        if(D_803305C4 > 100 && D_803305C0 == 1)
        {
            gDiagBoxState = DIAG_STATE_WAITBUTTON;
            level_set_transition(-1, 0);
            D_80360088 = 0;
            D_80330430 = 1;
        }
        break;
    case DIAG_STATE_WAITBUTTON:
        ShadeScreen();
        func_802DC330();
        Save(100, 86, &D_80330430, 20);

        if(D_803305C4 > 110 &&
           (gPlayer3Controller->buttonPressed & A_BUTTON ||
            gPlayer3Controller->buttonPressed & START_BUTTON))
        {
            level_set_transition(0, 0);
            SetSound(SOUND_MENU_STARSOUND, D_803320E0);
            gDiagBoxState = DIAG_STATE_OPENING;
            D_8033047C = -1;
            sp26 = D_80330430;
            D_803305C4 = 0;
            D_803305C8 = 0;
            D_803305C0 = 0;
            D_803305CC = 0;

            return sp26;
        }
        break;
    }

    if(D_80360088 < 250)
    {
        D_80360088 += 25;
    }

    D_803305C4++;

    return 0;
}

s16 func_802DCD98()
{
    s16 sp26 = 0;

    dl_add_new_ortho_matrix();

    if(D_8033047C != -1)
    {
        switch(D_8033047C)
        {
            case 0:
                sp26 = func_802DBBB0();
                break;
            case 1:
                sp26 = func_802DBBB0();
                break;
            case 2:
                sp26 = func_802DCBD4();
                break;
            case 3:
                sp26 = func_802DCBD4();
                break;
        }

        D_80360080 = (s16)D_80360080 + 0x1000;
    }
    else if(gDialogID != -1)
    {
        // is the "Dear Mario, please come
        // to the castle" message?
        if(gDialogID == 20)
        {
            print_peach_letter_message(); // the peach message needs to be
                                          // repositioned seperately
            return sp26;
        }

        func_802D93E0();
        D_80360080 = (s16)D_80360080 + 0x1000;
    }

    return sp26;
}
