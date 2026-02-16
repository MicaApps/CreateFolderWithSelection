#include "ExplorerCommand.h"
#include <wrl/module.h>
#include <stdio.h> // For swprintf_s

using namespace Microsoft::WRL;

void Log(PCWSTR format, ...)
{
    wchar_t buffer[1024];
    va_list args;
    va_start(args, format);
    _vsnwprintf_s(buffer, _countof(buffer), _TRUNCATE, format, args);
    va_end(args);
    OutputDebugStringW(buffer);
}

// Define the module for WRL
CoCreatableClass(ExplorerCommand)

STDAPI DllGetClassObject(_In_ REFCLSID rclsid, _In_ REFIID riid, _Outptr_ LPVOID* ppv)
{
    Log(L"[PasteEx] DllGetClassObject called\n");
    if (ppv == nullptr) return E_POINTER;
    *ppv = nullptr;

    if (rclsid == CLSID_ExplorerCommand)
    {
        return Module<InProc>::GetModule().GetClassObject(rclsid, riid, ppv);
    }
    
    return CLASS_E_CLASSNOTAVAILABLE;
}

STDAPI DllCanUnloadNow()
{
    return Module<InProc>::GetModule().Terminate() ? S_OK : S_FALSE;
}

HINSTANCE g_hInst = NULL;

STDAPI_(BOOL) DllMain(_In_opt_ HINSTANCE hinst, DWORD reason, _In_opt_ void*)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        g_hInst = hinst;
        Log(L"[PasteEx] DllMain: DLL_PROCESS_ATTACH\n");
        DisableThreadLibraryCalls(hinst);
    }
    else if (reason == DLL_PROCESS_DETACH)
    {
        Log(L"[PasteEx] DllMain: DLL_PROCESS_DETACH\n");
    }
    return TRUE;
}
