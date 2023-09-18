#include "render/render_objects_static_lighting.hpp"

REFERENCE_DECLARE(0x050FB668, s_render_object_first_person_globals, render_object_globals);

render_lighting const* object_get_cached_render_lighting(long object_index)
{
	return INVOKE(0x00A43A40, object_get_cached_render_lighting, object_index);
}

