#include "text/font_group.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065B620, font_header_validate);

bool __cdecl font_header_validate(s_font_header const* header)
{
	//return INVOKE(0x0065B620, font_header_validate, header);

	ASSERT(header);

	bool valid = header->version == 0xF0000005;
	if (!valid)
		generate_event(_event_level_error, "fonts: header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			header->version,
			0xF0000005);

	valid &= header->__unknown24 < 0x8000u && header->__unknown24 <= 64;
	valid &= header->__unknown26 < 0x8000u && header->__unknown26 <= 64;
	valid &= header->__unknown28 < 0x8000u && header->__unknown28 <= 64;
	valid &= header->__unknown2A < 0x8000u && header->__unknown2A <= 256
		  && header->__unknown24 + header->__unknown26 > 0;
	valid &= header->kerning_pair_count >= 0 && header->kerning_pair_count <= 255;

	long kerning_pair = 0;
	for (long i = 0; i <= NUMBEROF(header->kerning_pairs) - 1; i++)
	{
		valid &= header->kerning_pairs[i] <= header->kerning_pair_count
			  && header->kerning_pairs[i] >= (kerning_pair = header->kerning_pairs[i]);
	}

	valid &= header->location_table_offset >= (header->kerning_pairs_offset + sizeof(s_kerning_pair) * header->kerning_pair_count);
	valid &= header->location_table_count >= 0 && header->location_table_count <= 0x10000;
	valid &= header->__unknown13C > 0
		  && header->__unknown13C <= 0x10000
		  && header->__unknown13C <= header->location_table_count;

	if (valid && header->__unknown148 != NONE)
	{
		long v1 = (header->__unknown148 & 0xFFE00000) >> 21;
		long v2 = (header->__unknown148 & 0x1FFFFF + v1);
		valid &= v1 && v1 <= 0x402
			  && v2 <= header->character_data_size_bytes / 8;
	}

	valid &= header->__unknown14C > 0 && header->__unknown14C <= 0x2000;
	valid &= header->__unknown150 > 0 && header->__unknown150 <= 0x8000;
	valid &= header->__unknown154 > 0 && header->__unknown154 <= 0x8000000;

	return valid;
}

