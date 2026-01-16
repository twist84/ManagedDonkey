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
	int32 definition_index;
	real_point3d position;
	real_vector3d forward;
	uns32 tint_color;
	real32 age_function_input;
	real32 scale;
	real32 marker_scale;
	int32 last_pulse_tick;
	int32 expected_pixel_count;
	int32 attached_type_index;
	e_lens_flare_attachment_types attachment_type;
	int8 flags;
	int32 user_index;
};
COMPILE_ASSERT(sizeof(s_lens_flare) == 0x44);

struct _lens_flare_globals
{
	c_simple_hash_table<s_lens_flare, 256, 3307, 3, 337> lens_flare_array;
	int32 lens_flare_frame_index;
};
COMPILE_ASSERT(sizeof(_lens_flare_globals) == 0x5408);

extern bool& render_lens_flares_enabled;
extern _lens_flare_globals& g_lens_flare_globals;

extern void __cdecl lens_flare_get_bitmap_aspect_ratio(int32 bitmap_group_index, int32 bitmap_index, real_vector2d* aspect_scale);
extern void __cdecl lens_flare_render_internal(int32 user_index, int32 lens_flare_index, real32 opacity, int32 conditional_rendering_index, bool screen_facing);
extern void __cdecl lens_flare_render_occlusion(int32 lens_flare_index);
extern int32 __cdecl lens_flares_create(int32 definition_index, e_lens_flare_attachment_types attachment_type, int32 attachment_index, uns16 attachment_subindex, const real_point3d* position, const real_vector3d* forward, const real_rgb_color* tint_color, real32 age_function_input, real32 scale, real32 marker_scale, bool first_person, int32 user_index);
extern int32 __cdecl lens_flares_create_internal(int32 definition_index, e_lens_flare_attachment_types attachment_type, int32 attachment_index, uns16 attachment_subindex, const real_point3d* position, const real_vector3d* forward, const real_rgb_color* tint_color, real32 age_function_input, real32 scale, real32 marker_scale, bool first_person, int32 user_index);
extern void __cdecl lens_flares_dispose();
extern void __cdecl lens_flares_dispose_from_old_map();
extern void __cdecl lens_flares_garbage_collect();
extern void __cdecl lens_flares_initialize();
extern void __cdecl lens_flares_initialize_for_new_map();
extern void __cdecl lens_flares_render(int32 user_index);
extern void __cdecl lens_flares_reset_rasterizer_parameters();
extern void __cdecl lens_flares_setup_occlusion_shader();
extern void __cdecl lens_flares_setup_rendering_shader();
extern void __cdecl lens_flares_submit_occlusions(int32 user_index, e_lens_flare_occlusion_rendering_types rendering_type);

