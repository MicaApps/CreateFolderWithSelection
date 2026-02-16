# PasteEx

A modern Windows context menu extension that allows you to create a new folder containing the selected files, inspired by "Paste into file".

## Features

- **Context Menu Integration**: Adds a "Create folder with selection" option to the Windows 11 context menu.
- **Smart Naming**: Automatically handles folder name collisions (e.g., "New folder (2)") using native Windows Shell logic.
- **Full Localization**: Supports 42 languages, matching Windows Calculator's coverage.
- **WinUI 3 Settings App**: Modern configuration interface to enable/disable the menu item.
- **Undo Support**: Fully supports Ctrl+Z to undo folder creation and file movement.

## Project Structure

- **PasteEx.ShellExt**: C++ Project implementing the `IExplorerCommand` interface for the context menu handler.
- **PasteEx.Gui**: C# WinUI 3 Project for the settings application.
- **PasteEx.Package**: Windows App Packaging Project (WAP) to package both components into a single MSIX installer.

## How to Build and Run

1. Open `PasteEx.sln` in Visual Studio 2022.
2. Ensure you have the following workloads installed:
   - Desktop development with C++
   - .NET Desktop Development
   - Windows App SDK (WinUI 3)
3. Right-click on **PasteEx.Package** in the Solution Explorer and select **Set as Startup Project**.
4. Set the build configuration to **Debug** and platform to **x64**.
5. Press **F5** to build and deploy.

## Troubleshooting

- If the menu item does not appear immediately, try restarting Windows Explorer via Task Manager.
- Ensure Developer Mode is enabled in Windows Settings if you encounter deployment issues.

## Localization

Localization strings are managed in:
- `PasteEx.ShellExt/Resource.rc` (for the context menu)
- `PasteEx.Gui/Strings` (for the settings app)

The project supports 42 languages including English, Chinese (Simplified/Traditional), Japanese, German, French, Spanish, and more.
