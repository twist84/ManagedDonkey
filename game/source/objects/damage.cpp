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
long global_debug_damage_object_index = NONE;

void render_debug_object_damage()
{
	if (debug_damage_radius)
	{
		// #TODO: implement
	}

	if (debug_damage)
	{
		short_rectangle2d bounds{};
		interface_get_current_display_settings(NULL, NULL, NULL, &bounds);
		bounds.x0 += short(real(bounds.x1 - bounds.x0) * 0.7f);
		bounds.y0 += short(real(bounds.y1 - bounds.y0) * 0.1f);

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

				if (s_model_damage_info const* damage_info = object_get_damage_info(global_debug_damage_object_index))
				{
					if (damage_info->damage_sections.count() > 0)
					{
						long element_count = 0;
						struct object_damage_section* object_damage_section = (struct object_damage_section*)object_header_block_get_with_count(global_debug_damage_object_index, &object->object.damage_sections, sizeof(struct object_damage_section), &element_count);

						if (element_count > damage_info->damage_sections.count())
							element_count = damage_info->damage_sections.count();

						for (long i = 0; i < element_count; i++)
						{
							s_model_damage_section& damage_section = damage_info->damage_sections[i];

							csnzappendf(string, sizeof(string), "  %s %0.3f|n",
								damage_section.name.get_string(),
								real(1.0f - real(real(object_damage_section->__unknown2) / 255.0f)));
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

		if (input_key_frames_down(_key_code_spacebar, _input_type_game))
		{
			e_output_user_index user_index = c_player_view::get_current()->get_player_view_output_user_index();
			long unit_index = player_mapping_get_unit_by_output_user(user_index);

			real_matrix4x3 camera{};
			c_player_view::get_player_render_camera_orientation(&camera);
			vector3d forward{};
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

//real __cdecl compute_total_damage(struct s_damage_data* damage_data, struct s_damage_effect_definition* damage_effect_definition, struct damage_definition const* damage_definition, long object_index, bool* a5)
real __cdecl compute_total_damage(s_damage_data* damage_data, void* damage_effect_definition, void const* damage_definition, long object_index, bool* a5)
{
	real result = INVOKE(0x00B4FB10, compute_total_damage, damage_data, damage_effect_definition, damage_definition, object_index, a5);

	if (cheat.chevy && TEST_MASK(FLAG(object_get_type(object_index)), _object_mask_vehicle))
		result = 0.0f;

	return result;
};

void __cdecl damage_acceleration_apply(s_damage_globals::s_damage_acceleration const* damage_acceleration)
{
	INVOKE(0x00B4FD70, damage_acceleration_apply, damage_acceleration);
}

void __cdecl damage_acceleration_queue_begin()
{
	//INVOKE(0x00B50120, damage_acceleration_queue_begin);

	TLS_DATA_GET_VALUE_REFERENCE(damage_globals);

	damage_globals->damage_acceleration_queue_active = true;
}

void __cdecl damage_acceleration_queue_end()
{
	//INVOKE(0x00B50140, damage_acceleration_queue_end);

	TLS_DATA_GET_VALUE_REFERENCE(damage_globals);

	for (long i = 0; i < damage_globals->damage_acceleration_count; i++)
	{
		s_damage_globals::s_damage_acceleration& damage_acceleration = damage_globals->damage_accelerations[i];
		if (damage_acceleration.object_index != NONE)
			damage_acceleration_apply(&damage_acceleration);
	}

	damage_globals->damage_acceleration_count = 0;
	damage_globals->damage_accelerations_evictable.clear();
	damage_globals->damage_acceleration_queue_active = false;
}

void __cdecl damage_initialize_for_new_map()
{
	//INVOKE(0x00B50720, damage_initialize_for_new_map);

	TLS_DATA_GET_VALUE_REFERENCE(damage_globals);

	debug_damage_this_event = false;
	global_debug_damage_object_index = NONE;
	csmemset(damage_globals, 0, sizeof(s_damage_globals));
}

void __cdecl damage_update() // nullsub
{
	//INVOKE(0x00B51F70, damage_update);
}

void __cdecl object_cause_damage(s_damage_data* damage_data, long object_index, short node_index, short region_index, short material_index, long predictability)
{
	//INVOKE(0x00B532F0, object_cause_damage, damage_data, object_index, node_index, region_index, material_index, predictability);

	if (debug_damage_this_event = damage_data->damage_owner.player_index != NONE)
	{
		global_debug_damage_object_index = object_index;
		if (debug_damage_verbose)
		{
			object_datum* object = object_get(object_index);
			generate_event(_event_level_warning, "damaging '%s' 0x%08lx with '%s'",
				tag_name_strip_path(tag_get_name(object->definition_index)),
				object_index,
				tag_name_strip_path(tag_get_name(damage_data->damage_effect_definition_index)));
		}
	}

	HOOK_INVOKE(, object_cause_damage, damage_data, object_index, node_index, region_index, material_index, predictability);
}

s_model_damage_info const* __cdecl object_get_damage_info(long object_index)
{
	return INVOKE(0x00B578D0, object_get_damage_info, object_index);
}

