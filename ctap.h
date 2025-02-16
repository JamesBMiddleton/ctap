#ifndef CTAP_H
#define CTAP_H

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
    const char* message;
} ctap_Log;

typedef struct {
    // map..
    // input_cb..
    void (*event_handler_log)(const ctap_Log); // Optional.
    void (*event_handler_panic)(void); // Optional.
} ctap_InitArg;
typedef enum { ctap_InitRet_OK, ctap_InitRet_NULL_CALLBACK } ctap_InitRet;
ctap_InitRet ctap_Init(ctap_InitArg arg);

#endif // CTAP_H
