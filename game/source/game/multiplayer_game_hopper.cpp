#include "game/multiplayer_game_hopper.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game.hpp"
#include "main/levels.hpp"
#include "memory/bitstream.hpp"
#include "memory/byte_swapping.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/logic/storage/network_http_request_queue.hpp"
#include "networking/logic/storage/network_storage_files.hpp"
#include "networking/network_configuration.hpp"
#include "networking/network_time.hpp"
#include "networking/online/online.hpp"
#include "networking/online/online_lsp.hpp"
#include "networking/online/online_url.hpp"
#include "networking/tools/network_blf.hpp"

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
//HOOK_DECLARE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection);
HOOK_DECLARE(0x00549650, multiplayer_game_is_playable);

byte const multiplayer_game_is_playable_hopper_checks_patch_bytes[]
{
	// jnz     loc_549855


	// jmp     loc_549855
	// nop

	0xE9, 0xB0, 0x01, 0x00, 0x00,
	0x90
};

DATA_PATCH_DECLARE(0x005496A0, multiplayer_game_is_playable_hopper_checks, multiplayer_game_is_playable_hopper_checks_patch_bytes);

enum e_multiplayer_files
{
	_multiplayer_file_configuration = 0,
	_multiplayer_file_description,
	_multiplayer_file_game_set,
	_multiplayer_file_game_variant,
	_multiplayer_file_map_variant,

	k_multiplayer_file_count
};

struct
{
	c_hopper_configuration* current_configuration;

	byte __data4[0x4];
	multiplayer_file_load multiplayer_files[k_multiplayer_file_count];

	c_game_variant game_variant;
	c_map_variant map_variant;
	s_hopper_configuration_table configuration;
	s_game_hopper_description_table description;
	s_game_set game_set;

	byte configuration_download_buffer[sizeof(s_hopper_configuration_table)];
	byte description_download_buffer[sizeof(s_game_hopper_description_table)];
	byte game_set_download_buffer[sizeof(s_game_set)];
	byte game_variant_download_buffer[sizeof(c_game_variant)];
	byte map_variant_download_buffer[sizeof(c_map_variant)];

	byte __data35BE4[0x4];
} multiplayer_game_hopper_globals{};

e_hopper_load_status multiplayer_file_load::get_load_status()
{
	if (__unknown1)
		return _hopper_load_failed;

	if (request_cookie)
		return _hopper_load_pending;

	if (__unknown0)
		return _hopper_load_complete;

	return _hopper_load_none;
}

#define MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(multiplayer_files, url, buffer) multiplayer_game_hopper_load_retried_file_helper(multiplayer_files, url, buffer, sizeof(buffer))
void __cdecl multiplayer_game_hopper_load_retried_file_helper(e_multiplayer_files multiplayer_files, char const* url, void* buffer, long buffer_length)
{
	multiplayer_file_load* multiplayer_file = &multiplayer_game_hopper_globals.multiplayer_files[multiplayer_files];
	if (!multiplayer_file->request_cookie)
	{
		dword time = network_time_get();
		if (time >= dword(multiplayer_file->__time1C + multiplayer_file->__time20))
		{
			c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_unknown0);
			c_network_http_request_description request_description(url, _http_request_type_get, &multiplayer_file->hash);
			if (multiplayer_file->__unknown1)
			{
				multiplayer_file->request_cookie = request_queue->start_request_into_buffer(_online_lsp_service_type_title_files, &request_description, (char*)buffer, buffer_length);
			}
			else if (request_queue->has_file_changed(&request_description))
			{
				multiplayer_file->__time20 = 0;
				multiplayer_file->request_cookie = request_queue->start_request_into_buffer(_online_lsp_service_type_title_files, &request_description, (char*)buffer, buffer_length);
			}
			else
			{
				multiplayer_file->__time1C = time;
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
	return _hopper_load_complete;
	//return INVOKE(0x00545700, multiplayer_game_hopper_catalog_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_configuration].get_load_status();
}

//.text:00545710 ; e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool, bool)
e_session_game_start_error __cdecl multiplayer_game_hopper_check_required_files(bool check_hopper, bool valid_hopper_identifier)
{
	check_hopper = false;
	valid_hopper_identifier = false;

	e_session_game_start_error result = _session_game_start_error_none;
	//HOOK_INVOKE(result =, multiplayer_game_hopper_check_required_files, check_hopper, valid_hopper_identifier)
	return result;
}

//.text:005457F0 ; void __cdecl multiplayer_game_hopper_client_compute_repeated_play_adjustment_weight(e_controller_index, word, long, long, long, long, qword const*, long, long*, bool*)
//.text:00545930 ; void __cdecl multiplayer_game_hopper_client_modify_repeated_play_list(e_controller_index, word, qword const*, long)
//.text:005459D0 ; sub_5459D0
//.text:00545A70 ; sub_545A70
//.text:00545B00 ; real __cdecl multiplayer_game_hopper_compute_gather_chance(long)
//.text:00545B10 ; long __cdecl multiplayer_game_hopper_compute_match_quality(c_network_session_membership const*)
//.text:00545C20 ; long __cdecl multiplayer_game_hopper_compute_skill_match_delta(long)
//.text:00545C30 ; sub_545C30
//.text:00545E80 ; bool __cdecl multiplayer_game_hopper_decode(c_bitstream*, s_hopper_configuration_table*)
//.text:00546820 ; bool __cdecl multiplayer_game_hopper_description_decode(c_bitstream*, s_game_hopper_description_table*)
//.text:005468A0 ; void __cdecl multiplayer_game_hopper_description_encode(c_bitstream*, s_game_hopper_description_table const*)

void __cdecl multiplayer_game_hopper_dispose()
{
	//INVOKE(0x005469E0, multiplayer_game_hopper_dispose);

	csmemset(&multiplayer_game_hopper_globals, 0, sizeof(multiplayer_game_hopper_globals));
}

//.text:005469F0 ; void __cdecl multiplayer_game_hopper_encode(c_bitstream*, s_hopper_configuration_table const*)

e_hopper_load_status __cdecl multiplayer_game_hopper_game_set_load_status()
{
	return _hopper_load_complete;
	//return INVOKE(0x00548210, multiplayer_game_hopper_game_set_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set].get_load_status();
}

e_hopper_load_status __cdecl multiplayer_game_hopper_game_variant_load_status()
{
	return _hopper_load_complete;
	//return INVOKE(0x00548220, multiplayer_game_hopper_game_variant_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_variant].get_load_status();
}

s_game_hopper_custom_category const* __cdecl multiplayer_game_hopper_get_category_from_index(long category_index)
{
	//return INVOKE(0x00548230, multiplayer_game_hopper_get_category_from_index, category_index);

	if (category_index < multiplayer_game_hopper_globals.configuration.hopper_configuration_count)
		return &multiplayer_game_hopper_globals.configuration.hopper_category[category_index];

	return NULL;
}

c_game_variant const* __cdecl multiplayer_game_hopper_get_current_game_variant()
{
	//return INVOKE(0x00548240, multiplayer_game_hopper_get_current_game_variant);

	ASSERT(multiplayer_game_hopper_game_variant_load_status() == _hopper_load_complete);

	return &multiplayer_game_hopper_globals.game_variant;
}

word __cdecl multiplayer_game_hopper_get_current_hopper_identifier()
{
	//return INVOKE(0x00548250, multiplayer_game_hopper_get_current_hopper_identifier);

	if (multiplayer_game_hopper_globals.current_configuration)
		return multiplayer_game_hopper_globals.current_configuration->hopper_identifier;

	return NONE;
}

c_map_variant const* __cdecl multiplayer_game_hopper_get_current_map_variant()
{
	//return INVOKE(0x00548260, multiplayer_game_hopper_get_current_map_variant);

	ASSERT(multiplayer_game_hopper_map_variant_load_status() == _hopper_load_complete);

	return &multiplayer_game_hopper_globals.map_variant;
}

utf8 const* __cdecl multiplayer_game_hopper_get_description(word hopper_identifier)
{
	//INVOKE(0x00548270, multiplayer_game_hopper_get_description, hopper_identifier);

	for (long description_index = 0; description_index < multiplayer_game_hopper_globals.description.hopper_description_count; description_index++)
	{
		if (multiplayer_game_hopper_globals.description.hopper_descriptions[description_index].hopper_identifier == hopper_identifier)
			return multiplayer_game_hopper_globals.description.hopper_descriptions[description_index].hopper_description.get_string();
	}

	return NULL;
}

word __cdecl multiplayer_game_hopper_get_hopper_identifier(long hopper_index)
{
	//return INVOKE(0x00548280, multiplayer_game_hopper_get_hopper_identifier, hopper_index);

	ASSERT(hopper_index >= 0 && hopper_index < k_hopper_maximum_hopper_count);

	if (hopper_index < multiplayer_game_hopper_globals.configuration.hopper_category_count)
	{
		//word hopper_identifier = multiplayer_game_hopper_globals.configuration.hopper_configurations[hopper_index].get_hopper_identifier();
		word hopper_identifier = multiplayer_game_hopper_globals.configuration.hopper_configurations[hopper_index].hopper_identifier;
		if (hopper_identifier != NONE)
			return hopper_identifier;
	}

	return NONE;
}

//.text:00548290 ; sub_548290
//.text:005482A0 ; sub_5482A0
//.text:005483A0 ; sub_5483A0

void __cdecl multiplayer_game_hopper_initialize()
{
	//INVOKE(0x005483B0, multiplayer_game_hopper_initialize);

	csmemset(&multiplayer_game_hopper_globals, 0, sizeof(multiplayer_game_hopper_globals));

	// for now give us fake data
	s_game_set game_set{};
	initialize_fake_hopper(&multiplayer_game_hopper_globals.configuration, &multiplayer_game_hopper_globals.description, &multiplayer_game_hopper_globals.game_set, &game_set);
}

bool __cdecl multiplayer_game_hopper_is_hopper_visible(word hopper_identifier, c_network_session_membership const* session_membership)
{
	//return INVOKE(0x005483C0, multiplayer_game_hopper_is_hopper_visible, hopper_identifier, session_membership);

	//if (c_hopper_configuration* hopper_configuration = multiplayer_game_hoppers_get_hopper_configuration(hopper_identifier))
	//{
	//	multiplayer_hopper_check check{};
	//	multiplayer_game_hopper_get_players_status(hopper_configuration, session_membership, &check);
	//
	//	if (hopper_configuration->hide_hopper_from_experience_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_experience_requirements)
	//		{
	//
	//		}
	//	}
	//
	//	if (hopper_configuration->hide_hopper_from_access_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_access_requirements)
	//		{
	//
	//		}
	//	}
	//
	//	if (hopper_configuration->hide_hopper_from_live_account_access_restricted_players)
	//	{
	//		if (hopper_configuration->require_all_party_members_meet_live_account_access_requirements)
	//		{
	//
	//		}
	//	}
	//}

	return true;
}

//.text:005483D0 ; sub_5483D0

e_hopper_load_status __cdecl multiplayer_game_hopper_map_variant_load_status()
{
	return _hopper_load_complete;
	//return INVOKE(0x005483E0, multiplayer_game_hopper_map_variant_load_status);

	return multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_map_variant].get_load_status();
}

//.text:005483F0 ; long __cdecl multiplayer_game_hopper_pack_game_set(void*, long, s_game_set const*)

long __cdecl multiplayer_game_hopper_pack_game_variant(void* buffer, long buffer_size, c_game_variant const* game_variant)
{
	return INVOKE(0x00548610, multiplayer_game_hopper_pack_game_variant, buffer, buffer_size, game_variant);
}

//.text:00548830 ; long __cdecl multiplayer_game_hopper_pack_hopper_description(void*, long, s_game_hopper_description_table const*)
//.text:00548A50 ; long __cdecl multiplayer_game_hopper_pack_hopper_file(void*, long, s_hopper_configuration_table const*)

long __cdecl multiplayer_game_hopper_pack_map_variant(void* buffer, long buffer_size, c_map_variant const* map_variant)
{
	return INVOKE(0x00548C70, multiplayer_game_hopper_pack_map_variant, buffer, buffer_size, map_variant);
}

void __cdecl multiplayer_game_hopper_request_game_variant(word hopper_identifier, char const* variant_name, s_network_http_request_hash const* hash)
{
	//INVOKE(0x00548E90, multiplayer_game_hopper_request_game_variant, hopper_identifier, variant_name, hash);

	c_url_string url;
	multiplayer_file_load& multiplayer_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_variant];
	if (multiplayer_file.request_cookie)
	{
		c_network_http_request_queue::get(_network_http_request_queue_type_unknown0)->cancel_request(multiplayer_file.request_cookie);
		multiplayer_file.request_cookie = 0;
	}
	multiplayer_file.__time1C = 0;
	multiplayer_file.__time20 = 0;
	online_url_make_matchmaking_game_variant(&url, hopper_identifier, variant_name);
	MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_game_variant, url.get_string(), multiplayer_game_hopper_globals.game_variant_download_buffer);
	multiplayer_file.__unknown1 = false;
}

void __cdecl multiplayer_game_hopper_request_map_variant(word hopper_identifier, char const* variant_name, s_network_http_request_hash const* hash)
{
	//INVOKE(0x00548EA0, multiplayer_game_hopper_request_map_variant, hopper_identifier, variant_name, hash);

	c_url_string url;
	multiplayer_file_load& multiplayer_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_map_variant];
	if (multiplayer_file.request_cookie)
	{
		c_network_http_request_queue::get(_network_http_request_queue_type_unknown0)->cancel_request(multiplayer_file.request_cookie);
		multiplayer_file.request_cookie = 0;
	}
	multiplayer_file.__time1C = 0;
	multiplayer_file.__time20 = 0;
	online_url_make_matchmaking_game_variant(&url, hopper_identifier, variant_name);
	MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_map_variant, url.get_string(), multiplayer_game_hopper_globals.game_variant_download_buffer);
	multiplayer_file.__unknown1 = false;
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
		online_url_make_matchmaking_gameset(&url, multiplayer_game_hopper_globals.current_configuration->hopper_identifier);
		MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE(_multiplayer_file_game_set, url.get_string(), multiplayer_game_hopper_globals.game_set_download_buffer);
	}
}

bool __cdecl multiplayer_game_hopper_set_active_hopper_and_request_game_set(word hopper_identifier)
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
			ASSERT2("unreachable");
		}
	}

	if (!hopper_configuration ||
		hopper_configuration == multiplayer_game_hopper_globals.current_configuration &&
		multiplayer_game_hopper_game_set_load_status())
	{
		generate_event(_event_level_warning, "multiplayer_game_hopper: tried to set the current hopper to something invalid- %04X",
			hopper_identifier);
	}
	else
	{
		c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_unknown0);

		multiplayer_file_load& game_set_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set];
		if (game_set_file.request_cookie)
		{
			request_queue->cancel_request(game_set_file.request_cookie);
			game_set_file.request_cookie = 0;
		}

		game_set_file.__time1C = 0;
		game_set_file.__time20 = 0;

		result = true;
	}

	multiplayer_game_hopper_globals.current_configuration = hopper_configuration;

	return result;
}

//.text:00548EC0 ; sub_548EC0

bool __cdecl multiplayer_game_hopper_unpack_game_set(void const* buffer, long bytes_read, s_game_set* game_set)
{
	return INVOKE(0x00548ED0, multiplayer_game_hopper_unpack_game_set, buffer, bytes_read, game_set);
}

bool packed_game_variant_is_mcc(void const* buffer_, long bytes_read)
{
	byte const* buffer = static_cast<byte const*>(buffer_);
	void const* buffer_end = buffer + bytes_read;

	s_blf_header const* chunk_header = reinterpret_cast<s_blf_header const*>(buffer);
	while (chunk_header->chunk_type != 'msf_')
	{
		buffer += bswap_dword(chunk_header->chunk_size);
		chunk_header = reinterpret_cast<s_blf_header const*>(buffer);

		if (buffer >= buffer_end)
			return false;
	}

	return chunk_header->chunk_type == 'msf_';
}

bool __cdecl multiplayer_game_hopper_unpack_game_variant(void const* buffer, long bytes_read, c_game_variant* game_variant)
{
	//INVOKE(0x00549050, multiplayer_game_hopper_unpack_game_variant, buffer, bytes_read, game_variant);

	if (packed_game_variant_is_mcc(buffer, bytes_read))
	{
		void const* buffer_end = static_cast<byte const*>(buffer) + bytes_read;

		s_blf_header const* chunk_header = static_cast<s_blf_header const*>(buffer);
		csmemset(game_variant, 0, sizeof(c_game_variant));

		ASSERT(0x9D == sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));
		ASSERT(bytes_read > sizeof(s_blf_chunk_start_of_file) + sizeof(s_blf_chunk_author) + sizeof(s_blf_header) + sizeof(s_blf_chunk_end_of_file));

		while (buffer < buffer_end && chunk_header->chunk_type != 'ravg')
		{
			buffer = static_cast<byte const*>(buffer) + bswap_dword(chunk_header->chunk_size);
			chunk_header = static_cast<s_blf_header const*>(buffer);
		}

		if (buffer >= buffer_end)
			return false;

		long chunk_size = bswap_dword(chunk_header->chunk_size) - sizeof(s_blf_header);
		byte* chunk_data = const_cast<byte*>(static_cast<byte const*>(buffer) + sizeof(s_blf_header));

		c_bitstream packet(chunk_data, chunk_size);
		packet.begin_reading();
		bool decode_succeeded = game_variant->decode_from_mcc(&packet);
		packet.finish_reading();

		bool result = decode_succeeded;
		if (decode_succeeded)
		{
			buffer = static_cast<byte const*>(buffer) + bswap_dword(chunk_header->chunk_size);
			chunk_header = static_cast<s_blf_header const*>(buffer);

			if (buffer >= buffer_end)
			{
				return false;
			}
			else
			{
				// is end of file
				while (chunk_header->chunk_type != 'foe_')
				{
					buffer = static_cast<byte const*>(buffer) + bswap_dword(chunk_header->chunk_size);
					chunk_header = static_cast<s_blf_header const*>(buffer);

					if (buffer >= buffer_end)
						return false;
				}

				result = false;
				if (buffer < buffer_end)
					return true;
			}
		}

		return result;
	}

	return INVOKE(0x00549050, multiplayer_game_hopper_unpack_game_variant, buffer, bytes_read, game_variant);
}

bool __cdecl multiplayer_game_hopper_unpack_hopper_description(void const* buffer, long bytes_read, s_game_hopper_description_table* description)
{
	return INVOKE(0x005491D0, multiplayer_game_hopper_unpack_hopper_description, buffer, bytes_read, description);
}

bool __cdecl multiplayer_game_hopper_unpack_hopper_file(void const* buffer, long bytes_read, s_hopper_configuration_table* configuration)
{
	return INVOKE(0x00549350, multiplayer_game_hopper_unpack_hopper_file, buffer, bytes_read, configuration);
}

bool __cdecl multiplayer_game_hopper_unpack_map_variant(void const* buffer, long bytes_read, c_map_variant* map_variant)
{
	return INVOKE(0x005494D0, multiplayer_game_hopper_unpack_map_variant, buffer, bytes_read, map_variant);
}

void __cdecl multiplayer_game_hopper_update()
{
	//INVOKE(0x00549610, multiplayer_game_hopper_update);

	c_network_http_request_queue* request_queue = c_network_http_request_queue::get(_network_http_request_queue_type_unknown0);
	for (long multiplayer_game_file = 0; multiplayer_game_file < k_multiplayer_file_count; multiplayer_game_file++)
	{
		multiplayer_file_load* multiplayer_file = &multiplayer_game_hopper_globals.multiplayer_files[multiplayer_game_file];
		if (!multiplayer_file->request_cookie)
			continue;

		bool something = false;

		long bytes_read = 0;
		long request_result = request_queue->is_fill_buffer_complete(multiplayer_file->request_cookie, &bytes_read, &multiplayer_file->hash, NULL);
		if (request_result == 1)
		{
			switch (multiplayer_game_file)
			{
			case _multiplayer_file_configuration:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.configuration_download_buffer));

				if (multiplayer_game_hopper_unpack_hopper_file(multiplayer_game_hopper_globals.configuration_download_buffer, bytes_read, &multiplayer_game_hopper_globals.configuration))
				{
					multiplayer_file_load& game_set_file = multiplayer_game_hopper_globals.multiplayer_files[_multiplayer_file_game_set];
					if (game_set_file.request_cookie)
					{
						request_queue->cancel_request(game_set_file.request_cookie);
						game_set_file.request_cookie = 0;
						game_set_file.__time1C = 0;
						game_set_file.__time20 = 0;
					}
				}
				else
				{
					something = true;
				}
			}
			break;
			case _multiplayer_file_description:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.description_download_buffer));

				if (!multiplayer_game_hopper_unpack_hopper_description(multiplayer_game_hopper_globals.description_download_buffer, bytes_read, &multiplayer_game_hopper_globals.description))
				{
					something = true;
				}
			}
			break;
			case _multiplayer_file_game_set:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.game_set_download_buffer));

				if (!multiplayer_game_hopper_unpack_game_set(multiplayer_game_hopper_globals.game_set_download_buffer, bytes_read, &multiplayer_game_hopper_globals.game_set))
				{
					something = true;
				}
			}
			break;
			case _multiplayer_file_game_variant:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.game_variant_download_buffer));

				if (!multiplayer_game_hopper_unpack_game_variant(multiplayer_game_hopper_globals.game_variant_download_buffer, bytes_read, &multiplayer_game_hopper_globals.game_variant))
				{
					something = true;
				}
			}
			break;
			case _multiplayer_file_map_variant:
			{
				ASSERT(bytes_read < sizeof(multiplayer_game_hopper_globals.map_variant_download_buffer));

				if (!multiplayer_game_hopper_unpack_map_variant(multiplayer_game_hopper_globals.map_variant_download_buffer, bytes_read, &multiplayer_game_hopper_globals.map_variant))
				{
					something = true;
				}
			}
			break;
			default:
				ASSERT2("unreachable");
				break;
			}

			if (!something)
			{
				multiplayer_file->request_cookie = 0;
				multiplayer_file->__unknown1 = true;
				multiplayer_file->__time1C = network_time_get();
				multiplayer_file->__time20 = g_network_configuration.__unknown4;
			}
		}
		else if (request_result == 2)
		{
			something = true;
		}

		if (something)
		{
			multiplayer_file->request_cookie = 0;
			multiplayer_file->__unknown1 = true;
			multiplayer_file->__time1C = network_time_get();
			multiplayer_file->__time20 = network_configuration_calculate_next_retry_interval(multiplayer_file->__time20, g_network_configuration.__unknown8);

			generate_event(_event_level_message, "networking:configuration: could not retrieve multiplayer game file [%d]",
				multiplayer_game_file);
		}
	}
}

//.text: ; c_hopper_configuration * __cdecl multiplayer_game_hoppers_get_current_hopper_configuration()
c_hopper_configuration const* __cdecl multiplayer_game_hoppers_get_current_hopper_configuration()
{
	//INVOKE(0x00549620, multiplayer_game_hoppers_get_current_hopper_configuration);

	return multiplayer_game_hopper_globals.current_configuration;
}

c_hopper_configuration* __cdecl multiplayer_game_hoppers_get_hopper_configuration(word hopper_identifier)
{
	//return INVOKE(0x00549630, multiplayer_game_hoppers_get_hopper_configuration, hopper_identifier);

	//if (multiplayer_game_hopper_globals.current_configuration && multiplayer_game_hopper_globals.current_configuration->get_hopper_identifier() == hopper_identifier)
	if (multiplayer_game_hopper_globals.current_configuration && multiplayer_game_hopper_globals.current_configuration->hopper_identifier == hopper_identifier)
		return multiplayer_game_hopper_globals.current_configuration;

	for (long category_index = 0; category_index < multiplayer_game_hopper_globals.configuration.hopper_category_count; category_index++)
	{
		//if (multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index].get_hopper_identifier() == hopper_identifier)
		if (multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index].hopper_identifier == hopper_identifier)
		{
			return &multiplayer_game_hopper_globals.configuration.hopper_configurations[category_index];
		}
	}

	return NULL;
}

bool __cdecl multiplayer_game_hoppers_pick_random_game_collection(long player_count, long valid_map_mask, s_game_hopper_picked_game_collection* game_collection_out)
{
	//return INVOKE(0x00549640, multiplayer_game_hoppers_pick_random_game_collection, player_count, valid_map_mask, game_collection_out);

	return false;
}

//.text:00549650 ; enum e_session_game_start_error __cdecl multiplayer_game_is_playable(word, bool, bool, c_network_session_membership const*, word*);
e_session_game_start_error __cdecl multiplayer_game_is_playable(word hopper_identifier, bool is_matchmaking, bool check_hopper, c_network_session_membership const* session_membership, word* out_player_error_mask)
{
	is_matchmaking = false;
	check_hopper = false;

	e_session_game_start_error result = _session_game_start_error_none;
	HOOK_INVOKE(result =, multiplayer_game_is_playable, hopper_identifier, is_matchmaking, check_hopper, session_membership, out_player_error_mask);
	return result;
}

//.text:00549870 ; bool __cdecl multiplayer_game_set_decode(c_bitstream*, s_game_set*)
//.text:00549970 ; void __cdecl multiplayer_game_set_encode(c_bitstream*, s_game_set const*)

char const* __cdecl multiplayer_game_start_error_to_string(e_session_game_start_error error)
{
	return INVOKE(0x00549B70, multiplayer_game_start_error_to_string, error);
}

void __cdecl network_build_game_variant(char const* filename)
{
	byte* buffer = new byte[0x600]{};
	c_static_string<k_tag_long_string_length> filepath;

	c_game_variant const* game_variant = &game_options_get()->game_variant;
	long file_size = multiplayer_game_hopper_pack_game_variant(buffer, 0x600, game_variant);

	// 5:  halo3_cache_debug
	// 10: halo3_tag_test
	// 18: hf2p_game_client_cache_release, using `k_cache_file_version`
	filepath.print("game_variants\\%s_%03u.bin", filename, 18);
	if (!create_configuration_file(filepath.get_string(), buffer, file_size))
	{
		generate_event(_event_level_critical, "failed!");
	}

	delete[] buffer;
}

void __cdecl network_build_map_variant(char const* filename)
{
	byte* buffer = new byte[sizeof(s_blffile_map_variant)]{};
	c_static_string<k_tag_long_string_length> filepath;

	c_map_variant const* map_variant = &game_options_get()->map_variant;
	{
		TLS_DATA_GET_VALUE_REFERENCE(game_engine_globals); // use runtime map variant
		map_variant = &game_engine_globals->map_variant;
	}

	long file_size = multiplayer_game_hopper_pack_map_variant(buffer, sizeof(s_blffile_map_variant), map_variant);

	// 4:  halo3_cache_debug
	// 12: halo3_tag_test
	// 19: hf2p_game_client_cache_release, using `k_cache_file_version` + 1
	filepath.print("map_variants\\%s_%03u.mvar", filename, 19);
	if (!create_configuration_file(filepath.get_string(), buffer, file_size))
	{
		generate_event(_event_level_critical, "failed!");
	}

	delete[] buffer;
}

void __cdecl network_game_variant_file_juju(char const* filename, bool load_and_use)
{
	s_file_reference info;
	if (!file_reference_create_from_path(&info, filename, 0))
	{
		c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
		return;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
	{
		c_console::write_line("networking:configuration: failed to open file '%s'", filename);
		return;
	}

	dword size = 0;
	if (!file_get_size(&info, &size))
	{
		c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
		file_close(&info);
		return;
	}

	byte buffer[0x1000]{};
	csmemset(buffer, 0, sizeof(buffer));

	if (size > sizeof(buffer))
	{
		c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, sizeof(buffer));
		file_close(&info);
		return;
	}

	if (!file_read(&info, size, false, buffer))
	{
		c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
		file_close(&info);
		return;
	}

	s_blffile_game_variant* game_variant_file = reinterpret_cast<s_blffile_game_variant*>(buffer);
	c_game_variant* game_variant = &game_variant_file->game_variant_chunk.game_variant;

	if (!game_engine_variant_is_valid(game_variant))
	{
		c_console::write_line("networking:configuration: game variant in file '%s' is invalid", filename);

		file_close(&info);
		return;
	}

	c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

	if (!load_and_use)
	{
		file_close(&info);
		return;
	}

	if (!network_squad_session_set_game_variant(game_variant))
	{
		c_console::write_line("networking:configuration: failed to set session game variant traits, probably not in a session");

		file_close(&info);
		return;
	}

	file_close(&info);
}

void __cdecl network_packed_game_variant_file_juju(char const* filename, bool load_and_use)
{
	s_file_reference info;
	if (!file_reference_create_from_path(&info, filename, 0))
	{
		c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
		return;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
	{
		c_console::write_line("networking:configuration: failed to open file '%s'", filename);
		return;
	}

	dword size = 0;
	if (!file_get_size(&info, &size))
	{
		c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
		file_close(&info);
		return;
	}

	byte buffer[0x400]{};
	csmemset(buffer, 0, sizeof(buffer));

	if (size > sizeof(buffer))
	{
		c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, sizeof(buffer));
		file_close(&info);
		return;
	}

	if (!file_read(&info, size, false, buffer))
	{
		c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
		file_close(&info);
		return;
	}

	c_game_variant* game_variant = new c_game_variant();
	if (!multiplayer_game_hopper_unpack_game_variant(buffer, size, game_variant))
	{
		c_console::write_line("networking:configuration: failed to unpack game variant in file '%s'", filename);

		delete game_variant;
		file_close(&info);
		return;
	}

	if (!game_engine_variant_is_valid(game_variant))
	{
		c_console::write_line("networking:configuration: game variant in file '%s' is invalid", filename);

		delete game_variant;
		file_close(&info);
		return;
	}

	c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

	if (!load_and_use)
	{
		file_close(&info);
		delete game_variant;
		return;
	}

	if (!network_squad_session_set_game_variant(game_variant))
	{
		c_console::write_line("networking:configuration: failed to set session game variant traits, probably not in a session");

		file_close(&info);
		delete game_variant;
		return;
	}

	delete game_variant;
	file_close(&info);
}

void __cdecl network_map_variant_file_juju(char const* filename, bool load_and_use)
{
	s_file_reference info;
	if (!file_reference_create_from_path(&info, filename, 0))
	{
		c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
		return;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
	{
		c_console::write_line("networking:configuration: failed to open file '%s'", filename);
		return;
	}

	dword size = 0;
	if (!file_get_size(&info, &size))
	{
		c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
		file_close(&info);
		return;
	}

	byte* buffer = new byte[0xF000]{};
	csmemset(buffer, 0, 0xF000);

	if (size > 0xF000)
	{
		c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, 0xF000);
		file_close(&info);
		return;
	}

	if (!file_read(&info, size, false, buffer))
	{
		c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
		file_close(&info);
		return;
	}

	s_blffile_map_variant* map_variant_file = reinterpret_cast<s_blffile_map_variant*>(buffer);
	c_map_variant* map_variant = &map_variant_file->map_variant_chunk.map_variant;

	if (!map_variant->validate())
	{
		c_console::write_line("networking:configuration: map variant in file '%s' is invalid", filename);

		delete[] buffer;
		file_close(&info);
		return;
	}

	c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

	//map_variant->print();

	if (!load_and_use)
	{
		delete[] buffer;
		file_close(&info);
		return;
	}

	char scenario_path[256]{};
	levels_get_path(NONE, map_variant->get_map_id(), scenario_path, sizeof(scenario_path));
	if (!scenario_path[0])
	{
		c_console::write_line("attempting to set multiplayer map [map %d] that has bad scenario path", map_variant->get_map_id());

		delete[] buffer;
		file_close(&info);
		return;
	}

	if (!network_squad_session_set_map(NONE, map_variant->get_map_id(), scenario_path))
	{
		c_console::write_line("networking:configuration: failed to set session map, probably not in a session");

		delete[] buffer;
		file_close(&info);
		return;
	}

	short removed_variant_objects = 0;
	short removed_placeable_object_quotas = 0;

	for (s_variant_object_datum& object : map_variant->m_variant_objects)
	{
		if (object.variant_quota_index == NONE)
			continue;

		s_variant_quota& quota = map_variant->m_quotas[object.variant_quota_index];

		long object_definition_index = quota.object_definition_index;
		if (quota.object_definition_index < g_tag_total_count_pre_external_files)
			continue;

		//removed_variant_objects++;
		//object = s_variant_object_datum();
		//ASSERT(object.variant_quota_index == NONE);
	}

	for (s_variant_quota& quota : map_variant->m_quotas)
	{
		if (quota.object_definition_index < g_tag_total_count_pre_external_files)
			continue;

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
		c_console::write_line("networking:configuration: failed to set session map variant traits, probably not in a session");

		delete[] buffer;
		file_close(&info);
		return;
	}

	delete[] buffer;
	file_close(&info);
}

void __cdecl network_packed_map_variant_file_juju(char const* filename, bool load_and_use)
{
	s_file_reference info;
	if (!file_reference_create_from_path(&info, filename, 0))
	{
		c_console::write_line("networking:configuration: failed to create file reference for file '%s'", filename);
		return;
	}

	dword error = 0;
	if (!file_open(&info, FLAG(_file_open_flag_desired_access_read), &error))
	{
		c_console::write_line("networking:configuration: failed to open file '%s'", filename);
		return;
	}

	dword size = 0;
	if (!file_get_size(&info, &size))
	{
		c_console::write_line("networking:configuration: failed to determine file size for file '%s'", filename);
		file_close(&info);
		return;
	}

	byte* buffer = new byte[sizeof(s_blffile_map_variant)]{};
	csmemset(buffer, 0, sizeof(s_blffile_map_variant));

	if (size > sizeof(s_blffile_map_variant))
	{
		c_console::write_line("networking:configuration: invalid file size for '%s' (%ld bytes/%ld max)", filename, size, sizeof(s_blffile_map_variant));
		file_close(&info);
		return;
	}

	if (!file_read(&info, size, false, buffer))
	{
		c_console::write_line("networking:configuration: failed to read from file '%s'", filename);
		file_close(&info);
		return;
	}

	c_map_variant* map_variant = new c_map_variant();
	if (!multiplayer_game_hopper_unpack_map_variant(buffer, size, map_variant))
	{
		c_console::write_line("networking:configuration: failed to unpack map variant in file '%s'", filename);

		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	if (!map_variant->validate())
	{
		c_console::write_line("networking:configuration: map variant in file '%s' is invalid", filename);

		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	c_console::write_line("networking:configuration: CONGRATULATIONS! variant file '%s' is valid", filename);

	//map_variant->print();

	if (!load_and_use)
	{
		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	char scenario_path[256]{};
	levels_get_path(NONE, map_variant->get_map_id(), scenario_path, sizeof(scenario_path));
	if (!scenario_path[0])
	{
		c_console::write_line("attempting to set multiplayer map [map %d] that has bad scenario path", map_variant->get_map_id());

		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	if (!network_squad_session_set_map(NONE, map_variant->get_map_id(), scenario_path))
	{
		c_console::write_line("networking:configuration: failed to set session map, probably not in a session");

		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	short removed_variant_objects = 0;
	short removed_placeable_object_quotas = 0;

	for (s_variant_object_datum& object : map_variant->m_variant_objects)
	{
		if (object.variant_quota_index == NONE)
			continue;

		s_variant_quota& quota = map_variant->m_quotas[object.variant_quota_index];

		long object_definition_index = quota.object_definition_index;
		if (quota.object_definition_index < g_tag_total_count_pre_external_files)
			continue;

		removed_variant_objects++;
		object = s_variant_object_datum();
		ASSERT(object.variant_quota_index);
	}

	for (s_variant_quota& quota : map_variant->m_quotas)
	{
		if (quota.object_definition_index < g_tag_total_count_pre_external_files)
			continue;

		removed_placeable_object_quotas++;
		quota = s_variant_quota();
		ASSERT(quota.object_definition_index == NONE);
	}

	//map_variant->m_number_of_variant_objects -= removed_variant_objects;
	//map_variant->m_number_of_placeable_object_quotas -= removed_placeable_object_quotas;

	if (!network_squad_session_set_map_variant(map_variant))
	{
		c_console::write_line("networking:configuration: failed to set session map variant traits, probably not in a session");

		delete map_variant;
		delete[] buffer;
		file_close(&info);
		return;
	}

	delete map_variant;
	delete[] buffer;
	file_close(&info);
}

void __cdecl network_verify_game_variant_file(char const* filename)
{
	network_game_variant_file_juju(filename, false);
}

void __cdecl network_load_and_use_game_variant_file(char const* filename)
{
	network_game_variant_file_juju(filename, true);
}

void __cdecl network_verify_packed_game_variant_file(char const* filename)
{
	network_packed_game_variant_file_juju(filename, false);
}

void __cdecl network_load_and_use_packed_game_variant_file(char const* filename)
{
	network_packed_game_variant_file_juju(filename, true);
}

void __cdecl network_verify_map_variant_file(char const* filename)
{
	network_map_variant_file_juju(filename, false);
}

void __cdecl network_load_and_use_map_variant_file(char const* filename)
{
	network_map_variant_file_juju(filename, true);
}

void __cdecl network_verify_packed_map_variant_file(char const* filename)
{
	network_packed_map_variant_file_juju(filename, false);
}

void __cdecl network_load_and_use_packed_map_variant_file(char const* filename)
{
	network_packed_map_variant_file_juju(filename, true);
}

#undef MULTIPLAYER_GAME_HOPPER_LOAD_RETRIED_FILE

