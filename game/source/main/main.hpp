#pragma once

#include "cseries/cseries.hpp"

enum e_main_reset_events_reason
{
	_main_reset_events_reason_changing_the_map = 0,
	_main_reset_events_reason_xsync_in_progress,

	k_number_of_main_reset_event_reasons
};

enum e_game_state_revert_bit
{
	_game_state_revert_bit_user = 0,
	_game_state_revert_bit_scripting,
	_game_state_revert_bit_scripting_cinematic,

	k_game_state_revert_bits
};

struct s_scenario_zone_activation
{
	void clear()
	{
		deactivating_designer_zone_mask = 0;
		activating_designer_zone_mask = 0;
		deactivating_cinematic_zone_mask = 0;
		activating_cinematic_zone_mask = 0;
	}

	bool is_empty() const // .text:00505170
	{
		return !deactivating_designer_zone_mask
			&& !activating_designer_zone_mask
			&& !deactivating_cinematic_zone_mask
			&& !activating_cinematic_zone_mask;
	}

	long deactivating_designer_zone_mask;
	long activating_designer_zone_mask;
	long deactivating_cinematic_zone_mask;
	long activating_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_activation) == 0x10);

struct _main_globals
{
	bool suppress_startup_sequence;
	bool has_performed_startup_sequence;
	bool halted_with_errors;
	bool run_demos;
	bool exit_game;

	bool save_core;
	bool load_core;
	c_static_string<64> core_name;

	bool map_reset;
	bool map_reset_random;

	bool map_revert;
	c_flags<e_game_state_revert_bit, byte, k_game_state_revert_bits> map_revert_flags;

	bool save;
	bool save_and_exit;
	bool skip_cinematic;
	bool prepare_to_switch_zone_set;
	bool volatile switch_zone_set;
	bool user_interface_save_files;
	bool non_bsp_zone_activation;

	//bool cheat_drop_tag;
	byte __pad52[0x1]; // is this the above?

	bool game_state_decompression;
	bool reset_zone_resources;
	bool reloading_active_zone_set;
	bool activate_cinematic_tag;
	bool activate_cinematic_zone_from_tag;
	bool deactivate_cinematic_zone_from_tag;
	//bool cleanup_loading_screen; // unused is this the still here?
	long prepare_to_switch_zone_set_index;
	long switch_zone_set_index;
	s_scenario_zone_activation scenario_zone_activation;

	//dword cinematic_zone_group_tag;
	//long cinematic_zone_tag_index;
	byte __pad74[8]; // is this the above?

	long main_loop_pregame_entered;
	dword main_loop_time;
};
static_assert(sizeof(_main_globals) == 0x84);

struct s_main_status_value
{
	char status_type[256];
	char status_value[512];
};

class c_tag_resources_game_lock
{
public:
	c_tag_resources_game_lock();
	~c_tag_resources_game_lock();

protected:
	long lock;
};

struct c_interlocked_long;
extern bool& g_force_upload_even_if_untracked;
extern bool& g_render_thread_user_setting;
extern bool& disable_main_loop_throttle;
extern s_main_status_value(&g_status_values)[32];
extern c_interlocked_long& g_render_thread_waiting;
extern c_interlocked_long& g_render_thread_enabled;
extern c_interlocked_long& g_single_thread_request_flags;
extern _main_globals& main_globals;

extern bool g_fake_minidump_creation;
extern bool g_suppress_keyboard_for_minidump;

extern bool debug_console_pauses_game;
extern bool debug_no_drawing;

extern char const* const k_main_event_reason_description[k_number_of_main_reset_event_reasons];

struct s_file_reference;
struct s_game_tick_time_samples;
struct s_model_customization_region_permutation;

extern dword __cdecl _internal_halt_render_thread_and_lock_resources(char const* file, long line);
extern dword __cdecl audio_thread_loop(void* thread_parameter);
extern void __cdecl main_activate_cinematic_tag_private();
extern void __cdecl main_activate_cinematic_zone(long cinematic_zone_index);
extern void __cdecl main_activate_cinematic_zone_for_debugging(long cinematic_zone_index);
extern void __cdecl main_activate_designer_zone(long designer_zone_index);
extern void __cdecl main_clear_global_pending_zone_activation(long game_state_proc_flags);
extern void __cdecl main_deactivate_cinematic_tag_private();
extern void __cdecl main_deactivate_cinematic_zone(long cinematic_zone_index);
extern void __cdecl main_deactivate_designer_zone(long designer_zone_index);
extern void __cdecl main_decompress_gamestate();
extern void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool* variable);
extern void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool volatile* variable);
extern bool __cdecl main_events_pending();
extern void __cdecl main_events_reset(e_main_reset_events_reason reason);
extern bool __cdecl main_events_pending();
extern void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool* variable);
extern void __cdecl main_event_reset_internal(char const* name, e_main_reset_events_reason reason, bool volatile* variable);
extern void __cdecl main_events_reset(e_main_reset_events_reason reason);
extern void __cdecl main_exit_game();
extern void __cdecl main_game_gamestate_decompress_and_apply_private();
extern bool __cdecl main_game_is_exiting();
extern void __cdecl main_halt_and_catch_fire();
extern void __cdecl main_halt_and_display_errors();
extern bool __cdecl main_halted_with_errors();
extern bool __cdecl main_is_in_main_loop_pregame();
extern bool __cdecl main_is_switching_zone_sets();
extern bool __cdecl main_kick_startup_masking_sequence(bool kick_input);
extern void __cdecl main_load_core();
extern void __cdecl main_load_core_name(char const* core_name);
extern void __cdecl main_load_core_private();
extern void __cdecl main_loop_initialize_restricted_regions();
extern void __cdecl main_loop_dispose_restricted_regions();
extern void __cdecl main_loop_body(dword* wait_for_render_thread, dword* time);
extern void __cdecl main_loop();
extern void __cdecl main_loop_body_main_part();
extern void __cdecl main_loop_body_multi_threaded();
extern void __cdecl main_loop_body_single_threaded();
extern void __cdecl main_loop_enter();
extern void __cdecl main_loop_exit();
extern void __cdecl main_loop_pregame();
extern void __cdecl main_loop_pregame_disable(bool disable);
extern void __cdecl main_loop_pregame_show_progress_screen();
extern void __cdecl main_loop_process_global_state_changes();
extern void __cdecl main_loop_status_message(wchar_t const* status_message);
extern bool __cdecl main_menu_has_performed_startup_sequence();
extern void __cdecl main_modify_zone_activation_private();
extern void __cdecl main_prepare_for_switch_zone_set(long zone_set_index);
extern void __cdecl main_prepare_to_switch_zone_set_private();
extern void __cdecl main_print_version();
extern void __cdecl main_reload_active_zone_set_private();
extern void __cdecl main_reset_map();
extern void __cdecl main_reset_map_internal(bool random);
extern void __cdecl main_reset_map_private();
extern void __cdecl main_reset_map_random();
extern void __cdecl main_reset_zone_resources();
extern void __cdecl main_reset_zone_resources_private();
extern void __cdecl main_revert_map(bool user);
extern void __cdecl main_revert_map_private();
extern void __cdecl main_revert_map_scripting();
extern void __cdecl main_run_demos_private();
extern void __cdecl main_save_and_exit_campaign();
extern void __cdecl main_save_and_exit_campaign_immediately();
extern void __cdecl main_save_core();
extern void __cdecl main_save_core_name(char const* core_name);
extern void __cdecl main_save_core_private();
extern void __cdecl main_save_map();
extern void __cdecl main_save_map_and_exit_private();
extern bool __cdecl main_save_map_pending();
extern void __cdecl main_save_map_private();
extern void __cdecl main_set_active_designer_zone_mask(dword a1);
extern void __cdecl main_set_single_thread_request_flag(long single_threaded_request_flags, bool set);
extern void __cdecl main_skip_cinematic();
extern void __cdecl main_skip_cinematic_private();
extern bool __cdecl main_startup_sequence();

template<typename... parameters_t, long k_parameter_count = sizeof...(parameters_t)>
bool main_status(char const* status_type, char const* format, parameters_t... parameters)
{
	return DECLFUNC(0x00506FB0, bool, __cdecl, char const*, char const*, ...)(status_type, format, parameters...);
}

extern void __cdecl main_status_dump(s_file_reference* file);
extern void __cdecl main_status_print();
extern void __cdecl main_suppress_startup_sequence();
extern void __cdecl main_switch_bsp(long zone_set_index);
extern void __cdecl main_switch_zone_set(long zone_set_index);
extern void __cdecl main_switch_zone_set_private();
extern void __cdecl main_thread_combine_timing_data(void* address);
extern void __cdecl main_thread_lock_rasterizer_and_resources();
extern void __cdecl main_thread_process_pending_messages();
extern void __cdecl main_thread_unlock_rasterizer_and_resources();
extern bool __cdecl main_time_halted();
extern void __cdecl main_user_interface_save_files();
extern void __cdecl main_user_interface_save_files_private();
extern void __cdecl process_published_game_state(bool a1);
extern void __cdecl publish_waiting_gamestate();
extern bool __cdecl render_thread_enabled();
extern long __cdecl render_thread_get_mode();
extern bool __cdecl render_thread_set_mode(long mode_compare, long mode_exchange);
extern void __cdecl unlock_resources_and_resume_render_thread(dword flags);
extern void __cdecl main_write_stack_to_crash_info_status_file(char const* crash_info, void* context);
extern void __cdecl main_crash(char const* type);
extern void __cdecl main_cheat_drop_tag(long tag_index, long variant_name, s_model_customization_region_permutation const* permutations, long permutation_count);
extern void __cdecl main_cheat_drop_tag_private();

