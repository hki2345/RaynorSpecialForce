#include "Force_Unit.h"
#include "SC2_Force.h"


#include <Core_Class.h>

#include <ReadStream.h>
#include <WriteStream.h>

#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <KBox_Col.h>



void Force_Unit::Update_Game()
{

}

void Force_Unit::Damage(const float& _Value)
{
	m_Info.HP -= _Value;
}
