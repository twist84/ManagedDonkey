#include "render/render_lens_flares.hpp"

REFERENCE_DECLARE(0x0191347C, bool, render_lens_flares_enabled);
REFERENCE_DECLARE(0x05110668, _lens_flare_globals, g_lens_flare_globals);

//.text:00A58B30 ; bool __cdecl lens_flare_evaluate_parameters(long, long, long, real32, real_vector4d const*, bool, s_lens_flare_render_parameters*)

void __cdecl lens_flare_get_bitmap_aspect_ratio(long bitmap_group_index, long bitmap_index, real_vector2d* aspect_scale)
{
	INVOKE(0x00A59620, lens_flare_get_bitmap_aspect_ratio, bitmap_group_index, bitmap_index, aspect_scale);
}

//.text:00A596D0 ; bool __cdecl lens_flare_project_billboard(real_point3d const*, real32, real_vector2d const*, bool, real_vector4d*, real_vector2d*, real_vector2d*)

void __cdecl lens_flare_render_internal(long user_index, long lens_flare_index, real32 opacity, long conditional_rendering_index, bool screen_facing)
{
	INVOKE(0x00A598C0, lens_flare_render_internal, user_index, lens_flare_index, opacity, conditional_rendering_index, screen_facing);
}

void __cdecl lens_flare_render_occlusion(long lens_flare_index)
{
	INVOKE(0x00A5A230, lens_flare_render_occlusion, lens_flare_index);

	lens_flare_render_internal(NONE, lens_flare_index, 1.0f, NONE, false);
}

long __cdecl lens_flares_create(long definition_index, e_lens_flare_attachment_types attachment_type, long attachment_index, uint16 attachment_subindex, real_point3d const* position, real_vector3d const* forward, real_rgb_color const* tint_color, real32 age_function_input, real32 scale, real32 marker_scale, bool first_person, long user_index)
{
	//return INVOKE(0x00A5A250, lens_flares_create, definition_index, attachment_type, attachment_index, attachment_subindex, position, forward, tint_color, age_function_input, scale, marker_scale, first_person, user_index);

	long result = lens_flares_create_internal(definition_index, attachment_type, attachment_index, attachment_subindex, position, forward, tint_color, age_function_input, scale, marker_scale, first_person, user_index);
	return result;
}

long __cdecl lens_flares_create_internal(long definition_index, e_lens_flare_attachment_types attachment_type, long attachment_index, uint16 attachment_subindex, real_point3d const* position, real_vector3d const* forward, real_rgb_color const* tint_color, real32 age_function_input, real32 scale, real32 marker_scale, bool first_person, long user_index)
{
	return INVOKE(0x00A5A2A0, lens_flares_create_internal, definition_index, attachment_type, attachment_index, attachment_subindex, position, forward, tint_color, age_function_input, scale, marker_scale, first_person, user_index);
}

void __cdecl lens_flares_dispose()
{
	//INVOKE(0x00A5A460, lens_flares_dispose);
}

void __cdecl lens_flares_dispose_from_old_map()
{
	INVOKE(0x00A5A470, lens_flares_dispose_from_old_map);

	//g_lens_flare_globals.lens_flare_array.clear_all();
}

//.text:00A5A4B0 ; real32 __cdecl lens_flares_evaluate_corona_rotation_function(long function, real32 scale_in_radians, real_point3d const* position, real_vector3d const* forward)

void __cdecl lens_flares_garbage_collect()
{
	INVOKE(0x00A5A840, lens_flares_garbage_collect);
}

void __cdecl lens_flares_initialize()
{
	INVOKE(0x00A5A8E0, lens_flares_initialize);
}

void __cdecl lens_flares_initialize_for_new_map()
{
	INVOKE(0x00A5A920, lens_flares_initialize_for_new_map);
}

void __cdecl lens_flares_render(long user_index)
{
	INVOKE(0x00A5A960, lens_flares_render, user_index);
}

void __cdecl lens_flares_reset_rasterizer_parameters()
{
	INVOKE(0x00A5AAC0, lens_flares_reset_rasterizer_parameters);
}

void __cdecl lens_flares_setup_occlusion_shader()
{
	INVOKE(0x00A5AAE0, lens_flares_setup_occlusion_shader);
}

void __cdecl lens_flares_setup_rendering_shader()
{
	INVOKE(0x00A5AB10, lens_flares_setup_rendering_shader);
}

void __cdecl lens_flares_submit_occlusions(long user_index, e_lens_flare_occlusion_rendering_types rendering_type)
{
	INVOKE(0x00A5AB30, lens_flares_submit_occlusions, user_index, rendering_type);
}

