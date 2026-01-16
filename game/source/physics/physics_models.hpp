#pragma once

#include "cseries/cseries.hpp"

struct s_model_definition;
struct s_physics_model_definition;
struct s_physics_model_instance
{
	int32 object_index;
	int32 havok_component_index;
	real_matrix4x3 object_matrix;
	int32 model_definition_index;
	int32 physics_model_definition_index;
	s_model_definition* model;
	s_physics_model_definition* physics_model;
	const char* region_permutation_indices;
	const real_matrix4x3* matrices;
};
COMPILE_ASSERT(sizeof(s_physics_model_instance) == 0x54);

struct hkShape;
struct hkTransform;
extern void matrix4x3_from_hkTransform(real_matrix4x3* matrix, const hkTransform* transform);
extern bool __cdecl physics_model_instance_new(s_physics_model_instance* instance, int32 object_index);
extern void __cdecl render_debug_physics_model(const s_physics_model_instance* instance, const real_argb_color* color);
extern void __cdecl render_debug_physics_shape(const hkShape* shape, const real_matrix4x3* matrix, const real_argb_color* color);

