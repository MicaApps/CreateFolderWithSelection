#pragma once

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

// Define the CLSID for the Explorer Command: {0F04C44B-CF3D-4F79-A8D2-AA5748A9F0C0}
static const GUID CLSID_ExplorerCommand = 
{ 0x0F04C44B, 0xCF3D, 0x4F79, { 0xA8, 0xD2, 0xAA, 0x57, 0x48, 0xA9, 0xF0, 0xC0 } };

class __declspec(uuid("0F04C44B-CF3D-4F79-A8D2-AA5748A9F0C0")) ExplorerCommand : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IExplorerCommand, IObjectWithSite>
{
public:
    ExplorerCommand();
    virtual ~ExplorerCommand();

    // IExplorerCommand methods
    IFACEMETHODIMP GetTitle(IShellItemArray *psiItemArray, LPWSTR *ppszName);
    IFACEMETHODIMP GetIcon(IShellItemArray *psiItemArray, LPWSTR *ppszIcon);
    IFACEMETHODIMP GetToolTip(IShellItemArray *psiItemArray, LPWSTR *ppszInfotip);
    IFACEMETHODIMP GetCanonicalName(GUID *pguidCommandName);
    IFACEMETHODIMP GetState(IShellItemArray *psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE *pCmdState);
    IFACEMETHODIMP Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc);
    IFACEMETHODIMP GetFlags(EXPCMDFLAGS *pFlags);
    IFACEMETHODIMP EnumSubCommands(IEnumExplorerCommand **ppEnum);

    // IObjectWithSite methods
    IFACEMETHODIMP SetSite(IUnknown *pUnkSite);
    IFACEMETHODIMP GetSite(REFIID riid, void **ppvSite);

private:
    ComPtr<IUnknown> _punkSite;
};
