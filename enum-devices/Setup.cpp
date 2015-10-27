#include "Setup.h"

DeviceGroup* findDeviceGroup(wchar_t *groupName)
{
    char *cGroupName = new char[LINE_LEN];
    wcstombs(cGroupName, groupName, sizeof(char)*LINE_LEN);
    for (DeviceGroup* currentGroup : deviceGroups)
	{
        if (!strcmp(currentGroup->getGroupName(), cGroupName))
		{
			return currentGroup;
		}
	}

	return nullptr;
}

DeviceGroup* createNewDeviceGroup(wchar_t *groupName)
{
    DeviceGroup* newGroup = new DeviceGroup(groupName);
    deviceGroups.push_back(newGroup);

	return newGroup;
}

void addDeviceToGroup(Device *device, DeviceGroup *deviceGroup)
{
    deviceGroup->addDevice(device);
}

char EnumerateDevices(vector<DeviceGroup*> *deviceGroups)
{
    HDEVINFO hDevInfo = 0L;

    hDevInfo = SetupDiGetClassDevs(0L, 0L, NULL, DIGCF_PRESENT |
                                   DIGCF_ALLCLASSES | DIGCF_PROFILE);
    if (hDevInfo == (void*)-1)
    {
        return 0;
    };

    SP_DEVINFO_DATA spDevInfoData = {0};
    spDevInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    short wIndex = 0;

    while (true)
    {
        if (SetupDiEnumDeviceInfo(hDevInfo,
                                  wIndex,
                                  &spDevInfoData))
        {
            wchar_t  szBuf[MAX_PATH] = {0};

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
                wchar_t szName[64] = {0};
                wchar_t szID[LINE_LEN] = {0};
                wchar_t szPath[MAX_PATH] = {0};
                DWORD dwRequireSize;
                if (!SetupDiGetClassDescription(&spDevInfoData.ClassGuid,
                                                (PWSTR)szBuf,
                                                MAX_PATH,
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

                GetDeviceInstanceID(hDevInfo, &spDevInfoData, (char*)szID);
                GetDeviceInterfaceInfo(hDevInfo, spDevInfoData, (char*)szPath);

                if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                                     &spDevInfoData,
                                                     SPDRP_FRIENDLYNAME,
                                                     0L,
                                                     (PBYTE)szName,
                                                     63,
                                                     0))
                {
                    Device *device = new Device(spDevInfoData.ClassGuid, szName, szID, szPath);
					addDeviceToGroup(device, deviceGroup);    
                }
                else if (SetupDiGetDeviceRegistryProperty(hDevInfo,
                                                     &spDevInfoData,
                                                     SPDRP_DEVICEDESC,
                                                     0L,
                                                     (PBYTE)szName,
                                                     63,
                                                     0))
				{
					Device *device = new Device(spDevInfoData.ClassGuid, szName, szID, szPath);
					addDeviceToGroup(device, deviceGroup);
                };
                //GetOtherInfo(spDevInfoData.ClassGuid, wOrder);
            };
            //SetupDiDestroyDriverInfoList(hDevInfo, &spDevInfoData, SPDIT_COMPATDRIVER);
        }
        else
            break;

        wIndex++;
    };
    SetupDiDestroyDeviceInfoList(hDevInfo);

    for (DeviceGroup* currentGroup : deviceGroups)
    {
        cout << currentGroup->getGroupName() << endl;
        for (Device* device : *(currentGroup->getDevices()))
        {
           cout << "\t" << device->getSzName() << endl;
        }
    }

    return 1;
}

void GetDeviceInstanceID(HDEVINFO hDevInfo,
    SP_DEVINFO_DATA* pspDevInfoData,
    char *szInstanceID)
{
    if (!SetupDiGetDeviceInstanceId(hDevInfo,
        pspDevInfoData,
        (PWSTR)szInstanceID,
        LINE_LEN,
        0))
        //ShowErrorMsg(_hDlg, GetLastError(), "SetupDiBuildDriverInfoList")
        ;
}

void GetDeviceInterfaceInfo(HDEVINFO hDevInfo, SP_DEVINFO_DATA spDevInfoData,
	char *szPath)
{
    SP_DEVICE_INTERFACE_DATA spDevInterfaceData = { 0 };
    //
    spDevInterfaceData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
    if (!SetupDiCreateDeviceInterface(hDevInfo,
        &spDevInfoData,
        &spDevInfoData.ClassGuid,
        0L,
        0L,
        &spDevInterfaceData))
        //ShowErrorMsg(_hDlg, GetLastError(), "SetupDiBuildDriverInfoList")
        ;
    else
    {
        SP_DEVICE_INTERFACE_DETAIL_DATA *pspDevInterfaceDetail = 0L;
        DWORD                           dwRequire = 0L;

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
                //ShowErrorMsg(_hDlg, dwError, "SetupDiBuildDriverInfoList")
                ;
                return;
            };
        };

        pspDevInterfaceDetail = (SP_DEVICE_INTERFACE_DETAIL_DATA*)LocalAlloc(LPTR,
            sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA)*dwRequire);
        pspDevInterfaceDetail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
        if (!SetupDiGetDeviceInterfaceDetail(hDevInfo,
            &spDevInterfaceData,
            pspDevInterfaceDetail,
            dwRequire,
            &dwRequire,
            0L))
        {
            DWORD dwError = GetLastError();

            if (dwError != ERROR_INSUFFICIENT_BUFFER)
                //ShowErrorMsg(_hDlg, dwError, "SetupDiBuildDriverInfoList")
                ;
        }
        else
        {
            memcpy(szPath, pspDevInterfaceDetail->DevicePath,
                strlen((const char*)pspDevInterfaceDetail->DevicePath));
            //            switch(spDevInterfaceData.
        };

        if (pspDevInterfaceDetail)
            LocalFree(pspDevInterfaceDetail);
    };
};
