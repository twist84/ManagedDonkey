#include "cseries/cseries.hpp"

#include "memory/byte_swapping.hpp"
#include "tag_files/string_ids.hpp"

#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

REFERENCE_DECLARE(0x0189CD54, real_argb_color const* const, global_real_argb_white);
REFERENCE_DECLARE(0x0189CD58, real_argb_color const* const, global_real_argb_grey);
REFERENCE_DECLARE(0x0189CD5C, real_argb_color const* const, global_real_argb_black);
REFERENCE_DECLARE(0x0189CD60, real_argb_color const* const, global_real_argb_red);
REFERENCE_DECLARE(0x0189CD64, real_argb_color const* const, global_real_argb_green);
REFERENCE_DECLARE(0x0189CD68, real_argb_color const* const, global_real_argb_blue);
REFERENCE_DECLARE(0x0189CD6C, real_argb_color const* const, global_real_argb_yellow);
REFERENCE_DECLARE(0x0189CD70, real_argb_color const* const, global_real_argb_cyan);
REFERENCE_DECLARE(0x0189CD74, real_argb_color const* const, global_real_argb_magenta);
REFERENCE_DECLARE(0x0189CD78, real_argb_color const* const, global_real_argb_pink);
REFERENCE_DECLARE(0x0189CD7C, real_argb_color const* const, global_real_argb_lightblue);
REFERENCE_DECLARE(0x0189CD80, real_argb_color const* const, global_real_argb_orange);
REFERENCE_DECLARE(0x0189CD84, real_argb_color const* const, global_real_argb_purple);
REFERENCE_DECLARE(0x0189CD88, real_argb_color const* const, global_real_argb_aqua);
REFERENCE_DECLARE(0x0189CD8C, real_argb_color const* const, global_real_argb_darkgreen);
REFERENCE_DECLARE(0x0189CD90, real_argb_color const* const, global_real_argb_salmon);
REFERENCE_DECLARE(0x0189CD94, real_argb_color const* const, global_real_argb_violet);

int (__cdecl* csmemcmp)(void const* _Buf1, void const* _Buf2, size_t _Size) = memcmp;
void* (__cdecl* csmemcpy)(void* _Dst, void const* _Src, size_t _Size) = memcpy;
void* (__cdecl* csmemset)(void* _Dst, int _Val, size_t _Size) = memset;

#define MAXIMUM_STRING_SIZE 0x100000

long csstricmp(char const* s1, char const* s2)
{
    return _stricmp(s1, s2);
}

//long csstrnicmp(char const* s1, char const* s2, dword size)
//char* csstristr(char const* s1, char const* s2)

char* csstrnzcpy(char* s1, char const* s2, dword size)
{
    assert(s1 && s2);
    assert(size > 0 && size <= MAXIMUM_STRING_SIZE);

    strncpy_s(s1, size, s2, size);
    s1[size - 1] = 0;

    size_t s2_size = strlen(s2);
    memset(s1 + s2_size, 0, size - s2_size);

    return s1;
}

char* csstrnzcat(char* s1, char const* s2, dword size)
{
    assert(s1 && s2);
    assert(size > 0 && size <= MAXIMUM_STRING_SIZE);

    dword len = csstrnlen(s1, size);
    return csstrnzcpy(s1 + len, s2, size - len);
}

dword csstrnlen(char const* s, dword size)
{
    assert(s);
    assert(size > 0 && size <= MAXIMUM_STRING_SIZE);

    return strnlen(s, size);
}

char* csstrnupr(char* s, dword size)
{
    assert(s);
    assert(size >= 0 && size <= MAXIMUM_STRING_SIZE);

    for (dword i = 0; i < size; i++)
        s[i] = toupper(s[i]);

    return s;
}

char* csstrnlwr(char* s, dword size)
{
    assert(s);
    assert(size >= 0 && size <= MAXIMUM_STRING_SIZE);

    for (dword i = 0; i < size; i++)
        s[i] = tolower(s[i]);

    return s;
}

char const* csstrstr(char const* s1, char const* s2)
{
    return strstr(s1, s2);
}

//char* csstrtok(char*, char const*, bool, struct csstrtok_data* data)

long cvsnzprintf(char* buffer, dword size, char const* format, va_list list)
{
    assert(buffer);
    assert(format);
    assert(size > 0);

    long result = vsnprintf(buffer, size - 1, format, list);
    buffer[size - 1] = 0;

    size_t buf_size = strlen(buffer);
    memset(buffer + buf_size, 0, size - buf_size);

    return result;
}

char* csnzprintf(char* buffer, dword size, char const* format, ...)
{
    va_list list;
    va_start(list, format);

    cvsnzprintf(buffer, size, format, list);

    va_end(list);

    return buffer;
}

char* csnzappendf(char* buffer, dword size, char const* format, ...)
{
    dword current_length = strlen(buffer);
    assert(current_length >= 0 && current_length < size);

    va_list list;
    va_start(list, format);

    cvsnzprintf(&buffer[current_length], size - current_length, format, list);

    va_end(list);

    return buffer;
}

bool string_is_not_empty(char const* s)
{
    return s && *s;
}

char* tag_to_string(tag _tag, char* buffer)
{
    *(tag*)buffer = _byteswap_ulong(_tag);
    buffer[4] = 0;

    return buffer;
}

char const* c_string_id::get_string()
{
    return string_id_get_string_const(m_value);
}

char const* c_string_id::get_string() const
{
    return string_id_get_string_const(m_value);
}

__int64 make_int64(__int64 a, __int64 b)
{
    return ((a << 0) | (b << 32));
}
