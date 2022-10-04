#include "networking/tools/network_blf.hpp"

#include "config/version.hpp"
#include "cseries/console.hpp"
#include "game/game_globals.hpp"
#include "memory/byte_swapping.hpp"

#include <string.h>

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

	//byte_order_marker = -2;
	byte_order_marker = _byteswap_ushort(-2);

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

	memset(build_name, 0, sizeof(build_name));
	build_identifier = 0;
	memset(build_string, 0, sizeof(build_string));

	memset(author_name, 0, sizeof(author_name));
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
	memset(image_file_base, 0, sizeof(image_file_base));
	memset(scenario_path, 0, sizeof(scenario_path));
	multiplayer_minimum_desired_players = 0;
	multiplayer_maximum_desired_players = 0;
	memset(engine_maximum_teams, 0, sizeof(engine_maximum_teams));
	allows_saved_films = false;
	memset(insertions, 0, sizeof(insertions));
}
