#include "util.h"

#include <strsafe.h>


const char* ConvertGUIDToString(const GUID guid)
{
    char  szData[30] = {0};
    char  szTmp[3]   = {0};
    short wLoop;

    char pData[MAX_PATH];

    RtlZeroMemory(pData, sizeof(char)*MAX_PATH);
//
    sprintf(pData, "%04X-%02X-%02X-", guid.Data1,
                   guid.Data2, guid.Data3);
    for (wLoop = 0; wLoop < 8; wLoop++)
    {
         if (wLoop == 2)
             strcat(szData, "-");
         sprintf(szTmp, "%02X", guid.Data4[wLoop]);
         strcat(szData, szTmp);
    };
//         szTmp[wLoop] = guid.Data4[wLoop];
    memcpy(pData+strlen(pData), szData, strlen(szData));

    return pData;
}
