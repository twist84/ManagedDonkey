#include "objects/emblems.hpp"

#include "game/game_globals.hpp"
#include "math/color_math.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"
#include "scenario/scenario.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B26390, s_emblem_info, decode);
HOOK_DECLARE_CLASS_MEMBER(0x00B267B0, s_emblem_info, encode);

enum
{
	k_ps_emblem_color_background_argb = 55,
	k_ps_emblem_color_icon1_argb = 56,
	k_ps_emblem_color_icon2_argb = 57,
};

s_emblem_info g_user_interface_emblem_info;

void s_emblem_info::decode(c_bitstream* packet)
{
	foreground_emblem_index = (uns8)packet->read_integer("foreground-emblem", 6);
	background_emblem_index = (uns8)packet->read_integer("background-emblem", 6);
	emblem_info_flags.set_unsafe((uns8)packet->read_integer("emblem-flags", 3));
	primary_color_index.set_raw_value((int8)packet->read_integer("emblem-primary-color", 6));
	secondary_color_index.set_raw_value((int8)packet->read_integer("emblem-secondary-color", 6));
	background_color_index.set_raw_value((int8)packet->read_integer("emblem-background-color", 6));
}

void s_emblem_info::encode(c_bitstream* packet)
{
	packet->write_integer("foreground-emblem", foreground_emblem_index, 6);
	packet->write_integer("background-emblem", background_emblem_index, 6);
	packet->write_integer("emblem-flags", emblem_info_flags.get_unsafe(), 3);
	packet->write_integer("emblem-primary-color", primary_color_index, 6);
	packet->write_integer("emblem-secondary-color", secondary_color_index, 6);
	packet->write_integer("emblem-background-color", background_color_index, 6);
}

// $TODO: find this a home
real_rgb_color player_profile_get_rgb_color(int32 color_index)
{
	if (!global_scenario_try_and_get())
		return *global_real_rgb_white;

	s_game_globals* game_globals = scenario_get_game_globals();
	if (!VALID_INDEX(color_index, game_globals->profile_colors.count))
	{
		event(_event_message, "no player color defined in game globals tag for player color index #%ld",
			color_index);
		return *global_real_rgb_white;
	}

	// profile_colors is supposed to be `real_rgb_color`

	real_rgb_color real_color{};
	return *pixel32_to_real_rgb_color(game_globals->profile_colors[color_index], &real_color);
}

s_emblem_info* emblem_get_render_constants_emblem_info_from_user_interface(s_emblem_info* emblem_info)
{
	ASSERT(emblem_info != NULL);

	*emblem_info = g_user_interface_emblem_info;
	return emblem_info;
}

bool emblem_set_render_constants_from_user_interface(s_emblem_info const* emblem_info)
{
	ASSERT(emblem_info != NULL);

	g_user_interface_emblem_info = *emblem_info;

	//real_rgb_color emblem_color_background_rgb = player_profile_get_rgb_color(emblem_info->background_color_index);
	//real_rgb_color emblem_color_icon1_rgb = player_profile_get_rgb_color(emblem_info->primary_color_index);
	//real_rgb_color emblem_color_icon2_rgb = player_profile_get_rgb_color(emblem_info->secondary_color_index);
	//
	//bool alternate_foreground_channel_off = emblem_info->emblem_info_flags.test(_emblem_info_flag_alternate_foreground_channel_off);
	//bool flip_foreground = emblem_info->emblem_info_flags.test(_emblem_info_flag_flip_foreground);
	//bool flip_background = emblem_info->emblem_info_flags.test(_emblem_info_flag_flip_background);
	//
	//real_vector4d emblem_color_background_argb{};
	//real_vector4d emblem_color_icon1_argb{};
	//real_vector4d emblem_color_icon2_argb{};
	//
	//set_real_vector4d(&emblem_color_background_argb,
	//	emblem_color_background_rgb.red,
	//	emblem_color_background_rgb.green,
	//	emblem_color_background_rgb.blue,
	//	alternate_foreground_channel_off ? 0.0f : 1.0f);
	//
	//set_real_vector4d(&emblem_color_icon1_argb,
	//	emblem_color_icon1_rgb.red,
	//	emblem_color_icon1_rgb.green,
	//	emblem_color_icon1_rgb.blue,
	//	flip_foreground ? 0.0f : 1.0f);
	//
	//set_real_vector4d(&emblem_color_icon2_argb,
	//	emblem_color_icon2_rgb.red,
	//	emblem_color_icon2_rgb.green,
	//	emblem_color_icon2_rgb.blue,
	//	flip_background ? 0.0f : 1.0f);
	//
	//c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_background_argb, 1, &emblem_color_background_argb);
	//c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_icon1_argb, 1, &emblem_color_icon1_argb);
	//c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_icon2_argb, 1, &emblem_color_icon2_argb);

	return true;
}

