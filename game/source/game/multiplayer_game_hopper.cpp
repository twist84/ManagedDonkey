#include "game/multiplayer_game_hopper.hpp"

#include "cache/cache_files.hpp"
#include "config/version.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "interface/user_interface.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "math/random_math.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/logic/storage/network_storage_files.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_time.hpp"
#include "networking/network_utilities.hpp"
#include "networking/online/online.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/online/online_url.hpp"
#include "networking/session/network_session_membership.hpp"
#include "networking/tools/network_blf.hpp"

#include <string>

HOOK_DECLARE(0x00545700, multiplayer_game_hopper_catalog_load_status);
HOOK_DECLARE(0x00545710, multiplayer_game_hopper_check_required_files);
HOOK_DECLARE(0x005469E0, multiplayer_game_hopper_dispose);
HOOK_DECLARE(0x00548210, multiplayer_game_hopper_game_set_load_status);
HOOK_DECLARE(0x00548220, multiplayer_game_hopper_game_variant_load_status);
HOOK_DECLARE(0x00548230, multiplayer_game_hopper_get_category_from_index);
HOOK_DECLARE(0x00548240, multiplayer_game_hopper_get_current_game_variant);
HOOK_DECLARE(0x00548250, multiplayer_game_hopper_get_current_hopper_identifier);
HOOK_DECLARE(0x00548260, multiplayer_game_hopper_get_current_map_variant);
HOOK_DECLARE(0x00548270, multiplayer_game_hopper_get_description);
HOOK_DECLARE(0x00548280, multiplayer_game_hopper_get_hopper_identifier);
HOOK_DECLARE(0x005483B0, multiplayer_game_hopper_initialize);
HOOK_DECLARE(0x005483C0, multiplayer_game_hopper_is_hopper_visible);
HOOK_DECLARE(0x005483E0, multiplayer_game_hopper_map_variant_load_status);
HOOK_DECLARE(0x00548E90, multiplayer_game_hopper_request_game_variant);
HOOK_DECLARE(0x00548EA0, multiplayer_game_hopper_request_map_variant);
HOOK_DECLARE(0x00548EB0, multiplayer_game_hopper_set_active_hopper_and_request_game_set);
HOOK_DECLARE(0x00549610, multiplayer_game_hopper_update);
HOOK_DECLARE(0x00549620, multiplayer_game_hoppers_get_current_hopper_configuration);
HOOK_DECLARE(0x00549630, multiplayer_game_hoppers_get_hopper_configuration);
HOOK_DECLARE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection);
HOOK_DECLARE(0x00549650, multiplayer_game_is_playable);

int32 const k_multiplayer_game_hopper_pack_game_variant_buffer_size = sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(c_game_variant) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file);

enum e_multiplayer_files
{
	_multiplayer_file_configuration = 0,
	_multiplayer_file_description,
	_multiplayer_file_game_set,
	_multiplayer_file_game_variant,
	_multiplayer_file_map_variant,

	k_multiplayer_file_count
};

struct s_multiplayer_game_hopper_globals
{
	c_hopper_configuration* current_configuration;

	byte __data4[0x4];
	multiplayer_file_load multiplayer_files[k_multiplayer_file_count];

	c_game_variant game_variant;
	c_map_variant map_variant;
	s_hopper_configuration_table configuration;
	s_game_hopper_description_table description;
	s_game_set game_set;

	uns8 configuration_download_buffer[sizeof(s_hopper_configuration_table)];
	uns8 description_download_buffer[sizeof(s_game_hopper_description_table)];
	uns8 game_set_download_buffer[sizeof(s_game_set)];
	uns8 game_variant_download_buffer[sizeof(c_game_variant)];
	uns8 map_variant_download_buffer[sizeof(c_map_variant)];

	byte __data35BE4[0x4];
};

s_multiplayer_game_hopper_globals multiplayer_game_hopper_globals{};

bool force_hopper_load_status_complete = false;
int32 override_game_entry_index = NONE;

e_hopper_load_status multiplayer_file_load::get_load_status()
{
	if (force_hopper_load_status_complete)
	{
		return _hopper_load_complete;
	}

	if (had_load_failure)
	{
		return _hopper_load_failed;
	}

	if (request_cookie)
	{
		return _hopper_load_pending;
	}

	if (is_valid)
	{
		return _hopper_load_complete;
	}

	return _hopper_load_none;
}

#define MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(multiplayer_files, url, buffer) multiplayer_game_hopper_load_retried_file_helper(multiplayer_files, url, buffer, sizeof(buffer))
void __cdecl multiplayer_game_hopper_load_retried_file_helper(e_multiplayer_files multiplayer_files, const char* url, void* buffer, int32 buffer_length)
{
	multiplayer_file_load* multiplayer_file = &multiplayer_game_hopper_globals.multiplayer_files[multiplayer_files];
	if (!multiplayer_file->request_cookie)
	{
		uns32 time = network_time_get();
		if (time >= uns32(multiplayer_file->time_of_last_load + multiplayer_file->retry_interval))
		{
			c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_required);
			c_network_http_request_description request_description(url, _http_request_type_get, &multiplayer_file->configuration_hash);
			if (multiplayer_file->had_load_failure)
			{
				multiplayer_file->request_cookie = request_queue->start_request_into_buffer(_online_lsp_service_type_title_files, &request_description, (char*)buffer, buffer_length);
			}
			else if (request_queue->has_file_changed(&request_description))
			{
				multiplayer_file->retry_interval = 0;
				multiplayer_file->request_cookie = request_queue->start_request_into_buffer(_online_lsp_service_type_title_files, &request_description, (char*)buffer, buffer_length);
			}
			else
			{
				multiplayer_file->time_of_last_load = time;
			}
		}
	}
}

void __cdecl initialize_fake_hopper(s_hopper_configuration_table* configuration, s_game_hopper_description_table* description, s_game_set* game_set_a, s_game_set* game_set_b)
{
	INVOKE(0x00544AC0, initialize_fake_hopper, configuration, description, game_set_a, game_set_b);
}

e_hopper_load_status __cdecl multiplayer_game_hopper_catalog_load_status()
{
	//return INVOKE(0x00545700, multiplayer_game_hopper_catalog_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_configuration].get_load_status();
}

e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier)
{
	//return INVOKE(0x00545710, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier);

	check_hopper = false;
	valid_hopper_identifier = false;

	e_session_game_start_error result = _session_game_start_error_none;
	//HOOK_INVOKE(result =, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier)
	return result;
}

//.text:005457F0 ; void __cdecl multiplayer_game_hopper_client_compute_repeated_play_adjustment_weight(e_controller_index, uns16, int32, int32, int32, int32, const uns64*, int32, int32*, bool*)
//.text:00545930 ; void __cdecl multiplayer_game_hopper_client_modify_repeated_play_list(e_controller_index, uns16, const uns64*, int32)
//.text:005459C0 ; int32 __cdecl multiplayer_game_hopper_compute_displayed_skill_level(real32, real32, int32, int32)
//.text:005459D0 ; void __cdecl multiplayer_game_hopper_compute_experience_rank_and_grade(int32, int32, int32, int32*, e_experience_rank*, e_experience_grade*)
//.text:00545A70 ; int32 __cdecl multiplayer_game_hopper_compute_experience_to_rank_up(int32, int32, int32)
//.text:00545B00 ; real32 __cdecl multiplayer_game_hopper_compute_gather_chance(int32)
//.text:00545B10 ; int32 __cdecl multiplayer_game_hopper_compute_match_quality(const c_network_session_membership*)
//.text:00545B20 ; int32 __cdecl multiplayer_game_hopper_compute_quality_adjusted_skill_update_weight(int32, int32)
//.text:00545C10 ; int32 __cdecl multiplayer_game_hopper_compute_skill_level(real32, real32, int32)
//.text:00545C20 ; int32 __cdecl multiplayer_game_hopper_compute_skill_match_delta(int32)
//.text:00545C30 ; int32 __cdecl multiplayer_game_hopper_compute_skill_to_rank_up(int32, int32, int32)
//.text:00545CD0 ; int32 __cdecl multiplayer_game_hopper_compute_skill_update_weight(int32, int32)
//.text:00545E80 ; bool __cdecl multiplayer_game_hopper_decode(c_bitstream*, s_hopper_configuration_table*)
//.text:00546820 ; bool __cdecl multiplayer_game_hopper_description_decode(c_bitstream*, s_game_hopper_description_table*)
//.text:005468A0 ; void __cdecl multiplayer_game_hopper_description_encode(c_bitstream*, const s_game_hopper_description_table*)

void __cdecl multiplayer_game_hopper_dispose()
{
	//INVOKE(0x005469E0, multiplayer_game_hopper_dispose);

	csmemset(&multiplayer_game_hopper_globals, 0, sizeof(multiplayer_game_hopper_globals));
}

//.text:005469F0 ; void __cdecl multiplayer_game_hopper_encode(c_bitstream*, const s_hopper_configuration_table*)

e_hopper_load_status __cdecl multiplayer_game_hopper_game_set_load_status()
{
	//return INVOKE(0x00548210, multiplayer_game_hopper_game_set_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set].get_load_status();
}

e_hopper_load_status __cdecl multiplayer_game_hopper_game_variant_load_status()
{
	//return INVOKE(0x00548220, multiplayer_game_hopper_game_variant_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_variant].get_load_status();
}

const s_game_hopper_custom_category* __cdecl multiplayer_game_hopper_get_category_from_index(int32 category_index)
{
	//return INVOKE(0x00548230, multiplayer_game_hopper_get_category_from_index, category_index);

	if (category_index < multiplayer_game_hopper_globals.configuration.hopper_configuration_count)
	{
		return &multiplayer_game_hopper_globals.configuration.hopper_category[category_index];
	}

	return NULL;
}

const c_game_variant* __cdecl multiplayer_game_hopper_get_current_game_variant()
{
	//return INVOKE(0x00548240, multiplayer_game_hopper_get_current_game_variant);

	ASSERT(multiplayer_game_hopper_game_variant_load_status() == _hopper_load_complete);

	return &multiplayer_game_hopper_globals.game_variant;
}

uns16 __cdecl multiplayer_game_hopper_get_current_hopper_identifier()
{
	//return INVOKE(0x00548250, multiplayer_game_hopper_get_current_hopper_identifier);

	if (multiplayer_game_hopper_globals.current_configuration)
	{
		return multiplayer_game_hopper_globals.current_configuration->m_universal.hopper_identifier;
	}

	return NONE;
}

const c_map_variant* __cdecl multiplayer_game_hopper_get_current_map_variant()
{
	//return INVOKE(0x00548260, multiplayer_game_hopper_get_current_map_variant);

	ASSERT(multiplayer_game_hopper_map_variant_load_status() == _hopper_load_complete);

	return &multiplayer_game_hopper_globals.map_variant;
}

const utf8* __cdecl multiplayer_game_hopper_get_description(uns16 hopper_identifier)
{
	//INVOKE(0x00548270, multiplayer_game_hopper_get_description, hopper_identifier);

	for (int32 description_index = 0; description_index < multiplayer_game_hopper_globals.description.hopper_description_count; description_index++)
	{
		if (multiplayer_game_hopper_globals.description.hopper_descriptions[description_index].hopper_identifier == hopper_identifier)
		{
			return multiplayer_game_hopper_globals.description.hopper_descriptions[description_index].hopper_description.get_string();
		}
	}

	return NULL;
}

uns16 __cdecl multiplayer_game_hopper_get_hopper_identifier(int32 hopper_index)
{
	//return INVOKE(0x00548280, multiplayer_game_hopper_get_hopper_identifier, hopper_index);

	ASSERT(hopper_index >= 0 && hopper_index < k_hopper_maximum_hopper_count);

	if (hopper_index < multiplayer_game_hopper_globals.configuration.hopper_category_count)
	{
		//uns16 hopper_identifier = multiplayer_game_hopper_globals.configuration.hopper_configurations[hopper_index].get_hopper_identifier();
		uns16 hopper_identifier = multiplayer_game_hopper_globals.configuration.hopper_configurations[hopper_index].m_universal.hopper_identifier;
		if (hopper_identifier != NONE)
		{
			return hopper_identifier;
		}
	}

	return NONE;
}

//.text:00548290 ; const utf8* __cdecl multiplayer_game_hopper_get_name(uns16)

void __cdecl multiplayer_game_hopper_get_players_status(const c_hopper_configuration* hopper, const c_network_session_membership* session_membership, multiplayer_hopper_check* check)
{
	INVOKE(0x005482A0, multiplayer_game_hopper_get_players_status, hopper, session_membership, check);
}

//.text:005483A0 ; uns64 __cdecl multiplayer_game_hopper_get_required_map_mask_from_game_set()

void __cdecl multiplayer_game_hopper_initialize()
{
	//INVOKE(0x005483B0, multiplayer_game_hopper_initialize);

	csmemset(&multiplayer_game_hopper_globals, 0, sizeof(multiplayer_game_hopper_globals));

	// for now give us fake data
	s_game_set game_set{};
	initialize_fake_hopper(&multiplayer_game_hopper_globals.configuration, &multiplayer_game_hopper_globals.description, &multiplayer_game_hopper_globals.game_set, &game_set);
}

bool __cdecl multiplayer_game_hopper_is_hopper_visible(uns16 hopper_identifier, const c_network_session_membership* session_membership)
{
	//return INVOKE(0x005483C0, multiplayer_game_hopper_is_hopper_visible, hopper_identifier, session_membership);

	return true;

	//bool result = false;
	//if (c_hopper_configuration* hopper_configuration = multiplayer_game_hoppers_get_hopper_configuration(hopper_identifier))
	//{
	//	multiplayer_hopper_check check{};
	//	multiplayer_game_hopper_get_players_status(hopper_configuration, session_membership, &check);
	//
	//	result = check.player_mask == check.required_region_mask;
	//
	//	if (hopper_configuration->hide_hopper_from_games_played_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_games_played_requirements)
	//		{
	//			if (check.player_mask != (check.games_played_too_high_mask & check.games_played_too_low_mask))
	//				result = false;
	//		}
	//		else if ((check.games_played_too_high_mask & check.games_played_too_low_mask) == 0)
	//		{
	//			result = false;
	//		}
	//	}
	//
	//	if (hopper_configuration->hide_hopper_from_experience_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_experience_requirements)
	//		{
	//			if (check.player_mask != (check.games_played_too_high_mask & check.games_played_too_low_mask))
	//				result = false;
	//		}
	//		else if ((check.games_played_too_high_mask & check.games_played_too_low_mask) == 0)
	//		{
	//			result = false;
	//		}
	//	}
	//
	//	if (hopper_configuration->hide_hopper_from_access_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_access_requirements)
	//		{
	//			if (check.player_mask != (check.experience_too_high & check.experience_too_low))
	//				result = false;
	//		}
	//		else if ((check.experience_too_high & check.experience_too_low) == 0)
	//		{
	//			result = false;
	//		}
	//	}
	//
	//	if (hopper_configuration->hide_hopper_from_live_account_access_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_live_account_access_requirements)
	//		{
	//			if (check.player_mask != check.access_mask)
	//				result = false;
	//		}
	//		else if (!check.access_mask)
	//		{
	//			result = false;
	//		}
	//	}
	//
	//	if (false && hopper_configuration->hide_hopper_due_to_time_restriction)
	//	{
	//		s_file_last_modification_date current_time{};
	//		get_current_file_time(&current_time);
	//
	//		//if (hopper_configuration->get_hopper_start_time())
	//		//{
	//		//	s_file_last_modification_date hopper_start_time = hopper->get_hopper_start_time();
	//		//	if (hopper_start_time > current_time)
	//		//		result = false;
	//		//}
	//		//
	//		//if (hopper_configuration->get_hopper_end_time())
	//		//{
	//		//	s_file_last_modification_date hopper_end_time = hopper_configuration->get_hopper_end_time();
	//		//	if (hopper_end_time < current_time)
	//		//		result = false;
	//		//}
	//
	//		if (*(uns64*)&hopper_configuration->start_time > *(uns64*)&current_time)
	//		{
	//			result = false;
	//		}
	//
	//		if (*(uns64*)&hopper_configuration->end_time < *(uns64*)&current_time)
	//		{
	//			result = false;
	//		}
	//	}
	//}
	//
	//return result;
}

//.text:005483D0 ; bool __cdecl multiplayer_game_hopper_is_valid(uns16)

e_hopper_load_status __cdecl multiplayer_game_hopper_map_variant_load_status()
{
	//return INVOKE(0x005483E0, multiplayer_game_hopper_map_variant_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_map_variant].get_load_status();
}

//.text:005483F0 ; int32 __cdecl multiplayer_game_hopper_pack_game_set(void*, int32, const s_game_set*)

int32 __cdecl multiplayer_game_hopper_pack_game_variant(void* buffer, int32 buffer_size, const c_game_variant* game_variant)
{
	return INVOKE(0x00548610, multiplayer_game_hopper_pack_game_variant, buffer, buffer_size, game_variant);

	//ASSERT(buffer_size >= k_multiplayer_game_hopper_pack_game_variant_buffer_size);
	//
	//byte* start = (byte*)buffer;
	//s_blf_chunk_start_of_file* start_of_file = (s_blf_chunk_start_of_file*)start;
	//start_of_file->initialize();
	//start_of_file->name.set("game var");
	//
	//s_blf_chunk_author* author = (s_blf_chunk_author*)offset_pointer(start_of_file, sizeof(s_blf_chunk_start_of_file));
	//author->initialize();
	//author->build_name.set(version_get_build_name());
	//author->build_identifier = bswap_uns64(version_get_build_number_identifier());
	//author->build_string.set(version_get_build_string());
	//author->author_name.clear();
	//
	//s_blf_header* map_variant_header = (s_blf_header*)offset_pointer(author, sizeof(s_blf_chunk_author));
	//byte* map_variant_data = (byte*)offset_pointer(map_variant_header, sizeof(s_blf_header));
	//c_bitstream bitstream(map_variant_data, buffer_size - (map_variant_data - start));
	//bitstream.begin_writing(1);
	//
	//game_variant->encode(&bitstream);
	//
	//int32 space_used_in_bytes = (bitstream.get_space_used_in_bits() + CHAR_BITS - 1) / CHAR_BITS;
	//map_variant_header->setup('gvar', sizeof(s_blf_header) + space_used_in_bytes, 10, 1);
	//
	//bitstream.finish_writing(NULL);
	//
	//s_blf_chunk_end_of_file* end_of_file = (s_blf_chunk_end_of_file*)offset_pointer(map_variant_data, sizeof(s_blf_chunk_end_of_file));
	//end_of_file->initialize();
	//end_of_file->file_size = bswap_uns32((byte*)end_of_file - start);
	//end_of_file->authentication_type = _blf_file_authentication_type_none;
	//
	//byte* current = (byte*)offset_pointer(end_of_file, sizeof(s_blf_chunk_end_of_file));
	//
	//c_game_variant test_game_variant = c_game_variant();
	//bool success = multiplayer_game_hopper_unpack_game_variant(buffer, current - buffer, &test_game_variant);
	//ASSERT(success);
	//ASSERT(!csmemcmp(&test_game_variant, game_variant, sizeof(c_game_variant)));
	//
	//ASSERT(current - start <= buffer_size);
	//return current - start;
}

//.text:00548830 ; int32 __cdecl multiplayer_game_hopper_pack_hopper_description(void*, int32, const s_game_hopper_description_table*)
//.text:00548A50 ; int32 __cdecl multiplayer_game_hopper_pack_hopper_file(void*, int32, const s_hopper_configuration_table*)

int32 __cdecl multiplayer_game_hopper_pack_map_variant(void* buffer, int32 buffer_size, const c_map_variant* map_variant)
{
	return INVOKE(0x00548C70, multiplayer_game_hopper_pack_map_variant, buffer, buffer_size, map_variant);

	//ASSERT(buffer_size > sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));
	//
	//byte* start = (byte*)buffer;
	//s_blf_chunk_start_of_file* start_of_file = (s_blf_chunk_start_of_file*)start;
	//start_of_file->initialize();
	//start_of_file->name.set("map variant");
	//
	//s_blf_chunk_author* author = (s_blf_chunk_author*)offset_pointer(start_of_file, sizeof(s_blf_chunk_start_of_file));
	//author->initialize();
	//author->build_name.set(version_get_build_name());
	//author->build_identifier = bswap_uns64(version_get_build_number_identifier());
	//author->build_string.set(version_get_build_string());
	//author->author_name.clear();
	//
	//s_blf_header* map_variant_header = (s_blf_header*)offset_pointer(author, sizeof(s_blf_chunk_author));
	//byte* map_variant_data = (byte*)offset_pointer(map_variant_header, sizeof(s_blf_header));
	//c_bitstream bitstream(map_variant_data, buffer_size - (map_variant_data - start));
	//bitstream.begin_writing(1);
	//
	//map_variant->encode(&bitstream);
	//
	//int32 space_used_in_bytes = (bitstream.get_space_used_in_bits() + CHAR_BITS-1) / CHAR_BITS;
	//map_variant_header->setup('mvar', sizeof(s_blf_header) + space_used_in_bytes, 12, 1);
	//
	//bitstream.finish_writing(NULL);
	//
	//s_blf_chunk_end_of_file* end_of_file = (s_blf_chunk_end_of_file*)offset_pointer(map_variant_data, sizeof(s_blf_chunk_end_of_file));
	//end_of_file->initialize();
	//end_of_file->file_size = bswap_uns32((byte*)end_of_file - start);
	//end_of_file->authentication_type = _blf_file_authentication_type_none;
	//
	//byte* current = (byte*)offset_pointer(end_of_file, sizeof(s_blf_chunk_end_of_file));
	//
	//{
	//	c_map_variant test_map_variant = c_map_variant();
	//	c_map_variant map_variant_copy = c_map_variant();
	//
	//	csmemset(&test_map_variant, 0, sizeof(c_map_variant));
	//
	//	csmemcpy(&map_variant_copy, map_variant, sizeof(c_map_variant));
	//	bool success = multiplayer_game_hopper_unpack_map_variant(start, current - start, &test_map_variant);
	//	if (!map_variant_copy.lossily_compare_to(&test_map_variant))
	//		ASSERT(success);
	//
	//	byte* map_variant_copy_start = (byte*)&map_variant_copy;
	//	byte* test_map_variant_start = (byte*)&test_map_variant;
	//	for (int32 byte_index = 0; byte_index < sizeof(c_map_variant); byte_index++)
	//	{
	//		byte* map_variant_copy_byte = map_variant_copy_start + byte_index;
	//		byte* test_map_variant_byte = test_map_variant_start + byte_index;
	//		if (map_variant_copy_byte != test_map_variant_byte)
	//		{
	//			VASSERT(c_string_builder("Map variant encode/decode mismatch at byte index %d. 0x%02X != 0x%02X",
	//				byte_index,
	//				map_variant_copy_byte,
	//				test_map_variant_start).get_string());
	//		}
	//	}
	//
	//	// c_map_variant::~c_map_variant()
	//	DECLFUNC(0x00580C60, void, __thiscall, c_map_variant*)(&map_variant_copy);
	//	DECLFUNC(0x00580C60, void, __thiscall, c_map_variant*)(&test_map_variant);
	//}
	//
	//ASSERT(current - start <= buffer_size);
	//return current - start;
}

void __cdecl multiplayer_game_hopper_request_game_variant(uns16 hopper_identifier, const char* variant_name, const s_network_http_request_hash* hash)
{
	//INVOKE(0x00548E90, multiplayer_game_hopper_request_game_variant, hopper_identifier, variant_name, hash);

	c_url_string url;
	multiplayer_file_load& multiplayer_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_variant];
	if (multiplayer_file.request_cookie)
	{
		c_network_http_request_queue::get(_network_http_request_queue_type_required)->cancel_request(multiplayer_file.request_cookie);
		multiplayer_file.request_cookie = 0;
	}
	multiplayer_file.time_of_last_load = 0;
	multiplayer_file.retry_interval = 0;
	online_url_make_matchmaking_game_variant(&url, hopper_identifier, variant_name);
	MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_game_variant, url.get_string(), multiplayer_game_hopper_globals.game_variant_download_buffer);
	multiplayer_file.had_load_failure = false;
}

void __cdecl multiplayer_game_hopper_request_map_variant(uns16 hopper_identifier, const char* variant_name, const s_network_http_request_hash* hash)
{
	//INVOKE(0x00548EA0, multiplayer_game_hopper_request_map_variant, hopper_identifier, variant_name, hash);

	c_url_string url;
	multiplayer_file_load& multiplayer_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_map_variant];
	if (multiplayer_file.request_cookie)
	{
		c_network_http_request_queue::get(_network_http_request_queue_type_required)->cancel_request(multiplayer_file.request_cookie);
		multiplayer_file.request_cookie = 0;
	}
	multiplayer_file.time_of_last_load = 0;
	multiplayer_file.retry_interval = 0;
	online_url_make_matchmaking_map_variant(&url, hopper_identifier, variant_name);
	MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_map_variant, url.get_string(), multiplayer_game_hopper_globals.game_variant_download_buffer);
	multiplayer_file.had_load_failure = false;
}

bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(uns16 hopper_identifier)
{
	//return INVOKE(0x00548EB0, multiplayer_game_hopper_set_active_hopper_and_request_game_set, hopper_identifier);

	bool result = false;

	c_hopper_configuration* hopper_configuration = multiplayer_game_hoppers_get_hopper_configuration(hopper_identifier);
	if (!hopper_configuration)
	{
		if (multiplayer_game_hopper_globals.configuration.hopper_category_count > 0)
		{
			hopper_configuration = &multiplayer_game_hopper_globals.configuration.hopper_configurations[0];
		}
		else
		{
			VASSERT("unreachable");
		}
	}

	if (!hopper_configuration ||
		hopper_configuration == multiplayer_game_hopper_globals.current_configuration &&
		multiplayer_game_hopper_game_set_load_status())
	{
		event(_event_warning, "multiplayer_game_hopper: tried to set the current hopper to something invalid- %04X",
			hopper_identifier);
	}
	else
	{
		c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_required);

		multiplayer_file_load& game_set_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set];
		if (game_set_file.request_cookie)
		{
			request_queue->cancel_request(game_set_file.request_cookie);
			game_set_file.request_cookie = 0;
		}

		game_set_file.time_of_last_load = 0;
		game_set_file.retry_interval = 0;

		result = true;
	}

	multiplayer_game_hopper_globals.current_configuration = hopper_configuration;

	return result;
}

void __cdecl multiplayer_game_hopper_set_game_for_current_hopper(int32 game_index)
{
	INVOKE(0x00548EC0, multiplayer_game_hopper_set_game_for_current_hopper, game_index);

	//if (game_index == NONE)
	//{
	//	console_printf("resetting game for hopper");
	//	override_game_entry_index = NONE;
	//	return;
	//}
	//
	//s_game_set* game_set = &multiplayer_game_hopper_globals.game_set;
	//ASSERT(VALID_COUNT(game_set->game_entry_count, NUMBEROF(game_set->entries)));
	//
	//for (int32 game_entry_index = 0; game_entry_index < game_set->game_entry_count; game_entry_index++)
	//{
	//	if (game_entry_index != game_index)
	//		continue;
	//
	//	if (s_game_set_entry* entry = &game_set->entries[game_entry_index])
	//	{
	//		console_printf("setting game for hopper to %s on %d", entry->game_variant_file_name, entry->map_id);
	//		override_game_entry_index = game_index;
	//		return;
	//	}
	//	break;
	//}
	//
	//console_printf("failed to set game for current hopper");
	//override_game_entry_index = NONE;
}

bool __cdecl multiplayer_game_hopper_unpack_game_set(const void* buffer, int32 bytes_read, s_game_set* game_set)
{
	return INVOKE(0x00548ED0, multiplayer_game_hopper_unpack_game_set, buffer, bytes_read, game_set);
}

bool packed_game_variant_is_mcc(const void* buffer_, int32 bytes_read)
{
	const byte* buffer = (const byte*)buffer_;
	const void* buffer_end = buffer + bytes_read;

	const s_blf_header* chunk_header = (const s_blf_header*)buffer;
	while (chunk_header->chunk_type != 'msf_')
	{
		buffer += bswap_uns32(chunk_header->chunk_size);
		chunk_header = (const s_blf_header*)buffer;

		if (buffer >= buffer_end)
		{
			return false;
		}
	}

	return chunk_header->chunk_type == 'msf_';
}

bool __cdecl multiplayer_game_hopper_unpack_game_variant(const void* buffer, int32 bytes_read, c_game_variant* game_variant)
{
	//INVOKE(0x00549050, multiplayer_game_hopper_unpack_game_variant, buffer, bytes_read, game_variant);

	if (packed_game_variant_is_mcc(buffer, bytes_read))
	{
		const void* buffer_end = offset_pointer(buffer, bytes_read);

		const s_blf_header* chunk_header = (const s_blf_header*)buffer;
		csmemset(game_variant, 0, sizeof(c_game_variant));

		ASSERT(0x9D == sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));
		ASSERT(bytes_read > sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));

		while (buffer < buffer_end && chunk_header->chunk_type != 'ravg')
		{
			buffer = offset_pointer(buffer, bswap_uns32(chunk_header->chunk_size));
			chunk_header = (const s_blf_header*)buffer;
		}

		if (buffer >= buffer_end)
		{
			return false;
		}

		int32 chunk_size = bswap_uns32(chunk_header->chunk_size) - sizeof(s_blf_header);
		byte* chunk_data = (byte*)offset_pointer(buffer, sizeof(s_blf_header));

		c_bitstream packet(chunk_data, chunk_size);
		packet.begin_reading();
		bool decode_succeeded = game_variant->decode_from_mcc(&packet);
		packet.finish_reading();

		bool result = decode_succeeded;
		if (decode_succeeded)
		{
			buffer = offset_pointer(buffer, bswap_uns32(chunk_header->chunk_size));
			chunk_header = (const s_blf_header*)buffer;

			if (buffer >= buffer_end)
			{
				return false;
			}

			// is end of file
			while (chunk_header->chunk_type != 'foe_')
			{
				buffer = offset_pointer(buffer, bswap_uns32(chunk_header->chunk_size));
				chunk_header = (const s_blf_header*)buffer;

				if (buffer >= buffer_end)
					return false;
			}

			result = false;
			if (buffer < buffer_end)
			{
				return true;
			}
		}

		return result;
	}

	return INVOKE(0x00549050, multiplayer_game_hopper_unpack_game_variant, buffer, bytes_read, game_variant);
}

bool __cdecl multiplayer_game_hopper_unpack_hopper_description(const void* buffer, int32 bytes_read, s_game_hopper_description_table* description)
{
	return INVOKE(0x005491D0, multiplayer_game_hopper_unpack_hopper_description, buffer, bytes_read, description);
}

bool __cdecl multiplayer_game_hopper_unpack_hopper_file(const void* buffer, int32 bytes_read, s_hopper_configuration_table* configuration)
{
	return INVOKE(0x00549350, multiplayer_game_hopper_unpack_hopper_file, buffer, bytes_read, configuration);
}

bool __cdecl multiplayer_game_hopper_unpack_map_variant(const void* buffer, int32 bytes_read, c_map_variant* map_variant)
{
	return INVOKE(0x005494D0, multiplayer_game_hopper_unpack_map_variant, buffer, bytes_read, map_variant);
}

void __cdecl multiplayer_game_hopper_update()
{
	//INVOKE(0x00549610, multiplayer_game_hopper_update);

	c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_required);
	for (int32 multiplayer_game_file = 0; multiplayer_game_file < k_multiplayer_file_count; multiplayer_game_file++)
	{
		multiplayer_file_load* multiplayer_file = &multiplayer_game_hopper_globals.multiplayer_files[multiplayer_game_file];
		if (!multiplayer_file->request_cookie)
		{
			continue;
		}

		bool failed = false;

		int32 bytes_read = 0;
		e_network_http_request_result request_result = request_queue->is_fill_buffer_complete(multiplayer_file->request_cookie, &bytes_read, &multiplayer_file->configuration_hash, NULL);
		if (request_result == _network_http_request_result_success)
		{
			switch (multiplayer_game_file)
			{
			case _multiplayer_file_configuration:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.configuration_download_buffer));

				if (!multiplayer_game_hopper_unpack_hopper_file(multiplayer_game_hopper_globals.configuration_download_buffer, bytes_read, &multiplayer_game_hopper_globals.configuration))
				{
					failed = true;
				}
				else
				{
					multiplayer_file_load& game_set_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set];
					if (game_set_file.request_cookie)
					{
						request_queue->cancel_request(game_set_file.request_cookie);
						game_set_file.request_cookie = 0;
						game_set_file.time_of_last_load = 0;
						game_set_file.retry_interval = 0;
					}
				}
			}
			break;
			case _multiplayer_file_description:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.description_download_buffer));

				if (!multiplayer_game_hopper_unpack_hopper_description(multiplayer_game_hopper_globals.description_download_buffer, bytes_read, &multiplayer_game_hopper_globals.description))
				{
					failed = true;
				}
			}
			break;
			case _multiplayer_file_game_set:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.game_set_download_buffer));

				if (!multiplayer_game_hopper_unpack_game_set(multiplayer_game_hopper_globals.game_set_download_buffer, bytes_read, &multiplayer_game_hopper_globals.game_set))
				{
					failed = true;
				}
			}
			break;
			case _multiplayer_file_game_variant:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.game_variant_download_buffer));

				if (!multiplayer_game_hopper_unpack_game_variant(multiplayer_game_hopper_globals.game_variant_download_buffer, bytes_read, &multiplayer_game_hopper_globals.game_variant))
				{
					failed = true;
				}
			}
			break;
			case _multiplayer_file_map_variant:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.map_variant_download_buffer));

				if (!multiplayer_game_hopper_unpack_map_variant(multiplayer_game_hopper_globals.map_variant_download_buffer, bytes_read, &multiplayer_game_hopper_globals.map_variant))
				{
					failed = true;
				}
			}
			break;
			default:
			{
				VASSERT("unreachable");
			}
			break;
			}

			if (!failed)
			{
				multiplayer_file->request_cookie = 0;
				multiplayer_file->is_valid = true;
				multiplayer_file->had_load_failure = false;
				multiplayer_file->time_of_last_load = network_time_get();
				multiplayer_file->retry_interval = g_network_configuration.config_download.required_file_invalidation_check_interval_msec;
			}
		}
		else if (request_result == _network_http_request_result_failure)
		{
			failed = true;
		}

		if (failed)
		{
			multiplayer_file->request_cookie = 0;
			multiplayer_file->is_valid = true;
			multiplayer_file->time_of_last_load = network_time_get();
			multiplayer_file->retry_interval = network_configuration_calculate_next_retry_interval(multiplayer_file->retry_interval, g_network_configuration.config_download.required_file_download_retry_interval_msec);

			event(_event_message, "networking:configuration: could not retrieve multiplayer game file [%d]",
				multiplayer_game_file);
		}
	}
}

// called in `c_life_cycle_state_handler_pre_game::update`
//   multiplayer_game_hopper_update_in_matchmaking(session_membership->is_leader());
void __cdecl multiplayer_game_hopper_update_in_matchmaking(bool is_leader)
{
	c_url_string url;

	online_url_make_matchmaking_hopper(&url);
	MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_configuration, url.get_string(), multiplayer_game_hopper_globals.configuration_download_buffer);

	if (is_leader)
	{
		online_url_make_matchmaking_descriptions(&url);
		MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_description, url.get_string(), multiplayer_game_hopper_globals.description_download_buffer);
	}

	if (multiplayer_game_hopper_globals.current_configuration)
	{
		online_url_make_matchmaking_gameset(&url, multiplayer_game_hopper_globals.current_configuration->m_universal.hopper_identifier);
		MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_game_set, url.get_string(), multiplayer_game_hopper_globals.game_set_download_buffer);
	}
}

const c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration()
{
	//INVOKE(0x00549620, multiplayer_game_hoppers_get_current_hopper_configuration);

	return multiplayer_game_hopper_globals.current_configuration;
}

c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(uns16 hopper_identifier)
{
	//return INVOKE(0x00549630, multiplayer_game_hoppers_get_hopper_configuration, hopper_identifier);

	//if (multiplayer_game_hopper_globals.current_configuration && multiplayer_game_hopper_globals.current_configuration->get_hopper_identifier() == hopper_identifier)
	if (multiplayer_game_hopper_globals.current_configuration && multiplayer_game_hopper_globals.current_configuration->m_universal.hopper_identifier == hopper_identifier)
	{
		return multiplayer_game_hopper_globals.current_configuration;
	}

	for (int32 category_index = 0; category_index < multiplayer_game_hopper_globals.configuration.hopper_category_count; category_index++)
	{
		//if (multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index].get_hopper_identifier() == hopper_identifier)
		if (multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index].m_universal.hopper_identifier == hopper_identifier)
		{
			return &multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index];
		}
	}

	return NULL;
}

bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(int32 player_count, int32 valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out)
{
	//return INVOKE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection, player_count, valid_map_mask, game_collection_out);

	bool result = true;
	bool v7 = false;
	int32 map_ids[3] = { 0, 0, 0 };

	csmemset(game_collection_out, 0, sizeof(s_game_hopper_picked_game_collection));

	if (multiplayer_game_hopper_globals.game_set.game_entry_count > 0)
	{
		for (int32 game_entry_index = 0; game_entry_index < multiplayer_game_hopper_globals.game_set.game_entry_count; game_entry_index++)
		{
			s_game_set_entry* entry = &multiplayer_game_hopper_globals.game_set.entries[game_entry_index];

			char scenario_path[256]{};
			if (!levels_get_path(_campaign_id_none, entry->map_id, scenario_path, sizeof(scenario_path)))
			{
				event(_event_error, "networking:hopper: hopper game set has invalid map id %d",
					entry->map_id);
			}
		}

		for (int32 pick_index = 0; pick_index < game_collection_out->picked_games.get_count(); pick_index++)
		{
			s_game_hopper_picked_game* picked_game = &game_collection_out->picked_games[pick_index];

			for (int32 game_entry_index = 0; game_entry_index < multiplayer_game_hopper_globals.game_set.game_entry_count; game_entry_index++)
			{
				s_game_set_entry* entry = &multiplayer_game_hopper_globals.game_set.entries[game_entry_index];

				int32 total_entry_weight = 0;

				char scenario_path[256]{};
				if ((!v7 || !entry->skip_after_veto)
					&& player_count >= entry->minimum_player_count
					&& entry->map_id != map_ids[0]
					&& entry->map_id != map_ids[1]
					&& entry->map_id != map_ids[2]
					&& levels_get_path(_campaign_id_none, entry->map_id, scenario_path, sizeof(scenario_path)))
				{
					total_entry_weight += entry->game_entry_weight;
				}

				if (total_entry_weight > 0)
				{
					int32 random_entry_weight = _random_range(get_local_random_seed_address(), NULL, __FILE__, __LINE__, 0, (int16)total_entry_weight);
					for (int32 game_entry_index = 0; game_entry_index < multiplayer_game_hopper_globals.game_set.game_entry_count; game_entry_index++)
					{
						s_game_set_entry* entry = &multiplayer_game_hopper_globals.game_set.entries[game_entry_index];

						char scenario_path[256]{};
						if ((!v7 || !entry->skip_after_veto)
							&& player_count >= entry->minimum_player_count
							&& entry->map_id != map_ids[0]
							&& entry->map_id != map_ids[1]
							&& entry->map_id != map_ids[2]
							&& levels_get_path(_campaign_id_none, entry->map_id, scenario_path, sizeof(scenario_path)))
						{
							if (random_entry_weight < entry->game_entry_weight)
							{
								entry->game_variant_file_name.copy_to(picked_game->game_variant_name, 32);
								entry->map_variant_file_name.copy_to(picked_game->map_variant_name, 32);
								picked_game->map_id = entry->map_id;
								map_ids[pick_index] = entry->map_id;
								break;
							}

							random_entry_weight -= entry->game_entry_weight;
						}
					}

					v7 = true;
				}
				else
				{
					event(_event_warning, "networking:multiplayer_game_hopper: game/map entry index %d has no weight to use, repeating first selection",
						pick_index);
					ASSERT(pick_index != 0);

					csmemcpy(picked_game, game_collection_out, sizeof(*picked_game));
				}
			}
		}

		if (override_game_entry_index != NONE)
		{
			if (VALID_INDEX(override_game_entry_index, multiplayer_game_hopper_globals.game_set.game_entry_count))
			{
				s_game_set_entry* entry = &multiplayer_game_hopper_globals.game_set.entries[override_game_entry_index];

				for (int32 pick_index = 0; pick_index < game_collection_out->picked_games.get_count(); pick_index++)
				{
					s_game_hopper_picked_game* picked_game = &game_collection_out->picked_games[pick_index];

					entry->game_variant_file_name.copy_to(picked_game->game_variant_name, 32);
					entry->map_variant_file_name.copy_to(picked_game->map_variant_name, 32);
					picked_game->map_id = entry->map_id;
				}

				event(_event_warning, "networking:game:hopper: overriding hopper game selection with game %d",
					override_game_entry_index);
			}
			else
			{
				event(_event_warning, "networking:game:hopper: ignoring invalid game index %d for override",
					override_game_entry_index);
			}
		}
	}
	else
	{
		event(_event_error, "networking:multiplayer_game_hopper: current game set has 0 entries!");
		result = false;
	}

	if (result)
	{
		for (int32 pick_index = 0; pick_index < game_collection_out->picked_games.get_count(); pick_index++)
		{
			s_game_hopper_picked_game* game_map = &game_collection_out->picked_games[pick_index];
			ASSERT(strlen(game_map->game_variant_name) > 0);
			ASSERT(game_map->map_id > 0);
		}
	}

	return result;
}

e_session_game_start_error __cdecl multiplayer_game_is_playable(uns16 hopper_identifier, bool is_matchmaking, bool check_hopper, const c_network_session_membership* session_membership, uns16* out_player_error_mask)
{
	//return INVOKE(0x00549650, multiplayer_game_is_playable, hopper_identifier, is_matchmaking, check_hopper, session_membership, out_player_error_mask);

	e_session_game_start_error game_start_error = _session_game_start_error_none;
	uns16 player_mask = 0;

	c_hopper_configuration* hopper = multiplayer_game_hoppers_get_hopper_configuration(hopper_identifier);

	if (check_hopper && !is_matchmaking)
	{
		if (online_is_connected_to_live() || !online_has_any_silver_or_gold_live_users())
		{
			if (online_has_all_online_enabled_users())
			{
				game_start_error = _session_game_start_match_error_must_be_in_live_lobby_for_match;
			}
			else
			{
				//for (int32 player_index = session_membership->get_first_player();
				//	player_index != NONE;
				//	player_index = session_membership->get_next_player(player_index))
				//{
				//	if (!session_membership->is_player_online_enabled(player_index))
				//		player_mask |= FLAG(player_index);
				//}

				game_start_error = _session_game_start_match_error_must_have_online_enabled_profiles_for_match;
			}
		}
		else
		{
			game_start_error = _session_game_start_match_error_must_connect_to_live_for_match;
		}
	}

	if (game_start_error == _session_game_start_error_none && check_hopper)
	{
		game_start_error = multiplayer_game_hopper_check_required_files(check_hopper, hopper_identifier != int16(0xFFFF));
	}

	if (game_start_error == _session_game_start_error_none && check_hopper && (hopper_identifier == int16(0xFFFF) || hopper == NULL))
	{
		game_start_error = _session_game_start_match_error_invalid_hopper;
	}

	if (game_start_error == _session_game_start_error_none && is_matchmaking)
	{
		//for (int32 player_index = session_membership->get_first_player();
		//	player_index != NONE;
		//	player_index = session_membership->get_next_player(player_index))
		//{
		//	if (!session_membership->is_player_online_enabled(player_index))
		//	{
		//		player_mask |= FLAG(player_index);
		//		game_start_error = _session_game_start_error_account_not_online_enabled;
		//	}
		//}
	}

	if (game_start_error == _session_game_start_error_none && network_squad_session_get_start_mode() == _network_game_start_mode_custom_game && get_alpha_custom_games_disabled())
	{
		game_start_error = _session_game_start_error_custom_games_are_disabled_for_alpha;
	}

	if (game_start_error == _session_game_start_error_none)
	{
		for (int32 peer_index = session_membership->get_first_peer();
			peer_index != NONE;
			peer_index = session_membership->get_next_peer(peer_index))
		{
			//const s_network_session_peer* peer = session_membership->get_peer(peer_index);
			const s_network_session_peer* peer = &session_membership->m_shared_network_membership.peers[peer_index];
			if (bit_vector_count_bits(peer->player_mask, 16) > network_configuration_maximum_multiplayer_split_screen())
			{
				int32 player_index = bit_vector_lowest_bit_set(peer->player_mask, 16);
				if (player_index != NONE)
				{
					player_mask |= FLAG(player_index);
				}
			}

			if (player_mask)
			{
				game_start_error = _session_game_start_error_maximum_multiplayer_split_screen_exceeded;
			}
		}
	}

	if (false && hopper)
	{
		s_file_last_modification_date current_time{};
		get_current_file_time(&current_time);
		if (game_start_error == _session_game_start_error_none && hopper_identifier != int16(0xFFFF))
		{
			//if (hopper->get_hopper_start_time())
			//{
			//	s_file_last_modification_date hopper_start_time = hopper->get_hopper_start_time();
			//	if (hopper_start_time > current_time)
			//		game_start_error = _session_game_start_match_error_not_yet_start_time;
			//}

			if (*(uns64*)&hopper->m_universal.start_time > *(uns64*)&current_time)
			{
				game_start_error = _session_game_start_match_error_not_yet_start_time;
			}
		}

		if (game_start_error == _session_game_start_error_none && hopper_identifier != int16(0xFFFF))
		{
			//if (hopper->get_hopper_end_time())
			//{
			//	s_file_last_modification_date hopper_end_time = hopper->get_hopper_end_time();
			//	if (hopper_end_time < current_time)
			//		game_start_error = _session_game_start_match_error_end_time_has_passed;
			//}

			if (*(uns64*)&hopper->m_universal.end_time < *(uns64*)&current_time)
			{
				game_start_error = _session_game_start_match_error_end_time_has_passed;
			}
		}

		//if (game_start_error == _session_game_start_error_none && hopper_identifier != int16(0xFFFF))
		//{
		//	//int32 player_count = session_membership->get_player_count();
		//	//if (player_count < hopper->get_minimum_party_size())
		//	//{
		//	//	game_start_error = _session_game_start_match_error_squad_too_small;
		//	//}
		//	//else if (player_count > hopper->get_maximum_party_size())
		//	//{
		//	//	game_start_error = _session_game_start_match_error_squad_too_large;
		//	//}
		//
		//	const s_network_session_peer* peer = &session_membership->m_shared_network_membership.peers[peer_index];
		//	int32 player_count = bit_vector_count_bits(peer->player_mask, 16);
		//	if (player_count < hopper->m_universal.minimum_party_size)
		//	{
		//		game_start_error = _session_game_start_match_error_squad_too_small;
		//	}
		//	else if (player_count > hopper->m_universal.maximum_party_size)
		//	{
		//		game_start_error = _session_game_start_match_error_squad_too_large;
		//	}
		//}

		//while (game_start_error == _session_game_start_error_none && hopper)
		//{
		//	// $TODO: hopper checks
		//}
	}

	if (out_player_error_mask)
	{
		*out_player_error_mask = player_mask;
	}

	return game_start_error;
}

//.text:00549870 ; bool __cdecl multiplayer_game_set_decode(c_bitstream*, s_game_set*)
//.text:00549970 ; void __cdecl multiplayer_game_set_encode(c_bitstream*, const s_game_set*)

const char* __cdecl multiplayer_game_start_error_to_string(e_session_game_start_error error)
{
	return INVOKE(0x00549B70, multiplayer_game_start_error_to_string, error);
}

//.text:00549DB0 ; 
//.text:00549DC0 ; 
//.text:00549DD0 ; public: bool __cdecl c_hopper_configuration::require_all_party_members_meet_access_requirements() const
//.text:00549DE0 ; public: bool __cdecl c_hopper_configuration::require_all_party_members_meet_experience_requirements() const
//.text:00549DF0 ; public: bool __cdecl c_hopper_configuration::require_all_party_members_meet_games_played_requirements() const
//.text:00549E00 ; public: bool __cdecl c_hopper_configuration::require_all_party_members_meet_live_account_access_requirements() const

void __cdecl network_build_game_variant(const char* name)
{
	byte* buffer = new byte[0x600]{};
	c_static_string<k_tag_long_string_length> filepath;

	const c_game_variant* game_variant = &game_options_get()->multiplayer_variant;
	int32 file_size = multiplayer_game_hopper_pack_game_variant(buffer, 0x600, game_variant);

	// 5:  halo3_cache_debug
	// 10: halo3_tag_test
	// 18: hf2p_game_client_cache_release, using `k_cache_file_version`
	filepath.print("game_variants\\%s_%03u.bin", name, 18);
	if (!create_configuration_file(filepath.get_string(), buffer, file_size))
	{
		event(_event_critical, "failed!");
	}

	delete[] buffer;
}

void __cdecl network_build_map_variant(const char* name)
{
	byte* buffer = new byte[sizeof(s_blffile_map_variant)]{};
	c_static_string<k_tag_long_string_length> filepath;

	const c_map_variant* map_variant = &game_options_get()->map_variant;
	{
		map_variant = &game_engine_globals->runtime_map_variant;
	}

	int32 file_size = multiplayer_game_hopper_pack_map_variant(buffer, sizeof(s_blffile_map_variant), map_variant);

	// 4:  halo3_cache_debug
	// 12: halo3_tag_test
	// 19: hf2p_game_client_cache_release, using `k_cache_file_version` + 1
	filepath.print("map_variants\\%s_%03u.mvar", name, 19);
	if (!create_configuration_file(filepath.get_string(), buffer, file_size))
	{
		event(_event_critical, "failed!");
	}

	delete[] buffer;
}

void __cdecl network_game_variant_file_juju(const char* file_path, bool use_variant_for_current_session)
{
	s_file_reference file;
	if (!file_reference_create_from_path(&file, file_path, false))
	{
		event(_event_warning, "networking:configuration: failed to create file reference for file '%s'", file_path);
		return;
	}

	uns32 unused_error_code = 0;
	if (!file_open(&file, FLAG(_file_open_flag_desired_access_read), &unused_error_code))
	{
		event(_event_warning, "networking:configuration: failed to open file '%s'", file_path);
		return;
	}

	uns32 size = 0;
	if (!file_get_size(&file, &size))
	{
		event(_event_warning, "networking:configuration: failed to determine file size for file '%s'", file_path);
		file_close(&file);
		return;
	}

	byte buffer[0x1000]{};
	csmemset(buffer, 0, sizeof(buffer));

	if (size > sizeof(buffer))
	{
		event(_event_warning, "networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", file_path, size, sizeof(buffer));
		file_close(&file);
		return;
	}

	if (!file_read(&file, size, false, buffer))
	{
		event(_event_warning, "networking:configuration: failed to read from file '%s'", file_path);
		file_close(&file);
		return;
	}

	s_blffile_game_variant* game_variant_file = (s_blffile_game_variant*)buffer;
	c_game_variant* game_variant = &game_variant_file->variant.game_variant;

	if (!game_engine_variant_is_valid(game_variant))
	{
		event(_event_warning, "networking:configuration: game variant in file '%s' is invalid", file_path);

		file_close(&file);
		return;
	}

	event(_event_status, "networking:configuration: CONGRATULATIONS! variant file '%s' is valid", file_path);

	if (!use_variant_for_current_session)
	{
		file_close(&file);
		return;
	}

	if (!network_squad_session_set_game_variant(game_variant))
	{
		event(_event_warning, "networking:configuration: failed to set session game variant traits, probably not in a session");

		file_close(&file);
		return;
	}

	file_close(&file);
}

void __cdecl network_packed_game_variant_file_juju(const char* file_path, bool use_variant_for_current_session)
{
	s_file_reference file;
	if (!file_reference_create_from_path(&file, file_path, false))
	{
		event(_event_warning, "networking:configuration: failed to create file reference for file '%s'", file_path);
		return;
	}

	uns32 unused_error_code = 0;
	if (!file_open(&file, FLAG(_file_open_flag_desired_access_read), &unused_error_code))
	{
		event(_event_warning, "networking:configuration: failed to open file '%s'", file_path);
		return;
	}

	uns32 size = 0;
	if (!file_get_size(&file, &size))
	{
		event(_event_warning, "networking:configuration: failed to determine file size for file '%s'", file_path);
		file_close(&file);
		return;
	}

	byte buffer[0x400]{};
	csmemset(buffer, 0, sizeof(buffer));

	if (size > sizeof(buffer))
	{
		event(_event_warning, "networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", file_path, size, sizeof(buffer));
		file_close(&file);
		return;
	}

	if (!file_read(&file, size, false, buffer))
	{
		event(_event_warning, "networking:configuration: failed to read from file '%s'", file_path);
		file_close(&file);
		return;
	}

	c_game_variant* game_variant = new c_game_variant();
	if (!multiplayer_game_hopper_unpack_game_variant(buffer, size, game_variant))
	{
		event(_event_warning, "networking:configuration: failed to unpack game variant in file '%s'", file_path);

		delete game_variant;
		file_close(&file);
		return;
	}

	if (!game_engine_variant_is_valid(game_variant))
	{
		event(_event_warning, "networking:configuration: game variant in file '%s' is invalid", file_path);

		delete game_variant;
		file_close(&file);
		return;
	}

	event(_event_status, "networking:configuration: CONGRATULATIONS! variant file '%s' is valid", file_path);

	if (!use_variant_for_current_session)
	{
		file_close(&file);
		delete game_variant;
		return;
	}

	if (!network_squad_session_set_game_variant(game_variant))
	{
		event(_event_warning, "networking:configuration: failed to set session game variant traits, probably not in a session");

		file_close(&file);
		delete game_variant;
		return;
	}

	delete game_variant;
	file_close(&file);
}

void __cdecl network_map_variant_file_juju(const char* file_path, bool use_variant_for_current_session)
{
	s_file_reference file;
	if (!file_reference_create_from_path(&file, file_path, false))
	{
		event(_event_warning, "networking:configuration: failed to create file reference for file '%s'", file_path);
		return;
	}

	uns32 unused_error_code = 0;
	if (!file_open(&file, FLAG(_file_open_flag_desired_access_read), &unused_error_code))
	{
		event(_event_warning, "networking:configuration: failed to open file '%s'", file_path);
		return;
	}

	uns32 size = 0;
	if (!file_get_size(&file, &size))
	{
		event(_event_warning, "networking:configuration: failed to determine file size for file '%s'", file_path);
		file_close(&file);
		return;
	}

	byte* buffer = new byte[0xF000]{};
	csmemset(buffer, 0, 0xF000);

	if (size > 0xF000)
	{
		event(_event_warning, "networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", file_path, size, 0xF000);
		file_close(&file);
		return;
	}

	if (!file_read(&file, size, false, buffer))
	{
		event(_event_warning, "networking:configuration: failed to read from file '%s'", file_path);
		file_close(&file);
		return;
	}

	s_blffile_map_variant* map_variant_file = (s_blffile_map_variant*)buffer;
	c_map_variant* map_variant = &map_variant_file->variant.map_variant;

	if (!map_variant->validate())
	{
		event(_event_warning, "networking:configuration: map variant in file '%s' is invalid", file_path);

		delete[] buffer;
		file_close(&file);
		return;
	}

	event(_event_status, "networking:configuration: CONGRATULATIONS! variant file '%s' is valid", file_path);

	//map_variant->print();

	if (!use_variant_for_current_session)
	{
		delete[] buffer;
		file_close(&file);
		return;
	}

	char scenario_path[256]{};
	levels_get_path(_campaign_id_none, map_variant->get_map_id(), scenario_path, sizeof(scenario_path));
	if (!scenario_path[0])
	{
		event(_event_warning, "attempting to set multiplayer map [map %d] that has bad scenario path", map_variant->get_map_id());

		delete[] buffer;
		file_close(&file);
		return;
	}

	if (!network_squad_session_set_map(_campaign_id_none, map_variant->get_map_id(), scenario_path))
	{
		event(_event_warning, "networking:configuration: failed to set session map, probably not in a session");

		delete[] buffer;
		file_close(&file);
		return;
	}

	int16 removed_variant_objects = 0;
	int16 removed_placeable_object_quotas = 0;

	for (s_variant_object_datum& object : map_variant->m_variant_objects)
	{
		if (object.variant_quota_index == NONE)
		{
			continue;
		}

		s_variant_quota& quota = map_variant->m_quotas[object.variant_quota_index];

		//removed_variant_objects++;
		//object = s_variant_object_datum();
		//ASSERT(object.variant_quota_index == NONE);
	}

	for (s_variant_quota& quota : map_variant->m_quotas)
	{
		removed_placeable_object_quotas++;
		//quota = s_variant_quota();
		//ASSERT(quota.object_definition_index == NONE);

		quota.object_definition_index = NONE;
		//quota.object_definition_index = 0x00004221;
	}

	//map_variant->m_number_of_variant_objects -= removed_variant_objects;
	//map_variant->m_number_of_placeable_object_quotas -= removed_placeable_object_quotas;

	if (!network_squad_session_set_map_variant(map_variant))
	{
		event(_event_warning, "networking:configuration: failed to set session map variant traits, probably not in a session");

		delete[] buffer;
		file_close(&file);
		return;
	}

	delete[] buffer;
	file_close(&file);
}

void __cdecl network_packed_map_variant_file_juju(const char* file_path, bool use_variant_for_current_session)
{
	s_file_reference file;
	if (!file_reference_create_from_path(&file, file_path, false))
	{
		event(_event_warning, "networking:configuration: failed to create file reference for file '%s'", file_path);
		return;
	}

	uns32 unused_error_code = 0;
	if (!file_open(&file, FLAG(_file_open_flag_desired_access_read), &unused_error_code))
	{
		event(_event_warning, "networking:configuration: failed to open file '%s'", file_path);
		return;
	}

	uns32 size = 0;
	if (!file_get_size(&file, &size))
	{
		event(_event_warning, "networking:configuration: failed to determine file size for file '%s'", file_path);
		file_close(&file);
		return;
	}

	byte* buffer = new byte[sizeof(s_blffile_map_variant)]{};
	csmemset(buffer, 0, sizeof(s_blffile_map_variant));

	if (size > sizeof(s_blffile_map_variant))
	{
		event(_event_warning, "networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", file_path, size, sizeof(s_blffile_map_variant));
		file_close(&file);
		return;
	}

	if (!file_read(&file, size, false, buffer))
	{
		event(_event_warning, "networking:configuration: failed to read from file '%s'", file_path);
		file_close(&file);
		return;
	}

	c_map_variant* map_variant = new c_map_variant();
	if (!multiplayer_game_hopper_unpack_map_variant(buffer, size, map_variant))
	{
		event(_event_warning, "networking:configuration: failed to unpack map variant in file '%s'", file_path);

		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	if (!map_variant->validate())
	{
		event(_event_warning, "networking:configuration: map variant in file '%s' is invalid", file_path);

		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	event(_event_status, "networking:configuration: CONGRATULATIONS! variant file '%s' is valid", file_path);

	//map_variant->print();

	if (!use_variant_for_current_session)
	{
		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	char scenario_path[256]{};
	levels_get_path(_campaign_id_none, map_variant->get_map_id(), scenario_path, sizeof(scenario_path));
	if (!scenario_path[0])
	{
		event(_event_warning, "attempting to set multiplayer map [map %d] that has bad scenario path", map_variant->get_map_id());

		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	if (!network_squad_session_set_map(_campaign_id_none, map_variant->get_map_id(), scenario_path))
	{
		event(_event_warning, "networking:configuration: failed to set session map, probably not in a session");

		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	int16 removed_variant_objects = 0;
	int16 removed_placeable_object_quotas = 0;

	for (s_variant_object_datum& object : map_variant->m_variant_objects)
	{
		if (object.variant_quota_index == NONE)
			continue;

		s_variant_quota& quota = map_variant->m_quotas[object.variant_quota_index];

		removed_variant_objects++;
		object = s_variant_object_datum();
		ASSERT(object.variant_quota_index);
	}

	for (s_variant_quota& quota : map_variant->m_quotas)
	{
		removed_placeable_object_quotas++;
		quota = s_variant_quota();
		ASSERT(quota.object_definition_index == NONE);
	}

	//map_variant->m_number_of_variant_objects -= removed_variant_objects;
	//map_variant->m_number_of_placeable_object_quotas -= removed_placeable_object_quotas;

	if (!network_squad_session_set_map_variant(map_variant))
	{
		event(_event_warning, "networking:configuration: failed to set session map variant traits, probably not in a session");

		delete map_variant;
		delete[] buffer;
		file_close(&file);
		return;
	}

	delete map_variant;
	delete[] buffer;
	file_close(&file);
}

void __cdecl network_verify_game_variant_file(const char* file_path)
{
	network_game_variant_file_juju(file_path, false);
}

void __cdecl network_load_and_use_game_variant_file(const char* file_path)
{
	network_game_variant_file_juju(file_path, true);
}

void __cdecl network_verify_packed_game_variant_file(const char* file_path)
{
	network_packed_game_variant_file_juju(file_path, false);
}

void __cdecl network_load_and_use_packed_game_variant_file(const char* file_path)
{
	network_packed_game_variant_file_juju(file_path, true);
}

void __cdecl network_verify_map_variant_file(const char* file_path)
{
	network_map_variant_file_juju(file_path, false);
}

void __cdecl network_load_and_use_map_variant_file(const char* file_path)
{
	network_map_variant_file_juju(file_path, true);
}

void __cdecl network_verify_packed_map_variant_file(const char* file_path)
{
	network_packed_map_variant_file_juju(file_path, false);
}

void __cdecl network_load_and_use_packed_map_variant_file(const char* file_path)
{
	network_packed_map_variant_file_juju(file_path, true);
}

#undef MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE

