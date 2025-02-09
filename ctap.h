#ifndef CTAP_H
#define CTAP_H

#define MAX_LOG_SZ 256

typedef enum {
    ctap_LogLvl_ASSERT,
    ctap_LogLvl_DEBUG,
    ctap_LogLvl_WARN,
    ctap_LogLvl_ERROR,
    ctap_LogLvl_PANIC
} ctap_LogLvl;

typedef struct {
    ctap_LogLvl lvl;
    unsigned int line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} ctap_Log;

typedef struct {
    // map..
    // input_cb..
    void (*log_update_callback)(void); // Optional.
    void (*panic_callback)(void); // Optional.
} ctap_InitArg;
typedef enum { ctap_InitRet_OK, ctap_InitRet_NULL_CALLBACK } ctap_InitRet;
ctap_InitRet ctap_Init(ctap_InitArg arg);

ctap_Log ctap_GetLog(void);

#endif // CTAP_H
