#pragma once

#include "cseries/cseries.hpp"
#include "networking/session/network_session_parameters_base.hpp"
#include "networking/transport/transport_security.hpp"

struct s_network_message_parameters_update
{
	s_transport_secure_identifier session_id;
	bool initial_update;
	qword cleared_parameters;
	qword updated_parameters;
	s_network_session_parameter_update update;
};
static_assert(sizeof(s_network_message_parameters_update) == 0x20028);

struct s_network_message_parameters_request
{
	s_transport_secure_identifier session_id;
	qword change_request_parameters;
	s_network_session_parameter_update update;
};
static_assert(sizeof(s_network_message_parameters_request) == 0x20018);

