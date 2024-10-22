#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_text_parser.hpp"

struct s_saved_game_file_text_parser_input
{
	long user_index;
	wchar_t const* map_variant_name;
	wchar_t const* game_variant_name;
	wchar_t const* campaign_map_name;
	long campaign_difficulty;
};
static_assert(sizeof(s_saved_game_file_text_parser_input) == 0x14);

struct c_saved_game_files_globals
{
	byte_flags flags;
	byte __data1[3];
	s_saved_game_file_text_parser_input text_parser_input;
	c_magic_string_game_tag_parser map_variant_name_parser;
	c_magic_string_game_tag_parser game_variant_name_parser;
	c_magic_string_game_tag_parser campaign_map_name_parser;
	c_magic_string_game_tag_parser difficulty_parser;
};
static_assert(sizeof(c_saved_game_files_globals) == 0x68);

extern c_saved_game_files_globals& g_saved_game_files_globals;

extern void __cdecl saved_game_files_dispose();
extern void __cdecl saved_game_files_dispose_from_old_map();
extern void __cdecl saved_game_files_initialize();
extern void __cdecl saved_game_files_initialize_for_new_map();
extern void __cdecl saved_game_files_update();

