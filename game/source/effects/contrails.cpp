#include "effects/contrails.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS(0x00757370, c_contrail, render_callback);

bool contrail_render_enable = false;

void __cdecl c_contrail::render_callback(const void* a1, int32 a2)
{
	const c_contrail* contrail = (const c_contrail*)a1;

	if (contrail_render_enable)
	{
		contrail->render();
	}
}

void c_contrail::render() const
{
	INVOKE_CLASS_MEMBER(0x007572B0, c_contrail, render);
}

void __cdecl c_contrail_system::submit_all(int32 user_index, e_effect_pass effect_pass)
{
	INVOKE(0x00757890, c_contrail_system::submit_all, user_index, effect_pass);
}

