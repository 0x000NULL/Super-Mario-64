#ifndef _MATRIX_FUNCTIONS_H_
#define _MATRIX_FUNCTIONS_H_

#include <types.h>
#include "gd_types.h"

extern void func_80193B68(Mat4 *, f32, f32, f32, f32, f32, f32, f32, f32, f32);
extern void func_8019415C(Mat4 *, struct MyVec3f *);
extern void func_80194220(Mat4 *, struct MyVec3f *);
extern void func_801942E4(Mat4 *, struct MyVec3f *);
extern void func_80194358(Mat4 *, struct MyVec3f *, f32);
extern f32 func_80194728(f32, f32);     /* min(f1, +/-f2)? */
extern void limit_vec3f(struct MyVec3f *, f32);
extern void func_80194880(f32, f32 *, f32 *);
extern void absrot_mat4(Mat4 *, s32, f32);
extern f32 magnitude_vec3f(struct MyVec3f *);
extern s32 into_unit_vec3f(struct MyVec3f *);
extern void cross_product_vec3f(struct MyVec3f *a, struct MyVec3f *b, struct MyVec3f *dst);
extern f32 dot_product_vec3f(struct MyVec3f *, struct MyVec3f *);
extern void inverse_mat4(Mat4 *, Mat4 *);
extern void func_801961F4(Mat4 *, struct MyVec3f *, f32);
extern void set_identity_mat4(Mat4 *);
extern void cpy_mat4(const Mat4 *, Mat4 *);
extern void func_80196430(struct MyVec3f *, const Mat4 *);
extern void func_80196540(struct MyVec3f *, const Mat4 *);
extern void multiply_mat4(const Mat4 *, const Mat4 *, Mat4 *);
extern void gd_print_vec(const char *, const struct MyVec3f *);
extern void gd_print_plane(const char *, const struct GdPlaneF *);
extern void gd_print_mtx(const char *, const Mat4 *);

#endif /* _MATRIX_FUNCTIONS_H_ */
