#include "sound/sound_manager.hpp"

#include "cache/cache_files.hpp"
#include "game/player_mapping.hpp"
#include "memory/data.hpp"
#include "memory/module.hpp"
#include "render/render_debug.hpp"
#include "shell/shell.hpp"
#include "simulation/simulation.hpp"
#include "sound/sound_classes.hpp"

#include <string.h>

#define MAXIMUM_SOUND_SOURCES 196

#define NUMBER_OF_SOUND_SAMPLE_RATES 3

long const sound_sample_rate_samples_per_second[NUMBER_OF_SOUND_SAMPLE_RATES]
{
	22050,
	44100,
	32000
};

struct s_sound_location
{
	real_point3d position;
	real_quaternion orientation;
	vector3d vector;
	s_location location;
	byte __data2A[0x2];
};
static_assert(sizeof(s_sound_location) == 0x2C);

struct s_sound_source
{
	word_flags flags;
	char volume_type;
	char spatialization_mode;
	real scale;
	byte __dataC[0x4];
	s_sound_location location;
	real __unknown38;
	real minimum_distance;
	real maximum_distance;
	short inner_cone_angle_step;
	short outer_cone_angle_step;
	long __unknown48;
};
static_assert(sizeof(s_sound_source) == 0x4C);

struct s_sound_tracker
{
	bool(__cdecl* callback)(void*, long, long*, s_sound_source*);

	byte __data[0x2C];
};
static_assert(sizeof(s_sound_tracker) == 0x30);

struct s_cache_file_sound_permutation;
struct sound_channel_datum
{
	long sound_index;
	long __unknown4;
	long debug_entry_index;
	long __unknownC;
	char type_index; // c_enum<e_sound_channel_type, char>
	byte_flags flags;
	byte __unknownA_sound_playback; // c_flags<e_sound_playback_bit, byte>
	byte __unknownB_sound_playback; // c_flags<e_sound_playback_bit, byte>
	short hardware_source_index;
	short hardware_voice_index;

	// initial_sample_offset = sound_sample_rate_samples_per_second[sound->get_sample_rate()] * __unknown18
	real __unknown18;

	long __unknown1C;

	byte __data20[0x4];

	long playing_chunk_definition_index;
	long queued_chunk_definition_index;
	short playing_chunk_index;
	short queued_chunk_index;
	s_cache_file_sound_permutation* playing_chunk_permutation;
	s_cache_file_sound_permutation* queued_chunk_permutation;
};
static_assert(sizeof(sound_channel_datum) == 0x38);

struct s_sound_tracker_datum :
	s_datum_header
{
	byte_flags flags;
	byte flip_flop;
	long definition_index;
	void* tracker_callback_data;
	s_sound_tracker tracker_data;

	byte __data3C[0x4];
};
static_assert(sizeof(s_sound_tracker_datum) == 0x40);

struct sound_datum :
	s_datum_header
{
	char software_reference_count;
	char hardware_reference_count;

	dword_flags flags;

	byte __data8[0x8];

	long definition_index;
	long looping_sound_index;
	s_sound_tracker* tracker;
	s_sound_source source;
	long playback_controller_index;

	byte __data6C[4];

	char loop_track_index;
	byte __unknown71;
	byte __unknown72;
	byte __unknown73;

	byte __data74[0x24];

	long sound_tracker_index;
	long __unknown9C;
	real pitch_modifier;

	real __unknownA4;

	real maybe_pitch_scale;
	char pitch_range_index;

	char permutation_index;
	short permutation_chunk_index;

	c_enum<e_output_user_index, byte, _output_user_index0, k_number_of_output_users> listener_index;
	byte __unknownB1;
	byte __unknownB2;
	byte __unknownB3;

	long playback_controller_index_;

	short playing_channel_index; // index into `channel_get` and `g_sound_channels`
	char sound_fade_mode;
	char event_stop_reason;

	real __unknownBC;
	long __unknownC0;
	long __unknownC4;
};
static_assert(sizeof(sound_datum) == 0xC8);

struct s_xbox_sound_datum :
	s_datum_header
{
	byte __data0[0xA];
};
static_assert(sizeof(s_xbox_sound_datum) == 0xC);

struct looping_sound_track_datum
{
	long sound_index;
	byte __data4[0x8];
	char current_playback_state;
	char queued_playback_state;
	char playing_sound_count;
	byte __dataF[0x5];
	c_static_array<real, 4> gains;
};
static_assert(sizeof(looping_sound_track_datum) == 0x24);

struct looping_sound_datum :
	s_datum_header
{
	byte __data2[0x6];

	long definition_index;
	long sound_identifier;

	byte __data10[0x4];

	s_sound_source source;
	byte_flags flags;
	char component_sound_count;
	long playback_controller_index;

	byte __data68[0x30];

	c_static_array<looping_sound_track_datum, 4> tracks;
};
static_assert(sizeof(looping_sound_datum) == 0x128);

struct s_sound_effect_datum :
	s_datum_header
{
	byte __data2[0x1];

	char source_type;
	word_flags flags;
	long playback_controller_index;
	long tracker_index;
	s_sound_source source;

	byte __dataC[0x8];
};
static_assert(sizeof(s_sound_effect_datum) == 0x64);

struct c_sound_playback_controller :
	s_datum_header
{
	// reference ^= (reference ^ (reference + 1)) & 0x3F
	byte reference;

	char m_ready_count;
	long identifier;
	dword random_seed;

	byte __dataC[0x10];
};
static_assert(sizeof(c_sound_playback_controller) == 0x1C);

REFERENCE_DECLARE(0x0238E858, s_sound_manager_globals*, g_sound_manager_globals);

//  name: ""
// count: 196
//  size: 0x38
REFERENCE_DECLARE(0x0238E85C, sound_channel_datum*, g_sound_channels);

//  name: "sound sources"
// count: 384
//  size: 0xC8
REFERENCE_DECLARE(0x0238E860, c_smart_data_array<sound_datum>, g_sound_data);

REFERENCE_DECLARE(0x0238E864, bool, debug_sound_always_update_in_idle);
REFERENCE_DECLARE(0x0238E86E, bool, bool_238E86E); // unreferenced script globals
REFERENCE_DECLARE(0x0238E86F, bool, bool_238E86F); // unreferenced script globals
REFERENCE_DECLARE(0x0238E871, bool, debug_sound_force_first_person_listener);
REFERENCE_DECLARE(0x0238E874, long, g_sound_manager_reference_count);

//  name: "xbox sound"
// count: 8192
//  size: 0xC
REFERENCE_DECLARE(0x02497CF8, c_smart_data_array<s_xbox_sound_datum>*, g_xbox_sounds);

REFERENCE_DECLARE(0x02497CFC, bool, bool_2497CFC);

//  name: "looping sounds"
// count: 128
//  size: 0x128
REFERENCE_DECLARE(0x02497D20, c_smart_data_array<looping_sound_datum>, g_looping_sound_data);

REFERENCE_DECLARE(0x02497D26, bool, enable_pc_sound); // unreferenced script globals

//  name: "sounds effects"
// count: 16
//  size: 0x64
REFERENCE_DECLARE(0x02497D30, c_smart_data_array<s_sound_effect_datum>, g_sound_effect_data);

REFERENCE_DECLARE(0x02497D38, c_sound_playback_controller*, g_default_playback_controller);

//  name: "sound playback controllers"
// count: 64
//  size: 0x1C
REFERENCE_DECLARE(0x02497D3C, c_smart_data_array<c_sound_playback_controller>, g_sound_playback_controller_data);

//  name: "  sound tracker data"
// count: 384
//  size: 0x40
REFERENCE_DECLARE(0x02497D4C, c_smart_data_array<s_sound_tracker_datum>, g_sound_tracker_data);

REFERENCE_DECLARE(0x02497D51, bool, g_sound_tracker_inside_update);
REFERENCE_DECLARE(0x02497D51, byte, g_sound_tracker_flip_flop);

bool debug_sound_class_totals = false;
bool debug_sound_timing = false;
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

real sound_definition_map_pitch(void const* sound_definition, real a1, real a2)
{
	return INVOKE(0x00516650, sound_definition_map_pitch, sound_definition, a1, a2);
}

void __cdecl sound_delete(long sound_index)
{
	INVOKE(0x005169D0, sound_delete, sound_index);
}

void __cdecl sound_dispose()
{
	INVOKE(0x00516A30, sound_dispose);
}

void __cdecl sound_game_pause_handler(bool paused)
{
	INVOKE(0x00517100, sound_game_pause_handler, paused);
}

void __cdecl sound_idle()
{
	INVOKE(0x00517170, sound_idle);
}

void __cdecl sound_initialize()
{
	INVOKE(0x00517960, sound_initialize);
}

void __cdecl sound_initialize_for_new_map()
{
	INVOKE(0x00517CF0, sound_initialize_for_new_map);
}

void __cdecl sound_initialize_for_new_structure_bsp()
{
	INVOKE(0x00517F20, sound_initialize_for_new_structure_bsp);
}

bool __cdecl sound_is_active()
{
	return INVOKE(0x00518020, sound_is_active);
}

bool __cdecl sound_is_fading_out(long sound_index)
{
	return INVOKE(0x00518050, sound_is_fading_out, sound_index);
}

s_sound_listener const* __cdecl sound_manager_get_listener(long index)
{
	return INVOKE(0x005182A0, sound_manager_get_listener, index);
}

void __cdecl sound_render()
{
	INVOKE(0x00518E20, sound_render);
}

void __cdecl sound_render_dispatch()
{
	INVOKE(0x005190E0, sound_render_dispatch);
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

void __cdecl sound_update()
{
	INVOKE(0x0051A2C0, sound_update);

	//refresh_listener(false);
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

void __cdecl render_debug_sound(long sound_index)
{
	if (!debug_sound)
		return;

	sound_datum* sound = static_cast<sound_datum*>(datum_try_and_get(*g_sound_data, sound_index));
	if (!sound)
		return;

	if (!TEST_BIT(sound->flags, 1))
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
		sound->source.location.position.x,
		sound->source.location.position.y,
		sound->source.location.position.z);

	render_debug_string_at_point(&sound->source.location.position, debug_string.get_string(), global_real_argb_white);

	real minimum_distance = sound_definition_get_minimum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 2))
		minimum_distance = sound->source.minimum_distance;

	real maximum_distance = sound_definition_get_maximum_distance(sound->definition_index);
	if (TEST_BIT(sound->source.flags, 3))
		maximum_distance = sound->source.maximum_distance;

	angle outer_cone_angle = sound_source_get_outer_cone_angle(&sound->source, sound->definition_index);
	angle inner_cone_angle = sound_source_get_inner_cone_angle(&sound->source, sound->definition_index);

	real_point3d cone_point{};
	quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_forward3d, &cone_point);

	real_point3d volume_type_point{};
	switch (sound->source.volume_type)
	{
	case 0:
		render_debug_sphere(false, &sound->source.location.position, minimum_distance, global_real_argb_green);
		render_debug_sphere(false, &sound->source.location.position, maximum_distance, global_real_argb_darkgreen);
		break;
	case 1:
		quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_cylinder(false, &sound->source.location.position, (vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_cylinder(false, &sound->source.location.position, (vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
		break;
	case 2:
		quaternion_transform_point(&sound->source.location.orientation, (real_point3d*)global_up3d, &volume_type_point);
		volume_type_point.x *= sound->source.__unknown38;
		volume_type_point.y *= sound->source.__unknown38;
		volume_type_point.z *= sound->source.__unknown38;
		render_debug_pill(false, &sound->source.location.position, (vector3d*)&volume_type_point, minimum_distance, global_real_argb_green);
		render_debug_pill(false, &sound->source.location.position, (vector3d*)&volume_type_point, maximum_distance, global_real_argb_darkgreen);
		break;
	}

	if (inner_cone_angle > 0.0f && inner_cone_angle < PI || outer_cone_angle > 0.0f && outer_cone_angle < PI)
	{
		render_debug_cone_outline(false, &sound->source.location.position, (vector3d*)&cone_point, maximum_distance, inner_cone_angle, global_real_argb_orange);
		render_debug_cone_outline(false, &sound->source.location.position, (vector3d*)&cone_point, maximum_distance, outer_cone_angle, global_real_argb_red);
	}

	if (sound->listener_index == k_output_user_none || TEST_BIT(sound->source.flags, 7) || !TEST_BIT(sound->source.flags, 0))
		return;

	long unit_index = player_mapping_get_unit_by_output_user(sound->listener_index);
	if (unit_index != NONE)
	{
		s_sound_listener const* listener = sound_manager_get_listener(sound->listener_index); // listener_get_internal
		ASSERT(listener->valid);

		switch ((16 * sound->source.spatialization_mode) >> 4)
		{
		case 0:
			volume_type_point = listener->matrix.position;
			break;
		case 1:
			sound_source_get_world_position(&sound->source, sound->listener_index, &volume_type_point);
			break;
		case 2:
			matrix4x3_transform_point(&listener->matrix, &sound->source.location.position, &volume_type_point);
			break;
		}

		unit_get_head_position(unit_index, &cone_point);
		render_debug_line(true, &volume_type_point, &cone_point, global_real_argb_red);
	}
}

char const* const k_sound_playback_strings[5][2]
{
	{ "S_1", "Sound_Mono   " },
	{ "S_2", "Sound_Stereo " },
	{ "S_4", "Sound_quad   " },
	{ "S_6", "Sound_5_dot_1" },
	{ "S_C", "Sound_Codec  " },
};

char const* __cdecl sound_playback_to_string(long sound_playback, bool full_string)
{
	ASSERT(VALID_INDEX(sound_playback, 5));

	return k_sound_playback_strings[sound_playback][full_string];
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
		//		if (TEST_BIT(++v5, 0))
		//		{
		//			status_line_appendf(sound_class_totals_status_lines[v5 / 2], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//		else
		//		{
		//			status_line_appendf(sound_class_totals_status_lines[v5 / 2], "% 40s: % 2d", sound_class_names[i], sound_class_totals[i]);
		//		}
		//	}
		//}
	}

	if (debug_sound_timing)
	{
		debug_string.append_print("Sound system_time: %6.4f render_time: %6.4f delta_time: %1.4f|n",
			g_sound_manager_globals->system_time / 1000.0f,
			g_sound_manager_globals->render_time / 1000.0f,
			g_sound_manager_globals->delta_time);

		debug_string.append("|n");
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
		for (long i = 0; i < k_number_of_sound_manager_listeners; i++)
		{
			s_sound_listener const* listener = sound_manager_get_listener(i);
			if (listener->valid)
				render_debug_matrix(true, &listener->matrix, 0.3f);
		}
	}

	if (debug_sound)
	{
		c_data_iterator<sound_datum> sound_iterator;
		sound_iterator.begin(*g_sound_data);
		do
		{
			render_debug_sound(sound_iterator.get_index());
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
		long valid_channels[5]{};
		long total_channels[5]{};

		for (short channel_index = 0; channel_index < g_sound_manager_globals->channel_count; channel_index++)
		{
			sound_channel_datum* channel = channel_get(channel_index);
			if (channel->sound_index != NONE)
				valid_channels[channel->__unknownA_sound_playback]++;

			total_channels[channel->__unknownA_sound_playback]++;
		}

		for (long i = 0; i < NUMBEROF(valid_channels); i++)
		{
			debug_string.append_print("Channels %s: %4d / %4d |n",
				sound_playback_to_string(i, true),
				valid_channels[i],
				total_channels[i]);
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

			char const* sound_playback_name = sound_playback_to_string(channel->__unknownA_sound_playback, true);
			char const* sound_name = tag_name_strip_path(tag_get_name(sound->definition_index));

			debug_string.append_print("c%3d: %s: %7.2f %s|n",
				channel_index,
				sound_playback_name,
				channel->__unknown18,
				sound_name);
		}

		debug_string.append("|n");
	}

	if (!debug_string.is_empty())
		render_debug_string(debug_string.get_string());

	//looping_sound_render_debug();
}

