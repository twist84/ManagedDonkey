#include "networking/session/network_session_parameters_generic.hpp"

#include "cseries/cseries_events.hpp"

template<typename t_type>
inline t_type* c_generic_network_session_parameter<t_type>::get() const
{
	if (!get_allowed())
	{
		event(_event_warning, "networking:session_parameters: [%s] failed to get parameter %d [%s], data not available",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description);
		return nullptr;
	}

	return &this->m_data;
}

template<typename t_type>
inline bool c_generic_network_session_parameter<t_type>::set(const t_type* parameter)
{
	event(_event_status, "networking:session_parameters: [%s] parameter type %d [%s] being set",
		get_session_description(),
		m_parameter_type,
		m_parameter_type_description);

	if (!set_allowed())
	{
		event(_event_warning, "networking:session_parameters: [%s] failed to set parameter %d [%s], access denied [%s]",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description,
			get_set_denied_reason());
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
inline bool c_generic_network_session_parameter<t_type>::request_change(const t_type* parameter)
{
	if (set_allowed())
		return set(parameter);

	if (!change_request_allowed())
	{
		event(_event_warning, "networking:session_parameters: [%s] failed to request change for parameter %d [%s], access denied [%s]",
			get_session_description(),
			m_parameter_type,
			m_parameter_type_description,
			get_change_request_denied_reason());
		return false;
	}

	this->m_change_request_data = *parameter;
	set_desires_change_request();

	return true;
}

