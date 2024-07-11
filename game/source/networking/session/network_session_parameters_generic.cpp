#include "networking/session/network_session_parameters_generic.hpp"

#include "cseries/cseries_events.hpp"

template<typename t_type>
inline t_type* c_generic_network_session_parameter<t_type>::get() const
{
	if (!get_allowed())
	{
		generate_event(_event_level_warning, "networking:session_parameters: [%s] failed to get parameter %d [%s], data not available", get_session_description(), m_type, m_name);
		return nullptr;
	}

	return &this->m_data;
}

template<typename t_type>
inline bool c_generic_network_session_parameter<t_type>::set(t_type const* parameter)
{
	generate_event(_event_level_status, "networking:session_parameters: [%s] parameter type %d [%s] being set", get_session_description(), m_type, m_name);

	if (!set_allowed())
	{
		generate_event(_event_level_warning, "networking:session_parameters: [%s] failed to set parameter %d [%s], access denied [%s]", get_session_description(), m_type, m_name, get_set_denied_reason());
		return false;
	}

	if (this->m_data != *parameter || !get_allowed())
	{
		this->m_data = *parameter;
		set_update_required();
	}

	return true;
}

template<typename t_type>
inline bool c_generic_network_session_parameter<t_type>::request_change(t_type const* parameter)
{
	if (set_allowed())
		return set(parameter);

	if (!change_request_allowed())
	{
		generate_event(_event_level_warning, "networking:session_parameters: [%s] failed to request change for parameter %d [%s], access denied [%s]", get_session_description(), m_type, m_name, get_change_request_denied_reason());
		return false;
	}

	this->m_change_request_data = *parameter;
	set_desires_change_request();

	return true;
}

