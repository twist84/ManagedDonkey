#include "visibility/visibility_render_objects.hpp"

#include "memory/module.hpp"
#include "objects/objects.hpp"

HOOK_DECLARE_CALL(0x00A44961, visibility_render_objects_compute_alpha_and_shadow_alpha);

bool object_has_cinematic_lod(long object_index)
{
	object_datum* object = object_get(object_index);
	return object && object->object.flags.test(_object_cinematic_lod_bit);
}

void __cdecl visibility_render_objects_compute_alpha_and_shadow_alpha(long object_index, real shadow_fade_distance, byte* desired_alpha)
{
	bool cinematic_lod = object_has_cinematic_lod(object_index);
	if (!cinematic_lod)
		object_cinematic_lod(object_index, true);

	INVOKE(0x00A195E0, visibility_render_objects_compute_alpha_and_shadow_alpha, object_index, shadow_fade_distance, desired_alpha);

	if (!cinematic_lod)
		object_cinematic_lod(object_index, false);
}

