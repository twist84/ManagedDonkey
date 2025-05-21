#include "interface/user_interface_utilities.hpp"

#include "cseries/cseries.hpp"
#include "interface/c_gui_widget.hpp"
#include "math/color_math.hpp"

void __cdecl tint_widget_to_change_color(c_gui_widget* widget, int32 change_color_index, bool change_color_is_team_color)
{
	INVOKE(0x00AA4C80, tint_widget_to_change_color, widget, change_color_index, change_color_is_team_color);
}

void __cdecl tint_widget_to_change_argb_color(c_gui_widget* widget, argb_color color)
{
	real_argb_color real_color{};
	pixel32_to_real_argb_color(color, &real_color);
	widget->modulate_tint_color(&real_color);
}

//.text:00AA4CD0 ; void __cdecl user_interface_build_local_machine_and_players(s_machine_identifier*, int32*, game_player_options*)
//.text:00AA4D80 ; bool __cdecl user_interface_error_display_allowed()
//.text:00AA4DC0 ; bool __cdecl user_interface_get_bytes_size_string(int32, c_static_wchar_string<1024>*)
//.text:00AA4EF0 ; bool __cdecl user_interface_get_date_string(int64, c_static_wchar_string<1024>*)
//.text:00AA50D0 ; 
//.text:00AA5120 ; bool __cdecl user_interface_get_percentage_string(real32, c_static_wchar_string<1024>*)
//.text:00AA51C0 ; 
//.text:00AA5340 ; int32 __cdecl user_interface_get_player_appearance_biped(s_player_appearance const*)
//.text:00AA53B0 ; 
//.text:00AA5410 ; bool __cdecl user_interface_get_player_grade_string(e_experience_rank, e_experience_grade, bool, bool, c_static_wchar_string<1024>*)
//.text:00AA54B0 ; bool __cdecl user_interface_get_player_rank_and_grade_string(e_experience_rank, e_experience_grade, bool, c_static_wchar_string<1024>*)
//.text:00AA5590 ; int32 __cdecl user_interface_get_player_rank_image_index(e_experience_rank, e_experience_grade)
//.text:00AA55C0 ; int32 __cdecl user_interface_get_player_rank_element_handle(e_experience_rank, e_experience_grade)
//.text:00AA55F0 ; bool __cdecl user_interface_get_player_rank_string(e_experience_rank, e_experience_grade, c_static_wchar_string<1024>*)
//.text:00AA5660 ; bool __cdecl user_interface_get_player_total_xp(int32, c_static_wchar_string<1024>*)
//.text:00AA56D0 ; bool __cdecl user_interface_get_real_number_string(real32, c_static_wchar_string<1024>*)
//.text:00AA5700 ; 
//.text:00AA5740 ; bool __cdecl user_interface_get_timespan_string(int32, e_user_interface_timespan_format, bool, c_static_wchar_string<1024>*)

bool __cdecl user_interface_global_string_get(int32 id, c_static_wchar_string<1024>* text)
{
	return INVOKE(0x00AA5970, user_interface_global_string_get, id, text);
}

//.text:00AA59B0 ; bool __cdecl user_interface_global_string_get_and_parse_xml(e_controller_index, int32, int32, wchar_t*, int32)

bool __cdecl user_interface_play_sound(e_user_interface_sound_effect sound, int32 override_sound_tag_index)
{
	return INVOKE(0x00AA5CD0, user_interface_play_sound, sound, override_sound_tag_index);
}

bool __cdecl user_interface_precache_bitmaps_from_tag(int32 bitmap_tag_index)
{
	return INVOKE(0x00AA5FE0, user_interface_precache_bitmaps_from_tag, bitmap_tag_index);
}

