/*
    Author: James Middleton
    Repo: https://github.com/JamesBMiddleton/ctap
    License: MIT
    Docs: EOF
*/

// #define ctp_IMPL

typedef float f32;
typedef double f64;
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned int u32;
typedef unsigned long int u64;
typedef signed char i8;
typedef signed short int i16;
typedef signed int i32;
typedef signed long int i64;
typedef __SIZE_TYPE__ usize;
#define bool _Bool
#define true 1
#define false 0
#define NULL ((void*)0)

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CTAP API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#define MAX_LOG_SZ 256

typedef enum {
    ctp_loglvl_DEBUG,
    ctp_loglvl_WARN,
    ctp_loglvl_ERROR,
    ctp_loglvl_PANIC
} ctp_loglvl_e;

typedef struct {
    const ctp_loglvl_e lvl;
    const u32 line_num;
    const char* func_name;
    char message[MAX_LOG_SZ];
} ctp_log_t;

typedef enum { ctp_retcode_MAP_INVALID } ctp_retcode_e;

ctp_retcode_e ctp_load_map(void);

static void (*ctp_log_cb)(const ctp_log_t log);
static void (*ctp_panic_cb)(void);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// CORE API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

typedef enum { cor_retcode_MAP_INVALID } cor_retcode_e;

static cor_retcode_e cor_start_the_engines(void);

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// GRAPHICS API ///////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// PHYSICS API ////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// INPUT API //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// AUDIO API //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// UTILS ///////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

// assumes no overlap
// redundant void* cast to silence -Wcast-align
// if aligned, copy u32 chunks, else copy individual bytes
static void* utl_memcpy(void* dest, const void* src, const usize count)
{
    if (((usize)src | (usize)dest | count) & 3)
    {
        const u8* src_byte = (const u8*)src;
        const u8* end_byte = src_byte + count;
        u8* dest_byte = (u8*)dest;
        while (src_byte != end_byte)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const u32* src_word = (const u32*)src;
        u32* dest_word = (u32*)dest;
        const u32* end_word =
            (const u32*)((const void*)((const u8*)src_word + count));
        while (src_word != end_word)
            *(dest_word++) = *(src_word++);
    }
    return dest;
}
//itoa

// very simple sprintf with a single arg
static void nano_sprintf(char* buf, usize bufsz, const char* format,
                         void* value)
{
    buf[0] = 'x';
    buf[1] = '\0';
    if (format)
    {
    }
    if (value)
    {
    }
    if (bufsz)
    {
    }
    if (NULL)
    {
    }

    // check if length of 'format' + max length that 'value' could be is more
    // than 'bufsz'
    // find the %, check whether it should be treated as a string,
    // u32, u64 etc. - dangerous if we get it wrong? void*
    // construct the formatted string in the buf
}

static ctp_log_t new_log(ctp_loglvl_e lvl, u32 line_num, const char* func_name,
                         const char* format, void* value)
{
    ctp_log_t log = {.lvl = lvl, .line_num = line_num, .func_name = func_name};
    nano_sprintf(&log.message[0], MAX_LOG_SZ, format, value);
    return log;
}

#ifdef DEBUG
#define LOG_D(format, value)                                             \
    do                                                                   \
    {                                                                    \
        ctp_log_cb(new_log(ctp_loglvl_DEBUG, __LINE__, __func__, format, \
                           (void*)value));                               \
    } while (0)
#else
#define LOG_D(...)
#endif

// #define LOG_W(...)                                                      \
//     do                                                                  \
//     {                                                                   \
//         ctp_logger_cb("WARN  | ctap::%s::%d:    ", __func__, __LINE__); \
//         ctp_logger_cb(__VA_ARGS__), ctp_logger_cb("\n");                \
//     } while (0)
//
// #define LOG_E(...)                                                      \
//     do                                                                  \
//     {                                                                   \
//         ctp_logger_cb("ERROR | ctap::%s::%d:    ", __func__, __LINE__); \
//         ctp_logger_cb(__VA_ARGS__), ctp_logger_cb("\n");                \
//     } while (0)
//
// #define PANIC(...)                                                      \
//     do                                                                  \
//     {                                                                   \
//         ctp_logger_cb("ERROR | ctap::%s::%d:    ", __func__, __LINE__); \
//         ctp_logger_cb(__VA_ARGS__), ctp_logger_cb("\n");                \
//         ctp_abort_cb();                                                 \
//     } while (0)

// #define ASSIGN_IF_ZERO(val, def)               \
//     do                                         \
//     {                                          \
//         ((val) = ((val) == 0) ? (def) : (val)) \
//     } while (0)

// #ifdef DEBUG
// #define ASSERT(cond)                                                           \
//     do                                                                         \
//     {                                                                          \
//         if (!(cond))                                                           \
//             (ctp_logger_cb("ASSERT | ctap::%s::%d   %s\n", __func__, __LINE__, \
//                            #cond),                                             \
//              ctp_abort_cb());                                                  \
//     } while (0)
// #else
// #define ASSERT(...)
// #endif

#define STATIC_ASSERT(cond, msg) \
    typedef char static_assert_##msg[(cond) ? 1 : -1]

STATIC_ASSERT(sizeof(u8) == 1, u8_1_byte);
STATIC_ASSERT(sizeof(u16) == 2, u16_2_bytes);
STATIC_ASSERT(sizeof(u32) == 4, u32_4_bytes);
STATIC_ASSERT(sizeof(u64) == 8, u64_8_bytes);
STATIC_ASSERT(sizeof(i8) == 1, i8_1_byte);
STATIC_ASSERT(sizeof(i16) == 2, i16_2_bytes);
STATIC_ASSERT(sizeof(i32) == 4, i32_4_bytes);
STATIC_ASSERT(sizeof(i64) == 8, i64_8_bytes);
STATIC_ASSERT(sizeof(f64) == 8, f64_8_bytes);
STATIC_ASSERT(sizeof(f32) == 4, f32_4_bytes);

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CTAP IMPLEMENTATION //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

ctp_retcode_e ctp_load_map(void)
{
    cor_start_the_engines();
    return ctp_retcode_MAP_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
///////////////////////////// CORE IMPLEMENTATION //////////////////////////////
////////////////////////////////////////////////////////////////////////////////

static cor_retcode_e cor_start_the_engines(void)
{
    bool i = true;
    // bool* p = NULL;
    i = false;
    LOG_D("Hello world! %d", i);
    // ctp_log_cb("hello %s", "world");
    // ASSERT(p);
    return cor_retcode_MAP_INVALID;
}

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// GRAPHICS IMPLEMENTATION ////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// PHYSICS IMPLEMENTATION /////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// INPUT IMPLEMENTATION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/////////////////////////// AUDIO IMPLEMENTATION ///////////////////////////////
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
///////////////////////////////// LICENSE //////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
 
MIT License

Copyright (c) 2024 JamesBMiddleton

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/

////////////////////////////////////////////////////////////////////////////////
/////////////////////////////// DOCUMENTATION //////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/*
 
    __Hello world!__

*/
