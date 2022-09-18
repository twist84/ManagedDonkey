#pragma once

#include "memory/module.hpp"

#include "memory/crc.hpp"

// override any mismatched tag checksums
c_hook_call<0x0050286A> crc_checksum_buffer_alder32_hook({ .pointer = crc_checksum_buffer_adler32 });
