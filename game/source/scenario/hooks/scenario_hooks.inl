#pragma once

#include "memory/module.hpp"

#include "scenario/scenario.hpp"

c_hook_call<0x005679B5> scenario_load_hook({ .pointer = scenario_load });