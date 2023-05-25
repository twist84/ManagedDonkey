#pragma once

struct s_network_message_test
{
	bool in_sequence;
	long id;
};
static_assert(sizeof(s_network_message_test) == 0x8);

