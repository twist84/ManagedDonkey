#pragma once

#include <intrin.h>

#pragma intrinsic(_byteswap_ushort)
#pragma intrinsic(_byteswap_ulong)
#pragma intrinsic(_byteswap_uint64)

#define bswap_word_inplace(value) value = _byteswap_ushort(value)
#define bswap_dword_inplace(value) value = _byteswap_ulong(value)
#define bswap_qword_inplace(value) value = _byteswap_uint64(value)