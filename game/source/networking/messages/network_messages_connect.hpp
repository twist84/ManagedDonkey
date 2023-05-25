#pragma once

#include "cseries/cseries.hpp"
#include "shell/shell.hpp"

struct s_network_message_connect_request
{
	dword channel_identifier;
	dword_flags flags;
};
static_assert(sizeof(s_network_message_connect_request) == 0x8);

struct s_network_message_connect_refuse
{
	dword remote_identifier;
	long reason;
};
static_assert(sizeof(s_network_message_connect_refuse) == 0x8);

struct s_network_message_connect_establish
{
	dword identifier;
	dword remote_identifier;
};
static_assert(sizeof(s_network_message_connect_establish) == 0x8);

struct s_network_message_connect_closed
{
	dword remote_identifier;
	dword identifier;
	c_enum<e_network_connect_closed_reason, long, _network_connect_closed_reason_none, k_network_connect_closed_reason_count> reason;
};
static_assert(sizeof(s_network_message_connect_closed) == 0xC);

