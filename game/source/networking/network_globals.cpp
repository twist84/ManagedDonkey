#include "networking/network_globals.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "game/game_options.hpp"
#include "interface/user_interface.hpp"
#include "interface/user_interface_networking.hpp"
#include "interface/user_interface_session.hpp"
#include "main/levels.hpp"
#include "networking/delivery/network_link.hpp"
#include "networking/logic/network_session_interface.hpp"
#include "saved_games/scenario_map_variant.hpp"
#include "tag_files/tag_groups.hpp"

#include <stdlib.h>

REFERENCE_DECLARE(0x0224A4B4, s_network_globals, network_globals);

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
		c_console::write_line("set %s successfully", scenario_path);

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

