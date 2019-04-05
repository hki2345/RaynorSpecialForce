#pragma once
#include "Renderer.h"
#include "KFBX.h"
#include "KM3.h"
#include "Changer_Animation.h"

class Renderer_BonAni : public Renderer
{
public:
	Renderer_BonAni();
	~Renderer_BonAni();

private:
	KVector m_RotPivot;


	// 일단 본을 찍어내기 위해선 본을 다 알아야 하겠다.
	KPtr<MeshContainer> MCon;
	KPtr<Changer_Animation> CAni;

	//FBXLoader* m_pLoader;

	// 애니메이션 0번쨰 프레임 ~ X 프레임 보는 그 시간
	bool m_InitAni;
	bool m_loop;
	bool m_Done;

	int m_ClipInx;
	int m_FrameCnt;
	int iFrameInx;
	int PauseInx;

	float m_UpdateSpd;
	float m_CurTime;
	float m_UpdateTime;


	// 세력 설정
	std::wstring	m_FName;
	KColor			m_FColor;

	// 행렬과 본을 따로 저장해 둔다.
	std::vector<KMatrix> m_MXData_CurAni;
	std::vector<KMatrix> m_BoneData_CurAni;

	// 본의 정보가 쉐이더로 넘기기엔 버퍼의 크기가 너무 커지니
	// 차라리 텍스쳐로 보내는 방법 - 텍스쳐는 그 자체로 정보가 될 수 있다.
	KPtr<Texture> m_pBoneTex;
	KPtr<Texture> m_pColTex;


public:
	int index_frame()
	{
		return iFrameInx;
	}

	bool Check_AniDone()
	{
		return m_Done;
	}

	void rot_pivot(const KVector& _Value)
	{
		m_RotPivot = _Value;
	}

	// true - 스테틱, false
	void Set_Static()
	{
		ROpt.IsBoneAni = 0;
	}
	void Set_Dynamic()
	{
		ROpt.IsBoneAni = 1;
	}


	void force_name(const std::wstring _Value)
	{
		m_FName = _Value;
	}
	KColor& force_color()
	{
		return m_FColor;
	}
	void force_color(const KColor& _Value)
	{
		m_FColor = _Value;
	}

	int& cur_frame()
	{
		return iFrameInx;
	}

	int& pause_inx()
	{
		return PauseInx;
	}

	void pause_inx(const int& _Value)
	{
		PauseInx = _Value;
	}

	void loop(const bool& _Value)
	{
		m_loop = _Value;
	}

	void Reset_Frame()
	{
		m_UpdateTime = .0f;
	}
	// 일단, 듸우는 거에 초점을 맞춘다.

private:
	void Init_Mesh();


public:
	void Init_Ani()
	{
		m_InitAni = true;
	}

	KPtr<Changer_Animation> changer_animation()
	{
		return CAni;
	}
	KPtr<MeshContainer> mesh_container()
	{
		return MCon;
	}

	KMatrix Get_BoneMX(const wchar_t* _Name);
	KMatrix Get_WBoneMX(const wchar_t* _Name);


	void PrevUpdate_Ani();

public:
	void Load_FbxTest(const wchar_t* _Path);
	void Set_Fbx(const wchar_t* _Name);
	void Set_TexturePath(const TEX_TYPE& _Value, const wchar_t* _Path);
	void PrevUpdate() override;
	void RenderBegin(KPtr<Camera> _Cam, const KUINT& _MeshIdx, const KUINT& _MtlIdx) override;
	void Update_Trans(KPtr<Camera> _Camera) override;


	// 랜더러에서 시키면 자동으로 여과되서 들어간다.
	// 물론 직접 때려박을 수도 있다.
	KPtr<Changer_Animation> Create_Animation(const bool& _Find = true);


	KPtr<Changer_Animation::Ani_Clip> Create_Clip(const wchar_t* _Name, const int& _Start, const int& _End, const bool& _Find = true);
	bool Erase_Clip(const wchar_t* _Name);
	void Set_Clip(const wchar_t* _Name);
	void Set_Clip(const int& _Num);
};

