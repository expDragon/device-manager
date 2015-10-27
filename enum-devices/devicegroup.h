#ifndef DEVICEGROUP_H
#define DEVICEGROUP_H

#include <Windows.h>
#include <SetupAPI.h>
#include <vector>
using namespace std;

#include "device.h"

#pragma comment(lib, "setupapi.lib")

class DeviceGroup
{
private:
    char groupName[LINE_LEN];
    vector<Device*> *devices;

public:
	DeviceGroup();
    DeviceGroup(const char* groupName);
    DeviceGroup(const wchar_t* groupName);
	~DeviceGroup();

    const char* getGroupName();
    void setGroupName(const char* groupName);
    vector<Device*> *getDevices();
    void setDevices(vector<Device*> *devices);

	void addDevice(Device *device);
};

#endif
