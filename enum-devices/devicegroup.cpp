#include "devicegroup.h"

DeviceGroup::DeviceGroup()
{
	RtlZeroMemory(groupName, sizeof(char)*LINE_LEN);
    devices = new vector<Device*>;
}

DeviceGroup::DeviceGroup(const char *groupName) : DeviceGroup()
{
    memcpy(this->groupName, groupName, strlen(groupName));
}

DeviceGroup::DeviceGroup(const wchar_t *groupName) : DeviceGroup()
{
    wcstombs(this->groupName, groupName, wcslen(groupName));
}

DeviceGroup::~DeviceGroup()
{
}

const char *DeviceGroup::getGroupName()
{
    return this->groupName;
}

void DeviceGroup::setGroupName(const char *groupName)
{
    memcpy(this->groupName, groupName, strlen(groupName));
}

vector<Device *> *DeviceGroup::getDevices()
{
    return this->devices;
}

void DeviceGroup::setDevices(vector<Device *> *devices)
{
    this->devices->empty();
    for(Device* device : *devices)
    {
        this->devices->push_back(device);
    }
}

void DeviceGroup::addDevice(Device * device)
{
    this->devices->push_back(device);
}
