#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_text_parser.hpp"

struct s_saved_game_file_text_parser_input
{
	int32 user_index;
	wchar_t const* map_variant_name;
	wchar_t const* game_variant_name;
	wchar_t const* campaign_map_name;
	int32 campaign_difficulty;
};
static_assert(sizeof(s_saved_game_file_text_parser_input) == 0x14);

struct s_saved_game_file_globals
{
	c_flags<e_saved_game_flags, int32, k_saved_game_flags_count> flags;
	s_saved_game_file_text_parser_input saved_game_file_text_parse_data;
	c_magic_string_game_tag_parser map_variant_name_parser;
	c_magic_string_game_tag_parser game_variant_name_parser;
	c_magic_string_game_tag_parser campaign_map_name_parser;
	c_magic_string_game_tag_parser campaign_difficulty_parser;
};
static_assert(sizeof(s_saved_game_file_globals) == 0x68);

extern s_saved_game_file_globals& g_saved_game_files_globals;

extern bool __cdecl saved_game_files_controller_has_saved_game_state_blocking(e_controller_index controller_index);
extern void __cdecl saved_game_files_dispose();
extern void __cdecl saved_game_files_dispose_from_old_map();
extern void __cdecl saved_game_files_initialize();
extern void __cdecl saved_game_files_initialize_for_new_map();
extern void __cdecl saved_game_files_update();

