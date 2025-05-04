#pragma once

#include "cseries/cseries.hpp"

extern uns32 __cdecl crc_checksum_buffer_adler32(uns32 sum, byte* buffer, uns32 buffer_size);
extern uns32 __cdecl crc_checksum_buffer(uns32 checksum, byte* buffer, uns32 buffer_size);

extern uns32 adler_new();
extern uns32 adler32(uns32 adler, const byte* buf, uns32 len);

extern uns32 crc_new();
extern uns32 crc32(uns32 crc, const byte* buf, uns32 len);

