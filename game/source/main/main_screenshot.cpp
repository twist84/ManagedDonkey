#include "main/main_screenshot.hpp"

//.text:00610180 ; 

bool __cdecl screenshot_allow_antialias_blur()
{
	return INVOKE(0x00610190, screenshot_allow_antialias_blur);
}

bool __cdecl screenshot_allow_postprocess()
{
	return INVOKE(0x006101A0, screenshot_allow_postprocess);
}

bool __cdecl screenshot_allow_ui_render()
{
	return INVOKE(0x006101C0, screenshot_allow_ui_render);
}

//.text:006101E0 ; 
//.text:006101F0 ; 
//.text:00610200 ; 

bool __cdecl screenshot_disable_hud()
{
	return INVOKE(0x00610230, screenshot_disable_hud);
}

bool __cdecl sub_610260()
{
	return INVOKE(0x00610260, sub_610260);
}

//.text:00610280 ; 
//.text:006102A0 ; 
//.text:006102C0 ; void __cdecl screenshot_get_scales(real*, real*)

bool __cdecl screenshot_in_progress()
{
	return INVOKE(0x00610310, screenshot_in_progress);
}

//.text:00610320 ; 
//.text:00610330 ; void __cdecl screenshot_multiple(short, char const*)
//.text:00610390 ; 
//.text:006103F0 ; 

bool __cdecl screenshot_post_render()
{
	return INVOKE(0x00610400, screenshot_post_render);
}

//.text:00610410 ; bool __cdecl screenshot_render(c_player_view*, c_camera_fx_values*, long)
//.text:00610430 ; 
//.text:00610470 ; 
//.text:006104B0 ; 
//.text:006104F0 ; 
//.text:00610520 ; 
//.text:00610530 ; 
//.text:00610540 ; 
//.text:00610550 ; 

bool __cdecl sub_6105E0()
{
	return INVOKE(0x006105E0, sub_6105E0);
}

//.text:00610600 ; 

