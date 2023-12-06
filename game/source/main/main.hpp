#pragma once

#include "cseries/cseries.hpp"

struct c_interlocked_long;
extern bool& g_force_upload_even_if_untracked;
extern bool& g_render_thread_user_setting;
extern bool& disable_main_loop_throttle;
extern c_interlocked_long& g_render_thread_waiting;
extern c_interlocked_long& g_render_thread_enabled;
extern c_interlocked_long& g_single_thread_request_flags;
extern bool& g_main_game_exit;

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
extern void __cdecl main_set_single_thread_request_flag(long single_threaded_request_flags, bool set);

template<typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
bool main_status(char const* status_type, char const* format, parameters_t... parameters)
{
	return DECLFUNC(0x00506FB0, bool, __cdecl, char const*, char const*, ...)(status_type, format, parameters...);
}

struct s_file_reference;
struct s_model_customization_region_permutation;
extern void __cdecl main_status_dump(s_file_reference* file);
extern void __cdecl main_switch_zone_set(long zone_set_index);
extern dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern void __cdecl main_thread_process_pending_messages();
extern bool __cdecl main_time_halted();
extern void __cdecl publish_waiting_gamestate();
extern long __cdecl render_thread_get_mode();
extern bool __cdecl render_thread_enabled();
extern void __cdecl unlock_resources_and_resume_render_thread(dword flags);
extern void __cdecl main_write_stack_to_crash_info_status_file(char const* crash_info, void* context);
extern void __cdecl main_crash(char const* type);
extern void __cdecl main_cheat_drop_tag(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl main_cheat_drop_tag_private();

