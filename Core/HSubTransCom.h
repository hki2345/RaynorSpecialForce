#pragma once
#include "Component_DE.h"

// �浹ü
// ������ ���� ����� ���̴�.
class HSubTransCom : public Component_DE
{
private:
	enum SUBMODE
	{
		NONE,
		PARENT,
		SELF,
	};

private:
	SUBMODE m_eMode;
	KVector4 UScale;
	KVector4 m_Rot; // ������ ���� �ش�.
	KVector4 m_Pivot;

	KMatrix m_SMat; // ũ
	KMatrix m_RMat; // ��
	KMatrix m_PMat; // ��

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // ��

	KMatrix m_SubWMat;

public:
	void SelfMode() 	{		m_eMode = SUBMODE::SELF;	}
	void NoneMode()	{		m_eMode = SUBMODE::NONE;	}
	void ParentMode() { m_eMode = SUBMODE::PARENT; }

	void SubScale(const KVector4& _Scale) {
		UScale = _Scale;
		SelfMode();
	}

	void SubRot(const KVector4& _Rot) {
		m_Rot = _Rot;
		SelfMode();
	}

	void SubPivot(const KVector4& _Pivot) {
		m_Pivot = _Pivot;
		SelfMode();
	}

	KVector4 WSubPivot() 
	{
		return m_SubWMat.v4;
	}

public:
	const KMatrix& CSWMat() const
	{
		return m_SubWMat;
	}

public:
	void ComInit() override;

public:
	void SubTransUpdate();

public:
	HSubTransCom();
	~HSubTransCom();
};

