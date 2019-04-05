#pragma once
#include "KMath.h"
#include "Component.h"


// ��� ��ġ �Ӽ��� ���� �ٽ�
class TransPosition final : public Component 
{

public:
	TransPosition();
	~TransPosition();

public:
	enum AXIS
	{
		AX_F,
		AX_U,
		AX_R,
		AX_MAX
	};

public:
	static bool IsMulti(TheOne* _pActor);

private:

	// Ʈ���� ���� �ֽ�ȭ�� ����̷������ �ʴ´�.
	// �ٲ� ���� �����Ѵ�.
	bool m_bLS;
	bool m_bLR;
	bool m_bLP;
	bool m_bW;



	// ũ�� ���� ��ġ ����
	KVector4 m_LScale;		// ũ
	KVector4 m_LRot;		// ��
	KVector4 m_LPos;		// ��

	KVector4 m_WRot;		// ��
	KVector4 m_WScale;		// ũ
	KVector4 m_WPos;		// ��

	KVector4 m_LRevolve;	// ��

	// ��������
	KVector4 m_LAx[AXIS::AX_MAX];		// ��
	KVector4 m_WAx[AXIS::AX_MAX];		// ��


	// ��� �κ�
	KMatrix m_SMat; // ũ
	KMatrix m_RMat; // ��
	KMatrix m_PMat; // ��

	KMatrix m_WMat; // ��

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // ��



public:
	bool bworld() {		return m_bW;	}


	void pos_local(const KVector4& _Value)
	{
		m_LPos = _Value;

		// ���⼭ �ڽĵ��� 1������ ���̰� ���� -> 1 ������Ʈ
		if (nullptr != one()->Parent())
		{
			m_WPos = one()->Trans()->pos_world() + m_LPos;
		}

		m_bLP = true;
		m_bW = true;
	}

	void Moving(const KVector4& _Value)
	{
		m_LPos += _Value;
		m_bLP = true;
		m_bW = true;
	}

	void pos_world(const KVector4& _Value)
	{
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
		// �θ� �����ϰ� ���� ������ �ű�� ���� �ȴ�.
		if (nullptr != one()->Parent())
		{
			m_LPos = _Value - one()->Trans()->pos_world();
		}
		else {
			m_LPos = _Value;
		}
		m_WPos = _Value;
		m_bW = true;
	}

	void Rotating_Deg(const KVector4& _Value)
	{
		m_LRot += _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void Rotating_Rad(const KVector4& _Value)
	{
		m_LRot += _Value;
		m_bLR = true;
		m_bW = true;
	}

	void rotate_localrad(const KVector4& _Value)
	{
		m_LRot = _Value;
		m_bLR = true;
		m_bW = true;
	}

	void rotate_local(const KVector4& _Value)
	{
		m_LRot = _Value * HMATHVAR::DTOR;
		m_bLR = true;
		m_bW = true;
	}

	void rotate_world(const KVector4& _Value) 
	{
		// �ϴ� ���� ���� ���
		if (nullptr != one()->Parent())
		{
			// ȸ���� �ܼ��� �θӿ��� �ٲ�� ����
			m_LRot = (m_WRot * HMATHVAR::DTOR) - one()->Trans()->rotate_world();
		}
		else {
			m_LRot = _Value * HMATHVAR::DTOR;
		}

		m_bLR = true;
		m_bW = true;
	}


	void scale_local(const KVector4& _Scale)
	{
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
		m_LScale = _Scale;
		m_bLS = true;
		m_bW = true;
	}

	void scale_world(const KVector4& _Scale)
	{
		// ���⼭ �ٷ� ����� ������ ����� �ְ�.
		if (nullptr != one()->Parent())
		{
			m_LScale = m_WScale - one()->Trans()->scale_world();
		}
		else 
		{
			m_LScale = _Scale;
		}

		m_bLS = true;
		m_bW = true;
	}

	KVector4 forward_local() {		return m_LAx[AX_F];	}
	KVector4 forward_world() {		return m_WAx[AX_F];	}
	KVector4 up_local() { return m_LAx[AX_U]; }
	KVector4 up_world() { return m_WAx[AX_U]; }
	KVector4 right_local() { return m_LAx[AX_R]; }
	KVector4 right_world() { return m_WAx[AX_R]; }
	KVector4 back_local() { return -m_LAx[AX_F]; }
	KVector4 back_world() { return -m_WAx[AX_F]; }
	KVector4 left_local() { return -m_LAx[AX_R]; }
	KVector4 left_world() { return -m_WAx[AX_R]; }
	KVector4 down_local() { return -m_LAx[AX_U]; }
	KVector4 down_world() { return -m_WAx[AX_U]; }

	KVector4 pos_local() const	{ return m_LPos; }
	KVector4 pos_world() const { return m_WPos; }
	KVector4 scale_local() const { return m_LScale; }

	KVector4 scale_world() const { return m_WScale; }
	const KVector4& scale_worldconst() const { return m_WScale; }

	KVector4 rotate_localquat() { return m_LRot.Compute_Quarternion(); }
	KVector4 rotate_local()	{ return m_LRot;	}
	KVector4 rotate_localDeg() { return m_LRot * HMATHVAR::RTOD; }
	KVector4 rotate_world() { return m_WRot; }

	KMatrix matrix_world() const
	{
		return m_WMat;
	}

	
		
		

	const KMatrix& posmat_const() const
	{
		return m_PMat;
	}

	const KMatrix& scalemat_const() const
	{
		return m_SMat;
	}

	const KMatrix& rotatemat_const() const
	{
		return m_RMat;
	}

	const KMatrix& rotateYmat_const() const
	{
		return m_RMatY;
	}

	const KMatrix& worldmat_const() const
	{
		return m_WMat;
	}

	void Reset();

public:
	virtual bool Init() override;
	virtual void FinalUpdate() override;
	virtual void EndUpdate() override;
};

