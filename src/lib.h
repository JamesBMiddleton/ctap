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
CTP_InitRet CTP_init(CTP_InitArg arg);

CTP_Log CTP_get_log(void);

// typedef struct {
//     u32 frameWidth;
//     u32 frameLength;
//     enum { CTP_GetFrameArgBitrate_OPTION1 } bitrate;
// } CTP_GetFrameArg_t;
// typedef struct {
//     struct {
//         enum { 
//             CTP_GetFrameSize_OPT1, 
//             CTP_GetFrameSize_OPT2 
//         } frameSize;
//     } data;
//     enum { CTP_GetFrameRet_OK, CTP_GetFrameRet_NOTOK } rc;
// } CTP_GetFrameRet_t;
// CTP_GetFrameRet_t CTP_GetFrame(CTP_GetFrameArg_t arg);

