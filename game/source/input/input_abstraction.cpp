#include "input/input_abstraction.hpp"

#include <assert.h>

REFERENCE_DECLARE(0x0244C9D0, s_input_abstraction_globals, input_abstraction_globals);

c_abstract_button::c_abstract_button() :
	m_down_msec(),
	m_down_frames(),
	m_flags(),
	m_down_amount(),
	m_accessor(-1),
	m_locked(-1)
{
}

void c_abstract_button::update(word down_msec, word down_frames, byte down_amount)
{
	m_down_msec = down_msec;
	m_down_frames = static_cast<byte>(down_frames);
	m_down_amount = down_amount / 255.0f;

	if (!m_down_frames && latched())
		set_latch_bit(false);
}

void c_abstract_button::set_accessor(long accessor)
{
	m_accessor = static_cast<byte>(accessor);
}

void c_abstract_button::unlock()
{
	m_locked = -1;
}

bool c_abstract_button::locked()
{
	return (m_locked + 1) == 0;
}

void c_abstract_button::lock()
{
	assert(access_valid());
	m_locked = m_accessor;
}

real c_abstract_button::down_amount()
{
	if (!latched() && access_valid())
		return m_down_amount;

	return 0.0f;
}

bool c_abstract_button::access_valid() const
{
	return m_locked == 0xFF || m_locked == m_accessor;
}

word c_abstract_button::down_msec()
{
	if (!latched() && access_valid())
		return m_down_msec;

	return 0;
}

bool c_abstract_button::latched() const
{
	return TEST_BIT(m_flags, 0);
}

byte c_abstract_button::down_frames() const
{
	if (!latched() && access_valid())
		return m_down_frames;

	return 0;
}

void c_abstract_button::set_latch_bit(bool set_bit)
{
	if (set_bit)
		m_flags |= (1 << 0);
	else
		m_flags |= ~(1 << 0);
}

void c_abstract_button::latch()
{
	set_latch_bit(true);
}

