#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

enum e_controller_index;
struct s_player_configuration;
struct s_player_identifier;
struct c_map_variant;
struct c_game_variant;
struct s_saved_film_description;

extern bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode);
extern e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, dword* player_error_mask);
extern bool __cdecl user_interface_interactive_session_is_squad();
extern void __cdecl user_interface_leave_sessions(e_user_interface_session_leave_type leave_type, e_user_interface_session_leave_reason leave_reason);
extern long __cdecl user_interface_local_player_count();
extern e_controller_index user_interface_session_get_controller_index(long player_index);
extern bool __cdecl user_interface_session_get_map(long* campaign_id, long* map_id);
extern s_player_configuration* __cdecl user_interface_session_get_player_data(long player_index);
extern bool __cdecl user_interface_session_is_local_player(long player_index);
extern bool __cdecl user_interface_squad_is_player_valid(long player_index);
extern long __cdecl user_interface_squad_get_player_index(s_player_identifier const* player_identifier);
extern bool __cdecl user_interface_squad_exists();
extern long __cdecl user_interface_squad_get_countdown_delaying_player();
extern long __cdecl user_interface_squad_get_countdown_timer();
extern long __cdecl user_interface_squad_get_machine_count();
extern e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode();
extern bool __cdecl user_interface_squad_set_map_variant(c_map_variant const* map_variant);
extern bool __cdecl user_interface_squad_set_multiplayer_map_internal(c_map_variant const* map_variant, bool needs_verification);
extern bool __cdecl user_interface_squad_set_film(s_saved_film_description const* film);
extern bool __cdecl user_interface_squad_set_film_internal(s_saved_film_description const* film, bool needs_verification);
extern bool __cdecl user_interface_squad_set_game_variant(c_game_variant const* game_variant);

