#pragma once

#include "../utils/types.h" //!! "utils/types.h"

typedef enum 
{
    ctp_retcode_MAP_INVALID
} ctp_retcode_e;

typedef struct {
    ctp_retcode_e (*load_map) (void);
} ctp_api_t;

extern const ctp_api_t ctp_api;
