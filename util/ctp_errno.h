#ifndef CTP_ERRNO_H
#define CTP_ERRNO_H

enum ctp_errno {
    CTP_ERRNO_OK,
    CTP_ERRNO_NULL,
    CTP_ERRNO_NOMEM
};

inline void ctp_errno_set(enum ctp_errno err);
inline enum ctp_errno ctp_errno_get(void);

#endif // CTP_ERRNO_H
