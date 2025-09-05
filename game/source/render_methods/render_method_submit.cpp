#include "render_methods/render_method_submit.hpp"

REFERENCE_DECLARE(0x050FB608, bool, g_accum_LDR_hack);

void __cdecl render_method_clear_extern(e_render_method_extern render_method_extern)
{
	INVOKE(0x00A3CA30, render_method_clear_extern, render_method_extern);
}

void __cdecl render_method_clear_externs()
{
	INVOKE(0x00A3CA50, render_method_clear_externs);
}

void __cdecl render_method_submit_invalidate_cache()
{
	INVOKE(0x00A3D090, render_method_submit_invalidate_cache);
};

void __cdecl render_method_submit_single_extern(e_render_method_extern render_method_extern, bool fakeout)
{
	INVOKE(0x00A3D200, render_method_submit_single_extern, render_method_extern, fakeout);
}

void __cdecl render_state_cache_dispose()
{
	INVOKE(0x00A3DAF0, render_state_cache_dispose);
}

void __cdecl render_state_cache_dispose_from_old_map()
{
	INVOKE(0x00A3DB00, render_state_cache_dispose_from_old_map);
}

void __cdecl render_state_cache_dispose_from_old_structure_bsp(uns32 deactivating_structure_bsp_mask)
{
	INVOKE(0x00A3DB10, render_state_cache_dispose_from_old_structure_bsp, deactivating_structure_bsp_mask);
}

void __cdecl render_state_cache_initialize()
{
	INVOKE(0x00A3DB20, render_state_cache_initialize);
}

void __cdecl render_state_cache_initialize_for_new_map()
{
	INVOKE(0x00A3DB30, render_state_cache_initialize_for_new_map);
}

void __cdecl render_state_cache_initialize_for_new_structure_bsp(uns32 activating_structure_bsp_mask)
{
	INVOKE(0x00A3DB40, render_state_cache_initialize_for_new_structure_bsp, activating_structure_bsp_mask);
}

const char* k_render_method_extern_names[k_render_method_extern_count]
{
	"none",
	"texaccum target",
	"normal target",
	"z target",
	"shadow 1 target",
	"shadow 2 target",
	"shadow 3 target",
	"shadow 4 target",
	"texture camera target",
	"reflection target",
	"refraction target",
	"lightprobe texture",
	"dominant light intensity texture",
	"unused 1",
	"unused 2",
	"change color primary",
	"change color secondary",
	"change color tertiary",
	"change color quaternary",
	"emblem color background",
	"emblem color primary",
	"emblem color secondary",
	"dynamic environment map 1",
	"dynamic environment map 2",
	"cook torrance cc0236",
	"cook torrance dd0236",
	"cook torrance c78d78",
	"light dir 0",
	"light color 0",
	"light dir 1",
	"light color 1",
	"light dir 2",
	"light color 2",
	"light dir 3",
	"light color 3",
	"unused 3",
	"unused 4",
	"unused 5",
	"dynamic light gel 0",
	"flat envmap matrix x",
	"flat envmap matrix y",
	"flat envmap matrix z",
	"debug tint",
	"screen constants",
	"active camo distortion texture",
	"scene ldr texture",
	"scene hdr texture",
	"water memexport addr",
	"tree animation timer",
	"emblem player shoulder texture",
	"emblem clan chest texture"
};

