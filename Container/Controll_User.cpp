#include "Controll_User.h"
#include "Force_Unit.h"

#include <Core_Class.h>
#include <ResourceManager.h>
#include <Renderer_BonAni.h>
#include <Renderer_Terrain.h>
#include <Renderer_UI.h>
#include <InputManager.h>
#include <KWindow.h>


#include <KBox_Col.h>
#include <KRay3D.h>
#include <KFont.h>


#include "SC2_Camera.h"
#include <SoundPlayer.h>


Controll_User::Controll_User() :
	RayCol(nullptr),
	m_UTime(.0f),
	m_ASound(false),
	m_MSound(false),
	m_FiATime(.0f),
	m_MeATime(.0f),
	m_OpATime(.0f)
{
}


Controll_User::~Controll_User()
{
	if(nullptr != m_uMedic)      {m_uMedic->one()->Set_Death();	 }
	if (nullptr != m_uFire)		 {m_uFire->one()->Set_Death();	 }
	if (nullptr != m_uOpti)		 {m_uOpti->one()->Set_Death();	 }
	if (nullptr != m_uCMedic)	 {m_uCMedic->one()->Set_Death(); }
	if (nullptr != m_uCFire)	 {m_uCFire->one()->Set_Death();	 }
	if (nullptr != m_uCOpti)	 {m_uCOpti->one()->Set_Death();	 }
	if (nullptr != m_uHp)		 {m_uHp->one()->Set_Death();	 }
	if (nullptr != m_uBHP)		 {m_uBHP->one()->Set_Death();	 }
	if (nullptr != m_uFocusHP)	 {m_uFocusHP->one()->Set_Death();}
	if (nullptr != m_uFBackHP)	 {m_uFBackHP->one()->Set_Death();}
	if (nullptr != m_uAim)	 { m_uAim->one()->Set_Death(); }
	if (nullptr != m_uConsole) { m_uConsole->one()->Set_Death(); }
	if (nullptr != m_uCover) { m_uCover->one()->Set_Death(); }

	if (nullptr != m_pSound)
	{
		m_pSound->Stop();
	}

	if (nullptr != RayCol)
	{
		RayCol->StayFunc_Delete(L"Controll User");
		RayCol->ExitFunc_Delete(L"Controll User");
	}
}


void Controll_User::Set_Render()
{
	m_RList = one()->Get_Component_List<Renderer_BonAni>();
}

void Controll_User::Add_Render(KPtr<Renderer_BonAni> _Other)
{
	m_RList.push_back(_Other);
}

void Controll_User::Del_Render(KPtr<Renderer_BonAni> _Other)
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		if ((*m_SRI) == _Other)
		{
			m_RList.erase(m_SRI);
			return;
		}
	}
}

bool Controll_User::Init(KPtr<Force_Unit> _Unit, KPtr<SC2_Camera> _Cam)
{
	if (false == IS_KEY(L"MUNLOCK"))
	{
		InputManager::Create_Command(L"MUNLOCK", VK_MENU);
	}
	if (false == IS_KEY(L"F"))
	{
		InputManager::Create_Command(L"F", 'F');
	}
	if (false == IS_KEY(L"MFOR"))
	{
		InputManager::Create_Command(L"MFOR", 'W');
	}
	if (false == IS_KEY(L"MBACK"))
	{
		InputManager::Create_Command(L"MBACK", 'S');
	}
	if (false == IS_KEY(L"MLEFT"))
	{
		InputManager::Create_Command(L"MLEFT", 'A');
	}
	if (false == IS_KEY(L"MRIGHT"))
	{
		InputManager::Create_Command(L"MRIGHT", 'D');
	}

	if (false == IS_KEY(L"SHOT"))
	{
		InputManager::Create_Command(L"SHOT", VK_LBUTTON);
	}
	if (false == IS_KEY(L"BOMB"))
	{
		InputManager::Create_Command(L"BOMB", VK_RBUTTON);
	}


	if (false == IS_KEY(L"HEAL"))
	{
		InputManager::Create_Command(L"HEAL", 'E');
	}
	if (false == IS_KEY(L"RUN"))
	{
		InputManager::Create_Command(L"RUN", VK_LSHIFT);
	}
	if (false == IS_KEY(L"OPTI"))
	{
		InputManager::Create_Command(L"OPTI", 'Q');
	}


	if (nullptr == _Unit->terrain())
	{
		BBY;
	}


	m_pUnit = _Unit;
	m_pCam = _Cam;
	m_pCam->Set_User(this);
	
	m_FiTime = 10.0f;
	m_MeTime = 45.0f;
	

	m_pUnit->playable_type(PLAYABLE_TYPE::PBT_USER);
	m_MirrorY = false;



	RayCol = state()->camera()->Get_Component<KRay3D>();

	if (nullptr == RayCol)
	{
		return true;
	}


	RayCol->StayFunc(L"Controll User", this, &Controll_User::Update_StayCol);
	RayCol->ExitFunc(L"Controll User", this, &Controll_User::Update_ExitCol);


	if (m_RList.size() == 0)
	{
		m_RList = m_pUnit->list_renderer();
	}


	Init_UI();
	Init_Game();
	return true;
}


void Controll_User::Update()
{

	if (SC2_Camera::SC2_CAMMODE::S2M_EDIT == m_pCam->cam_mode())
	{
		InputManager::Set_MUnLock();
		return;
	}

	if (true == m_pUnit->one()->Is_Death())
	{
		return;
	}
	
	if (true == m_pUnit->Is_HPDeath())
	{
		m_AType = AT_DEATH;
	}
	
	Update_Game();
	Update_Move();
	Update_Act();
	
	Update_UI();
	Update_AUI();


	Update_Terrain();
	Update_Mouse();
	Update_RenCol();
}



void Controll_User::Update_StayCol(KCollision* _Left, KCollision* _Right)
{
	KPtr<KBox_Col> Tmp = _Left->Get_Component<KBox_Col>();

	if (nullptr == Tmp)
	{
		Tmp = _Right->Get_Component<KBox_Col>();
		if (nullptr == Tmp)
		{
			m_pFocusUnit = nullptr;
			return;
		}
	}

	m_pFocusUnit = Tmp->Get_Component<Force_Unit>();

	if (m_pFocusUnit == m_pUnit || m_pFocusUnit->Is_HPDeath())
	{
		m_pFocusUnit = nullptr;
		return;
	}
}


void Controll_User::Update_ExitCol(KCollision* _Left, KCollision* _Right)
{
	m_pFocusUnit = nullptr;
}

void Controll_User::Update_RenCol()
{
	m_SRI = m_RList.begin();
	m_ERI = m_RList.end();

	for (; m_SRI != m_ERI; ++m_SRI)
	{
		(*m_SRI)->rot_pivot(m_RenderRot + KVector(.0f, KPI, .0f));
	}
	
	m_pUnit->Rot_Unit(m_RenderRot + KVector(.0f, KPI, .0f));
}


void Controll_User::Update_Move()
{
	if (true == m_OutGame)
	{
		return;
	}
	if (Controll_User::AT_DEATH == m_AType)
	{
		Update_DEATH();
		KLOG(L"Unit Move: DEATH");
		return;
	}

	switch (m_MType)
	{
	case Controll_User::MT_NONE:
		KLOG(L"Unit Move: NONE");
		break;
	case Controll_User::MT_IDLE:
		Update_MIDLE();
		KLOG(L"Unit Move: IDLE");
		break;
	case Controll_User::MT_MOVE:
		Update_MOVE();
		KLOG(L"Unit Move: MOVE");
		break;
	case Controll_User::MT_RUN:
		Update_RUN();
		KLOG(L"Unit Move: RUN");
		break;
	default:
		break;
	}	
}


void Controll_User::Update_Act()
{
	if (true == m_OutGame)
	{
		return;
	}
	m_FiATime += DELTATIME;
	m_MeATime += DELTATIME;
	m_OpATime += DELTATIME;

	switch (m_AType)
	{
	case Controll_User::AT_IDLE:
		Update_AIDLE();
		KLOG(L"Unit Act: IDLE");
		break;
	case Controll_User::AT_ATTACK:
		Update_ATTACK();
		KLOG(L"Unit Act: ATTACK");
		break;
	case Controll_User::AT_BOMB:
		Update_BOMB();
		KLOG(L"Unit Act: BOMB");
		break;
	case Controll_User::AT_STORY:
		KLOG(L"Unit Act: STORY");
		break;
	case Controll_User::AT_HEAL:
		Update_HEAL();
		KLOG(L"Unit Act: HEAL");
		break;
	case Controll_User::AT_OPTI:
		Update_OPTI();
		KLOG(L"Unit Act: OPTI");
		break;
	default:
		break;
	}
}

void Controll_User::Update_Mouse()
{
	if (Controll_User::AT_STORY == m_AType)
	{
		return;
	}
	if (true == m_OutGame)
	{
		return;
	}

	if (true == InputManager::Is_MouseMove())
	{
		if (true == KEY_PRESS(L"MUNLOCK"))
		{
			InputManager::Set_MUnLock();
		}
		else
		{
			m_PlayRot.x += InputManager::MouseDir().y * .25f *  DELTATIME;
			m_PlayRot.y += InputManager::MouseDir().x * .25f *  DELTATIME;

			KMatrix RMatX;
			KMatrix RMatY;
			KMatrix RMatZ;

			RMatX.RotX(m_PlayRot.x); // X
			RMatY.RotY(m_PlayRot.y); // Y
			RMatZ.RotZ(m_PlayRot.z); // Z
								
			KMatrix RMat = RMatX * RMatY * RMatZ;

			
			m_PlayPos = KVector4::Forword;
			m_PlayPos = RMat.MulVecZero(m_PlayPos);
			m_PlayPos.NormalizeVec3();

			InputManager::Set_MLock();
		}
	}
}


void Controll_User::Update_Terrain()
{
	KVector4 TT = one()->Trans()->pos_local();


	if (m_pUnit->terrain()->TER_Left() > TT.x)
	{
		TT.x = m_pUnit->terrain()->TER_Left();
	}
	// ²¨ ¾¾¹ß
	// if (m_pUnit->terrain()->TER_Right() < TT.x)
	// {
	// 	TT.x = m_pUnit->terrain()->TER_Right();
	// }
	if (m_pUnit->terrain()->TER_Down() > TT.z)
	{
		TT.z = m_pUnit->terrain()->TER_Down();
	}
	// if (m_pUnit->terrain()->TER_Up() < TT.z)
	// {
	// 	TT.z = m_pUnit->terrain()->TER_Up();
	// }
	one()->Trans()->pos_local(TT);
}