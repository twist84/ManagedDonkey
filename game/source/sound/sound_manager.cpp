#include "sound/sound_manager.hpp"

#include "cache/cache_files.hpp"
#include "game/player_mapping.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "render/render_debug.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_classes.hpp"

struct s_cache_file_sound_permutation;
struct sound_channel_datum
{
	long sound_index;
	long __unknown4;
	long channel_debugger_index;
	long __unknownC;
	char sound_channel_type; // c_enum<e_sound_channel_type, char>
	char __unknown9;
	byte __unknownA_sound_playback; // c_flags<e_sound_playback_bit, byte>
	byte __unknownB_sound_playback; // c_flags<e_sound_playback_bit, byte>
	word hardware_source_index;
	word hardware_voice_index;
	real __unknown18;
	real __unknown1C;
	byte __data20[4];
	long playing_chunk_definition_index;
	long queued_chunk_definition_index;
	short playing_chunk_index;
	short queued_chunk_index;
	s_cache_file_sound_permutation* playing_chunk_permutation;
	s_cache_file_sound_permutation* queued_chunk_permutation;
};
static_assert(sizeof(sound_channel_datum) == 0x38);

struct s_sound_source
{
	word_flags flags;
	char volume_type;
	char spatialization_mode;
	real scale;
	byte __dataC[0x4];
	real_point3d position;
	real_quaternion orientation;
	byte __data28[0xC];
	short end_cluster_index;
	char __data36[0x2];
	real __unknown38;
	real minimum_distance;
	real maximum_distance;
	short inner_cone_angle_step;
	short outer_cone_angle_step;
	byte __data48[0x4];
	long playback_controller_index;
};
static_assert(sizeof(s_sound_source) == 0x50);

struct sound_datum : s_datum_header
{
	char software_reference_count;
	char hardware_reference_count;

	word_flags unknown_flags;
	byte_flags playback_flags;

	byte __data7[0x7];

	long definition_index;

	byte __data14[0x8];

	s_sound_source source;

	byte __data6C[4];

	char loop_track_index;
	byte __unknown71;
	byte __unknown72;
	byte __unknown73;

	byte __data74[48];

	real __unknownA4;

	real maybe_pitch_scale;
	char pitch_range_index;
	char permutation_index;
	short permutation_chunk_index;
	char listener_index;

	byte __unknownB1;
	byte __unknownB2;
	byte __dataB3[0x4];

	short playing_channel_index;
	char sound_fade_mode;

	real __unknownBC;
	byte __dataC0[0x8];
};
static_assert(sizeof(sound_datum) == 0xC8);

struct looping_sound_datum : s_datum_header
{
	byte __data0[0x126];
};
static_assert(sizeof(looping_sound_datum) == 0x128);

struct s_sound_effect_datum : s_datum_header
{
	byte __data0[0x62];
};
static_assert(sizeof(s_sound_effect_datum) == 0x64);

struct c_sound_playback_controller : s_datum_header
{
	byte __data0[0x1A];
};
static_assert(sizeof(c_sound_playback_controller) == 0x1C);

REFERENCE_DECLARE(0x0238E858, s_sound_manager_globals*, g_sound_manager_globals);

//  name: "sound sources"
// count: 384
//  size: 0xC8
REFERENCE_DECLARE(0x0238E860, c_smart_data_array<sound_datum>, g_sound_data);

REFERENCE_DECLARE(0x0238E864, bool, debug_sound_always_update_in_idle);
REFERENCE_DECLARE(0x0238E86E, bool, byte_238E86E); // unreferenced script globals
REFERENCE_DECLARE(0x0238E86F, bool, byte_238E86F); // unreferenced script globals
REFERENCE_DECLARE(0x0238E871, bool, debug_sound_force_first_person_listener);
REFERENCE_DECLARE(0x0238E874, long, g_sound_manager_reference_count);

//  name: "looping sounds"
// count: 128
//  size: 0x128
REFERENCE_DECLARE(0x02497D20, c_smart_data_array<looping_sound_datum>, g_looping_sound_data);

//  name: "sounds effects"
// count: 16
//  size: 0x64
REFERENCE_DECLARE(0x02497D30, c_smart_data_array<s_sound_effect_datum>, g_sound_effect_data);

//  name: "sound playback controllers"
// count: 64
//  size: 0x1C
REFERENCE_DECLARE(0x02497D3C, c_smart_data_array<c_sound_playback_controller>, g_sound_playback_controller_data);

bool debug_sound_class_totals = false;
bool debug_duckers = false;
bool debug_sound_listeners = false;
bool debug_sound = false;
bool debug_sound_manager_channels = false;

void __cdecl quaternion_transform_point(real_quaternion const* in_quaternion, real_point3d const* in_point, real_point3d* point)
{
	INVOKE(0x004FAAF0, quaternion_transform_point, in_quaternion, in_point, point);
}

sound_channel_datum* __cdecl channel_get(short index)
{
	return INVOKE(0x00513230, channel_get, index);
}

s_sound_listener const* __cdecl sound_manager_get_listener(long index)
{
	return INVOKE(0x005182A0, sound_manager_get_listener, index);
}

real sound_definition_map_pitch(void const* sound_definition, real a1, real a2)
{
	return INVOKE(0x00516650, sound_definition_map_pitch, sound_definition, a1, a2);
}

real __cdecl sound_source_get_inner_cone_angle(s_sound_source const* source, long sound_definition_index)
{
	ASSERT(source);

	return INVOKE(0x005196D0, sound_source_get_inner_cone_angle, source, sound_definition_index);
}

real __cdecl sound_source_get_outer_cone_angle(s_sound_source const* source, long sound_definition_index)
{
	ASSERT(source);

	return INVOKE(0x00519810, sound_source_get_outer_cone_angle, source, sound_definition_index);
}

void __cdecl sound_source_get_world_position(s_sound_source const* source, long listener_index, real_point3d* world_position)
{
	INVOKE(0x005199B0, sound_source_get_world_position, source, listener_index, world_position);
}

real __cdecl sound_definition_get_maximum_distance(long sound_definition_index)
{
	return INVOKE(0x00661EF0, sound_definition_get_maximum_distance, sound_definition_index);
}

real __cdecl sound_definition_get_minimum_distance(long sound_definition_index)
{
	return INVOKE(0x00661FC0, sound_definition_get_minimum_distance, sound_definition_index);
}

char const* path_add_parent_directory(char const* original_path, char const* stripped_path)
{
	char const* path_end = &original_path[strlen(original_path)];
	ASSERT(IN_RANGE_INCLUSIVE(stripped_path, original_path, path_end));

	char const* v5 = stripped_path - 1;
	if (stripped_path - 1 <= original_path)
		v5 = original_path;

	char const* i = 0;
	for (i = v5; i > original_path && *(i - 1) != '\\'; i--)
		;

	return i;
}

void __cdecl sound_render_debug(long sound_index)
{
	if (!debug_sound)
		return;

	sound_datum* sound = static_cast<sound_datum*>(datum_try_and_get(*g_sound_data, sound_index));
	if (!sound)
		return;

	if (!TEST_BIT(sound->unknown_flags, 1))
		return;

	sound_channel_datum* channel = nullptr;
	for (short channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
	{
		sound_channel_datum* channel_ = channel_get(channel_index);
		if (channel_->sound_index == sound_index)
		{
			channel = channel_;
			break;
		}
	}

	short hardware_source_index = NONE;
	short hardware_voice_index = NONE;
	if (channel)
	{
		hardware_source_index = channel->hardware_source_index;
		hardware_voice_index = channel->hardware_voice_index;
	}

	void* sound_definition = tag_get(SOUND_TAG, sound->definition_index);

	char const* tag_name = tag_get_name(sound->definition_index);

	c_static_string<512> debug_string;
	debug_string.print(
		"%s|n"
		"source %d|n"
		"voice %d|n"
		"pitch %.2f|n"
		"scale %.2f|n"
		"%.2f,%.2f,%.2f",
		path_add_parent_directory(tag_name, tag_name_strip_path(tag_name)),
		hardware_source_index,
		hardware_voice_index,
		sound_definition_map_pitch(sound_definition, sound->__unknownA4, sound->source.scale),
		sound->source.scale,
		sound->source.position.x,
		sound->source.position.y,
		sound->source.position.z);

	render_debug_string_at_point(&sound->source.position, debug_string.get_string(), global_real_argb_white);

	real minimum_distance = sound_definition_get_minimum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 2))
		minimum_distance = sound->source.minimum_distance;

	real maximum_distance = sound_definition_get_maximum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 3))
		maximum_distance = sound->source.maximum_distance;

	angle outer_cone_angle = sound_source_get_outer_cone_angle(&sound->source, sound->definition_index);
	angle inner_cone_angle = sound_source_get_inner_cone_angle(&sound->source, sound->definition_index);

	real_point3d cone_point{};
	quaternion_transform_point(&sound->source.orientation, (real_point3d*)global_forward3d, &cone_point);

	real_point3d volume_type_point{};
	switch (sound->source.volume_type)
	{
	case 0:
		render_debug_sphere(/*false*/true, &sound->source.position, minimum_distance, global_real_argb_green);
		render_debug_sphere(/*false*/true, &sound->source.position, maximum_distance, global_real_argb_darkgreen);
		break;
	case 1:
		quaternion_transform_point(&sound->source.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_cylinder(/*false*/true, &sound->source.position, (vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_cylinder(/*false*/true, &sound->source.position, (vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
		break;
	case 2:
		quaternion_transform_point(&sound->source.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_pill(/*false*/true, &sound->source.position, (vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_pill(/*false*/true, &sound->source.position, (vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
		break;
	}

	if (inner_cone_angle > 0.0f && inner_cone_angle < real(PI) || outer_cone_angle > 0.0f && outer_cone_angle < real(PI))
	{
		render_debug_cone_outline(/*false*/true, &sound->source.position, (vector3d*)&cone_point, maximum_distance, inner_cone_angle, global_real_argb_orange);
		render_debug_cone_outline(/*false*/true, &sound->source.position, (vector3d*)&cone_point, maximum_distance, outer_cone_angle, global_real_argb_red);
	}

	if (sound->listener_index == NONE || TEST_BIT(sound->source.flags, 7) || !TEST_BIT(sound->source.flags, 0))
		return;

	long listener_unit = player_mapping_get_unit_by_output_user(sound->listener_index);
	if (listener_unit != NONE)
	{
		s_sound_listener const* listener = sound_manager_get_listener(sound->listener_index); // listener_get_internal
		ASSERT(listener->valid);

		switch ((16 * sound->source.spatialization_mode) >> 4)
		{
		case 0:
			volume_type_point = listener->matrix.center;
			break;
		case 1:
			sound_source_get_world_position(&sound->source, sound->listener_index, &volume_type_point);
			break;
		case 2:
			matrix4x3_transform_point(&listener->matrix, &sound->source.position, &volume_type_point);
			break;
		}

		unit_get_head_position(listener_unit, &cone_point);
		render_debug_line(true, &volume_type_point, &cone_point, global_real_argb_red);
	}
}

void __cdecl sound_debug_render()
{
	c_static_string<8192> debug_string;

	//platform_sound_debug_render();

	if (debug_sound_class_totals)
	{
		//long v5 = NONE;
		//for (long i = 0; i < k_sound_class_count; i++)
		//{
		//	if (!strchr(sound_class_names[i], '!'))
		//	{
		//		long v7 = ++v5 / 2;
		//		if (TEST_BIT(v5, 0))
		//		{
		//			//status_line_appendf(sound_class_totals_status_lines[v7], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//		else
		//		{
		//			//status_line_appendf(sound_class_totals_status_lines[v7], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//	}
		//}
	}

	if (debug_duckers)
	{
		if (g_sound_manager_globals->ducker.active_ducker != NONE && g_sound_manager_globals->ducker.active_ducker_time >= 5.0f)
		{
			debug_string.append_print("ducker inactive for %1.2f seconds|n",
				g_sound_manager_globals->ducker.inactive_time);
		}
		else
		{
			debug_string.append_print("active ducker: %d time: %1.2f last ducker: %d inactive time: %1.2f|n",
				g_sound_manager_globals->ducker.active_ducker,
				g_sound_manager_globals->ducker.active_ducker_time,
				g_sound_manager_globals->ducker.last_ducker,
				g_sound_manager_globals->ducker.inactive_time);
		}
		debug_string.append("|n");
	}

	if (debug_sound_listeners)
	{
		for (long i = 0; i < 4; i++)
		{
			s_sound_listener const* listener = sound_manager_get_listener(i);
			if (listener->valid)
				render_debug_matrix(true, &listener->matrix, 0.3f);
		}
	}

	if (debug_sound)
	{
		c_data_iterator<sound_datum> sound_iterator = g_sound_data.begin();
		do
		{
			sound_render_debug(sound_iterator.get_index());
		} while (sound_iterator.next());

		long sound_datums = NONE;
		if (*g_sound_data)
			sound_datums = g_sound_data->actual_count;

		long looping_sound_datums = NONE;
		if (*g_looping_sound_data)
			looping_sound_datums = g_looping_sound_data->actual_count;

		long playback_controllers = NONE;
		if (*g_sound_playback_controller_data)
			playback_controllers = g_sound_playback_controller_data->actual_count;

		long sound_effects = NONE;
		if (*g_sound_effect_data)
			sound_effects = g_sound_effect_data->actual_count;

		long voices = NONE;

		debug_string.append_print(
			"|tsound datums: %d|n"
			"|tlooping sound datums: %d|n"
			"|tplayback controllers: %d|n"
			"|tsound effects: %d|n"
			"|tvoices: %d|n",
			sound_datums,
			looping_sound_datums,
			playback_controllers,
			sound_effects,
			voices);
		debug_string.append("|n");
	}

	if (debug_sound_manager_channels)
	{
		long v30[5]{};
		long v31[5]{};

		for (short channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
		{
			sound_channel_datum* channel = channel_get(channel_index);
			if (channel->sound_index != NONE)
				v30[channel->__unknownA_sound_playback]++;

			v31[channel->__unknownA_sound_playback]++;
		}

		for (long i = 0; i < NUMBEROF(v30); i++)
		{
			bool long_string = true;
			char const* sound_playback = "";
			switch (i)
			{
			case 0:
			{
				sound_playback = "S_1";
				if (long_string)
					sound_playback = "Sound_Mono   ";
			}
			break;
			case 1:
			{
				sound_playback = "S_2";
				if (long_string)
					sound_playback = "Sound_Stereo ";
			}
			break;
			case 2:
			{
				sound_playback = "S_4";
				if (long_string)
					sound_playback = "Sound_quad   ";
			}
			break;
			case 3:
			{
				sound_playback = "S_6";
				if (long_string)
					sound_playback = "Sound_5_dot_1";
			}
			break;
			case 4:
			{
				sound_playback = "S_C";
				if (long_string)
					sound_playback = "Sound_Codec  ";
			}
			break;
			}

			debug_string.append_print("Channels %s: %4d / %4d |n", sound_playback, v30[i], v31[i]);
		}
		debug_string.append("|n");
	}

	if (debug_sound_manager_channels)
	{
		for (short channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
		{
			sound_channel_datum* channel = channel_get(channel_index);
			if (channel->sound_index == NONE)
				continue;
	
			sound_datum* sound = static_cast<sound_datum*>(datum_try_and_get(*g_sound_data, channel->sound_index));
			if (!sound)
				continue;

			bool long_string = false;
			char const* sound_playback = "";
			switch (channel->__unknownA_sound_playback)
			{
			case 0:
			{
				sound_playback = "S_1";
				if (long_string)
					sound_playback = "Sound_Mono   ";
			}
			break;
			case 1:
			{
				sound_playback = "S_2";
				if (long_string)
					sound_playback = "Sound_Stereo ";
			}
			break;
			case 2:
			{
				sound_playback = "S_4";
				if (long_string)
					sound_playback = "Sound_quad   ";
			}
			break;
			case 3:
			{
				sound_playback = "S_6";
				if (long_string)
					sound_playback = "Sound_5_dot_1";
			}
			break;
			case 4:
			{
				sound_playback = "S_C";
				if (long_string)
					sound_playback = "Sound_Codec  ";
			}
			break;
			}

			debug_string.append_print("c%3d: %s: %7.2f %s|n", channel_index, sound_playback, channel->__unknown1C, tag_name_strip_path(tag_get_name(sound->definition_index)));
		}
		debug_string.append("|n");
	}

	if (!debug_string.is_empty())
		render_debug_string(debug_string.get_string());

	//looping_sound_render_debug();
}

