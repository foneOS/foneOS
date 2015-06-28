#pragma once
class Storage
{
public:
	Storage();

	// Gets the base path for file storage. Ends in a path seperator.
	static FoneOSString GetBasePath();

	// Gets the full path to the specified file.
	static FoneOSString GetFullPath(FoneOSString baseStr);

	// Checks if the specified file exists and is readable.
	static bool FileExists(FoneOSString fileName);

	// Removes the specified file.
	static bool RemoveFile(FoneOSString fileName);

	// Reads the specified file.
	static FoneOSString ReadAllText(FoneOSString fileName);
};

