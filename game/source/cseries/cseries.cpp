#include "cseries/cseries.hpp"

#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAXIMUM_STRING_SIZE 0x100000

//long csstricmp(charchar const* s1, char const* s1)
//long csstrnicmp(char const* s1, char const* s1, dword size)
//char* csstristr(char const* s1, char const* s1)

char* csstrnzcpy(char* s1, char const* s2, dword size)
{
    assert(s1 && s2);
    assert(size > 0 && size <= MAXIMUM_STRING_SIZE);

    strncpy_s(s1, size, s2, size);
    s1[size - 1] = 0;

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

//char* csstrnupr(char* s, dword size)
//char* csstrnlwr(char* s, dword size)
//char* csstrtok(char*, char const*, bool, struct csstrtok_data* data)

long cvsnzprintf(char* buffer, dword size, char const* format, char* list)
{
    assert(buffer);
    assert(format);
    assert(size > 0);

    long result = vsnprintf(buffer, size - 1, format, list);
    buffer[size - 1] = 0;

    return result;
}
