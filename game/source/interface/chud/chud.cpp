#include "interface/chud/chud.hpp"

#include "camera/director.hpp"
#include "cutscene/cinematics.hpp"
#include "game/cheats.hpp"
#include "interface/chud/chud_draw.hpp"
#include "interface/gui_screens/scoreboard/gui_screen_scoreboard.hpp"
#include "items/weapons.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
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

//.text:00A85100 ; tls
//.text:00A85120 ; tls
//.text:00A85140 ; tls
//.text:00A85160 ; tls
//.text:00A85180 ; tls
//.text:00A851A0 ; tls
//.text:00A851C0 ; public: c_chud_cortana_effect::c_chud_cortana_effect()
//.text:00A85240 ; public: c_chud_update_user_data::c_chud_update_user_data(int32 user_index, int32 previous_unit_index, bool force_initialization)
//.text:00A87ED0 ; c_chud_update_user_data::s_unit_state::s_unit_state
//.text:00A87EF0 ; tls
//.text:00A87F00 ; tls
//.text:00A87F10 ; tls
//.text:00A87F20 ; tls
//.text:00A87F30 ; tls
//.text:00A87F40 ; tls
//.text:00A87F50 ; 
//.text:00A87F60 ; 
//.text:00A87F80 ; 
//.text:00A87FA0 ; 
//.text:00A88540 ; tls
//.text:00A88560 ; tls
//.text:00A88580 ; tls
//.text:00A885B0 ; tls
//.text:00A885E0 ; tls
//.text:00A88610 ; tls
//.text:00A88640 ; public: void c_chud_persistent_user_data::active_chud_definition_notify_widget_created(int32 active_definition_num)
//.text:00A88660 ; public: void c_chud_persistent_user_data::active_chud_definition_notify_widget_deleted(int32 active_definition_num)
//.text:00A88690 ; public: int32 c_chud_persistent_user_data::add_active_chud_definition_index(int32 chud_definition_index, int32 weapon_index, bool* full_delete_happened)
//.text:00A888D0 ; public: void c_chud_equipment_effect_manager::add_noisemaker_zone(int32 datum_index, const real_point3d* position, real32 blip_radius, real32 flash_radius, int32 blip_count)
//.text:00A88970 ; tls
//.text:00A889B0 ; tls
//.text:00A889F0 ; tls
//.text:00A88A30 ; tls
//.text:00A88A70 ; tls
//.text:00A88AB0 ; tls
//.text:00A88AF0 ; void __cdecl chud_add_noisemaker_zone(int32 datum_index, const real_point3d* position, real32 blip_radius, real32 flash_radius, int32 blip_count)
//.text:00A88B30 ; void __cdecl chud_apply_distortion(int32 user_index, int32 target_width, int32 target_height)
//.text:00A88BE0 ; int32 __cdecl chud_compute_unit_backpack_weapon_index(int32 unit_index)
//.text:00A88D00 ; // chud_c `special_hud_version` related
//.text:00A88D50 ; void __cdecl chud_cortana_set_range_multiplier(real32 multiplier)
//.text:00A88D60 ; void __cdecl chud_cortana_suck_set(int32 object_index, int32 marker_id, bool enabled)

void __cdecl chud_dispose()
{
	INVOKE(0x00A88D70, chud_dispose);

	//cortana_effect_dispose();
}

void __cdecl chud_dispose_from_old_map()
{
	INVOKE(0x00A88D80, chud_dispose_from_old_map);

	//c_chud_manager::dispose_from_old_map();
	//cortana_effect_dispose_from_old_map();
	//chud_globals = NULL;
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
	//	VALID_INDEX(user_index, k_number_of_users))
	//{
	//	if (chud_draw_begin(user_index, 1.0f, false, true))
	//	{
	//		s_chud_runtime_widget_datum* widget = NULL;
	//		for (int32 widget_index = chud_shared_persistent_user_data_get(user_index)->first_drawn_saved_film_widget_index;
	//			widget_index != NONE;
	//			widget_index = widget->next_drawn_widget_index)
	//		{
	//			widget = DATUM_GET(*g_chud_manager_user_widget_data, s_chud_runtime_widget_datum, widget_index);
	//			int32 chud_definition_index = g_chud_manager_persistent_user_data->m_persistent_definitions[widget->runtime_hud_num].chud_definition_index;
	//			chud_draw_widget(user_index, widget, chud_definition_index, 0);
	//		}
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

	//if (chud_enabled && chud_globals)
	//{
	//	c_chud_manager::game_tick();
	//}
	//cortana_effect_update();
}

bool __cdecl chud_generate_damage_flash_texture(int32 user_index)
{
	return INVOKE(0x00A89440, chud_generate_damage_flash_texture, user_index);
}

//.text:00A895C0 ; int32 __cdecl chud_find_active_skin_index(int32 user_index)
//.text:00A89630 ; e_chud_skin_type __cdecl chud_get_active_skin_type(int32 user_index)
//.text:00A89670 ; s_player_navpoint_data* __cdecl chud_get_campaign_navpoint_data(int32)
//.text:00A896A0 ; int32 __cdecl chud_get_grunt_birthday_effect()
//.text:00A896C0 ; void __cdecl chud_get_widget_collection_and_definition(int32 chud_definition_index, int32 collection_index, int32 widget_index, s_chud_widget_collection** widget_collection, s_chud_widget_base** widget_definition, bool* is_text_widget)
//.text:00A89720 ; void __cdecl chud_handle_tag_changes()
//.text:00A89770 ; bool __cdecl chud_in_first_person(int32 user_index)

void __cdecl chud_initialize()
{
	INVOKE(0x00A89790, chud_initialize);
}

void __cdecl chud_initialize_for_new_map()
{
	INVOKE(0x00A897A0, chud_initialize_for_new_map);
}

//.text:00A89820 ; bool __cdecl chud_motion_sensor_enemies_nearby(int32 user_index)
//.text:00A89860 ; void __cdecl chud_motion_sensor_invalidate(int32 user_index)
//.text:00A898A0 ; bool __cdecl chud_motion_sensor_object_velocity_exceeds_threshold(int32 object_index)
//.text:00A898B0 ; bool __cdecl chud_motion_sensor_slow_moving_boardable_enemy_vehicles_nearby(int32 user_index)
//.text:00A898F0 ; 
//.text:00A89940 ; 
//.text:00A89980 ; void __cdecl chud_picked_up_ammunition(int32 user_index, int32 weapon_definition_index, int16 count)
//.text:00A899D0 ; void __cdecl chud_picked_up_grenade(int32 user_index, int32 grenade_definition_index)
//.text:00A89A20 ; void __cdecl chud_picked_up_survival_mode_lives(int32)
//.text:00A89A70 ; void __cdecl chud_play_sound(int32 user_index, int32 type_flags, const s_tag_block* sounds, int32* sound_handles, uint16* sound_flags)
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
//.text:00A89F80 ; void __cdecl chud_scripting_show_compass(bool show)
//.text:00A89FC0 ; void __cdecl chud_scripting_show_consumables(bool show)
//.text:00A8A000 ; void __cdecl chud_scripting_show_crosshair(bool show)
//.text:00A8A040 ; void __cdecl chud_scripting_show_energy_meters(bool show)
//.text:00A8A080 ; void __cdecl chud_scripting_show_fire_grenades(bool show)
//.text:00A8A0C0 ; void __cdecl chud_scripting_show_grenades(bool show)
//.text:00A8A100 ; void __cdecl chud_scripting_show_messages(bool show)
//.text:00A8A150 ; void __cdecl chud_scripting_show_motion_sensor(bool show)
//.text:00A8A190 ; void __cdecl chud_scripting_show_shield(bool show)
//.text:00A8A1D0 ; void __cdecl chud_scripting_show_spike_grenades(bool show)
//.text:00A8A210 ; void __cdecl chud_scripting_show_stamina(bool show)
//.text:00A8A250 ; void __cdecl chud_scripting_show_weapon_stats(bool show)
//.text:00A8A290 ; 
//.text:00A8A310 ; void __cdecl chud_set_campaign_objective(const wchar_t* text, real32 up_time_seconds)
//.text:00A8A3C0 ; 
//.text:00A8A410 ; 
//.text:00A8A4A0 ; void __cdecl chud_set_player_training_text(int32 user_index, const wchar_t* text)
//.text:00A8A500 ; void __cdecl chud_set_player_special_hud_version(int32 user_index);

s_chud_shared_persistent_user_data* __cdecl chud_shared_persistent_user_data_get(int32 user_index)
{
	return INVOKE(0x00A8A570, chud_shared_persistent_user_data_get, user_index);
}

//.text:00A8A5A0 ; bool __cdecl chud_should_draw_motion_sensor(int32 user_index)
//.text:00A8A6D0 ; bool __cdecl chud_should_draw_navpoints(int32 user_index)
//.text:00A8A730 ; bool __cdecl chud_should_draw_minimap(int32 user_index)
//.text:00A8A7A0 ; bool __cdecl chud_should_draw_screen_normal(int32 user_index)

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

void __cdecl chud_submit_navpoint(int32 user_index, const s_chud_navpoint* navpoint)
{
	INVOKE(0x00A8A9F0, chud_submit_navpoint, user_index, navpoint);
}

//.text:00A8AA30 ; void __cdecl chud_tick_shield(int32 player_index, real32 amount)
//.text:00A8AA90 ; void __cdecl chud_trigger_forced_initialization()

void __cdecl chud_update(real32 world_seconds_elapsed)
{
	INVOKE(0x00A8AAE0, chud_update, world_seconds_elapsed);

	//if (chud_enabled && chud_globals)
	//	c_chud_manager::update(world_seconds_elapsed);
}

//.text:00A8AB10 ; void __cdecl chud_user_switched_grenades(int32 user_index, int32 grenade_slot_index)
//.text:00A8AB70 ; void __cdecl chud_user_tried_banned_vehicle_entrance(int32 user_index)
//.text:00A8ABC0 ; 
//.text:00A8ABD0 ; public: long c_chud_update_user_data::compute_actual_widget_state(int32 chud_definition_index, int32 collection_index, int32 widget_index, int32 weapon_index, int32 desired_widget_state, int32 current_widget_state, int32 current_widget_timer)
//.text:00A8AD70 ; public: long c_chud_update_user_data::compute_desired_widget_state(int32 user_index, int32 chud_definition_index, int32 collection_index, int32 widget_index, int32 weapon_index, bool* hidden)
//.text:00A8AED0 ; protected: void c_chud_update_user_data::compute_weapon_update(int32 weapon_index, int32 chud_definition_type, const s_aim_assist_targeting_result* aim_assist_targeting)
//.text:00A8B260 ; 
//.text:00A8B290 ; 
//.text:00A8B2B0 ; tls
//.text:00A8B2E0 ; tls
//.text:00A8B310 ; tls
//.text:00A8B340 ; tls
//.text:00A8B370 ; tls
//.text:00A8B3A0 ; tls
//.text:00A8B3D0 ; public: static void __cdecl c_chud_manager::dispose()
//.text:00A8B3E0 ; public: static void __cdecl c_chud_manager::dispose_from_old_map()
//.text:00A8B490 ; 
//.text:00A8B4A0 ; public: real32 c_chud_update_user_data::evaluate_external_input(int32 external_input_type, int32 weapon_index, int32 widget_anchor_type, real32 previous_value) // real64
//.text:00A8C1E0 ; protected: c_chud_update_user_data::s_chud_definition_info::s_chud_definition_weapon_state* c_chud_update_user_data::find_weapon_state(int32 weapon_index)
//.text:00A8C220 ; 
//.text:00A8C230 ; tls
//.text:00A8C260 ; tls
//.text:00A8C290 ; tls
//.text:00A8C2C0 ; tls
//.text:00A8C2F0 ; tls
//.text:00A8C320 ; tls
//.text:00A8C350 ; public: void c_chud_equipment_effect_manager::game_tick()
//.text:00A8C3A0 ; public: static void __cdecl c_chud_manager::game_tick()
//.text:00A8C500 ; public: static c_chud_equipment_effect_manager* __cdecl c_chud_equipment_effect_manager::get()
//.text:00A8C520 ; public: static c_chud_impulse_manager* __cdecl c_chud_impulse_manager::get(int32 user_index)
//.text:00A8C550 ; public: static c_chud_messaging_manager* __cdecl c_chud_messaging_manager::get(int32 user_index)
//.text:00A8C580 ; public: static c_chud_navpoint_manager* __cdecl c_chud_navpoint_manager::get(int32 user_index)
//.text:00A8C5B0 ; 
//.text:00A8C5D0 ; public: int32 c_chud_persistent_user_data::get_active_chud_definition_index(int32 chud_definition_index, int32 weapon_index) const
//.text:00A8C600 ; // c_chud_update_user_data:: `consumable_cooldown_meter` related
//.text:00A8C610 ; public: int32 c_chud_impulse_manager::get_ammo_pickup_count(int32)
//.text:00A8C660 ; 
//.text:00A8C670 ; 
//.text:00A8C680 ; public: c_chud_update_user_data::s_chud_definition_info* c_chud_update_user_data::get_definition_info(int32 index)
//.text:00A8C6A0 ; 
//.text:00A8C6B0 ; 
//.text:00A8C6C0 ; 
//.text:00A8C6D0 ; public: bool c_chud_update_user_data::get_fresh_unit()
//.text:00A8C6E0 ; public: game_engine_interface_state* c_chud_update_user_data::get_game_engine_interface_state()
//.text:00A8C6F0 ; public: int32 c_chud_impulse_manager::get_grenade_pickup_count(int32 grenade_index) const
//.text:00A8C700 ; public: bool c_chud_update_user_data::get_hologram_target_available() const
//.text:00A8C710 ; public: void c_chud_update_user_data::get_hologram_target_point(real_point3d* hologram_target_point)
//.text:00A8C730 ; 
//.text:00A8C750 ; 
//.text:00A8C760 ; 
//.text:00A8C770 ; public: s_campaign_metagame_interface_state* c_chud_update_user_data::get_metagame_interface_state()
//.text:00A8C780 ; public: int32 c_chud_update_user_data::get_next_definition_info_index(int32 index) const
//.text:00A8C7B0 ; private: int32 c_chud_directional_damage::get_oldest_index()
//.text:00A8C800 ; public: s_saved_film_hud_interface_state* c_chud_update_user_data::get_saved_film_interface_state()
//.text:00A8C810 ; public: uns32 c_chud_update_user_data::get_sound_flags()
//.text:00A8CE20 ; 
//.text:00A8CE60 ; 
//.text:00A8CE70 ; 
//.text:00A8CE80 ; 
//.text:00A8CE90 ; 
//.text:00A8CEA0 ; 
//.text:00A8CEB0 ; 
//.text:00A8CEC0 ; 
//.text:00A8CED0 ; 
//.text:00A8CEE0 ; public: static int32 __cdecl c_chud_persistent_user_data::get_widget_anchor_type(const s_chud_widget_base* widget_base, const s_chud_widget_collection* widget_collection)
//.text:00A8CF20 ; 
//.text:00A8CF30 ; private: int32 c_chud_equipment_effect_manager::get_zone_index(int32 datum_index) const
//.text:00A8CF70 ; 
//.text:00A8CFA0 ; 
//.text:00A8CFB0 ; 
//.text:00A8CFE0 ; public: void c_chud_equipment_effect_manager::initialize()
//.text:00A8D040 ; public: static void __cdecl c_chud_manager::initialize()
//.text:00A8D1E0 ; public: void c_chud_persistent_global_data::initialize()
//.text:00A8D240 ; public: static void __cdecl c_chud_manager::initialize_for_new_map()
//.text:00A8D330 ; public: void c_chud_persistent_user_data::initialize(int32 user_index)
//.text:00A8D3D0 ; public: void c_chud_scripting::initialize_for_new_map()
//.text:00A8D440 ; public: void c_chud_persistent_user_data::initialize_sounds(int32 user_index)
//.text:00A8D470 ; public: bool c_chud_update_user_data::is_dual_wielding() const
//.text:00A8D480 ; 
//.text:00A8D490 ; public: bool c_chud_update_user_data::is_zoomed_and_targeting() const
//.text:00A8D4B0 ; public: bool c_chud_update_user_data::lock_target_available(real_point3d* lock_target)
//.text:00A8D530 ; int32 __cdecl map_quality_value_to_shader_value(e_network_rough_quality quality)
//.text:00A8D570 ; public: void c_chud_equipment_effect_manager::noisemaker_zone_calculate_blip(int32 zone_index, int32 blip_index, real_point3d* blip_position)
//.text:00A8D7A0 ; public: int32 c_chud_equipment_effect_manager::noisemaker_zone_get_blip_count(int32 zone_index)
//.text:00A8D7C0 ; public: void c_chud_equipment_effect_manager::noisemaker_zone_make_fake_directional_damage(int32 user_index, int32 zone_index, c_chud_directional_damage* directional_damage)
//.text:00A8DA00 ; public: void c_chud_impulse_manager::picked_up_ammunition(int32 weapon_definition_index, int16 count)
//.text:00A8DA50 ; public: void c_chud_impulse_manager::picked_up_grenade(int32 grenade_definition_index)
//.text:00A8DAB0 ; 
//.text:00A8DAC0 ; 
//.text:00A8DAD0 ; tls
//.text:00A8DB10 ; tls
//.text:00A8DB50 ; tls
//.text:00A8DB90 ; tls
//.text:00A8DBD0 ; tls
//.text:00A8DC10 ; tls
//.text:00A8DC50 ; public: void c_chud_cinematic_fade::reset()
//.text:00A8DC70 ; public: void c_chud_cortana_effect::reset()
//.text:00A8DCD0 ; public: void c_chud_directional_damage::reset()
//.text:00A8DD20 ; public: void c_chud_impulse_manager::reset()
//.text:00A8DD60 ; public: void c_chud_persistent_user_data::reset()
//.text:00A8DF40 ; 
//.text:00A8DF90 ; public: void c_chud_persistent_user_data::reset_sounds(int32 user_index)
//.text:00A8DFF0 ; 
//.text:00A8E000 ; 
//.text:00A8E050 ; 
//.text:00A8E090 ; 
//.text:00A8E0D0 ; 
//.text:00A8E0F0 ; public: int32 c_chud_equipment_effect_manager::sensor_get_noisemaker_zone(const real_point3d* sensor_origin, bool use_flash_radius)
//.text:00A8E1A0 ; 
//.text:00A8E1C0 ; 
//.text:00A8E200 ; 
//.text:00A8E270 ; 
//.text:00A8E280 ; 
//.text:00A8E2A0 ; 
//.text:00A8E2B0 ; 
//.text:00A8E2C0 ; 
//.text:00A8E2E0 ; 
//.text:00A8E300 ; 
//.text:00A8E320 ; 
//.text:00A8E340 ; 
//.text:00A8E360 ; 
//.text:00A8E380 ; 
//.text:00A8E3A0 ; 
//.text:00A8E3C0 ; 
//.text:00A8E3E0 ; 
//.text:00A8E400 ; 
//.text:00A8E420 ; 
//.text:00A8E440 ; 
//.text:00A8E460 ; 
//.text:00A8E6B0 ; private: bool c_chud_directional_damage::something_to_draw(int32 user_index)
//.text:00A8E700 ; get `fov_multiplier`
//.text:00A8E740 ; public: void c_chud_directional_damage::start(const real_vector3d* direction, real32 duration, int16 fade_function, real32 size, real32 inner_scale, real32 outer_scale, real_argb_color* color)
//.text:00A8E810 ; protected: void c_chud_update_user_data::study_autoaim(int32 unit_index)
//.text:00A8EA70 ; 
//.text:00A8EA90 ; 
//.text:00A8EAB0 ; 
//.text:00A8EB20 ; public: void c_chud_cortana_effect::update(int32 user_index)
//.text:00A8ED40 ; public: void c_chud_directional_damage::update(int32 user_index, real32 dt)
//.text:00A8F080 ; public: void c_chud_equipment_effect_manager::update()
//.text:00A8F0F0 ; public: void c_chud_damage_tracker::update(int32 user_index)
//.text:00A8F390 ; public: static void __cdecl c_chud_manager::update(real32 dt)
//.text:00A8F400 ; 
//.text:00A8F4A0 ; public: void c_chud_persistent_global_data::update()
//.text:00A8F520 ; public: void c_chud_persistent_user_data::update(int32 user_index, real32 dt)
//.text:00A901E0 ; public: void c_chud_persistent_user_data::update_from_update_data(int32 user_index, c_chud_update_user_data* user_data)
//.text:00A90D70 ; public: void c_chud_persistent_user_data::update_sounds(int32 user_index, c_chud_update_user_data* user_data)
//.text:00A90DE0 ; public: void c_chud_persistent_user_data::update_widget_external_inputs(int32 user_index, s_chud_runtime_widget_datum* widget, c_chud_update_user_data* update_user_data)
//.text:00A90EF0 ; 
//.text:00A90F00 ; 
//.text:00A90F10 ; public: bool c_chud_update_user_data::widget_definition_should_be_active(const s_chud_widget_base* widget_definition, const s_chud_widget_collection* widget_collection, int32 weapon_index)
//.text:00A912C0 ; public: bool c_chud_update_user_data::widget_definition_should_be_flashing(const s_chud_widget_base* widget_definition, const s_chud_widget_collection* widget_collection, int32 weapon_index)
//.text:00A91390 ; public: bool c_chud_update_user_data::widget_definition_should_be_hidden(const s_chud_widget_base* widget_definition, const s_chud_widget_collection* widget_collection, int32 weapon_index)
//.text:00A91440 ; public: static bool __cdecl c_chud_update_user_data::widget_definition_should_be_impulse(int32 user_index, const s_chud_widget_base* widget_definition, const s_chud_widget_collection* widget_collection, int32 weapon_index, int32* impulse_value)
//.text:00A916D0 ; public: static bool __cdecl c_chud_update_user_data::widget_definition_type_is_impulse(const s_chud_widget_base* widget_definition, const s_chud_widget_collection* widget_collection, int32 weapon_index)

