#pragma once

#include "cseries/cseries.hpp"
#include "networking/logic/life_cycle/life_cycle_state_handler.hpp"

enum e_life_cycle_state_handler_in_game_flags
{
	_life_cycle_state_handler_in_game_map_load = 0,
	_life_cycle_state_handler_in_game_core_load_pending,

	// ODST?
	_life_cycle_state_handler_in_game_bit2,

	// any more ODST flags?

	k_life_cycle_state_handler_in_game_flags_count,
};
typedef c_flags<e_life_cycle_state_handler_in_game_flags, uns8, k_life_cycle_state_handler_in_game_flags_count> c_life_cycle_state_handler_in_game_flags;

class c_life_cycle_state_handler_in_game :
	public c_life_cycle_state_handler
{
public:
	virtual void update() override;
	virtual e_life_cycle_state_transition_type update_for_state_transition() override;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data) override;
	virtual void exit(c_life_cycle_state_handler* to) override;
	virtual const char* get_state_string() override;

public:
	c_life_cycle_state_handler_in_game();
	void dispose();
	void initialize(c_life_cycle_state_manager* manager);
	bool is_map_load_pending() const;
	void notify_core_load();
	
//protected:
	c_life_cycle_state_handler_in_game_flags m_flags;
	bool m_handled_simulation_abort;
	int32 m_time_of_handled_campaign_quit;
	uns32 m_simulation_aborted_timestamp;
	uns64 m_game_instance;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler_in_game) == 0x40);
COMPILE_ASSERT(0x28 == OFFSETOF(c_life_cycle_state_handler_in_game, m_flags));
COMPILE_ASSERT(0x29 == OFFSETOF(c_life_cycle_state_handler_in_game, m_handled_simulation_abort));
COMPILE_ASSERT(0x2C == OFFSETOF(c_life_cycle_state_handler_in_game, m_time_of_handled_campaign_quit));
COMPILE_ASSERT(0x30 == OFFSETOF(c_life_cycle_state_handler_in_game, m_simulation_aborted_timestamp));
COMPILE_ASSERT(0x38 == OFFSETOF(c_life_cycle_state_handler_in_game, m_game_instance));

