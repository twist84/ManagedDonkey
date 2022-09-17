#include "memory/module.hpp"

#include "cache/cache_files.hpp"
#include "cache/security_functions.hpp"
#include "game/game.hpp"
#include "game/game_engine_util.hpp"
#include "hf2p/hf2p.hpp"
#include "interface/damaged_media.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "scenario/scenario.hpp"

#include <windows.h>
#include <stdio.h>

#include "memory/module.inl"
#include "editor/editor_hooks.inl"
#include "items/weapons_hooks.inl"

c_hook_call<0x00505C2B> main_loop_body_begin_hook({ .pointer = main_loop_body_begin });
c_hook_call<0x0050605C> main_loop_body_end_hook({ .pointer = main_loop_body_end });

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

// test dirty disc error hook
c_hook_call<0x00567964> main_load_map_hook({ .pointer = main_load_map });

// override any mismatched tag checksums
c_hook_call<0x0050286A> crc_checksum_buffer_alder32_hook({ .pointer = crc_checksum_buffer_adler32 });

// override header verification
c_hook_call<0x00501A51> cache_files_verify_header_rsa_signature_hook({ .pointer = cache_files_verify_header_rsa_signature });

// override `scenario_load::secure_working_memory` verification
c_hook_call<0x0050309E> security_rsa_compute_and_verify_signature_hook({ .pointer = security_rsa_compute_and_verify_signature });

c_hook_call<0x005679B5> scenario_load_hook({ .pointer = scenario_load });

// called from `game_engine_running`
c_hook_call<0x00552410> current_game_engine_hook({ .pointer = current_game_engine });

c_hook_call<0x005677AA> game_options_get_launch_settings_hook({ .pointer = game_options_get_launch_settings });

c_hook_call<0x00530CFF> hf2p_dispose_hook({ .pointer = hf2p_dispose });