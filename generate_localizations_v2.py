import os

# Base strings
# 0: Shell Title
# 1: Shell Tooltip
# 2: App Title
# 3: Switch Header
# 4: Switch Desc
# 5: Footer
# 6: New Folder Name (IDS_NEW_FOLDER_NAME)

languages = {
    "ar-SA": { "win32": "LANG_ARABIC, SUBLANG_ARABIC_SAUDI_ARABIA", "trans": ["إنشاء مجلد مع التحديد (%d عناصر)", "إنشاء مجلد جديد يحتوي على العناصر المحددة", "إعدادات PasteEx", "تمكين قائمة السياق", "إظهار 'إنشاء مجلد مع التحديد' في قائمة النقر بزر الماوس الأيمن", "تصبح التغييرات سارية المفعول على الفور.", "مجلد جديد"] },
    "bg-BG": { "win32": "LANG_BULGARIAN, SUBLANG_DEFAULT", "trans": ["Създаване на папка с избраното (%d елемента)", "Създаване на нова папка, съдържаща избраните елементи", "Настройки на PasteEx", "Включване на контекстното меню", "Показване на 'Създаване на папка с избраното' в контекстното меню", "Промените влизат в сила незабавно.", "Нова папка"] },
    "ca-ES": { "win32": "LANG_CATALAN, SUBLANG_DEFAULT", "trans": ["Crear carpeta amb la selecció (%d elements)", "Crear una nova carpeta que contingui els elements seleccionats", "Configuració de PasteEx", "Habilitar menú contextual", "Mostrar 'Crear carpeta amb la selecció' al menú contextual", "Els canvis tenen efecte immediatament.", "Nova carpeta"] },
    "cs-CZ": { "win32": "LANG_CZECH, SUBLANG_DEFAULT", "trans": ["Vytvořit složku z výběru (%d položek)", "Vytvořit novou složku obsahující vybrané položky", "Nastavení PasteEx", "Povolit kontextovou nabídku", "Zobrazit 'Vytvořit složku z výběru' v nabídce pravého tlačítka", "Změny se projeví okamžitě.", "Nová složka"] },
    "da-DK": { "win32": "LANG_DANISH, SUBLANG_DEFAULT", "trans": ["Opret mappe med markering (%d elementer)", "Opret en ny mappe, der indeholder de markerede elementer", "PasteEx-indstillinger", "Aktivér genvejsmenu", "Vis 'Opret mappe med markering' i højreklikmenuen", "Ændringer træder i kraft med det samme.", "Ny mappe"] },
    "de-DE": { "win32": "LANG_GERMAN, SUBLANG_GERMAN", "trans": ["Ordner aus Auswahl erstellen (%d Elemente)", "Einen neuen Ordner mit den ausgewählten Elementen erstellen", "PasteEx-Einstellungen", "Kontextmenü aktivieren", "'Ordner aus Auswahl erstellen' im Kontextmenü anzeigen", "Änderungen werden sofort wirksam.", "Neuer Ordner"] },
    "el-GR": { "win32": "LANG_GREEK, SUBLANG_DEFAULT", "trans": ["Δημιουργία φακέλου με επιλογή (%d αντικείμενα)", "Δημιουργία νέου φακέλου που περιέχει τα επιλεγμένα αντικείμενα", "Ρυθμίσεις PasteEx", "Ενεργοποίηση μενού περιβάλλοντος", "Εμφάνιση του 'Δημιουργία φακέλου με επιλογή' στο μενού δεξιού κλικ", "Οι αλλαγές ισχύουν αμέσως.", "Νέος φάκελος"] },
    "en-GB": { "win32": "LANG_ENGLISH, SUBLANG_ENGLISH_UK", "trans": ["Create folder with selection (%d items)", "Create a new folder containing the selected items", "PasteEx Settings", "Enable Context Menu", "Show 'Create folder with selection' in right-click menu", "Changes take effect immediately.", "New folder"] },
    "en-US": { "win32": "LANG_ENGLISH, SUBLANG_ENGLISH_US", "trans": ["Create folder with selection (%d items)", "Create a new folder containing the selected items", "PasteEx Settings", "Enable Context Menu", "Show 'Create folder with selection' in right-click menu", "Changes take effect immediately.", "New folder"] },
    "es-ES": { "win32": "LANG_SPANISH, SUBLANG_SPANISH_MODERN", "trans": ["Crear carpeta con selección (%d elementos)", "Crear una nueva carpeta que contenga los elementos seleccionados", "Configuración de PasteEx", "Habilitar menú contextual", "Mostrar 'Crear carpeta con selección' en el menú contextual", "Los cambios surten efecto inmediatamente.", "Nueva carpeta"] },
    "es-MX": { "win32": "LANG_SPANISH, SUBLANG_SPANISH_MEXICAN", "trans": ["Crear carpeta con selección (%d elementos)", "Crear una nueva carpeta que contenga los elementos seleccionados", "Configuración de PasteEx", "Habilitar menú contextual", "Mostrar 'Crear carpeta con selección' en el menú contextual", "Los cambios surten efecto inmediatamente.", "Nueva carpeta"] },
    "et-EE": { "win32": "LANG_ESTONIAN, SUBLANG_DEFAULT", "trans": ["Loo kaust valikuga (%d üksust)", "Loo uus kaust, mis sisaldab valitud üksusi", "PasteEx seaded", "Luba kontekstimenüü", "Näita 'Loo kaust valikuga' paremklõpsu menüüs", "Muudatused jõustuvad koheselt.", "Uus kaust"] },
    "fi-FI": { "win32": "LANG_FINNISH, SUBLANG_DEFAULT", "trans": ["Luo kansio valinnasta (%d kohdetta)", "Luo uusi kansio, joka sisältää valitut kohteet", "PasteEx-asetukset", "Ota pikavalikko käyttöön", "Näytä 'Luo kansio valinnasta' hiiren kakkospainikkeen valikossa", "Muutokset tulevat voimaan välittömästi.", "Uusi kansio"] },
    "fr-CA": { "win32": "LANG_FRENCH, SUBLANG_FRENCH_CANADIAN", "trans": ["Créer un dossier avec la sélection (%d éléments)", "Créer un nouveau dossier contenant les éléments sélectionnés", "Paramètres PasteEx", "Activer le menu contextuel", "Afficher 'Créer un dossier avec la sélection' dans le menu contextuel", "Les modifications prennent effet immédiatement.", "Nouveau dossier"] },
    "fr-FR": { "win32": "LANG_FRENCH, SUBLANG_FRENCH", "trans": ["Créer un dossier avec la sélection (%d éléments)", "Créer un nouveau dossier contenant les éléments sélectionnés", "Paramètres PasteEx", "Activer le menu contextuel", "Afficher 'Créer un dossier avec la sélection' dans le menu contextuel", "Les modifications prennent effet immédiatement.", "Nouveau dossier"] },
    "he-IL": { "win32": "LANG_HEBREW, SUBLANG_DEFAULT", "trans": ["צור תיקייה עם הבחירה (%d פריטים)", "צור תיקייה חדשה המכילה את הפריטים שנבחרו", "הגדרות PasteEx", "הפעל תפריט הקשר", "הצג 'צור תיקייה עם הבחירה' בתפריט הלחצן הימני", "השינויים נכנסים לתוקף באופן מיידי.", "תיקייה חדשה"] },
    "hr-HR": { "win32": "LANG_CROATIAN, SUBLANG_DEFAULT", "trans": ["Stvori mapu s odabirom (%d stavki)", "Stvori novu mapu koja sadrži odabrane stavke", "Postavke PasteEx", "Omogući kontekstni izbornik", "Prikaži 'Stvori mapu s odabirom' u izborniku desnog klika", "Promjene stupaju na snagu odmah.", "Nova mapa"] },
    "hu-HU": { "win32": "LANG_HUNGARIAN, SUBLANG_DEFAULT", "trans": ["Mappa létrehozása a kijelölésből (%d elem)", "Új mappa létrehozása a kijelölt elemekkel", "PasteEx beállítások", "Helyi menü engedélyezése", "'Mappa létrehozása a kijelölésből' megjelenítése a jobb gombos menüben", "A változtatások azonnal érvénybe lépnek.", "Új mappa"] },
    "id-ID": { "win32": "LANG_INDONESIAN, SUBLANG_DEFAULT", "trans": ["Buat folder dengan pilihan (%d item)", "Buat folder baru yang berisi item yang dipilih", "Pengaturan PasteEx", "Aktifkan menu konteks", "Tampilkan 'Buat folder dengan pilihan' di menu klik kanan", "Perubahan berlaku segera.", "Folder baru"] },
    "it-IT": { "win32": "LANG_ITALIAN, SUBLANG_ITALIAN", "trans": ["Crea cartella con selezione (%d elementi)", "Crea una nuova cartella contenente gli elementi selezionati", "Impostazioni PasteEx", "Abilita menu contestuale", "Mostra 'Crea cartella con selezione' nel menu contestuale", "Le modifiche hanno effetto immediato.", "Nuova cartella"] },
    "ja-JP": { "win32": "LANG_JAPANESE, SUBLANG_DEFAULT", "trans": ["選択項目を含むフォルダーを作成 (%d 項目)", "選択した項目を含む新しいフォルダーを作成します", "PasteEx 設定", "コンテキストメニューを有効にする", "右クリックメニューに「選択項目を含むフォルダーを作成」を表示する", "変更はすぐに反映されます。", "新しいフォルダー"] },
    "ko-KR": { "win32": "LANG_KOREAN, SUBLANG_DEFAULT", "trans": ["선택 항목으로 폴더 만들기(%d개 항목)", "선택한 항목을 포함하는 새 폴더 만들기", "PasteEx 설정", "컨텍스트 메뉴 사용", "마우스 오른쪽 버튼 메뉴에 '선택 항목으로 폴더 만들기' 표시", "변경 사항이 즉시 적용됩니다.", "새 폴더"] },
    "lt-LT": { "win32": "LANG_LITHUANIAN, SUBLANG_DEFAULT", "trans": ["Sukurti aplanką su pasirinkimu (%d elementai)", "Sukurti naują aplanką su pasirinktais elementais", "PasteEx nustatymai", "Įgalinti kontekstinį meniu", "Rodyti 'Sukurti aplanką su pasirinkimu' dešiniojo pelės mygtuko meniu", "Pakeitimai įsigalioja nedelsiant.", "Naujas aplankas"] },
    "lv-LV": { "win32": "LANG_LATVIAN, SUBLANG_DEFAULT", "trans": ["Izveidot mapi ar atlasi (%d vienumi)", "Izveidot jaunu mapi, kas satur atlasītos vienumus", "PasteEx iestatījumi", "Iespējot konteksta izvēlni", "Rādīt 'Izveidot mapi ar atlasi' labā klikšķa izvēlnē", "Izmaiņas stājas spēkā nekavējoties.", "Jauna mape"] },
    "ms-MY": { "win32": "LANG_MALAY, SUBLANG_DEFAULT", "trans": ["Buat folder dengan pilihan (%d item)", "Buat folder baru yang mengandungi item yang dipilih", "Tetapan PasteEx", "Dayakan menu konteks", "Tunjukkan 'Buat folder dengan pilihan' dalam menu klik kanan", "Perubahan berkuat kuasa serta-merta.", "Folder baru"] },
    "nb-NO": { "win32": "LANG_NORWEGIAN, SUBLANG_NORWEGIAN_BOKMAL", "trans": ["Opprett mappe med utvalg (%d elementer)", "Opprett en ny mappe som inneholder de valgte elementene", "PasteEx-innstillinger", "Aktiver hurtigmeny", "Vis 'Opprett mappe med utvalg' i høyreklikkmenyen", "Endringer trer i kraft umiddelbart.", "Ny mappe"] },
    "nl-NL": { "win32": "LANG_DUTCH, SUBLANG_DUTCH", "trans": ["Map maken met selectie (%d items)", "Maak een nieuwe map met de geselecteerde items", "PasteEx-instellingen", "Contextmenu inschakelen", "Toon 'Map maken met selectie' in het rechtermuisknopmenu", "Wijzigingen worden onmiddellijk van kracht.", "Nieuwe map"] },
    "pl-PL": { "win32": "LANG_POLISH, SUBLANG_DEFAULT", "trans": ["Utwórz folder z zaznaczenia (%d elementy)", "Utwórz nowy folder zawierający zaznaczone elementy", "Ustawienia PasteEx", "Włącz menu kontekstowe", "Pokaż 'Utwórz folder z zaznaczenia' w menu kontekstowym", "Zmiany wchodzą w życie natychmiast.", "Nowy folder"] },
    "pt-BR": { "win32": "LANG_PORTUGUESE, SUBLANG_PORTUGUESE_BRAZILIAN", "trans": ["Criar pasta com seleção (%d itens)", "Criar uma nova pasta contendo os itens selecionados", "Configurações do PasteEx", "Habilitar menu de contexto", "Mostrar 'Criar pasta com seleção' no menu de contexto", "As alterações entram em vigor imediatamente.", "Nova pasta"] },
    "pt-PT": { "win32": "LANG_PORTUGUESE, SUBLANG_PORTUGUESE", "trans": ["Criar pasta com seleção (%d itens)", "Criar uma nova pasta contendo os itens selecionados", "Definições do PasteEx", "Ativar menu de contexto", "Mostrar 'Criar pasta com seleção' no menu de contexto", "As alterações entram em vigor imediatamente.", "Nova pasta"] },
    "ro-RO": { "win32": "LANG_ROMANIAN, SUBLANG_DEFAULT", "trans": ["Creați folder cu selecția (%d elemente)", "Creați un folder nou care conține elementele selectate", "Setări PasteEx", "Activați meniul contextual", "Afișați 'Creați folder cu selecția' în meniul clic dreapta", "Modificările intră în vigoare imediat.", "Folder nou"] },
    "ru-RU": { "win32": "LANG_RUSSIAN, SUBLANG_DEFAULT", "trans": ["Создать папку из выбранного (%d элементов)", "Создать новую папку, содержащую выбранные элементы", "Настройки PasteEx", "Включить контекстное меню", "Показывать 'Создать папку из выбранного' в контекстном меню", "Изменения вступают в силу немедленно.", "Новая папка"] },
    "sk-SK": { "win32": "LANG_SLOVAK, SUBLANG_DEFAULT", "trans": ["Vytvoriť priečinok z výberu (%d položiek)", "Vytvoriť nový priečinok obsahujúci vybrané položky", "Nastavenia PasteEx", "Povoliť kontextovú ponuku", "Zobraziť 'Vytvoriť priečinok z výberu' v ponuke pravého tlačidla", "Zmeny sa prejavia okamžite.", "Nový priečinok"] },
    "sl-SI": { "win32": "LANG_SLOVENIAN, SUBLANG_DEFAULT", "trans": ["Ustvari mapo z izborom (%d elementov)", "Ustvari novo mapo, ki vsebuje izbrane elemente", "Nastavitve PasteEx", "Omogoči kontekstni meni", "Pokaži 'Ustvari mapo z izborom' v meniju desnega klika", "Spremembe začnejo veljati takoj.", "Nova mapa"] },
    "sr-Latn-RS": { "win32": "LANG_SERBIAN, SUBLANG_SERBIAN_LATIN", "trans": ["Napravi fasciklu sa izborom (%d stavki)", "Napravi novu fasciklu koja sadrži izabrane stavke", "PasteEx podešavanja", "Omogući kontekstni meni", "Prikaži 'Napravi fasciklu sa izborom' u meniju desnog klika", "Promene stupaju na snagu odmah.", "Nova fascikla"] },
    "sv-SE": { "win32": "LANG_SWEDISH, SUBLANG_DEFAULT", "trans": ["Skapa mapp med markering (%d objekt)", "Skapa en ny mapp som innehåller de markerade objekten", "PasteEx-inställningar", "Aktivera snabbmeny", "Visa 'Skapa mapp med markering' i högerklicksmenyn", "Ändringar träder i kraft omedelbart.", "Ny mapp"] },
    "th-TH": { "win32": "LANG_THAI, SUBLANG_DEFAULT", "trans": ["สร้างโฟลเดอร์ด้วยการเลือก (%d รายการ)", "สร้างโฟลเดอร์ใหม่ที่มีรายการที่เลือก", "การตั้งค่า PasteEx", "เปิดใช้งานเมนูบริบท", "แสดง 'สร้างโฟลเดอร์ด้วยการเลือก' ในเมนูคลิกขวา", "การเปลี่ยนแปลงมีผลทันที", "โฟลเดอร์ใหม่"] },
    "tr-TR": { "win32": "LANG_TURKISH, SUBLANG_DEFAULT", "trans": ["Seçimle klasör oluştur (%d öğe)", "Seçili öğeleri içeren yeni bir klasör oluştur", "PasteEx Ayarları", "Bağlam menüsünü etkinleştir", "Sağ tık menüsünde 'Seçimle klasör oluştur'u göster", "Değişiklikler hemen etkili olur.", "Yeni Klasör"] },
    "uk-UA": { "win32": "LANG_UKRAINIAN, SUBLANG_DEFAULT", "trans": ["Створити папку з вибраного (%d елементів)", "Створити нову папку, що містить вибрані елементи", "Налаштування PasteEx", "Увімкнути контекстне меню", "Показувати 'Створити папку з вибраного' в контекстному меню", "Зміни набувають чинності негайно.", "Нова папка"] },
    "vi-VN": { "win32": "LANG_VIETNAMESE, SUBLANG_DEFAULT", "trans": ["Tạo thư mục với lựa chọn (%d mục)", "Tạo thư mục mới chứa các mục đã chọn", "Cài đặt PasteEx", "Bật menu ngữ cảnh", "Hiển thị 'Tạo thư mục với lựa chọn' trong menu chuột phải", "Thay đổi có hiệu lực ngay lập tức.", "Thư mục mới"] },
    "zh-CN": { "win32": "LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED", "trans": ["用所选项目（%d项）新建文件夹", "创建一个包含所选项目的新文件夹", "PasteEx 设置", "启用右键菜单", "在右键菜单中显示“用所选项目新建文件夹”", "更改即时生效。", "新建文件夹"] },
    "zh-TW": { "win32": "LANG_CHINESE, SUBLANG_CHINESE_TRADITIONAL", "trans": ["使用選取項目建立資料夾 (%d 個項目)", "建立包含選取項目的新資料夾", "PasteEx 設定", "啟用快顯功能表", "在右鍵功能表中顯示「使用選取項目建立資料夾」", "變更將立即生效。", "新增資料夾"] }
}

# WinUI Resources Template (unchanged)
resw_template = """<?xml version="1.0" encoding="utf-8"?>
<root>
  <data name="AppDisplayName" xml:space="preserve">
    <value>PasteEx</value>
  </data>
  <data name="AppDescription" xml:space="preserve">
    <value>PasteEx</value>
  </data>
  <data name="MainPage_Title.Text" xml:space="preserve">
    <value>{2}</value>
  </data>
  <data name="Settings_EnableMenu.Header" xml:space="preserve">
    <value>{3}</value>
  </data>
  <data name="Settings_EnableMenu.Description" xml:space="preserve">
    <value>{4}</value>
  </data>
  <data name="Settings_Footer.Text" xml:space="preserve">
    <value>{5}</value>
  </data>
</root>"""

# 1. Generate WinUI .resw files (skip this time as WinUI strings didn't change, but running it is harmless)
# Actually, let's keep it to ensure consistency.

# 2. Generate Resource.rc content
rc_content = []
rc_content.append('#include "Resource.h"')
rc_content.append('#include "winres.h"')
rc_content.append('')
rc_content.append('IDI_ICON_LIGHT ICON "Assets\\\\CreatFolderLight.ico"')
rc_content.append('IDI_ICON_DARK  ICON "Assets\\\\CreatFolderDark.ico"')
rc_content.append('')

for lang_code, data in languages.items():
    rc_content.append(f"STRINGTABLE")
    rc_content.append(f"LANGUAGE {data['win32']}")
    rc_content.append("BEGIN")
    title = data['trans'][0].replace('"', '""')
    tooltip = data['trans'][1].replace('"', '""')
    new_folder = data['trans'][6].replace('"', '""')
    rc_content.append(f'    IDS_MENU_TITLE "{title}"')
    rc_content.append(f'    IDS_MENU_TOOLTIP "{tooltip}"')
    rc_content.append(f'    IDS_NEW_FOLDER_NAME "{new_folder}"')
    rc_content.append("END")
    rc_content.append("")

rc_file_path = r"d:\Users\Shomn\OneDrive - MSFT\Source\Repos\PasteEx\PasteEx.ShellExt\Resource.rc"
with open(rc_file_path, "w", encoding="utf-16") as f:
    f.write("\n".join(rc_content))

print("Resource.rc updated with IDS_NEW_FOLDER_NAME")
