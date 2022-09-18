#pragma once

#include "memory/module.hpp"

#include "effects/contrails.hpp"

c_data_patch<0x007577D7 + 1> contrail_render_callback_patch(c_contrail::render_callback);