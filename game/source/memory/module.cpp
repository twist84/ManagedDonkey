#include "memory/module.hpp"

#include "cache/cache_files.hpp"
#include "cache/security_functions.hpp"
#include "interface/damaged_media.hpp"
#include "main/loading.hpp"
#include "main/main.hpp"
#include "memory/crc.hpp"
#include "scenario/scenario.hpp"

#include <windows.h>
#include <stdio.h>

#include "memory/module.inl"

c_hook_call<0x00505C2B> main_loop_body_begin_call({ .pointer = main_loop_body_begin });
c_hook_call<0x0050605C> main_loop_body_end_call({ .pointer = main_loop_body_end });

// stops hf2p startup functions from running
//c_data_patch<0x018B59D4> g_hf2p_first_run(false);

// Bungie OS exception, dirty disc error
c_hook_call<0x00A9F6C0> damaged_media_display_call({ .pointer = damaged_media_display });
c_data_patch<0x00A9F6C0 + 5> damaged_media_display_call_ret({ 0x90, 0x90, 0x90, 0x90, 0x90 });

c_hook_call<0x00A9F6D9> damaged_media_update_call({ .pointer = damaged_media_display });
c_data_patch<0x00A9F6D9 + 5> damaged_media_update_call_ret({ 0x90, 0x90, 0x90, 0x90, 0x90 });

// test dirty disc error hook
c_hook_call<0x00567964> main_load_map_call({ .pointer = main_load_map });

// override any mismatched tag checksums
c_hook_call<0x0050286A> crc_checksum_buffer_alder32_call({ .pointer = crc_checksum_buffer_alder32 });

// override header verification
c_hook_call<0x00501A51> cache_files_verify_header_rsa_signature_call({ .pointer = cache_files_verify_header_rsa_signature });

// override `scenario_load::secure_working_memory` verification
c_hook_call<0x0050309E> security_rsa_compute_and_verify_signature_call({ .pointer = security_rsa_compute_and_verify_signature });

c_hook_call<0x005679B5> scenario_load_call({ .pointer = scenario_load });