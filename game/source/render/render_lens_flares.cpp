#include "render/render_lens_flares.hpp"

#include "cseries/cseries.hpp"

//.text:00A5A840 ; void __cdecl lens_flares_garbage_collect()

void __cdecl lens_flares_initialize()
{
	INVOKE(0x00A5A8E0, lens_flares_initialize);
}

//.text:00A5A920 ; 

void __cdecl lens_flares_render(long a1)
{
	INVOKE(0x00A5A960, lens_flares_render, a1);
}

