#include "deviceenumerator.h"
#include <string>

DeviceEnumerator::DeviceEnumerator()
{
    deviceGroups = new vector<DeviceGroup*>;
}

vector<DeviceGroup *> *DeviceEnumerator::getDeviceGroups() const
{
    return deviceGroups;
}

void DeviceEnumerator::setDeviceGroups(vector<DeviceGroup *> *value)
{
    deviceGroups = value;
}

DeviceGroup* DeviceEnumerator::findDeviceGroup(wchar_t *groupName)
{
    char *cGroupName = new char[LINE_LEN];
    wcstombs(cGroupName, groupName, sizeof(char)*LINE_LEN);
    for (DeviceGroup* currentGroup : *deviceGroups)
    {
        if (!strcmp(currentGroup->getGroupName(), cGroupName))
        {
            return currentGroup;
        }
    }

    return nullptr;
}

DeviceGroup* DeviceEnumerator::createNewDeviceGroup(wchar_t *groupName)
{
    DeviceGroup* newGroup = new DeviceGroup(groupName);
    deviceGroups->push_back(newGroup);

    return newGroup;
}

void DeviceEnumerator::addDeviceToGroup(Device *device, DeviceGroup *deviceGroup)
{
    deviceGroup->addDevice(device);
}

void DeviceEnumerator::enumerateDevices()
{
    HDEVINFO hDevInfo = 0L;

    hDevInfo = SetupDiGetClassDevs(0L, 0L, NULL, DIGCF_PRESENT |
                                   DIGCF_ALLCLASSES | DIGCF_PROFILE);
    if (hDevInfo == (void*)-1)
    {
        return;
    };

    SP_DEVINFO_DATA spDevInfoData = {0};
    spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    short wIndex = 0;

    while (true)
    {
        if (SetupDiEnumDeviceInfo(hDevInfo, wIndex, &spDevInfoData))
        {
            wchar_t szBuf[MAX_PATH] = {0};

            if (!SetupDiGetDeviceRegistryProperty(hDevInfo,
                                                  &spDevInfoData,
                                                  SPDRP_CLASS, //SPDRP_DEVICEDESC,
                                                  0L,
                                                  (PBYTE)szBuf,
                                                  2048,
                                                  0))
            {
                wIndex++;
                continue;
            };

            if (&spDevInfoData.ClassGuid)
            {
                wchar_t szName[LINE_LEN] = {0};
                wchar_t szID[LINE_LEN] = {0};
                wchar_t szPath[MAX_PATH] = {0};
                DWORD dwRequireSize;

                if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid,
                                                (PWSTR)szBuf,
                                                MAX_PATH * sizeof(wchar_t),
                                                &dwRequireSize))
                {
                    wIndex++;
                    continue;
                };

                DeviceGroup *deviceGroup = findDeviceGroup(szBuf);

                if (!deviceGroup)
                {
                    deviceGroup = createNewDeviceGroup(szBuf);
                }

                getDeviceInstanceID(hDevInfo, &spDevInfoData, szID);
                getDeviceInterfaceInfo(hDevInfo, spDevInfoData, szPath);

                if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                                     &spDevInfoData,
                                                     SPDRP_FRIENDLYNAME,
                                                     0L,
                                                     (PBYTE)szName,
                                                     LINE_LEN * sizeof(wchar_t),
                                                     0))
                {
                    Device *device = new Device(spDevInfoData.ClassGuid, szName, szID, szPath);
                    addDeviceToGroup(device, deviceGroup);
                    getMoreInformation(hDevInfo, spDevInfoData, device);
                }
                else if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                                     &spDevInfoData,
                                                     SPDRP_DEVICEDESC,
                                                     0L,
                                                     (PBYTE)szName,
                                                     LINE_LEN * sizeof(wchar_t),
                                                     0))
                {
                    Device *device = new Device(spDevInfoData.ClassGuid, szName, szID, szPath);
                    addDeviceToGroup(device, deviceGroup);
                    getMoreInformation(hDevInfo, spDevInfoData, device);
                };
            };
        }
        else
            break;

        wIndex++;
    };
    SetupDiDestroyDeviceInfoList(hDevInfo);
}

void DeviceEnumerator::getDeviceInstanceID(HDEVINFO hDevInfo,
                                           SP_DEVINFO_DATA* pspDevInfoData,
                                           wchar_t *szInstanceID)
{
    SetupDiGetDeviceInstanceId(hDevInfo, pspDevInfoData, (PWSTR)szInstanceID,
        LINE_LEN, 0);
}

void DeviceEnumerator::getDeviceInterfaceInfo(HDEVINFO hDevInfo,
                                              SP_DEVINFO_DATA spDevInfoData,
                                              wchar_t *szPath)
{
    SP_DEVICE_INTERFACE_DATA spDevInterfaceData = {0};

    spDevInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    if (!SetupDiCreateDeviceInterface(hDevInfo,
                                      &spDevInfoData,
                                      &spDevInfoData.ClassGuid,
                                      0L,
                                      0L,
                                      &spDevInterfaceData))
        cout << "err" << endl;
    else
    {
        SP_DEVICE_INTERFACE_DETAIL_DATA *pspDevInterfaceDetail = 0L;
        DWORD                           dwRequire              = 0L;

        if (!SetupDiGetDeviceInterfaceDetail(hDevInfo,
                                             &spDevInterfaceData,
                                             0L,
                                             0,
                                             &dwRequire,
                                             0L))
        {
            DWORD dwError = GetLastError();

            if (dwError != ERROR_INSUFFICIENT_BUFFER)
            {
                return;
            };
        };

        pspDevInterfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*)LocalAlloc(LPTR,
                                    sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA)*dwRequire);
        pspDevInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        if (SetupDiGetDeviceInterfaceDetail(hDevInfo,
                                             &spDevInterfaceData,
                                             pspDevInterfaceDetail,
                                             dwRequire,
                                             &dwRequire,
                                             0L))
        {
            memcpy(szPath, pspDevInterfaceDetail->DevicePath,
                   sizeof(wchar_t) * wcslen(((pspDevInterfaceDetail->DevicePath))));
        };

        if (pspDevInterfaceDetail)
        {
            LocalFree(pspDevInterfaceDetail);
        };
    };
}

int DeviceEnumerator::updateDevice(wchar_t szInstanceID[], DWORD dwControlCode)
{
    int retcode = FALSE;
    HDEVINFO devs = SetupDiCreateDeviceInfoListEx(NULL, NULL, NULL, NULL);
    if(devs == INVALID_HANDLE_VALUE)
    {
        return retcode;
    }

    SetupDiOpenDeviceInfo(devs, (PCWSTR)szInstanceID, NULL, 0, NULL);

    SP_DEVINFO_LIST_DETAIL_DATA devInfoListDetail;
    devInfoListDetail.cbSize = sizeof(devInfoListDetail);

    if(!SetupDiGetDeviceInfoListDetail(devs, &devInfoListDetail))
    {
        if(devs != INVALID_HANDLE_VALUE) SetupDiDestroyDeviceInfoList(devs);

        return retcode;
    }

    SP_DEVINFO_DATA devInfo;
    devInfo.cbSize = sizeof(devInfo);
    SetupDiEnumDeviceInfo(devs, 0, &devInfo);

    SP_PROPCHANGE_PARAMS pcp;

    if(dwControlCode == DICS_ENABLE)
    {
        pcp.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
        pcp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
        pcp.StateChange = dwControlCode;
        pcp.Scope = DICS_FLAG_GLOBAL;
        pcp.HwProfile = 0;
        if(SetupDiSetClassInstallParams(devs, &devInfo, &pcp.ClassInstallHeader, sizeof(pcp))){
            SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devs, &devInfo);
        }
    }

    pcp.ClassInstallHeader.cbSize = sizeof(SP_CLASSINSTALL_HEADER);
    pcp.ClassInstallHeader.InstallFunction = DIF_PROPERTYCHANGE;
    pcp.StateChange = dwControlCode;
    pcp.Scope = DICS_FLAG_CONFIGSPECIFIC;
    pcp.HwProfile = 0;
    if(!SetupDiSetClassInstallParams(devs, &devInfo, &pcp.ClassInstallHeader, sizeof(pcp)) ||
        !SetupDiCallClassInstaller(DIF_PROPERTYCHANGE, devs, &devInfo))
    {
        retcode = FALSE;
    }
    else
    {
        SP_DEVINSTALL_PARAMS devParams;
        devParams.cbSize = sizeof(devParams);
        if(SetupDiGetDeviceInstallParams(devs, &devInfo, &devParams) && (devParams.Flags & (DI_NEEDRESTART|DI_NEEDREBOOT)))
        {
            retcode = FALSE;
        }
        else
        {
            retcode = TRUE;
        }
    }

    if(devs != INVALID_HANDLE_VALUE) SetupDiDestroyDeviceInfoList(devs);
    return retcode;
}

void DeviceEnumerator::showDevicesInfo()
{
    for (DeviceGroup* currentGroup : *deviceGroups)
    {
        cout << currentGroup->getGroupName() << endl;
        for (Device* device : *(currentGroup->getDevices()))
        {
           cout << "\tName: " << device->getName() << endl;
           cout << "\t\tGUID: " << ConvertGUIDToString(device->getGuid()) << endl;
           cout << "\t\tInstallID: " << device->getID() << endl;
           cout << "\t\tPath: " << device->getPath() << endl;
        }
    }
}

void DeviceEnumerator::getMoreInformation(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData, Device *device)
{
    wchar_t szName[64] = {0};
    DWORD dwAddr;

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_BUSNUMBER,
                                         0L,
                                         (PBYTE)&dwAddr,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        swprintf(szName, L"%X", dwAddr);
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_CAPABILITIES,
                                         0L,
                                         (PBYTE)&dwAddr,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        if (dwAddr)
        {

            if (dwAddr & CM_DEVCAP_LOCKSUPPORTED)
                device->addCapability("LockSupported");
            if (dwAddr & CM_DEVCAP_EJECTSUPPORTED)
                device->addCapability("EjectSupported");
            if (dwAddr & CM_DEVCAP_REMOVABLE)
                device->addCapability("Removable");
            if (dwAddr & CM_DEVCAP_DOCKDEVICE)
                device->addCapability("DockDevice");
            if (dwAddr & CM_DEVCAP_UNIQUEID)
                device->addCapability("UniqueID");
            if (dwAddr & CM_DEVCAP_SILENTINSTALL)
                device->addCapability("SilentInstall");
            if (dwAddr & CM_DEVCAP_RAWDEVICEOK)
                device->addCapability("RawDeviceOK");
            if (dwAddr & CM_DEVCAP_SURPRISEREMOVALOK)
                device->addCapability("SurpriseRemovalOK");
            if (dwAddr & CM_DEVCAP_HARDWAREDISABLED)
                device->addCapability("HardwareDisabled");
            if (dwAddr & CM_DEVCAP_NONDYNAMIC)
                device->addCapability("NonDynamic");
        };
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_DRIVER,
                                         0L,
                                         (PBYTE)szName,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        device->setDriver(szName);
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_ENUMERATOR_NAME,
                                         0L,
                                         (PBYTE)szName,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        device->setEnumerator(szName);
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_HARDWAREID,
                                         0L,
                                         (PBYTE)szName,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        device->setHardwareID(szName);
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_LOCATION_INFORMATION,
                                         0L,
                                         (PBYTE)szName,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        device->setLocation_info(szName);
    };

    if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                         &spDevInfoData,
                                         SPDRP_PHYSICAL_DEVICE_OBJECT_NAME,
                                         0L,
                                         (PBYTE)szName,
                                         63 * sizeof(wchar_t),
                                         0))
    {
        device->setPhysicalName(szName);
    };
}

