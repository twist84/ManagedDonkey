#pragma once

class c_io_result
{
public:
	virtual void set_fail_boat() = 0;
	virtual void set_disk_failure() = 0;
	virtual void set_checksum_failure() = 0;

	c_io_result();
};

class c_simple_io_result :
	public c_io_result
{
public:
	virtual void set_fail_boat() override;
	virtual void set_disk_failure() override;
	virtual void set_checksum_failure() override;

	c_simple_io_result();
	~c_simple_io_result();

	bool check_success();
	void handle_failure();

	bool checksum_failure() const;
	bool disk_failure() const;
	bool unknown_failure() const;

protected:
	bool m_any_progress;
	bool m_fail_boat;
	bool m_disk_failure;
	bool m_checksum_failure;
	bool m_checked_failure;

	static bool m_halt_and_display_error;
};
COMPILE_ASSERT(sizeof(c_simple_io_result) == 0xC);

