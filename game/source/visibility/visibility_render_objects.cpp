#include "visibility/visibility_render_objects.hpp"

#include "memory/module.hpp"
#include "objects/objects.hpp"

HOOK_DECLARE_CALL(0x00A44961, visibility_render_objects_compute_alpha_and_shadow_alpha);

void __cdecl visibility_render_objects_compute_alpha_and_shadow_alpha(long object_index, real shadow_fade_distance, byte* desired_alpha)
{
	// get the object
	if (void* object = object_try_and_get_and_verify_type(object_index, 0xFFFFFFFF))
	{
		// get the object flags
		dword_flags* flags = &reinterpret_cast<dword_flags*>(object)[1];

		// save the flags
		dword_flags previous_flags = *flags;

		// set cinematic lod bit
		*flags |= FLAG(19);

		INVOKE(0x00A195E0, visibility_render_objects_compute_alpha_and_shadow_alpha, object_index, shadow_fade_distance, desired_alpha);

		// reset the flags
		*flags = previous_flags;

		return;
	}

	INVOKE(0x00A195E0, visibility_render_objects_compute_alpha_and_shadow_alpha, object_index, shadow_fade_distance, desired_alpha);
}

