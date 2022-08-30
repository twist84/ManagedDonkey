#pragma once

#include "cseries/cseries.hpp"

struct s_game_cluster_bit_vectors
{
	c_static_array<c_big_flags<long, 256>, 16> flags;
};
static_assert(sizeof(s_game_cluster_bit_vectors) == 0x200);
