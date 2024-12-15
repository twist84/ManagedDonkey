#include "cache/cache_file_io_failure.hpp"

#include "cseries/cseries.hpp"

c_io_result::c_io_result()
{
	//DECLFUNC(0x005FB380, c_io_result*, __thiscall, c_io_result*)(this);
}

c_simple_io_result::c_simple_io_result() :
	c_io_result(),
	m_any_progress(false),
	m_fail_boat(false),
	m_disk_failure(false),
	m_checksum_failure(false),
	m_checked_failure(false)
{
	//DECLFUNC(0x005FB380, c_simple_io_result*, __thiscall, c_simple_io_result*)(this);
}

c_simple_io_result::~c_simple_io_result()
{
	//DECLFUNC(0x005FB3A0, c_simple_io_result*, __thiscall, c_simple_io_result*)(this);

	// you must check for failure!
}

bool c_simple_io_result::check_success()
{
	//return INVOKE_CLASS_MEMBER(0x005FB3B0, c_simple_io_result, check_success);

	m_checked_failure = true;
	return !m_disk_failure && !m_fail_boat && !m_checksum_failure;
}

bool c_simple_io_result::checksum_failure() const
{
	//return INVOKE_CLASS_MEMBER(0x005FB3D0, c_simple_io_result, checksum_failure);

	return m_checksum_failure;
}

bool c_simple_io_result::disk_failure() const
{
	//return INVOKE_CLASS_MEMBER(0x005FB3E0, c_simple_io_result, disk_failure);

	return m_disk_failure;
}

void c_simple_io_result::handle_failure()
{
	//INVOKE_CLASS_MEMBER(0x005FB3F0, c_simple_io_result, handle_failure);

	// damaged_media_halt_and_display_error("c_simple_io_result: i/o failed: fail_boat %d, disk %d, checksum %d")
	//damaged_media_halt_and_display_error();

	throw;
}

void c_simple_io_result::set_checksum_failure()
{
	//INVOKE_CLASS_MEMBER(0x005FB400, c_simple_io_result, set_checksum_failure);

	//if (m_halt_and_display_error)
	//	damaged_media_halt_and_display_error();

	m_checksum_failure = true;
}

void c_simple_io_result::set_disk_failure()
{
	//INVOKE_CLASS_MEMBER(0x005FB420, c_simple_io_result, set_disk_failure);

	//if (m_halt_and_display_error)
	//	damaged_media_halt_and_display_error();

	m_disk_failure = true;
}

void c_simple_io_result::set_fail_boat()
{
	//INVOKE_CLASS_MEMBER(0x005FB440, c_simple_io_result, set_fail_boat);

	//if (m_halt_and_display_error)
	//	damaged_media_halt_and_display_error();

	m_fail_boat = true;
}

bool c_simple_io_result::unknown_failure() const
{
	//INVOKE_CLASS_MEMBER(0x005FB460, c_simple_io_result, unknown_failure);

	return m_fail_boat && !m_disk_failure && !m_checksum_failure;
}

