#include <ultra64.h>

#include "sm64.h"
#include "game/sound_init.h"
#include "math_util.h"
#include "surface_collision.h"

extern s16 D_8038B000[];

Vec4s *D_8038BC90;
float D_8038BC94;
int D_8038BC98;

// These functions have bogus return values.
// Disable the compiler warning.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wreturn-local-addr"

void *vec3f_copy(Vec3f dest, Vec3f src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_set(Vec3f dest, f32 x, f32 y, f32 z)
{
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_add(Vec3f dest, Vec3f a)
{
    dest[0] += a[0];
    dest[1] += a[1];
    dest[2] += a[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_sum(Vec3f dest, Vec3f a, Vec3f b)
{
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_copy(Vec3s dest, Vec3s src)
{
    dest[0] = src[0];
    dest[1] = src[1];
    dest[2] = src[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_set(Vec3s dest, s16 x, s16 y, s16 z)
{
    dest[0] = x;
    dest[1] = y;
    dest[2] = z;
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_add(Vec3s dest, Vec3s a)
{
    dest[0] += a[0];
    dest[1] += a[1];
    dest[2] += a[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_sum(Vec3s dest, Vec3s a, Vec3s b)
{
    dest[0] = a[0] + b[0];
    dest[1] = a[1] + b[1];
    dest[2] = a[2] + b[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_sub(Vec3s dest, Vec3s a)
{
    dest[0] -= a[0];
    dest[1] -= a[1];
    dest[2] -= a[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3s_to_vec3f(Vec3f dest, Vec3s a)
{
    dest[0] = a[0];
    dest[1] = a[1];
    dest[2] = a[2];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_to_vec3s(Vec3s dest, Vec3f a)
{
    // add/subtract 0.5 in order to round to the nearest s32 instead of truncating
    dest[0] = a[0] + ((a[0] > 0) ? 0.5f : -0.5f);
    dest[1] = a[1] + ((a[1] > 0) ? 0.5f : -0.5f);
    dest[2] = a[2] + ((a[2] > 0) ? 0.5f : -0.5f);
    return &dest;  //! warning: function returns address of local variable
}

void *find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c)
{
    dest[0] = (b[1] - a[1]) * (c[2] - b[2]) - (c[1] - b[1]) * (b[2] - a[2]);
    dest[1] = (b[2] - a[2]) * (c[0] - b[0]) - (c[2] - b[2]) * (b[0] - a[0]);
    dest[2] = (b[0] - a[0]) * (c[1] - b[1]) - (c[0] - b[0]) * (b[1] - a[1]);
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_cross(Vec3f dest, Vec3f a, Vec3f b)
{
    dest[0] = a[1] * b[2] - b[1] * a[2];
    dest[1] = a[2] * b[0] - b[2] * a[0];
    dest[2] = a[0] * b[1] - b[0] * a[1];
    return &dest;  //! warning: function returns address of local variable
}

void *vec3f_normalize(Vec3f dest)
{
    //! Possible division by zero
    f32 invsqrt = 1.0f / sqrtf(dest[0] * dest[0] + dest[1] * dest[1] + dest[2] * dest[2]);

    dest[0] *= invsqrt;
    dest[1] *= invsqrt;
    dest[2] *= invsqrt;
    return &dest;  //! warning: function returns address of local variable
}

#pragma GCC diagnostic pop

void mtxf_copy(f32 dest[4][4], f32 src[4][4])
{
    register s32 i;
    register u32 *d = (u32 *)dest;
    register u32 *s = (u32 *)src;

    for (i = 0; i < 16; i++)
        *d++ = *s++;
}

void mtxf_identity(f32 mtx[4][4])
{
    register s32 i;
    register f32 *dest;

    // initialize everything except the first and last cells to 0
    // (this need to be on one line to match on PAL)
    for (dest = (f32 *)mtx + 1, i = 0; i < 14; dest++, i++) *dest = 0;

    // initialize the diagonal cells to 1
    for (dest = (f32 *)mtx, i = 0; i < 4; dest += 5, i++) *dest = 1;
}

void mtxf_translate(f32 dest[4][4], Vec3f b)
{
    mtxf_identity(dest);
    dest[3][0] = b[0];
    dest[3][1] = b[1];
    dest[3][2] = b[2];
}

void func_80378F84(f32 mtx[4][4], Vec3f b, Vec3f c, s16 d)
{
    register f32 f20;
    f32 sp48;
    f32 sp44;
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp34;
    f32 sp30;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;

    sp48 = c[0] - b[0];
    sp44 = c[2] - b[2];

    f20 = -1.0 / sqrtf(sp48 * sp48 + sp44 * sp44);
    sp48 *= f20;
    sp44 *= f20;

    sp3C = coss(d);
    sp40 = sins(d) * sp44;
    sp38 = -sins(d) * sp48;

    sp34 = c[0] - b[0];
    sp30 = c[1] - b[1];
    sp2C = c[2] - b[2];

    f20 = -1.0 / sqrtf(sp34 * sp34 + sp30 * sp30 + sp2C * sp2C);
    sp34 *= f20;
    sp30 *= f20;
    sp2C *= f20;

    sp28 = sp3C * sp2C - sp38 * sp30;
    sp24 = sp38 * sp34 - sp40 * sp2C;
    sp20 = sp40 * sp30 - sp3C * sp34;

    f20 = 1.0 / sqrtf(sp28 * sp28 + sp24 * sp24 + sp20 * sp20);

    sp28 *= f20;
    sp24 *= f20;
    sp20 *= f20;

    sp40 = sp30 * sp20 - sp2C * sp24;
    sp3C = sp2C * sp28 - sp34 * sp20;
    sp38 = sp34 * sp24 - sp30 * sp28;

    f20 = 1.0 / sqrtf(sp40 * sp40 + sp3C * sp3C + sp38 * sp38);
    sp40 *= f20;
    sp3C *= f20;
    sp38 *= f20;

    mtx[0][0] = sp28;
    mtx[1][0] = sp24;
    mtx[2][0] = sp20;
    mtx[3][0] = -(b[0] * sp28 + b[1] * sp24 + b[2] * sp20);

    mtx[0][1] = sp40;
    mtx[1][1] = sp3C;
    mtx[2][1] = sp38;
    mtx[3][1] = -(b[0] * sp40 + b[1] * sp3C + b[2] * sp38);

    mtx[0][2] = sp34;
    mtx[1][2] = sp30;
    mtx[2][2] = sp2C;
    mtx[3][2] = -(b[0] * sp34 + b[1] * sp30 + b[2] * sp2C);

    mtx[0][3] = 0;
    mtx[1][3] = 0;
    mtx[2][3] = 0;
    mtx[3][3] = 1;
}

/**
 * Build a matrix that rotates around the z axis, then the x axis, then the y
 * axis, and then translates.
 */
void mtxf_rotate_zxy_and_translate(f32 dest[4][4], Vec3f translate, Vec3s rotate)
{
    register f32 sx = sins(rotate[0]);
    register f32 cx = coss(rotate[0]);

    register f32 sy = sins(rotate[1]);
    register f32 cy = coss(rotate[1]);
    
    register f32 sz = sins(rotate[2]);
    register f32 cz = coss(rotate[2]);

    dest[0][0] = cy * cz + sx * sy * sz;
    dest[1][0] = -cy * sz + sx * sy * cz;
    dest[2][0] = cx * sy;
    dest[3][0] = translate[0];

    dest[0][1] = cx * sz;
    dest[1][1] = cx * cz;
    dest[2][1] = -sx;
    dest[3][1] = translate[1];

    dest[0][2] = -sy * cz + sx * cy * sz;
    dest[1][2] = sy * sz + sx * cy * cz;
    dest[2][2] = cx * cy;
    dest[3][2] = translate[2];

    dest[0][3] = dest[1][3] = dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

/**
 * Build a matrix that rotates around the x axis, then the y axis, then the z
 * axis, and then translates.
 */
void mtxf_rotate_xyz_and_translate(f32 dest[4][4], Vec3f b, Vec3s c)
{
    register f32 sx = sins(c[0]);
    register f32 cx = coss(c[0]);

    register f32 sy = sins(c[1]);
    register f32 cy = coss(c[1]);
    
    register f32 sz = sins(c[2]);
    register f32 cz = coss(c[2]);

    dest[0][0] = cy * cz;
    dest[0][1] = cy * sz;
    dest[0][2] = -sy;
    dest[0][3] = 0;

    dest[1][0] = sx * sy * cz - cx * sz;
    dest[1][1] = sx * sy * sz + cx * cz;
    dest[1][2] = sx * cy;
    dest[1][3] = 0;

    dest[2][0] = cx * sy * cz + sx * sz;
    dest[2][1] = cx * sy * sz - sx * cz;
    dest[2][2] = cx * cy;
    dest[2][3] = 0;

    dest[3][0] = b[0];
    dest[3][1] = b[1];
    dest[3][2] = b[2];
    dest[3][3] = 1;
}

void func_80379798(f32 dest[4][4], f32 mtx[4][4], Vec3f c, s16 angle)
{
    dest[0][0] = coss(angle);
    dest[0][1] = sins(angle);
    dest[0][2] = 0;
    dest[0][3] = 0;

    dest[1][0] = -dest[0][1];
    dest[1][1] = dest[0][0];
    dest[1][2] = 0;
    dest[1][3] = 0;

    dest[2][0] = 0;
    dest[2][1] = 0;
    dest[2][2] = 1;
    dest[2][3] = 0;

    dest[3][0] = mtx[0][0] * c[0] + mtx[1][0] * c[1] + mtx[2][0] * c[2] + mtx[3][0];
    dest[3][1] = mtx[0][1] * c[0] + mtx[1][1] * c[1] + mtx[2][1] * c[2] + mtx[3][1];
    dest[3][2] = mtx[0][2] * c[0] + mtx[1][2] * c[1] + mtx[2][2] * c[2] + mtx[3][2];
    dest[3][3] = 1;
}

void mtxf_from_up_direction(f32 dest[4][4], Vec3f upDir, Vec3f pos, s16 yaw)
{
    Vec3f lateralDir;
    Vec3f leftDir;
    Vec3f forwardDir;

    vec3f_set(lateralDir, sins(yaw), 0, coss(yaw));
    vec3f_normalize(upDir);

    vec3f_cross(leftDir, upDir, lateralDir);
    vec3f_normalize(leftDir);

    vec3f_cross(forwardDir, leftDir, upDir);
    vec3f_normalize(forwardDir);

    dest[0][0] = leftDir[0];
    dest[0][1] = leftDir[1];
    dest[0][2] = leftDir[2];
    dest[3][0] = pos[0];

    dest[1][0] = upDir[0];
    dest[1][1] = upDir[1];
    dest[1][2] = upDir[2];
    dest[3][1] = pos[1];

    dest[2][0] = forwardDir[0];
    dest[2][1] = forwardDir[1];
    dest[2][2] = forwardDir[2];
    dest[3][2] = pos[2];

    dest[0][3] = 0.0f;
    dest[1][3] = 0.0f;
    dest[2][3] = 0.0f;
    dest[3][3] = 1.0f;
}

void func_80379AA4(f32 mtx[4][4], Vec3f b, s16 c, f32 d)
{
    struct Surface *sp74;
    Vec3f sp68;
    Vec3f sp5C;
    Vec3f sp50;
    Vec3f sp44;
    Vec3f sp38;
    Vec3f sp2C;
    Vec3f sp20;
    f32 sp1C;
    f32 sp18 = -d * 3;

    sp68[0] = b[0] + d * sins(c + 0x2AAA);
    sp68[2] = b[2] + d * coss(c + 0x2AAA);
    sp5C[0] = b[0] + d * sins(c + 0x8000);
    sp5C[2] = b[2] + d * coss(c + 0x8000);
    sp50[0] = b[0] + d * sins(c + 0xD555);
    sp50[2] = b[2] + d * coss(c + 0xD555);

    sp68[1] = find_floor(sp68[0], b[1] + 150, sp68[2], &sp74);
    sp5C[1] = find_floor(sp5C[0], b[1] + 150, sp5C[2], &sp74);
    sp50[1] = find_floor(sp50[0], b[1] + 150, sp50[2], &sp74);

    if (sp68[1] - b[1] < sp18)
        sp68[1] = b[1];

    if (sp5C[1] - b[1] < sp18)
        sp5C[1] = b[1];

    if (sp50[1] - b[1] < sp18)
        sp50[1] = b[1];

    sp1C = (sp68[1] + sp5C[1] + sp50[1]) / 3;

    vec3f_set(sp44, sins(c), 0, coss(c));
    find_vector_perpendicular_to_plane(sp2C, sp68, sp5C, sp50);
    vec3f_normalize(sp2C);
    vec3f_cross(sp38, sp2C, sp44);
    vec3f_normalize(sp38);
    vec3f_cross(sp20, sp38, sp2C);
    vec3f_normalize(sp20);

    mtx[0][0] = sp38[0];
    mtx[0][1] = sp38[1];
    mtx[0][2] = sp38[2];
    mtx[3][0] = b[0];

    mtx[1][0] = sp2C[0];
    mtx[1][1] = sp2C[1];
    mtx[1][2] = sp2C[2];
    mtx[3][1] = (sp1C < b[1]) ? b[1] : sp1C;

    mtx[2][0] = sp20[0];
    mtx[2][1] = sp20[1];
    mtx[2][2] = sp20[2];
    mtx[3][2] = b[2];

    mtx[0][3] = 0;
    mtx[1][3] = 0;
    mtx[2][3] = 0;
    mtx[3][3] = 1;
}

// Multiplies b * a, where b is an affine matrix and a is linear.
void mtxf_mul(f32 dest[4][4], f32 a[4][4], f32 b[4][4])
{
    f32 temp[4][4];
    register f32 entry0;
    register f32 entry1;
    register f32 entry2;

    // column 0
    entry0 = a[0][0];
    entry1 = a[0][1];
    entry2 = a[0][2];
    temp[0][0] = entry0 * b[0][0] + entry1 * b[1][0] + entry2 * b[2][0];
    temp[0][1] = entry0 * b[0][1] + entry1 * b[1][1] + entry2 * b[2][1];
    temp[0][2] = entry0 * b[0][2] + entry1 * b[1][2] + entry2 * b[2][2];

    // column 1
    entry0 = a[1][0];
    entry1 = a[1][1];
    entry2 = a[1][2];
    temp[1][0] = entry0 * b[0][0] + entry1 * b[1][0] + entry2 * b[2][0];
    temp[1][1] = entry0 * b[0][1] + entry1 * b[1][1] + entry2 * b[2][1];
    temp[1][2] = entry0 * b[0][2] + entry1 * b[1][2] + entry2 * b[2][2];

    // column 2
    entry0 = a[2][0];
    entry1 = a[2][1];
    entry2 = a[2][2];
    temp[2][0] = entry0 * b[0][0] + entry1 * b[1][0] + entry2 * b[2][0];
    temp[2][1] = entry0 * b[0][1] + entry1 * b[1][1] + entry2 * b[2][1];
    temp[2][2] = entry0 * b[0][2] + entry1 * b[1][2] + entry2 * b[2][2];

    // column 3
    entry0 = a[3][0];
    entry1 = a[3][1];
    entry2 = a[3][2];
    temp[3][0] = entry0 * b[0][0] + entry1 * b[1][0] + entry2 * b[2][0] + b[3][0];
    temp[3][1] = entry0 * b[0][1] + entry1 * b[1][1] + entry2 * b[2][1] + b[3][1];
    temp[3][2] = entry0 * b[0][2] + entry1 * b[1][2] + entry2 * b[2][2] + b[3][2];

    temp[0][3] = temp[1][3] = temp[2][3] = 0;
    temp[3][3] = 1;

    mtxf_copy(dest, temp);
}

void mtxf_scale_vec3f(f32 a[4][4], f32 b[4][4], Vec3f c)
{
    register s32 i;

    for (i = 0; i < 4; i++)
    {
        a[0][i] = b[0][i] * c[0];
        a[1][i] = b[1][i] * c[1];
        a[2][i] = b[2][i] * c[2];
        a[3][i] = b[3][i];
    }
}

void mtxf_mul_vec3s(f32 a[4][4], Vec3s b)
{
    register f32 x = b[0];
    register f32 y = b[1];
    register f32 z = b[2];

    b[0] = x * a[0][0] + y * a[1][0] + z * a[2][0] + a[3][0];
    b[1] = x * a[0][1] + y * a[1][1] + z * a[2][1] + a[3][1];
    b[2] = x * a[0][2] + y * a[1][2] + z * a[2][2] + a[3][2];
}

void mtxf_to_mtx(Mtx *a, f32 b[4][4])
{
    s32 sp14;
    register s32 i;
    register s16 *a3 = (s16 *)a;
    register s16 *t0 = (s16 *)a + 16;
    register f32 *t1 = (f32 *)b;

    for (i = 0; i < 16; i++)
    {
        sp14 = *t1++ * 0x10000;
        //! endian-dependent code
        *a3++ = GET_HIGH_S16_OF_32(sp14);  // upper bits
        *t0++ = GET_LOW_S16_OF_32(sp14);  // lower bits
    }
}

void mtxf_rotate_xy(Mtx *a, s16 b)
{
    f32 temp[4][4];

    mtxf_identity(temp);
    temp[0][0] = coss(b);
    temp[0][1] = sins(b);
    temp[1][0] = -temp[0][1];
    temp[1][1] = temp[0][0];
    mtxf_to_mtx(a, temp);
}

/**
 * If a = | A r | and b = | B t |
 *        | 0 1 |         | 0 1 |
 * return B^T (r - t). If B is orthogonal, then this is the vector v such that
 * bv = r.
 */
void func_8037A550(Vec3f a, f32 b[4][4], f32 c[4][4])
{
    f32 spC = c[3][0] * c[0][0] + c[3][1] * c[0][1] + c[3][2] * c[0][2];
    f32 sp8 = c[3][0] * c[1][0] + c[3][1] * c[1][1] + c[3][2] * c[1][2];
    f32 sp4 = c[3][0] * c[2][0] + c[3][1] * c[2][1] + c[3][2] * c[2][2];

    a[0] = b[3][0] * c[0][0] + b[3][1] * c[0][1] + b[3][2] * c[0][2] - spC;
    a[1] = b[3][0] * c[1][0] + b[3][1] * c[1][1] + b[3][2] * c[1][2] - sp8;
    a[2] = b[3][0] * c[2][0] + b[3][1] * c[2][1] + b[3][2] * c[2][2] - sp4;
}

void vec3f_get_dist_and_angle(Vec3f a, Vec3f b, f32 *c, s16 *d, s16 *e)
{
    register f32 f20 = b[0] - a[0];
    register f32 f22 = b[1] - a[1];
    register f32 f24 = b[2] - a[2];

    *c = sqrtf(f20 * f20 + f22 * f22 + f24 * f24);
    *d = atan2s(sqrtf(f20 * f20 + f24 * f24), f22);
    *e = atan2s(f24, f20);
}

void vec3f_set_dist_and_angle(Vec3f a, Vec3f b, f32 c, s16 d, s16 e)
{
    b[0] = a[0] + c * coss(d) * sins(e);
    b[1] = a[1] + c * sins(d);
    b[2] = a[2] + c * coss(d) * coss(e);
}

s32 approach_s32(s32 current, s32 target, s32 inc, s32 dec)
{
    //! If target is close to the max or min s32, then it's possible to overflow
    // past it without stopping.

    if (current < target)
    {
        current += inc;
        if (current > target)
            current = target;
    }
    else
    {
        current -= dec;
        if (current < target)
            current = target;
    }
    return current;
}

f32 approach_f32(f32 current, f32 target, f32 inc, f32 dec)
{
    if (current < target)
    {
        current += inc;
        if (current > target)
            current = target;
    }
    else
    {
        current -= dec;
        if (current < target)
            current = target;
    }
    return current;
}

static u16 atan2_lookup(f32 a, f32 b)
{
    u16 ret;

    if (b == 0)
        ret = D_8038B000[0];
    else
        ret = D_8038B000[(s32)(a / b * 1024 + 0.5f)];
    return ret;
}

/**
 * Note that the argument order is swapped from the standard atan2.
 */
s16 atan2s(f32 a, f32 b)
{
    u16 ret;

    if (b >= 0)
    {
        if (a >= 0)
        {
            if (a >= b)
                ret = atan2_lookup(b, a);
            else
                ret = 0x4000 - atan2_lookup(a, b);
        }
        else
        {
            a = -a;
            if (a < b)
                ret = 0x4000 + atan2_lookup(a, b);
            else
                ret = 0x8000 - atan2_lookup(b, a);
        }
    }
    else
    {
        b = -b;
        if (a < 0)
        {
            a = -a;
            if (a >= b)
                ret = 0x8000 + atan2_lookup(b, a);
            else
                ret = 0xC000 - atan2_lookup(a, b);
        }
        else
        {
            if (a < b)
                ret = 0xC000 + atan2_lookup(a, b);
            else
                ret = -atan2_lookup(b, a);
        }
    }
    return ret;
}

/**
 * Note that the argument order is swapped from the standard atan2.
 */
f32 atan2f(f32 a, f32 b)
{
    return (f32)atan2s(a, b) * M_PI / 0x8000;
}

void func_8037ABEC(Vec4f a, f32 b, UNUSED s32 c)
{
    f32 sp14 = 1 - b;
    f32 sp10 = sp14 * sp14;
    f32 spC = sp10 * sp14;
    f32 sp8 = b * b;
    f32 sp4 = sp8 * b;

    switch (D_8038BC98)
    {
    case 1:
        a[0] = spC;
        a[1] = sp4 * 1.75f - sp8 * 4.5f + b * 3.0f;
        a[2] = -sp4 * 0.9166667f + sp8 * 1.5f;
        a[3] = sp4 * 0.16666667f;
        break;
    case 2:
        a[0] = spC * 0.25f;
        a[1] = sp4 * 0.5833333f - sp8 * 1.25f + b * 0.25f + 0.5833333f;
        a[2] = -sp4 * 0.5f + sp8 * 0.5f + b * 0.5f + 0.16666667f;
        a[3] = sp4 * 0.16666667f;
        break;
    case 3:
        a[0] = spC * 0.16666667f;
        a[1] = sp4 * 0.5f - sp8 + 0.6666667f;
        a[2] = -sp4 * 0.5f + sp8 * 0.5f + b * 0.5f + 0.16666667f;
        a[3] = sp4 * 0.16666667f;
        break;
    case 4:
        a[0] = spC * 0.16666667f;
        a[1] = -spC * 0.5f + sp10 * 0.5f + sp14 * 0.5f + 0.16666667f;
        a[2] = spC * 0.5833333f - sp10 * 1.25f + sp14 * 0.25f + 0.5833333f;
        a[3] = sp4 * 0.25f;
        break;
    case 5:
        a[0] = spC * 0.16666667f;
        a[1] = -spC * 0.9166667f + sp10 * 1.5f;
        a[2] = spC * 1.75f - sp10 * 4.5f + sp14 * 3.0f;
        a[3] = sp4;
        break;
    }
}

void func_8037AFB8(Vec4s *a)
{
    D_8038BC90 = a;
    D_8038BC94 = 0;
    D_8038BC98 = 1;
}

s32 func_8037AFE8(Vec3f a)
{
    Vec4f sp30;
    s32 i;
    s32 sp28 = 0;

    vec3f_copy(a, D_80385FD0);
    func_8037ABEC(sp30, D_8038BC94, D_8038BC98);
    for (i = 0; i < 4; i++)
    {
        a[0] += sp30[i] * D_8038BC90[i][1];
        a[1] += sp30[i] * D_8038BC90[i][2];
        a[2] += sp30[i] * D_8038BC90[i][3];
    }
    
    if ((D_8038BC94 += D_8038BC90[0][0] / 1000.0f) >= 1)
    {
        D_8038BC90++;
        D_8038BC94--;
        switch (D_8038BC98)
        {
        case 5:
            sp28 = 1;
            break;
        case 3:
            if (D_8038BC90[2][0] == 0)
                D_8038BC98 = 4;
            break;
        default:
            D_8038BC98++;
            break;
        }
        
    }
    
    return sp28;
}
