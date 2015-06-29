#include "stdafx.h"
#include "Device.h"

FoneOSString Device::GetSerialNumber()
{
	return STR("12345678901");
}

FoneOSString Device::GetCPUSerialNumber()
{
	return STR("ABCD1234EFGH5678");
}

FoneOSString getRegKey(const FoneOSString location, const FoneOSString name)
{
	HKEY key;
	FoneOSChar value[1024];
	DWORD origBufLen = 1024 * sizeof(FoneOSChar);
	DWORD bufLen = origBufLen;
	long ret;
	ret = RegOpenKeyEx(HKEY_LOCAL_MACHINE, location.c_str(), 0, KEY_QUERY_VALUE, &key);
	if (ret != ERROR_SUCCESS)
	{
		return STR("");
	}
	ret = RegQueryValueEx(key, name.c_str(), 0, 0, (LPBYTE)value, &bufLen);
	RegCloseKey(key);
	if (ret != ERROR_SUCCESS || bufLen > origBufLen)
	{
		return STR("");
	}
	FoneOSString strVal = FoneOSString(value, (size_t)bufLen - 1);
	size_t i = strVal.length();
	/*while (i > 0 && strVal[i - 1] == '\0') {
		--i;
	}*/
	return strVal.substr(0, strVal.find_first_of(STR("\0")));
}

FoneOSString Device::GetCPUName()
{
	FoneOSString regVal = getRegKey(STR("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), STR("ProcessorNameString"));
	if (regVal == STR(""))
	{
		return STR("Unknown CPU");
	}
	return Utils::CondenseString(regVal);
}

FoneOSString Device::GetCPUArchitecture()
{
	LPSYSTEM_INFO systemInfo = new _SYSTEM_INFO();
	GetNativeSystemInfo(systemInfo);
	switch (systemInfo->wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_ALPHA:
		return STR("Alpha");
	case PROCESSOR_ARCHITECTURE_ALPHA64:
		return STR("Alpha 64-bit");
	case PROCESSOR_ARCHITECTURE_AMD64:
		return STR("x64");
	case PROCESSOR_ARCHITECTURE_ARM:
		return STR("ARM");
	case PROCESSOR_ARCHITECTURE_IA64:
		return STR("Itanium-based");
	case PROCESSOR_ARCHITECTURE_INTEL:
		return STR("x86");
	case PROCESSOR_ARCHITECTURE_MIPS:
		return STR("MIPS");
	case PROCESSOR_ARCHITECTURE_PPC:
		return STR("PPC");
	default:
		return STR("Unknown");
	}
}