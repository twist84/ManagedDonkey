#pragma once

class c_symbols_reader
{
public:
    c_symbols_reader(wchar_t const* symbols_path);
    ~c_symbols_reader();

    unsigned long get_rva_blocking(wchar_t const* function_name);

private:
    wchar_t m_symbols_path_buffer[260]; // MAX_PATH
    wchar_t m_function_name_buffer[1024];

    bool read_blocked;
};

extern c_symbols_reader symbols_reader;