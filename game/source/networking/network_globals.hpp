#pragma once

struct c_map_variant;
struct c_game_variant;
enum e_game_engine_type;

extern void __cdecl network_update();
extern void __cdecl network_test_set_map_name(char const* scenario_path);
extern void __cdecl network_test_set_map_variant(c_map_variant const* map_variant);
extern void __cdecl network_test_set_game_variant(char const* game_engine_name);
extern void __cdecl network_test_set_session_mode(char const* session_mode_name);
extern void __cdecl network_test_set_ui_game_mode(char const* ui_game_mode_name);
extern void __cdecl network_test_set_advertisement_mode(char const* advertisement_mode_name);
extern void __cdecl network_test_set_game_variant_parameter(char const* parameter_name, long value, long* old_value);

