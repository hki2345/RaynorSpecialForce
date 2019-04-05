#pragma once
#include "Component_DE.h"
#include <list>
#include <set>

class Camera;
class KMesh;
class KMaterial;
class Light : public Component_DE
{
public:
	friend class RenderManager;

	enum LIGHTTYPE
	{
		DIR,
		POINT,
		SPOT,
	};

	class LightColor 
	{
	public:
		KVector4 Diff; // 확산광
		KVector4 Spec; // 반사광
		KVector4 Ambi; // 환경광
	};

	class LightData 
	{
	public:
		LightColor Lc;
		KVector4 LightColor;
		KVector4 SpecColor;
		KVector4 LightPos;
		KVector4 LightDir;
		KVector4 LightDirInv;
		float Range;
		float Angle;
		int Type;
		int Dump;
	};

	class LightCBDATA 
	{
	public:
		LightData ArrLight[10];
		int LightCount;
		int LightCount1;
		int LightCount2;
		int LightCount3;
	};

private:
	LIGHTTYPE m_eType;
	LightData Data;
	std::set<int> m_Layer;

public:
	MatrixContainer		m_MD;
	KPtr<KMesh> m_LightMesh;
	KPtr<KMesh> m_VolumMesh;
	KPtr<KMaterial> m_VolumeMat;
	KPtr<KMaterial> m_LightMat;

	// 나중에 여러분들이 필터를 넣을것이라면
	// 필터도 또 따로 세팅을 해줘야 할거다.
public:
	virtual bool Init();
	void EndUpdate() override;

public:
	bool IsLight(int _GroupOrder) const
	{
		std::set<int>::iterator FindIter = m_Layer.find(_GroupOrder);

		if (FindIter == m_Layer.end())
		{
			return false;
		}

		return true;
	}

public:
	template<typename ...Rest>
	void PushLightLayer(Rest ...Arg)
	{
		LightLayerPush(Arg...);
	}

public:
	template<typename ...Rest>
	void LightLayerPush(int _Data, Rest ...Arg)
	{
		std::set<int>::iterator FindIter = m_Layer.find(_Data);

		if (FindIter == m_Layer.end())
		{
			m_Layer.insert(_Data);
		}
		LightLayerPush(Arg...);
	}

	void Light_Color(const KVector _Value)
	{
		Data.LightColor = _Value;
	}

	void Spec_Color(const KVector _Value)
	{
		Data.SpecColor = _Value;
	}
	void LightLayerPush() {	}
	void SetType(LIGHTTYPE _Type);

private: ////////// Render_Light
	void Render_Light(KPtr<Camera> _Camera);
	void Set_LightData(KPtr<Camera> _Camera);


public:
	Light();
	~Light();
};

