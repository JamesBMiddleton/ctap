#pragma once

#include "../../../utils/types.h" //!!

typedef struct {
    void (*start_gravity)(void);
} phy_api_t;

extern const phy_api_t phy_api;
