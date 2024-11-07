#include "createandpinsnaarpfolder.h"
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#ifdef _WIN32
#include <windows.h>
#include <shlobj.h>
#include <QUuid>
#endif

CreateAndPinSnaarpFolder::CreateAndPinSnaarpFolder() {
}

void CreateAndPinSnaarpFolder::createFolder(const QString &path) {
    QDir dir(path);
    if (!dir.exists()) {
        if (dir.mkpath(path)) {
            qDebug() << "Folder created successfully at:" << path;
            pinFolderToNavigationPane(path);
        } else {
            qDebug() << "Failed to create folder at:" << path;
        }
    } else {
        qDebug() << "Folder already exists at:" << path;
        pinFolderToNavigationPane(path);
    }
}

void CreateAndPinSnaarpFolder::pinFolderToNavigationPane(const QString &path) {
#ifdef _WIN32
    // Convert path to native format
    QString nativePath = QDir::toNativeSeparators(path);

    // Use the specific icon path
    QString iconPath = QCoreApplication::applicationDirPath() + "/icons/s.ico";

    // Verify icon file exists
    if (!QFileInfo::exists(iconPath)) {
        qDebug() << "Warning: Icon file not found at:" << iconPath;
    }

    // Create desktop.ini file in the folder
    QString desktopIniPath = QDir::toNativeSeparators(path + "/desktop.ini");
    QFile desktopIni(desktopIniPath);
    if (desktopIni.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&desktopIni);
        stream << "[.ShellClassInfo]\n";
        stream << "IconFile=" << iconPath << "\n";
        stream << "IconIndex=0\n";
        desktopIni.close();

        // Set desktop.ini as system and hidden
        SetFileAttributesW(desktopIniPath.toStdWString().c_str(),
                           FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);

        // Set the folder as system to make Windows read the desktop.ini
        SetFileAttributesW(path.toStdWString().c_str(),
                           FILE_ATTRIBUTE_SYSTEM);
    }

    // Create registry entries
    HKEY hKey;
    DWORD disposition;
    QString clsidPath = "Software\\Classes\\CLSID\\{27d3fa3e-06f3-495e-953f-7410df06838e}";

    // Create main CLSID key
    if (RegCreateKeyExW(HKEY_CURRENT_USER, clsidPath.toStdWString().c_str(),
                        0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {

        // Set default value
        QString name = "Snaarp";
        RegSetValueExW(hKey, L"", 0, REG_SZ,
                       (BYTE*)name.toStdWString().c_str(),
                       (name.length() + 1) * sizeof(wchar_t));

        // Set IsPinnedToNamespaceTree
        DWORD isPinned = 1;
        RegSetValueExW(hKey, L"System.IsPinnedToNamespaceTree", 0, REG_DWORD,
                       (BYTE*)&isPinned, sizeof(DWORD));

        // Set SortOrderIndex
        DWORD sortOrder = 0x42;
        RegSetValueExW(hKey, L"SortOrderIndex", 0, REG_DWORD,
                       (BYTE*)&sortOrder, sizeof(DWORD));

        RegCloseKey(hKey);

        // Create DefaultIcon subkey with the specific icon path
        QString iconKeyPath = clsidPath + "\\DefaultIcon";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, iconKeyPath.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            QString iconRegPath = iconPath + ",0";  // Add icon index
            RegSetValueExW(hKey, L"", 0, REG_EXPAND_SZ,
                           (BYTE*)iconRegPath.toStdWString().c_str(),
                           (iconRegPath.length() + 1) * sizeof(wchar_t));
            RegCloseKey(hKey);
        }

        // Create InProcServer32 subkey
        QString serverKeyPath = clsidPath + "\\InProcServer32";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, serverKeyPath.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            QString serverPath = "%SystemRoot%\\System32\\shell32.dll";
            RegSetValueExW(hKey, L"", 0, REG_SZ,
                           (BYTE*)serverPath.toStdWString().c_str(),
                           (serverPath.length() + 1) * sizeof(wchar_t));

            QString threadModel = "Apartment";
            RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ,
                           (BYTE*)threadModel.toStdWString().c_str(),
                           (threadModel.length() + 1) * sizeof(wchar_t));
            RegCloseKey(hKey);
        }

        // Create Instance subkey
        QString instanceKeyPath = clsidPath + "\\Instance";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, instanceKeyPath.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            QString instanceClsid = "{0E5AAE11-A475-4c5b-AB00-C66DE400274E}";
            RegSetValueExW(hKey, L"CLSID", 0, REG_SZ,
                           (BYTE*)instanceClsid.toStdWString().c_str(),
                           (instanceClsid.length() + 1) * sizeof(wchar_t));
            RegCloseKey(hKey);
        }

        // Create InitPropertyBag subkey
        QString propBagKeyPath = clsidPath + "\\Instance\\InitPropertyBag";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, propBagKeyPath.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            DWORD attributes = 0x11;
            RegSetValueExW(hKey, L"Attributes", 0, REG_DWORD,
                           (BYTE*)&attributes, sizeof(DWORD));

            RegSetValueExW(hKey, L"TargetFolderPath", 0, REG_SZ,
                           (BYTE*)nativePath.toStdWString().c_str(),
                           (nativePath.length() + 1) * sizeof(wchar_t));
            RegCloseKey(hKey);
        }

        // Create ShellFolder subkey
        QString shellFolderKeyPath = clsidPath + "\\ShellFolder";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, shellFolderKeyPath.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            DWORD folderFlags = 0x28;
            RegSetValueExW(hKey, L"FolderValueFlags", 0, REG_DWORD,
                           (BYTE*)&folderFlags, sizeof(DWORD));

            DWORD shellAttributes = 0xf080004d;
            RegSetValueExW(hKey, L"Attributes", 0, REG_DWORD,
                           (BYTE*)&shellAttributes, sizeof(DWORD));
            RegCloseKey(hKey);
        }

        // Create namespace entries
        QString computerNamespace = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\MyComputer\\NameSpace\\{27d3fa3e-06f3-495e-953f-7410df06838e}";
        RegCreateKeyExW(HKEY_CURRENT_USER, computerNamespace.toStdWString().c_str(),
                        0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition);
        RegCloseKey(hKey);

        QString desktopNamespace = "Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\Desktop\\NameSpace\\{27d3fa3e-06f3-495e-953f-7410df06838e}";
        if (RegCreateKeyExW(HKEY_CURRENT_USER, desktopNamespace.toStdWString().c_str(),
                            0, NULL, REG_OPTION_NON_VOLATILE, KEY_WRITE, NULL, &hKey, &disposition) == ERROR_SUCCESS) {
            RegSetValueExW(hKey, L"", 0, REG_SZ,
                           (BYTE*)name.toStdWString().c_str(),
                           (name.length() + 1) * sizeof(wchar_t));
            RegCloseKey(hKey);
        }

        // Notify shell of the change
        SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST | SHCNF_FLUSHNOWAIT, NULL, NULL);
        SHChangeNotify(SHCNE_UPDATEITEM, SHCNF_PATH | SHCNF_FLUSHNOWAIT,
                       path.toStdWString().c_str(), NULL);

        qDebug() << "Successfully added folder to navigation pane with custom icon";
    } else {
        qDebug() << "Failed to create registry entries";
    }
#else
    qDebug() << "Pinning to the navigation pane is only supported on Windows.";
#endif
}
