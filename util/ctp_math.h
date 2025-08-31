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

#endif // CTP_MATH_H
