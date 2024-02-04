#include "simulation/simulation_encoding.hpp"

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

