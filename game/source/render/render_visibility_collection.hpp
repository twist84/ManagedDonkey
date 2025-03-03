#pragma once

#include "cseries/cseries.hpp"

struct s_game_cluster_bit_vectors
{
	c_static_array<c_static_flags<256>, 16> flags;
};
static_assert(sizeof(s_game_cluster_bit_vectors) == 0x200);

extern void __cdecl render_visibility_reset_visible_clusters_for_frame();

