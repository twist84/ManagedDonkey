#pragma once

#include "cseries/cseries.hpp"
#include "hs/hs_scenario_definitions.hpp"
//#include "main/console.hpp"
#include "hs/hs_globals_external.hpp"

template<typename t_type>
class c_debug_menu_value_hs_global_external
{
public:
	c_debug_menu_value_hs_global_external(const char* hs_global_name)
	{
		ASSERT(hs_global_name);

		m_hs_global_external_index = NONE;
		for (int16 global_index = 0; global_index < k_hs_external_global_count && m_hs_global_external_index == NONE; global_index++)
		{
			const hs_global_external* global_external = hs_external_globals[global_index];

			if (csstricmp(hs_global_name, global_external->name) == 0
				&& global_external->pointer
				&& IN_RANGE_INCLUSIVE(global_external->type, _hs_type_boolean, _hs_type_long_integer))
			{
				m_hs_global_external_index = global_index;
			}
		}
	}

	t_type get()
	{
		byte bytes[sizeof(t_type)];
		csmemset(bytes, 0, sizeof(t_type));

		if (m_hs_global_external_index != NONE)
		{
			ASSERT(hs_external_globals[m_hs_global_external_index]->pointer != NULL);
			*(t_type*)bytes = *(t_type*)hs_external_globals[m_hs_global_external_index]->pointer;
		}

		return *(t_type*)bytes;
	}

	void set(t_type value)
	{
		if (m_hs_global_external_index != NONE)
		{
			ASSERT(hs_external_globals[m_hs_global_external_index]->pointer != NULL);
			*(t_type*)hs_external_globals[m_hs_global_external_index]->pointer = value;
		}
	}

protected:
	int16 m_hs_global_external_index;
};

