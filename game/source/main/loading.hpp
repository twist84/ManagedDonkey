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
static_assert(sizeof(s_main_loading_action) == 0x10C);

struct loading_globals_definition
{
	char const* scenario_path;
	int32 insertion_point;
	bool tag_load_in_progress;
	bool tag_sync_in_progress;
	int32 tag_load_time;
	bool basic_progress_enabled;
	bool has_progress_sizes;
	int32 basic_progress_phase;
	real32 progress_sizes0[k_basic_loading_phase_count];
	real32 progress_sizes1[k_basic_loading_phase_count];
	real32 progress;
	bool spinner_enabled;
	bool loading_in_progress;

	static c_static_string<256> loading_progress;
	static c_static_string<256> copy_progress;
};
static_assert(sizeof(loading_globals_definition) == 0x38);

extern bool& disable_progress_screen;
extern loading_globals_definition& loading_globals;
extern int32& loaded_resource_bytes;
extern int32& total_resource_bytes;

extern void __cdecl loading_basic_progress_complete();
extern void __cdecl loading_basic_progress_disable();
extern void __cdecl loading_basic_progress_enable(char const* scenario_path, int32 insertion_point);
extern bool __cdecl loading_basic_progress_enabled();
extern real32 __cdecl loading_basic_progress_get();
extern void __cdecl loading_basic_progress_phase_begin(int32 phase, uint32 update_size);
extern void __cdecl loading_basic_progress_phase_end();
extern void __cdecl loading_basic_progress_update_fraction(real32 progress_fraction);
extern void __cdecl loading_basic_progress_update_phase_sizes();
extern void __cdecl loading_basic_progress_update_size(uint32 update_size);
extern bool __cdecl main_blocking_load_in_progress(real32* out_progress);
extern bool __cdecl main_load_map(char const* scenario_path, int32 map_load_type);
extern real32 __cdecl main_load_map_loading_progress(int32 scenario_type, int16 a2, char const* scenario_path);
extern int32 __cdecl main_load_map_status(char const* scenario_path);
extern int32 __cdecl main_load_map_status_with_insertion_point(int16 insertion_point, char const* scenario_path);
extern bool __cdecl main_load_map_with_insertion_point(int16 insertion_point, char const* scenario_path, int32 map_load_type);
extern bool __cdecl main_load_next_map_loading();
extern void __cdecl main_loading_dispose();
extern void __cdecl main_loading_enable_spinner(bool enable);
extern bool __cdecl main_loading_get_action(s_main_loading_action* out_action);
extern int32 __cdecl main_loading_get_gui_game_mode();
extern e_main_pregame_frame __cdecl main_loading_get_loading_status(c_static_wchar_string<12288>* status_message);
extern void __cdecl main_loading_idle();
extern void __cdecl main_loading_initialize();
extern bool __cdecl main_loading_is_idle();
extern void __cdecl main_loading_progress_done(char const* description, void* userdata);
extern void __cdecl main_loading_progress_new(char const* description, void* userdata);
extern void __cdecl main_loading_progress_update(char const* description, char const* scenario_path, int32 progress, void* userdata);

