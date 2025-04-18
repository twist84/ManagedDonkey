#include "visibility/visibility_render_objects.hpp"

#include "memory/module.hpp"
#include "objects/objects.hpp"
#include "visibility/visibility_collection.hpp"

HOOK_DECLARE_CALL(0x00A44961, visibility_render_objects_compute_lod_transparency);

bool object_has_cinematic_lod(long object_index)
{
	object_datum* object = object_get(object_index);
	return object && object->object.flags.test(_object_cinematic_lod_bit);
}

void __cdecl visibility_render_objects_compute_lod_transparency(long object_index, real lod_distance, s_lod_transparency* desired_transparency)
{
	bool cinematic_lod = object_has_cinematic_lod(object_index);
	if (!cinematic_lod)
		object_cinematic_lod(object_index, true);

	INVOKE(0x00A195E0, visibility_render_objects_compute_lod_transparency, object_index, lod_distance, desired_transparency);

	if (!cinematic_lod)
		object_cinematic_lod(object_index, false);
}

