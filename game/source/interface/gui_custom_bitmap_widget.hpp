#pragma once

#include "cseries/cseries.hpp"

struct c_gui_custom_bitmap_widget
{
	enum e_custom_map_image_type
	{
		_custom_map_image_type_blf = 0,
		_custom_map_image_type_sm_blf,
		_custom_map_image_type_variant_blf,
		_custom_map_image_type_film_blf,
		_custom_map_image_type_clip_blf,

		k_custom_map_image_type_count
	};

	static bool __cdecl get_map_filename(e_custom_map_image_type type, long map_id, c_static_string<256>* out_filename);
};