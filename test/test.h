// #include "src/lib.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <stdint.h>
// #include <assert.h>
//
// // --------------------------- PRIMITIVES --------------------------------------
// #ifndef PRIMITIVES
// #define PRIMITIVES
//
// #define bool _Bool
// #define true 1
// #define false 0
// #define NULL ((void*)0)
//
// typedef float f32;
// typedef uint8_t u8;
// typedef uint16_t u16;
// typedef uint32_t u32;
// typedef int8_t i8;
// typedef int16_t i16;
// typedef int32_t i32;
// typedef __SIZE_TYPE__ usize; //! GCC/Clang compiler dependent.
//                              
// #endif // PRIMITIVES
// // -----------------------------------------------------------------------------
//
// void tst_LogUpdateCallback(void);
// void tst_PanicCallback(void);
// void tst_Init(void);
//
// typedef struct {
//     bool suppress_logs;
// } tst_State;
// static tst_State tst_state_mutable;
// static const tst_State* tst_state = &tst_state_mutable;
//
// void tst_LogUpdateCallback(void)
// {
//     if (tst_state->suppress_logs == true)
//         return;
//
//     CTP_Log log = CTP_GetLog();
//
//     const char* lvl = "UNKNOWN";
//     switch (log.lvl)
//     {
//         case CTP_LogLvl_DEBUG:
//             lvl = "DEBUG";
//             break;
//         case CTP_LogLvl_WARN:
//             lvl = "WARN";
//             break;
//         case CTP_LogLvl_ERROR:
//             lvl = "ERROR";
//             break;
//         case CTP_LogLvl_PANIC:
//             lvl = "PANIC";
//             break;
//         case CTP_LogLvl_ASSERT:
//             lvl = "ASSERT";
//             break;
//     }
//     printf("%s | %s::%d | %s\n", lvl, log.func_name, log.line_num, log.message);
//     (void)fflush(stdout);
// }
//
// __attribute__((noreturn)) void tst_PanicCallback(void)
// {
//     (void)fflush(stdout);
//     abort();
// }
//
// // tst_user_stub()
// // tst_str_equals()...
//
// void tst_Init(void)
// {
//     assert(CTP_Init((CTP_InitArg){.log_update_callback = tst_LogUpdateCallback,
//                           .panic_callback = tst_PanicCallback}) == CTP_InitRet_OK);
//     tst_state_mutable.suppress_logs = false;
// }
