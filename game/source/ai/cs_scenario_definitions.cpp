#include "ai/cs_scenario_definitions.hpp"

#include "scenario/scenario.hpp"

int16 cs_point_set_index_by_name(const char* name)
{
	s_scenario* scenario = global_scenario_get();
	if (scenario->cs_script_data.count > 0)
	{
		for (int16 point_set_index = 0; point_set_index < scenario->cs_script_data[0].point_sets.count; point_set_index++)
		{
			cs_point_set& point_set = scenario->cs_script_data[0].point_sets[point_set_index];
			if (csstrcmp(point_set.name, name) == 0)
				return point_set_index;
		}
	}

	return NONE;
}

int16 cs_point_index_by_name(cs_point_set* point_set, const char* name)
{
	for (int16 point_index = 0; point_index < point_set->point_data.count; point_index++)
	{
		cs_point& point = point_set->point_data[point_index];
		if (point.name.is_equal(name))
			return point_index;
	}

	return NONE;
}

