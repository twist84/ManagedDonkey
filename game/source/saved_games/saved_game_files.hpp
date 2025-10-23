#pragma once

#include "cseries/cseries.hpp"
#include "interface/user_interface_text_parser.hpp"

class c_synchronized_long;
struct s_saved_game_item_metadata;

struct s_saved_game_file_text_parser_input
{
	int32 user_index;
	const wchar_t* map_variant_name;
	const wchar_t* game_variant_name;
	const wchar_t* campaign_map_name;
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

extern e_saved_game_file_type __cdecl game_engine_index_to_saved_game_file_type(e_game_engine_type game_engine_type);
extern void __cdecl hs_saved_game_files_delete_saved_game_state_test(int16 controller_index);
extern void __cdecl hs_saved_game_files_enumeration_test(int16 controller_index);
extern void __cdecl hs_saved_game_files_save_last_film_test(int16 controller_index);
extern e_game_content_type __cdecl saved_game_file_type_to_game_content_type(e_saved_game_file_type saved_game_file_type);
extern const wchar_t* __cdecl saved_game_filename_extension_by_game_file_type(e_saved_game_file_type game_file_type, bool exclude_dot_character);
extern bool __cdecl saved_game_files_controller_has_saved_game_state_blocking(e_controller_index controller_index);
extern void __cdecl saved_game_files_delete_saved_game_state_blocking(e_controller_index controller_index);
extern void __cdecl saved_game_files_dispose();
extern void __cdecl saved_game_files_dispose_from_old_map();
extern bool __cdecl saved_game_files_get_default_film_save_description(e_controller_index controller_index, e_gui_game_mode game_mode, const wchar_t* map_name, const wchar_t* game_variant_name, e_campaign_difficulty_level difficulty, wchar_t* description, int32 maximum_characters);
extern bool __cdecl saved_game_files_get_default_film_save_name(e_controller_index controller_index, e_gui_game_mode game_mode, const wchar_t* map_name, const wchar_t* game_variant_name, e_campaign_difficulty_level difficulty, bool uniquify, wchar_t* name, int32 maximum_characters);
extern void __cdecl saved_game_files_initialize();
extern void __cdecl saved_game_files_initialize_for_new_map();
extern int32 __cdecl saved_game_files_save_last_film(e_controller_index controller_index, c_synchronized_long* success, c_synchronized_long* done, wchar_t* out_display_name, int32 maximum_display_name_characters);
extern int32 __cdecl saved_game_files_save_last_film_to_debugging_hard_drive(e_controller_index controller_index, void* copy_buffer, uns32 copy_buffer_size, c_synchronized_long* success, c_synchronized_long* done);
extern void __cdecl saved_game_files_uniquify_name(e_controller_index controller_index, e_saved_game_file_type saved_game_file_type, wchar_t* name, int32 maximum_characters);
extern void __cdecl saved_game_files_update();
extern bool __cdecl saved_game_read_metadata_from_file(s_file_reference* file, s_saved_game_item_metadata* out_metadata);

