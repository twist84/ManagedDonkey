#pragma once

#include "cseries/cseries.hpp"

struct c_bitstream;
struct player_action;
struct s_simulation_camera_update;
struct simulation_machine_update;
struct simulation_player_update;
struct unit_control_data;

extern bool __cdecl player_action_compare(player_action const* action_a, player_action* action_b);
extern bool __cdecl player_action_decode(c_bitstream* packet, player_action* action);
extern void __cdecl player_action_encode(c_bitstream* packet, player_action const* action);
extern bool __cdecl simulation_camera_update_decode(c_bitstream* packet, s_simulation_camera_update* camera_update);
extern void __cdecl simulation_camera_update_encode(c_bitstream* packet, s_simulation_camera_update const* camera_update);
extern void __cdecl simulation_encoding_get_world_bounds(real_rectangle3d* world_bounds);
extern bool __cdecl simulation_encoding_point_within_world(real_point3d const* point);
extern bool __cdecl simulation_machine_update_decode(c_bitstream* packet, simulation_machine_update* machine_update);
extern void __cdecl simulation_machine_update_encode(c_bitstream* packet, simulation_machine_update const* machine_update);
extern bool __cdecl simulation_player_update_decode(c_bitstream* packet, simulation_player_update* player_update);
extern void __cdecl simulation_player_update_encode(c_bitstream* packet, simulation_player_update const* player_update);
extern void __cdecl simulation_read_location(c_bitstream* packet, s_location* location);
extern void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, long encoding_type, long axis_encoding_size_in_bits);
extern void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, long axis_encoding_size_in_bits, real_rectangle3d const* world_bounds);
extern bool __cdecl simulation_update_compare(struct simulation_update const* update1, struct simulation_update* update2);
extern bool __cdecl simulation_update_decode(c_bitstream* packet, struct simulation_update* update);
extern void __cdecl simulation_update_encode(c_bitstream* packet, struct simulation_update const* update);
extern void __cdecl simulation_write_location(c_bitstream* packet, s_location const* location);
extern void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, real_point3d const* node_space_position, long encoding_type, long axis_encoding_size_in_bits);
extern void __cdecl simulation_write_quantized_position(c_bitstream* packet, real_point3d const* position, long a3, bool a4, real_rectangle3d const* world_bounds);
extern bool __cdecl unit_control_decode(c_bitstream* packet, unit_control_data* unit_control);
extern bool __cdecl unit_control_encode(c_bitstream* packet, unit_control_data const* unit_control);

