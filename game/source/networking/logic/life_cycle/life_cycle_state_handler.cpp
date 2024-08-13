#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

c_life_cycle_state_manager* c_life_cycle_state_handler::get_manager() const
{
    ASSERT(m_manager != NULL);

    return m_manager;
}

