#pragma once

long const k_minimum_physics_model_shape_reference_count = 128;

struct hkReferencedObject;
extern void __cdecl halo_external_breakpoint();
extern bool __cdecl halo_managed_memory_address(hkReferencedObject const* object);
extern void __cdecl halo_verify_reference_count(hkReferencedObject* object, long new_reference_count);

