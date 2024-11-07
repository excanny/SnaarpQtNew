// usbcontroller.h
#ifndef USBCONTROLLER_H
#define USBCONTROLLER_H

#include <QObject>
#include <Windows.h>
#include <SetupAPI.h>
#include <QString>
#include <QStringList>

class UsbController : public QObject {
    Q_OBJECT

public:
    explicit UsbController(QObject *parent = nullptr);
    ~UsbController();
    bool disableUsbPorts();
    bool enableUsbPorts();
    QStringList listUsbDevices();

private:
    HDEVINFO deviceInfoSet;
    bool changeDeviceState(const GUID& classGuid, bool enable);
    QString getLastErrorString();
    void logError(const QString& operation);
     bool isRunningAsAdmin();  // Added declaration
};

#endif // USBCONTROLLER_H
