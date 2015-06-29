#include "stdafx.h"
#include "Storage.h"


Storage::Storage()
{
}


FoneOSString Storage::GetBasePath()
{
	char * prefPath = SDL_GetPrefPath("Flippers", "foneOS");
	FoneOSString prefPathStr;

	prefPathStr = Utils::CharArrayToFoneOSString(prefPath);

	return prefPathStr;
}


FoneOSString Storage::GetFullPath(FoneOSString baseStr)
{
	return Storage::GetBasePath() + baseStr;
}


bool Storage::FileExists(FoneOSString fileName)
{
	std::ifstream test(Storage::GetFullPath(fileName));
	bool resp = test.good();
	test.close();
	return resp;
}


bool Storage::RemoveFile(FoneOSString fileName)
{
	// "Why isn't this called DeleteFile?"
	// "Because then Windows' macros would change it to DeleteFileW."
	if (!Storage::FileExists(fileName))
	{
		return false;
	}
	return remove(Utils::FoneOSStringToCharArray(Storage::GetFullPath(fileName))) == 0;
}


FoneOSString Storage::ReadAllText(FoneOSString fileName)
{
	FoneOSIFileStream file;
	file.open(Storage::GetFullPath(fileName));
	if (file.good())
	{
		FoneOSString retVal;
		FoneOSString chunk;
		bool firstTime = true;
		while (!file.eof())
		{
			if (firstTime)
			{
				firstTime = false;
			}
			else
			{
				retVal += STR(" ");
			}
			file >> chunk;
			retVal += chunk;
		}

		file.close();

		return retVal;
	}
	else
	{
		return STR("");
	}
}