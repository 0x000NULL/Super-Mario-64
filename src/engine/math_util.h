#ifndef _MATH_UTIL_H_
#define _MATH_UTIL_H_

extern f32 gSineTable[];
extern f32 gCosineTable[];

#define sins(x) gSineTable[(u16) (x) >> 4]
#define coss(x) gCosineTable[(u16) (x) >> 4]

#define min(a, b) ((a) <= (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))

#define sqr(x) ((x) * (x))

void *vec3f_copy(Vec3f dest, Vec3f src);
void *vec3f_set(Vec3f dest, f32 x, f32 y, f32 z);
void *vec3f_add(Vec3f dest, Vec3f a);
void *vec3f_sum(Vec3f dest, Vec3f a, Vec3f b);
void *vec3s_copy(Vec3s dest, Vec3s src);
void *vec3s_set(Vec3s dest, s16 x, s16 y, s16 z);
void *vec3s_add(Vec3s dest, Vec3s a);
void *vec3s_sum(Vec3s dest, Vec3s a, Vec3s b);
void *vec3s_sub(Vec3s dest, Vec3s a);
void *vec3s_to_vec3f(Vec3f dest, Vec3s a);
void *vec3f_to_vec3s(Vec3s dest, Vec3f a);
void *find_vector_perpendicular_to_plane(Vec3f dest, Vec3f a, Vec3f b, Vec3f c);
void *vec3f_cross(Vec3f dest, Vec3f a, Vec3f b);
void *vec3f_normalize(Vec3f dest);
void mtxf_copy(f32 dest[4][4], f32 src[4][4]);
void mtxf_identity(f32 mtx[4][4]);
void mtxf_translate(f32 a[4][4], Vec3f b);
void func_80378F84(f32 mtx[4][4], Vec3f b, Vec3f c, s16 d);
void mtxf_rotate_zxy_and_translate(f32 mtx[4][4], Vec3f b, Vec3s c);
void mtxf_rotate_xyz_and_translate(f32 mtx[4][4], Vec3f b, Vec3s c);
void func_80379798(f32 mtx1[4][4], f32 mtx2[4][4], Vec3f c, s16 d);
void mtxf_from_up_direction(f32 mtx[4][4], Vec3f b, Vec3f c, s16 d);
void func_80379AA4(f32 mtx[4][4], Vec3f b, s16 c, f32 d);
void mtxf_mul(f32 dest[4][4], f32 a[4][4], f32 b[4][4]);
void mtxf_scale_vec3f(f32 a[4][4], f32 b[4][4], Vec3f c);
void mtxf_mul_vec3s(f32 a[4][4], Vec3s b);
void mtxf_to_mtx(Mtx *a, f32 b[4][4]);
void mtxf_rotate_xy(Mtx *a, s16 b);
void func_8037A550(Vec3f a, f32 b[4][4], f32 c[4][4]);
void vec3f_get_dist_and_angle(Vec3f a, Vec3f b, f32 *c, s16 *d, s16 *e);
void vec3f_set_dist_and_angle(Vec3f a, Vec3f b, f32 c, s16 d, s16 e);
s32 approach_s32(s32 a, s32 b, s32 c, s32 d);
f32 approach_f32(f32 a, f32 b, f32 c, f32 d);
s16 atan2s(f32 a, f32 b);
f32 atan2f(f32 a, f32 b);
void func_8037ABEC(Vec4f a, f32 b, UNUSED s32 c);
void func_8037AFB8(Vec4s *a);
s32 func_8037AFE8(Vec3f a);

#endif
