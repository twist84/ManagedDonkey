#pragma once

#include "cseries/cseries.hpp"

extern dword __cdecl crc_checksum_buffer_adler32(dword sum, byte* buffer, dword buffer_size);
extern dword __cdecl crc_checksum_buffer(dword checksum, byte* buffer, dword buffer_size);

extern dword adler_new();
extern dword adler32(dword adler, const byte* buf, dword len);

extern dword crc_new();
extern dword crc32(dword crc, const byte* buf, dword len);

