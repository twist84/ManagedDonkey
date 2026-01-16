#pragma once

#include "cseries/cseries.hpp"

class c_life_cycle_state_manager;
class c_network_session;

enum e_life_cycle_state_transition_type
{
	_life_cycle_state_transition_unhandled = 0,
	_life_cycle_state_transition_unchanged,
	_life_cycle_state_transition_change_requested,

	k_life_cycle_state_transition_type_count
};

enum e_life_cycle_state_handler_flags
{
	_life_cycle_state_handler_requires_squad_session_bit = 0,
	_life_cycle_state_handler_requires_group_session_bit,
	_life_cycle_state_handler_allows_group_session_bit,
	_life_cycle_state_handler_group_session_disconnect_leaves_squad_bit,
	_life_cycle_state_handler_group_session_disconnect_recreates_group_bit,
	_life_cycle_state_handler_live_disconnection_returns_to_pre_game_bit,

	k_life_cycle_state_handler_flags_count
};
typedef c_flags<e_life_cycle_state_handler_flags, uns8, k_life_cycle_state_handler_flags_count> c_life_cycle_state_handler_flags;

class c_life_cycle_state_handler
{
public:
	virtual void update() = 0;
	virtual e_life_cycle_state_transition_type update_for_state_transition() = 0;
	virtual void enter(c_life_cycle_state_handler* from, int32 entry_data_size, void* entry_data);
	virtual void exit(c_life_cycle_state_handler* to);
	virtual const char* get_state_string() = 0;
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type);

public:
	c_life_cycle_state_handler();

protected:
	bool all_peers_have_main_menu_ready(const c_network_session* session);

public:
	void dispose();
	c_life_cycle_state_manager* get_manager() const;
	uns64 get_required_group_session_parameter_mask() const;
	uns64 get_required_squad_session_parameter_mask() const;
	e_life_cycle_state get_state() const;
	void initialize(c_life_cycle_state_manager* manager, e_life_cycle_state state, const c_life_cycle_state_handler_flags* flags, uns64 required_squad_session_parameter_mask, uns64 required_group_session_parameter_mask);

//protected:
	bool setup_initial_participants(c_network_session* session);

public:
	bool test_flag(e_life_cycle_state_handler_flags flag);

//protected:
	e_life_cycle_state m_state;
	c_life_cycle_state_manager* m_manager;
	c_life_cycle_state_handler_flags m_handler_flags;
	byte pad[0x7];
	uns64 m_required_squad_session_parameter_mask;
	uns64 m_required_group_session_parameter_mask;
};
COMPILE_ASSERT(sizeof(c_life_cycle_state_handler) == 0x28);

#ifdef __INTELLISENSE__
COMPILE_ASSERT(0x04 == OFFSETOF(c_life_cycle_state_handler, m_state));
COMPILE_ASSERT(0x08 == OFFSETOF(c_life_cycle_state_handler, m_manager));
COMPILE_ASSERT(0x0C == OFFSETOF(c_life_cycle_state_handler, m_handler_flags));
COMPILE_ASSERT(0x0D == OFFSETOF(c_life_cycle_state_handler, pad));
COMPILE_ASSERT(0x18 == OFFSETOF(c_life_cycle_state_handler, m_required_squad_session_parameter_mask));
COMPILE_ASSERT(0x20 == OFFSETOF(c_life_cycle_state_handler, m_required_group_session_parameter_mask));
#else
COMPILE_ASSERT(0x08 == OFFSETOF(c_life_cycle_state_handler, m_state));
COMPILE_ASSERT(0x0C == OFFSETOF(c_life_cycle_state_handler, m_manager));
COMPILE_ASSERT(0x10 == OFFSETOF(c_life_cycle_state_handler, m_handler_flags));
COMPILE_ASSERT(0x11 == OFFSETOF(c_life_cycle_state_handler, pad));
COMPILE_ASSERT(0x18 == OFFSETOF(c_life_cycle_state_handler, m_required_squad_session_parameter_mask));
COMPILE_ASSERT(0x20 == OFFSETOF(c_life_cycle_state_handler, m_required_group_session_parameter_mask));
#endif // __INTELLENSE__

