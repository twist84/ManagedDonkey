#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"
#include "shell/shell.hpp"

struct c_service_client
{
	enum e_json_data_type
	{
		_json_data_type_undefined = 0,
		_json_data_type_null,
		_json_data_type_object,
		_json_data_type_array,
		_json_data_type_bool,
		_json_data_type_int,
		_json_data_type_float,
		_json_data_type_string,

		k_json_data_type_count
	};

	struct s_json_data
	{
		c_enum<e_json_data_type, long, _json_data_type_undefined, k_json_data_type_count> json_data_type;

		long object_or_array_count;
		union
		{
			void* null_value;
			void** object_value;
			s_json_data** array_value;
			bool* bool_value;
			long long* integer_value;
			float* float_value;
			wchar_t const** string_value;
		};

		dword __unknownC;
	};
	static_assert(sizeof(s_json_data) == 0x10);

	struct s_json_request
	{
		dword __unknown0;

		// success: __unknown4 == 3
		long __unknown4;

		long service_type;

		s_json_data* json_data;

		void(__cdecl* callback)(long user_val, bool success, s_json_data* json_data);
		long user_val;
		bool __unknown18;
	};
	static_assert(sizeof(s_json_request) == 0x1C);

	s_json_request m_requests[16];
	long m_current_request;
	long m_next_request;
	long m_previous_request;

	c_http_stored_buffer_downloader<358400> __downloader1CC;

	char m_contents[4096];
	long m_content_length;
};

extern bool& g_hf2p_first_run;
extern c_service_client*& g_service_client;
extern long& mainmenu_spartan_unit_index;
extern long& mainmenu_elite_unit_index;

extern void __cdecl hf2p_handle_deleted_object(long object_index);
extern void __cdecl hf2p_initialize_for_new_map();
extern void __cdecl hf2p_initialize();
extern void __cdecl hf2p_game_initialize();
extern void __cdecl hf2p_scenario_tags_load_finished();
extern void __cdecl hf2p_scenario_load();
extern void __cdecl hf2p_game_dispose();
extern void __cdecl hf2p_dispose_from_old_map();
extern void __cdecl hf2p_game_update();
extern void __cdecl hf2p_idle();
extern void __cdecl hf2p_render();
extern void __cdecl hf2p_backend_register(void* backend);
extern void __cdecl hf2p_backend_unregister(void* backend);
extern void __cdecl hf2p_backend_update();

struct s_s3d_player_armor_configuration_loadout;
struct s_s3d_player_weapon_configuration_loadout;
extern s_s3d_player_armor_configuration_loadout& get_armor_loadout();
extern s_s3d_player_weapon_configuration_loadout& get_weapon_loadout();
extern c_static_array<c_static_array<c_static_string<64>, 100>, k_armor_type_count>& get_armor_regions(e_player_model_choice player_model_choice = _player_model_choice_spartan, bool* cache_file_has_halo3_armors = nullptr);

