#include "hs/hs_runtime.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "scenario/scenario.hpp"

//bool hs_evaluate(long, long, hs_destination_pointer, long*)
bool __cdecl hs_evaluate(long thread_index, long expression_index, long destination_pointer, long* out_cast)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00594510, hs_evaluate, thread_index, expression_index, destination_pointer, out_cast);
}

//long hs_runtime_script_begin(short, e_hs_script_type, e_hs_thread_type)
long __cdecl hs_runtime_script_begin(short script_index, short script_type, char thread_type)
{
    FUNCTION_BEGIN(true);

    //return INVOKE(0x00598050, hs_runtime_script_begin, script_index, script_type, thread_type);

    s_scenario* scenario = global_scenario_get();
    if (!scenario)
        return NONE;

    if (script_index < 0 || script_index >= scenario->scripts.count)
        return NONE;

    hs_script& script = scenario->scripts.elements[script_index];
    if (script.script_type != script_type)
        return NONE;

    long thread_index = hs_thread_new(thread_type, script_index, true);
    if (thread_index != NONE)
        hs_evaluate(thread_index, script.root_expression_index, 3, nullptr);

    return thread_index;
}

//long hs_thread_new(e_hs_thread_type, long, bool)
long __cdecl hs_thread_new(char thread_index, long script_index, bool deterministic)
{
    FUNCTION_BEGIN(true);

    return INVOKE(0x00598E70, hs_thread_new, thread_index, script_index, deterministic);
}
