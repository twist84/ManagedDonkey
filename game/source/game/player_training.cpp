#include "game/player_training.hpp"

#include "memory/module.hpp"
#include "rasterizer/rasterizer_profile.hpp"

HOOK_DECLARE(0x00689EF0, player_training_render_screen);

//.text:00689550 ; 
//.text:00689570 ; 
//.text:00689580 ; void __cdecl __tls_set_g_player_training_globals_allocator(void*)
//.text:006895A0 ; 
//.text:006895D0 ; public: bool __cdecl s_player_training_profile_data::add_training_data(const s_player_training_profile_data*)
//.text:00689640 ; 
//.text:00689680 ; public: void __cdecl c_hold_watcher::check_action(int32, const player_action*)
//.text:006896F0 ; 
//.text:00689720 ; public: bool __cdecl c_recent_actions::did_player_recently_perform_action(int32, int32)
//.text:00689770 ; 
//.text:006897A0 ; s_player_training_entry_data* __cdecl get_player_training_entry_data(int32)
//.text:006897D0 ; int32 __cdecl get_training_text(int32)
//.text:00689800 ; bool __cdecl increment_training_completion(int32, s_player_training_user*, const s_player_training_entry_data*, s_player_training_state*)
//.text:006898F0 ; void __cdecl player_training_activate_crouch()
//.text:00689900 ; void __cdecl player_training_activate_equipment()
//.text:00689910 ; void __cdecl player_training_activate_flashlight()
//.text:00689960 ; void __cdecl player_training_activate_internal(int32)
//.text:006899C0 ; void __cdecl player_training_activate_jump()
//.text:006899D0 ; void __cdecl player_training_activate_stealth(void)
//.text:006899E0 ; void __cdecl player_training_device_touched(int32, int32)

void __cdecl player_training_dispose()
{
	INVOKE(0x00689A20, player_training_dispose);
}

void __cdecl player_training_dispose_from_old_map()
{
	INVOKE(0x00689A30, player_training_dispose_from_old_map);
}

//.text:00689A40 ; void __cdecl player_training_enable_scripted(bool)
//.text:00689A70 ; void __cdecl player_training_examine_action(int32, const player_action*)
//.text:00689B80 ; void __cdecl player_training_handle_saved_game(int32)

void __cdecl player_training_initialize()
{
	INVOKE(0x00689B90, player_training_initialize);
}

void __cdecl player_training_initialize_for_new_map()
{
	INVOKE(0x00689BF0, player_training_initialize_for_new_map);
}

void __cdecl player_training_notify_equipment_pickup(int32 player_index)
{
	INVOKE(0x00689C90, player_training_notify_equipment_pickup, player_index);
}

//.text:00689CD0 ; void __cdecl player_training_notify_grenade_pickup(int32)
//.text:00689D10 ; void __cdecl player_training_notify_kill(int32, int32, uns8)
//.text:00689E10 ; int32 __cdecl player_training_profile_data_get_completion_count(const s_player_training_profile_data*, int32)
//.text:00689E50 ; void __cdecl player_training_profile_data_set_completion_count(s_player_training_profile_data*, int32, int32)
//.text:00689EB0 ; void __cdecl player_training_profile_data_set_completion_count_all(s_player_training_profile_data*, bool)

void __cdecl player_training_render_screen(int32 user_index)
{
	//INVOKE(0x00689EF0, player_training_render_screen, output_user_index);
}

//.text:00689F00 ; void __cdecl player_training_save_changes_to_profiles(e_training_data_save)
//.text:0068A050 ; void __cdecl player_training_set_scripted_training_text(int32)
//.text:0068A080 ; void __cdecl player_training_show_scripted_training_text(bool)
//.text:0068A0B0 ; void __cdecl player_training_unit_submitted_action(int32, int32, int32, bool)
//.text:0068A100 ; void __cdecl player_training_update_player(int32)
//.text:0068A810 ; void __cdecl player_training_update_players_from_profiles()
//.text:0068AA30 ; 
//.text:0068AA70 ; public: void __cdecl c_recent_actions::submit_action(int32, bool)
//.text:0068AAD0 ; bool __cdecl training_can_be_satisfied_when_not_triggered(int32, int32)
//.text:0068AB20 ; bool __cdecl training_can_override_earlier_training(int32, int32)
//.text:0068AB60 ; int32 __cdecl training_get_completion_count(int32, int32)
//.text:0068ABD0 ; bool __cdecl training_is_completed(int32, int32)
//.text:0068AC40 ; bool __cdecl training_is_satisfied(int32, int32)
//.text:0068B4C0 ; bool __cdecl training_is_triggerable(int32, int32)
//.text:0068C110 ; bool __cdecl training_marked_as_triggerable(int32, int32)
//.text:0068C160 ; bool __cdecl unit_in_driving_seat(int32, int32, bool)

