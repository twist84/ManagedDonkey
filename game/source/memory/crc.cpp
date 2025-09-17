#include "memory/crc.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CALL(0x0050286A, crc_checksum_buffer_adler32); // 0x0052CCC0
//HOOK_DECLARE(0x0052CD20, crc_checksum_buffer);

const bool g_require_secure_tag_instances = true;

uns32 __cdecl crc_checksum_buffer_adler32(uns32 adler, uns8* buffer, uns32 buffer_size)
{
	//uns32 checksum = INVOKE(0x0052CCC0, crc_checksum_buffer_adler32, adler, buffer, buffer_size);

	uns32 checksum = adler32(adler, buffer, buffer_size);
	cache_file_tag_instance* instance = reinterpret_cast<cache_file_tag_instance*>(buffer - 4);

	if (checksum != instance->checksum)
	{
		if (g_require_secure_tag_instances)
		{
			return instance->checksum;
		}
		else
		{
			char group_string[8]{};
			tag_to_string(instance->tag_group.group_tag, group_string);
			int32 tag_index = g_cache_file_globals.absolute_index_tag_mapping[g_cache_file_globals.tag_loaded_count];
			event(_event_warning, "tags: tag instance checksum mismatch calcutaled/expected %08X/%08X, ['%s', 0x%04X]", checksum, instance->checksum, group_string, tag_index);

			instance->checksum = checksum;
		}
	}

	return checksum;
}

uns32 __cdecl crc_checksum_buffer(uns32 crc, byte* buffer, uns32 buffer_size)
{
	uns32 checksum = crc32(crc, buffer, buffer_size);

	return checksum;
}

uns32 adler_new()
{
	return adler32(0, 0, 0);
}

uns32 adler32(uns32 adler, const byte* buf, uns32 len)
{
	uns32 sum2 = (adler >> 16) & 0xFFFF;
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
		uns32 n = 0x15B0 / 16;

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

uns32 crc_new()
{
	return 0xFFFFFFFF;
}

uns32 __cdecl crc32(uns32 crc, const byte* buf, uns32 len)
{
	static uns32 crc_table[256]{};
	static bool crc_table_initialized = false;

	if (!crc_table_initialized)
	{
		for (int32 uns8 = 0; uns8 < 256; uns8++)
		{
			uns32 crc = static_cast<uns32>(uns8);

			for (int8 bit = 0; bit < 8; bit++)
			{
				if (TEST_BIT(crc, 0))
					crc = (crc >> 1) ^ 0xEDB88320;
				else
					crc >>= 1;
			}

			crc_table[uns8] = crc;
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

