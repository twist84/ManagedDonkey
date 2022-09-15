#include "memory/crc.hpp"

#include "cseries/cseries_windows.hpp"
#include "cache/cache_files.hpp"

#include <stdlib.h>

// this function is closer to the adler32 implementation in zlib
dword __cdecl crc_checksum_buffer_adler32(dword sum, byte* buffer, dword buffer_size)
{
	dword result = INVOKE(0x0052CCC0, crc_checksum_buffer_adler32, sum, buffer, buffer_size);

	dword* checksum = reinterpret_cast<dword*>(buffer - 4);
	if (result != *checksum)
	{
		union { tag t; char s[8]; } group_tag = { .t = _byteswap_ulong(reinterpret_cast<cache_file_tag_instance*>(checksum)->group_tags[0]) };
		display_debug_string("tags: tag instance checksum mismatch calcutaled/expected %u/%u, ['%s', 0x%04X]", result, *checksum, group_tag.s, 0 /* tag index or tag section offset */);

		*checksum = result;
	}

	return result;
}

dword adler_new()
{
	return adler32_update(0, 0, 0);
}

// modified version of https://github.com/skeeto/scratch/blob/365892d47ddb264415b5d9760dcd77c35f72219a/checksums/adler32.h
dword adler32_update(dword sum, void const* data, dword len)
{
	if (!data)
		return 1;

	byte const* p = static_cast<byte const*>(data);

	dword a = sum & 0xffff;
	dword b = sum >> 16;

	for (dword i = 0; i < len; i++)
		b = (b + (a = (a + p[i]) % 65521)) % 65521;

	return (b << 16) | a;
}
