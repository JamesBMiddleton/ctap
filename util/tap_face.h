#ifndef TAP_FACE_H
#define TAP_FACE_H

#include "tap_vec.h"
#include "tap_color.h"

typedef struct {
    TapVec3 vertices[3];
    TapColor color;
} TapFace;

#endif /* TAP_FACE_H */
