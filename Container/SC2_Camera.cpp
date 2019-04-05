#include "SC2_Camera.h"

#include "Controll_AI.h"
#include "Controll_User.h"

#include "Con_Class.h"
#include "SC2Manager.h"
#include "SC2_Force.h"
#include "Force_Unit.h"

#include <InputManager.h>
#include <TimeManager.h>
#include <DebugManager.h>
#include <Core_Class.h>

#include <KMesh.h>
#include <KMaterial.h>
#include <Sampler.h>


#include <KThread.h>

SC2_Camera::SC2_Camera() : 
	m_Speed(10.0f)
	, m_PlayCamInfo(.0f, -5.5f, -1.0f, .0f)
{
}


SC2_Camera::~SC2_Camera()
{
}

bool SC2_Camera::Init(const int& _Order /*= 0*/)
{
	m_Trans = one()->Trans();
	m_CMode = SC2_CAMMODE::S2M_EDIT;
	m_Cam = Get_Component<Camera>();

	if (nullptr == m_Cam)
	{
		KPtr<TheOne> CamOne2 = Core_Class::MainScene()->Create_One(L"Sc2");
		KPtr<Camera> Cam2 = CamOne2->Add_Component<Camera>(_Order);
	}

	m_Cam->ProjectionMode(Camera::PROJ_MODE::PM_PERS);

	m_Mesh = ResourceManager<KMesh>::Find(L"RECT");
	m_Mtl = ResourceManager<KMaterial>::Find(L"IMGMAT");
	m_Smp = ResourceManager<Sampler>::Find(L"DefaultSmp");


#pragma region KEYCREATE


	if (false == InputManager::IsKey(L"FREELEFT"))
	{
		InputManager::Create_Command(L"FREELEFT", 'A');
	}

	if (false == InputManager::IsKey(L"FREERIGHT"))
	{
		InputManager::Create_Command(L"FREERIGHT", 'D');
	}

	if (false == InputManager::IsKey(L"FREEUP"))
	{
		InputManager::Create_Command(L"FREEUP", VK_SPACE);
	}

	if (false == InputManager::IsKey(L"FREEDOWN"))
	{
		InputManager::Create_Command(L"FREEDOWN", VK_LCONTROL);
	}

	if (false == InputManager::IsKey(L"FREEFORWARD"))
	{
		InputManager::Create_Command(L"FREEFORWARD", 'W');
	}

	if (false == InputManager::IsKey(L"FREEBACK"))
	{
		InputManager::Create_Command(L"FREEBACK", 'S');
	}

	if (false == InputManager::IsKey(L"FREEEYE"))
	{
		InputManager::Create_Command(L"FREEEYE", VK_MBUTTON);
	}

	if (false == InputManager::IsKey(L"Boost"))
	{
		InputManager::Create_Command(L"Boost", VK_SHIFT);
	}

	if (false == InputManager::IsKey(L"MODECHANAGE"))
	{
		InputManager::Create_Command(L"MODECHANAGE", 'G');
	}

	if (false == InputManager::IsKey(L"Z"))
	{
		InputManager::Create_Command(L"Z", 'Z');
	}

	if (false == InputManager::IsKey(L"X"))
	{
		InputManager::Create_Command(L"X", 'X');
	}

	if (false == InputManager::IsKey(L"C"))
	{
		InputManager::Create_Command(L"C", 'C');
	}

	if (false == InputManager::IsKey(L"F"))
	{
		InputManager::Create_Command(L"F", 'F');
	}



#pragma endregion
	return true;
}

void SC2_Camera::Update()
{
	KLOG(L"%f", 1 / DELTATIME);

	switch (m_CMode)
	{
	case SC2_Camera::S2M_EDIT:
		Update_Edit();
		break;

	case SC2_Camera::S2M_INGAME:
		Update_InGame();
		break;

	case SC2_Camera::S2M_PART:
		Update_Part();
		break;
	default:
		break;
	}
}


/************ 카메라 상태 ***************/
void SC2_Camera::Update_Edit()
{
	Update_ScrCheck();
	Update_Key();
	Update_Wheel();
}

void SC2_Camera::Update_InGame()
{
	if (0 != Con_Class::s2_manager()->m_GameSet)
	{
		return;
	}

	if (nullptr == m_pUser || true == m_pUser->one()->Is_Death())
	{
		m_CMode = SC2_CAMMODE::S2M_EDIT;
		return;
	}

	// 한정거리
	int A = 0;
	InputManager::WheelValue(&A);

	// 마우스 휠에 따라 캐릭터 줌인
	if (0 != A)
	{
		m_PlayCamInfo.x += (float)A * .005f;
	}

	// 최소 거리
	if (m_PlayCamInfo.y > m_PlayCamInfo.x)
	{
		m_PlayCamInfo.x = m_PlayCamInfo.y;
	}
	// 최대 거리
	if (m_PlayCamInfo.z < m_PlayCamInfo.x)
	{
		m_PlayCamInfo.x = m_PlayCamInfo.z;
	}


	KPtr<TransPosition> UTrans = m_pUser->one()->Trans();
	m_Trans->pos_local(UTrans->pos_local() - 
		m_pUser->pos_player() * m_PlayCamInfo.x * KVector(1.0f, -1.0f, 1.0f, 1.0f) + KVector(.0f, 1.25f, .0f));
	m_Trans->rotate_localrad(m_pUser->rot_player() + KVector(.0f, KPI));
	UTrans->rotate_localrad(m_pUser->rot_player() + KVector(.0f, KPI));
}

void SC2_Camera::Update_Part()
{
	MatrixContainer tMXData;
	KMatrix UScale;
	KMatrix m_Pos;

	UScale.Identity();
	UScale.Scale(KVector4(OutSize.x, OutSize.y, 1.0f));

	m_Pos.Identity();
	m_Pos.Translate(
		KVector4((-Core_Class::MainWindow().width_f() * 0.5f) + OutPos.x + (OutSize.x * 0.5f)
			, (Core_Class::MainWindow().height_f() * 0.5f) + OutPos.y - (OutSize.y * 0.5f)
			, 1.1f));

	KMatrix m_W = UScale * m_Pos;

	tMXData.m_V = m_Cam->View();
	tMXData.m_P = m_Cam->Proj();
	tMXData.m_W = m_W;
	tMXData.m_WV = m_W * tMXData.m_V;
	tMXData.m_WVP = tMXData.m_WV * tMXData.m_P;
	tMXData.RTrans();

	
	Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", tMXData, SHTYPE::ST_VS);

	m_Mtl->Update();
	m_Mesh->Update();
	m_Mesh->Render();
}



/**************** 구현 **************/

bool SC2_Camera::Update_ScrCheck()
{
	KVector2 MPos = InputManager::MousePos();
	bool Check;

	if (kwindow()->size() > MPos && KVector2::Zero < MPos)
	{
		Check = true;
	}
	else
	{
		Check = false;
	}

	return Check;
}

void SC2_Camera::Update_Key()
{

	if (true == KEY_PRESS(L"Boost"))
	{
		m_Speed = 100.0f;
	}
	else {
		m_RotSpeed = 30.0f;
		m_Speed = 10.0f;
	}

	if (true == KEY_DOWN(L"MODECHANAGE"))
	{
		m_Cam->Change_Mode();
	}

	
	if (true == KEY_DOWN(L"Z"))
	{
		KVector4 Rot = m_Trans->rotate_local();
		Rot.z = 0.0f;
		m_Trans->rotate_local(Rot);
	}

	if (true == KEY_DOWN(L"X"))
	{
		KVector4 Rot = m_Trans->rotate_local();
		Rot.x = 0.0f;
		m_Trans->rotate_local(Rot);
	}

	if (true == KEY_DOWN(L"F"))
	{
		m_Trans->Reset();
		m_Trans->Moving(KVector4(0.0f, 0.0f, -10.0f));
	}

	if (true == KEY_PRESS(L"FREELEFT"))
	{
		m_Trans->Moving(m_Trans->left_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREERIGHT"))
	{
		m_Trans->Moving(m_Trans->right_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREEUP"))
	{
		m_Trans->Moving(m_Trans->up_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREEDOWN"))
	{
		m_Trans->Moving(m_Trans->down_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREEFORWARD"))
	{
		m_Trans->Moving(m_Trans->forward_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREEBACK"))
	{
		m_Trans->Moving(m_Trans->back_local() * DELTATIME * m_Speed);
	}

	if (true == KEY_PRESS(L"FREEEYE"))
	{
		m_Trans->Rotating_Deg(KVector4(InputManager::MouseDir().y * m_RotSpeed * DELTATIME, InputManager::MouseDir().x * m_RotSpeed *  DELTATIME));
	}
}

void SC2_Camera::Update_Wheel()
{
	int Check = 0;

	// 휠을 움직였을 때 상황
	if (0 != InputManager::WheelValue(&Check))
	{
		m_Trans->Moving(m_Trans->forward_local() * (float)Check * .02f);
	}
}



void SC2_Camera::Set_Edit()
{
	m_CMode = SC2_CAMMODE::S2M_EDIT;

	KPtr<Force_Unit> TU = Con_Class::force_player()->Find_Unit(0);
	TU->Delete_Component<Controll_User>();
}


void SC2_Camera::Set_Part()
{
	m_CMode = SC2_CAMMODE::S2M_PART;
}

KPtr<Force_Unit> SC2_Camera::Set_InGame()
{
	m_CMode = SC2_CAMMODE::S2M_INGAME;

	KPtr<Force_Unit> TU = Con_Class::force_player()->Find_Unit(0);
	KPtr<Controll_User> TC = TU->Get_Component<Controll_User>();

	Set_User(TC);

	return TU;
}