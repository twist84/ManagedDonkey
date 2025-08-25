#include "profiler/profiler.hpp"

#include "ai/ai_profile.hpp"
#include "interface/interface.hpp"
#include "main/console.hpp"
#include "objects/objects.hpp"
#include "physics/havok_profile.hpp"
#include "render/render_visibility.hpp"
#include "text/draw_string.hpp"

bool profile_summary_objects_enabled = true;
bool profile_summary_effects_enabled = true;
bool profile_summary_ai_enabled = true;
bool profile_summary_enabled = false;
bool profile_summary_graphics_enabled = false;
bool profile_summary_environment_artist_enabled = false;
e_profile_summary_modes g_profile_summary_mode = _profile_summary_all;

const char* const profile_dump_directory = "profiling";
const char* const k_profile_summary_names[k_profile_summary_count]
{
	"all",
	"objects",
	"graphics",
	"effects",
	"ai",
	"game-state",
	"environment artist"
};

void profiler_initialize()
{
	//INVOKE(0x00530230, profiler_initialize);
}

void profiler_dispose()
{
	//INVOKE(0x00530240, profiler_dispose);
}

void profiler_initialize_for_new_map()
{
	//INVOKE(0x00530250, profiler_initialize_for_new_map);
}

void profiler_dispose_from_old_map()
{
	//INVOKE(0x00530260, profiler_dispose_from_old_map);
}

void profile_render(const rectangle2d* screen_pixel_bounds, const rectangle2d* screen_safe_pixel_bounds)
{
	ASSERT(screen_pixel_bounds && screen_safe_pixel_bounds);

	c_rasterizer_draw_string draw_string;
	c_font_cache_mt_safe font_cache;

	if (profile_summary_enabled)
	{
		interface_set_bitmap_text_draw_mode(&draw_string, _terminal_font, _text_style_plain, _text_justification_left, 0, 5, 0);
		
		objects_information objects_information{};
		objects_information_get(&objects_information);

		s_system_memory_information memory_information{};
		system_memory_information_get(&memory_information);

		/*
		effects_information effects_information{};
		effects_information effects_information_by_priority[3]{};
		effects_information_get(&effects_information, effects_information_by_priority);

		s_beam_information_by_priority* beam_information_by_priority = g_beam_information_by_priority;
		c_beam_system::count_by_priority();

		s_contrail_information_by_priority* contrail_information_by_priority = g_contrail_information_by_priority;
		c_contrail_system::count_by_priority();

		s_light_volume_information_by_priority* light_volume_information_by_priority = g_light_volume_information_by_priority;
		c_light_volume_system::count_by_priority();

		s_particle_information_by_priority* particle_information_by_priority = g_particle_information_by_priority;
		c_particle_system::count_all_by_priority();
		*/

		char buffer[8192]{};
		csstrnzcpy(buffer, "", sizeof(buffer));

		if (profile_summary_objects_enabled)
		{
			real32 free_memory = real32(memory_information.free);
			if (memory_information.free < 0)
				free_memory += 1.8446744e19f;

			real32 total_memory = real32(memory_information.total);
			if (memory_information.total < 0)
				total_memory += 1.8446744e19f;

			csnzappendf(buffer, sizeof(buffer), "% 6.1fk free of % 6.1fk total|n",
				free_memory * 0.0009765625f,
				total_memory * 0.0009765625f);

			csnzappendf(buffer, sizeof(buffer), "% 5d active of % 5d objects (% 3.1f%% used)|n",
				objects_information.active_object_count,
				objects_information.object_count,
				objects_information.used_memory * 100.0f);
			
			csnzappendf(buffer, sizeof(buffer), "% 5d active-garbage % 5d slots-free % 3.1f%% contiguous-free|n",
				objects_information.active_garbage_count,
				objects_information.free_object_count,
				objects_information.contiguous_free_memory * 100.0f);
			
			csnzappendf(buffer, sizeof(buffer), "%d/%d/%d/%d bipeds %d/%d/%d/%d creatures %d/%d/%d/%d vehicles|n",
				object_count(_object_mask_biped, 0x4),
				object_count(_object_mask_biped, 0x3),
				object_count(_object_mask_biped, 0x1),
				object_count(_object_mask_biped, 0x0),
				object_count(_object_mask_creature, 0x4),
				object_count(_object_mask_creature, 0x3),
				object_count(_object_mask_creature, 0x1),
				object_count(_object_mask_creature, 0x0),
				object_count(_object_mask_vehicle, 0x4),
				object_count(_object_mask_vehicle, 0x3),
				object_count(_object_mask_vehicle, 0x1),
				object_count(_object_mask_vehicle, 0x0));
			
			csnzappendf(buffer, sizeof(buffer), "%d/%d/%d/%d items %d/%d/%d/%d crates|n",
				object_count(_object_mask_item, 0x4),
				object_count(_object_mask_item, 0x3),
				object_count(_object_mask_item, 0x1),
				object_count(_object_mask_item, 0x0),
				object_count(_object_mask_crate, 0x4),
				object_count(_object_mask_crate, 0x3),
				object_count(_object_mask_crate, 0x1),
				object_count(_object_mask_crate, 0x0));
			
			havok_profile_display(buffer, sizeof(buffer));
		}
		
		if (profile_summary_effects_enabled)
		{
			csnzappendf(buffer, sizeof(buffer), "Active/Allocated:  %3d/%3d effects with %3d/%3d events at %3d locations with %3d lightprobes|n",
				0,  // $TODO effects_information.active_effect_count
				0,  // $TODO effects_information.effect_count
				0,  // $TODO effects_information.active_event_count
				0,  // $TODO effects_information.event_count
				0,  // $TODO effects_information.location_count
				0); // $TODO effects_information.lightprobe_count
		
			csnzappendf(buffer, sizeof(buffer), "Normal/High/Essential:  %3d/%3d/%3d effects at %3d/%3d/%3d events|n",
				0,  // $TODO effects_information_by_priority[0].effect_count
				0,  // $TODO effects_information_by_priority[1].effect_count
				0,  // $TODO effects_information_by_priority[2].effect_count
				0,  // $TODO effects_information_by_priority[0].event_count
				0,  // $TODO effects_information_by_priority[1].event_count
				0); // $TODO effects_information_by_priority[2].event_count
		
			csnzappendf(buffer, sizeof(buffer), "Active/Submitted/Allocated:  %3d/%3d/%3d particle systems at %3d/%3d/%3d locations with %3d/%3d/%3d emitters|n",
				0,  // $TODO g_particle_information.m_active_system_count
				0,  // $TODO g_particle_information.m_submitted_system_count
				0,  // $TODO g_particle_information.m_allocated_system_count
				0,  // $TODO g_particle_information.m_active_location_count
				0,  // $TODO g_particle_information.m_submitted_location_count
				0,  // $TODO g_particle_information.m_allocated_location_count
				0,  // $TODO g_particle_information.m_active_emitter_count
				0,  // $TODO g_particle_information.m_submitted_emitter_count
				0); // $TODO g_particle_information.m_allocated_emitter_count
		
			csnzappendf(buffer, sizeof(buffer), "Normal/High/Essential:  %3d/%3d/%3d particle systems at %3d/%3d/%3d locations with %3d/%3d/%3d emitters|n",
				0,  // $TODO particle_information_by_priority[0].m_system_count
				0,  // $TODO particle_information_by_priority[1].m_system_count
				0,  // $TODO particle_information_by_priority[2].m_system_count
				0,  // $TODO particle_information_by_priority[0].m_location_count
				0,  // $TODO particle_information_by_priority[1].m_location_count
				0,  // $TODO particle_information_by_priority[2].m_location_count
				0,  // $TODO particle_information_by_priority[0].m_emitter_count
				0,  // $TODO particle_information_by_priority[1].m_emitter_count
				0); // $TODO particle_information_by_priority[2].m_emitter_count
		
			csnzappendf(buffer, sizeof(buffer), "%4d gpu particles including %3d/%3d cpu particles with %3d physics and %3d/%3d collision|n",
				0,  // $TODO g_particle_information.m_gpu_particle_count
				0,  // $TODO g_particle_information.m_active_cpu_particle_count
				0,  // $TODO g_particle_information.m_allocated_cpu_particle_count
				0,  // $TODO g_particle_information.m_physics_particle_count
				0,  // $TODO g_particle_information.m_colliding_particle_count
				0); // $TODO g_particle_information.m_collision_particle_count
		
			csnzappendf(buffer, sizeof(buffer), "Active/Submitted/Allocated:  %3d/%3d/%3d beams, %3d/%3d/%3d contrails, %3d/%3d/%3d light_volumes|n",
				0,  // $TODO g_beam_information.m_active_count
				0,  // $TODO g_beam_information.m_submitted_count
				0,  // $TODO g_beam_information.m_allocated_count
				0,  // $TODO g_contrail_information.m_active_count
				0,  // $TODO g_contrail_information.m_submitted_count
				0,  // $TODO g_contrail_information.m_allocated_count
				0,  // $TODO g_light_volume_information.m_active_count
				0,  // $TODO g_light_volume_information.m_submitted_count
				0); // $TODO g_light_volume_information.m_allocated_count
		
			csnzappendf(buffer, sizeof(buffer), "Normal/High/Essential:  %3d/%3d/%3d beams, %3d/%3d/%3d contrails, %3d/%3d/%3d light_volumes|n",
				0,  // $TODO beam_information_by_priority[0].m_count
				0,  // $TODO beam_information_by_priority[1].m_count
				0,  // $TODO beam_information_by_priority[2].m_count
				0,  // $TODO contrail_information_by_priority[0].m_count
				0,  // $TODO contrail_information_by_priority[1].m_count
				0,  // $TODO contrail_information_by_priority[2].m_count
				0,  // $TODO light_volume_information_by_priority[0].m_count
				0,  // $TODO light_volume_information_by_priority[1].m_count
				0); // $TODO light_volume_information_by_priority[2].m_count
		
			//c_decal_system::profile_dump(buffer, sizeof(buffer));
		}

		if (profile_summary_ai_enabled)
		{
			ai_profile_display(buffer, sizeof(buffer));
			//if (profile_summary_ai_enabled)
			//	collision_log_display(buffer, sizeof(buffer));
		}

		if (profile_summary_graphics_enabled)
		{
			visibility_profile_display(buffer, sizeof(buffer));
			//dip_profile_display(buffer, sizeof(buffer));
		}

		/*
		if (profile_summary_environment_artist_enabled)
		{
			profile_environment_artist_display(buffer, sizeof(buffer));
		}
		*/

		draw_string.draw(&font_cache, buffer);
		for (const char* new_line = csstrstr(buffer, "|n"); new_line; new_line = csstrstr(new_line + 2, "|n"));
	}
}

void profile_summary_cycle()
{
	e_profile_summary_modes current_mode = _profile_summary_all;

	if (profile_summary_enabled)
	{
		current_mode = e_profile_summary_modes(g_profile_summary_mode + 1);
	}
	else
	{
		profile_summary_enabled = true;
	}

	g_profile_summary_mode = current_mode;

	switch (current_mode)
	{
	case _profile_summary_all:
		profile_summary_environment_artist_enabled = false;
		profile_summary_objects_enabled = true;
		profile_summary_graphics_enabled = true;
		profile_summary_effects_enabled = true;
		profile_summary_ai_enabled = true;
		break;
	case _profile_summary_objects:
		profile_summary_environment_artist_enabled = false;
		profile_summary_objects_enabled = true;
		profile_summary_graphics_enabled = false;
		profile_summary_effects_enabled = false;
		profile_summary_ai_enabled = false;
		break;
	case _profile_summary_graphics:
		profile_summary_environment_artist_enabled = false;
		profile_summary_objects_enabled = false;
		profile_summary_graphics_enabled = true;
		profile_summary_effects_enabled = false;
		profile_summary_ai_enabled = false;
		break;
	case _profile_summary_effects:
		profile_summary_environment_artist_enabled = false;
		profile_summary_objects_enabled = false;
		profile_summary_graphics_enabled = false;
		profile_summary_effects_enabled = true;
		profile_summary_ai_enabled = false;
		break;
	case _profile_summary_ai:
		profile_summary_objects_enabled = false;
		profile_summary_environment_artist_enabled = false;
		profile_summary_graphics_enabled = false;
		profile_summary_effects_enabled = false;
		profile_summary_ai_enabled = true;
		break;
	case _profile_summary_game_state:
		profile_summary_objects_enabled = true;
		profile_summary_environment_artist_enabled = false;
		profile_summary_graphics_enabled = false;
		profile_summary_effects_enabled = false;
		profile_summary_ai_enabled = true;
		break;
	case _profile_summary_environment_artist:
		profile_summary_environment_artist_enabled = true;
		profile_summary_objects_enabled = false;
		profile_summary_graphics_enabled = false;
		profile_summary_effects_enabled = false;
		profile_summary_ai_enabled = false;
		break;
	default:
		profile_summary_enabled = false;
		current_mode = _profile_summary_all;
		break;
	}

	console_printf("summary: %s", profile_summary_enabled ? k_profile_summary_names[current_mode] : "disabled");
}

