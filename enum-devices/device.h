#ifndef DEVICE_H
#define DEVICE_H

#include <Windows.h>
#include <SetupAPI.h>
#include <vector>
#include <algorithm>
using namespace std;

#pragma comment(lib, "setupapi.lib")

class Device
{
private:
    GUID guid;
    char instanceID[LINE_LEN];
    char name[MAX_PATH];
    char path[MAX_PATH];

    char driver[MAX_PATH];
    char enumerator[MAX_PATH];
    char hardwareID[MAX_PATH];
    char location_info[MAX_PATH];
    char physicalName[MAX_PATH];

    vector<const char*> capabilities;

public:
	Device();
    Device(const GUID guid, const char* name, const char* instanceID, const char* path);
    Device(const GUID guid, wchar_t *name, wchar_t *installID, wchar_t *path);
	~Device();

    const GUID getGuid();
    void setGuid(const GUID guid);

    const char* getID();
    void setID(const char* instanceID);

    const char* getName();
    void setName(const char* name);

    const char* getPath();
    void setPath(const char* path);

    void setDriver(const wchar_t *value);
    const char *getEnumerator();

    void setEnumerator(const wchar_t *value);
    const char *getHardwareID();

    void setHardwareID(const wchar_t *value);
    const char *getLocation_info();

    void setLocation_info(const wchar_t *value);
    const char *getPhysicalName();

    void setPhysicalName(const wchar_t *value);
    vector<const char *> getCapabilities() const;

    void setCapabilities(const vector<const char *> &value);
    vector<const char *> getDevtypes() const;
    void addCapability(const char* capability);

    const char *getDriver();
};

#endif
