#pragma once

#include "cseries/cseries.hpp"

enum e_lens_flare_attachment_types
{
	_lens_flare_attachment_type_effect = 0,
	_lens_flare_attachment_type_light,
	_lens_flare_attachment_type_object,

	k_number_of_lens_flare_attachment_types
};

enum e_lens_flare_occlusion_rendering_types
{
	_lens_flare_occlusion_type_occlusion = 0,
	_lens_flare_occlusion_type_conditional,

	k_number_of_lens_flare_occlusion_types
};

struct s_lens_flare
{
	long definition_index;
	real_point3d position;
	real_vector3d forward;
	dword tint_color;
	real age_function_input;
	real scale;
	real marker_scale;
	long last_pulse_tick;
	long expected_pixel_count;
	long attached_type_index;
	e_lens_flare_attachment_types attachment_type;
	char flags;
	long user_index;
};
static_assert(sizeof(s_lens_flare) == 0x44);

struct _lens_flare_globals
{
	c_simple_hash_table<s_lens_flare, 256, 3307, 3, 337> lens_flare_array;
	long lens_flare_frame_index;
};
static_assert(sizeof(_lens_flare_globals) == 0x5408);

extern bool& render_lens_flares_enabled;
extern _lens_flare_globals& g_lens_flare_globals;

extern void __cdecl lens_flare_get_bitmap_aspect_ratio(long bitmap_group_index, long bitmap_index, real_vector2d* aspect_scale);
extern void __cdecl lens_flare_render_internal(long user_index, long lens_flare_index, real opacity, long conditional_rendering_index, bool screen_facing);
extern void __cdecl lens_flare_render_occlusion(long lens_flare_index);
extern long __cdecl lens_flares_create(long definition_index, e_lens_flare_attachment_types attachment_type, long attachment_index, word attachment_subindex, real_point3d const* position, real_vector3d const* forward, real_rgb_color const* tint_color, real age_function_input, real scale, real marker_scale, bool first_person, long user_index);
extern long __cdecl lens_flares_create_internal(long definition_index, e_lens_flare_attachment_types attachment_type, long attachment_index, word attachment_subindex, real_point3d const* position, real_vector3d const* forward, real_rgb_color const* tint_color, real age_function_input, real scale, real marker_scale, bool first_person, long user_index);
extern void __cdecl lens_flares_dispose();
extern void __cdecl lens_flares_dispose_from_old_map();
extern void __cdecl lens_flares_garbage_collect();
extern void __cdecl lens_flares_initialize();
extern void __cdecl lens_flares_initialize_for_new_map();
extern void __cdecl lens_flares_render(long user_index);
extern void __cdecl lens_flares_reset_rasterizer_parameters();
extern void __cdecl lens_flares_setup_occlusion_shader();
extern void __cdecl lens_flares_setup_rendering_shader();
extern void __cdecl lens_flares_submit_occlusions(long user_index, e_lens_flare_occlusion_rendering_types rendering_type);

