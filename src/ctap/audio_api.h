#pragma once

#include "utils/types.h" 

typedef struct {
    void (*start_the_audio) (void);
} aud_api_t;

extern const aud_api_t aud_api;
