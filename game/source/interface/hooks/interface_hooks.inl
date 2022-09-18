#pragma once

#include "memory/module.hpp"

#include "interface/damaged_media.hpp"

// Bungie OS exception, dirty disc error
c_hook_call<0x00A9F6C0> damaged_media_exception_hook0({ .pointer = damaged_media_exception });
c_hook_call<0x00A9F6D9> damaged_media_exception_hook1({ .pointer = damaged_media_exception });
c_data_patch_array damaged_media_exception_patch(
	{
		damaged_media_exception_hook0.get_address() + 5,
		damaged_media_exception_hook1.get_address() + 5
	},
	{ 0x90, 0x90, 0x90, 0x90, 0x90 }
);
