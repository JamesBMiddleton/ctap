#ifndef CTP_GFX_H
#define CTP_GFX_H

#include "util/ctp_result.h"

struct ctp_gfx_init_args {
    unsigned placeholder;
};

ctp_result ctp_gfx_init(struct ctp_gfx_init_args args);

#endif // CTP_GFX_H
