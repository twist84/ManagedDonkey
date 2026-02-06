#include "networking/tools/network_blf.hpp"

#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "game/game_globals.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00462F00, network_blf_find_chunk);
HOOK_DECLARE(0x004630D0, network_blf_read_for_known_chunk);
HOOK_DECLARE(0x00463240, network_blf_verify_end_of_file);
HOOK_DECLARE(0x00463540, network_blf_verify_start_of_file);

HOOK_DECLARE_CLASS_MEMBER(0x014E9FE0, s_blf_saved_film, copy_to_and_validate_);
bool __thiscall s_blf_saved_film::copy_to_and_validate_(game_options* options, int32* out_length_in_ticks, int32* out_start_tick, bool* out_was_valid)
{
	return s_blf_saved_film::copy_to_and_validate(options, out_length_in_ticks, out_start_tick, out_was_valid);
}

void s_blf_header::setup(int32 _chunk_type, int32 _chunk_size, int32 _major_version, int32 _minor_version)
{
	chunk_type = _chunk_type;
	chunk_size = _chunk_size;
	major_version = static_cast<int16>(_major_version);
	minor_version = static_cast<int16>(_minor_version);
}

s_blf_chunk_start_of_file::s_blf_chunk_start_of_file()
{
	s_blf_chunk_start_of_file::initialize();
}

void s_blf_chunk_start_of_file::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	byte_order_mark = 0xFFFE;
	csmemset(file_type, 0, sizeof(file_type));
	zero_array(alignment_padding_unused);
}

s_blf_chunk_end_of_file::s_blf_chunk_end_of_file()
{
	s_blf_chunk_end_of_file::initialize();
}

void s_blf_chunk_end_of_file::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	total_file_size = 0;
	authentication_type = _blf_file_authentication_type_none;
}

s_blf_chunk_end_of_file_with_crc::s_blf_chunk_end_of_file_with_crc()
{
	s_blf_chunk_end_of_file_with_crc::initialize();
}

void s_blf_chunk_end_of_file_with_crc::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	total_file_size = 0;
	authentication_type = _blf_file_authentication_type_crc;
}

s_blf_chunk_content_header::s_blf_chunk_content_header()
{
	s_blf_chunk_content_header::initialize();
}

void s_blf_chunk_content_header::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	build_number = static_cast<int16>(version_get_build_number());
	map_minor_version = static_cast<int16>(get_map_minor_version());

	metadata.display_name[0] = 0;
	metadata.description[0] = 0;
	metadata.file_type = k_saved_game_file_type_none;
	metadata.author[0] = 0;
	metadata.date = 0;
	metadata.length_seconds = 0;
	metadata.map_id = _map_id_none;
	metadata.game_engine_index = _game_engine_type_none;
}

void s_blf_chunk_content_header::initialize_from_current_game_settings(e_controller_index controller_index, e_saved_game_file_type saved_game_file_type, wchar_t const* name, wchar_t const* description, uns64 size, int32 length_in_seconds)
{
	//INVOKE_CLASS_MEMBER(0x00462ED0, s_blf_chunk_content_header, initialize_from_current_game_settings, controller_index, saved_game_file_type, name, description, size, length_in_seconds);

	s_blf_chunk_content_header::initialize();
	metadata.initialize_from_current_game_settings(controller_index, saved_game_file_type, name, description, size);
}

s_blf_chunk_author::s_blf_chunk_author()
{
	s_blf_chunk_author::initialize();
}

void s_blf_chunk_author::initialize()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	csmemset(program_name, 0, sizeof(program_name));
	program_build_number = 0;
	csmemset(program_build_string, 0, sizeof(program_build_string));
	csmemset(user, 0, sizeof(user));
}

s_blf_chunk_game_variant::s_blf_chunk_game_variant() :
	game_variant()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);
}

s_blf_chunk_map_variant::s_blf_chunk_map_variant() :
	map_variant()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);
	zero_array(pad);
}

s_blffile_saved_game_file::s_blffile_saved_game_file() :
	start_of_file(),
	content_header()
{
}

s_blffile_game_variant::s_blffile_game_variant() :
	s_blffile_saved_game_file(),
	variant(),
	end_of_file()
{
	zero_array(pad);
}

bool s_blffile_game_variant::copy_to_and_validate(c_game_variant* game_variant, bool* out_was_valid) const
{
	return INVOKE_CLASS_MEMBER(0x00573150, s_blffile_game_variant, copy_to_and_validate, game_variant, out_was_valid);
}

s_blffile_map_variant::s_blffile_map_variant() :
	s_blffile_saved_game_file(),
	variant(),
	end_of_file()
{
	zero_array(pad);
}

bool s_blffile_map_variant::copy_to_and_validate(c_map_variant* map_variant, bool* out_was_valid) const
{
	//return INVOKE_CLASS_MEMBER(0x00573250, s_blffile_map_variant, copy_to_and_validate, map_variant, out_was_valid);

	bool must_byte_swap = false;
	int32 chunk_size = 0;
	if (!network_blf_verify_start_of_file((char*)this, sizeof(s_blf_chunk_map_variant), &must_byte_swap, &chunk_size) || chunk_size <= 0)
	{
		return false;
	}

	const char* chunk = (const char*)this + chunk_size;
	const char* chunk_data = nullptr;
	bool eof_found = false;
	if (network_blf_read_for_known_chunk(
		chunk,
		sizeof(s_blf_chunk_map_variant) - chunk_size,
		must_byte_swap,
		s_blf_chunk_map_variant::k_chunk_type,
		s_blf_chunk_map_variant::k_chunk_major_version,
		&chunk_size,
		&chunk_data,
		nullptr,
		nullptr,
		&eof_found))
	{
		while (!eof_found && chunk_size > 0)
		{
			if (must_byte_swap)
			{
				event(_event_critical, "map_variant: doesn't support byte swapping BLF files");
			}
			else
			{
				c_map_variant* variant = (c_map_variant*)((byte*)chunk_data - sizeof(s_blf_header));
				if (chunk_data)
				{
					variant->save_to(map_variant);

					if (out_was_valid)
					{
						*out_was_valid = true;
					}

					return true;
				}
			}

			chunk += chunk_size;

			int32 buffer_count = (uns32)this - (uns32)chunk + sizeof(s_blf_chunk_map_variant);

			if (!network_blf_read_for_known_chunk(
				chunk,
				buffer_count,
				must_byte_swap,
				s_blf_chunk_map_variant::k_chunk_type,
				s_blf_chunk_map_variant::k_chunk_major_version,
				&chunk_size,
				&chunk_data,
				nullptr,
				nullptr,
				&eof_found))
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
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	// game_options::game_options
	DECLFUNC(0x00485420, void, __thiscall, game_options*)(&options);
}

s_blf_saved_film::s_blf_chunk_saved_film_data::s_blf_chunk_saved_film_data()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);
}

s_blf_saved_film::s_blf_saved_film() :
	s_blffile_saved_game_file(),
	author(),
	film_header(),
	film_data()
{
}

bool s_blf_saved_film::copy_to_and_validate(game_options* options, int32* out_length_in_ticks, int32* out_start_tick, bool* out_was_valid) const
{
	//return INVOKE_CLASS_MEMBER(0x014E9FE0, s_blf_saved_film, copy_to_and_validate, options, out_length_in_ticks, out_start_tick, out_was_valid);

	ASSERT(options);
	ASSERT(out_length_in_ticks);
	ASSERT(out_start_tick);

	bool must_byte_swap = false;
	int32 chunk_size = 0;
	if (!network_blf_verify_start_of_file((char*)this, sizeof(s_blf_saved_film), &must_byte_swap, &chunk_size) || chunk_size <= 0)
	{
		return false;
	}

	const char* chunk = (const char*)this + chunk_size;
	const char* chunk_data = nullptr;
	bool eof_found = false;
	if (network_blf_read_for_known_chunk(
		chunk,
		sizeof(s_blf_saved_film) - chunk_size,
		must_byte_swap,
		s_blf_saved_film::s_blf_chunk_saved_film_header::k_chunk_type,
		s_blf_saved_film::s_blf_chunk_saved_film_header::k_chunk_major_version,
		&chunk_size,
		&chunk_data,
		nullptr,
		nullptr,
		&eof_found))
	{
		while (!eof_found && chunk_size > 0)
		{
			if (must_byte_swap)
			{
				event(_event_critical, "saved_film: doesn't support byte swapping BLF files");
			}
			else
			{
				
				s_blf_saved_film::s_blf_chunk_saved_film_header* saved_film_header = (s_blf_saved_film::s_blf_chunk_saved_film_header*)((byte*)chunk_data - sizeof(s_blf_header));
				if (chunk_data)
				{
					csmemcpy(options, &saved_film_header->options, sizeof(game_options));
					game_options_validate(options);

					bool was_valid = true;
					bool result = true;

					char error_string_storage[1024]{};
					if (!game_options_verify(options, error_string_storage, sizeof(error_string_storage)))
					{
						event(_event_warning, "networking:saved_film: film header has invalid game options [%s]",
							error_string_storage);

						was_valid = false;
						result = false;
					}

					*out_length_in_ticks = saved_film_header->length_in_ticks;
					*out_start_tick = saved_film_header->snippet_start_tick;

					if (out_was_valid)
					{
						*out_was_valid = was_valid;
					}

					return result;
				}
			}

			chunk += chunk_size;

			int32 buffer_count = (uns32)this - (uns32)chunk + sizeof(s_blf_saved_film);

			if (!network_blf_read_for_known_chunk(
				chunk,
				buffer_count,
				must_byte_swap,
				s_blf_saved_film::s_blf_chunk_saved_film_header::k_chunk_type,
				s_blf_saved_film::s_blf_chunk_saved_film_header::k_chunk_major_version,
				&chunk_size,
				&chunk_data,
				nullptr,
				nullptr,
				&eof_found))
			{
				return false;
			}
		}
	}

	return false;
}

s_blf_chunk_campaign::s_blf_chunk_campaign()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

	type_flags = 0;
	campaign_id = _campaign_id_none;
	csmemset(names, 0, sizeof(names));
	csmemset(descriptions, 0, sizeof(descriptions));
	csmemset(map_ids, 0, sizeof(map_ids));
	zero_array(pad);
}

s_blf_chunk_scenario::s_blf_chunk_scenario()
{
	header.setup(k_chunk_type, sizeof(*this), k_chunk_major_version, k_chunk_minor_version);

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

bool __cdecl network_blf_find_chunk(const char* buffer, int32 buffer_count, bool must_byte_swap, int32 desired_chunk_type, int16 desired_version_major, int32* out_chunk_size, const char** out_found_chunk_data_size, int32* out_chunk_buffer_size, int16* out_version_minor, bool* out_eof_found)
{
	bool result = false;

	while (!result)
	{
		const char* chunk_buffer = nullptr;
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

bool __cdecl network_blf_read_for_known_chunk(const char* buffer, int32 buffer_count, bool must_byte_swap, int32 desired_chunk_type, int16 desired_version_major, int32* out_chunk_size, const char** found_chunk_data, int32* out_found_chunk_data_size, int16* out_version_minor, bool* out_eof_found)
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
		const s_blf_header* chunk = (const s_blf_header*)buffer;

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

bool __cdecl network_blf_verify_end_of_file(const char* buffer, int32 buffer_count, bool byte_swap, const char* eof_chunk_buffer, e_blf_file_authentication_type authentication_type)
{
	bool result = false;
	HOOK_INVOKE(result =, network_blf_verify_end_of_file, buffer, buffer_count, byte_swap, eof_chunk_buffer, authentication_type);
	return result;
}

bool __cdecl network_blf_verify_start_of_file(const char* buffer, int32 buffer_count, bool* out_byte_swap, int32* out_chunk_size)
{
	bool result = false;

	if (out_chunk_size)
		*out_chunk_size = 0;

	if (buffer_count >= sizeof(s_blf_chunk_start_of_file))
	{
		const s_blf_chunk_start_of_file* sof_chunk = (const s_blf_chunk_start_of_file*)buffer;

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

