#define MAX_LOG_SZ 256

typedef enum {
    CTP_LogLvl_ASSERT,
    CTP_LogLvl_DEBUG,
    CTP_LogLvl_WARN,
    CTP_LogLvl_ERROR,
    CTP_LogLvl_PANIC
} CTP_LogLvl;

typedef struct {
    CTP_LogLvl lvl;
    unsigned int line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} CTP_Log;

typedef struct {
    // map..
    // input_cb..
    void (*log_update_callback)(void); // Optional.
    void (*panic_callback)(void); // Optional.
} CTP_InitArg;
typedef enum { CTP_InitRet_OK, CTP_InitRet_NULL_CALLBACK } CTP_InitRet;
CTP_InitRet CTP_Init(CTP_InitArg arg);

CTP_Log CTP_GetLog(void);
