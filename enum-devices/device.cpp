#include "device.h"

Device::Device()
{
    RtlZeroMemory(&guid, sizeof(GUID));
    RtlZeroMemory(instanceID, sizeof(char)*LINE_LEN);
    RtlZeroMemory(name, sizeof(char)*MAX_PATH);
    RtlZeroMemory(path, sizeof(char)*MAX_PATH);

    RtlZeroMemory(driver, sizeof(char)*MAX_PATH);
    RtlZeroMemory(enumerator, sizeof(char)*MAX_PATH);
    RtlZeroMemory(hardwareID, sizeof(char)*MAX_PATH);
    RtlZeroMemory(location_info, sizeof(char)*MAX_PATH);
    RtlZeroMemory(physicalName, sizeof(char)*MAX_PATH);
}

Device::Device(const GUID guid,
    const char* name,
    const char* instanceID,
    const char* path) : Device()
{
	memcpy(&this->guid, &guid, sizeof(GUID));
    memcpy(this->instanceID, instanceID, strlen(instanceID));
    memcpy(this->name, name, strlen(name));
    memcpy(this->path, path, strlen(path));
}

Device::Device(const GUID guid,
    wchar_t* name,
    wchar_t* installID,
    wchar_t* path) : Device()
{
    memcpy(&this->guid, &guid, sizeof(GUID));
    wcstombs(this->instanceID, installID, wcslen(installID));
    wcstombs(this->name, name, wcslen(name));
    wcstombs(this->path, path, wcslen(path));
}

Device::~Device()
{
}

const GUID Device::getGuid()
{
    return this->guid;
}

void Device::setGuid(const GUID guid)
{
    memcpy(&this->guid, &guid, sizeof(GUID));
}

const char *Device::getID()
{
    return this->instanceID;
}

void Device::setID(const char *szInstallID)
{
    memcpy(this->instanceID, szInstallID, strlen(szInstallID));
}

const char *Device::getName()
{
    return this->name;
}

void Device::setName(const char *szName)
{
    memcpy(this->name, szName, strlen(szName));
}

const char *Device::getPath()
{
    return this->path;
}

void Device::setPath(const char *szPath)
{
    memcpy(this->path, szPath, strlen(szPath));
}

const char *Device::getDriver()
{
    return driver;
}

void Device::setDriver(const wchar_t *value)
{
    wcstombs(this->driver, value, wcslen(value));
}

const char *Device::getEnumerator()
{
    return enumerator;
}

void Device::setEnumerator(const wchar_t *value)
{
    wcstombs(this->enumerator, value, wcslen(value));
}

const char *Device::getHardwareID()
{
    return hardwareID;
}

void Device::setHardwareID(const wchar_t *value)
{
    wcstombs(this->hardwareID, value, wcslen(value));
}

const char *Device::getLocation_info()
{
    return location_info;
}

void Device::setLocation_info(const wchar_t *value)
{
    wcstombs(this->location_info, value, wcslen(value));
}

const char *Device::getPhysicalName()
{
    return physicalName;
}

void Device::setPhysicalName(const wchar_t *value)
{
    wcstombs(this->physicalName, value, wcslen(value));
}

vector<const char *> Device::getCapabilities() const
{
    return capabilities;
}

void Device::setCapabilities(const vector<const char *> &value)
{
    capabilities = value;
}

void Device::addCapability(const char *capability)
{
    capabilities.push_back(capability);
}
