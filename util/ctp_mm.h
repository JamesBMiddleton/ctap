#include <stddef.h>

/*
 * If aligned copy 32bit chunks from dest to src, else copy bytes.
 * Assumes no overlap.
 *
 * @param dest - copy destination
 * @param src  - copy source
 * @param count - number of bytes to copy
 * @return copy destination
 */
static inline const void* ctp_mm_memcpy(void* dest, const void* src, size_t count)
{
    if (((size_t)src | (size_t)dest | count) & (sizeof(unsigned) - 1))
    {
        const unsigned char* src_byte = (const unsigned char*)src;
        unsigned char* dest_byte = (unsigned char*)dest;
        for (size_t i = 0; i < count; ++i)
            *(dest_byte++) = *(src_byte++);
    }
    else
    {
        const unsigned* src_word = (const unsigned*)src;
        unsigned* dest_word = (unsigned*)dest;
        for (size_t i = 0; i < count; i += sizeof(unsigned))
            *(dest_word++) = *(src_word++);
    }
    return dest;
}

static inline const void* ctp_mm_memset(void);
static inline const void* ctp_mm_bitset(void);
// ...
