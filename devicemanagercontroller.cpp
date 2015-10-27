#include "devicemanagercontroller.h"

DeviceManagerController::DeviceManagerController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow;
    devicesEnumerator = new DeviceEnumerator;

    connect(mainWindow, &MainWindow::deviceSelected, this, &DeviceManagerController::renderDeviceInfo);
    connect(mainWindow, &MainWindow::disableDeviceButtonPressed, this, &DeviceManagerController::disableDevice);
    connect(mainWindow, &MainWindow::enableDeviceButtonPressed, this, &DeviceManagerController::enableDevice);
}

void DeviceManagerController::launch()
{
    devicesEnumerator->enumerateDevices();
    renderDevices();
    mainWindow->show();
}

void DeviceManagerController::renderDevices()
{
    QStringList deviceNames;

    for (DeviceGroup *deviceGroup : *(devicesEnumerator->getDeviceGroups()))
    {
        deviceNames.clear();
        for(Device *device : *(deviceGroup->getDevices()))
        {
            deviceNames << QString::fromStdString(device->getName());
        }

        mainWindow->addDeviceGroup(deviceGroup->getGroupName(), deviceNames);
    }
}

void DeviceManagerController::renderDeviceInfo(int deviceGroupIndex, int deviceIndex)
{
    QStringList deviceInfoList;

    selectedDevice = devicesEnumerator->getDeviceGroups()
            ->at(deviceGroupIndex)->getDevices()->at(deviceIndex);

    char *guid = new char[256];
    strcpy(guid, ConvertGUIDToString(selectedDevice->getGuid()));

    deviceInfoList << guid;
    deviceInfoList << selectedDevice->getID();
    deviceInfoList << selectedDevice->getPath();
    deviceInfoList << selectedDevice->getDriver();
    deviceInfoList << selectedDevice->getEnumerator();
    deviceInfoList << selectedDevice->getHardwareID();
    deviceInfoList << selectedDevice->getLocation_info();
    deviceInfoList << selectedDevice->getPhysicalName();

    QString capabilities;

    for(const char* capability : selectedDevice->getCapabilities())
    {
        capabilities.append(capability);
        capabilities.append("; ");
    }

    deviceInfoList << capabilities;

    mainWindow->setDeviceInfo(deviceInfoList);
}

void DeviceManagerController::disableDevice()
{
    wchar_t id[MAX_PATH] = {0};
    mbstowcs(id, selectedDevice->getID(), strlen(selectedDevice->getID()));

    devicesEnumerator->updateDevice(id, DICS_DISABLE);
}

void DeviceManagerController::enableDevice()
{
    wchar_t id[MAX_PATH] = {0};
    mbstowcs(id, selectedDevice->getID(), strlen(selectedDevice->getID()));

    devicesEnumerator->updateDevice(id, DICS_ENABLE);
}
