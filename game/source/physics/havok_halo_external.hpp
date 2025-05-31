#pragma once

int32 const k_minimum_physics_model_shape_reference_count = 128;

struct hkReferencedObject;
extern void __cdecl halo_external_breakpoint();
extern bool __cdecl halo_managed_memory_address(const hkReferencedObject* object);
extern void __cdecl halo_verify_reference_count(hkReferencedObject* object, int32 new_reference_count);

