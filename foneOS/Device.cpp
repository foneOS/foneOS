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

#ifdef WINDOWS
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
#endif

FoneOSString Device::GetCPUName()
{
    FoneOSString cpuVal = STR("Unknown CPU");
#ifdef WINDOWS
    cpuVal = getRegKey(STR("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"), STR("ProcessorNameString"));
#elif defined(EDISON)
    cpuVal = "Intel Edison";
#endif
    if (cpuVal == STR(""))
	{
		return STR("Unknown CPU");
	}
    return Utils::CondenseString(cpuVal);
}

FoneOSString Device::GetCPUArchitecture()
{
#ifdef WINDOWS
	LPSYSTEM_INFO systemInfo = new _SYSTEM_INFO();
	GetNativeSystemInfo(systemInfo);
	switch (systemInfo->wProcessorArchitecture)
	{
	case PROCESSOR_ARCHITECTURE_ALPHA:
		delete systemInfo;
		return STR("Alpha");
	case PROCESSOR_ARCHITECTURE_ALPHA64:
		delete systemInfo;
		return STR("Alpha 64-bit");
	case PROCESSOR_ARCHITECTURE_AMD64:
		delete systemInfo;
		return STR("x64");
	case PROCESSOR_ARCHITECTURE_ARM:
		delete systemInfo;
		return STR("ARM");
	case PROCESSOR_ARCHITECTURE_IA64:
		delete systemInfo;
		return STR("Itanium-based");
	case PROCESSOR_ARCHITECTURE_INTEL:
		delete systemInfo;
		return STR("x86");
	case PROCESSOR_ARCHITECTURE_MIPS:
		delete systemInfo;
		return STR("MIPS");
	case PROCESSOR_ARCHITECTURE_PPC:
		delete systemInfo;
		return STR("PPC");
	default:
		delete systemInfo;
		return STR("Unknown");
	}
#elif defined(EDISON)
	return STR("Pentium-compatible");
#else
	return STR("Unknown architecture");
#endif
}
