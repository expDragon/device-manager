#ifndef SETUP_H
#define SETUP_H

#include <Windows.h>
#include <setupapi.h>
#include "Util.h"

#include <iostream>
#include <algorithm>
using namespace std;

#pragma comment(lib, "setupapi.lib")

#include "DeviceGroup.h"

DeviceGroup* findDeviceGroup(wchar_t *groupName);
DeviceGroup* createNewDeviceGroup(wchar_t *groupName);
void addDeviceToGroup(Device *device, DeviceGroup *deviceGroup);
char EnumerateDevices(vector<DeviceGroup *> *deviceGroups);
void GetDeviceInstanceID(HDEVINFO hDevInfo,
    SP_DEVINFO_DATA* pspDevInfoData,
    char *szInstanceID);
void GetDeviceInterfaceInfo(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData,
    char *szPath);

#endif
