#define NULL_TERMINATOR_SZ 1
#define UINT_MAX_CHARS (10) // '4294967295'
#define UINT_MAX_CHAR_THRESHOLD 1000000000
#define FLOAT_DECIMAL_CHARS (3)
#define NUMERIC_MAX_CHARS \
    (1 + UINT_MAX_CHARS + 1 + FLOAT_DECIMAL_CHARS) // '-2147483648.123'

/*
 * @param str - null terminated string.
 * @return length of str, null terminator not included.
 * */
static inline usize util_Strlen(const char* str)
{
    usize len = 0;
    while (*str++ != '\0')
        ++len;
    return len;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
static char* util_Uinttostr(uint val, char* buf)
{
    static const uchar base = 10;

    if (val >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (uint digits = base; digits <= val; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (val % base));
        val /= base;
    } while (val != 0);
    return buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @return buf
 */
static char* util_Inttostr(int val, char* buf)
{
    static const uchar base = 10;

    uint abs = (uint)val;
    if (val < 0)
    {
        abs = -(uint)val;
        *buf++ = '-';
    }
    if (abs >= UINT_MAX_CHAR_THRESHOLD)
        buf += UINT_MAX_CHARS - 1;
    else
        for (uint digits = base; digits <= abs; digits *= base)
            ++buf;
    *++buf = '\0';
    do
    {
        *--buf = (char)('0' + (abs % base));
        abs /= base;
    } while (abs != 0);
    return (val < 0) ? --buf : buf;
}

/*
 * Assumes buf is larger than num digits + null terminator.
 * We're checking for inf/nan before float casts, so silencing UB sanitizer.
 *
 * @param val - value to convert
 * @param buf - destination string buffer
 * @param decimals - number of decimal places
 * @return buf
 */
__attribute__((no_sanitize("undefined"))) static char*
util_Floattostr(const float val, char* buf, uchar decimals)
{
    static const float base = 10;

    if (util_Isnan(val))
    {
        static const char* nan = "NaN";
        util_Memcpy(buf, nan, util_Strlen(nan) + NULL_TERMINATOR_SZ);
        return buf;
    }

    if (util_Isinf(val))
    {
        static const char* inf = "Inf";
        util_Memcpy(buf, inf, util_Strlen(inf) + NULL_TERMINATOR_SZ);
        return buf;
    }

    int whole = (int)val;
    float fraction = util_Fabs((val - (float)whole));
    char* start = util_Inttostr(whole, buf);

    buf += util_Strlen(buf);
    *buf++ = '.';

    if (decimals--)
    {
        fraction *= base;
        while ((int)fraction == 0 && decimals--)
        {
            *buf++ = '0';
            fraction *= base;
        }
        while (decimals--)
            fraction *= base;
        util_Uinttostr((uint)fraction, buf);
    }
    else
        *buf = '\0';
    return start;
}

/*
 * Takes format specifiers of type %d, %u, %f, %s or %c.
 * e.g. util_sprintf(buf, bufsz, "%f", (util_Fmts){.arr = {{.f = 123.456f}}})
 *
 * @param buf - the destination string buffer
 * @param bufsz - size of destination string buffer
 * @param format - the string format
 * @param vals - struct wrapped array of format specifier values
 * @return - the destination string buffer
 */
__attribute__((used)) static const char* util_Sprintf(char* buf,
                                                      const usize bufsz,
                                                      const char* format,
                                                      const util_Fmts vals)
{
    const char* first = buf;
    const char* last = buf + bufsz - 1;
    usize i_vals = 0;

    while (*format != '\0')
    {
        if (buf == last)
        {
            util_LOG_ERROR("Destination buffer too small.");
            break;
        }

        if (*format != '%')
        {
            *buf++ = *format++;
        }
        else
        {
            if (i_vals == util_MAX_FMT_ARGS)
            {
                util_LOG_ERROR("Too many specifiers in format string.");
                break;
            }

            ++format;
            const char specifier = *format++;
            const util_Fmt val = vals.arr[i_vals++];

            if (((specifier == 'd' || specifier == 'u' || specifier == 'f') &&
                 (buf + NUMERIC_MAX_CHARS) > last) ||
                ((specifier == 's') && (buf + util_Strlen(val.s) > last)))
            {
                util_LOG_ERROR("Destination buffer too small.");
                break;
            }

            switch (specifier)
            {
                case 'c': *buf++ = val.c; continue;
                case 'd': util_Inttostr(val.d, buf); break;
                case 'u': util_Uinttostr(val.u, buf); break;
                case 'f': util_Floattostr(val.f, buf, 3); break;
                case 's': {
                    const char* str = val.s;
                    do
                    {
                        *buf = *str;
                    } while (*str++ != '\0' && ++buf);
                    break;
                }
                default:
                    util_LOG_ERROR("Invalid specifier used.");
                    *buf = '\0';
                    return first;
            }
            buf += util_Strlen(buf);
        }
    }
    *buf = '\0';
    return first;
}
