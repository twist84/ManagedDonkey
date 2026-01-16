#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

enum e_life_cycle_state_handler_in_match_flags
{
	_life_cycle_state_handler_in_match_map_load = 0,

	// any ODST flags?

	k_life_cycle_state_handler_in_match_flags_count,
};
typedef c_flags<e_life_cycle_state_handler_in_match_flags, uns8, k_life_cycle_state_handler_in_match_flags_count> c_life_cycle_state_handler_in_match_flags;

class c_life_cycle_state_handler_in_match :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_in_match();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
	bool is_map_load_pending() const;
	
//protected:
	c_life_cycle_state_handler_in_match_flags m_flags;
	uns32 m_match_start_time;
	uns64 m_match_game_instance;
	bool m_handled_simulation_abort;
	uns32 m_simulation_aborted_timestamp;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_in_match) == 0x40);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_in_match, m_flags));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_in_match, m_match_start_time));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_in_match, m_match_game_instance));
COMPILE_ASSERT(0x38 == OFFSETOF(c_life_cycle_state_handler_in_match, m_handled_simulation_abort));
COMPILE_ASSERT(0x3C == OFFSETOF(c_life_cycle_state_handler_in_match, m_simulation_aborted_timestamp));

