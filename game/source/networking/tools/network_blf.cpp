#include "networking/tools/network_blf.hpp"

#include "config/version.hpp"
#include "game/game_globals.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"

#include <string.h>

HOOK_DECLARE(0x00462F00, network_blf_find_chunk);
HOOK_DECLARE(0x004630D0, network_blf_read_for_known_chunk);
HOOK_DECLARE(0x00463240, network_blf_verify_end_of_file);
HOOK_DECLARE(0x00463540, network_blf_verify_start_of_file);

void s_blf_header::setup(long _chunk_type, long _chunk_size, long _major_version, long _minor_version)
{
	chunk_type = bswap_dword(_chunk_type);
	chunk_size = bswap_dword(_chunk_size);
	major_version = bswap_word(static_cast<short>(_major_version));
	minor_version = bswap_word(static_cast<short>(_minor_version));
}

s_blf_chunk_start_of_file::s_blf_chunk_start_of_file()
{
	initialize();
}

void s_blf_chunk_start_of_file::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	byte_order_mark = bswap_word(0xFFFE);
	name.clear();
	memset(pad, 0, sizeof(pad));
}

s_blf_chunk_end_of_file::s_blf_chunk_end_of_file()
{
	initialize();
}

void s_blf_chunk_end_of_file::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	file_size = 0;
	authentication_type = _blf_file_authentication_type_none;
}

s_blf_chunk_end_of_file_with_crc::s_blf_chunk_end_of_file_with_crc()
{
	initialize();
}

void s_blf_chunk_end_of_file_with_crc::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	file_size = 0;
	authentication_type = _blf_file_authentication_type_crc;
}

s_blf_chunk_content_header::s_blf_chunk_content_header()
{
	initialize();
}

void s_blf_chunk_content_header::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	build_number = static_cast<short>(version_get_build_number());
	map_minor_version = static_cast<short>(get_map_minor_version());

	metadata.name[0] = 0;
	metadata.description[0] = 0;
	metadata.file_type = _saved_game_file_type_none;
	metadata.author[0] = 0;
	metadata.date = 0;
	metadata.length_seconds = 0;
	metadata.map_id = -1;
	metadata.game_engine_type = 0;
}

s_blf_chunk_author::s_blf_chunk_author()
{
	initialize();
}

void s_blf_chunk_author::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	build_name.set("");
	build_identifier = 0;
	build_string.set("");
	author_name.set("");
}

s_blf_chunk_game_variant::s_blf_chunk_game_variant() :
	game_variant()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);
}

s_blf_chunk_map_variant::s_blf_chunk_map_variant() :
	map_variant()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);
}

s_blffile_saved_game_file::s_blffile_saved_game_file() :
	start_of_file_chunk(),
	content_header_chunk()
{
}

s_blffile_game_variant::s_blffile_game_variant() :
	s_blffile_saved_game_file(),
	game_variant_chunk(),
	end_of_file_chunk()
{
	memset(pad, 0, sizeof(pad));
}

s_blffile_map_variant::s_blffile_map_variant() :
	s_blffile_saved_game_file(),
	map_variant_chunk(),
	end_of_file_chunk()
{
	memset(pad, 0, sizeof(pad));
}

bool s_blffile_map_variant::copy_to_and_validate(c_map_variant* map_variant, bool* is_valid) const
{
	//return INVOKE_CLASS_MEMBER(0x00573250, s_blffile_map_variant::copy_to_and_validate, map_variant, is_valid);

	bool byte_swap = false;
	long chunk_size = 0;
	if (!network_blf_verify_start_of_file((char*)this, sizeof(s_blf_chunk_map_variant), &byte_swap, &chunk_size))
		return false;

	char const* next_chunk = (char*)this + chunk_size;

	bool eof_chunk = false;
	char const* chunk = nullptr;
	if (network_blf_read_for_known_chunk(
		(char const*)this + chunk_size,
		sizeof(s_blf_chunk_map_variant) - (chunk - (char const*)this),
		byte_swap,
		s_blf_chunk_map_variant::k_chunk_type,
		s_blf_chunk_map_variant::k_version_major,
		&chunk_size,
		&chunk,
		nullptr,
		nullptr,
		&eof_chunk))
	{
		while (!eof_chunk)
		{
			if (!byte_swap && chunk)
			{
				s_blf_chunk_map_variant* map_variant_chunk = (s_blf_chunk_map_variant*)chunk;
				map_variant_chunk->map_variant.read_from(map_variant);

				if (is_valid)
					*is_valid = true;

				return true;
			}

			next_chunk += chunk_size;
			if (!network_blf_read_for_known_chunk(
				next_chunk,
				(char const*)this - chunk + sizeof(s_blf_chunk_map_variant),
				byte_swap,
				s_blf_chunk_map_variant::k_chunk_type,
				s_blf_chunk_map_variant::k_version_major,
				&chunk_size,
				&chunk,
				nullptr,
				nullptr,
				&eof_chunk))
			{
				return false;
			}
		}
	}

	return false;
}

s_blf_saved_film::s_blf_chunk_saved_film_header::s_blf_chunk_saved_film_header() :
	__dataC(),
	options(),
	__data()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	// game_options::game_options
	DECLFUNC(0x00485420, void, __thiscall, game_options*)(&options);
}

s_blf_saved_film::s_blf_chunk_saved_film_data::s_blf_chunk_saved_film_data()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);
}

s_blf_saved_film::s_blf_saved_film() :
	s_blffile_saved_game_file(),
	s_blf_chunk_author(),
	film_header(),
	film_data()
{
}

bool s_blf_saved_film::copy_to_and_validate(c_game_variant* game_variant, c_map_variant* map_variant, bool* is_valid) const
{
	bool byte_swap = false;
	long chunk_size = 0;
	if (!network_blf_verify_start_of_file((char*)this, sizeof(s_blf_saved_film), &byte_swap, &chunk_size))
		return false;

	while (true)
	{
		char const* chunk = (char const*)this + chunk_size;

		bool eof_chunk = false;
		if (!network_blf_read_for_known_chunk(chunk, sizeof(s_blf_saved_film) - (chunk - (char const*)this), byte_swap, s_blf_saved_film::k_chunk_type, s_blf_saved_film::k_version_major, &chunk_size, &chunk, nullptr, nullptr, &eof_chunk) || eof_chunk)
			break;

		if (!chunk)
			continue;

		s_blf_saved_film::s_blf_chunk_saved_film_header* saved_film_header = (s_blf_saved_film::s_blf_chunk_saved_film_header*)chunk;

		game_variant->copy_from_and_validate(&saved_film_header->options.game_variant);

		bool valid = true;
		valid &= game_engine_variant_validate(game_variant);
		valid &= map_variant->read_from(&saved_film_header->options.map_variant);

		if (is_valid)
			*is_valid = valid;

		return true;
	}

	return false;
}

s_blf_chunk_campaign::s_blf_chunk_campaign()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	type_flags = 0;
	campaign_id = -1;
	memset(names, 0, sizeof(names));
	memset(descriptions, 0, sizeof(descriptions));
	map_ids.clear();
}

s_blf_chunk_scenario::s_blf_chunk_scenario()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	map_id = -1;
	type_flags = 0;
	memset(names, 0, sizeof(names));
	memset(descriptions, 0, sizeof(descriptions));
	image_file_base.set(0);
	scenario_path.set(0);
	multiplayer_minimum_desired_players = 0;
	multiplayer_maximum_desired_players = 0;
	memset(engine_maximum_teams, 0, sizeof(engine_maximum_teams));
	allows_saved_films = false;
	memset(insertions, 0, sizeof(insertions));
}

bool __cdecl network_blf_find_chunk(char const* buffer, long buffer_count, bool byte_swap, long chunk_type, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* out_chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk)
{
	bool result = false;

	while (!result)
	{
		char const* chunk_buffer = nullptr;
		long chunk_size = 0;
		bool eof_chunk = false;

		bool read_result = network_blf_read_for_known_chunk(buffer, buffer_count, byte_swap, chunk_type, major_version, &chunk_size, &chunk_buffer, out_chunk_buffer_size, out_minor_version, &eof_chunk);

		if (out_chunk_size)
			*out_chunk_size = chunk_size;

		if (out_chunk_buffer)
			*out_chunk_buffer = chunk_buffer;

		if (out_eof_chunk)
			*out_eof_chunk = eof_chunk;

		result = chunk_buffer && chunk_size > 0;

		if (!read_result || eof_chunk || chunk_size <= 0)
			break;

		buffer_count -= chunk_size;
		buffer += chunk_size;
	}

	return result;
}

bool __cdecl network_blf_read_for_known_chunk(char const* buffer, long buffer_count, bool byte_swap, long type, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* out_chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk)
{
	ASSERT(out_chunk_size);

	bool result = false;

	*out_chunk_size = 0;

	if (out_chunk_buffer)
		*out_chunk_buffer = 0;

	if (out_chunk_buffer_size)
		*out_chunk_buffer_size = 0;

	if (out_minor_version)
		*out_minor_version = 0;

	if (out_eof_chunk)
		*out_eof_chunk = 0;

	if (buffer_count >= sizeof(s_blf_header))
	{
		s_blf_header const* chunk = reinterpret_cast<s_blf_header const*>(buffer);

		long chunk_type = chunk->chunk_type;
		long chunk_size = chunk->chunk_size;
		short chunk_major_version = chunk->major_version;
		short chunk_minor_version = chunk->minor_version;

		if (byte_swap)
		{
			bswap_dword_inplace(chunk_type);
			bswap_dword_inplace(chunk_size);
			bswap_word_inplace(chunk_major_version);
			bswap_word_inplace(chunk_minor_version);
		}

		if (chunk_size >= sizeof(s_blf_header) && chunk_size <= buffer_count && chunk_major_version >= 0 && chunk_minor_version >= 0)
		{
			*out_chunk_size = chunk_size;
			if (chunk_type == type && chunk_major_version == major_version)
			{
				if (out_minor_version)
					*out_minor_version = chunk_minor_version;

				if (out_chunk_buffer)
					*out_chunk_buffer = buffer + sizeof(s_blf_header);

				if (out_chunk_buffer_size)
					*out_chunk_buffer_size = chunk_size - sizeof(s_blf_header);

				result = true;

				if (chunk_type == s_blf_chunk_end_of_file::k_chunk_type && chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_count >= sizeof(s_blf_chunk_end_of_file) && out_eof_chunk)
				{
					*out_eof_chunk = true;
					result = true;
				}
			}
			else if (chunk_type == s_blf_chunk_end_of_file::k_chunk_type)
			{
				if (chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_count >= sizeof(s_blf_chunk_end_of_file))
				{
					result = true;

					if (out_eof_chunk)
					{
						*out_eof_chunk = true;
						result = true;
					}
				}
			}
			else
			{
				result = true;
			}
		}
	}

	return result;
}

bool __cdecl network_blf_verify_end_of_file(char const* buffer, long buffer_count, bool byte_swap, char const* eof_chunk_buffer, e_blf_file_authentication_type authentication_type)
{
	bool result = false;
	HOOK_INVOKE(result =, network_blf_verify_end_of_file, buffer, buffer_count, byte_swap, eof_chunk_buffer, authentication_type);
	return result;
}

bool __cdecl network_blf_verify_start_of_file(char const* buffer, long buffer_count, bool* out_byte_swap, long* out_chunk_size)
{
	bool result = false;

	if (out_chunk_size)
		*out_chunk_size = 0;

	if (buffer_count >= sizeof(s_blf_chunk_start_of_file))
	{
		s_blf_chunk_start_of_file const* sof_chunk = reinterpret_cast<s_blf_chunk_start_of_file const*>(buffer);

		long chunk_type = sof_chunk->header.chunk_type;
		long chunk_size = sof_chunk->header.chunk_size;
		short chunk_major_version = sof_chunk->header.major_version;
		short chunk_minor_version = sof_chunk->header.minor_version;

		if (out_byte_swap)
			*out_byte_swap = false;

		if (sof_chunk->byte_order_mark == bswap_word(0xFFFE))
		{
			bswap_dword_inplace(chunk_type);
			bswap_dword_inplace(chunk_size);
			bswap_word_inplace(chunk_major_version);
			bswap_word_inplace(chunk_minor_version);

			if (out_byte_swap)
				*out_byte_swap = true;
		}

		if (chunk_type == s_blf_chunk_start_of_file::k_chunk_type && chunk_major_version == 1 && chunk_size >= sizeof(s_blf_chunk_start_of_file))
		{
			if (out_chunk_size)
				*out_chunk_size = chunk_size;

			result = true;
		}
	}


	return result;
}

