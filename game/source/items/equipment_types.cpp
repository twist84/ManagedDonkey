#include "items/equipment_types.hpp"

#include "ai/ai.hpp"
#include "cache/cache_files.hpp"
#include "game/game.hpp"
#include "items/equipment.hpp"
#include "items/equipment_definitions.hpp"
#include "physics/havok_component.hpp"
#include "simulation/game_interface/simulation_game_action.hpp"
#include "sound/game_sound.hpp"

class c_equipment_type_authoritative :
	public c_equipment_type
{
public:
	virtual void activate_local_predicted(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void deactivate_local_predicted(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void activate_local_authoritative(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void deactivate_local_authoritative(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void handle_activation_request(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void handle_deactivation_request(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_activation_message_remote(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_deactivation_message_remote(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_activation_message_local(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_deactivation_message_local(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void activate(int32 equipment_index) const {}
	virtual void deactivate(int32 equipment_index) const {}
};

class c_equipment_type_multiplayer_powerup :
	public c_equipment_type
{
public:
};

class c_equipment_type_spawner :
	public c_equipment_type
{
public:
	virtual void activate_local_predicted(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void activate_local_authoritative(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual bool update(int32 equipment_index) const override
	{
		// $IMPLEMENT
		return false;
	}
	void spawn_equipment(int32 equipment_index) const
	{
		// $IMPLEMENT
	}
};

class c_equipment_type_proximity_mine :
	public c_equipment_type
{
public:
	virtual bool compute_function_value(int32 object_index, int32 function, int32 function_owner_definition_index, real32* value, bool* active, bool* deterministic) const override
	{
		// $IMPLEMENT
		return false;
	}
	virtual bool update(int32 equipment_index) const override
	{
		equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
		const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);

		bool waiting_to_self_destruct = false;
		if (equipment_is_active(equipment_index) && !game_is_predicted())
		{
			const s_equipment_type_proximity_mine* proximity_mine = equipment_get_proximity_mine_definition(equipment->definition_index);
			const real32 age = game_ticks_to_seconds((real32)(game_time_get() - equipment->equipment.last_use_time));
			ASSERT(age >= 0.0f);

			if (proximity_mine->self_destruct_time != 0.0f && age > proximity_mine->self_destruct_time)
			{
				waiting_to_self_destruct = true;

				object_set_at_rest(equipment_index, false);
				if (equipment->object.havok_component_index != NONE)
				{
					c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
					havok_component->wake_all_bodies_in_phantoms();
				}
				if (equipment_definition->equipment.deactivate_sound.index != NONE)
				{
					object_impulse_sound_new(
						equipment_index,
						equipment_definition->equipment.deactivate_sound.index,
						NONE,
						global_origin3d,
						global_forward3d,
						1.0f);
				}
				s_damage_owner damage_owner = equipment->equipment.creator_damage_owner;
				s_damage_reporting_info damage_reporting_info = { .type = _damage_reporting_type_generic_explosion };
				object_deplete_body(equipment_index, &damage_owner, damage_reporting_info);
			}
			else if (age > proximity_mine->arm_time)
			{
				bool object_moving_in_proximity = false;
				if (equipment->object.havok_component_index != NONE)
				{
					c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
					int32 phantom_body_count = havok_component->get_bodies_in_phantoms_count();
					for (int32 phantom_body_index = 0; phantom_body_index < phantom_body_count; phantom_body_index++)
					{
						int32 body_object_index;
						int32 body_physics_model_material_index;
						int32 body_rigid_body_index;
						havok_component->get_bodies_in_phantom_info(
							phantom_body_index,
							&body_object_index,
							&body_physics_model_material_index,
							&body_rigid_body_index);
						if (body_object_index != NONE && body_object_index != equipment_index)
						{
							c_havok_component* body_havok_component = DATUM_GET(g_havok_component_data, c_havok_component, OBJECT_GET(body_object_index)->object.havok_component_index);

							real_vector3d linear_velocity{};
							object_get_localized_velocities(body_havok_component->get_object_index(), &linear_velocity, NULL, NULL, NULL);
							if (magnitude_squared3d(&linear_velocity) >= proximity_mine->minimum_velocity_to_trigger * proximity_mine->minimum_velocity_to_trigger)
							{
								object_moving_in_proximity = true;
								break;
							}
						}
					}
				}

				if (object_moving_in_proximity)
				{
					equipment->equipment.proximity_triggered_counter = FLOOR(equipment->equipment.proximity_triggered_counter + 1, 254);
					if (game_ticks_to_seconds((real32)equipment->equipment.proximity_triggered_counter) > proximity_mine->trigger_time)
					{
						s_damage_reporting_info damage_reporting_info = { .type = _damage_reporting_type_prox_mine };
						if (proximity_mine->explosion_effect.index != NONE)
						{
							real_point3d center_of_mass{};
							object_get_center_of_mass(equipment_index, &center_of_mass);
							effect_new_from_point_vector(
								proximity_mine->explosion_effect.index,
								&center_of_mass,
								global_up3d,
								global_up3d,
								_match_all_markers,
								_effect_deterministic,
								NULL,
								&equipment->object.location);
						}
						if (proximity_mine->explosion_damage_effect.index != NONE)
						{
							s_damage_data damage_data{};
							damage_data_new(&damage_data, proximity_mine->explosion_damage_effect.index);
							damage_data.location = equipment->object.location;
							object_get_center_of_mass(equipment_index, &damage_data.origin);
							damage_data.epicenter = damage_data.origin;
							damage_data.direction = equipment->object.forward;
							damage_data.area_of_effect_forward = damage_data.direction;
							damage_data.damage_reporting_info = damage_reporting_info;
							damage_data.damage_owner = equipment->equipment.creator_damage_owner;
							area_of_effect_cause_damage(&damage_data, NONE, 5);
						}
						object_deplete_body(equipment_index, global_damage_owner_unknown, damage_reporting_info);
					}
				}
				else
				{
					equipment->equipment.proximity_triggered_counter = 0;
				}
			}
		}
		return waiting_to_self_destruct;
	}
};

class c_equipment_type_motion_tracker_noise :
	public c_equipment_type
{
public:
	virtual bool update(int32 equipment_index) const override
	{
		equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
		const s_equipment_type_motion_tracker_noise* motion_tracker_noise = equipment_get_motion_tracker_noise_definition(equipment->definition_index);
		real_point3d equipment_origin{};
		object_get_center_of_mass(equipment_index, &equipment_origin);
		chud_add_noisemaker_zone(
			equipment_index,
			&equipment_origin,
			motion_tracker_noise->noise_radius,
			motion_tracker_noise->flash_radius,
			motion_tracker_noise->noise_count);
		ai_handle_noisemaker(equipment_index);

		return false;
	}
};

class c_equipment_type_invincibility_mode :
	public c_equipment_type_authoritative
{
public:
	virtual void deactivate_local_predicted(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void deactivate_local_authoritative(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void handle_deactivation_request(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_deactivation_message_remote(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void apply_deactivation_message_local(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual bool update(int32 equipment_index) const override
	{
		// $IMPLEMENT
		return false;
	}
	virtual void update(int32 equipment_index, int32 owner_unit_index) const override
	{
		equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
		const struct equipment_definition* equipment_definition = TAG_GET(EQUIPMENT_TAG, const struct equipment_definition, equipment->definition_index);
		const s_equipment_type_invincibility* invincibility = equipment_get_invincibility_mode_definition(equipment->definition_index);

		if (equipment_is_active(equipment_index))
		{
			int32 age = equipment_get_age(equipment_index);
			int32 recharge_time = game_seconds_to_ticks_round(invincibility->shield_recharge_time) - 1;
			int32 duration_ticks = game_seconds_to_ticks_round(equipment_definition->equipment.duration);
			if (!age)
			{
				if (invincibility->activation_effect.index != NONE)
				{
					s_damage_reporting_info damage_reporting_info{};
					effect_new_from_object(invincibility->activation_effect.index, NULL, damage_reporting_info, owner_unit_index, 0.0f, 0.0f, NULL, NULL, _effect_deterministic);
				}
			}

			if (age + 1 == duration_ticks)
			{
				//// $TODO add back the `ending_effect` tag reference within `s_equipment_type_invincibility`
				//if (invincibility->ending_effect.index != NONE)
				//{
				//	s_damage_reporting_info damage_reporting_info{};
				//	effect_new_from_object(invincibility->ending_effect.index, NULL, damage_reporting_info, owner_unit_index, 0.0, 0.0, NULL, NULL, _effect_deterministic);
				//}

				// sub_B888E0
				if (invincibility->ongoing_effect.index != NONE)
				{
					s_damage_reporting_info damage_reporting_info{};
					effect_new_from_object(
						invincibility->ongoing_effect.index,
						NULL,
						damage_reporting_info,
						owner_unit_index,
						0.0f,
						0.0f,
						NULL,
						NULL,
						_effect_deterministic);
				}
			}

			if (invincibility->ongoing_effect.index != NONE)
			{
				if (equipment->equipment.looping_effect_index == NONE)
				{
					s_damage_reporting_info damage_reporting_info{};
					equipment->equipment.looping_effect_index = effect_new_from_object(
						invincibility->ongoing_effect.index,
						NULL,
						damage_reporting_info,
						owner_unit_index,
						0.0f,
						0.0f,
						NULL,
						NULL,
						_effect_not_deterministic);
				}
				else
				{
					effect_ping(
						equipment->equipment.looping_effect_index,
						invincibility->ongoing_effect.index,
						0.0f,
						0.0f,
						false);
				}
			}

			if (IN_RANGE_INCLUSIVE(age, 0, recharge_time) && owner_unit_index != NONE)
			{
				unit_datum* owner_unit = UNIT_GET(owner_unit_index);
				if (recharge_time <= 0)
				{
					owner_unit->object.shield_vitality = 1.0f;
				}
				else
				{
					real32 fraction = (1.0f / recharge_time) + owner_unit->object.shield_vitality;
					if (fraction > 1.0f)
					{
						fraction = 1.0f;
					}
					owner_unit->object.shield_vitality = fraction;
				}
			}
		}
	}
	virtual void activate(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
	virtual void deactivate(int32 equipment_index) const override
	{
		// $IMPLEMENT
	}
};

class c_equipment_type_treeoflife :
	public c_equipment_type
{
public:
	virtual void equipment_new(int32 equipment_index) const override
	{
		//if (game_is_authoritative())
		//{
		//	equipment_activate(equipment_index, _equipment_activation_local);
		//}
	}
	virtual bool update(int32 equipment_index) const override
	{
		bool updated = equipment_active_fraction(equipment_index) > 0.0f;
		if (updated)
		{
			if (!game_is_predicted())
			{
				equipment_datum* equipment = EQUIPMENT_GET(equipment_index);
				if (equipment->object.havok_component_index != NONE)
				{
					const c_havok_component* havok_component = DATUM_GET(g_havok_component_data, c_havok_component, equipment->object.havok_component_index);
					int32 phantom_body_count = havok_component->get_bodies_in_phantoms_count();
					for (int32 phantom_body_index = 0; phantom_body_index < phantom_body_count; phantom_body_index++)
					{
						int32 body_object_index;
						int32 body_physics_model_material_index;
						int32 body_rigid_body_index;
						havok_component->get_bodies_in_phantom_info(
							phantom_body_index,
							&body_object_index,
							&body_physics_model_material_index,
							&body_rigid_body_index);
						if (body_object_index != NONE && body_object_index != equipment_index)
						{
							unit_datum* unit = UNIT_GET(body_object_index);
							unit->object.shield_stun_ticks = 0;
							unit->object.body_stun_ticks = 0;
						}
					}
				}
			}
		}
		return updated;
	}
};

c_equipment_type g_equipment_type_super_shield_instance;
c_equipment_type* g_equipment_type_super_shield = &g_equipment_type_super_shield_instance;
c_equipment_type_multiplayer_powerup g_equipment_type_multiplayer_powerup_instance;
c_equipment_type* g_equipment_type_multiplayer_powerup = &g_equipment_type_multiplayer_powerup_instance;
c_equipment_type_spawner g_equipment_type_spawner_instance;
c_equipment_type* g_equipment_type_spawner = &g_equipment_type_spawner_instance;
c_equipment_type_proximity_mine g_equipment_type_proximity_mine_instance;
c_equipment_type* g_equipment_type_proximity_mine = &g_equipment_type_proximity_mine_instance;
c_equipment_type_motion_tracker_noise g_equipment_type_motion_tracker_noise_instance;
c_equipment_type* g_equipment_type_motion_tracker_noise = &g_equipment_type_motion_tracker_noise_instance;
c_equipment_type g_equipment_type_showme_instance;
c_equipment_type* g_equipment_type_showme = &g_equipment_type_showme_instance;
c_equipment_type g_equipment_type_invisibility_mode_instance;
c_equipment_type* g_equipment_type_invisibility_mode = &g_equipment_type_invisibility_mode_instance;
c_equipment_type_invincibility_mode g_equipment_type_invincibility_mode_instance;
c_equipment_type* g_equipment_type_invincibility_mode = &g_equipment_type_invincibility_mode_instance;
c_equipment_type_treeoflife g_equipment_type_treeoflife_instance;
c_equipment_type* g_equipment_type_treeoflife = &g_equipment_type_treeoflife_instance;

c_equipment_type* g_equipment_types[k_equipment_type_count]
{
	g_equipment_type_super_shield,
	g_equipment_type_multiplayer_powerup,
	g_equipment_type_spawner,
	g_equipment_type_proximity_mine,
	g_equipment_type_motion_tracker_noise,
	g_equipment_type_showme,
	g_equipment_type_invisibility_mode,
	g_equipment_type_invincibility_mode,
	g_equipment_type_treeoflife,
};

