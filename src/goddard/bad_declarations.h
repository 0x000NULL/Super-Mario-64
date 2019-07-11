#ifndef _BAD_GD_DECLARATIONS_H_
#define _BAD_GD_DECLARATIONS_H_

#include "sm64.h"
#include "gd_types.h"

/**
 * @file bad_declarations.h
 * 
 * Match incorrect type promotion for two declared functions.
 * 
 * There is an issue with the compiled code for these function calls in files
 * outside of the files in which they were defined. 
 * The only possible reason I can come up with for this behavior is that
 * goddard only declared (not prototyped) his functions in the headers,
 * and didn't include the header in the function's defining .c file
 * (Even ido 5.0 cares about illegal promotion of types!)
 *
 * Since that's an awful practice to emulate, include this file (first!) to prevent
 * the proper prototypes of these functions from being seen by files with the 
 * the incorrectly compiled calls.
*/

#if !BUGFIX_GODDARD_BAD_DECLARATIONS

#define GD_USE_BAD_DECLARATIONS 1

/* half6 */ 
extern struct ObjFace *make_face_with_colour(); 
/* should be: make_face_with_colour(f32, f32, f32)
** '0.0, 0.0, 0.0' floats are being incorrectly promoted to doubles */

/* oldobj */ 
extern struct ObjLabel *make_label();
/* should be: make_label(struct ObjValPtrs *, char *, s32, f32, f32, f32)
** But, it seems that goddard called this with '0, 0, 0' for the floats, 
** which ends with them being compiled as ints... */

#endif /* !BUGFIX_GODDARD_BAD_DECLARATIONS */
#endif /* _BAD_GD_DECLARATIONS_H_ */
