#pragma once

#include <windows.h>
#include <detours.h>
#include <shlwapi.h>
#include <stdio.h>

#define STRING_ENDS_WITH(STRING, END_STRING) (memcmp(STRING + strlen(STRING) - strlen(END_STRING), END_STRING, strlen(END_STRING)) == 0)

CHAR ExecutablePath[4096]{};
CHAR CurrentDirectory[4096]{};
CHAR CommandLine[4096]{};
CHAR DllPath[4096]{};

STARTUPINFOA StartupInfo = { .cb = sizeof(STARTUPINFOA) };
PROCESS_INFORMATION ProcessInfo = { };

char const usage[] =
R"(
Info:
	this launcher is required to be in the executable directory
	things denoted with <> are required
	things denoted with [] are optional

Usage:
	launcher.exe <executable_name> <dll_name> [executable command line]

Example:
	launcher.exe eldorado.exe game.dll -centered -cache-memory-increase 1200
)";

template<typename t_data_type>
class c_simple_span
{
public:
	constexpr c_simple_span() :
		m_data(nullptr),
		m_size(0)
	{
	}

	constexpr c_simple_span(t_data_type* data, size_t size) :
		m_data(data),
		m_size(size)
	{
	}

	constexpr c_simple_span(t_data_type* first, t_data_type* last) :
		m_data(first),
		m_size((size_t)(last - first))
	{
	}

	template<size_t t_size>
	constexpr c_simple_span(t_data_type(&array)[t_size]) :
		m_data(array),
		m_size(t_size)
	{
	}

	constexpr t_data_type* begin() const
	{
		return m_data;
	}

	constexpr t_data_type* end() const
	{
		return m_data + m_size;
	}

	constexpr size_t size() const
	{
		return m_size;
	}

	constexpr bool empty() const
	{
		return m_size == 0;
	}

	constexpr t_data_type& operator[](size_t index) const
	{
		return m_data[index];
	}

	constexpr t_data_type& front() const
	{
		return m_data[0];
	}

	constexpr t_data_type& back() const
	{
		return m_data[m_size - 1];
	}

	constexpr bool is_front(const t_data_type& value) const
	{
		return m_size != 0 && value == front();
	}

	constexpr bool is_back(const t_data_type& value) const
	{
		return m_size != 0 && value == back();
	}

private:
	t_data_type* m_data;
	size_t       m_size;
};

