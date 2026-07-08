#ifndef TAP_PHYS_H
#define TAP_PHYS_H

#include "util/tap_result.h"
#include "util/tap_vec.h"

typedef struct {
    float yaw;
    float pitch;
    float forward;
    float side;
    float up;
} TapPhysPlayerMoveDeltas;

typedef struct {
    TapVec3 position;
    float yaw;
    float pitch;
} TapPhysPlayerPose;

TapResult tap_phys_init(void);
TapResult tap_phys_cycle(const TapPhysPlayerMoveDeltas *in_movedeltas, TapPhysPlayerPose *out_pose);
TapResult tap_phys_deinit(void);

#endif // TAP_PHYS_H 
