#include "text/font_group.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065B620, font_header_validate);

bool __cdecl font_character_validate(s_font_character const* character)
{
	return INVOKE(0x0065B4B0, font_character_validate, character);

	//ASSERT(character);
	//short character_width = character->character_width;
	//if (character->character_width < 0)
	//	character_width = -character_width;
	//short initial_offset = character->initial_offset;
	//if (initial_offset < 0)
	//	initial_offset = -initial_offset;
	//bool valid = initial_offset < 256
	//	&& character->bitmap_height < 64
	//	&& character->bitmap_width < 256
	//	&& character->packed_size < 0x2000
	//	&& character_width <= 256;
	//short bitmap_origin_y = character->bitmap_origin_y;
	//if (bitmap_origin_y < 0)
	//	bitmap_origin_y = -bitmap_origin_y;
	//return bitmap_origin_y < 64 && valid;
}

short __cdecl font_get_kerning_pair_offset(s_font_header const* header, uint32 a2, uint32 a3)
{
	return INVOKE(0x0065B560, font_get_kerning_pair_offset, header, a2, a3);
}

long __cdecl font_get_line_height(s_font_header const* header)
{
	//return INVOKE(0x0065B5E0, font_get_line_height, header);

	if (header)
		return header->descending_height + header->leading_height + header->ascending_height;

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

	bool valid = header->version == k_latest_font_header_version;
	if (!valid)
		event(_event_error, "fonts: header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			header->version,
			k_latest_font_header_version);

	valid &= IN_RANGE_INCLUSIVE(header->ascending_height, 0, 64);
	valid &= IN_RANGE_INCLUSIVE(header->descending_height, 0, 64);
	valid &= IN_RANGE_INCLUSIVE(header->leading_height, 0, 64);
	valid &= IN_RANGE_INCLUSIVE(header->leading_width, 0, 256) && header->ascending_height + header->descending_height > 0;
	valid &= IN_RANGE_INCLUSIVE(header->kerning_pair_count, 0, k_font_header_kerning_pair_index_count - 1);

	long kerning_pair = 0;
	for (long i = 0; i <= k_font_header_kerning_pair_index_count - 1; i++)
	{
		valid &= header->character_first_kerning_pair_index[i] <= header->kerning_pair_count
			&& header->character_first_kerning_pair_index[i] >= (kerning_pair = header->character_first_kerning_pair_index[i]);
	}

	valid &= header->location_table_offset >= (header->kerning_pairs_offset + (long)sizeof(s_kerning_pair) * header->kerning_pair_count);
	valid &= IN_RANGE_INCLUSIVE(header->location_table_count, 0, 65536);
	valid &= IN_RANGE_INCLUSIVE(header->character_count, 1, 65536) && header->character_count <= header->location_table_count;

	if (valid && header->no_such_character_data_location != NONE)
	{
		long v1 = (header->no_such_character_data_location & 0xFFE00000) >> 21;
		long v2 = (header->no_such_character_data_location & 0x1FFFFF + v1);
		valid &= v1 && v1 <= 0x402 && v2 <= header->character_data_size_bytes / CHAR_BYTES;
	}

	valid &= IN_RANGE_INCLUSIVE(header->maximum_packed_pixel_size_bytes, 1, 0x2000);
	valid &= IN_RANGE_INCLUSIVE(header->maximum_unpacked_pixel_size_bytes, 1, 0x8000);
	valid &= IN_RANGE_INCLUSIVE(header->total_packed_pixel_size_bytes, 1, 0x8000000);

	return valid;
}

void __cdecl font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, long kerning_pair_count)
{
	INVOKE(0x0065B810, font_kerning_pairs_byteswap, kerning_pairs, kerning_pair_count);
}

