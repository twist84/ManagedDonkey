#pragma once

class c_animation_id
{
public:
	//c_animation_id(int32, int32);

	c_animation_id() :
		m_subgraph(NONE),
		m_index(NONE)
	{
	}

	~c_animation_id()
	{
	}

	bool valid() const
	{
		return m_index >= 0;
	}

	void clear()
	{
		m_subgraph = NONE;
		m_index = NONE;
	}

	int16 index() const
	{
		return m_index;
	}

	int16 subgraph() const
	{
		return m_subgraph;
	}

	void set_index(int16 index)
	{
		m_index = index;
	}

	void set_subgraph(int16 subgraph)
	{
		m_subgraph = subgraph;
	}

	bool operator==(c_animation_id const& rsa) const
	{
		return m_subgraph == rsa.m_subgraph && m_index == rsa.m_index;
	}

	bool operator!=(c_animation_id const& rsa) const
	{
		return m_subgraph != rsa.m_subgraph || m_index != rsa.m_index;
	}

private:
	int16 m_subgraph;
	int16 m_index;
};
COMPILE_ASSERT(sizeof(c_animation_id) == sizeof(int16) * 2);

