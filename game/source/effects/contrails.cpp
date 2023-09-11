#include "effects/contrails.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00757370, c_contrail, render_callback);

bool contrail_render_enable = false;

void __cdecl c_contrail::render_callback(void const* a1, long a2)
{
	c_contrail const* contrail = static_cast<c_contrail const*>(a1);

	if (contrail_render_enable)
		contrail->render();
}

void __cdecl c_contrail::render() const
{
	INVOKE_CLASS_MEMBER(0x007572B0, c_contrail::render);
}

