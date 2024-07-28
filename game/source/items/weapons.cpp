#include "items/weapons.hpp"

#include "cache/cache_files.hpp"
#include "game/cheats.hpp"
#include "items/weapon_definitions.hpp"
#include "memory/module.hpp"

HOOK_DECLARE(0x00B61550, weapon_can_be_dual_wielded);
HOOK_DECLARE(0x00B62AC0, weapon_get_age);
HOOK_DECLARE(0x00B63C30, weapon_has_infinite_ammo);

//.text:00B5B120 ; 
//.text:00B5B130 ; 
//.text:00B5B150 ; 
//.text:00B5B160 ; 
//.text:00B5B180 ; 
//.text:00B5B1A0 ; 
//.text:00B5B3D0 ; 
//.text:00B5B460 ; 
//.text:00B5B470 ; 
//.text:00B5B480 ; 
//.text:00B5B490 ; 
//.text:00B5B4A0 ; 
//.text:00B5B4B0 ; 
//.text:00B5B4C0 ; 
//.text:00B5B4D0 ; double __cdecl indirect_fire_compute_arcing_fraction(long, float)
//.text:00B5B5B0 ; bool __cdecl indirect_fire_compute_ballistics(long, long, long, real_point3d const*, vector3d*, real*)
//.text:00B5B770 ; bool __cdecl indirect_fire_get_aim_target_position(long, real_point3d*)
//.text:00B5B7D0 ; 
//.text:00B5B7E0 ; 
//.text:00B5B8A0 ; 
//.text:00B5B8B0 ; 
//.text:00B5B8D0 ; 
//.text:00B5B8F0 ; 
//.text:00B5B910 ; 
//.text:00B5B930 ; 
//.text:00B5BA70 ; 
//.text:00B5BA90 ; 
//.text:00B5BD90 ; bool __cdecl weapon_action_can_be_interrupted(long)
//.text:00B5BDD0 ; void __cdecl weapon_age(long, real)
//.text:00B5BEC0 ; bool __cdecl weapon_age_external(long)
//.text:00B5BF40 ; bool __cdecl weapon_aim(long, short, real_point3d const*, real_point3d const*, real*, bool, vector3d*, real*, real*, real*, bool*)
//.text:00B5C010 ; bool __cdecl weapon_allows_unaimed_lunge(long)
//.text:00B5C060 ; void __cdecl weapon_apply_predicted_state(long)
//.text:00B5C100 ; 
//.text:00B5C1D0 ; void __cdecl weapon_barrel_accuracy_ding(long, s_projectile_accuracy_penalty_info::e_penalty_ding)
//.text:00B5C2E0 ; void __cdecl weapon_barrel_accuracy_penalty_firing(long, long)
//.text:00B5C430 ; void __cdecl weapon_barrel_accuracy_penalty_jump(long)
//.text:00B5C450 ; void __cdecl weapon_barrel_accuracy_penalty_movement(long, long)
//.text:00B5C610 ; void __cdecl weapon_barrel_accuracy_penalty_reload(long)
//.text:00B5C630 ; void __cdecl weapon_barrel_accuracy_penalty_rotation(long, vector3d const*, vector3d const*)
//.text:00B5C830 ; void __cdecl weapon_barrel_accuracy_penalty_weapon_switch(long)
//.text:00B5C850 ; void __cdecl weapon_barrel_accuracy_penalty_zoom(long)
//.text:00B5C870 ; bool __cdecl weapon_barrel_apply_fire_error(long, long, bool)
//.text:00B5C940 ; bool __cdecl weapon_barrel_can_fire(long, short, bool)
//.text:00B5CB90 ; bool __cdecl weapon_barrel_can_fire_this_tick(long, short, bool*, bool*,bool*)
//.text:00B5CE60 ; weapon_barrel_can_fire_this_tick_predicted
//.text:00B5CF80 ; bool __cdecl weapon_barrel_check_for_reload(long, long)
//.text:00B5D100 ; void __cdecl weapon_barrel_clear_empty_click_bit(long, short)
//.text:00B5D150 ; void __cdecl weapon_barrel_compute_recovery_ticks(long, short, real*, real*)
//.text:00B5D360 ; double __cdecl weapon_barrel_compute_ticks_per_round(long, short)
//.text:00B5D5A0 ; void __cdecl weapon_barrel_create_projectiles(long, short, s_predicted_weapon_fire_data const*, bool)
//.text:00B5F2D0 ; dword __cdecl weapon_barrel_create_projectiles_get_initial_random_seed(long, s_predicted_weapon_fire_data const*, bool, bool, dword*)
//.text:00B5F470 ; bool __cdecl weapon_barrel_evaluate_penalty_function(long, long, bool, s_projectile_accuracy_penalty_functions::e_penalty_function, real, real*)
//.text:00B5F5D0 ; 
//.text:00B5F860 ; void __cdecl weapon_barrel_fire(long, short, bool)
//.text:00B605D0 ; double __cdecl weapon_barrel_get_barrel_error_angle_from_weapon(long, long, bool, short, real)
//.text:00B606D0 ; long __cdecl weapon_barrel_get_marker_name(long, long)
//.text:00B60750 ; 
//.text:00B607F0 ; bool __cdecl weapon_barrel_is_locked_recovering(long, short)
//.text:00B60840 ; 
//.text:00B60890 ; bool __cdecl weapon_barrel_is_spew_networking_suppressed(long, short)
//.text:00B608D0 ; bool __cdecl weapon_barrel_must_recover_predicted(long, short)
//.text:00B60950 ; void __cdecl weapon_barrel_recover(long, short, bool)
//.text:00B60A80 ; void __cdecl weapon_barrel_set_fire_bit(long, short, bool)
//.text:00B60AE0 ; void __cdecl weapon_barrel_set_predicted_recover(long, short)
//.text:00B60B40 ; 
//.text:00B60BD0 ; bool __cdecl weapon_barrel_update(long, long)
//.text:00B611F0 ; void __cdecl weapon_build_weapon_interface_state(long, weapon_interface_state*)

bool __cdecl weapon_can_be_dual_wielded(long weapon_index)
{
	//return INVOKE(0x00B61550, weapon_can_be_dual_wielded, weapon_index);

	weapon_datum* weapon = (weapon_datum*)object_get_and_verify_type(weapon_index, _object_mask_weapon);
	struct weapon_definition* weapon_definition = (struct weapon_definition*)tag_get(WEAPON_TAG, weapon->definition_index);
	return weapon_definition->weapon.flags.test(_weapon_can_be_dual_wielded_bit) || weapon_definition->weapon.flags.test(_weapon_can_only_be_dual_wielded_bit);
}

//.text:00B61560 ; bool __cdecl weapon_can_be_fired(long)
//.text:00B61600 ; bool __cdecl weapon_can_be_stowed(long, long, bool)
//.text:00B61740 ; bool __cdecl weapon_can_interrupt_reload(long)
//.text:00B61790 ; bool __cdecl weapon_can_only_be_dual_wielded(long)
//.text:00B617E0 ; bool __cdecl weapon_can_track(long)
//.text:00B61840 ; bool __cdecl weapon_can_track_bipeds(long)
//.text:00B618B0 ; bool __cdecl weapon_can_zoom(long)
//.text:00B61920 ; bool __cdecl weapon_compute_function_value(long, long, long, real*, bool*, bool*)
//.text:00B62390 ; double __cdecl weapon_compute_movement_penalty(long, bool, bool)
//.text:00B62420 ; bool __cdecl weapon_counts_towards_inventory_limit(long)
//.text:00B62480 ; 
//.text:00B624E0 ; bool __cdecl weapon_definition_is_age_based(long)
//.text:00B62560 ; void __cdecl weapon_delay_predicted_state(long)
//.text:00B625C0 ; 
//.text:00B62620 ; void __cdecl weapon_detonate(long)
//.text:00B626A0 ; bool __cdecl weapon_disables_non_lunge_melee_dash(long)
//.text:00B626F0 ; 
//.text:00B62770 ; long __cdecl weapon_effect_new(long, long, long, real, real)
//.text:00B628C0 ; 
//.text:00B62950 ; 
//.text:00B629F0 ; bool __cdecl weapon_firing_is_disabled(long)
//.text:00B62A50 ; long __cdecl weapon_get_active_barrel_index(long)

real __cdecl weapon_get_age(long weapon_index)
{
	//return INVOKE(0x00B62AC0, weapon_get_age, weapon_index);

	real result = 0.0f;
	HOOK_INVOKE(, weapon_get_age, weapon_index);

	if (cheat.bottomless_clip)
		result = 0.0f;

	return result;
}

//.text:00B62B20 ; long __cdecl weapon_get_effect_object_index(long)

real __cdecl weapon_get_field_of_view_change_time(long weapon_index)
{
	return INVOKE(0x00B62B80, weapon_get_field_of_view_change_time, weapon_index);
}

//.text:00B62BF0 ; 
//.text:00B62C40 ; 
//.text:00B62CE0 ; real __cdecl weapon_get_maximum_damage_range(long)
//.text:00B62DB0 ; short __cdecl weapon_get_multiplayer_type(long)
//.text:00B62DF0 ; long __cdecl weapon_get_owner_unit_index(long)
//.text:00B62E30 ; long __cdecl weapon_get_owner_unit_inventory_index(long)
//.text:00B62EA0 ; double __cdecl weapon_get_pitch_throttling_while_firing(long)
//.text:00B62FA0 ; double __cdecl weapon_get_projectile_error(long, long)
//.text:00B63030 ; long __cdecl weapon_get_projectile_owner_unit_index(long)
//.text:00B63100 ; 
//.text:00B63150 ; long __cdecl weapon_get_rounds_inventory(long, long, bool)
//.text:00B63280 ; 
//.text:00B632C0 ; 
//.text:00B63310 ; 
//.text:00B63350 ; double __cdecl weapon_get_switch_weapon_speed_modifier(long)
//.text:00B63410 ; void __cdecl weapon_get_target_tracking_reticle_position(c_aim_target_object const*, real_point3d*)
//.text:00B63550 ; bool __cdecl weapon_get_trigger_position(long, short, real_point3d*)
//.text:00B63690 ; 
//.text:00B636E0 ; double __cdecl weapon_get_zoom_magnification(long, short)
//.text:00B637C0 ; 
//.text:00B63840 ; bool __cdecl weapon_handle_potential_inventory_item(long, long, long, long, short*)
//.text:00B63BE0 ; 

bool __cdecl weapon_has_infinite_ammo(long weapon_index)
{
	//return INVOKE(0x00B63C30, weapon_has_infinite_ammo, weapon_index);

	bool result = false;
	HOOK_INVOKE(result =, weapon_has_infinite_ammo, weapon_index);

	if (cheat.infinite_ammo)
		result = true;

	return result;
}

//.text:00B63CB0 ; bool __cdecl weapon_has_networked_aiming_consequences(long)
//.text:00B63CF0 ; void __cdecl weapon_hold_trigger_hs(long, long, bool)
//.text:00B63D80 ; bool __cdecl weapon_is_age_based(long)
//.text:00B63DC0 ; 
//.text:00B63E40 ; 
//.text:00B63EA0 ; bool __cdecl weapon_is_controlled_by_player(long)
//.text:00B63F20 ; bool __cdecl weapon_is_depleted(long)
//.text:00B64050 ; bool __cdecl weapon_is_dual_wielded(long)
//.text:00B64140 ; bool __cdecl weapon_is_melee_only(long)
//.text:00B64190 ; bool __cdecl weapon_is_ready_for_use(long)
//.text:00B641D0 ; bool __cdecl weapon_is_support_weapon(long)
//.text:00B64220 ; bool __cdecl weapon_is_support_weapon_or_must_be_readied(long)
//.text:00B64280 ; bool __cdecl weapon_is_tracking(long)
//.text:00B64300 ; bool __cdecl weapon_is_zoomable(long)
//.text:00B64350 ; void __cdecl weapon_magazine_execute_reload(long, short)
//.text:00B644D0 ; void __cdecl weapon_magazine_finish_reload(long, short, bool)
//.text:00B64540 ; e_unit_messages __cdecl weapon_magazine_get_reload_unit_message(long, short, bool)
//.text:00B64590 ; 
//.text:00B645D0 ; 
//.text:00B64650 ; void __cdecl weapon_magazine_set_state(long, short, short)
//.text:00B647E0 ; void __cdecl weapon_magazine_start_chamber(long, short)
//.text:00B648A0 ; bool __cdecl weapon_magazine_start_or_continue_reload(long, short)
//.text:00B64A70 ; bool __cdecl weapon_magazine_starting_continuous_reload(long, short)
//.text:00B64AB0 ; 
//.text:00B64B00 ; 
//.text:00B64B30 ; bool __cdecl weapon_magazine_update(long, long)
//.text:00B64E00 ; bool __cdecl weapon_must_be_readied(long)
//.text:00B64E50 ; bool __cdecl weapon_must_stay_awake_for_accuracy_penalties(long)
//.text:00B64F20 ; bool __cdecl weapon_new(long, object_placement_data*, bool*)
//.text:00B650F0 ; bool __cdecl weapon_overcharged(long)
//.text:00B65130 ; bool __cdecl weapon_owner_submit_reload_action(long, short, bool)
//.text:00B65270 ; void __cdecl weapon_owner_update(long, word, real)
//.text:00B652F0 ; long __cdecl weapon_place(long, s_scenario_weapon const*)
//.text:00B65390 ; bool __cdecl weapon_prevents_crouching(long)
//.text:00B653E0 ; bool __cdecl weapon_prevents_equipment_throwing(long)
//.text:00B65460 ; bool __cdecl weapon_prevents_grenade_throwing(long)
//.text:00B654D0 ; bool __cdecl weapon_prevents_melee_attack(long)
//.text:00B65530 ; bool __cdecl weapon_put_away(long, bool, bool)
//.text:00B65950 ; void __cdecl weapon_ready(long, bool, bool)
//.text:00B65C20 ; bool __cdecl weapon_reload(long, long)
//.text:00B65E30 ; 
//.text:00B66220 ; bool __cdecl weapon_reloading(long, long, short*)
//.text:00B66310 ; 
//.text:00B663E0 ; void __cdecl weapon_report_kill(long, long)
//.text:00B664A0 ; void __cdecl weapon_reset(long)
//.text:00B665B0 ; void __cdecl weapon_reset_and_idle_if_not_reloading(long, long)
//.text:00B665E0 ; void __cdecl weapon_reset_first_person_emulation(long)
//.text:00B66640 ; long __cdecl weapon_respond_to_drop(long)
//.text:00B66760 ; short __cdecl weapon_rotate_zoom_level(long, short)
//.text:00B667F0 ; void __cdecl weapon_send_message_to_unit(long, e_unit_messages)
//.text:00B66870 ; void __cdecl weapon_set_current_amount(long, real)
//.text:00B669A0 ; void __cdecl weapon_set_initial_rounds(long, short, short, short)
//.text:00B66A80 ; void __cdecl weapon_set_initial_rounds_from_scenario_weapon(long, s_scenario_weapon const*)
//.text:00B66B30 ; void __cdecl weapon_set_power(long, real)
//.text:00B66B90 ; void __cdecl weapon_set_predicted_age(long, real)
//.text:00B66BE0 ; void __cdecl weapon_set_predicted_rounds_loaded(long, short, short, short)
//.text:00B66CB0 ; 
//.text:00B66D00 ; bool __cdecl weapon_set_state(long, long, bool)
//.text:00B66F80 ; void __cdecl weapon_set_total_rounds(long, short*)
//.text:00B67050 ; bool __cdecl weapon_setup_first_person_emulation(long, long, short)
//.text:00B67300 ; 
//.text:00B67360 ; 
//.text:00B67390 ; 
//.text:00B67450 ; 
//.text:00B674C0 ; 
//.text:00B67500 ; void __cdecl weapon_steal_goodies(long, long)
//.text:00B67680 ; 
//.text:00B676D0 ; void __cdecl weapon_stop_reload(long, bool)
//.text:00B67830 ; void __cdecl weapon_stow(long, long)
//.text:00B67880 ; bool __cdecl weapon_take_inventory_rounds(long, long, long)
//.text:00B67A00 ; void __cdecl weapon_trigger_begin_charging(long, short)
//.text:00B67BF0 ; bool __cdecl weapon_trigger_can_begin_charging(long, short)
//.text:00B67C80 ; 
//.text:00B67CD0 ; void __cdecl weapon_trigger_determine_analog_autofire_behavior(long, long, bool*, bool*, bool*)
//.text:00B67F10 ; void __cdecl weapon_trigger_fully_charged(long, short)
//.text:00B67FB0 ; double __cdecl weapon_trigger_get_charged_with_cooldown_fraction(long, short)
//.text:00B68080 ; 
//.text:00B680D0 ; void __cdecl weapon_trigger_overcharged(long, short)
//.text:00B68140 ; void __cdecl weapon_trigger_recover(long, short, bool)
//.text:00B68210 ; void __cdecl weapon_trigger_release_charge(long, short)
//.text:00B68460 ; bool __cdecl weapon_trigger_update(long, long)
//.text:00B68ED0 ; 
//.text:00B68F30 ; 
//.text:00B68F90 ; void __cdecl weapon_unready(long)
//.text:00B68FD0 ; bool __cdecl weapon_update(long)
//.text:00B69710 ; bool __cdecl weapon_update_first_person_emulation(long)
//.text:00B69870 ; bool __cdecl weapon_update_targeting(long)
//.text:00B69A00 ; bool __cdecl weapon_useful(long)
//.text:00B69A20 ; bool __cdecl weapon_uses_3rd_person_camera(long)
//.text:00B69A70 ; bool __cdecl weapon_uses_charging_melee(long)
//.text:00B69AD0 ; void __cdecl weapons_fire_barrels()

