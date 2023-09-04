#pragma once

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)

#define bswap_word(value) _byteswap_ushort(value)
#define bswap_dword(value) _byteswap_ulong(value)
#define bswap_qword(value) _byteswap_uint64(value)
#define bswap_word_inplace(value) value = bswap_word(value)
#define bswap_dword_inplace(value) value = bswap_dword(value)
#define bswap_qword_inplace(value) value = bswap_qword(value)