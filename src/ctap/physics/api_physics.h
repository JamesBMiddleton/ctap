#pragma once

#include "../../../utils/types.h" //!! 

typedef struct {
    void (*start_gravity) (void);
} phy_t;

extern const phy_t phy;
