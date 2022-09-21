#include "memory/crc.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_windows.hpp"
#include "memory/module.hpp"

c_hook_call crc_checksum_buffer_alder32_hook(0x0050286A, { .pointer = crc_checksum_buffer_adler32 });

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
