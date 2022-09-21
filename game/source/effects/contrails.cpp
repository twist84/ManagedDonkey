#include "effects/contrails.hpp"

#include "cseries/cseries.hpp"
#include "memory/module.hpp"

c_hook render_callback_hook(0x00757370, { .pointer = c_contrail::render_callback });

void __cdecl c_contrail::render_callback(void const* a1, long a2)
{
	//HOOK_INVOKE(, render_callback, a1, a2);
}
