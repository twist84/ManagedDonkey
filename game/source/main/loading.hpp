#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

struct s_main_loading_action
{
	char scenario_path[256];
	int32 load_action;
	e_campaign_id campaign_id;
	bool copy_map_start_only;
	bool copy_stop;
	bool map_has_progression;
	bool stop_map_prefetch;
};
COMPILE_ASSERT(sizeof(s_main_loading_action) == 0x10C);

struct loading_globals_definition
{
	const char* blocking_map_name;
	int32 insertion_point;
	bool tag_load_in_progress;
	bool tag_sync_in_progress;
	int32 tag_load_start_milliseconds;
	bool basic_progress_enabled;
	bool has_progress_sizes;
	e_basic_loading_phase basic_progress_phase;
	real32 basic_progress_maximum_bytes[k_basic_loading_phase_count];
	real32 basic_progress_current_bytes[k_basic_loading_phase_count];
	real32 basic_progress_fraction;
	bool spinner_enabled;
	bool progress_valid;

	static c_static_string<256> loading_progress;
};
COMPILE_ASSERT(sizeof(loading_globals_definition) == 0x38);

extern bool& disable_progress_screen;
extern loading_globals_definition& loading_globals;
extern int32& loaded_resource_bytes;
extern int32& total_resource_bytes;

extern void __cdecl loading_basic_progress_complete();
extern void __cdecl loading_basic_progress_disable();
extern void __cdecl loading_basic_progress_enable(const char* map_name, int32 insertion_point);
extern bool __cdecl loading_basic_progress_enabled();
extern real32 __cdecl loading_basic_progress_get();
extern void __cdecl loading_basic_progress_phase_begin(e_basic_loading_phase phase, uns32 bytes_remaining);
extern void __cdecl loading_basic_progress_phase_end();
extern void __cdecl loading_basic_progress_update_fraction(real32 progress_fraction);
extern void __cdecl loading_basic_progress_update_phase_sizes();
extern void __cdecl loading_basic_progress_update_size(uns32 bytes_remaining);
extern bool __cdecl main_blocking_load_in_progress(real32* progress);
extern bool __cdecl main_load_map(const char* map_name, int32 map_load_type);
extern real32 __cdecl main_load_map_loading_progress(int32 scenario_type, int16 campaign_insertion_point_index, const char* map_name);
extern int32 __cdecl main_load_map_status(const char* map_name);
extern int32 __cdecl main_load_map_status_with_insertion_point(int16 campaign_insertion_point_index, const char* map_name);
extern bool __cdecl main_load_map_with_insertion_point(int16 campaign_insertion_point_index, const char* map_name, int32 map_load_type);
extern bool __cdecl main_load_next_map_loading();
extern void __cdecl main_loading_dispose();
extern void __cdecl main_loading_enable_spinner(bool enable);
extern bool __cdecl main_loading_get_action(s_main_loading_action* out_action);
extern int32 __cdecl main_loading_get_gui_game_mode();
extern e_main_pregame_frame __cdecl main_loading_get_loading_status(c_static_wchar_string<12288>* loading_progress_string);
extern void __cdecl main_loading_idle();
extern void __cdecl main_loading_initialize();
extern bool __cdecl main_loading_is_idle();
extern void __cdecl main_loading_progress_done(const char* description, void* user_data);
extern void __cdecl main_loading_progress_new(const char* description, void* user_data);
extern void __cdecl main_loading_progress_update(const char* description, const char* optional_description, int32 progress, void* user_data);

