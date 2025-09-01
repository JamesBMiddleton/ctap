#ifndef CTP_MATH_H
#define CTP_MATH_H

#include <stdbool.h>

static inline unsigned ctp_math_abs(int val);
static inline int ctp_math_powi(int base, unsigned exp);
static inline unsigned ctp_math_powu(unsigned base, unsigned exp);
static inline float ctp_math_powf(float base, int exp);
static inline float ctp_math_fabs(float val);
static inline bool ctp_math_isnan(float val);
static inline bool ctp_math_isinf(float val);

#define ctp_math_min(left, right) (((left) < (right)) ? (left) : (right))
#define ctp_math_max(left, right) (((left) > (right)) ? (left) : (right))

#define CTP_MATH_FLT_EQUAL_DELTA 1E-6F
#define ctp_math_flt_equal(left, right)           \
    ((((left) - (right)) > -(FLT_EQUAL_DELTA)) && \
     (((left) - (right)) < (FLT_EQUAL_DELTA)))

#endif // CTP_MATH_H
