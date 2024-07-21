#pragma once

#include "../../../utils/types.h" //!! 

typedef struct {
    void (*start_the_engines) (void);
} cor_t;

extern const cor_t cor;
