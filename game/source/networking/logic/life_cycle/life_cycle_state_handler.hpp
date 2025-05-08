#pragma once

#include "cseries/cseries.hpp"

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

struct c_life_cycle_state_manager;
struct c_life_cycle_state_handler
{
public:
	virtual void update();
	virtual e_life_cycle_state_transition_type update_for_state_transition();
	virtual void enter(c_life_cycle_state_handler* handler, int32 entry_data_size, void* entry_data);
	virtual void exit(c_life_cycle_state_handler* handler);
	virtual char const* get_state_string();
	virtual void handle_missing_required_session_parameter(e_network_session_type session_type);

	c_life_cycle_state_manager* get_manager() const;

//protected:

	byte __data4[0x4]; // What are you?

	e_life_cycle_state m_state;
	c_life_cycle_state_manager* m_manager;
	c_life_cycle_state_handler_flags m_handler_flags;
	uns64 m_required_squad_session_parameter_mask;
	uns64 m_required_group_session_parameter_mask;
};
static_assert(sizeof(c_life_cycle_state_handler) == 0x28);

#ifdef __INTELLISENSE__
static_assert(0x08 == OFFSETOF(c_life_cycle_state_handler, m_state));
static_assert(0x0C == OFFSETOF(c_life_cycle_state_handler, m_manager));
static_assert(0x10 == OFFSETOF(c_life_cycle_state_handler, m_handler_flags));
static_assert(0x18 == OFFSETOF(c_life_cycle_state_handler, m_required_squad_session_parameter_mask));
static_assert(0x20 == OFFSETOF(c_life_cycle_state_handler, m_required_group_session_parameter_mask));
#else
static_assert(0x0C == OFFSETOF(c_life_cycle_state_handler, m_state));
static_assert(0x10 == OFFSETOF(c_life_cycle_state_handler, m_manager));
static_assert(0x14 == OFFSETOF(c_life_cycle_state_handler, m_handler_flags));
static_assert(0x18 == OFFSETOF(c_life_cycle_state_handler, m_required_squad_session_parameter_mask));
static_assert(0x20 == OFFSETOF(c_life_cycle_state_handler, m_required_group_session_parameter_mask));
#endif // __INTELLENSE__

