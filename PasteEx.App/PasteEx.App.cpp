#include <windows.h>

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    MessageBox(NULL, L"PasteEx Shell Extension is installed.\nYou can now use 'New Folder with Selection' in File Explorer.", L"PasteEx", MB_OK | MB_ICONINFORMATION);
    return 0;
}
