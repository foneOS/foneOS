#include "stdafx.h"
#include "Database.h"

bool Database::ProvDB(FoneOSString dbPath, FoneOSString provScript)
{
	if (Storage::FileExists(dbPath))
	{
		return true;
	}
	sqlite3 * db;
	sqlite3_open(Utils::FoneOSStringToCharArray(Storage::GetFullPath(dbPath)), &db);
	char * errMsg;

	FoneOSString provScriptTxt = Storage::ReadAllText(provScript);
	if (sqlite3_exec(db, Utils::FoneOSStringToCharArray(provScriptTxt), NULL, NULL, &errMsg) != SQLITE_OK)
	{
		Logging::LogMessage(FoneOSString(STR("ProvDB(")) + dbPath + FoneOSString(STR(", ")) + provScript + FoneOSString(STR(") -> failure!")));
		Logging::LogMessage(FoneOSString(STR("SQLite error! Message: ")) + Utils::CharArrayToFoneOSString(errMsg));

		sqlite3_free(errMsg);
		sqlite3_close(db);

		Storage::RemoveFile(dbPath);
		return false;
	}
	sqlite3_close(db);
	Logging::LogMessage(FoneOSString(STR("ProvDB(")) + dbPath + FoneOSString(STR(", ")) + provScript + FoneOSString(STR(") -> success!")));
	return true;
}