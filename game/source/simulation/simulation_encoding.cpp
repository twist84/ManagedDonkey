#include "simulation/simulation_encoding.hpp"

bool __cdecl player_action_compare(player_action const* action_a, player_action* action_b)
{
	return INVOKE(0x0046DCA0, player_action_compare, action_a, action_b);
}

bool __cdecl player_action_decode(c_bitstream* packet, player_action* action)
{
	return INVOKE(0x0046DF80, player_action_decode, packet, action);
}

void __cdecl player_action_encode(c_bitstream* packet, player_action const* action)
{
	INVOKE(0x0046E240, player_action_encode, packet, action);
}

bool __cdecl simulation_camera_update_decode(c_bitstream* packet, s_simulation_camera_update* camera_update)
{
	return INVOKE(0x0046EA20, simulation_camera_update_decode, packet, camera_update);
}

void __cdecl simulation_camera_update_encode(c_bitstream* packet, s_simulation_camera_update const* camera_update)
{
	INVOKE(0x0046EAB0, simulation_camera_update_encode, packet, camera_update);
}

void __cdecl simulation_encoding_get_world_bounds(real_rectangle3d* world_bounds)
{
	INVOKE(0x0046EE60, simulation_encoding_get_world_bounds, world_bounds);
}

bool __cdecl simulation_encoding_point_within_world(real_point3d const* point)
{
	return INVOKE(0x0046EED0, simulation_encoding_point_within_world, point);
}

// is this part of `simulation_encoding`?
// checks if an objects type is biped
//.text:0046EF20 ;

bool __cdecl simulation_machine_update_decode(c_bitstream* packet, simulation_machine_update* machine_update)
{
	return INVOKE(0x0046EF60, simulation_machine_update_decode, packet, machine_update);
}

void __cdecl simulation_machine_update_encode(c_bitstream* packet, simulation_machine_update const* machine_update)
{
	INVOKE(0x0046EFC0, simulation_machine_update_encode, packet, machine_update);
}

bool __cdecl simulation_player_update_decode(c_bitstream* packet, simulation_player_update* player_update)
{
	return INVOKE(0x0046F050, simulation_player_update_decode, packet, player_update);
}

void __cdecl simulation_player_update_encode(c_bitstream* packet, simulation_player_update const* player_update)
{
	INVOKE(0x0046F170, simulation_player_update_encode, packet, player_update);
}

void __cdecl simulation_read_location(c_bitstream* packet, s_location* location)
{
	INVOKE(0x0046F330, simulation_read_location, packet, location);
}

//void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, long axis_encoding_size_in_bits)
void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, long encoding_type, long axis_encoding_size_in_bits)
{
	INVOKE(0x0046F360, simulation_read_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, long axis_encoding_size_in_bits, real_rectangle3d const* world_bounds)
{
	INVOKE(0x0046F3F0, simulation_read_quantized_position, packet, position, axis_encoding_size_in_bits, world_bounds);
}

bool __cdecl simulation_update_compare(struct simulation_update const* update, struct simulation_update* other)
{
	return INVOKE(0x0046F4A0, simulation_update_compare, update, other);
}

bool __cdecl simulation_update_decode(c_bitstream* packet, struct simulation_update* update)
{
	return INVOKE(0x0046F5A0, simulation_update_decode, packet, update);
}

void __cdecl simulation_update_encode(c_bitstream* packet, struct simulation_update const* update)
{
	INVOKE(0x0046F830, simulation_update_encode, packet, update);
}

void __cdecl simulation_write_location(c_bitstream* packet, s_location const* location)
{
	INVOKE(0x0046FC30, simulation_write_location, packet, location);
}

//void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, real_point3d const* node_space_position, enum e_simulation_node_space_encoding_type encoding_type, long axis_encoding_size_in_bits)
void __cdecl simulation_write_quantized_node_space_position(c_bitstream* packet, real_point3d const* node_space_position, long encoding_type, long axis_encoding_size_in_bits)
{
	INVOKE(0x0046FCC0, simulation_write_quantized_node_space_position, packet, node_space_position, encoding_type, axis_encoding_size_in_bits);
}

void __cdecl simulation_write_quantized_position(c_bitstream* packet, real_point3d const* position, long a3, bool a4, real_rectangle3d const* world_bounds)
{
	INVOKE(0x0046FD80, simulation_write_quantized_position, packet, position, a3, a4, world_bounds);
}

bool __cdecl unit_control_decode(c_bitstream* packet, unit_control_data* unit_control)
{
	return INVOKE(0x0046FFE0, unit_control_decode, packet, unit_control);
}

bool __cdecl unit_control_encode(c_bitstream* packet, unit_control_data const* unit_control)
{
	return INVOKE(0x00470180, unit_control_encode, packet, unit_control);
}

