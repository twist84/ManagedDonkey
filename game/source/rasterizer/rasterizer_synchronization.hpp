#pragma once

#include "cseries/cseries.hpp"

class c_cpu_gpu_synchronizer
{
public:
	static void __cdecl insert_fence_at_beginning_of_frame();
	static void __cdecl wait_for_gpu_to_catch_up();

private:
	static uns32& m_fence;
	static bool& m_fence_is_valid;
};

extern bool& g_enable_better_cpu_gpu_sync_hs_setting;

