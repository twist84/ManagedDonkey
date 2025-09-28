#include "rasterizer/rasterizer_synchronization.hpp"

REFERENCE_DECLARE(0x01917CED, bool, g_enable_better_cpu_gpu_sync_hs_setting) = true;
REFERENCE_DECLARE(0x05115AF0, uns32, c_cpu_gpu_synchronizer::m_fence);
REFERENCE_DECLARE(0x05115AF4, bool, c_cpu_gpu_synchronizer::m_fence_is_valid);

void __cdecl c_cpu_gpu_synchronizer::insert_fence_at_beginning_of_frame()
{
	INVOKE(0x00A60000, c_cpu_gpu_synchronizer::insert_fence_at_beginning_of_frame);
}

void __cdecl c_cpu_gpu_synchronizer::wait_for_gpu_to_catch_up()
{
	INVOKE(0x00A60020, c_cpu_gpu_synchronizer::wait_for_gpu_to_catch_up);
}

