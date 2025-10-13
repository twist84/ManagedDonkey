#include "units/unit_scripting.hpp"

#include "cseries/cseries.hpp"

//.text:00B45B00 ; void __cdecl unit_scripting_kill(int32)
//.text:00B45BB0 ; void __cdecl unit_scripting_kill_silent(int32)
//.text:00BB1C00 ; 
//.text:00BB1C20 ; bool __cdecl unit_custom_animation_at_frame(int32, int32, int32, bool, int16)
//.text:00BB1CB0 ; bool __cdecl unit_custom_animation_relative_at_frame(int32, int32, int32, int32, bool, int16)
//.text:00BB1D40 ; bool __cdecl unit_get_custom_animation_root_velocity(int32, real_vector3d*)
//.text:00BB1E50 ; int16 __cdecl unit_get_custom_animation_scripting_ticks_remaining(int32)
//.text:00BB1EB0 ; real64 __cdecl unit_get_custom_animation_time_remaining(int32)
//.text:00BB1F00 ; bool __cdecl unit_is_playing_custom_animation(int32)
//.text:00BB1F20 ; void __cdecl unit_play_random_ping(int32)
//.text:00BB2140 ; void __cdecl unit_scripting_actively_controlled(int32, bool)
//.text:00BB2190 ; void __cdecl unit_scripting_aim_without_turning(int32, bool)
//.text:00BB21E0 ; void __cdecl unit_scripting_board_vehicle(int32, int32)
//.text:00BB2280 ; void __cdecl unit_scripting_can_blink(int32, bool)
//.text:00BB22D0 ; void __cdecl unit_scripting_close(int32)
//.text:00BB22F0 ; void __cdecl unit_scripting_doesnt_drop_items(int32)
//.text:00BB2350 ; void __cdecl unit_scripting_drop_support_weapon(int32)
//.text:00BB23C0 ; void __cdecl unit_scripting_enable_vision_mode(int32, bool)
//.text:00BB2410 ; void __cdecl unit_scripting_enter_vehicle(int32, int32, int32)
//.text:00BB2550 ; void __cdecl unit_scripting_exit_vehicle2(int32, int16)
//.text:00BB2650 ; void __cdecl unit_scripting_exit_vehicle(int32)
//.text:00BB26C0 ; void __cdecl unit_scripting_force_reload(int32)
//.text:00BB2720 ; bool __cdecl unit_scripting_get_enterable_by_player(int32)
//.text:00BB2760 ; int16 __cdecl unit_scripting_get_grenade_count(int32)
//.text:00BB27B0 ; real64 __cdecl unit_scripting_get_health(int32)
//.text:00BB27C0 ; real64 __cdecl unit_scripting_get_shield(int32)
//.text:00BB27D0 ; bool __cdecl unit_scripting_has_any_equipment(int32)
//.text:00BB2800 ; bool __cdecl unit_scripting_has_equipment(int32, int32)
//.text:00BB2870 ; bool __cdecl unit_scripting_has_weapon(int32, int32)
//.text:00BB28A0 ; bool __cdecl unit_scripting_has_weapon_readied(int32, int32)
//.text:00BB2950 ; void __cdecl unit_scripting_impervious(int32, bool)
//.text:00BB29C0 ; bool __cdecl unit_scripting_in_vehicle(int32)
//.text:00BB2A60 ; bool __cdecl unit_scripting_is_emitting(int32)

void __cdecl unit_scripting_limit_lipsync_to_mouth_only(int32 unit_index, bool limit)
{
	INVOKE(0x00BB2AB0, unit_scripting_limit_lipsync_to_mouth_only, unit_index, limit);
}

//.text:00BB2B00 ; void __cdecl unit_scripting_lower_weapon(int32, int16)
//.text:00BB2B30 ; void __cdecl unit_scripting_magic_melee_attack()
//.text:00BB2B80 ; int32 __cdecl unit_scripting_object_get_turret(int32, int16)
//.text:00BB2C20 ; int16 __cdecl unit_scripting_object_get_turret_count(int32)
//.text:00BB2CB0 ; void __cdecl unit_scripting_open(int32)
//.text:00BB2CF0 ; void __cdecl unit_scripting_player_lock_gaze(int32, int32, real32)
//.text:00BB2D30 ; void __cdecl unit_scripting_player_unlock_gaze(int32)
//.text:00BB2D60 ; void __cdecl unit_scripting_raise_weapon(int32, int16)
//.text:00BB2D90 ; void __cdecl unit_scripting_set_active_camo(int32, bool, real32)
//.text:00BB2DD0 ; void __cdecl unit_scripting_set_animation_mode(int32, int32)
//.text:00BB2E10 ; void __cdecl unit_scripting_set_current_vitality(int32, real32, real32)
//.text:00BB2F20 ; void __cdecl unit_scripting_set_emotion(int32, int32, real32, real32)
//.text:00BB2F60 ; void __cdecl unit_scripting_set_emotion_by_name(int32, int32, real32, real32)
//.text:00BB2FA0 ; void __cdecl unit_scripting_set_enterable_by_player(int32, bool)
//.text:00BB3000 ; unit_set_seat_enterable_by_player?
//.text:00BB30C0 ; void __cdecl unit_scripting_set_integrated_flashlight(int32, bool)
//.text:00BB3180 ; void __cdecl unit_scripting_set_maximum_vitality(int32, real32, real32)
//.text:00BB31D0 ; void __cdecl unit_scripting_set_only_takes_damage_from_players_team_bit(int32, bool)
//.text:00BB3220 ; void __cdecl unit_scripting_set_voice(int32, int32)
//.text:00BB3260 ; void __cdecl unit_scripting_spew_action(int32)
//.text:00BB3280 ; bool __cdecl unit_scripting_start_user_animation_list(int32, int32, int32, bool)

void __cdecl unit_scripting_stop_custom_animation(int32 unit_index)
{
	INVOKE(0x00BB3340, unit_scripting_stop_custom_animation, unit_index);
}

//.text:00BB3360 ; void __cdecl unit_scripting_suspended(int32, bool)
//.text:00BB33D0 ; int32 __cdecl unit_scripting_unit_driver(int32)
//.text:00BB3400 ; int32 __cdecl unit_scripting_unit_gunner(int32)
//.text:00BB3430 ; bool __cdecl unit_scripting_unit_in_vehicle_type(int32, int32)
//.text:00BB3490 ; int32 __cdecl unit_scripting_unit_riders(int32)
//.text:00BB3530 ; bool __cdecl unit_scripting_vehicle_test_seat(int32, int32)
//.text:00BB3570 ; bool __cdecl unit_scripting_vehicle_test_seat_unit(int32, int32, int32)
//.text:00BB3620 ; bool __cdecl unit_scripting_vehicle_test_seat_list(int32, int32, int32)
//.text:00BB3730 ; bool __cdecl unit_start_user_animation(int32, int32, int32, bool)
//.text:00BB3790 ; bool __cdecl unit_start_user_animation_loop(int32, int32, int32, bool)
//.text:00BB37F0 ; bool __cdecl unit_start_user_animation_relative(int32, int32, int32, bool, int32)
//.text:00BB3850 ; bool __cdecl unit_start_user_animation_relative_loop(int32, int32, int32, bool, int32)
//.text:00BB38B0 ; void __cdecl units_scripting_set_current_vitality(int32, real32, real32)
//.text:00BB3A00 ; void __cdecl units_scripting_set_maximum_vitality(int32, real32, real32)
//.text:00BB3AA0 ; int16 __cdecl vehicle_scripting_load_magic(int32, int32, int32)
//.text:00BB3C70 ; int16 __cdecl vehicle_scripting_unload(int32, int32)
//.text:00BB3D20 ; 

