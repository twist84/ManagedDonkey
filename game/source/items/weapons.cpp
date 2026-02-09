#include "items/weapons.hpp"

#include "cache/cache_files.hpp"
#include "game/cheats.hpp"
#include "game/player_mapping.hpp"
#include "interface/first_person_weapons.hpp"
#include "items/weapon_definitions.hpp"
#include "memory/module.hpp"
#include "render/simple_font.hpp"

HOOK_DECLARE(0x00B5F860, weapon_barrel_fire);
HOOK_DECLARE(0x00B61550, weapon_can_be_dual_wielded);
HOOK_DECLARE(0x00B62AC0, weapon_get_age);
HOOK_DECLARE(0x00B63C30, weapon_has_infinite_ammo);

bool debug_weapons = false;
bool debug_weapons_triggers = true;
bool debug_weapons_barrels = true;
bool debug_weapons_magazines = true;
bool debug_weapons_primary = true;
bool debug_weapons_secondary = false;

//.text:00B5B120 ; 
//.text:00B5B130 ; 
//.text:00B5B150 ; 
//.text:00B5B160 ; 
//.text:00B5B180 ; 
//.text:00B5B1A0 ; void __cdecl emulated_first_person_animation_callback(const s_animation_event_data*, int32)
//.text:00B5B3D0 ; 
//.text:00B5B460 ; 
//.text:00B5B470 ; 
//.text:00B5B480 ; 
//.text:00B5B490 ; 
//.text:00B5B4A0 ; 
//.text:00B5B4B0 ; 
//.text:00B5B4C0 ; 
//.text:00B5B4D0 ; real32 __cdecl indirect_fire_compute_arcing_fraction(int32, real32)
//.text:00B5B5B0 ; bool __cdecl indirect_fire_compute_ballistics(int32, int32, int32, const real_point3d*, real_vector3d*, real32*)
//.text:00B5B770 ; bool __cdecl indirect_fire_get_aim_target_position(int32, real_point3d*)
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
//.text:00B5BD90 ; bool __cdecl weapon_action_can_be_interrupted(int32)
//.text:00B5BDD0 ; void __cdecl weapon_age(int32, real32)
//.text:00B5BEC0 ; bool __cdecl weapon_age_external(int32)
//.text:00B5BF40 ; bool __cdecl weapon_aim(int32, int16, const real_point3d*, const real_point3d*, real32*, bool, real_vector3d*, real32*, real32*, real32*, bool*)
//.text:00B5C010 ; bool __cdecl weapon_allows_unaimed_lunge(int32)
//.text:00B5C060 ; void __cdecl weapon_apply_predicted_state(int32)
//.text:00B5C100 ; 
//.text:00B5C1D0 ; void __cdecl weapon_barrel_accuracy_ding(int32, s_projectile_accuracy_penalty_info::e_penalty_ding)
//.text:00B5C2E0 ; void __cdecl weapon_barrel_accuracy_penalty_firing(int32, int32)
//.text:00B5C430 ; void __cdecl weapon_barrel_accuracy_penalty_jump(int32)
//.text:00B5C450 ; void __cdecl weapon_barrel_accuracy_penalty_movement(int32, int32)
//.text:00B5C610 ; void __cdecl weapon_barrel_accuracy_penalty_reload(int32)
//.text:00B5C630 ; void __cdecl weapon_barrel_accuracy_penalty_rotation(int32, const real_vector3d*, const real_vector3d*)
//.text:00B5C830 ; void __cdecl weapon_barrel_accuracy_penalty_weapon_switch(int32)
//.text:00B5C850 ; void __cdecl weapon_barrel_accuracy_penalty_zoom(int32)
//.text:00B5C870 ; bool __cdecl weapon_barrel_apply_fire_error(int32, int32, bool)
//.text:00B5C940 ; bool __cdecl weapon_barrel_can_fire(int32, int16, bool)
//.text:00B5CB90 ; bool __cdecl weapon_barrel_can_fire_this_tick(int32, int16, bool*, bool*,bool*)
//.text:00B5CE60 ; bool __cdecl weapon_barrel_can_fire_this_tick_predicted(int32, int16, int32*, int32*, int32*, int32*, int32*)
//.text:00B5CF80 ; bool __cdecl weapon_barrel_check_for_reload(int32, int32)
//.text:00B5D100 ; void __cdecl weapon_barrel_clear_empty_click_bit(int32, int16)
//.text:00B5D150 ; void __cdecl weapon_barrel_compute_recovery_ticks(int32, int16, real32*, real32*)
//.text:00B5D360 ; real32 __cdecl weapon_barrel_compute_ticks_per_round(int32, int16)
//.text:00B5D5A0 ; void __cdecl weapon_barrel_create_projectiles(int32, int16, const s_predicted_weapon_fire_data*, bool)
//.text:00B5F2D0 ; uns32 __cdecl weapon_barrel_create_projectiles_get_initial_random_seed(int32, const s_predicted_weapon_fire_data*, bool, bool, uns32*)
//.text:00B5F470 ; bool __cdecl weapon_barrel_evaluate_penalty_function(int32, int32, bool, s_projectile_accuracy_penalty_functions::e_penalty_function, real32, real32*)
//.text:00B5F5D0 ; 

void __cdecl weapon_barrel_fire(int32 weapon_index, int16 barrel_index, bool predicted)
{
	//INVOKE(0x00B5F860, weapon_barrel_fire, weapon_index, barrel_index, predicted);
	HOOK_INVOKE(, weapon_barrel_fire, weapon_index, barrel_index, predicted);

	if (!cheat.bottomless_clip)
		return;

	weapon_datum* weapon = WEAPON_GET(weapon_index);
	if (!weapon || weapon->item.inventory_unit_index == NONE)
		return;

	unit_datum* unit = UNIT_GET(weapon->item.inventory_unit_index);
	if (!unit || unit->unit.player_index == NONE)
		return;

	weapon->weapon.heat = 0.0f;
	weapon->weapon.age = 0.0f;

	s_weapon_definition* weapon_definition = TAG_GET(WEAPON_TAG, s_weapon_definition, weapon->definition_index);
	int16 magazine_index = weapon_definition->weapon.barrels[barrel_index].magazine;
	if (magazine_index != NONE && magazine_index < weapon_definition->weapon.magazines.count)
		const_cast<weapon_magazine*>(weapon->weapon.magazines)[magazine_index].rounds_loaded = weapon_definition->weapon.magazines[magazine_index].rounds_loaded_maximum;
}

//.text:00B605D0 ; real32 __cdecl weapon_barrel_get_barrel_error_angle_from_weapon(int32, int32, bool, int16, real32)
//.text:00B606D0 ; int32 __cdecl weapon_barrel_get_marker_name(int32, int32)
//.text:00B60750 ; void __cdecl weapon_barrel_idle(int32, int16)
//.text:00B607F0 ; bool __cdecl weapon_barrel_is_locked_recovering(int32, int16)
//.text:00B60840 ; bool __cdecl weapon_barrel_is_recovering(int32, int16)
//.text:00B60890 ; bool __cdecl weapon_barrel_is_spew_networking_suppressed(int32, int16)
//.text:00B608D0 ; bool __cdecl weapon_barrel_must_recover_predicted(int32, int16)
//.text:00B60950 ; void __cdecl weapon_barrel_recover(int32, int16, bool)
//.text:00B60A80 ; void __cdecl weapon_barrel_set_fire_bit(int32, int16, bool)
//.text:00B60AE0 ; void __cdecl weapon_barrel_set_predicted_recover(int32, int16)
//.text:00B60B40 ; void __cdecl weapon_barrel_start_ejection_port(int32, int16, bool)
//.text:00B60BD0 ; bool __cdecl weapon_barrel_update(int32, int32)
//.text:00B611F0 ; void __cdecl weapon_build_weapon_interface_state(int32, weapon_interface_state*)

bool __cdecl weapon_can_be_dual_wielded(int32 weapon_index)
{
	//return INVOKE(0x00B61550, weapon_can_be_dual_wielded, weapon_index);

	weapon_datum* weapon = WEAPON_GET(weapon_index);
	s_weapon_definition* weapon_definition = TAG_GET(WEAPON_TAG, s_weapon_definition, weapon->definition_index);
	return weapon_definition->weapon.flags.test(_weapon_can_be_dual_wielded_bit) || weapon_definition->weapon.flags.test(_weapon_can_only_be_dual_wielded_bit);
}

//.text:00B61560 ; bool __cdecl weapon_can_be_fired(int32)
//.text:00B61600 ; bool __cdecl weapon_can_be_stowed(int32, int32, bool)
//.text:00B61740 ; bool __cdecl weapon_can_interrupt_reload(int32)
//.text:00B61790 ; bool __cdecl weapon_can_only_be_dual_wielded(int32)
//.text:00B617E0 ; bool __cdecl weapon_can_track(int32)
//.text:00B61840 ; bool __cdecl weapon_can_track_bipeds(int32)
//.text:00B618B0 ; bool __cdecl weapon_can_zoom(int32)
//.text:00B61920 ; bool __cdecl weapon_compute_function_value(int32, int32, int32, real32*, bool*, bool*)
//.text:00B62390 ; real32 __cdecl weapon_compute_movement_penalty(int32, bool, bool)
//.text:00B62420 ; bool __cdecl weapon_counts_towards_inventory_limit(int32)
//.text:00B62480 ; bool __cdecl weapon_damage_barrel(int32, int32)

void weapon_debug_render(int32 weapon_index, int32 weapon_slot)
{
	weapon_datum* weapon = WEAPON_GET(weapon_index);
	s_weapon_definition* weapon_definition = TAG_GET(WEAPON_TAG, s_weapon_definition, weapon->definition_index);

	int32 inventory_unit_index = NONE;
	if (weapon->item.inventory_state)
	{
		inventory_unit_index = weapon->item.inventory_unit_index;
		ASSERT(weapon->item.inventory_unit_index != NONE);
	}

	c_simple_font_screen_display screen_display{};
	if (screen_display.open_session(1.25f))
	{
		int32 text_x_offset = (200 - screen_display.m_base_x) / screen_display.m_char_width;
		int32 text_y_offset = (525 - screen_display.m_base_y) / screen_display.m_char_height;

		const char* weapon_state_string = nullptr;

		e_weapon_state weapon_state = weapon->weapon.state;
		switch (weapon_state)
		{
		case _weapon_state_idle:
			weapon_state_string = "idle";
			break;
		case _weapon_state_fire_1:
			weapon_state_string = "fire_1";
			break;
		case _weapon_state_fire_2:
			weapon_state_string = "fire_2";
			break;
		case _weapon_state_chamber_1:
			weapon_state_string = "chamber_1";
			break;
		case _weapon_state_chamber_2:
			weapon_state_string = "chamber_2";
			break;
		case _weapon_state_reload_1:
			weapon_state_string = "reload_1";
			break;
		case _weapon_state_reload_2:
			weapon_state_string = "reload_2";
			break;
		case _weapon_state_charged_1:
			weapon_state_string = "charged_1";
			break;
		case _weapon_state_charged_2:
			weapon_state_string = "charged_2";
			break;
		case _weapon_state_ready:
			weapon_state_string = "ready";
			break;
		case _weapon_state_put_away:
			weapon_state_string = "put_away";
			break;
		default:
			weapon_state_string = "<unknown>";
			break;
		}

		screen_display.draw(text_x_offset, text_y_offset++, NONE, "weapon(3P) state: %s timer: %d",
			weapon_state_string,
			weapon->weapon.state_timer
		);

		if (inventory_unit_index != NONE)
		{
			int32 current_state_string_id = first_person_weapon_get_current_state_string(inventory_unit_index, weapon_slot);
			int32 pending_state_string_id = first_person_weapon_get_pending_state_string(inventory_unit_index, weapon_slot);

			bool current_state_invalid = current_state_string_id == _string_id_invalid;
			bool pending_state_invalid = pending_state_string_id == _string_id_invalid;

			const char* current_state_string = current_state_invalid ? "<unknown>" : string_id_get_string_const(current_state_string_id);
			const char* pending_state_string = pending_state_invalid ? "<unknown>" : string_id_get_string_const(pending_state_string_id);

			if (pending_state_invalid)
			{
				screen_display.draw(text_x_offset, text_y_offset++, NONE, "1P state cur: %s",
					current_state_string);
			}
			else
			{
				screen_display.draw(text_x_offset, text_y_offset++, NONE, "1P state cur: %s pending: %s",
					current_state_string,
					pending_state_string);
			}
		}

		screen_display.draw(text_x_offset, text_y_offset++, NONE, "flags: overheated: %s oh_exit: %s",
			TEST_FLAG(weapon->weapon.flags, _weapon_overheated_bit) ? "y" : "n",
			TEST_FLAG(weapon->weapon.flags, _weapon_overheated_exit_bit) ? "y" : "n"
		);

		int32 magazine_index = 0;
		if (debug_weapons_triggers)
		{
			for (int32 trigger_index = 0; trigger_index < weapon_definition->weapon.triggers.count; trigger_index++)
			{
				const weapon_trigger& trigger = weapon->weapon.triggers[trigger_index];
				ASSERT(trigger_index >= 0 && trigger_index < weapon_definition->weapon.triggers.count);

				const char* trigger_behavior_string = nullptr;
				e_weapon_trigger_behavior trigger_behavior = weapon_definition->weapon.triggers[trigger_index].behavior;
				switch (trigger_behavior)
				{
				case _weapon_trigger_behavior_spew:
					trigger_behavior_string = "spew";
					break;
				case _weapon_trigger_behavior_latch:
					trigger_behavior_string = "latch";
					break;
				case _weapon_trigger_behavior_latch_autofire:
					trigger_behavior_string = "latch-autofire";
					break;
				case _weapon_trigger_behavior_charge:
					trigger_behavior_string = "charge";
					break;
				case _weapon_trigger_behavior_latch_zoom:
					trigger_behavior_string = "latch-zoom";
					break;
				case _weapon_trigger_behavior_latch_rocket_launcher:
					trigger_behavior_string = "latch-rl";
					break;
				case _weapon_trigger_behavior_spew_charge:
					trigger_behavior_string = "spew-charge";
					break;
				case _weapon_trigger_behavior_sword_charge:
					trigger_behavior_string = "sword-charge";
					break;
				default:
					trigger_behavior_string = "<unknown>";
					break;
				}

				const char* trigger_state_string = nullptr;
				e_weapon_trigger_state trigger_state = trigger.state;
				switch (trigger_state)
				{
				case _weapon_trigger_state_idle:
					trigger_state_string = "idle";
					break;
				case _weapon_trigger_state_charging:
					trigger_state_string = "charging";
					break;
				case _weapon_trigger_state_charged:
					trigger_state_string = "charged";
					break;
				case _weapon_trigger_state_spewing:
					trigger_state_string = "spewing";
					break;
				default:
					trigger_state_string = "<unknown>";
					break;
				}

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Trigger[%d] behavior: %s state: %s  timer: %d",
					trigger_index,
					trigger_behavior_string,
					trigger_state_string,
					trigger.timer
				);

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Trigger[%d] flags: rel: %s dest: %s f b4 charge %s 1aa %s 2aa %s spew: %s p-charge: %s s-held: %s",
					trigger_index,
					TEST_FLAG(trigger.flags, _weapon_trigger_rel_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_dest_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_f_b4_charge_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_1aa_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_2aa_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_spew_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_p_charge_bit) ? "y" : "n",
					TEST_FLAG(trigger.flags, _weapon_trigger_s_held_bit) ? "y" : "n"
				);
			}

			magazine_index = 0;
		}

		if (debug_weapons_barrels)
		{
			for (int32 barrel_index = 0; barrel_index < weapon_definition->weapon.barrels.count; barrel_index++)
			{
				const weapon_barrel& barrel = weapon->weapon.barrels[barrel_index];
				ASSERT(barrel_index >= 0 && barrel_index < weapon_definition->weapon.barrels.count);

				const char* barrel_state_string = nullptr;

				e_weapon_barrel_state barrel_state = barrel.state;
				switch (barrel_state)
				{
				case _weapon_barrel_state_idle:
					barrel_state_string = "idle";
					break;
				case _weapon_barrel_state_firing:
					barrel_state_string = "firing";
					break;
				case _weapon_barrel_state_locked_recovering:
					barrel_state_string = "locked recovering";
					break;
				case _weapon_barrel_state_locked_recovering_empty:
					barrel_state_string = "locked recovering empty";
					break;
				case _weapon_barrel_state_recovering:
					barrel_state_string = "recovering";
					break;
				default:
					barrel_state_string = "<unknown>";
					break;
				}

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Barrel[%d] state: %s  timer: %d  overflow: %f  idle_ticks: %d  bonus_round_fraction: %1.3f",
					barrel_index,
					barrel_state_string,
					barrel.timer,
					barrel.overflow,
					barrel.idle_ticks,
					barrel.bonus_round_fraction
				);

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Barrel[%d] current_error: %f  angle_change_scale: %f",
					barrel_index,
					barrel.current_error,
					barrel.angle_change_scale
				);

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Barrel[%d] flags: fire: %s create: %s dest: %s blur %s f b4 charge %s dmg %s click %s fx: %s",
					barrel_index,
					TEST_FLAG(barrel.flags, _weapon_barrel_fire_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_create_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_dest_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_blur_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_f_b4_charge_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_dmg_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_click_bit) ? "y" : "n",
					TEST_FLAG(barrel.flags, _weapon_barrel_fx_bit) ? "y" : "n"
				);
			}

			magazine_index = 0;
		}

		if (debug_weapons_magazines)
		{
			for (; magazine_index < weapon_definition->weapon.magazines.count; magazine_index++)
			{
				const weapon_magazine& magazine = weapon->weapon.magazines[magazine_index];
				ASSERT(magazine_index >= 0 && magazine_index < weapon_definition->weapon.magazines.count);

				const char* magazine_state_string = nullptr;

				e_weapon_magazine_state magazine_state = magazine.state;
				switch (magazine_state)
				{
				case _weapon_magazine_state_idle:
					magazine_state_string = "idle\t\t\t\t\t\t";
					break;
				case _weapon_magazine_state_reload_single:
					magazine_state_string = "reload_single\t\t\t\t";
					break;
				case _weapon_magazine_state_reload_continuous_starting:
					magazine_state_string = "reload_continuous_starting ";
					break;
				case _weapon_magazine_state_reload_continue_underway:
					magazine_state_string = "reload_continue_underway\t";
					break;
				case _weapon_magazine_state_reload_continue_ending:
					magazine_state_string = "reload_continue_ending\t\t";
					break;
				case _weapon_magazine_state_unchambered:
					magazine_state_string = "unchambered\t\t\t\t";
					break;
				case _weapon_magazine_state_chambering:
					magazine_state_string = "chambering\t\t\t\t\t";
					break;
				case _weapon_magazine_state_busy:
					magazine_state_string = "busy\t\t\t\t\t\t";
					break;
				default:
					magazine_state_string = "<unknown>\t\t\t\t\t";
					break;
				}

				screen_display.draw(text_x_offset, text_y_offset++, NONE, "Magazine[%d] state: %s  timers(state/reload): %d/%d",
					magazine_index,
					magazine_state_string,
					magazine.reload_cooldown,
					magazine.firing_cooldown
				);
			}
		}

		screen_display.close_session();
	}

	screen_display.close_session();
}

//.text:00B624E0 ; bool __cdecl weapon_definition_is_age_based(int32)
//.text:00B62560 ; void __cdecl weapon_delay_predicted_state(int32)
//.text:00B625C0 ; bool __cdecl weapon_destroy_barrel(int32, int32)
//.text:00B62620 ; void __cdecl weapon_detonate(int32)
//.text:00B626A0 ; bool __cdecl weapon_disables_non_lunge_melee_dash(int32)
//.text:00B626F0 ; void __cdecl weapon_effect_fire_empty_click(int32)
//.text:00B62770 ; int32 __cdecl weapon_effect_new(int32, int32, int32, real32, real32)
//.text:00B628C0 ; real32 __cdecl weapon_estimate_time_to_target(int32, int16, real32, bool)
//.text:00B62950 ; void __cdecl weapon_fire_barrels(int32, const s_predicted_weapon_fire_data*, bool, int32)
//.text:00B629F0 ; bool __cdecl weapon_firing_is_disabled(int32)
//.text:00B62A50 ; int32 __cdecl weapon_get_active_barrel_index(int32)

real32 __cdecl weapon_get_age(int32 weapon_index)
{
	//return INVOKE(0x00B62AC0, weapon_get_age, weapon_index);

	real32 result = 0.0f;
	HOOK_INVOKE(, weapon_get_age, weapon_index);

	if (cheat.bottomless_clip)
		result = 0.0f;

	return result;
}

//.text:00B62B20 ; int32 __cdecl weapon_get_effect_object_index(int32)

real32 __cdecl weapon_get_field_of_view_change_time(int32 weapon_index)
{
	return INVOKE(0x00B62B80, weapon_get_field_of_view_change_time, weapon_index);
}

//.text:00B62BF0 ; int32 __cdecl weapon_get_label(int32)
//.text:00B62C40 ; int32 __cdecl weapon_get_magazine_state(int32, int32)
//.text:00B62CE0 ; real32 __cdecl weapon_get_maximum_damage_range(int32)
//.text:00B62DB0 ; int16 __cdecl weapon_get_multiplayer_type(int32)
//.text:00B62DF0 ; int32 __cdecl weapon_get_owner_unit_index(int32)
//.text:00B62E30 ; int32 __cdecl weapon_get_owner_unit_inventory_index(int32)
//.text:00B62EA0 ; real32 __cdecl weapon_get_pitch_throttling_while_firing(int32)
//.text:00B62FA0 ; real32 __cdecl weapon_get_projectile_error(int32, int32)
//.text:00B63030 ; int32 __cdecl weapon_get_projectile_owner_unit_index(int32)
//.text:00B63100 ; real32 __cdecl weapon_get_reload_speed_modifier(int32)
//.text:00B63150 ; int32 __cdecl weapon_get_rounds_inventory(int32, int32, bool)
//.text:00B63280 ; int32 __cdecl weapon_get_rounds_loaded(int32, int32)
//.text:00B632C0 ; int32 __cdecl weapon_get_rounds_total(int32, int32, bool)
//.text:00B63310 ; 
//.text:00B63350 ; real32 __cdecl weapon_get_switch_weapon_speed_modifier(int32)
//.text:00B63410 ; void __cdecl weapon_get_target_tracking_reticle_position(const c_aim_target_object*, real_point3d*)
//.text:00B63550 ; bool __cdecl weapon_get_trigger_position(int32, int16, real_point3d*)
//.text:00B63690 ; 
//.text:00B636E0 ; real32 __cdecl weapon_get_zoom_magnification(int32, int16)
//.text:00B637C0 ; void __cdecl weapon_handle_deleted_projectile(int32, int32)

bool __cdecl weapon_handle_potential_inventory_item(int32 weapon_index, int32 item_index, int32 unit_index, int32 player_index, int16* rounds_picked_up)
{
	return INVOKE(0x00B63840, weapon_handle_potential_inventory_item, weapon_index, item_index, unit_index, player_index, rounds_picked_up);
}

//.text:00B63BE0 ; bool __cdecl weapon_has_clang_melee(int32)

bool __cdecl weapon_has_infinite_ammo(int32 weapon_index)
{
	//return INVOKE(0x00B63C30, weapon_has_infinite_ammo, weapon_index);

	bool result = false;
	HOOK_INVOKE(result =, weapon_has_infinite_ammo, weapon_index);

	if (cheat.infinite_ammo)
		result = true;

	return result;
}

//.text:00B63CB0 ; bool __cdecl weapon_has_networked_aiming_consequences(int32)
//.text:00B63CF0 ; void __cdecl weapon_hold_trigger_hs(int32, int32, bool)
//.text:00B63D80 ; bool __cdecl weapon_is_age_based(int32)
//.text:00B63DC0 ; bool __cdecl weapon_is_charged_plasma_pistol(int32, bool)
//.text:00B63E40 ; 
//.text:00B63EA0 ; bool __cdecl weapon_is_controlled_by_player(int32)
//.text:00B63F20 ; bool __cdecl weapon_is_depleted(int32)
//.text:00B64050 ; bool __cdecl weapon_is_dual_wielded(int32)
//.text:00B64140 ; bool __cdecl weapon_is_melee_only(int32)
//.text:00B64190 ; bool __cdecl weapon_is_ready_for_use(int32)

bool __cdecl weapon_is_support_weapon(int32 weapon_index)
{
	return INVOKE(0x00B641D0, weapon_is_support_weapon, weapon_index);
}

//.text:00B64220 ; bool __cdecl weapon_is_support_weapon_or_must_be_readied(int32)
//.text:00B64280 ; bool __cdecl weapon_is_tracking(int32)
//.text:00B64300 ; bool __cdecl weapon_is_zoomable(int32)
//.text:00B64350 ; void __cdecl weapon_magazine_execute_reload(int32, int16)
//.text:00B644D0 ; void __cdecl weapon_magazine_finish_reload(int32, int16, bool)
//.text:00B64540 ; e_unit_messages __cdecl weapon_magazine_get_reload_unit_message(int32, int16, bool)
//.text:00B64590 ; int32 __cdecl weapon_magazine_get_state(int32, int16)
//.text:00B645D0 ; bool __cdecl weapon_magazine_is_reloading(int32, int16, int16*)
//.text:00B64650 ; void __cdecl weapon_magazine_set_state(int32, int16, int16)
//.text:00B647E0 ; void __cdecl weapon_magazine_start_chamber(int32, int16)
//.text:00B648A0 ; bool __cdecl weapon_magazine_start_or_continue_reload(int32, int16)
//.text:00B64A70 ; bool __cdecl weapon_magazine_starting_continuous_reload(int32, int16)
//.text:00B64AB0 ; bool __cdecl weapon_magazine_state_change_ok(int32)
//.text:00B64B00 ; bool __cdecl weapon_magazine_state_interruptable(int16, int16)
//.text:00B64B30 ; bool __cdecl weapon_magazine_update(int32, int32)
//.text:00B64E00 ; bool __cdecl weapon_must_be_readied(int32)
//.text:00B64E50 ; bool __cdecl weapon_must_stay_awake_for_accuracy_penalties(int32)
//.text:00B64F20 ; bool __cdecl weapon_new(int32, object_placement_data*, bool*)
//.text:00B650F0 ; bool __cdecl weapon_overcharged(int32)
//.text:00B65130 ; bool __cdecl weapon_owner_submit_reload_action(int32, int16, bool)
//.text:00B65270 ; void __cdecl weapon_owner_update(int32, uns16, real32)
//.text:00B652F0 ; int32 __cdecl weapon_place(int32, const s_scenario_weapon*)
//.text:00B65390 ; bool __cdecl weapon_prevents_crouching(int32)
//.text:00B653E0 ; bool __cdecl weapon_prevents_equipment_throwing(int32)
//.text:00B65460 ; bool __cdecl weapon_prevents_grenade_throwing(int32)
//.text:00B654D0 ; bool __cdecl weapon_prevents_melee_attack(int32)
//.text:00B65530 ; bool __cdecl weapon_put_away(int32, bool, bool)
//.text:00B65950 ; void __cdecl weapon_ready(int32, bool, bool)
//.text:00B65C20 ; bool __cdecl weapon_reload(int32, int32)
//.text:00B65E30 ; 
//.text:00B66220 ; bool __cdecl weapon_reloading(int32, int32, int16*)
//.text:00B66310 ; 
//.text:00B663E0 ; void __cdecl weapon_report_kill(int32, int32)
//.text:00B664A0 ; void __cdecl weapon_reset(int32)
//.text:00B665B0 ; void __cdecl weapon_reset_and_idle_if_not_reloading(int32, int32)
//.text:00B665E0 ; void __cdecl weapon_reset_first_person_emulation(int32)
//.text:00B66640 ; int32 __cdecl weapon_respond_to_drop(int32)
//.text:00B66760 ; int16 __cdecl weapon_rotate_zoom_level(int32, int16)
//.text:00B667F0 ; void __cdecl weapon_send_message_to_unit(int32, e_unit_messages)
//.text:00B66870 ; void __cdecl weapon_set_current_amount(int32, real32)
//.text:00B669A0 ; void __cdecl weapon_set_initial_rounds(int32, int16, int16, int16)
//.text:00B66A80 ; void __cdecl weapon_set_initial_rounds_from_scenario_weapon(int32, const s_scenario_weapon*)
//.text:00B66B30 ; void __cdecl weapon_set_power(int32, real32)
//.text:00B66B90 ; void __cdecl weapon_set_predicted_age(int32, real32)
//.text:00B66BE0 ; void __cdecl weapon_set_predicted_rounds_loaded(int32, int16, int16, int16)
//.text:00B66CB0 ; void __cdecl weapon_set_projectile_error(int32, int32, real32)
//.text:00B66D00 ; bool __cdecl weapon_set_state(int32, int32, bool)
//.text:00B66F80 ; void __cdecl weapon_set_total_rounds(int32, int16*)
//.text:00B67050 ; bool __cdecl weapon_setup_first_person_emulation(int32, int32, int16)
//.text:00B67360 ; bool __cdecl weapon_state_interruptable(int32, int32)
//.text:00B67390 ; void __cdecl weapon_state_key_frame(int32)
//.text:00B67450 ; void __cdecl weapon_state_next(int32, bool)
//.text:00B674C0 ; bool __cdecl weapon_state_prevents_throwing(int32)
//.text:00B67500 ; void __cdecl weapon_steal_goodies(int32, int32)
//.text:00B67680 ; bool __cdecl weapon_stop_put_away(int32)
//.text:00B676D0 ; void __cdecl weapon_stop_reload(int32, bool)
//.text:00B67830 ; void __cdecl weapon_stow(int32, int32)
//.text:00B67880 ; bool __cdecl weapon_take_inventory_rounds(int32, int32, int32)
//.text:00B67A00 ; void __cdecl weapon_trigger_begin_charging(int32, int16)
//.text:00B67BF0 ; bool __cdecl weapon_trigger_can_begin_charging(int32, int16)
//.text:00B67C80 ; void __cdecl weapon_trigger_change_state(int32, int16, int16, int16)
//.text:00B67CD0 ; void __cdecl weapon_trigger_determine_analog_autofire_behavior(int32, int32, bool*, bool*, bool*)
//.text:00B67F10 ; void __cdecl weapon_trigger_fully_charged(int32, int16)
//.text:00B67FB0 ; real32 __cdecl weapon_trigger_get_charged_with_cooldown_fraction(int32, int16)
//.text:00B68080 ; void __cdecl weapon_trigger_idle(int32, int16)
//.text:00B680D0 ; void __cdecl weapon_trigger_overcharged(int32, int16)
//.text:00B68140 ; void __cdecl weapon_trigger_recover(int32, int16, bool)
//.text:00B68210 ; void __cdecl weapon_trigger_release_charge(int32, int16)
//.text:00B68460 ; bool __cdecl weapon_trigger_update(int32, int32)
//.text:00B68ED0 ; int32 __cdecl weapon_try_and_get_locked_sound_definition_index(int32)
//.text:00B68F30 ; int32 __cdecl weapon_try_and_get_tracking_sound_definition_index(int32)
//.text:00B68F90 ; void __cdecl weapon_unready(int32)
//.text:00B68FD0 ; bool __cdecl weapon_update(int32)
//.text:00B69710 ; bool __cdecl weapon_update_first_person_emulation(int32)
//.text:00B69870 ; bool __cdecl weapon_update_targeting(int32)
//.text:00B69A00 ; bool __cdecl weapon_useful(int32)
//.text:00B69A20 ; bool __cdecl weapon_uses_3rd_person_camera(int32)
//.text:00B69A70 ; bool __cdecl weapon_uses_charging_melee(int32)

void __cdecl weapons_debug_render()
{
	if (debug_weapons)
	{
		int32 output_user = player_mapping_first_active_output_user();
		int32 unit_index = player_mapping_get_unit_by_output_user(output_user);

		if (unit_index != NONE)
		{
			if (debug_weapons_primary)
			{
				unit_datum* unit = UNIT_GET(unit_index);
				int32 primary_weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_set.weapon_indices[0]);
				if (unit->object.parent_object_index != NONE)
				{
					object_datum* parent_object = object_get(unit->object.parent_object_index);
					if (TEST_BIT(_object_mask_unit, parent_object->object.object_identifier.get_type()))
					{
						if (((unit_datum*)parent_object)->unit.gunner_object_index == unit_index)
						{
							primary_weapon_index = unit_get_active_primary_weapon(unit->object.parent_object_index, nullptr);
						}
					}
				}

				if (primary_weapon_index != NONE)
					weapon_debug_render(primary_weapon_index, 0);
			}

			if (debug_weapons_secondary)
			{
				unit_datum* unit = UNIT_GET(unit_index);
				int32 secondary_weapon_index = unit_inventory_get_weapon(unit_index, unit->unit.current_weapon_set.weapon_indices[1]);
				if (secondary_weapon_index != NONE)
					weapon_debug_render(secondary_weapon_index, 1);
			}
		}
	}
}

void __cdecl weapons_debug_render_toggle()
{
	debug_weapons = !debug_weapons;
}

//.text:00B69AD0 ; void __cdecl weapons_fire_barrels()

