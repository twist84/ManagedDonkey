#pragma once

#include "networking/session/network_session_parameters_base.hpp"

template <typename t_type>
struct c_generic_network_session_parameter_data
{
protected:
	t_type m_data;
	t_type m_change_request_data;
};

template <typename t_type>
struct c_generic_network_session_parameter :
	public c_network_session_parameter_base, 
	c_generic_network_session_parameter_data<t_type>
{
public:
	t_type* get() const;
	bool set(const t_type* parameter);
	bool request_change(const t_type* parameter);
};

