#pragma once
#include "KMath.h"
#include "Component.h"


// 모든 위치 속성에 대한 핵심
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

	// 트렌스 폼의 최신화는 계속이루어지지 않는다.
	// 바뀔 때만 연산한다.
	bool m_bLS;
	bool m_bLR;
	bool m_bLP;
	bool m_bW;



	// 크기 자전 위치 공전
	KVector4 m_LScale;		// 크
	KVector4 m_LRot;		// 자
	KVector4 m_LPos;		// 이

	KVector4 m_WRot;		// 이
	KVector4 m_WScale;		// 크
	KVector4 m_WPos;		// 이

	KVector4 m_LRevolve;	// 공

	// 기저벡터
	KVector4 m_LAx[AXIS::AX_MAX];		// 이
	KVector4 m_WAx[AXIS::AX_MAX];		// 이


	// 행렬 부분
	KMatrix m_SMat; // 크
	KMatrix m_RMat; // 자
	KMatrix m_PMat; // 이

	KMatrix m_WMat; // 이

	KMatrix m_RMatX, m_RMatY, m_RMatZ; // 자



public:
	bool bworld() {		return m_bW;	}


	void pos_local(const KVector4& _Value)
	{
		m_LPos = _Value;

		// 여기서 자식들은 1프레임 차이가 난다 -> 1 업데이트
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
		// 여기서 바로 계산해 버리는 방법이 있고.
		// 부모를 무시하고 나는 무조건 거기로 가면 된다.
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
		// 일단 라디안 기준 계산
		if (nullptr != one()->Parent())
		{
			// 회전은 단순히 부머에서 바뀌는 형식
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
		// 여기서 바로 계산해 버리는 방법이 있고.
		m_LScale = _Scale;
		m_bLS = true;
		m_bW = true;
	}

	void scale_world(const KVector4& _Scale)
	{
		// 여기서 바로 계산해 버리는 방법이 있고.
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

