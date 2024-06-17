#include "interface/chud/chud.hpp"

REFERENCE_DECLARE(0x05257C40, s_chud_globals_definition*, chud_globals);

//.text:00A88640 ; 
//.text:00A88660 ; 
//.text:00A88690 ; 
//.text:00A888D0 ; 
//.text:00A88970 ; 
//.text:00A889B0 ; 
//.text:00A889F0 ; 
//.text:00A88A30 ; 
//.text:00A88A70 ; 
//.text:00A88AB0 ; 
//.text:00A88AF0 ; 
//.text:00A88B30 ; void __cdecl chud_apply_distortion(e_output_user_index, long, long)
//.text:00A88BE0 ; long __cdecl chud_compute_unit_backpack_weapon_index(long)
//.text:00A88D00 ; 
//.text:00A88D50 ; 
//.text:00A88D60 ; 
//.text:00A88D70 ; void __cdecl chud_dispose()
//.text:00A88D80 ; void __cdecl chud_dispose_from_old_map()
//.text:00A88DA0 ; void __cdecl chud_draw_screen(e_output_user_index)
//.text:00A88FE0 ; 
//.text:00A89100 ; void __cdecl chud_draw_screen_saved_film(e_output_user_index)
//.text:00A89250 ; void __cdecl chud_draw_turbulence(e_output_user_index)

void __cdecl chud_game_tick()
{
	INVOKE(0x00A89420, chud_game_tick);
}

//.text:00A89440 ; 
//.text:00A895C0 ; long __cdecl chud_find_active_skin_index(long)
//.text:00A89630 ; 
//.text:00A89670 ; s_player_navpoint_data* __cdecl chud_get_campaign_navpoint_data(long)
//.text:00A896A0 ; 
//.text:00A896C0 ; 
//.text:00A89720 ; void __cdecl chud_handle_tag_changes()
//.text:00A89770 ; 
//.text:00A89790 ; void __cdecl chud_initialize()
//.text:00A897A0 ; void __cdecl chud_initialize_for_new_map()
//.text:00A89820 ; 
//.text:00A89860 ; void __cdecl chud_motion_sensor_invalidate(e_output_user_index)
//.text:00A898A0 ; bool __cdecl chud_motion_sensor_object_velocity_exceeds_threshold(long)
//.text:00A898B0 ; 
//.text:00A898F0 ; 
//.text:00A89940 ; 
//.text:00A89980 ; void __cdecl chud_picked_up_ammunition(long, long, short)
//.text:00A899D0 ; 
//.text:00A89A20 ; void __cdecl chud_picked_up_survival_mode_lives(e_campaign_team, long)
//.text:00A89A70 ; 
//.text:00A89BB0 ; 
//.text:00A89BD0 ; 
//.text:00A89C00 ; void __cdecl chud_scripting_bonus_round_set_timer(real)
//.text:00A89C40 ; void __cdecl chud_scripting_bonus_round_show_timer(bool)
//.text:00A89C70 ; void __cdecl chud_scripting_bonus_round_start_timer(bool)
//.text:00A89CA0 ; 
//.text:00A89D70 ; 
//.text:00A89DC0 ; 
//.text:00A89E20 ; 
//.text:00A89E70 ; 
//.text:00A89ED0 ; void __cdecl chud_scripting_set_fade(real, real)
//.text:00A89F50 ; 
//.text:00A89F80 ; 
//.text:00A89FC0 ; 
//.text:00A8A000 ; 
//.text:00A8A040 ; 
//.text:00A8A080 ; 
//.text:00A8A0C0 ; 
//.text:00A8A100 ; 
//.text:00A8A150 ; 
//.text:00A8A190 ; 
//.text:00A8A1D0 ; 
//.text:00A8A210 ; 
//.text:00A8A250 ; 
//.text:00A8A290 ; 
//.text:00A8A310 ; void __cdecl chud_set_campaign_objective(wchar_t const*, real)
//.text:00A8A3C0 ; 
//.text:00A8A410 ; 
//.text:00A8A4A0 ; void __cdecl chud_set_player_training_text(e_output_user_index, wchar_t const*)
//.text:00A8A500 ; 
//.text:00A8A570 ; s_chud_shared_persistent_user_data* __cdecl chud_shared_persistent_user_data_get(e_output_user_index)
//.text:00A8A5A0 ; bool __cdecl game_engine_hud_should_render_motion_sensor(e_output_user_index)
//.text:00A8A6D0 ; 
//.text:00A8A730 ; 
//.text:00A8A7A0 ; 
//.text:00A8A7F0 ; 
//.text:00A8A830 ; 
//.text:00A8A860 ; 
//.text:00A8A900 ; void __cdecl chud_start_directional_damage(long, vector3d const*, real, short, real, real, real, real_argb_color*)

//void __cdecl chud_submit_navpoint(e_output_user_index user_index, c_chud_navpoint const* navpoint)
void __cdecl chud_submit_navpoint(long user_index, s_chud_navpoint* navpoint)
{
	INVOKE(0x00A8A9F0, chud_submit_navpoint, user_index, navpoint);
}

//.text:00A8AA30 ; 
//.text:00A8AA90 ; 

void __cdecl chud_update(real world_seconds_elapsed)
{
	INVOKE(0x00A8AAE0, chud_update, world_seconds_elapsed);

	//if (chud_enabled && chud_globals)
	//	c_chud_manager::update(world_seconds_elapsed);
}

//.text:00A8AB10 ; void __cdecl chud_user_switched_grenades(long, long)
//.text:00A8AB70 ; 
//.text:00A8ABD0 ; 
//.text:00A8AD70 ; 
//.text:00A8AED0 ; 
//.text:00A8B260 ; 
//.text:00A8B290 ; 
//.text:00A8B2B0 ; 

