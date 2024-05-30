#pragma once

#include "cseries/cseries.hpp"
#include "networking/transport/transport_security.hpp"

struct s_online_session_player
{
	word_flags flags;
	qword xuid;
};
static_assert(sizeof(s_online_session_player) == 0x10);

enum e_controller_index;
struct s_online_session
{
	e_controller_index controller_index;
	word_flags public_slots_flags;
	word_flags private_slots_flags;
	long public_slot_count;
	long private_slot_count;
	s_transport_session_description description;
	qword nonce;
	void* handle;
	c_static_array<s_online_session_player, 16> players;
};
static_assert(sizeof(s_online_session) == 0x150);

