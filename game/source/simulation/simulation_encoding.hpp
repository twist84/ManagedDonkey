#pragma once

#include "cseries/cseries.hpp"

struct c_bitstream;

extern void __cdecl simulation_read_quantized_node_space_position(c_bitstream* packet, real_point3d* node_space_position, long encoding_type, long axis_encoding_size_in_bits);
extern void __cdecl simulation_read_quantized_position(c_bitstream* packet, real_point3d* position, long axis_encoding_size_in_bits, real_rectangle3d const* world_bounds);
extern bool __cdecl simulation_update_compare(struct simulation_update const* update, struct simulation_update* other);
extern bool __cdecl simulation_update_decode(c_bitstream* packet, struct simulation_update* update);

