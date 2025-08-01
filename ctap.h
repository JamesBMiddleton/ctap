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
    unsigned int lineNum;
    const char* funcName;
    const char* message;
} ctap_Log;

typedef struct {
    // map..
    // input_cb..
    void (*eventHandlerLog)(const ctap_Log); // Optional.
    void (*eventHandlerPanic)(void); // Optional.
} ctap_InitA;
typedef enum { ctap_InitR_OK, ctap_InitR_NULL_CALLBACK } ctap_InitR;
ctap_InitR ctap_Init(ctap_InitA arg);

#endif // CTAP_H
