#include "animations/animation_manager.hpp"

//.text:006DA5A0 ; public: __cdecl c_animation_manager::c_animation_manager(void)

c_model_animation_graph const* c_animation_manager::get_graph() const
{
    return DECLFUNC(0x006DBEC0, c_model_animation_graph const*, __thiscall, c_animation_manager const*)(this);
}

bool c_animation_manager::valid_graph() const
{
    return m_graph_tag_index != NONE;
}

long c_animation_manager::get_state_name() const
{
    return m_state_name.get_value();
}

