#include "text/font_group.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065B620, font_header_validate);

bool __cdecl font_character_validate(s_font_character const* character)
{
	return INVOKE(0x0065B4B0, font_character_validate, character);
}

short __cdecl font_get_kerning_pair_offset(s_font_header const* header, dword a2, dword a3)
{
	return INVOKE(0x0065B560, font_get_kerning_pair_offset, header, a2, a3);
}

long __cdecl font_get_line_height(s_font_header const* header)
{
	//return INVOKE(0x0065B5E0, font_get_line_height, header);

	if (header)
		return header->__unknown26 + header->__unknown28 + header->__unknown24;

	return 10;
}

void __cdecl font_header_byteswap(s_font_header* header)
{
	INVOKE(0x0065B610, font_header_byteswap, header);
}

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

	//s_font_header_mcc const* header_mcc = reinterpret_cast<s_font_header_mcc const*>(header);
	//
	//bool valid = header_mcc->version == 0xF0000006;
	//if (!valid)
	//	generate_event(_event_level_error, "fonts: header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
	//		header_mcc->version,
	//		0xF0000006);
	//
	//valid &= header_mcc->__unknown24 < 0x8000u && header_mcc->__unknown24 <= 512;
	//valid &= header_mcc->__unknown26 < 0x8000u && header_mcc->__unknown26 <= 512;
	//valid &= header_mcc->__unknown28 < 0x8000u && header_mcc->__unknown28 <= 512;
	//valid &= header_mcc->__unknown2A < 0x8000u && header_mcc->__unknown2A <= 768
	//	&& header_mcc->__unknown24 + header_mcc->__unknown26 > 0;
	//valid &= header_mcc->kerning_pair_count >= 0 && header_mcc->kerning_pair_count <= 255;
	//
	//long kerning_pair = 0;
	//for (long i = 0; i <= NUMBEROF(header_mcc->kerning_pairs) - 1; i++)
	//{
	//	valid &= header_mcc->kerning_pairs[i] <= header_mcc->kerning_pair_count
	//		&& header_mcc->kerning_pairs[i] >= (kerning_pair = header_mcc->kerning_pairs[i]);
	//}
	//
	//valid &= header_mcc->location_table_offset >= (header_mcc->kerning_pairs_offset + sizeof(s_kerning_pair) * header->kerning_pair_count);
	//valid &= header_mcc->location_table_count >= 0 && header_mcc->location_table_count <= 0x10000;
	//valid &= header_mcc->__unknown13C > 0
	//	&& header_mcc->__unknown13C <= 0x10000
	//	&& header_mcc->__unknown13C <= header_mcc->location_table_count;
	//
	//if (valid && header_mcc->__unknown148 != NONE)
	//{
	//	long v1 = (header_mcc->__unknown148 & 0xFFE00000) >> 21;
	//	long v2 = (header_mcc->__unknown148 & 0x1FFFFF + v1);
	//	valid &= v1 && v1 <= 0x1802
	//		&& v2 <= header_mcc->character_data_size_bytes / 8;
	//}
	//
	//valid &= header_mcc->__unknown150 > 0 && header_mcc->__unknown150 <= 0xC000;
	//valid &= header_mcc->__unknown154 > 0 && header_mcc->__unknown154 <= 0xC0000;
	//
	//if (header_mcc->__unknown160 > 0)
	//	valid &= header_mcc->__unknown158 <= 0x8000000;
	//
	//return valid;
}

void __cdecl font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, long kerning_pair_count)
{
	INVOKE(0x0065B810, font_kerning_pairs_byteswap, kerning_pairs, kerning_pair_count);
}

