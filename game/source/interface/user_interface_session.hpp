#pragma once

#include "cseries/cseries.hpp"

struct c_map_variant;
struct c_game_variant;
struct s_player_configuration;
struct s_player_identifier;
struct s_saved_film_description;

extern bool __cdecl user_interface_squad_set_session_advertisement(e_gui_network_session_advertisement_mode advertisement_mode);
extern e_campaign_difficulty_level __cdecl user_interface_game_settings_get_campaign_difficulty();
extern e_session_game_start_status __cdecl user_interface_get_session_game_start_status(e_session_game_start_error* error, uns32* player_error_mask);
extern bool __cdecl user_interface_interactive_session_is_squad();
extern void __cdecl user_interface_leave_sessions(e_user_interface_session_leave_type leave_type, e_user_interface_session_leave_reason leave_reason);
extern int32 __cdecl user_interface_local_player_count();
extern e_controller_index user_interface_session_get_controller_index(int32 player_index);
extern bool __cdecl user_interface_session_get_map(e_campaign_id* campaign_id, e_map_id* map_id);
extern s_player_configuration* __cdecl user_interface_session_get_player_data(int32 player_index);
extern bool __cdecl user_interface_session_is_local_player(int32 player_index);
extern bool __cdecl user_interface_squad_is_player_valid(int32 player_index);
extern int32 __cdecl user_interface_squad_get_player_index(s_player_identifier const* player_identifier);
extern bool __cdecl user_interface_squad_exists();
extern int32 __cdecl user_interface_squad_get_countdown_delaying_player();
extern int32 __cdecl user_interface_squad_get_countdown_timer();
extern int32 __cdecl user_interface_squad_get_machine_count();
extern int32 __cdecl user_interface_squad_get_player_count();
extern s_player_identifier const* __cdecl user_interface_session_get_player_identifier(int32 session_player_index);
extern e_gui_game_mode __cdecl user_interface_squad_get_ui_game_mode();
extern bool __cdecl user_interface_squad_in_or_after_countdown();
extern bool __cdecl user_interface_squad_local_peer_is_leader();
extern bool __cdecl user_interface_squad_set_film(s_saved_film_description const* film);
extern bool __cdecl user_interface_squad_set_film_internal(s_saved_film_description const* film, bool needs_verification);
extern bool __cdecl user_interface_squad_set_game_variant(c_game_variant const* game_variant);
extern bool __cdecl user_interface_squad_set_map_variant(c_map_variant const* map_variant);
extern bool __cdecl user_interface_squad_set_multiplayer_map_internal(c_map_variant const* map_variant, bool needs_verification);
extern bool __cdecl user_interface_squad_start_countdown_timer(e_controller_index controller_index, int32 countdown_timer, int32 accelerate_countdown_timer);
extern bool __cdecl user_interface_squad_stop_countdown_timer(e_controller_index controller_index, int32 countdown_timer, bool halo2);

