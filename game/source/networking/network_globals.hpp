#pragma once

struct s_network_globals
{
	bool initialized;
	bool entered;

	// halo 3: thread_id = system_get_current_thread_id()
	// halo reach: thread_index = get_current_thread_index()
	// halo online: unused
	unsigned long thread_id;
};

extern struct c_network_session_parameter_type_collection*& g_network_parameter_types;
extern struct c_network_link*& g_network_link;
extern struct c_network_message_type_collection*& g_network_message_types;
extern struct c_network_message_gateway*& g_network_message_gateway;
extern struct c_network_message_handler*& g_network_message_handler;
extern struct c_network_observer*& g_network_observer;
extern struct c_network_session*& g_network_sessions;
extern struct c_network_session_parameter_type_collection*& g_network_session_parameter_types;
extern struct c_network_session_manager*& g_network_session_manager;
extern struct s_network_globals& network_globals;

struct c_network_session_manager;
struct c_map_variant;
struct c_game_variant;
struct transport_address;
enum e_game_engine_type;

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

extern void network_test_set_map_name(char const* scenario_path);
extern void network_test_set_map_variant(c_map_variant const* map_variant);
extern void network_test_set_game_variant(char const* game_engine_name);
extern void network_test_set_session_mode(char const* session_mode_name);
extern void network_test_set_ui_game_mode(char const* ui_game_mode_name);
extern void network_test_set_advertisement_mode(char const* advertisement_mode_name);
extern void network_test_set_game_variant_parameter(char const* parameter_name, long value, long* old_value);
extern void network_test_ping();
extern void network_test_reset_objects();
extern void network_test_ping_directed(transport_address const* address);
extern void network_test_text_chat(char const* text);
extern void network_test_text_chat_directed(transport_address const* address, char const* text);

