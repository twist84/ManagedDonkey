#include "hkWorld.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00C78040, hkWorld, stepDeltaTime);

hkStepResult __thiscall hkWorld::stepDeltaTime(float physicsDeltaTime)
{
	this;

	//static float physicsDeltaTime_ = 1.0f / 30.0f;
	//c_console::write_line("hkWorld::stepDeltaTime: %f in %f out", physicsDeltaTime, physicsDeltaTime_);
	//physicsDeltaTime = physicsDeltaTime_;

	hkStepResult result = HK_STEP_RESULT_SUCCESS;
	HOOK_INVOKE_CLASS_MEMBER(result =, hkWorld, stepDeltaTime, physicsDeltaTime);
	return result;
}

