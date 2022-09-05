#include "memory/crc.hpp"

dword __cdecl crc_checksum_buffer_alder32(dword seed, byte* buffer, dword buffer_size)
{
	dword result = DECLTHUNK(0x0052CCC0, crc_checksum_buffer_alder32, seed, buffer, buffer_size);

	dword* checksum = reinterpret_cast<dword*>(buffer - 4);
	if (result != *checksum)
	{
		//cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(checksum);
		//union { tag t; char s[8]; } group_tag = { .t = _byteswap_ulong(instance->group_tags[0]) };
		//display_debug_string("tags: tag instance checksum mismatch calcutaled/expected %u/%u, ['%s', 0x%04X]", result, *checksum, group_tag.s, 0);

		*checksum = result;
	}

	return result;
}
