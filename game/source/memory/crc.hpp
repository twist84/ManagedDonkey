#pragma once

#include "cseries/cseries.hpp"

extern uint32 __cdecl crc_checksum_buffer_adler32(uint32 sum, uint8* buffer, uint32 buffer_size);
extern uint32 __cdecl crc_checksum_buffer(uint32 checksum, uint8* buffer, uint32 buffer_size);

extern uint32 adler_new();
extern uint32 adler32(uint32 adler, const uint8* buf, uint32 len);

extern uint32 crc_new();
extern uint32 crc32(uint32 crc, const uint8* buf, uint32 len);

