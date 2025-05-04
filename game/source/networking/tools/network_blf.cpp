#include "networking/tools/network_blf.hpp"

#include "config/version.hpp"
#include "game/game_globals.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00462F00, network_blf_find_chunk);
HOOK_DECLARE(0x004630D0, network_blf_read_for_known_chunk);
HOOK_DECLARE(0x00463240, network_blf_verify_end_of_file);
HOOK_DECLARE(0x00463540, network_blf_verify_start_of_file);

void s_blf_header::setup(int32 _chunk_type, int32 _chunk_size, int32 _major_version, int32 _minor_version)
{
	chunk_type = _chunk_type;
	chunk_size = _chunk_size;
	major_version = static_cast<int16>(_major_version);
	minor_version = static_cast<int16>(_minor_version);
}

s_blf_chunk_start_of_file::s_blf_chunk_start_of_file()
{
	initialize();
}

void s_blf_chunk_start_of_file::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	byte_order_mark = 0xFFFE;
	name.clear();
	zero_array(pad);
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

	build_number = static_cast<int16>(version_get_build_number());
	map_minor_version = static_cast<int16>(get_map_minor_version());

	metadata.name[0] = 0;
	metadata.description[0] = 0;
	metadata.file_type = _saved_game_file_type_none;
	metadata.author[0] = 0;
	metadata.date = 0;
	metadata.length_seconds = 0;
	metadata.map_id = _map_id_none;
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
	zero_array(pad);
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
	zero_array(pad);
}

s_blffile_map_variant::s_blffile_map_variant() :
	s_blffile_saved_game_file(),
	map_variant_chunk(),
	end_of_file_chunk()
{
	zero_array(pad);
}

bool s_blffile_map_variant::copy_to_and_validate(c_map_variant* map_variant, bool* is_valid) const
{
	//return INVOKE_CLASS_MEMBER(0x00573250, s_blffile_map_variant, copy_to_and_validate, map_variant, is_valid);

	bool byte_swap = false;
	int32 chunk_size = 0;
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

s_blf_saved_film::s_blf_chunk_saved_film_header::s_saved_film_build_compatibility::s_saved_film_build_compatibility() :
	build_number(),
	executable_type(),
	network_executable_version(),
	network_compatible_version(),
	map_language(),
	map_minor_version(),
	map_minor_version_is_tracked(),
	pad1(),
	pad2(),
	map_signature_size(),
	map_signature_bytes()
{
}

s_blf_saved_film::s_blf_chunk_saved_film_header::s_blf_chunk_saved_film_header() :
	pad0(),
	build_compatibility(),
	is_host_film(false),
	contains_gamestate(false),
	is_snippet(false),
	pad3(),
	session_id(),
	options(),
	recorded_time(),
	length_in_ticks(),
	snippet_start_tick(),
	padding_to_align_for_utility_drive()
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
	int32 chunk_size = 0;
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

		game_variant->copy_from_and_validate(&saved_film_header->options.multiplayer_variant);

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
	campaign_id = _campaign_id_none;
	csmemset(names, 0, sizeof(names));
	csmemset(descriptions, 0, sizeof(descriptions));
	csmemset(map_ids, 0, sizeof(map_ids));
	zero_array(pad);
}

s_blf_chunk_scenario::s_blf_chunk_scenario()
{
	header.setup(k_chunk_type, sizeof(*this), k_version_major, k_version_minor);

	map_id = _map_id_none;
	flags.clear();
	csmemset(name, 0, sizeof(name));
	csmemset(description, 0, sizeof(description));
	csmemset(image_file_base, 0, sizeof(image_file_base));
	csmemset(scenario_file, 0, sizeof(scenario_file));
	presence_context_id = 0;
	sort_order = 0;
	mp_minimum_desired_players = 0;
	mp_maximum_desired_players = 0;
	csmemset(maximum_teams, 0, sizeof(maximum_teams));
	allows_saved_films = false;
	zero_array(__pad112A);
}

template<typename insertion_struct, int32 insertion_count>
s_blf_chunk_scenario_minor_version<insertion_struct, insertion_count>::s_blf_chunk_scenario_minor_version() :
	s_blf_chunk_scenario()
{
	csmemset(insertions, 0, sizeof(insertions));
}

s_blf_chunk_scenario_halo3::s_blf_chunk_scenario_halo3() :
	s_blf_chunk_scenario_minor_version<s_scenario_insertion_point_halo3, 4>()
{
}

s_blf_chunk_scenario_atlas::s_blf_chunk_scenario_atlas() :
	s_blf_chunk_scenario_minor_version<s_scenario_insertion_point_atlas, 9>()
{
}

bool __cdecl network_blf_find_chunk(char const* buffer, int32 buffer_count, bool must_byte_swap, int32 desired_chunk_type, int16 desired_version_major, int32* out_chunk_size, char const** out_found_chunk_data_size, int32* out_chunk_buffer_size, int16* out_version_minor, bool* out_eof_found)
{
	bool result = false;

	while (!result)
	{
		char const* chunk_buffer = nullptr;
		int32 chunk_size = 0;
		bool eof_chunk = false;

		bool read_result = network_blf_read_for_known_chunk(buffer, buffer_count, must_byte_swap, desired_chunk_type, desired_version_major, &chunk_size, &chunk_buffer, out_chunk_buffer_size, out_version_minor, &eof_chunk);

		if (out_chunk_size)
			*out_chunk_size = chunk_size;

		if (out_found_chunk_data_size)
			*out_found_chunk_data_size = chunk_buffer;

		if (out_eof_found)
			*out_eof_found = eof_chunk;

		result = chunk_buffer && chunk_size > 0;

		if (!read_result || eof_chunk || chunk_size <= 0)
			break;

		buffer_count -= chunk_size;
		buffer += chunk_size;
	}

	return result;
}

bool __cdecl network_blf_read_for_known_chunk(char const* buffer, int32 buffer_count, bool must_byte_swap, int32 desired_chunk_type, int16 desired_version_major, int32* out_chunk_size, char const** found_chunk_data, int32* out_found_chunk_data_size, int16* out_version_minor, bool* out_eof_found)
{
	ASSERT(out_chunk_size);

	bool result = false;

	*out_chunk_size = 0;

	if (found_chunk_data)
		*found_chunk_data = 0;

	if (out_found_chunk_data_size)
		*out_found_chunk_data_size = 0;

	if (out_version_minor)
		*out_version_minor = 0;

	if (out_eof_found)
		*out_eof_found = 0;

	if (buffer_count >= sizeof(s_blf_header))
	{
		s_blf_header const* chunk = reinterpret_cast<s_blf_header const*>(buffer);

		int32 chunk_type = chunk->chunk_type;
		int32 chunk_size = chunk->chunk_size;
		int16 chunk_major_version = chunk->major_version;
		int16 chunk_minor_version = chunk->minor_version;

		if (must_byte_swap)
		{
			bswap_uns32_inplace(chunk_type);
			bswap_uns32_inplace(chunk_size);
			bswap_uns16_inplace(chunk_major_version);
			bswap_uns16_inplace(chunk_minor_version);
		}

		if (chunk_size >= sizeof(s_blf_header) && chunk_size <= buffer_count && chunk_major_version >= 0 && chunk_minor_version >= 0)
		{
			*out_chunk_size = chunk_size;
			if (chunk_type == desired_chunk_type && chunk_major_version == desired_version_major)
			{
				if (out_version_minor)
					*out_version_minor = chunk_minor_version;

				if (found_chunk_data)
					*found_chunk_data = buffer + sizeof(s_blf_header);

				if (out_found_chunk_data_size)
					*out_found_chunk_data_size = chunk_size - sizeof(s_blf_header);

				result = true;

				if (chunk_type == s_blf_chunk_end_of_file::k_chunk_type && chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_count >= sizeof(s_blf_chunk_end_of_file) && out_eof_found)
				{
					*out_eof_found = true;
					result = true;
				}
			}
			else if (chunk_type == s_blf_chunk_end_of_file::k_chunk_type)
			{
				if (chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_count >= sizeof(s_blf_chunk_end_of_file))
				{
					result = true;

					if (out_eof_found)
					{
						*out_eof_found = true;
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

bool __cdecl network_blf_verify_end_of_file(char const* buffer, int32 buffer_count, bool byte_swap, char const* eof_chunk_buffer, e_blf_file_authentication_type authentication_type)
{
	bool result = false;
	HOOK_INVOKE(result =, network_blf_verify_end_of_file, buffer, buffer_count, byte_swap, eof_chunk_buffer, authentication_type);
	return result;
}

bool __cdecl network_blf_verify_start_of_file(char const* buffer, int32 buffer_count, bool* out_byte_swap, int32* out_chunk_size)
{
	bool result = false;

	if (out_chunk_size)
		*out_chunk_size = 0;

	if (buffer_count >= sizeof(s_blf_chunk_start_of_file))
	{
		s_blf_chunk_start_of_file const* sof_chunk = reinterpret_cast<s_blf_chunk_start_of_file const*>(buffer);

		int32 chunk_type = sof_chunk->header.chunk_type;
		int32 chunk_size = sof_chunk->header.chunk_size;
		int16 chunk_major_version = sof_chunk->header.major_version;
		int16 chunk_minor_version = sof_chunk->header.minor_version;

		if (out_byte_swap)
			*out_byte_swap = false;

		if (sof_chunk->byte_order_mark == bswap_uns16(0xFFFE))
		{
			bswap_uns32_inplace(chunk_type);
			bswap_uns32_inplace(chunk_size);
			bswap_uns16_inplace(chunk_major_version);
			bswap_uns16_inplace(chunk_minor_version);

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

