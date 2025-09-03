#include "rasterizer/rasterizer_performance_throttles.hpp"

#include "objects/objects.hpp"

REFERENCE_DECLARE_ARRAY(0x01917BA0, s_performance_throttles, c_performance_throttles::m_default_throttles, 4);
REFERENCE_DECLARE_ARRAY(0x01917C80, real32, c_performance_throttles::m_object_modifiers, k_object_type_count);
REFERENCE_DECLARE(0x05115AB4, bool, c_performance_throttles::m_ignore_predefined_performance_throttles);
REFERENCE_DECLARE(0x05115AB8, s_performance_throttles, c_performance_throttles::m_current_throttles);

s_performance_throttles* __cdecl c_performance_throttles::get_current_performance_throttles()
{
	//return INVOKE(0x00A5F8E0, c_performance_throttles::get_current_performance_throttles);

	return &m_current_throttles;
}

//.text:00A5F8F0 ; public: const s_tag_reference* __cdecl c_rasterizer_globals::get_default_performance_throttles_ref() const

real32 __cdecl get_performance_throttle(const char* name, int32 player_count)
{
	//INVOKE(0x00A5F900, get_performance_throttle, name, player_count);

	return c_performance_throttles::get_performance_throttle(name, player_count);
}

real32 __cdecl c_performance_throttles::get_performance_throttle(const char* name, int32 player_count)
{
	//INVOKE(0x00A5F910, c_performance_throttles::get_performance_throttle, name, player_count);

	if (IN_RANGE_INCLUSIVE(player_count, 1, 4))
	{
		s_performance_throttles& throttle = m_default_throttles[player_count - 1];

		if (csstricmp(name, "water") == 0)
			return throttle.water_lod;

		if (csstricmp(name, "decorator") == 0)
			return throttle.decorator_fade_distance_scale;

		if (csstricmp(name, "effect") == 0)
			return throttle.effects_lod_distance_scale;

		if (csstricmp(name, "instance") == 0)
			return throttle.instance_fade_modifier;

		if (csstricmp(name, "object_fade") == 0)
			return throttle.object_fade_modifier;

		if (csstricmp(name, "biped_modifier") == 0)
			return m_object_modifiers[_object_type_biped];

		if (csstricmp(name, "vehicle_modifier") == 0)
			return m_object_modifiers[_object_type_vehicle];

		if (csstricmp(name, "weapon_modifier") == 0)
			return m_object_modifiers[_object_type_weapon];

		if (csstricmp(name, "equipment_modifier") == 0)
			return m_object_modifiers[_object_type_equipment];

		if (csstricmp(name, "scenery_modifier") == 0)
			return m_object_modifiers[_object_type_scenery];

		if (csstricmp(name, "machine_modifier") == 0)
			return m_object_modifiers[_object_type_machine];

		if (csstricmp(name, "crate_modifier") == 0)
			return m_object_modifiers[_object_type_crate];

		if (csstricmp(name, "object_lod") == 0)
			return throttle.object_lod_modifier;

		if (csstricmp(name, "decals") == 0)
			return throttle.decal_fade_distance_scale;

		if (csstricmp(name, "cpu_light_count") == 0)
			return (real32)throttle.cpu_dynamic_light_max_count;

		if (csstricmp(name, "gpu_light_count") == 0)
			return (real32)throttle.gpu_dynamic_light_max_count;

		if (csstricmp(name, "shadow_count") == 0)
			return (real32)throttle.shadow_generate_count;

		if (csstricmp(name, "shadow_quality") == 0)
			return throttle.shadow_quality_lod;
	}

	return -1.0f;
}

//.text:00A5FBB0 ; 

void __cdecl set_performance_throttle(const char* name, int32 player_count, real32 value)
{
	//INVOKE(0x00A5FBE0, set_performance_throttle, name, player_count, value);

	c_performance_throttles::set_performance_throttle(name, player_count, value);
}

void __cdecl c_performance_throttles::set_performance_throttle(const char* name, int32 player_count, real32 value)
{
	//INVOKE(0x00A5FC00, c_performance_throttles::set_performance_throttle, name, player_count, value);

	if (IN_RANGE_INCLUSIVE(player_count, 1, 4))
	{
		s_performance_throttles& throttle = m_default_throttles[player_count - 1];

		if (csstricmp(name, "water") == 0)
		{
			throttle.water_lod = value;
			return;
		}

		if (csstricmp(name, "decorator") == 0)
		{
			throttle.decorator_fade_distance_scale = value;
			return;
		}

		if (csstricmp(name, "effect") == 0)
		{
			throttle.effects_lod_distance_scale = value;
			return;
		}

		if (csstricmp(name, "instance") == 0)
		{
			throttle.instance_fade_modifier = value;
			return;
		}

		if (csstricmp(name, "object_fade") == 0)
		{
			throttle.object_fade_modifier = value;
			return;
		}

		if (csstricmp(name, "biped_modifier") == 0)
		{
			m_object_modifiers[_object_type_biped] = value;
			return;
		}

		if (csstricmp(name, "vehicle_modifier") == 0)
		{
			m_object_modifiers[_object_type_vehicle] = value;
			return;
		}

		if (csstricmp(name, "weapon_modifier") == 0)
		{
			m_object_modifiers[_object_type_weapon] = value;
			return;
		}

		if (csstricmp(name, "equipment_modifier") == 0)
		{
			m_object_modifiers[_object_type_equipment] = value;
			return;
		}

		if (csstricmp(name, "scenery_modifier") == 0)
		{
			m_object_modifiers[_object_type_scenery] = value;
			return;
		}

		if (csstricmp(name, "machine_modifier") == 0)
		{
			m_object_modifiers[_object_type_machine] = value;
			return;
		}

		if (csstricmp(name, "crate_modifier") == 0)
		{
			m_object_modifiers[_object_type_crate] = value;
			return;
		}

		if (csstricmp(name, "object_lod") == 0)
		{
			throttle.object_lod_modifier = value;
			return;
		}

		if (csstricmp(name, "decals") == 0)
		{
			throttle.decal_fade_distance_scale = value;
			return;
		}

		if (csstricmp(name, "cpu_light_count") == 0)
		{
			throttle.cpu_dynamic_light_max_count = (int32)value;
			return;
		}

		if (csstricmp(name, "gpu_light_count") == 0)
		{
			throttle.gpu_dynamic_light_max_count = (int32)value;
			return;
		}

		if (csstricmp(name, "shadow_count") == 0)
		{
			throttle.shadow_generate_count = (int32)value;
			return;
		}

		if (csstricmp(name, "shadow_quality") == 0)
		{
			throttle.shadow_quality_lod = value;
			return;
		}
	}
}

void __cdecl c_performance_throttles::update_current_performance_throttles()
{
	INVOKE(0x00A5FF00, c_performance_throttles::update_current_performance_throttles);
}

