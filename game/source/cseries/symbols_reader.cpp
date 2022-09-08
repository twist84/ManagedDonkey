#include "cseries/symbols_reader.hpp"


#include <atlbase.h>
#include <dia2.h>
#pragma comment(lib, "diaguids.lib")

HRESULT pdb_get_rva_from_name(WCHAR(&pdb_path)[MAX_PATH], WCHAR(&function_name)[1024], DWORD* out_rva)
{
    HRESULT result = S_OK;

    CComPtr<IDiaDataSource> data_source;
    if (FAILED(CoCreateInstance(CLSID_DiaSource, NULL, CLSCTX_INPROC_SERVER, __uuidof(IDiaDataSource), (void**)&data_source)))
    {
        // hack
        HMODULE hmodule = LoadLibraryW(L"C:\\Program Files\\Microsoft Visual Studio\\2022\\Community\\DIA SDK\\bin\\msdia140.dll");
        if (!hmodule)
            return HRESULT_FROM_WIN32(GetLastError()); // library not found

        BOOL(WINAPI * DllGetClassObject)(REFCLSID, REFIID, LPVOID*) = (decltype(DllGetClassObject))GetProcAddress(hmodule, "DllGetClassObject");
        if (!DllGetClassObject)
            return HRESULT_FROM_WIN32(GetLastError());

        CComPtr<IClassFactory> pClassFactory;
        if (FAILED(result = DllGetClassObject(CLSID_DiaSource, IID_IClassFactory, (LPVOID*)&pClassFactory)))
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
        if (FAILED(current_symbol->get_relativeVirtualAddress(&rva)))
            continue;

        if (!rva)
            continue;

        if (out_rva)
            *out_rva = rva;
    }
    current_symbol.Release();
    enum_symbols.Release();
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
