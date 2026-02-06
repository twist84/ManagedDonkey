#include "text/font_group.hpp"

#include "cseries/cseries_events.hpp"
#include "memory/module.hpp"
#include "text/font_loading.hpp"

HOOK_DECLARE(0x0065B4B0, font_character_validate);
HOOK_DECLARE(0x0065B560, font_get_kerning_pair_offset);
HOOK_DECLARE(0x0065B5E0, font_get_line_height);
HOOK_DECLARE(0x0065B610, font_header_byteswap);
HOOK_DECLARE(0x0065B620, font_header_validate);
HOOK_DECLARE(0x0065B810, font_kerning_pairs_byteswap);

bool __cdecl font_character_validate(const s_font_character* character)
{
	//return INVOKE(0x0065B4B0, font_character_validate, character);

	ASSERT(character != nullptr);

	bool valid = IN_RANGE(character->initial_offset, -k_font_character_maximum_bitmap_width, k_font_character_maximum_bitmap_width);
	valid = valid && character->bitmap_height < k_font_character_maximum_bitmap_height;
	valid = valid && character->bitmap_width < k_font_character_maximum_bitmap_width;
	valid = valid && character->packed_size < k_font_maximum_packed_byte_count;
	valid = valid && IN_RANGE_INCLUSIVE(character->character_width, -k_font_character_maximum_bitmap_width, k_font_character_maximum_bitmap_width);
	valid = valid && IN_RANGE_INCLUSIVE(character->bitmap_origin_y, -k_font_character_maximum_bitmap_height, k_font_character_maximum_bitmap_height);

	return valid;
}

int16 __cdecl font_get_kerning_pair_offset(const s_font_header* header, uns32 first_character, uns32 second_character)
{
	//return INVOKE(0x0065B560, font_get_kerning_pair_offset, header, first_character, second_character);

	ASSERT(header != nullptr);

	int16 offset = 0;

	if (IN_RANGE_INCLUSIVE(first_character, 1, k_font_maximum_kerned_character) && IN_RANGE_INCLUSIVE(second_character, 1, k_font_maximum_kerned_character))
	{
		const s_kerning_pair* kerning_pairs = (const s_kerning_pair*)offset_pointer(header, header->kerning_pairs_offset);

		int32 first_kerning_pair_index = header->character_first_kerning_pair_index[first_character];
		int32 last_kerning_pair_index = first_character < k_font_maximum_kerned_character ? header->character_first_kerning_pair_index[first_character + 1] : header->kerning_pair_count;

		ASSERT(first_kerning_pair_index <= header->kerning_pair_count);
		ASSERT(last_kerning_pair_index <= header->kerning_pair_count);
		ASSERT(first_kerning_pair_index <= last_kerning_pair_index);

		for (int32 kerning_pair_index = first_kerning_pair_index; kerning_pair_index < last_kerning_pair_index; kerning_pair_index++)
		{
			if (kerning_pairs[kerning_pair_index].second_character == second_character)
			{
				offset = kerning_pairs[kerning_pair_index].offset;
				break;
			}

			if (kerning_pairs[kerning_pair_index].second_character > second_character)
			{
				break;
			}
		}
	}

	return offset;
}

int32 __cdecl font_get_line_height(const s_font_header* header)
{
	//return INVOKE(0x0065B5E0, font_get_line_height, header);

	long line_height;
	if (header != nullptr)
	{
		line_height = header->descending_height + header->leading_height + header->ascending_height;
	}
	else
	{
		line_height = 10;
	}
	return line_height;
}

void __cdecl font_header_byteswap(s_font_header* header)
{
	//INVOKE(0x0065B610, font_header_byteswap, header);
}

bool __cdecl font_header_validate(const s_font_header* header)
{
	//return INVOKE(0x0065B620, font_header_validate, header);

	ASSERT(header != nullptr);

	bool valid = header->version == k_font_version;
	if (!valid)
	{
		event(_event_error, "fonts: header version mismatch 0x%08X != 0x%08X, maybe you need to get new fonts?",
			header->version,
			k_font_version);
	}

	valid = valid && IN_RANGE_INCLUSIVE(header->ascending_height, 0, k_font_character_maximum_bitmap_height);
	valid = valid && IN_RANGE_INCLUSIVE(header->descending_height, 0, k_font_character_maximum_bitmap_height);
	valid = valid && IN_RANGE_INCLUSIVE(header->leading_height, 0, k_font_character_maximum_bitmap_height);
	valid = valid && IN_RANGE_INCLUSIVE(header->leading_width, 0, k_font_character_maximum_bitmap_width);
	valid = valid && header->ascending_height + header->descending_height > 0;
	valid = valid && IN_RANGE_INCLUSIVE(header->kerning_pair_count, 0, k_font_maximum_number_of_kerning_pairs);

	int32 last_kerning_pair_index = 0;
	for (int32 kerned_character_index = 0; kerned_character_index <= k_font_maximum_number_of_kerning_pairs; kerned_character_index++)
	{
		int32 kerning_pair_index = header->character_first_kerning_pair_index[kerned_character_index];
		valid = valid && kerning_pair_index <= header->kerning_pair_count;
		valid = valid && kerning_pair_index >= last_kerning_pair_index;

		last_kerning_pair_index = kerning_pair_index;
	}

	valid = valid && header->location_table_offset >= (header->kerning_pairs_offset + (int32)sizeof(s_kerning_pair) * header->kerning_pair_count);
	valid = valid && IN_RANGE_INCLUSIVE(header->location_table_count, 0, 65536);
	valid = valid && IN_RANGE_INCLUSIVE(header->character_count, 1, k_font_maximum_characters);
	valid = valid && header->character_count <= header->location_table_count;

	if (valid && header->no_such_character_data_location != k_font_location_invalid)
	{
		constexpr int32 k_page_count_mask = MASK(k_font_location_page_count_bits) << k_font_location_page_index_bits;
		constexpr int32 k_page_index_mask = MASK(k_font_location_page_index_bits);
		static_assert(k_page_count_mask == 0xFFE00000);
		static_assert(k_page_index_mask == 0x001FFFFF);

		int32 page_count = (header->no_such_character_data_location & k_page_count_mask) >> k_font_location_page_index_bits;
		int32 page_index = header->no_such_character_data_location & k_page_index_mask;

		valid = valid && IN_RANGE_INCLUSIVE(page_count, 1, k_font_character_maximum_pages);
		valid = valid && page_index + page_count <= header->character_data_size_bytes / k_font_character_page_size;
	}

	valid = valid && IN_RANGE_INCLUSIVE(header->maximum_packed_pixel_size_bytes, 1, k_font_maximum_packed_byte_count);
	valid = valid && IN_RANGE_INCLUSIVE(header->maximum_unpacked_pixel_size_bytes, 1, 0x8000);
	valid = valid && IN_RANGE_INCLUSIVE(header->total_packed_pixel_size_bytes, 1, 0x8000000);

	return valid;
}

void __cdecl font_kerning_pairs_byteswap(s_kerning_pair* kerning_pairs, int32 kerning_pair_count)
{
	//INVOKE(0x0065B810, font_kerning_pairs_byteswap, kerning_pairs, kerning_pair_count);
}

