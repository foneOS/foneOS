#pragma once
class Database
{
public:
	// Check if a database exists. If it doesn't, create it and run the provided provscript SQL file. Returns true on success, false on failure.
	static bool ProvDB(FoneOSString dbPath, FoneOSString provScript);
};

