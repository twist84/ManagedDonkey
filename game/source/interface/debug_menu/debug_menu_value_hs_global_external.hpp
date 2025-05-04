#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"
#include "main/console.hpp"

template<typename t_type>
class c_debug_menu_value_hs_global_external
{
public:
	c_debug_menu_value_hs_global_external(char const* hs_global_name)
	{
		ASSERT(hs_global_name);

		m_hs_global_external_index = NONE;
		for (int16 hs_global_external_index = 0; hs_global_external_index < k_console_global_count && m_hs_global_external_index == NONE; hs_global_external_index++)
		{
			if (csstricmp(hs_global_name, k_console_globals[hs_global_external_index].name))
				continue;

			if (!k_console_globals[hs_global_external_index].pointer)
				continue;

			e_hs_type type = k_console_globals[hs_global_external_index].type;
			if (IN_RANGE_INCLUSIVE(type, _hs_type_boolean, _hs_type_long_integer))
				m_hs_global_external_index = hs_global_external_index;
		}
	}


	t_type get()
	{
		byte bytes[sizeof(t_type)];
		csmemset(bytes, 0, sizeof(t_type));

		if (m_hs_global_external_index != NONE)
		{
			ASSERT(k_console_globals[m_hs_global_external_index].pointer != NULL);

			*reinterpret_cast<t_type*>(bytes) = *static_cast<t_type*>(k_console_globals[m_hs_global_external_index].pointer);
		}

		return *reinterpret_cast<t_type*>(bytes);
	}

	void set(t_type value)
	{
		if (m_hs_global_external_index != NONE)
		{
			ASSERT(k_console_globals[m_hs_global_external_index].pointer != NULL);

			*static_cast<t_type*>(k_console_globals[m_hs_global_external_index].pointer) = value;
		}
	}

protected:
	int16 m_hs_global_external_index;
};

