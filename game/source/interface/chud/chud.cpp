#include "interface/chud/chud.hpp"

#include "camera/director.hpp"
#include "cutscene/cinematics.hpp"
#include "game/cheats.hpp"
#include "interface/chud/chud_draw.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "items/weapons.hpp"
#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "rasterizer/rasterizer_profile.hpp"
#include "render/views/render_view.hpp"
#include "units/units.hpp"

REFERENCE_DECLARE(0x05257C40, s_chud_globals_definition*, chud_globals);

HOOK_DECLARE(0x00A88DA0, chud_draw_screen);
HOOK_DECLARE(0x00A89100, chud_draw_screen_saved_film);
HOOK_DECLARE(0x00A89250, chud_draw_turbulence);

HOOK_DECLARE_CLASS_MEMBER(0x00A8AED0, c_chud_update_user_data, compute_weapon_update_);

// bottomless clip hud symbol
void __thiscall c_chud_update_user_data::compute_weapon_update_(int32 weapon_index, int32 chud_definition_type, s_aim_assist_targeting_result* aim_assist_targeting)
{
	this;

	HOOK_INVOKE_CLASS_MEMBER(, c_chud_update_user_data, compute_weapon_update_, weapon_index, chud_definition_type, aim_assist_targeting);

	if (!cheat.bottomless_clip)
	{
		return;
	}

	weapon_datum* weapon = WEAPON_GET(weapon_index);
	if (!weapon || weapon->item.inventory_unit_index == NONE)
	{
		return;
	}

	unit_datum* unit = UNIT_GET(weapon->item.inventory_unit_index);
	if (!unit || unit->unit.player_index == NONE)
	{
		return;
	}

	s_chud_definition_info::s_chud_definition_weapon_state& weapon_state = m_incoming_definition_infos[chud_definition_type].weapon_state;
	weapon_state.battery = k_real_max;
	weapon_state.primary_clip_remaining = k_real_max;
	weapon_state.primary_clip_max = k_real_max;
	weapon_state.primary_total_remaining = k_real_max;
	weapon_state.primary_total_max = k_real_max;
}

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
//.text:00A88B30 ; void __cdecl chud_apply_distortion(int32, int32, int32)
//.text:00A88BE0 ; int32 __cdecl chud_compute_unit_backpack_weapon_index(int32)
//.text:00A88D00 ; 
//.text:00A88D50 ; 
//.text:00A88D60 ; 

void __cdecl chud_dispose()
{
	INVOKE(0x00A88D70, chud_dispose);
}

void __cdecl chud_dispose_from_old_map()
{
	INVOKE(0x00A88D80, chud_dispose_from_old_map);
}

void __cdecl chud_draw_screen(int32 user_index)
{
	//INVOKE(0x00A88DA0, chud_draw_screen, user_index);

	HOOK_INVOKE(, chud_draw_screen, user_index);
}

void __cdecl chud_draw_screen_LDR(int32 user_index)
{
	INVOKE(0x00A88FE0, chud_draw_screen_LDR, user_index);
}

void __cdecl chud_draw_screen_saved_film(int32 user_index)
{
	//INVOKE(0x00A89100, chud_draw_screen_saved_film, user_index);

	if (!chud_should_draw_screen_saved_film(user_index) && chud_enabled && chud_globals && VALID_INDEX(user_index, k_number_of_users))
	{
		return;
	}
	HOOK_INVOKE(, chud_draw_screen_saved_film, user_index);

	//if (chud_should_draw_screen_saved_film(user_index) &&
	//	chud_enabled && chud_globals &&
	//	VALID_INDEX(user_index, 4))
	//{
	//	if (chud_draw_begin(user_index, 1.0f, false, true))
	//	{
	//		// $IMPLEMENT
	//	}
	//	chud_draw_end(user_index, false);
	//}
	////cortana_effect_render_ui();
}

void __cdecl chud_draw_turbulence(int32 user_index)
{
	//INVOKE(0x00A89250, chud_draw_turbulence, user_index);

	HOOK_INVOKE(, chud_draw_turbulence, user_index);
}

void __cdecl chud_game_tick()
{
	INVOKE(0x00A89420, chud_game_tick);
}

bool __cdecl chud_generate_damage_flash_texture(int32 user_index)
{
	return INVOKE(0x00A89440, chud_generate_damage_flash_texture, user_index);
}

//.text:00A895C0 ; int32 __cdecl chud_find_active_skin_index(int32)
//.text:00A89630 ; 
//.text:00A89670 ; s_player_navpoint_data* __cdecl chud_get_campaign_navpoint_data(int32)
//.text:00A896A0 ; 
//.text:00A896C0 ; 
//.text:00A89720 ; void __cdecl chud_handle_tag_changes()
//.text:00A89770 ; 

void __cdecl chud_initialize()
{
	INVOKE(0x00A89790, chud_initialize);
}

void __cdecl chud_initialize_for_new_map()
{
	INVOKE(0x00A897A0, chud_initialize_for_new_map);
}

//.text:00A89820 ; 
//.text:00A89860 ; void __cdecl chud_motion_sensor_invalidate(int32)
//.text:00A898A0 ; bool __cdecl chud_motion_sensor_object_velocity_exceeds_threshold(int32)
//.text:00A898B0 ; 
//.text:00A898F0 ; 
//.text:00A89940 ; 
//.text:00A89980 ; void __cdecl chud_picked_up_ammunition(int32, int32, int16)
//.text:00A899D0 ; 
//.text:00A89A20 ; void __cdecl chud_picked_up_survival_mode_lives(e_game_team, int32)
//.text:00A89A70 ; 
//.text:00A89BB0 ; 
//.text:00A89BD0 ; 
//.text:00A89C00 ; void __cdecl chud_scripting_bonus_round_set_timer(real32)
//.text:00A89C40 ; void __cdecl chud_scripting_bonus_round_show_timer(bool)
//.text:00A89C70 ; void __cdecl chud_scripting_bonus_round_start_timer(bool)
//.text:00A89CA0 ; 
//.text:00A89D70 ; 
//.text:00A89DC0 ; 
//.text:00A89E20 ; 
//.text:00A89E70 ; 
//.text:00A89ED0 ; void __cdecl chud_scripting_set_fade(real32, real32)
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
//.text:00A8A310 ; void __cdecl chud_set_campaign_objective(const wchar_t*, real32)
//.text:00A8A3C0 ; 
//.text:00A8A410 ; 
//.text:00A8A4A0 ; void __cdecl chud_set_player_training_text(int32, const wchar_t*)
//.text:00A8A500 ; 
//.text:00A8A570 ; s_chud_shared_persistent_user_data* __cdecl chud_shared_persistent_user_data_get(int32)
//.text:00A8A5A0 ; bool __cdecl game_engine_hud_should_render_motion_sensor(int32)
//.text:00A8A6D0 ; 
//.text:00A8A730 ; 
//.text:00A8A7A0 ; 

bool __cdecl chud_should_draw_screen_saved_film(int32 user_index)
{
	//return INVOKE(0x00A8A7F0, chud_should_draw_screen_saved_film, user_index);

	if (user_index == NONE)
	{
		return false;
	}

	if (!director_get(user_index)->should_draw_hud_saved_film())
	{
		return false;
	}

	if (c_gui_screen_scoreboard::is_scoreboard_displayed(controller_index_from_user_index(user_index)))
	{
		return false;
	}

	if (c_gui_screen_scoreboard::is_scoreboard_displayed(k_any_controller))
	{
		return false;
	}

	if (cinematic_in_progress() && cinematic_is_letterbox())
	{
		return false;
	}

	return true;
}

//.text:00A8A830 ; 
//.text:00A8A860 ; 
//.text:00A8A900 ; void __cdecl chud_start_directional_damage(int32, const real_vector3d*, real32, int16, real32, real32, real32, real_argb_color*)

//void __cdecl chud_submit_navpoint(int32 user_index, const c_chud_navpoint* navpoint)
void __cdecl chud_submit_navpoint(int32 user_index, s_chud_navpoint* navpoint)
{
	INVOKE(0x00A8A9F0, chud_submit_navpoint, user_index, navpoint);
}

//.text:00A8AA30 ; 
//.text:00A8AA90 ; 

void __cdecl chud_update(real32 world_seconds_elapsed)
{
	INVOKE(0x00A8AAE0, chud_update, world_seconds_elapsed);

	//if (chud_enabled && chud_globals)
	//	c_chud_manager::update(world_seconds_elapsed);
}

//.text:00A8AB10 ; void __cdecl chud_user_switched_grenades(int32, int32)
//.text:00A8AB70 ; 
//.text:00A8ABD0 ; 
//.text:00A8AD70 ; 
//.text:00A8AED0 ; 
//.text:00A8B260 ; 
//.text:00A8B290 ; 
//.text:00A8B2B0 ; 

