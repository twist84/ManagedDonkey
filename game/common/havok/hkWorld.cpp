#include "hkWorld.hpp"

#include "memory/module.hpp"

HOOK_DECLARE_CLASS_MEMBER(0x00C78040, hkWorld, stepDeltaTime);

template<>
hkArray<hkEntity*>::hkArray()
{
	m_data = nullptr;
	m_size = 0;
	m_capacityAndFlags = 0x80000000;
}

template<>
void hkArray<hkEntity*>::pushBack(hkEntity** t)
{
	INVOKE_CLASS_MEMBER(0x00C7FAA0, hkArray<hkEntity*>, pushBack, t);
}

template<>
int hkArray<hkEntity*>::getSize() const
{
	return m_size;
}

//template<>
//const hkEntity*& hkArray<hkEntity*>::operator[](int i) const
//{
//	return m_data[i];
//}

template<>
hkEntity*& hkArray<hkEntity*>::operator[](int i)
{
	return m_data[i];
}

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

