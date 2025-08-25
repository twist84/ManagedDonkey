#pragma once

class c_network_session_manager;
class c_map_variant;
class c_game_variant;
struct transport_address;

struct s_network_globals
{
	bool initialized;
	bool entered;
	uns32 thread_id;

	// $TODO add back all the missing globals
};

extern class c_network_session_parameter_type_collection*& g_network_parameter_types;
extern class c_network_link*& g_network_link;
extern class c_network_message_type_collection*& g_network_message_types;
extern class c_network_message_gateway*& g_network_message_gateway;
extern class c_network_message_handler*& g_network_message_handler;
extern class c_network_observer*& g_network_observer;
extern class c_network_session*& g_network_sessions;
extern class c_network_session_parameter_type_collection*& g_network_session_parameter_types;
extern class c_network_session_manager*& g_network_session_manager;
extern struct s_network_globals& network_globals;

extern void __cdecl network_memory_base_dispose();
extern bool __cdecl network_memory_base_initialize(
	c_network_link** link,
	c_network_message_type_collection** message_types,
	c_network_message_gateway** message_gateway,
	c_network_message_handler** message_handler,
	c_network_observer** observer,
	c_network_session** sessions,
	c_network_session_manager** session_manager,
	c_network_session_parameter_type_collection** session_parameter_types);
extern void __cdecl network_dispose();
extern c_network_session_manager* __cdecl network_get_session_manager();
extern void __cdecl network_initialize();
extern bool __cdecl network_initialized();
extern void __cdecl network_idle();
extern void __cdecl network_receive();
extern void __cdecl network_send();
extern void __cdecl network_set_online_environment(bool online_environment);
extern void __cdecl network_shutdown_transport(void* userdata);
extern void __cdecl network_startup_transport(void* userdata);
extern void __cdecl network_update();

extern void network_test_set_map_name(const char* scenario_path);
extern void network_test_set_map_variant(const c_map_variant* map_variant);
extern void network_test_set_game_variant(const char* game_engine_name);
extern void network_test_set_session_mode(const char* session_mode_name);
extern void network_test_set_ui_game_mode(const char* ui_game_mode_name);
extern void network_test_set_advertisement_mode(const char* advertisement_mode_name);
extern void network_test_set_game_variant_parameter(const char* parameter_name, int32 value, int32* old_value);
extern void network_test_ping();
extern void network_test_reset_objects();
extern void network_test_ping_directed(const transport_address* address);
extern void network_test_text_chat(const char* text);
extern void network_test_text_chat_directed(const transport_address* address, const char* text);
extern void network_test_set_player_color(int32 profile_color_index);

