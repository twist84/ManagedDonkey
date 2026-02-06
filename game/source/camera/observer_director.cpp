#include "camera/observer_director.hpp"

#include "memory/module.hpp"
#include "saved_games/saved_film_manager.hpp"

bool __thiscall c_observer_director::should_draw_hud_saved_film_()
{
    return !saved_film_manager_film_is_ended(nullptr) && m_show_hud_state == _observer_director_show_hud_state_both_on;
}

