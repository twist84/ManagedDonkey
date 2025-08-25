#include "objects/emblems.hpp"

#include "interface/c_controller.hpp"
#include "interface/user_interface_session.hpp"
#include "memory/bitstream.hpp"
#include "memory/module.hpp"
#include "rasterizer/rasterizer.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00B26390, s_emblem_info, decode);
HOOK_DECLARE_CLASS_MEMBER(0x00B267B0, s_emblem_info, encode);

enum
{
	k_ps_emblem_color_background_argb = 55,
	k_ps_emblem_color_icon1_argb = 56,
	k_ps_emblem_color_icon2_argb = 57,
};

s_emblem_info g_user_interface_emblem_info;

void __thiscall s_emblem_info::decode(c_bitstream* packet)
{
	foreground_emblem_index = (uns8)packet->read_integer("foreground-emblem", 6);
	background_emblem_index = (uns8)packet->read_integer("background-emblem", 6);
	emblem_info_flags.set_unsafe((uns8)packet->read_integer("emblem-flags", 3));
	primary_color_index.set_raw_value((int8)packet->read_integer("emblem-primary-color", 6));
	secondary_color_index.set_raw_value((int8)packet->read_integer("emblem-secondary-color", 6));
	background_color_index.set_raw_value((int8)packet->read_integer("emblem-background-color", 6));
}

// $TODO This function only exists because member_to_static does not support const members.
void __thiscall s_emblem_info::encode(c_bitstream* packet)
{
	packet->write_integer("foreground-emblem", foreground_emblem_index, 6);
	packet->write_integer("background-emblem", background_emblem_index, 6);
	packet->write_integer("emblem-flags", emblem_info_flags.get_unsafe(), 3);
	packet->write_integer("emblem-primary-color", primary_color_index, 6);
	packet->write_integer("emblem-secondary-color", secondary_color_index, 6);
	packet->write_integer("emblem-background-color", background_color_index, 6);
}

void __thiscall s_emblem_info::encode_const(c_bitstream* packet) const
{
	packet->write_integer("foreground-emblem", foreground_emblem_index, 6);
	packet->write_integer("background-emblem", background_emblem_index, 6);
	packet->write_integer("emblem-flags", emblem_info_flags.get_unsafe(), 3);
	packet->write_integer("emblem-primary-color", primary_color_index, 6);
	packet->write_integer("emblem-secondary-color", secondary_color_index, 6);
	packet->write_integer("emblem-background-color", background_color_index, 6);
}

s_emblem_info* emblem_get_render_constants_emblem_info_from_user_interface(s_emblem_info* emblem_info)
{
	ASSERT(emblem_info != NULL);

	*emblem_info = g_user_interface_emblem_info;
	return emblem_info;
}

bool emblem_set_render_constants_for_local_user(e_controller_index controller_index, int16* foreground_emblem, int16* background_emblem)
{
	if (VALID_CONTROLLER(controller_index))
	{
		s_emblem_info emblem_info = controller_get(controller_index)->get_player_profile_interface()->get_emblem_info();
		emblem_set_render_constants_from_user_interface(&emblem_info);

		if (foreground_emblem)
		{
			*foreground_emblem = emblem_info.foreground_emblem_index;
		}

		if (background_emblem)
		{
			*background_emblem = emblem_info.background_emblem_index;
		}

		return true;
	}

	if (controller_index == k_any_controller)
	{
		if (const s_player_configuration* player_data = user_interface_squad_get_player_data(0))
		{
			if (foreground_emblem)
			{
				*foreground_emblem = player_data->host.appearance.emblem_info.foreground_emblem_index;
			}

			if (background_emblem)
			{
				*background_emblem = player_data->host.appearance.emblem_info.background_emblem_index;
			}

			return true;
		}
	}

	return false;
}

bool emblem_set_render_constants_from_user_interface(const s_emblem_info* emblem_info)
{
	ASSERT(emblem_info != NULL);

	g_user_interface_emblem_info = *emblem_info;

	real_rgb_color emblem_color_background_rgb = player_profile_get_rgb_color(emblem_info->background_color_index);
	real_rgb_color emblem_color_icon1_rgb = player_profile_get_rgb_color(emblem_info->primary_color_index);
	real_rgb_color emblem_color_icon2_rgb = player_profile_get_rgb_color(emblem_info->secondary_color_index);
	
	bool emblem_alternate_foreground_channel_enabled = emblem_info->emblem_info_flags.test(_emblem_info_flag_alternate_foreground_channel_off);
	bool emblem_flip_foreground = emblem_info->emblem_info_flags.test(_emblem_info_flag_flip_foreground);
	bool emblem_flag_flip_background = emblem_info->emblem_info_flags.test(_emblem_info_flag_flip_background);
	
	real_vector4d emblem_color_background_argb{};
	real_vector4d emblem_color_icon1_argb{};
	real_vector4d emblem_color_icon2_argb{};
	
	real_rgb_color_to_real_vector4d(&emblem_color_background_rgb, emblem_alternate_foreground_channel_enabled ? 0.0f : 1.0f, &emblem_color_background_argb);
	real_rgb_color_to_real_vector4d(&emblem_color_icon1_rgb, emblem_flip_foreground ? 0.0f : 1.0f, &emblem_color_icon1_argb);
	real_rgb_color_to_real_vector4d(&emblem_color_icon2_rgb, emblem_flag_flip_background ? 0.0f : 1.0f, &emblem_color_icon2_argb);
	
	c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_background_argb, 1, &emblem_color_background_argb);
	c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_icon1_argb, 1, &emblem_color_icon1_argb);
	c_rasterizer::set_pixel_shader_constant(k_ps_emblem_color_icon2_argb, 1, &emblem_color_icon2_argb);

	return true;
}

