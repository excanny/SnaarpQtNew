#ifndef CREATEANDPINSNAARPFOLDER_H
#define CREATEANDPINSNAARPFOLDER_H

#include <QString>

class CreateAndPinSnaarpFolder {
public:
    CreateAndPinSnaarpFolder();
    void createFolder(const QString &path);
    void pinFolderToNavigationPane(const QString &path);
};

#endif // CREATEANDPINSNAARPFOLDER_H
