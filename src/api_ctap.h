#pragma once

#include "../utils/types.h" //!! "utils/types.h"

typedef struct {
    void (*load_map) (void);
} ctap_t;

extern const ctap_t ctap;
