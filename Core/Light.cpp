#include "Light.h"
#include "State.h"
#include "ResourceManager.h"
#include "Core_Class.h"
#include "KMesh.h"
#include "KMaterial.h"
#include "DebugManager.h"


Light::Light() : m_eType(LIGHTTYPE::DIR)
{
	Data.Lc.Diff = KVector4(1.0f, 1.0f, 1.0f, 1.0f);
	Data.Lc.Spec = KVector4(0.2f, 0.2f, 0.2f, 1.0f);
	Data.Lc.Ambi = KVector4(0.f, 0.f, 0.f, .0f);

	Data.LightColor = KVector4::White;
	Data.SpecColor = KVector4::White;
	SetType(LIGHTTYPE::DIR);
}


Light::~Light()
{
}


bool Light::Init() 
{
	state()->This_RenderManager.Insert_Light(this);

	return true;
}

void Light::EndUpdate()
{

}

void Light::Set_LightData(KPtr<Camera> _Camera) 
{
	Data.LightDir = Trans()->forward_world();
	Data.LightPos = Trans()->pos_world();
	Data.Type = m_eType;
	Data.Range = Trans()->scale_world().x;

	//wchar_t Arr[256];
	//swprintf_s(Arr, L"LightDir : %f, %f, %f", Data.LightDir.x, Data.LightDir.y, Data.LightDir.z);
	//DebugManager::Draw_Log(Arr);
	//swprintf_s(Arr, L"LightPos : %f, %f, %f", Data.LightPos.x, Data.LightPos.y, Data.LightPos.z);
	//DebugManager::Draw_Log(Arr);

	Data.LightDir = _Camera->View().MulVecZero(Data.LightDir);
	Data.LightDirInv = -Data.LightDir;
	Data.LightPos = _Camera->View().MulVecOne(Data.LightPos);
}

void Light::Render_Light(KPtr<Camera> _Camera)
{
	// 라이트 데이터
	
	Core_Class::MainDevice().SettingCB<LightData>(L"DEFFERDLIGHTDATA", Data, SHTYPE::ST_PS);

	if (nullptr == m_LightMat || nullptr == m_LightMesh)
	{
		BBY;
	}

	if (m_eType == LIGHTTYPE::POINT)
	{
		Core_Class::MainDevice().Set_BS(L"VOLUME");
		KMatrix Scale;
		Scale.Scale(KVector4(m_Trans->scale_world().x, m_Trans->scale_world().x, m_Trans->scale_world().x));
		KMatrix World;
		World.Identity();
		World = Scale * m_Trans->rotatemat_const() * m_Trans->posmat_const();

		m_MD.m_W = World.RVTranspose();
		m_MD.m_V = _Camera->View().RVTranspose();
		m_MD.m_P = _Camera->Proj().RVTranspose();
		m_MD.m_WV = (World * _Camera->View()).RTranspose();
		m_MD.m_WVP = (World * _Camera->ViewProj()).RTranspose();

		Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_VS);
		Core_Class::MainDevice().SettingCB<MatrixContainer>(L"MATCON", m_MD, SHTYPE::ST_PS);

		// 두번해야 하느냐?
		// 후면보다 앞에 있는 픽셀 검색.
		Core_Class::MainDevice().Set_RS(L"SFRONT");
		Core_Class::MainDevice().Set_DSS(L"BACK_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();

		Core_Class::MainDevice().Set_RS(L"SBACK");
		Core_Class::MainDevice().Set_DSS(L"FRONT_ST", 1);
		m_VolumeMat->Update();
		m_VolumMesh->Update();
		m_VolumMesh->Render();
		Core_Class::MainDevice().Set_DSS(L"PASS_ST", 1);
	}
	else if (m_eType == LIGHTTYPE::SPOT)
	{

	}
	else 
	{
		Core_Class::MainDevice().Set_DSS(L"PASS_ST", 0);
	}


	// 색이 어떻든 더한다.
	Core_Class::MainDevice().Set_BS(L"LIGHTONE");
	m_LightMat->Update();
	m_LightMat->Update_Tex();
	m_LightMesh->Update();
	m_LightMesh->Render();
	m_LightMat->Reset_Tex();
}

void Light::SetType(LIGHTTYPE _Type) 
{
	m_eType = _Type;

	switch (_Type)
	{
	case Light::DIR:
		m_LightMesh = ResourceManager<KMesh>::Find(L"RECT");
		break;
	case Light::POINT:
		m_VolumMesh = ResourceManager<KMesh>::Find(L"SPHERE");
		m_LightMesh = ResourceManager<KMesh>::Find(L"RECT");		
		break;
	case Light::SPOT:
		break;
	default:
		break;
	}

	m_VolumeMat = ResourceManager<KMaterial>::Find(L"VOLUMEMAT");
	m_LightMat = ResourceManager<KMaterial>::Find(L"DEFFERDLIGHTMAT");
}