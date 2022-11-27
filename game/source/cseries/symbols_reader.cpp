#include "cseries/symbols_reader.hpp"

#include "cseries/console.hpp"

#include <atlbase.h>
#include <dia2.h>
#pragma comment(lib, "diaguids.lib")

#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

#include <map>
#include <string>

// I don't like this, but also don't know of a nostd way of doing it
std::map<std::wstring, DWORD> name_to_rva_map;
std::map<DWORD, std::wstring> rva_to_name_map;

HRESULT pdb_get_rva_from_name(WCHAR(&pdb_path)[MAX_PATH], WCHAR(&function_name)[1024], DWORD* out_rva)
{
    HRESULT result = S_OK;

    if (name_to_rva_map.contains(function_name))
    {
        if (out_rva)
            *out_rva = name_to_rva_map[function_name];

        return result;
    }

    CComPtr<IDiaDataSource> data_source;
    if (FAILED(CoCreateInstance(CLSID_DiaSource, NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource), (void**)&data_source)))
    {
        // hack
        HMODULE hmodule = LoadLibraryW(L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\DIA SDK\\bin\\msdia140.dll");
        if (!hmodule)
            return HRESULT_FROM_WIN32(GetLastError()); // library not found

        BOOL(WINAPI * DllGetClassObject)(REFCLSID, REFIID, LPVOID*) = (decltype(DllGetClassObject))GetProcAddress(hmodule, "DllGetClassObject");
        if (DllGetClassObject == NULL)
            return HRESULT_FROM_WIN32(GetLastError());

        CComPtr<IClassFactory> pClassFactory;
        if (DllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (LPVOID*)&pClassFactory) == FALSE)
            return result;

        if (FAILED(result = pClassFactory->CreateInstance(NULL, IID_IDiaDataSource, (void**)&data_source)))
            return result;

        pClassFactory.Release();
    }

    if (FAILED(result = data_source->loadDataFromPdb(pdb_path)))
        return result;

    CComPtr<IDiaSession> session;
    if (FAILED(result = data_source->openSession(&session)))
        return result;

    CComPtr<IDiaSymbol> global;
    if (FAILED(result = session->get_globalScope(&global)))
        return result;

    CComPtr<IDiaEnumSymbols> enum_symbols;
    if (FAILED(result = global->findChildren(SymTagNull, function_name, nsNone, &enum_symbols)))
        return result;

    CComPtr<IDiaSymbol> current_symbol;
    ULONG fetched = 0;
    while (fetched != 1 && SUCCEEDED(enum_symbols->Next(1, &current_symbol, &fetched)))
    {
        DWORD rva;
        if (FAILED(result = current_symbol->get_relativeVirtualAddress(&rva)))
            continue;

        if (!rva)
            continue;

        if (out_rva)
            *out_rva = rva;
        name_to_rva_map[function_name] = rva;
    }
    current_symbol.Release();
    enum_symbols.Release();
    global.Release();
    data_source.Release();
    session.Release();

    return result;
}

HRESULT pdb_get_name_from_rva(WCHAR(&pdb_path)[MAX_PATH], DWORD rva, WCHAR(&out_function_name)[1024])
{
    HRESULT result = S_OK;

    if (rva_to_name_map.contains(rva))
    {
        wcsncpy_s(out_function_name, rva_to_name_map[rva].c_str(), 1024);

        return result;
    }

    CComPtr<IDiaDataSource> data_source;
    if (FAILED(CoCreateInstance(CLSID_DiaSource, NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource), (void**)&data_source)))
    {
        // hack
        HMODULE hmodule = LoadLibraryW(L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\DIA SDK\\bin\\msdia140.dll");
        if (!hmodule)
            return HRESULT_FROM_WIN32(GetLastError()); // library not found

        BOOL(WINAPI * DllGetClassObject)(REFCLSID, REFIID, LPVOID*) = (decltype(DllGetClassObject))GetProcAddress(hmodule, "DllGetClassObject");
        if (DllGetClassObject == NULL)
            return HRESULT_FROM_WIN32(GetLastError());

        CComPtr<IClassFactory> pClassFactory;
        if (DllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (LPVOID*)&pClassFactory) == FALSE)
            return result;

        if (FAILED(result = pClassFactory->CreateInstance(NULL, IID_IDiaDataSource, (void**)&data_source)))
            return result;

        pClassFactory.Release();
    }

    if (FAILED(result = data_source->loadDataFromPdb(pdb_path)))
        return result;

    CComPtr<IDiaSession> session;
    if (FAILED(result = data_source->openSession(&session)))
        return result;

    CComPtr<IDiaSymbol> global;
    if (FAILED(result = session->get_globalScope(&global)))
        return result;

    CComPtr<IDiaEnumSymbolsByAddr> enum_symbols_by_addr;
    if (FAILED(result = session->getSymbolsByAddr(&enum_symbols_by_addr)))
        return result;

    CComPtr<IDiaSymbol> current_symbol;
    if (FAILED(result = enum_symbols_by_addr->symbolByRVA(rva, &current_symbol)))
        return result;

    BSTR current_symbol_name;
    if (FAILED(result = current_symbol->get_undecoratedNameEx(UNDNAME_NAME_ONLY, &current_symbol_name)))
        return result;

    swprintf_s(out_function_name, current_symbol_name);
    rva_to_name_map[rva] = current_symbol_name;

    current_symbol.Release();
    enum_symbols_by_addr.Release();
    global.Release();
    data_source.Release();
    session.Release();

    return result;
}

c_symbols_reader symbols_reader(L"hf2p_game_client_cache_release.pdb");

c_symbols_reader::c_symbols_reader(wchar_t const* symbols_path) :
    m_symbols_path_buffer(),
    m_function_name_buffer(),
    read_blocked(false)
{
    swprintf_s(m_symbols_path_buffer, symbols_path);
}

c_symbols_reader::~c_symbols_reader()
{
}

unsigned long c_symbols_reader::get_rva_blocking(wchar_t const* function_name)
{
    while (read_blocked)
    {
        Sleep(25);
    }

    unsigned long rva = 0;

    read_blocked = true;
    swprintf_s(m_function_name_buffer, function_name);
    pdb_get_rva_from_name(m_symbols_path_buffer, m_function_name_buffer, &rva);
    read_blocked = false;

    return rva;
}

wchar_t const* c_symbols_reader::get_name_blocking(unsigned long rva)
{
    while (read_blocked)
    {
        Sleep(25);
    }

    read_blocked = true;
    pdb_get_name_from_rva(m_symbols_path_buffer, rva, m_function_name_buffer);
    read_blocked = false;

    return m_function_name_buffer;
}