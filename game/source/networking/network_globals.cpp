#include "networking/network_globals.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_console.hpp"
#include "game/game_options.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "main/console.hpp"
#include "main/levels.hpp"
#include "memory/module.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "networking/messages/network_messages_out_of_band.hpp"
#include "networking/messages/network_messages_text_chat.hpp"
#include "networking/network_time.hpp"
#include "networking/network_memory.hpp"
#include "networking/session/network_session.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "tag_files/tag_groups.hpp"

#include <stdlib.h>

REFERENCE_DECLARE(0x0224A490, c_network_session_parameter_type_collection*, g_network_parameter_types);
REFERENCE_DECLARE(0x0224A494, c_network_link*, g_network_link);
REFERENCE_DECLARE(0x0224A498, c_network_message_type_collection*, g_network_message_types);
REFERENCE_DECLARE(0x0224A49C, c_network_message_gateway*, g_network_message_gateway);
REFERENCE_DECLARE(0x0224A4A0, c_network_message_handler*, g_network_message_handler);
REFERENCE_DECLARE(0x0224A4A4, c_network_observer*, g_network_observer);
REFERENCE_DECLARE(0x0224A4A8, c_network_session*, g_network_sessions);
REFERENCE_DECLARE(0x0224A4AC, c_network_session_parameter_type_collection*, g_network_session_parameter_types);
REFERENCE_DECLARE(0x0224A4B0, c_network_session_manager*, g_network_session_manager);
REFERENCE_DECLARE(0x0224A4B4, s_network_globals, network_globals);

c_network_message_type_collection custom_message_types_override = {};

#define UI_WAIT(_time, _set_value, _get_value, _value) \
_set_value(_value);                                    \
do                                                     \
{                                                      \
    user_interface_update(_time);                      \
    network_update();                                  \
} while (_get_value() != _value);

void __cdecl network_dispose()
{
	INVOKE(0x0049E050, network_dispose);
}

c_network_session_manager* __cdecl network_get_session_manager()
{
	return INVOKE(0x0049E1A0, network_get_session_manager);
}

void __cdecl network_initialize()
{
	INVOKE(0x0049E1B0, network_initialize);
}

bool __cdecl network_memory_base_initialize(
	c_network_link** link,
	c_network_message_type_collection** message_types,
	c_network_message_gateway** message_gateway,
	c_network_message_handler** message_handler,
	c_network_observer** observer,
	c_network_session** sessions,
	c_network_session_manager** session_manager,
	c_network_session_parameter_type_collection** session_parameter_types)
{
	ASSERT(link);
	ASSERT(message_types);
	ASSERT(message_gateway);
	ASSERT(message_handler);
	ASSERT(observer);
	ASSERT(sessions);
	ASSERT(session_parameter_types);

	bool result = INVOKE(0x004623F0, network_memory_base_initialize, link, message_types, message_gateway, message_handler, observer, sessions, session_manager, session_parameter_types);

	*message_types = &custom_message_types_override;

	return result;
}
HOOK_DECLARE_CALL(0x0049E200, network_memory_base_initialize);

void __cdecl network_message_types_register_test(c_network_message_type_collection* message_collection)
{
	ASSERT(message_collection);

	network_message_types_register_text_chat(message_collection);

	INVOKE(0x004E1000, network_message_types_register_test, message_collection);
}
HOOK_DECLARE_CALL(0x0049E289, network_message_types_register_test);

bool __cdecl network_initialized()
{
	return INVOKE(0x0049E5A0, network_initialized);
}

void __cdecl network_receive()
{
	INVOKE(0x0049E600, network_receive);
}

void __cdecl network_send()
{
	INVOKE(0x0049E640, network_send);
}

void __cdecl network_set_online_environment(bool online_environment)
{
	INVOKE(0x0049E6C0, network_set_online_environment, online_environment);
}

void __cdecl network_shutdown_transport(void* userdata)
{
	//INVOKE(0x0049E6E0, network_shutdown_transport, userdata);

	if (network_globals.initialized)
	{
		if (g_network_link)
			g_network_link->create_endpoints();
	}
}

void __cdecl network_startup_transport(void* userdata)
{
	INVOKE(0x0049E770, network_startup_transport, userdata);
}

void __cdecl network_update()
{
	INVOKE(0x0049E7B0, network_update);
}

void __cdecl network_test_set_map_name(char const* scenario_path)
{
	if (network_squad_session_set_map(-1, -2, scenario_path))
	{
		console_printf("set %s successfully", scenario_path);

		static c_static_wchar_string<256> map_path;
		static c_map_variant map_variant;

		map_path.print(L"%hs", tag_name_strip_path(scenario_path));
		long multiplayer_map = levels_get_multiplayer_map_by_display_name(map_path.get_string());
		map_variant.create_default(multiplayer_map);

		network_test_set_map_variant(&map_variant);

		//BUILD_DEFAULT_GAME_VARIANT(game_variant, _game_engine_slayer_variant);
		//network_test_set_game_variant(&game_variant);
	}
	else
	{
		c_console::write_line("unable to set map %s", scenario_path);
	}
}

void __cdecl network_test_set_map_variant(c_map_variant const* map_variant)
{
	if (!user_interface_squad_set_map_variant(map_variant))
	{
		static c_map_variant default_map_variant;
		long multiplayer_map = levels_get_default_multiplayer_map_id();
		default_map_variant.create_default(multiplayer_map);
		user_interface_squad_set_map_variant(&default_map_variant);
	}
}

#define BUILD_DEFAULT_GAME_VARIANT(_game_variant, _game_engine_index)   \
if (DECLFUNC(0x00572560, long, __cdecl, long)(_game_engine_index) <= 0) \
    build_default_game_variant(&_game_variant, _game_engine_index);     \
else                                                                    \
    game_engine_tag_defined_variant_get_built_in_variant(_game_engine_index, 0, &_game_variant);

void __cdecl network_test_set_game_variant(e_game_engine_type game_engine_index)
{
	static c_game_variant game_variant;
	BUILD_DEFAULT_GAME_VARIANT(game_variant, game_engine_index);

	if (!user_interface_squad_set_game_variant(&game_variant))
	{
		static c_game_variant default_game_variant;
		BUILD_DEFAULT_GAME_VARIANT(default_game_variant, _game_engine_slayer_variant);
		user_interface_squad_set_game_variant(&default_game_variant);
	}
}

void __cdecl network_test_set_game_variant(char const* game_engine_name)
{
	e_game_engine_type game_engine_index = _game_engine_base_variant;

	for (long i = _game_engine_base_variant; i < k_game_engine_type_count; i++)
	{
		if (csstricmp(game_engine_name, game_engine_variant_get_name(i)) != 0)
			continue;

		game_engine_index = e_game_engine_type(i);
	}

	network_test_set_game_variant(game_engine_index);
}

void __cdecl network_test_set_session_mode(char const* session_mode_name)
{
	e_network_session_mode session_mode = _network_session_mode_none;

	for (long i = _network_session_mode_none; i < k_network_session_mode_count; i++)
	{
		if (csstricmp(session_mode_name, network_session_mode_get_name(i)) != 0)
			continue;

		session_mode = e_network_session_mode(i);
	}

	network_squad_session_set_session_mode(session_mode);
}

void __cdecl network_test_set_ui_game_mode(char const* ui_game_mode_name)
{
	e_gui_game_mode ui_game_mode = _ui_game_mode_none;

	for (long i = _ui_game_mode_campaign; i < k_ui_game_mode_count; i++)
	{
		if (csstricmp(ui_game_mode_name, ui_game_mode_get_name(i)) != 0)
			continue;

		ui_game_mode = e_gui_game_mode(i);
	}

	UI_WAIT(0.1f, user_interface_networking_enter_pregame_location, user_interface_squad_get_ui_game_mode, ui_game_mode);
}

void __cdecl network_test_set_advertisement_mode(char const* advertisement_mode_name)
{
	e_gui_network_session_advertisement_mode advertisement_mode = _gui_network_session_advertisement_mode_invalid;

	for (long i = _gui_network_session_advertisement_mode_open_to_public; i < k_gui_network_session_advertisement_mode_count; i++)
	{
		if (csstricmp(advertisement_mode_name, gui_network_session_advertisement_mode_get_name(i)) != 0)
			continue;

		advertisement_mode = e_gui_network_session_advertisement_mode(i);
	}

	UI_WAIT(0.1f, user_interface_squad_set_session_advertisement, user_interface_networking_get_session_advertisement, advertisement_mode);
}

void __cdecl network_test_set_game_variant_parameter(char const* parameter_name, long value, long* old_value)
{
	e_game_variant_parameter parameter = k_game_variant_parameter_none;

	for (long i = _game_variant_parameter_game_misc_teams; i < k_game_variant_parameter_count; i++)
	{
		if (csstricmp(parameter_name, game_variant_parameter_get_name(i)) != 0)
			continue;

		parameter = e_game_variant_parameter(i);
	}

	c_game_variant* game_variant = new c_game_variant();
	game_variant->copy_from_and_validate(network_life_cycle_session_get_game_variant());

	if (*old_value)
		game_variant->get_integer_game_engine_setting(parameter, old_value);

	game_variant->set_integer_game_engine_setting(parameter, value);
	user_interface_squad_set_game_variant(game_variant);
	delete game_variant;
}

void __cdecl network_test_ping()
{
	static word id = 0;
	if (network_initialized())
	{
		s_network_message_ping ping =
		{
			.id = id++,
			.timestamp = network_time_get(),
			.request_qos = 0
		};

		c_console::write_line("networking:test:ping: ping #%d sent at local %dms", id, network_time_get_exact());
		for (word broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			g_network_message_gateway->send_message_broadcast(_network_message_ping, sizeof(s_network_message_ping), &ping, broadcast_port);
	}
	else
	{
		c_console::write_line("networking:test: networking is not initialized");
	}
}

void __cdecl network_test_ping_directed(transport_address const* address)
{
	static word id = 0;
	if (network_initialized())
	{
		s_network_message_ping ping =
		{
			.id = id++,
			.timestamp = network_time_get(),
			.request_qos = 0
		};

		c_console::write_line("networking:test:ping: ping #%d sent at local %dms", id, network_time_get_exact());
		g_network_message_gateway->send_message_directed(address, _network_message_ping, sizeof(s_network_message_ping), &ping);
	}
	else
	{
		c_console::write_line("networking:test: networking is not initialized");
	}
}

void __cdecl network_test_text_chat(char const* text)
{
	if (network_initialized())
	{
		static s_network_message_text_chat text_chat{};
		memset(&text_chat, 0, sizeof(s_network_message_text_chat));

		text_chat.payload.destination_player_count = 16;
		text_chat.payload.text.print(L"%hs", text);

		for (word broadcast_port = k_broadcast_port; broadcast_port < k_broadcast_port + k_broadcast_port_alt_ammount; broadcast_port++)
			g_network_message_gateway->send_message_broadcast(_custom_network_message_text_chat, sizeof(s_network_message_text_chat), &text_chat, broadcast_port);
	}
	else
	{
		c_console::write_line("networking:test: networking is not initialized");
	}

}

void __cdecl network_test_text_chat_directed(transport_address const* address, char const* text)
{
	if (network_initialized())
	{
		static s_network_message_text_chat text_chat{};
		memset(&text_chat, 0xFF, sizeof(s_network_message_text_chat));
		memset(text_chat.payload.text_buffer, 0, sizeof(text_chat.payload.text_buffer));

		text_chat.payload.destination_player_count = 16;
		text_chat.payload.text.print(L"%hs", text);

		g_network_message_gateway->send_message_directed(address, _custom_network_message_text_chat, sizeof(s_network_message_text_chat), &text_chat);
	}
	else
	{
		c_console::write_line("networking:test: networking is not initialized");
	}

}

