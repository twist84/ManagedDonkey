#include "render_methods/render_method_submit.hpp"

#include "cseries/cseries.hpp"

void __cdecl render_method_submit_invalidate_cache()
{
	INVOKE(0x00A3D090, render_method_submit_invalidate_cache);
};

char const* k_render_method_extern_names[k_render_method_extern_count]
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

