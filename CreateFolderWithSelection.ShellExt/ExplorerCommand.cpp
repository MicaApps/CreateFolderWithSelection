#include "ExplorerCommand.h"
#include <vector>
#include <stdio.h> // For swprintf_s

#include <shlwapi.h>
#include <strsafe.h>
#include <Uxtheme.h>
#include "Resource.h"

extern void Log(PCWSTR format, ...);
extern HINSTANCE g_hInst;

// Helper to check if system is in Dark Mode
bool IsDarkMode()
{
    // Registry key for Personalize settings
    // AppsUseLightTheme: 0 = Dark, 1 = Light
    DWORD value = 1;
    DWORD size = sizeof(value);
    LSTATUS status = SHGetValueW(
        HKEY_CURRENT_USER,
        L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",
        L"AppsUseLightTheme",
        NULL,
        &value,
        &size
    );

    if (status == ERROR_SUCCESS)
    {
        return value == 0;
    }
    
    // Default to Light mode if check fails
    return false;
}

// Helper class to capture the newly created item from IFileOperation
class CGetNewItemSink : public IFileOperationProgressSink
{
public:
    CGetNewItemSink() : _cRef(1), _psiNewItem(NULL) {}
    ~CGetNewItemSink() { if (_psiNewItem) _psiNewItem->Release(); }

    // IUnknown
    IFACEMETHODIMP QueryInterface(REFIID riid, void **ppv)
    {
        static const QITAB qit[] = {
            QITABENT(CGetNewItemSink, IFileOperationProgressSink),
            { 0 },
        };
        return QISearch(this, qit, riid, ppv);
    }

    IFACEMETHODIMP_(ULONG) AddRef() { return InterlockedIncrement(&_cRef); }
    IFACEMETHODIMP_(ULONG) Release()
    {
        ULONG cRef = InterlockedDecrement(&_cRef);
        if (0 == cRef) delete this;
        return cRef;
    }

    // IFileOperationProgressSink methods
    IFACEMETHODIMP StartOperations() { return S_OK; }
    IFACEMETHODIMP FinishOperations(HRESULT hrResult) { return S_OK; }
    IFACEMETHODIMP PreRenameItem(DWORD, IShellItem *, LPCWSTR) { return S_OK; }
    IFACEMETHODIMP PostRenameItem(DWORD, IShellItem *, LPCWSTR, HRESULT, IShellItem *) { return S_OK; }
    IFACEMETHODIMP PreMoveItem(DWORD, IShellItem *, IShellItem *, LPCWSTR) { return S_OK; }
    IFACEMETHODIMP PostMoveItem(DWORD, IShellItem *, IShellItem *, LPCWSTR, HRESULT, IShellItem *) { return S_OK; }
    IFACEMETHODIMP PreCopyItem(DWORD, IShellItem *, IShellItem *, LPCWSTR) { return S_OK; }
    IFACEMETHODIMP PostCopyItem(DWORD, IShellItem *, IShellItem *, LPCWSTR, HRESULT, IShellItem *) { return S_OK; }
    IFACEMETHODIMP PreDeleteItem(DWORD, IShellItem *) { return S_OK; }
    IFACEMETHODIMP PostDeleteItem(DWORD, IShellItem *, HRESULT, IShellItem *) { return S_OK; }
    IFACEMETHODIMP PreNewItem(DWORD, IShellItem *, LPCWSTR) { return S_OK; }
    
    IFACEMETHODIMP PostNewItem(DWORD dwFlags, IShellItem *psiDestinationFolder, LPCWSTR pszNewName, LPCWSTR pszTemplateName, DWORD dwFileAttributes, HRESULT hrNew, IShellItem *psiNewItem)
    {
        if (SUCCEEDED(hrNew) && psiNewItem)
        {
            // Capture the newly created item
            _psiNewItem = psiNewItem;
            _psiNewItem->AddRef();
        }
        return S_OK;
    }

    IFACEMETHODIMP UpdateProgress(UINT, UINT) { return S_OK; }
    IFACEMETHODIMP ResetTimer() { return S_OK; }
    IFACEMETHODIMP PauseTimer() { return S_OK; }
    IFACEMETHODIMP ResumeTimer() { return S_OK; }

    IShellItem* GetNewItem() { return _psiNewItem; }

private:
    long _cRef;
    IShellItem *_psiNewItem;
};

ExplorerCommand::ExplorerCommand()
{
    Log(L"[PasteEx] ExplorerCommand Constructor\n");
}

ExplorerCommand::~ExplorerCommand()
{
    Log(L"[PasteEx] ExplorerCommand Destructor\n");
}

IFACEMETHODIMP ExplorerCommand::GetTitle(IShellItemArray *psiItemArray, LPWSTR *ppszName)
{
    Log(L"[PasteEx] GetTitle called\n");
    
    DWORD count = 0;
    if (psiItemArray)
    {
        psiItemArray->GetCount(&count);
    }

    // Load format string from resource
    WCHAR szFormat[128];
    if (!LoadStringW(g_hInst, IDS_MENU_TITLE, szFormat, ARRAYSIZE(szFormat)))
    {
        // Fallback if load fails
        StringCchCopyW(szFormat, ARRAYSIZE(szFormat), L"Create folder with selection (%d items)");
    }
    
    WCHAR szTitle[128];
    StringCchPrintfW(szTitle, ARRAYSIZE(szTitle), szFormat, count);

    return SHStrDupW(szTitle, ppszName);
}

IFACEMETHODIMP ExplorerCommand::GetIcon(IShellItemArray *psiItemArray, LPWSTR *ppszIcon)
{
    // Determine which icon to use based on system theme
    // Dark Mode -> Use Dark Icon (IDI_ICON_DARK) - Fix: User feedback indicates IDI_ICON_DARK is the correct icon for Dark Mode
    // Light Mode -> Use Light Icon (IDI_ICON_LIGHT)
    int iconId = IsDarkMode() ? IDI_ICON_DARK : IDI_ICON_LIGHT;

    // Get Module Path
    WCHAR szModulePath[MAX_PATH];
    if (GetModuleFileNameW(g_hInst, szModulePath, ARRAYSIZE(szModulePath))) 
    {
        // Format string: "path,-ID"
        WCHAR szIconPath[MAX_PATH + 20];
        StringCchPrintfW(szIconPath, ARRAYSIZE(szIconPath), L"%s,-%d", szModulePath, iconId);
        return SHStrDupW(szIconPath, ppszIcon);
    }

    return E_FAIL;
}

IFACEMETHODIMP ExplorerCommand::GetToolTip(IShellItemArray *psiItemArray, LPWSTR *ppszInfotip)
{
    // Load tooltip from resource
    WCHAR szTooltip[256];
    if (!LoadStringW(g_hInst, IDS_MENU_TOOLTIP, szTooltip, ARRAYSIZE(szTooltip)))
    {
        // Fallback
        StringCchCopyW(szTooltip, ARRAYSIZE(szTooltip), L"Create a new folder containing the selected items");
    }

    return SHStrDupW(szTooltip, ppszInfotip);
}

IFACEMETHODIMP ExplorerCommand::GetCanonicalName(GUID *pguidCommandName)
{
    *pguidCommandName = CLSID_ExplorerCommand;
    return S_OK;
}

IFACEMETHODIMP ExplorerCommand::GetState(IShellItemArray *psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE *pCmdState)
{
    // Log(L"[PasteEx] GetState called\n"); // Reduce noise

    if (!pCmdState) return E_POINTER;

    // Check Registry setting
    HKEY hKey;
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\PasteEx", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
    {
        DWORD dwEnabled = 1;
        DWORD dwSize = sizeof(dwEnabled);
        if (RegQueryValueExW(hKey, L"Enabled", NULL, NULL, (LPBYTE)&dwEnabled, &dwSize) == ERROR_SUCCESS)
        {
            if (dwEnabled == 0)
            {
                *pCmdState = ECS_HIDDEN;
                RegCloseKey(hKey);
                return S_OK;
            }
        }
        RegCloseKey(hKey);
    }
    
    DWORD count = 0;
    if (psiItemArray)
    {
        psiItemArray->GetCount(&count);
    }

    if (count < 2)
    {
        *pCmdState = ECS_HIDDEN;
    }
    else
    {
        *pCmdState = ECS_ENABLED;
    }

    return S_OK;
}

IFACEMETHODIMP ExplorerCommand::Invoke(IShellItemArray *psiItemArray, IBindCtx *pbc)
{
    if (!psiItemArray) return S_OK;

    // Get the first item to determine the parent folder
    ComPtr<IShellItem> psiFirst;
    HRESULT hr = psiItemArray->GetItemAt(0, &psiFirst);
    if (FAILED(hr)) return hr;

    ComPtr<IShellItem> psiParent;
    hr = psiFirst->GetParent(&psiParent);
    if (FAILED(hr)) return hr;

    // Prepare IFileOperation
    ComPtr<IFileOperation> pfo;
    hr = CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfo));
    if (FAILED(hr)) return hr;

    // Allow Undo and handle collisions automatically
    hr = pfo->SetOperationFlags(FOF_ALLOWUNDO | FOF_RENAMEONCOLLISION);
    if (FAILED(hr)) return hr;

    // Get parent path
    LPWSTR pszParentPath = NULL;
    hr = psiParent->GetDisplayName(SIGDN_FILESYSPATH, &pszParentPath);
    if (FAILED(hr)) return hr;
    
    // Load localized new folder name from shell32.dll
    wchar_t szNewFolderName[MAX_PATH] = { 0 };
    HINSTANCE hShell32 = LoadLibraryW(L"shell32.dll");
    bool loaded = false;
    if (hShell32)
    {
        // 30396 is the standard resource ID for "New Folder" in shell32.dll
        if (LoadStringW(hShell32, 30396, szNewFolderName, ARRAYSIZE(szNewFolderName)) > 0)
        {
            loaded = true;
        }
        FreeLibrary(hShell32);
    }
    
    if (!loaded)
    {
        // Fallback to our resource if shell32 fails
        if (LoadStringW(g_hInst, IDS_NEW_FOLDER_NAME, szNewFolderName, ARRAYSIZE(szNewFolderName)) == 0)
        {
            wcscpy_s(szNewFolderName, MAX_PATH, L"New folder");
        }
    }

    // Create Sink to capture the new item
    // We use 'new' because it's a COM object with ref count
    CGetNewItemSink *pSink = new CGetNewItemSink();
    
    // Create the new folder using IFileOperation (Step 1)
    // We pass szNewFolderName. The shell will handle collisions (e.g. append " (2)") automatically
    // because we specified FOF_RENAMEONCOLLISION.
    hr = pfo->NewItem(psiParent.Get(), FILE_ATTRIBUTE_DIRECTORY, szNewFolderName, NULL, pSink);
    if (FAILED(hr))
    {
        pSink->Release();
        CoTaskMemFree(pszParentPath);
        return hr;
    }

    hr = pfo->PerformOperations();
    if (FAILED(hr))
    {
        pSink->Release();
        CoTaskMemFree(pszParentPath);
        return hr;
    }

    // Move items to the new folder (Step 2)
    IShellItem *psiNewFolderRaw = pSink->GetNewItem();
    if (psiNewFolderRaw)
    {
        ComPtr<IShellItem> psiNewFolder = psiNewFolderRaw; // Take ownership via AddRef (implicit in ComPtr assignment from raw ptr? No, ComPtr(T*) does AddRef)
        // Actually, ComPtr assignment from raw pointer calls AddRef.
        // And pSink holds a ref. So we have 2 refs.
        // When pSink is released, it releases 1.
        // When psiNewFolder is destroyed, it releases 1.
        // Correct.
        
        ComPtr<IFileOperation> pfoMove;
        if (SUCCEEDED(CoCreateInstance(CLSID_FileOperation, NULL, CLSCTX_ALL, IID_PPV_ARGS(&pfoMove))))
        {
            pfoMove->SetOperationFlags(FOF_ALLOWUNDO | FOF_RENAMEONCOLLISION);
            pfoMove->MoveItems(psiItemArray, psiNewFolder.Get());
            pfoMove->PerformOperations();
        }
    }
    
    pSink->Release();
    CoTaskMemFree(pszParentPath);
    return S_OK;
}

IFACEMETHODIMP ExplorerCommand::GetFlags(EXPCMDFLAGS *pFlags)
{
    *pFlags = ECF_DEFAULT;
    return S_OK;
}

IFACEMETHODIMP ExplorerCommand::EnumSubCommands(IEnumExplorerCommand **ppEnum)
{
    *ppEnum = NULL;
    return E_NOTIMPL;
}

IFACEMETHODIMP ExplorerCommand::SetSite(IUnknown *pUnkSite)
{
    _punkSite = pUnkSite;
    return S_OK;
}

IFACEMETHODIMP ExplorerCommand::GetSite(REFIID riid, void **ppvSite)
{
    if (_punkSite)
    {
        return _punkSite->QueryInterface(riid, ppvSite);
    }
    return E_FAIL;
}
