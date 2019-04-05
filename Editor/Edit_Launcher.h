#pragma once
#include <KCore.h>

class Edit_Launcher : public KCore::Launcher
{
public:
	void Build() override;
	
public:
	Edit_Launcher();
	~Edit_Launcher();
};

