#ifndef CTP_H
#define CTP_H

#include "util/ctp_result.h"

struct ctp_init_args{
    int placeholder;
    // map..
    // input_cb..
};

ctp_result ctp_init(struct ctp_init_args arg);

#endif // CTP_H
