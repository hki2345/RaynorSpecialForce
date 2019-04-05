#include "MultiCritical.h"

bool MultiCritical::bInit = false;
CRITICAL_SECTION MultiCritical::MapCB = CRITICAL_SECTION();
std::unordered_map<int, CRITICAL_SECTION*> MultiCritical::m_Map;

MultiCritical::Creator MultiCritical::CreatorInst = MultiCritical::Creator();
MultiCritical::Destroy MultiCritical::DestroyInst = MultiCritical::Destroy();
