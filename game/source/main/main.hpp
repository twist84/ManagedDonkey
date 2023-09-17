#pragma once

#include "cseries/cseries.hpp"

extern bool& g_force_upload_even_if_untracked;

extern bool __cdecl main_events_pending();
extern void __cdecl main_exit_game();
extern void __cdecl main_halt_and_catch_fire();
extern void __cdecl main_loop_body_begin();
extern void __cdecl main_loop_body_mid(real shell_seconds_elapsed);
extern void __cdecl main_loop_body_end();
extern void __cdecl main_loop_enter();
extern void __cdecl main_loop_exit();
extern void __cdecl main_loop_pregame();
extern void __cdecl main_loop_pregame_disable(bool disable);
extern void __cdecl main_loop_pregame_show_progress_screen();
extern void __cdecl main_loop_status_message(wchar_t const* status_message);
extern void main_set_single_thread_request_flag(long single_threaded_request_flags, bool set);

template<typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
bool main_status(char const* status_type, char const* format, parameters_t... parameters)
{
	return DECLFUNC(0x00506FB0, bool, __cdecl, char const*, char const*, ...)(status_type, format, parameters...);
}

struct s_file_reference;
struct s_model_customization_region_permutation;
extern void __cdecl main_status_dump(s_file_reference* file);
extern dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern bool __cdecl main_time_halted();
extern bool __cdecl render_thread_enabled();
extern void __cdecl unlock_resources_and_resume_render_thread(dword flags);
extern void __cdecl main_write_stack_to_crash_info_status_file(char const* crash_info, void* context);
extern void __cdecl main_crash(char const* type);
extern void __cdecl main_cheat_drop_tag(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl main_cheat_drop_tag_private();
extern void __cdecl main_switch_zone_set(long zone_set_index);

