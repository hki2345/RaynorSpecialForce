#include "Begin_Updater.h"

Begin_Updater::Begin_Updater() : m_bUpdate(true), m_bDeath(false)
{
}

Begin_Updater::~Begin_Updater()
{
}

bool Begin_Updater::Init() { return true; }
void Begin_Updater::PrevUpdate() {}
void Begin_Updater::Update() {}
void Begin_Updater::NextUpdate() {}
void Begin_Updater::DebugUpdate() {}
void Begin_Updater::DebugRender() {}
void Begin_Updater::UIRender() {};