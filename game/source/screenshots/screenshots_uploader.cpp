#include "screenshots/screenshots_uploader.hpp"

REFERENCE_DECLARE(0x0244ECE0, c_screenshots_uploader*, g_screenshots_uploader);

void __cdecl screenshots_uploader_dispose()
{
	INVOKE(0x00619610, screenshots_uploader_dispose);
}

void __cdecl screenshots_uploader_dispose_from_old_map()
{
	INVOKE(0x00619620, screenshots_uploader_dispose_from_old_map);
}

//.text:00619670 ; 

void __cdecl screenshots_uploader_initialize()
{
	INVOKE(0x00619680, screenshots_uploader_initialize);
}

void __cdecl screenshots_uploader_initialize_for_new_map()
{
	INVOKE(0x00619690, screenshots_uploader_initialize_for_new_map);
}

//.text:006196A0 ; void __cdecl screenshots_uploader_memory_dispose()
//.text:006196D0 ; void __cdecl screenshots_uploader_memory_initialize(e_map_memory_configuration)
//.text:00619760 ; 

void __cdecl screenshots_uploader_update()
{
	INVOKE(0x00619770, screenshots_uploader_update);
}

