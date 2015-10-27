#ifndef DEVICEENUMERATOR_H
#define DEVICEENUMERATOR_H

#include <iostream>
#include <algorithm>
#include <Windows.h>
#include <SetupAPI.h>
#include "devicegroup.h"
#include "Cfgmgr32.h"
#include "util.h"

using namespace std;

#pragma comment(lib, "setupapi.lib")
#pragma comment(lib, "Cfgmgr32.lib")

class DeviceEnumerator
{
private:
    vector<DeviceGroup*> *deviceGroups;

public:
    DeviceEnumerator();
    vector<DeviceGroup *> *getDeviceGroups() const;
    void setDeviceGroups(vector<DeviceGroup *> *value);

    DeviceGroup* findDeviceGroup(wchar_t *groupName);
    DeviceGroup* createNewDeviceGroup(wchar_t *groupName);
    void addDeviceToGroup(Device *device, DeviceGroup *deviceGroup);

    void enumerateDevices();

    void getDeviceInstanceID(HDEVINFO hDevInfo,
        SP_DEVINFO_DATA* pspDevInfoData,
        wchar_t *szInstanceID);
    void getDeviceInterfaceInfo(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData,
        wchar_t *szPath);
    void getMoreInformation(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData, Device *device);

    void showDevicesInfo();

    int updateDevice(wchar_t szInstanceID[], DWORD dwControlCode);

};

#endif // DEVICESENUMERATOR_H
