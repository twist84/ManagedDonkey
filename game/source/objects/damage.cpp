#include "objects/damage.hpp"

#include "cache/cache_files.hpp"
#include "cseries/cseries_events.hpp"
#include "game/cheats.hpp"
#include "game/player_mapping.hpp"
#include "interface/interface_constants.hpp"
#include "memory/module.hpp"
#include "memory/thread_local.hpp"
#include "models/model_definitions.hpp"
#include "physics/collisions.hpp"
#include "render/views/render_view.hpp"
#include "text/draw_string.hpp"
#include "units/bipeds.hpp"

#include <string.h>

HOOK_DECLARE_CALL(0x00B4F3FC, compute_total_damage);
HOOK_DECLARE_CALL(0x00B53410, compute_total_damage);
HOOK_DECLARE(0x00B50720, damage_initialize_for_new_map);
HOOK_DECLARE(0x00B532F0, object_cause_damage);

bool debug_damage_radius = false;
bool debug_damage_this_event = false;
bool debug_damage_verbose = false;
bool debug_player_damage = false;
bool debug_damage = false;
int32 global_debug_damage_object_index = NONE;

//int32 __cdecl area_of_effect_cause_damage(s_damage_data*, int32, e_predictability)
int32 __cdecl area_of_effect_cause_damage(s_damage_data* damage_data, int32 ignore_object_index, int32 predictability)
{
	return INVOKE(0x00B4E810, area_of_effect_cause_damage, damage_data, ignore_object_index, predictability);
}

//real32 __cdecl compute_total_damage(s_damage_data* damage_data, s_damage_effect_definition* damage_effect_definition, const damage_definition* damage_definition, int32 object_index, bool* a5)
real32 __cdecl compute_total_damage(s_damage_data* damage_data, void* damage_effect_definition, const void* damage_definition, int32 object_index, bool* a5)
{
	real32 result = INVOKE(0x00B4FB10, compute_total_damage, damage_data, damage_effect_definition, damage_definition, object_index, a5);

	if (cheat.deathless_player && TEST_BIT(_object_mask_biped, object_get_type(object_index)))
	{
		if (object_index == player_mapping_get_unit_by_output_user(0))
			result = 0.0f;
	}

	if (cheat.jetpack && TEST_BIT(_object_mask_biped, object_get_type(object_index)))
	{
		if (damage_data->damage_reporting_info.type == _damage_reporting_type_generic_collision_damage)
			result = 0.0f;
	}

	if (cheat.chevy && TEST_BIT(_object_mask_vehicle, object_get_type(object_index)))
		result = 0.0f;

	return result;
};

void __cdecl damage_acceleration_apply(const s_damage_globals::s_damage_acceleration* damage_acceleration)
{
	INVOKE(0x00B4FD70, damage_acceleration_apply, damage_acceleration);
}

void __cdecl damage_acceleration_queue_begin()
{
	//INVOKE(0x00B50120, damage_acceleration_queue_begin);

	damage_globals->damage_acceleration_queue_active = true;
}

void __cdecl damage_acceleration_queue_end()
{
	//INVOKE(0x00B50140, damage_acceleration_queue_end);

	for (int32 damage_acceleration_index = 0; damage_acceleration_index < damage_globals->damage_acceleration_count; damage_acceleration_index++)
	{
		s_damage_globals::s_damage_acceleration& damage_acceleration = damage_globals->damage_accelerations[damage_acceleration_index];
		if (damage_acceleration.object_index != NONE)
		{
			damage_acceleration_apply(&damage_acceleration);
		}
	}

	damage_globals->damage_acceleration_count = 0;
	damage_globals->damage_accelerations_evictable.clear();
	damage_globals->damage_acceleration_queue_active = false;
}

void __cdecl damage_data_new(s_damage_data* damage_data, int32 definition_index)
{
	INVOKE(0x00B50330, damage_data_new, damage_data, definition_index);
}

void __cdecl damage_initialize_for_new_map()
{
	//INVOKE(0x00B50720, damage_initialize_for_new_map);

	debug_damage_this_event = false;
	global_debug_damage_object_index = NONE;
	csmemset(damage_globals, 0, sizeof(s_damage_globals));
}

void __cdecl damage_update() // nullsub
{
	//INVOKE(0x00B51F70, damage_update);
}

void __cdecl object_damage_damage_section(int32 object_index, int32 damage_section_string_id, real32 damage)
{
	INVOKE(0x00B55030, object_damage_damage_section, object_index, damage_section_string_id, damage);
}

bool __cdecl object_model_target_destroyed(int32 object_index, int32 model_target_name_id)
{
	return INVOKE(0x00B58150, object_model_target_destroyed, object_index, model_target_name_id);
}

int16 __cdecl object_model_targets_destroyed(int32 object_index, int32 model_target_name_id)
{
	return INVOKE(0x00B58170, object_model_targets_destroyed, object_index, model_target_name_id);
}

//.text:00B58250 ; 

void __cdecl object_scripting_cannot_die(int32 object_index, bool cannot_die)
{
	INVOKE(0x00B582E0, object_scripting_cannot_die, object_index, cannot_die);
}

bool __cdecl object_scripting_vitality_pinned(int32 object_index)
{
	return INVOKE(0x00B58340, object_scripting_vitality_pinned, object_index);
}

void __cdecl object_can_take_damage(int32 object_list_index)
{
	INVOKE(0x00B532B0, object_can_take_damage, object_list_index);
}

void __cdecl object_cannot_take_damage(int32 object_list_index)
{
	INVOKE(0x00B532D0, object_cannot_take_damage, object_list_index);
}

void __cdecl object_cause_damage(s_damage_data* damage_data, int32 object_index, int16 node_index, int16 region_index, int16 material_index, int32 predictability)
{
	//INVOKE(0x00B532F0, object_cause_damage, damage_data, object_index, node_index, region_index, material_index, predictability);

	if (debug_damage_this_event = damage_data->damage_owner.player_index != NONE)
	{
		global_debug_damage_object_index = object_index;
		if (debug_damage_verbose)
		{
			object_datum* object = object_get(object_index);
			event(_event_warning, "damaging '%s' 0x%08lx with '%s'",
				tag_name_strip_path(tag_get_name(object->definition_index)),
				object_index,
				tag_name_strip_path(tag_get_name(damage_data->definition_index)));
		}
	}

	HOOK_INVOKE(, object_cause_damage, damage_data, object_index, node_index, region_index, material_index, predictability);
}

__declspec(naked) void omnipotent_inline()
{
	// original instructions
	//      test    dword ptr [edi+0x4], 0x40000

	// all this for the following
	//if (cheat.omnipotent && damage_data->damage_owner.damage_owner_player_index != NONE)
	//    v106 = true;

	ASM_ADDR(0x00B53A72, addr_B53A72);

	__asm
	{
		// execute our instructions
		cmp     cheat.omnipotent, 0
		jz      short loc_B53A6B
		cmp     dword ptr[edi+ 0x8], NONE
		jz      short loc_B53A6B
		mov     byte ptr[ebp-0x30], 1

		// execute the original instructions
	loc_B53A6B:
		test    dword ptr[edi+0x4], 0x40000

		// jump out to after our hook
		jmp     addr_B53A72
	}
}
HOOK_DECLARE(0x00B53A6B, omnipotent_inline);

//__declspec(naked) void deathless_inline1()
//{
//	// original instructions
//	//      test    edx, _object_mask_unit
//	//      jz      short loc_B53B25
//
//	ASM_ADDR(0x00B53AFB, addr_B53AFB);
//	ASM_ADDR(0x00B53B25, addr_B53B25);
//
//	__asm
//	{
//		// execute the original instructions
//		test    edx, _object_mask_unit
//		jz      short loc_B53B25
//
//		// execute our instructions
//		cmp     cheat.deathless, 0
//		jnz     short loc_B53B25
//		jmp     addr_B53AFB
//
//		// jump out to after our hook
//	loc_B53B25:
//		jmp     addr_B53B25
//	}
//}
//HOOK_DECLARE(0x00B53AF3, deathless_inline1);

//void __cdecl object_cause_damage_simple(s_damage_data* damage_data, int32 object_index, e_predictability predictability)
void __cdecl object_cause_damage_simple(s_damage_data* damage_data, int32 object_index, int32 predictability)
{
	INVOKE(0x00B542A0, object_cause_damage_simple, damage_data, object_index, predictability);
}

const s_model_damage_info* __cdecl object_get_damage_info(int32 object_index)
{
	return INVOKE(0x00B578D0, object_get_damage_info, object_index);
}

void __cdecl object_deplete_body(int32 object_index, const s_damage_owner* depleter, const s_damage_reporting_info& damage_reporting_info)
{
	INVOKE(0x00B56B20, object_deplete_body, object_index, depleter, damage_reporting_info);
}

void render_debug_object_damage()
{
	if (debug_damage_radius)
	{
		// $IMPLEMENT
	}

	if (debug_damage)
	{
		rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		bounds.x0 += int16((real32)rectangle2d_width(&bounds) * 0.7f);
		bounds.y0 += int16((real32)rectangle2d_height(&bounds) * 0.1f);

		char string[2048]{};
		if (global_debug_damage_object_index == NONE)
		{
			csnzprintf(string, sizeof(string), "no object to debug|n(point and press space)");
		}
		else
		{
			if (object_datum* object = object_get(global_debug_damage_object_index))
			{
				csnzprintf(string, sizeof(string), "%s|nbody %0.3f|n  current %0.3f|n  recent %0.3f|nshield %0.3f|n  current %0.3f|n  recent %0.3f|n",
					strrchr(tag_get_name(object->definition_index), '\\'),
					object->object.body_vitality,
					object->object.current_shield_damage,
					object->object.recent_shield_damage,
					object->object.shield_vitality,
					object->object.current_body_damage,
					object->object.recent_body_damage);

				if (const s_model_damage_info* damage_info = object_get_damage_info(global_debug_damage_object_index))
				{
					if (damage_info->damage_sections.count > 0)
					{
						int32 element_count = 0;
						struct object_damage_section* object_damage_section = OBJECT_HEADER_BLOCK_GET_WITH_COUNT(global_debug_damage_object_index, struct object_damage_section, &object->object.damage_sections, &element_count);

						if (element_count > damage_info->damage_sections.count)
							element_count = damage_info->damage_sections.count;

						for (int32 i = 0; i < element_count; i++)
						{
							s_model_damage_section& damage_section = damage_info->damage_sections[i];

							csnzappendf(string, sizeof(string), "  %s %0.3f|n",
								damage_section.name.get_string(),
								real32(1.0f - real32(real32(object_damage_section->current_damage) / 255.0f)));
						}
					}
				}
			}
			else
			{
				global_debug_damage_object_index = NONE;
			}
		}

		c_rasterizer_draw_string draw_string;
		c_font_cache_mt_safe font_cache;

		draw_string.set_bounds(&bounds);
		draw_string.draw(&font_cache, string);

		if (input_key_frames_down(_key_space, _input_type_game))
		{
			int32 user_index = c_player_view::get_current()->get_player_view_user_index();
			int32 unit_index = player_mapping_get_unit_by_output_user(user_index);

			real_matrix4x3 camera{};
			c_player_view::get_player_render_camera_orientation(&camera);
			real_vector3d forward{};
			scale_vector3d(&camera.forward, 50.0f, &forward);

			s_collision_test_flags flags{};
			flags.collision_flags.set(_collision_test_front_facing_surfaces_bit, true);
			flags.object_flags.set(_collision_test_objects_bit, true);

			collision_result collision{};
			if (collision_test_vector(flags, &camera.position, &forward, unit_index, NONE, &collision))
			{
				ASSERT(collision.type == _collision_result_object);
				global_debug_damage_object_index = collision.object_index;
			}
		}
	}
}

