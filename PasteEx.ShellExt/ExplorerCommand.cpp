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
    
    // Using standard name without debug count
    return SHStrDupW(L"\u7528\u6240\u9009\u6587\u4ef6\u65b0\u5efa\u6587\u4ef6\u5939", ppszName);
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
    // Return tooltip
    return SHStrDupW(L"Create a new folder containing the selected items", ppszInfotip);
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
    // FOF_RENAMEONCOLLISION is crucial if we were moving to an existing folder,
    // but here we are creating a new one.
    hr = pfo->SetOperationFlags(FOF_ALLOWUNDO | FOF_RENAMEONCOLLISION);
    if (FAILED(hr)) return hr;

    // Get parent path
    LPWSTR pszParentPath = NULL;
    hr = psiParent->GetDisplayName(SIGDN_FILESYSPATH, &pszParentPath);
    if (FAILED(hr)) return hr;
    
    // Generate a unique name for the new folder
    wchar_t szNewFolderPath[MAX_PATH];
    PathCombineW(szNewFolderPath, pszParentPath, L"New Folder");
    
    int i = 2;
    while (PathFileExistsW(szNewFolderPath))
    {
        wchar_t szName[MAX_PATH];
        swprintf_s(szName, MAX_PATH, L"New Folder (%d)", i++);
        PathCombineW(szNewFolderPath, pszParentPath, szName);
    }
    
    CoTaskMemFree(pszParentPath);

    // Create the directory using standard Win32 API first to ensure we know the path.
    // NOTE: This creation step is NOT undoable via Ctrl+Z, but the subsequent move IS.
    // If the user undoes the move, an empty folder will remain.
    // This is a trade-off for simplicity vs implementing a full IFileOperationProgressSink.
    if (!CreateDirectoryW(szNewFolderPath, NULL))
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // Get IShellItem for the new folder
    ComPtr<IShellItem> psiDestination;
    hr = SHCreateItemFromParsingName(szNewFolderPath, NULL, IID_PPV_ARGS(&psiDestination));
    if (FAILED(hr)) return hr;

    // Queue the move operation
    // Using MoveItems is efficient and shows the standard progress dialog
    hr = pfo->MoveItems(psiItemArray, psiDestination.Get());
    if (FAILED(hr)) return hr;

    hr = pfo->PerformOperations();

    return hr;
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
