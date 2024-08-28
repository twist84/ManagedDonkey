#include "game/player_training.hpp"

//.text:00689550 ; 
//.text:00689570 ; 
//.text:00689580 ; void __cdecl __tls_set_g_player_training_globals_allocator(void*)
//.text:006895A0 ; 
//.text:006895D0 ; public: bool __cdecl s_player_training_profile_data::add_training_data(s_player_training_profile_data const*)
//.text:00689640 ; 
//.text:00689680 ; public: void __cdecl c_hold_watcher::check_action(e_input_user_index, s_player_action const*)
//.text:006896F0 ; 
//.text:00689720 ; public: bool __cdecl c_recent_actions::did_player_recently_perform_action(long, long)
//.text:00689770 ; 
//.text:006897A0 ; 
//.text:006897D0 ; 
//.text:00689800 ; bool __cdecl increment_training_completion(long, s_player_training_user*, s_player_training_entry_data const*, s_player_training_state*)
//.text:006898F0 ; 
//.text:00689900 ; 
//.text:00689910 ; 
//.text:00689960 ; 
//.text:006899C0 ; 
//.text:006899D0 ; 
//.text:006899E0 ; void __cdecl player_training_device_touched(long, long)

void __cdecl player_training_dispose()
{
	INVOKE(0x00689A20, player_training_dispose);
}

void __cdecl player_training_dispose_from_old_map()
{
	INVOKE(0x00689A30, player_training_dispose_from_old_map);
}

//.text:00689A40 ; 
//.text:00689A70 ; void __cdecl player_training_examine_action(long, s_player_action const*)
//.text:00689B80 ; void __cdecl player_training_handle_saved_game(long)

void __cdecl player_training_initialize()
{
	INVOKE(0x00689B90, player_training_initialize);
}

void __cdecl player_training_initialize_for_new_map()
{
	INVOKE(0x00689BF0, player_training_initialize_for_new_map);
}

//.text:00689C90 ; void __cdecl player_training_notify_equipment_pickup(long)
//.text:00689CD0 ; void __cdecl player_training_notify_grenade_pickup(long)
//.text:00689D10 ; void __cdecl player_training_notify_kill(long, long, byte)
//.text:00689E10 ; 
//.text:00689E50 ; void __cdecl player_training_profile_data_set_completion_count(s_player_training_profile_data*, long, long)
//.text:00689EB0 ; 
//.text:00689EF0 ; void __cdecl player_training_render_screen(long)
//.text:00689F00 ; void __cdecl player_training_save_changes_to_profiles(e_training_data_save)
//.text:0068A050 ; 
//.text:0068A080 ; 
//.text:0068A0B0 ; void __cdecl player_training_unit_submitted_action(long, long, long, bool)
//.text:0068A100 ; void __cdecl player_training_update_player(long)
//.text:0068A810 ; void __cdecl player_training_update_players_from_profiles()
//.text:0068AA30 ; 
//.text:0068AA70 ; public: void __cdecl c_recent_actions::submit_action(long, bool)
//.text:0068AAD0 ; 
//.text:0068AB20 ; 
//.text:0068AB60 ; long __cdecl training_get_completion_count(long, long)
//.text:0068ABD0 ; bool __cdecl training_is_completed(e_input_user_index, long)
//.text:0068AC40 ; bool __cdecl training_is_satisfied(e_input_user_index, long)
//.text:0068B4C0 ; bool __cdecl training_is_triggerable(e_input_user_index, long)
//.text:0068C110 ; long __cdecl player_training_profile_data_get_completion_count(s_player_training_profile_data const*, long)
//.text:0068C160 ; bool __cdecl unit_in_driving_seat(long, long, bool)

