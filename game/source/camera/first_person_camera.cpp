#include "camera/first_person_camera.hpp"

#include "camera/director.hpp"
#include "cseries/cseries_events.hpp"
#include "game/game_globals.hpp"
#include "game/player_control.hpp"
#include "game/player_mapping.hpp"
#include "input/input_abstraction.hpp"
#include "interface/c_controller.hpp"
#include "interface/chud/cortana_effect.hpp"
#include "items/weapons.hpp"
#include "main/global_preferences.hpp"
#include "memory/module.hpp"
#include "scenario/scenario.hpp"
#include "units/bipeds.hpp"
#include "units/units.hpp"

#include <math.h>

HOOK_DECLARE_CLASS_MEMBER(0x0065F8A0, c_first_person_camera, update_);

void __cdecl first_person_camera_for_unit_and_vector(int32 unit_index, const real_vector3d* forward, s_observer_command* result)
{
	INVOKE(0x0065F5B0, first_person_camera_for_unit_and_vector, unit_index, forward, result);
}

void __thiscall c_first_person_camera::update_(int32 user_index, real32 dt, s_observer_command* result)
{
	ASSERT(result);
	
	int32 unit_index = player_mapping_get_unit_by_output_user(user_index);
	if (m_target_object_index != unit_index)
	{
		event(_event_error, "camera: first person camera #%d attached to object 0x%08X != user object 0x%08X, this should never happen",
			user_index,
			m_target_object_index,
			unit_index);

		set_target(unit_index);
	}

	e_controller_index controller_index = controller_index_from_user_index(user_index);
	if (m_allow_controlled_facing && controller_index != k_no_controller && !camera_input_inhibited(controller_index))
	{
		s_game_input_state* input_state = nullptr;
		real_euler_angles2d player_look_angular_velocity{};

		input_abstraction_get_input_state(controller_index, &input_state);
		input_abstraction_get_player_look_angular_velocity(controller_index, &player_look_angular_velocity);

		m_orientation_offset.yaw += ((input_state->yaw * dt) * player_look_angular_velocity.yaw);
		m_orientation_offset.pitch += ((input_state->pitch * dt) * player_look_angular_velocity.pitch);
		m_orientation_offset.pitch = fminf(fmaxf(m_orientation_offset.pitch, k_quarter_circle * -0.95f), k_quarter_circle * 0.95f);
	}

	if (m_target_object_index != NONE)
	{
		real_vector3d forward{};
		if (m_allow_controlled_facing)
		{
			vector3d_from_euler_angles2d(&forward, &m_orientation_offset);
			first_person_camera_for_unit_and_vector(m_target_object_index, &forward, result);
		}
		else
		{
			player_control_get_aiming_vector(user_index, &forward);
			first_person_camera_for_unit_and_vector(m_target_object_index, &forward, result);
		}
	
		if (controller_has_centered_crosshair(controller_index))
		{
			set_real_point2d(&result->crosshair_location, 1.0f, 0.0f);
		}
		else
		{
			if (scenario_get_game_globals()->player_control.count)
				set_real_point2d(&result->crosshair_location, 1.0f, scenario_get_game_globals()->player_control[0].crosshair_location.y);
		}

		real32 camera_fov = fminf(fmaxf(global_preferences_get_camera_fov(), 70.0f), k_horizontal_field_of_view_max * RAD);
	
		//c_first_person_view::m_fov_scale = 1.700000047683716f - camera_fov * 0.01f;
	
		result->field_of_view = unit_get_field_of_view(m_target_object_index, camera_fov * DEG, unit_get_zoom_level(m_target_object_index));
		result->field_of_view *= cortana_effect_get_fov_scale();
		result->field_of_view *= sub_A8E700(); // chud
		result->field_of_view = fminf(fmaxf(result->field_of_view, k_horizontal_field_of_view_min), k_horizontal_field_of_view_max);
	
		if (m_last_horizontal_field_of_view != result->field_of_view)
		{
			result->field_of_view_timer = weapon_get_field_of_view_change_time(unit_get_current_primary_weapon(m_target_object_index));
			result->field_of_view_flags = 1;
			m_last_horizontal_field_of_view = result->field_of_view;
		}
	
		if (TEST_BIT(_object_mask_biped, object_get_type(m_target_object_index)))
		{
			result->flags |= FLAG(13);
			biped_get_physics_pill(m_target_object_index, &result->pill_base, &result->pill_height, &result->pill_width);
		}
	
		object_get_center_of_mass(object_get_ultimate_parent(m_target_object_index), &result->safe_position);
		result->flags |= FLAG(10);
		result->flags |= FLAG(11);
		result->flags |= FLAG(0);
	}
	
	observer_validate_camera_command(result);
}

