#include "memory/crc.hpp"

#include "cache/cache_files.hpp"
#include "cseries/console.hpp"
#include "cseries/cseries_windows.hpp"
#include "memory/module.hpp"


HOOK_DECLARE_CALL(0x0050286A, crc_checksum_buffer_adler32); // 0x0052CCC0
//HOOK_DECLARE(0x0052CD20, crc_checksum_buffer);

const bool g_require_secure_tag_instances = true;

dword __cdecl crc_checksum_buffer_adler32(dword adler, byte* buffer, dword buffer_size)
{
	//dword checksum = INVOKE(0x0052CCC0, crc_checksum_buffer_adler32, adler, buffer, buffer_size);

	dword checksum = adler32(adler, buffer, buffer_size);
	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(buffer - 4);

	if (checksum != instance->checksum)
	{
		if (!g_require_secure_tag_instances)
			return instance->checksum;

		char group_string[8]{};
		tag_to_string(instance->group_tags[0], group_string);
		display_debug_string("tags: tag instance checksum mismatch calcutaled/expected %08u/%08u, ['%s', 0x%04X]", checksum, instance->checksum, group_string, 0 /* tag index or tag section offset */);

		instance->checksum = checksum;
	}

	return checksum;
}

dword __cdecl crc_checksum_buffer(dword crc, byte* buffer, dword buffer_size)
{
	dword checksum = crc32(crc, buffer, buffer_size);

	return checksum;
}

dword adler_new()
{
	return adler32(0, 0, 0);
}

dword adler32(dword adler, const byte* buf, dword len)
{
	dword sum2 = (adler >> 16) & 0xFFFF;
	adler &= 0xFFFF;

	if (len == 1ul)
	{
		adler += buf[0];

		if (adler >= 0xFFF1ul)
			adler -= 0xFFF1ul;

		sum2 += adler;

		if (sum2 >= 0xFFF1ul)
			sum2 -= 0xFFF1ul;

		return adler | (sum2 << 16);
	}

	if (buf == nullptr)
		return 1ul;

	if (len < 16)
	{
		while (len--)
		{
			adler += *buf++;
			sum2 += adler;
		}

		if (adler >= 0xFFF1ul)
			adler -= 0xFFF1ul;

		sum2 %= 0xFFF1ul;

		return adler | (sum2 << 16);
	}

	while (len >= 0x15B0)
	{
		len -= 0x15B0;
		dword n = 0x15B0 / 16;

		do
		{
			adler += (buf)[0];  sum2 += adler;
			adler += (buf)[1];  sum2 += adler;
			adler += (buf)[2];  sum2 += adler;
			adler += (buf)[3];  sum2 += adler;
			adler += (buf)[4];  sum2 += adler;
			adler += (buf)[5];  sum2 += adler;
			adler += (buf)[6];  sum2 += adler;
			adler += (buf)[7];  sum2 += adler;
			adler += (buf)[8];  sum2 += adler;
			adler += (buf)[9];  sum2 += adler;
			adler += (buf)[10]; sum2 += adler;
			adler += (buf)[11]; sum2 += adler;
			adler += (buf)[12]; sum2 += adler;
			adler += (buf)[13]; sum2 += adler;
			adler += (buf)[14]; sum2 += adler;
			adler += (buf)[15]; sum2 += adler;

			buf += 16;
		} while (--n);

		adler %= 0xFFF1ul;
		sum2 %= 0xFFF1ul;
	}

	if (len)
	{
		while (len >= 16)
		{
			len -= 16;

			adler += (buf)[0];  sum2 += adler;
			adler += (buf)[1];  sum2 += adler;
			adler += (buf)[2];  sum2 += adler;
			adler += (buf)[3];  sum2 += adler;
			adler += (buf)[4];  sum2 += adler;
			adler += (buf)[5];  sum2 += adler;
			adler += (buf)[6];  sum2 += adler;
			adler += (buf)[7];  sum2 += adler;
			adler += (buf)[8];  sum2 += adler;
			adler += (buf)[9];  sum2 += adler;
			adler += (buf)[10]; sum2 += adler;
			adler += (buf)[11]; sum2 += adler;
			adler += (buf)[12]; sum2 += adler;
			adler += (buf)[13]; sum2 += adler;
			adler += (buf)[14]; sum2 += adler;
			adler += (buf)[15]; sum2 += adler;

			buf += 16;
		}

		while (len--)
		{
			adler += *buf++;
			sum2 += adler;
		}

		adler %= 0xFFF1ul;
		sum2 %= 0xFFF1ul;
	}

	return adler | (sum2 << 16);
}

dword crc_new()
{
	return 0xFFFFFFFF;
}

dword __cdecl crc32(dword crc, const byte* buf, dword len)
{
	static dword crc_table[256]{};
	static bool crc_table_initialized = false;

	if (!crc_table_initialized)
	{
		for (long byte = 0; byte < 256; byte++)
		{
			dword crc = static_cast<dword>(byte);

			for (char bit = 0; bit < 8; bit++)
			{
				if (TEST_BIT(crc, 0))
					crc = (crc >> 1) ^ 0xEDB88320;
				else
					crc >>= 1;
			}

			crc_table[byte] = crc;
		}

		ASSERT(crc_table[128] == 0xEDB88320);

		crc_table_initialized = true;
	}

	if (len <= 0)
		return crc;

	while (len-- > 0)
	{
		crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
	}

	return crc;
}

