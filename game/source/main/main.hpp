#pragma once

#include "cseries/cseries.hpp"

struct s_async_task;
struct s_file_reference;
struct s_game_tick_time_samples;
struct s_model_customization_region_permutation;

struct s_scenario_zone_activation
{
	void clear()
	{
		deactivating_designer_zone_mask = 0;
		activating_designer_zone_mask = 0;
		deactivating_cinematic_zone_mask = 0;
		activating_cinematic_zone_mask = 0;
	}

	bool is_empty() const //.text:00505170
	{
		return !deactivating_designer_zone_mask
			&& !activating_designer_zone_mask
			&& !deactivating_cinematic_zone_mask
			&& !activating_cinematic_zone_mask;
	}

	int32 deactivating_designer_zone_mask;
	int32 activating_designer_zone_mask;
	int32 deactivating_cinematic_zone_mask;
	int32 activating_cinematic_zone_mask;
};
static_assert(sizeof(s_scenario_zone_activation) == 0x10);

struct _main_globals
{
	bool startup_sequence;
	bool startup_sequence_performed;
	bool halted_with_errors;
	bool run_xdemos;
	bool exit_game;

	bool save_core;
	bool load_core;
	c_static_string<64> core_file_name;

	bool reset_map;
	bool reset_map_random_seed;

	bool revert_map;
	c_flags<e_game_state_revert_bit, uns8, k_number_of_game_state_revert_bits> revert_map_flags;

	bool save_map;
	bool save_map_and_exit;
	bool skip_cinematic;
	bool prepare_to_switch_zone_set;
	bool volatile switch_zone_set;
	bool ui_saving_files;
	bool modify_zone_activation;

	bool drop_cheat_tag;

	bool gamestate_decompression_pending;
	bool reset_zone_resources;
	bool reload_active_zone_set;
	bool activate_cinematic_tag;
	bool activate_cinematic_zone_from_tag;
	bool deactivate_cinematic_zone_from_tag;
	//bool cleanup_loading_screen; // unused is this the still here?
	int32 prepare_to_switch_to_zone_set_index;
	int32 switch_to_zone_set_index;
	s_scenario_zone_activation pending_zone_activation;

	//uns32 cinematic_zone_group_tag;
	//int32 cinematic_zone_tag_index;
	byte __pad74[0x8]; // is this the above?

	int32 main_loop_pregame_entered;
	uns32 main_loop_pregame_last_time;
};
static_assert(sizeof(_main_globals) == 0x84);

struct s_main_status_value
{
	char status_type[256];
	char status_data[512];
};

class c_tag_resources_game_lock
{
public:
	c_tag_resources_game_lock();
	~c_tag_resources_game_lock();

protected:
	int32 m_resource_key;
};
#define TAG_RESOURCES_GAME_LOCK(...) c_tag_resources_game_lock __local_tag_resource_scope_lock{}

class c_interlocked_long;
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

extern bool g_toggle_game_scripts;

extern bool debug_console_pauses_game;
extern bool debug_no_drawing;

extern const char* const k_main_event_reason_description[k_number_of_main_reset_event_reasons];

extern void __cdecl __tls_set_g_main_gamestate_timing_data_allocator(void* new_address);
extern void __cdecl __tls_set_g_main_render_timing_data_allocator(void* new_address);
extern uns32 __cdecl _internal_halt_render_thread_and_lock_resources(const char* file, int32 line);
extern uns32 __cdecl audio_thread_loop(void* blah);
extern void __cdecl main_activate_cinematic_tag_private();
extern void __cdecl main_activate_cinematic_zone(int32 cinematic_zone_index);
extern void __cdecl main_activate_cinematic_zone_for_debugging(int32 cinematic_zone_index);
extern void __cdecl main_activate_designer_zone(int32 designer_zone_index);
extern void __cdecl main_cheat_drop_tag(int32 tag_index, int32 variant_name, const s_model_customization_region_permutation* permutations, int32 permutation_count);
extern void __cdecl main_cheat_drop_tag_private();
extern void __cdecl main_clear_global_pending_zone_activation(int32 game_state_proc_flags);
extern void __cdecl main_crash(const char* type);
extern e_async_completion __cdecl main_crash_async(s_async_task* task, void* data, int32 data_size);
extern void __cdecl main_crash_just_upload_dammit();
extern void __cdecl main_deactivate_cinematic_tag_private();
extern void __cdecl main_deactivate_cinematic_zone(int32 cinematic_zone_index);
extern void __cdecl main_deactivate_designer_zone(int32 designer_zone_index);
extern void __cdecl main_decompress_gamestate();
extern void __cdecl main_event_reset_internal(const char* description, e_main_reset_events_reason reason, bool* event_flag);
extern void __cdecl main_event_reset_internal(const char* description, e_main_reset_events_reason reason, bool volatile* event_flag);
extern bool __cdecl main_events_pending();
extern void __cdecl main_events_reset(e_main_reset_events_reason reason);
extern bool __cdecl main_events_pending();
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
extern void __cdecl main_load_core_name(const char* core_name);
extern void __cdecl main_load_core_private();
extern void __cdecl main_loop_dispose_restricted_regions();
extern void __cdecl main_loop_initialize_restricted_regions();
extern void __cdecl main_loop();
extern void __cdecl main_loop_body();
extern void __cdecl main_loop_body_multi_threaded();
extern void __cdecl main_loop_body_single_threaded();
extern void __cdecl main_loop_enter();
extern void __cdecl main_loop_exit();
extern bool __cdecl main_loop_is_suspended();
extern void __cdecl main_loop_pregame();
extern void __cdecl main_loop_pregame_disable(bool disable);
extern void __cdecl main_loop_pregame_halt_and_catch_fire_pop();
extern void __cdecl main_loop_pregame_halt_and_catch_fire_push();
extern void __cdecl main_loop_pregame_show_progress_screen();
extern void __cdecl main_loop_process_global_state_changes();
extern void __cdecl main_loop_resume();
extern void __cdecl main_loop_suspend();
extern void __cdecl main_loop_status_message(const wchar_t* status_message);
extern bool __cdecl main_menu_has_performed_startup_sequence();
extern void __cdecl main_modify_zone_activation_private();
extern void __cdecl main_prepare_for_switch_zone_set(int32 zone_set_index);
extern void __cdecl main_prepare_to_switch_zone_set_private();
extern void __cdecl main_print_version();
extern void __cdecl main_reload_active_zone_set_private();
extern void __cdecl main_reset_map();
extern void __cdecl main_reset_map_internal(bool use_new_random_seed);
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
extern void __cdecl main_save_core_name(const char* core_name);
extern void __cdecl main_save_core_private();
extern void __cdecl main_save_map();
extern void __cdecl main_save_map_and_exit_private();
extern bool __cdecl main_save_map_pending();
extern void __cdecl main_save_map_private();
extern void __cdecl main_set_active_designer_zone_mask(uns32 designer_zone_mask);
extern void __cdecl main_set_single_thread_request_flag(e_single_threaded_request_flags flag, bool setting);
extern void __cdecl main_skip_cinematic();
extern void __cdecl main_skip_cinematic_private();
extern bool __cdecl main_startup_sequence();

template<typename... parameters_t, int32 k_parameter_count = sizeof...(parameters_t)>
bool main_status(const char* status_type, const char* format, parameters_t... parameters)
{
	return DECLFUNC(0x00506FB0, bool, __cdecl, const char*, const char*, ...)(status_type, format, parameters...);
}

extern void __cdecl main_status_dump(s_file_reference* file);
extern void __cdecl main_status_print();
extern void __cdecl main_suppress_startup_sequence();
extern void __cdecl main_switch_bsp(int32 zone_set_index);
extern void __cdecl main_switch_scenario_and_zone_set(const char* map_name, int32 initial_zone_set);
extern void __cdecl main_switch_zone_set(int32 zone_set_index);
extern void __cdecl main_switch_zone_set_private();
extern void __cdecl main_thread_combine_timing_data(void* address);
extern void __cdecl main_thread_lock_rasterizer_and_resources();
extern void __cdecl main_thread_process_pending_messages();
extern void __cdecl main_thread_unlock_rasterizer_and_resources();
extern bool __cdecl main_time_halted();
extern void __cdecl main_user_interface_save_files();
extern void __cdecl main_user_interface_save_files_private();
extern void __cdecl process_published_game_state(bool render);
extern void __cdecl publish_waiting_gamestate();
extern e_render_thread_mode __cdecl render_thread_get_mode();
extern bool __cdecl render_thread_set_mode(e_render_thread_mode old_setting, e_render_thread_mode setting);
extern void __cdecl unlock_resources_and_resume_render_thread(uns32 token);
extern void __cdecl main_write_stack_to_crash_info_status_file(const char* crash_info, void* context);
extern void __cdecl main_trace_event_internal(const char* context);

