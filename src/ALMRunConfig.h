#pragma once
#ifndef _ALMRUN_CONFIG_H_
#define _ALMRUN_CONFIG_H_
#include "ALMRunVersion.h"
#define TASKBARICON_TIP "ALMRun v"##VERSION_STR##"\n�����������"
#include "MerryWx.h"

class ALMRunConfig
{
public:
	int CompareMode;
	bool NumberKey;
	bool ShowTrayIcon;
	bool ShowTopTen;
	bool ExecuteIfOnlyOne;
	wxString Explorer;
	wxString Root;
	void get(const wxString& name);
	bool set(const wxString& name,const wxString& value);
	bool set(const wxString& name,const int value);
	ALMRunConfig(void);
	virtual ~ALMRunConfig(void);
};

extern ALMRunConfig* g_config;

enum
{
	MENU_ITEM_OPEN = 10001,
	MENU_ITEM_OPEN_CONFIG,
	MENU_ITEM_CONFIG,
	MENU_ITEM_ABOUT,
	MENU_ITEM_EXIT,
};

#endif