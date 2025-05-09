#pragma once

#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

//typedef c_flags<e_life_cycle_state_handler_in_match_flags, uns8, k_life_cycle_state_handler_in_match_flags_count> c_life_cycle_state_handler_in_match_flags;
typedef uns8 c_life_cycle_state_handler_in_match_flags;

struct c_life_cycle_state_handler_in_match :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual char const* get_state_string() override;

	c_life_cycle_state_handler_in_match();
	void initialize(c_life_cycle_state_manager* manager);
	
//protected:

	c_life_cycle_state_handler_in_match_flags m_flags;
	uns32 m_match_start_time;
	uns64 m_match_game_instance;
	bool m_handled_simulation_abort;
	uns32 m_simulation_aborted_timestamp;
};
static_assert(sizeof(c_life_cycle_state_handler_in_match) == 0x40);
static_assert(0x28 == OFFSETOF(c_life_cycle_state_handler_in_match, m_flags));
static_assert(0x2C == OFFSETOF(c_life_cycle_state_handler_in_match, m_match_start_time));
static_assert(0x30 == OFFSETOF(c_life_cycle_state_handler_in_match, m_match_game_instance));
static_assert(0x38 == OFFSETOF(c_life_cycle_state_handler_in_match, m_handled_simulation_abort));
static_assert(0x3C == OFFSETOF(c_life_cycle_state_handler_in_match, m_simulation_aborted_timestamp));

