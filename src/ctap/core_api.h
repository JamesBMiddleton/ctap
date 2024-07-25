#pragma once

#include "utils/types.h"

typedef struct {
    void (*start_the_engines) (void);
} cor_api_t;

extern const cor_api_t cor_api;
