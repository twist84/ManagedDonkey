#pragma once

#include "cseries/cseries.hpp"
#include "text/unicode.hpp"

extern bool& disable_progress_screen;

// struct `s_loading_globals`
extern char const*& loading_globals_scenario_path;
extern long& loading_globals_insertion_point;
extern bool& loading_globals_tag_load_in_progress;
extern bool& loading_globals_basic_progress_enabled;
extern bool& loading_globals_progress_sizes;
extern long& loading_globals_basic_progress_phase;
extern real& loading_globals_progress;
extern bool& loading_globals_spinner_enabled;
extern bool& loading_globals_progress_start;

extern void __cdecl loading_basic_progress_complete();
extern void __cdecl loading_basic_progress_disable();
extern void __cdecl loading_basic_progress_enable(char const* scenario_path, long insertion_point);
extern bool __cdecl loading_basic_progress_enabled();
extern real __cdecl loading_basic_progress_get();
extern void __cdecl loading_basic_progress_phase_begin(long phase, dword update_size);
extern void __cdecl loading_basic_progress_phase_end();
extern void __cdecl loading_basic_progress_update_fraction(real update_fraction);
extern void __cdecl loading_basic_progress_update_phase_sizes();
extern void __cdecl loading_basic_progress_update_size(dword update_size);
extern bool __cdecl main_blocking_load_in_progress(real* out_progress);

extern bool force_load_map_failed;
extern bool __cdecl main_load_map(char* scenario_path, long map_load_type);

extern real __cdecl main_load_map_loading_progress(long scenario_type, short a2, char const* scenario_path);
extern long __cdecl main_load_map_status(char const* scenario_path);
extern long __cdecl main_load_map_status_with_insertion_point(short insertion_point, char const* scenario_path);
extern bool __cdecl main_load_map_with_insertion_point(short insertion_point, char const* scenario_path, long map_load_type);
extern bool __cdecl main_load_next_map_loading();
extern void __cdecl main_loading_dispose();
extern void __cdecl main_loading_enable_spinner(bool enable_spinner);
extern bool __cdecl main_loading_get_action(struct s_main_loading_action* out_loading_action);
extern long __cdecl main_loading_get_gui_game_mode();
extern long __cdecl main_loading_get_loading_status(c_static_wchar_string<12288>* status_message);
extern void __cdecl main_loading_idle_with_blocking_load(bool blocking_load);
extern void __cdecl main_loading_initialize();
extern bool __cdecl main_loading_is_idle();
extern void __cdecl main_loading_progress_done(char const* scenario_path, void* a2);
extern void __cdecl main_loading_progress_new(char const* scenario_path, void* a2);
extern void __cdecl main_loading_progress_update(char const* scenario_path, char const* a2, long progress, void* a4);

