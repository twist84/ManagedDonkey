#include "effects/contrails.hpp"

#include "cseries/console.hpp"
#include "cseries/cseries.hpp"
#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00757370, c_contrail, render_callback);

void __cdecl c_contrail::render_callback(void const* a1, long a2)
{
	//HOOK_INVOKE_CLASS(, c_contrail, render_callback, decltype(render_callback)*, a1, a2);
}
