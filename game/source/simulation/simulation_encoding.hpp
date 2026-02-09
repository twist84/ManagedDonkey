#pragma once

class c_bitstream;
struct player_action;
struct s_location;
struct s_simulation_camera_update;
struct simulation_machine_update;
struct simulation_player_update;
typedef struct simulation_update s_simulation_update;
struct unit_control_data;
union real_point3d;
union real_rectangle3d;

extern bool __cdecl player_action_compare(const player_action* action_a, player_action* action_b);
extern bool __cdecl player_action_decode(c_bitstream* packet, player_action* action);
extern void __cdecl player_action_encode(c_bitstream* packet, const player_action* action);
extern bool __cdecl simulation_camera_update_decode(c_bitstream* packet, s_simulation_camera_update* camera_update);
extern void __cdecl simulation_camera_update_encode(c_bitstream* packet, const s_simulation_camera_update* camera_update);
extern void __cdecl simulation_encoding_get_world_bounds(real_rectangle3d* world_bounds);
extern bool __cdecl simulation_encoding_point_within_world(const real_point3d* point);
extern bool __cdecl simulation_machine_update_decode(c_bitstream* packet, simulation_machine_update* machine_update);
extern void __cdecl simulation_machine_update_encode(c_bitstream* packet, const simulation_machine_update* machine_update);
extern bool __cdecl simulation_player_update_decode(c_bitstream* packet, simulation_player_update* player_update);
extern void __cdecl simulation_player_update_encode(c_bitstream* packet, const simulation_player_update* player_update);
extern void __cdecl simulation_read_location(c_bitstream* packet, s_location* location);
extern void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, int32 encoding_type, int32 axis_encoding_size_in_bits);
extern void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, int32 axis_encoding_size_in_bits, const real_rectangle3d* world_bounds);
extern bool __cdecl simulation_update_compare(const s_simulation_update* update1, s_simulation_update* update2);
extern bool __cdecl simulation_update_decode(c_bitstream* packet, s_simulation_update* update);
extern void __cdecl simulation_update_encode(c_bitstream* packet, const s_simulation_update* update);
extern void __cdecl simulation_write_location(c_bitstream* packet, const s_location* location);
extern void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, const real_point3d* node_space_position, int32 encoding_type, int32 axis_encoding_size_in_bits);
extern void __cdecl simulation_write_quantized_position(c_bitstream* packet, const real_point3d* position, int32 a3, bool a4, const real_rectangle3d* world_bounds);
extern bool __cdecl unit_control_decode(c_bitstream* bitstream, unit_control_data* unit_control);
extern bool __cdecl unit_control_encode(c_bitstream* bitstream, const unit_control_data* unit_control);

