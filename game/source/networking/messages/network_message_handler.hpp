#pragma once

#include "cseries/cseries.hpp"

struct c_network_link;
struct c_network_message_type_collection;
struct c_network_message_gateway;
struct c_network_observer;
struct c_network_session_manager;
struct c_network_message_handler
{
	bool m_initialized;
	c_network_link* m_link;
	c_network_message_type_collection* m_message_types;
	c_network_message_gateway* m_message_gateway;
	c_network_observer* m_observer;
	c_network_session_manager* m_session_manager;
};
static_assert(sizeof(c_network_message_handler) == 0x18);