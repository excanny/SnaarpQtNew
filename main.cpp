#include "mainwindow.h"
#include "CreateAndPinSnaarpFolder.h"  // Include the new class
#include "usbcontroller.h"
#include <QTimer>

#include <QApplication>
#include <QStandardPaths>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Define the Snaarp folder path (e.g., on the Desktop)
    QString folderPath = QStandardPaths::writableLocation(QStandardPaths::DesktopLocation) + "/Snaarp";

    // Create an instance of CreateAndPinSnaarpFolder and call createFolder
    CreateAndPinSnaarpFolder folderManager;
    folderManager.createFolder(folderPath);

    UsbController controller;

    // List current USB devices
    qDebug() << "Current USB devices:";
    QStringList devices = controller.listUsbDevices();
    for (const QString& device : devices) {
        qDebug() << device;
    }

    // Disable USB ports
    if (controller.disableUsbPorts()) {
        qDebug() << "USB ports disabled successfully";
    } else {
        qDebug() << "Failed to disable USB ports";
    }

    // Enable USB ports after 5 seconds
    QTimer::singleShot(5000, &controller, [&controller]() {
        if (controller.enableUsbPorts()) {
            qDebug() << "USB ports enabled successfully";
        } else {
            qDebug() << "Failed to enable USB ports";
        }
        qApp->quit();
    });

    // Show the main window after creating and pinning the folder
    MainWindow w;
    w.show();

    return a.exec();
}
