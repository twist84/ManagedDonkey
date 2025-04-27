#include "main/main_screenshot.hpp"

REFERENCE_DECLARE(0x0244DEE8, bool, g_screenshot_needs_taking);
REFERENCE_DECLARE(0x0244DEE9, bool, g_screenshot_use_relative_path);
REFERENCE_DECLARE(0x0244DEF0, s_screenshot_globals, screenshot_globals);
REFERENCE_DECLARE_ARRAY(0x0244DFC0, wchar_t, g_screenshot_path, 256);

//.text:00610180 ; 

bool __cdecl screenshot_allow_motion_blur()
{
	return INVOKE(0x00610190, screenshot_allow_motion_blur);
}

bool __cdecl screenshot_allow_postprocess()
{
	return INVOKE(0x006101A0, screenshot_allow_postprocess);
}

bool __cdecl screenshot_allow_ui_render()
{
	return INVOKE(0x006101C0, screenshot_allow_ui_render);
}

//.text:006101E0 ; void __cdecl screenshot_cubemap(char const*)
//.text:006101F0 ; void __cdecl screenshot_cubemap_callback(void(__cdecl*)(long, long, long, long, long, long, long, long), bool(__cdecl*)(long, real_point3d*), long)

bool __cdecl screenshot_disable_hud()
{
	return INVOKE(0x00610200, screenshot_disable_hud);
}

bool __cdecl screenshot_sub_610230()
{
	return INVOKE(0x00610230, screenshot_sub_610230);
}

bool __cdecl screenshot_sub_610260()
{
	return INVOKE(0x00610260, screenshot_sub_610260);
}

//.text:00610280 ; bool __cdecl screenshot_full_anisotropic()
//.text:006102A0 ; long __cdecl screenshot_get_anisotropic_level()
//.text:006102C0 ; void __cdecl screenshot_get_scales(real32*, real32*)

bool __cdecl screenshot_in_progress()
{
	return INVOKE(0x00610310, screenshot_in_progress);
}

//.text:00610320 ; real32 __cdecl screenshot_mipmap_lod_bias()
//.text:00610330 ; void __cdecl screenshot_multiple(short, char const*)
//.text:00610390 ; void __cdecl screenshot_multiple_raw(short, char const*)
//.text:006103F0 ; bool __cdecl is_taking_screenshot()

bool __cdecl screenshot_post_render()
{
	return INVOKE(0x00610400, screenshot_post_render);
}

//.text:00610410 ; bool __cdecl screenshot_render(c_player_view*, c_camera_fx_values*, long)
//.text:00610430 ; void __cdecl screenshot_simple(char const*)
//.text:00610470 ; void __cdecl screenshot_single(char const*)
//.text:006104B0 ; void __cdecl screenshot_single_webmap(char const*)
//.text:006104F0 ; void __cdecl screenshot_size(short, short, char const*)
//.text:00610520 ; void __cdecl screenshot_sub_610520()
//.text:00610530 ; bool __cdecl screenshot_sub_610530()
//.text:00610540 ; 
//.text:00610550 ; void __cdecl screenshot_sub_610550(char const*, long, real32, real32, real32)

bool __cdecl screenshot_sub_6105E0()
{
	return INVOKE(0x006105E0, screenshot_sub_6105E0);
}

//.text:00610600 ; 

