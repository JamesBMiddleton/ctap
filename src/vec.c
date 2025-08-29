#ifndef VEC_C
#define VEC_C
VEC_C

#include "alloc.c"
#define vec_CONCAT(left, right) vec_CONCAT1(left, right)
#define vec_CONCAT1(left, right) left##right
#define vec_TYPE uchar

#endif // VEC_C

#define vec_NAME vec_CONCAT(vec_TYPE, Vec)
#define vec_INIT vec_CONCAT(vec_TYPE, VecInit)
#define vec_PUSH vec_CONCAT(vec_TYPE, VecPush)
#define vec_INSERT vec_CONCAT(vec_TYPE, VecInsert)
#define vec_POP vec_CONCAT(vec_TYPE, VecPop)
#define vec_CLEAR vec_CONCAT(vec_TYPE, VecClear)

typedef struct { 
    vec_TYPE* data;
    usize len;
    usize cap;
} vec_NAME;

static vec_NAME vec_INIT(alloc_Arena* arena)
{
    return (vec_NAME){.cap = arena->cap};
}

static void vec_PUSH(vec_NAME* vec, vec_TYPE elem)
{
}

static void vec_INSERT(vec_NAME* vec, usize pos, vec_TYPE elem)
{
}

static vec_TYPE vec_POP(vec_NAME* vec)
{
    return *(vec->data);
}

static void vec_CLEAR(vec_NAME* vec)
{
}

#undef vec_TYPE
#undef vec_NAME
#undef vec_INIT
#undef vec_PUSH
#undef vec_INSERT
#undef vec_POP
#undef vec_CLEAR
