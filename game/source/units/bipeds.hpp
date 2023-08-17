#pragma once

#include "cseries/cseries.hpp"

extern bool debug_objects_physics_control_node;
extern bool debug_objects_biped_autoaim_pills;
extern bool debug_objects_ground_plane;
extern bool debug_objects_movement_mode;
extern bool debug_biped_throttle;
extern bool debug_objects_unit_pathfinding_surface;
extern bool debug_objects_pendulum;

extern void __cdecl biped_bumped_object(long object_index, long bump_object_index, vector3d const* linear_velocity);
extern void __cdecl biped_get_autoaim_pill(long biped_index, real_point3d* base, vector3d* height, real* autoaim_width);
extern void __cdecl biped_render_debug(long biped_index);

