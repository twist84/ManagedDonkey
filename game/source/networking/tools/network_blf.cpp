#include "networking/tools/network_blf.hpp"

#include "config/version.hpp"
#include "cseries/console.hpp"
#include "game/game_globals.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"

#include <assert.h>
#include <string.h>

HOOK_DECLARE(0x00463540, network_blf_verify_start_of_file);
HOOK_DECLARE(0x00462F00, network_blf_find_chunk);
HOOK_DECLARE(0x004630D0, network_blf_read_for_known_chunk);
HOOK_DECLARE(0x00463240, network_blf_verify_end_of_file);

void s_blf_header::setup(long _signature, long _chunk_size, long _major_version, long _minor_version)
{
	FUNCTION_BEGIN(true);

	//signature = _signature;
	//chunk_size = _chunk_size;
	//major_version = static_cast<short>(_major_version);
	//minor_version = static_cast<short>(_minor_version);
	signature = _byteswap_ulong(_signature);
	chunk_size = _byteswap_ulong(_chunk_size);
	major_version = _byteswap_ushort(static_cast<short>(_major_version));
	minor_version = _byteswap_ushort(static_cast<short>(_minor_version));
}

s_blf_chunk_start_of_file::s_blf_chunk_start_of_file()
{
	FUNCTION_BEGIN(true);

	initialize();
}

void s_blf_chunk_start_of_file::initialize()
{
	FUNCTION_BEGIN(true);

	setup('_blf', sizeof(*this), 1, 2);

	byte_order_mark = _byteswap_ushort(0xFFFE);
	memset(name, 0, sizeof(name));
	memset(pad, 0, sizeof(pad));
}

s_blf_chunk_end_of_file::s_blf_chunk_end_of_file()
{
	FUNCTION_BEGIN(true);

	initialize();
}

void s_blf_chunk_end_of_file::initialize()
{
	FUNCTION_BEGIN(true);

	setup('_eof', sizeof(*this), 1, 1);

	file_size = 0;
	authentication_type = _blf_file_authentication_type_none;
}

s_blf_chunk_end_of_file_with_crc::s_blf_chunk_end_of_file_with_crc()
{
	FUNCTION_BEGIN(true);

	initialize();
}

void s_blf_chunk_end_of_file_with_crc::initialize()
{
	FUNCTION_BEGIN(true);

	setup('_eof', sizeof(*this), 1, 1);

	file_size = 0;
	authentication_type = _blf_file_authentication_type_crc;
}

s_blf_chunk_content_header::s_blf_chunk_content_header()
{
	FUNCTION_BEGIN(true);

	initialize();
}

void s_blf_chunk_content_header::initialize()
{
	FUNCTION_BEGIN(true);

	setup('chdr', sizeof(*this), 9, 1);

	build_number = static_cast<short>(version_get_build_number());
	map_minor_version = static_cast<short>(get_map_minor_version());

	metadata.name[0] = 0;
	metadata.description[0] = 0;
	metadata.file_type = -1;
	metadata.author[0] = 0;
	metadata.date = 0;
	metadata.length_seconds = 0;
	metadata.map_id = -1;
	metadata.game_engine_type = 0;
}

s_blf_chunk_author::s_blf_chunk_author()
{
	FUNCTION_BEGIN(true);

	initialize();
}

void s_blf_chunk_author::initialize()
{
	FUNCTION_BEGIN(true);

	setup('athr', sizeof(*this), 3, 1);

	build_name.set(0);
	build_identifier = 0;
	build_string.set(0);
	author_name.set(0);
}

s_blf_chunk_game_variant::s_blf_chunk_game_variant() :
	game_variant()
{
	FUNCTION_BEGIN(true);

	s_blf_header::setup('mpvr', sizeof(*this), 3, 1);
}

s_blf_chunk_map_variant::s_blf_chunk_map_variant() :
	map_variant()
{
	FUNCTION_BEGIN(true);

	s_blf_header::setup('mpvr', sizeof(*this), 12, 1);
	memset(pad, 0, sizeof(pad));
}

s_blffile_saved_game_file::s_blffile_saved_game_file() :
	start_of_file_chunk(),
	content_header_chunk()
{
	FUNCTION_BEGIN(true);
}

s_blffile_game_variant::s_blffile_game_variant() :
	s_blffile_saved_game_file(),
	game_variant_chunk(),
	end_of_file_chunk()
{
	FUNCTION_BEGIN(true);

	memset(pad, 0, sizeof(pad));
}

s_blffile_map_variant::s_blffile_map_variant() :
	s_blffile_saved_game_file(),
	map_variant_chunk(),
	end_of_file_chunk()
{
	FUNCTION_BEGIN(true);

	memset(pad, 0, sizeof(pad));
}

s_blf_chunk_campaign::s_blf_chunk_campaign()
{
	FUNCTION_BEGIN(true);

	s_blf_header::setup('cmpn', sizeof(*this), 1, 1);

	type_flags = 0;
	campaign_id = -1;
	memset(names, 0, sizeof(names));
	memset(descriptions, 0, sizeof(descriptions));
	memset(map_ids, 0, sizeof(map_ids));
}

s_blf_chunk_scenario::s_blf_chunk_scenario()
{
	FUNCTION_BEGIN(true);

	s_blf_header::setup('levl', sizeof(*this), 3, 2);

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

bool __cdecl network_blf_verify_start_of_file(char const* buffer, long buffer_size, bool* out_byte_swap, long* out_chunk_size)
{
	FUNCTION_BEGIN(true);

	if (out_chunk_size)
		*out_chunk_size = 0;

	if (buffer_size < sizeof(s_blf_chunk_start_of_file))
		return false;

	s_blf_chunk_start_of_file const* chunk = reinterpret_cast<s_blf_chunk_start_of_file const*>(buffer);

	tag chunk_signature = chunk->signature;
	long chunk_size = chunk->chunk_size;
	short chunk_major_version = chunk->major_version;
	short chunk_minor_version = chunk->minor_version;

	if (out_byte_swap)
		*out_byte_swap = false;

	if (_byteswap_ushort(chunk->byte_order_mark) == 0xFFFE)
	{
		chunk_signature = _byteswap_ulong(chunk_signature);
		chunk_size = _byteswap_ulong(chunk_size);
		chunk_major_version = _byteswap_ushort(chunk_major_version);
		chunk_minor_version = _byteswap_ushort(chunk_minor_version);

		if (out_byte_swap)
			*out_byte_swap = true;
	}

	if (chunk_signature != '_blf' || chunk_major_version != 1 || chunk_size < sizeof(s_blf_chunk_start_of_file))
		return false;

	if (out_chunk_size)
		*out_chunk_size = chunk_size;

	return true;
}

bool __cdecl network_blf_find_chunk(char const* buffer, long buffer_size, bool byte_swap, long signature, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* out_chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk)
{
	FUNCTION_BEGIN(true);

	bool result = false;

	while (!result)
	{
		char const* chunk_buffer = nullptr;
		long chunk_size = 0;
		bool eof_chunk = false;

		bool read_result = network_blf_read_for_known_chunk(buffer, buffer_size, byte_swap, signature, major_version, &chunk_size, &chunk_buffer, out_chunk_buffer_size, out_minor_version, &eof_chunk);

		if (out_chunk_size)
			*out_chunk_size = chunk_size;

		if (out_chunk_buffer)
			*out_chunk_buffer = chunk_buffer;

		if (out_eof_chunk)
			*out_eof_chunk = eof_chunk;

		result = chunk_buffer && chunk_size > 0;

		if (!read_result || eof_chunk || chunk_size <= 0)
			break;

		buffer_size -= chunk_size;
		buffer += chunk_size;
	}

	return result;
}

bool __cdecl network_blf_read_for_known_chunk(char const* buffer, long buffer_size, bool byte_swap, long signature, short major_version, long* out_chunk_size, char const** out_chunk_buffer, long* out_chunk_buffer_size, short* out_minor_version, bool* out_eof_chunk)
{
	FUNCTION_BEGIN(true);

	assert(out_chunk_size);

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

	s_blf_header const* chunk = reinterpret_cast<s_blf_header const*>(buffer);

	if (buffer_size >= sizeof(s_blf_header))
	{
		tag chunk_signature = chunk->signature;
		long chunk_size = chunk->chunk_size;
		short chunk_major_version = chunk->major_version;
		short chunk_minor_version = chunk->minor_version;

		if (byte_swap)
		{
			chunk_signature = _byteswap_ulong(chunk_signature);
			chunk_size = _byteswap_ulong(chunk_size);
			chunk_major_version = _byteswap_ushort(chunk_major_version);
			chunk_minor_version = _byteswap_ushort(chunk_minor_version);
		}

		if (chunk_size >= sizeof(s_blf_header) && chunk_size <= buffer_size && chunk_major_version >= 0 && chunk_minor_version >= 0)
		{
			*out_chunk_size = chunk_size;
			if (chunk_signature == signature && chunk_major_version == major_version)
			{
				if (out_minor_version)
					*out_minor_version = chunk_minor_version;

				if (out_chunk_buffer)
					*out_chunk_buffer = buffer + sizeof(s_blf_header);

				if (out_chunk_buffer_size)
					*out_chunk_buffer_size = chunk_size - sizeof(s_blf_header);

				result = true;

				if (chunk_signature == '_eof' && chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_size >= sizeof(s_blf_chunk_end_of_file) && out_eof_chunk)
				{
					*out_eof_chunk = true;
					result = true;
				}
			}
			else if (chunk_signature == '_eof')
			{
				if (chunk_size >= sizeof(s_blf_chunk_end_of_file) && buffer_size >= sizeof(s_blf_chunk_end_of_file))
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

bool network_blf_verify_end_of_file(char const* buffer, long buffer_size, bool byte_swap, char const* eof_chunk_buffer, e_blf_file_authentication_type authentication_type)
{
	FUNCTION_BEGIN(true);

	bool result = false;
	HOOK_INVOKE(result =, network_blf_verify_end_of_file, buffer, buffer_size, byte_swap, eof_chunk_buffer, authentication_type);
	return result;
}
