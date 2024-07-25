#pragma once

#include "utils/types.h" 

typedef struct {
    void (*start_the_input) (void);
} inp_api_t;

extern const inp_api_t inp_api;
