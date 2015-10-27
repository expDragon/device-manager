#ifndef DEVICEMANAGERCONTROLLER_H
#define DEVICEMANAGERCONTROLLER_H

#include <QObject>
#include <enum-devices/device.h>

#include "mainwindow.h"
#include "enum-devices/deviceenumerator.h"

class DeviceManagerController : public QObject
{
    Q_OBJECT

private:
    MainWindow *mainWindow;
    DeviceEnumerator *devicesEnumerator;

    Device *selectedDevice;

public:
    explicit DeviceManagerController(QObject *parent = 0);
    void launch();

    void disableDevice();
    void renderDeviceInfo(int deviceGroupIndex, int deviceIndex);
    void enableDevice();
    void renderDevices();
public slots:

};

#endif // DEVICEMANAGERCONTROLLER_H
