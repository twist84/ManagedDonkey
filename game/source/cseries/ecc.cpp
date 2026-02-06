#include "cseries/ecc.hpp"

#include "cseries/cseries.hpp"
#include "cseries/cseries_events.hpp"

c_robust_void_pointer::c_robust_void_pointer(void* value) :
	c_robust_void_pointer()
{
	set_value(value);
}

c_robust_void_pointer::c_robust_void_pointer()
{
	set_value(nullptr);
}

void* c_robust_void_pointer::get_corrected_value() const
{
	//INVOKE_CLASS_MEMBER(0x014E9F30, c_robust_void_pointer, get_corrected_value);

	if (!c_robust_void_pointer::internally_consistent())
	{
		event(_event_warning, "ecc: memory error detected, values were 0x%08x, 0x%08x, 0x%08x",
			m_value[0],
			m_value[1],
			m_value[2]);

		if (m_value[1] == m_value[2])
		{
			return m_value[1];
		}

		if (m_value[2] == m_value[0])
		{
			return m_value[2];
		}

		bool value0 = m_value[0] && count_bits(m_value[0]) != 1;
		bool value1 = m_value[1] && count_bits(m_value[1]) != 1;
		bool value2 = m_value[2] && count_bits(m_value[2]) != 1;
		if (!value0 && !value1 && !value2)
		{
			return nullptr;
		}
		else
		{
			event(_event_critical, "ecc: memory error was uncorrectable!");
		}
	}

	return m_value[0];
}

void* c_robust_void_pointer::get_value()
{
	if (!c_robust_void_pointer::internally_consistent())
	{
		c_robust_void_pointer::set_value(get_corrected_value());
	}

	return m_value[0];
}

//void* c_robust_void_pointer::get_value_const() const;

bool c_robust_void_pointer::internally_consistent() const
{
	return m_value[0] == m_value[1] && m_value[0] == m_value[2];
}

void c_robust_void_pointer::set_value(void* value)
{
	do
	{
		m_value[0] = value;
		m_value[1] = value;
		m_value[2] = value;
	}
	while (!c_robust_void_pointer::internally_consistent());
}

