using Microsoft.UI.Xaml;
using Microsoft.UI.Xaml.Controls;
using Microsoft.Win32;

namespace PasteEx.Gui.Views
{
    public partial class MainPage : Page
    {
        private const string RegistryKeyPath = @"Software\PasteEx";
        private const string ValueName = "Enabled";

        public MainPage()
        {
            this.InitializeComponent();
            LoadSettings();
        }

        private void LoadSettings()
        {
            try
            {
                using (RegistryKey key = Registry.CurrentUser.OpenSubKey(RegistryKeyPath))
                {
                    if (key != null)
                    {
                        object val = key.GetValue(ValueName);
                        if (val is int intVal)
                        {
                            MenuToggle.IsOn = (intVal != 0);
                        }
                    }
                }
            }
            catch { }
        }

        private void MenuToggle_Toggled(object sender, RoutedEventArgs e)
        {
            try
            {
                using (RegistryKey key = Registry.CurrentUser.CreateSubKey(RegistryKeyPath))
                {
                    if (key != null)
                    {
                        key.SetValue(ValueName, MenuToggle.IsOn ? 1 : 0, RegistryValueKind.DWord);
                    }
                }
            }
            catch { }
        }
    }
}
