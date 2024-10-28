#include "animations/animation_definitions.hpp"

s_animation_graph_node* c_model_animation_graph::get_node(long node_index) const
{
	//return DECLFUNC(0x006DBEC0, s_animation_graph_node*, __thiscall, c_model_animation_graph const*, long)(this, node_index);

	return &definitions.skeleton_nodes[node_index];
}

