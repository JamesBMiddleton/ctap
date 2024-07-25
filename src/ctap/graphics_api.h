#pragma once

#include "utils/types.h" 

typedef struct {
    void (*start_the_graphics) (void);
} gfx_api_t;

extern const gfx_api_t gfx_api;
