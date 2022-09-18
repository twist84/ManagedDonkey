#pragma once

#include "memory/module.hpp"

#include "cache/cache_files.hpp"
#include "cache/security_functions.hpp"

// override header verification
c_hook_call<0x00501A51> cache_files_verify_header_rsa_signature_hook({ .pointer = cache_files_verify_header_rsa_signature });

// override `scenario_load::secure_working_memory` verification
c_hook_call<0x0050309E> security_rsa_compute_and_verify_signature_hook({ .pointer = security_rsa_compute_and_verify_signature });
