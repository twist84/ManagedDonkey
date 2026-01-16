#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/storage/network_http_buffer_downloader.hpp"

class c_service_client
{
public:
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
		c_enum<e_json_data_type, int32, _json_data_type_undefined, k_json_data_type_count> json_data_type;

		int32 object_or_array_count;
		union
		{
			void* null_value;
			void** object_value;
			s_json_data** array_value;
			bool* bool_value;
			int64* integer_value;
			real32* float_value;
			const wchar_t** string_value;
		};

		uns32 __unknownC;
	};
	COMPILE_ASSERT(sizeof(s_json_data) == 0x10);

	struct s_json_request
	{
		uns32 __unknown0;

		// success: __unknown4 == 3
		int32 __unknown4;

		int32 service_type;

		s_json_data* json_data;

		void(__cdecl* callback)(int32 user_val, bool success, s_json_data* json_data);
		int32 user_val;
		bool __unknown18;
	};
	COMPILE_ASSERT(sizeof(s_json_request) == 0x1C);

	s_json_request m_requests[16];
	int32 m_current_request;
	int32 m_next_request;
	int32 m_previous_request;

	c_http_stored_buffer_downloader<358400> __downloader1CC;

	char m_contents[4096];
	int32 m_content_length;
};

extern int32& mainmenu_spartan_unit_index;
extern int32& mainmenu_elite_unit_index;

extern void __cdecl hf2p_handle_deleted_object(int32 object_index);
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
extern void __cdecl hf2p_bot_client_initialize();
extern void __cdecl hf2p_dedicated_server_initialize();
extern void __cdecl hf2p_client_initialize();
extern void __cdecl hf2p_security_initialize();
extern void __cdecl hf2p_editor_initialize();
extern void __cdecl hf2p_main_initialize();
extern void __cdecl hf2p_config_initialize();
extern void __cdecl hf2p_bot_client_dispose();
extern void __cdecl hf2p_dedicated_server_dispose();
extern void __cdecl hf2p_client_dispose();
extern void __cdecl hf2p_main_dispose();
extern void __cdecl hf2p_bot_client_update();
extern void __cdecl hf2p_dedicated_server_update();
extern void __cdecl hf2p_client_update();
extern void __cdecl hf2p_backend_register(void* backend);
extern void __cdecl hf2p_backend_unregister(void* backend);
extern void __cdecl hf2p_backend_update();

struct s_s3d_player_armor_configuration_loadout;
struct s_s3d_player_weapon_configuration_loadout;
extern s_s3d_player_armor_configuration_loadout& get_armor_loadout();
extern s_s3d_player_weapon_configuration_loadout& get_weapon_loadout();
extern bool load_preference(const char* name, const char* value);
extern void load_preferences_from_file_hs(const char* filename);
extern void load_customization_from_file_hs(const char* filename);

