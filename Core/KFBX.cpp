#include "KFBX.h"
#include <atlstr.h>
#include "KMacro.h"
#include "DXContainer.h"

KBone* KFBX::Find_Bone(const wchar_t* _Name)
{
	std::multimap<std::wstring, KBone*>::iterator FIter = Bone_Map.find(_Name);

	if (Bone_Map.end() == FIter)
	{
		BBY;
		return nullptr;
	}

	return FIter->second;
}