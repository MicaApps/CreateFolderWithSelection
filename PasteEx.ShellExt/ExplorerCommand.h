#pragma once

#include <windows.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wrl.h>

using namespace Microsoft::WRL;

// Define the CLSID for the Explorer Command: {E062D552-3269-4564-98C0-410714272456}
static const GUID CLSID_ExplorerCommand = 
{ 0xe062d552, 0x3269, 0x4564, { 0x98, 0xc0, 0x41, 0x07, 0x14, 0x27, 0x24, 0x56 } };

class __declspec(uuid("E062D552-3269-4564-98C0-410714272456")) ExplorerCommand : public RuntimeClass<RuntimeClassFlags<ClassicCom>, IExplorerCommand, IObjectWithSite>
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
