#pragma once

struct hs_global_external
{
	const char* name;
	int16 type; // e_hs_type
	void* pointer;
};
COMPILE_ASSERT(sizeof(hs_global_external) == 0xC);

extern hs_global_external* const hs_external_globals[];
extern const int16 k_hs_external_global_count;

