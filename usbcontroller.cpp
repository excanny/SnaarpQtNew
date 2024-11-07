#include "usbcontroller.h"
#include <QDebug>
#include <Setupapi.h>
#include <Usbiodef.h>
#include <cfgmgr32.h>
#include <initguid.h>
#include <devguid.h>

// USB Hub Class GUID
DEFINE_GUID(GUID_USB_HUB, 0xf18a0e88, 0xc30c, 0x11d0, 0x88, 0x15, 0x00, 0xa0, 0xc9, 0x06, 0xbe, 0xd8);

UsbController::UsbController(QObject *parent)
    : QObject(parent)
{
    deviceInfoSet = INVALID_HANDLE_VALUE;
}

UsbController::~UsbController()
{
    if (deviceInfoSet != INVALID_HANDLE_VALUE) {
        SetupDiDestroyDeviceInfoList(deviceInfoSet);
    }
}

QString UsbController::getLastErrorString()
{
    DWORD error = GetLastError();
    if (error == 0) {
        CONFIGRET cmError = CM_Get_Global_State(nullptr, 0);
        if (cmError != CR_SUCCESS) {
            wchar_t cmErrorText[256] = {0};
            CM_Get_Device_ID_List_Size(nullptr, nullptr, 0);
            return QString("CM Error %1").arg(cmError);
        }
        return "No error";
    }

    LPWSTR messageBuffer = nullptr;
    size_t size = FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        error,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&messageBuffer,
        0,
        nullptr
        );

    QString message = QString::fromWCharArray(messageBuffer, size);
    LocalFree(messageBuffer);
    return QString("Error %1: %2").arg(error).arg(message);
}

void UsbController::logError(const QString& operation)
{
    qDebug() << "Failed to" << operation << "-" << getLastErrorString();
}

bool UsbController::isRunningAsAdmin()
{
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;

    if (!AllocateAndInitializeSid(&ntAuthority, 2,
                                  SECURITY_BUILTIN_DOMAIN_RID,
                                  DOMAIN_ALIAS_RID_ADMINS,
                                  0, 0, 0, 0, 0, 0, &adminGroup)) {
        return false;
    }

    CheckTokenMembership(NULL, adminGroup, &isAdmin);
    FreeSid(adminGroup);

    return isAdmin == TRUE;
}

bool UsbController::changeDeviceState(const GUID& classGuid, bool enable)
{
    if (!isRunningAsAdmin()) {
        qDebug() << "Error: Administrative privileges required";
        return false;
    }

    deviceInfoSet = SetupDiGetClassDevs(
        &classGuid,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_PROFILE
        );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        logError("get device information set");
        return false;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;
    bool anySuccess = false;

    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        WCHAR deviceDesc[256] = {0};
        if (SetupDiGetDeviceRegistryProperty(
                deviceInfoSet,
                &deviceInfoData,
                SPDRP_DEVICEDESC,
                nullptr,
                (PBYTE)deviceDesc,
                sizeof(deviceDesc),
                nullptr)) {
            qDebug() << "Processing device:" << QString::fromWCharArray(deviceDesc);
        }

        SP_PROPCHANGE_PARAMS params;
        params.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
        params.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
        params.StateChange = enable ? DICS_ENABLE : DICS_DISABLE;
        params.Scope = DICS_FLAG_GLOBAL;
        params.HwProfile = 0;

        if (SetupDiSetClassInstallParams(deviceInfoSet, &deviceInfoData,
                                         (PSP_CLASSINSTALL_HEADER)&params,
                                         sizeof(params)) &&
            SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, deviceInfoSet, &deviceInfoData)) {
            // Verify change
            ULONG status = 0, problem = 0;
            if (CM_Get_DevNode_Status(&status, &problem, deviceInfoData.DevInst, 0) == CR_SUCCESS) {
                bool isCurrentlyEnabled = !(status & DN_DISABLEABLE) || (status & DN_STARTED);
                if (isCurrentlyEnabled == enable) {
                    anySuccess = true;
                    qDebug() << "Successfully" << (enable ? "enabled" : "disabled")
                             << "device:" << QString::fromWCharArray(deviceDesc);
                    CM_Reenumerate_DevNode(deviceInfoData.DevInst, 0);  // Added re-enumeration
                    break;
                }
            }
        } else {
            logError(QString("change state for device %1").arg(QString::fromWCharArray(deviceDesc)));
        }

        deviceIndex++;
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    deviceInfoSet = INVALID_HANDLE_VALUE;

    return anySuccess;
}

bool UsbController::enableUsbPorts()
{
    qDebug() << "Attempting to enable USB ports...";

    // Enable USB root hubs first
    bool rootSuccess = changeDeviceState(GUID_DEVCLASS_USB, true);

    // Wait for root hubs to initialize
    Sleep(2000);

    // Then enable USB hub controllers
    bool hubSuccess = changeDeviceState(GUID_USB_HUB, true);

    return rootSuccess || hubSuccess;
}

bool UsbController::disableUsbPorts()
{
    qDebug() << "Attempting to disable USB ports....";

    // Disable USB hub controllers first
    bool hubSuccess = changeDeviceState(GUID_USB_HUB, false);

    // Then disable USB root hubs
    bool rootSuccess = changeDeviceState(GUID_DEVCLASS_USB, false);

    return hubSuccess || rootSuccess;
}

QStringList UsbController::listUsbDevices()
{
    QStringList devices;
    deviceInfoSet = SetupDiGetClassDevs(
        &GUID_DEVCLASS_USB,
        nullptr,
        nullptr,
        DIGCF_PRESENT | DIGCF_PROFILE
        );

    if (deviceInfoSet == INVALID_HANDLE_VALUE) {
        logError("get USB device list");
        return devices;
    }

    SP_DEVINFO_DATA deviceInfoData;
    deviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    DWORD deviceIndex = 0;

    while (SetupDiEnumDeviceInfo(deviceInfoSet, deviceIndex, &deviceInfoData)) {
        WCHAR deviceDesc[256] = {0};
        WCHAR deviceID[256] = {0};
        WCHAR deviceStatus[256] = {0};

        // Get device description
        if (SetupDiGetDeviceRegistryProperty(
                deviceInfoSet,
                &deviceInfoData,
                SPDRP_DEVICEDESC,
                nullptr,
                (PBYTE)deviceDesc,
                sizeof(deviceDesc),
                nullptr))
        {
            QString deviceInfo = QString::fromWCharArray(deviceDesc);

            // Get device ID
            if (CM_Get_Device_ID(deviceInfoData.DevInst, deviceID, sizeof(deviceID)/sizeof(WCHAR), 0) == CR_SUCCESS) {
                deviceInfo += QString(" (ID: %1)").arg(QString::fromWCharArray(deviceID));
            }

            // Get device status
            ULONG status = 0, problem = 0;
            if (CM_Get_DevNode_Status(&status, &problem, deviceInfoData.DevInst, 0) == CR_SUCCESS) {
                bool isEnabled = !(status & DN_DISABLEABLE) || (status & DN_STARTED);
                deviceInfo += QString(" [%1]").arg(isEnabled ? "Enabled" : "Disabled");
            }

            devices.append(deviceInfo);
        }
        deviceIndex++;
    }

    if (GetLastError() != NO_ERROR && GetLastError() != ERROR_NO_MORE_ITEMS) {
        logError("enumerate devices");
    }

    SetupDiDestroyDeviceInfoList(deviceInfoSet);
    deviceInfoSet = INVALID_HANDLE_VALUE;

    return devices;
}

