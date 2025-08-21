#pragma once

class c_robust_void_pointer
{
public:
	c_robust_void_pointer(void* value);
	c_robust_void_pointer();
	void* get_corrected_value() const;
	void* get_value();
	//void* get_value_const() const;
	bool internally_consistent() const;
	void set_value(void* value);

	void* m_value[3];
};

template<typename t_type>
class c_robust_pointer
{
public:
	c_robust_pointer() :
		m_pointer()
	{
	}

	c_robust_pointer(t_type* value) :
		m_pointer(value)
	{
	}

	t_type* get_value()
	{
		return (t_type*)m_pointer.get_value();
	}

	//t_type* get_value_const() const
	//{
	//	return m_pointer.get_value_const();
	//}

	void set_value(t_type* value)
	{
		m_pointer.set_value(value);
	}

	c_robust_void_pointer m_pointer;
};

