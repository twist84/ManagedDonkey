#pragma once

struct c_io_result
{
public:
	virtual void set_fail_boat() = 0;
	virtual void set_disk_failure() = 0;
	virtual void set_checksum_failure() = 0;

	c_io_result();
};

struct c_simple_io_result :
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
	bool __unknown4;
	bool m_fail_boat;
	bool m_disk_failure;
	bool m_checksum_failure;
	bool m_check_success;

	static bool m_halt_and_display_error;
};
static_assert(sizeof(c_simple_io_result) == 0xC);

