#pragma once
#include "DataTable.h"
#include <map>
#include <vector>

using namespace std;


enum class Languages
{
	KOR,
	ENG,
	JPN,
	COUNT,
};

class StringTable : public DataTable
{
private:
	//map<Languages, map<string, string>*> tables;

	map<string, string> table;	// 
	Languages currentLang;

public:
	StringTable();
	virtual ~StringTable();

	void SetLanguage(Languages lang);

	const string& Get(const string& id);
	//const string& Get(Languages lang, const string& id);

	virtual void Release();
	virtual bool Load();

	vector<string> fileNames;
};
