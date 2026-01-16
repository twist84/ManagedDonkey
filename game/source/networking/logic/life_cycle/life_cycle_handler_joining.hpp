#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_joining_flags, uns8, k_joining_flags_count> c_joining_flags;
typedef uns8 c_joining_flags;

class c_life_cycle_state_handler_joining :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_joining();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
	bool rematch_join_in_progress() const;
	
//protected:
	c_joining_flags m_flags;
	e_gui_game_mode m_failure_game_mode;
	e_life_cycle_state m_previous_state;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_joining) == 0x38);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_joining, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_joining, m_failure_game_mode));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_joining, m_previous_state));

