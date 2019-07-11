#ifndef _GD_TEXTURE_DL_H_
#define _GD_TEXTURE_DL_H_

#include "types.h"
#include "gd_types.h"

/* These data arrays were probably in C as data in the mario_head_6
 * but it is easier to leave these as `.incbin`s in an ASM file */

extern Gfx *redStarDlArray[];
extern Gfx *silverStarDlArray[];
extern Gfx *redSparkleDlArray[];
extern Gfx *silverSparkleDlArray[];
extern Gfx marioHeadDl801B5170[];
extern Gfx marioHeadDl801B5188[];
extern Gfx marioHeadDl801B5100[];
extern f32 D_801B520C;
extern u16 textureHandOpen[];
extern u16 textureHandClosed[];
extern Gfx marioHeadDl801B52D8[];

#endif /* _GD_TEXTURE_DL_H_ */
