#include "ai/cl_engine.hpp"

#include "scenario/scenario.hpp"
#include "scenario/scenario_definitions.hpp"

cs_script_data* cs_scenario_get_script_data(struct scenario* scenario)
{
	return &scenario->scripting_data[0];
}

cs_point_set* cs_get_point_set(long index)
{
	return &global_scenario_get()->scripting_data[0].point_sets[index];
}

