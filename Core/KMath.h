#pragma once
#include "DXContainer.h"

// rand를 쓰는 건 멍청한 짓
// 이젠 std에서 난수를 직접 제공한다.
#include <random>

#include "DebugManager.h"


class TransPosition;
class KMath
{

#pragma region Random
private:
	static std::random_device RD;
	static std::mt19937 MT;

public:
	static float random_f()
	{
		return (float)MT();
	}

	static float random_f(const float& _Min, const float _Max)
	{
		/*if (_Min > _Max)
		{
			BBY;
		}
		std::random_device tRD;
		std::mt19937 tMT(tRD());	
		std::uniform_real_distribution<float> disF(_Min, _Max);
		return disF(tMT);		*/
		
		float Min = _Min;
		float Max = _Max;
		if (Min > Max)
		{
			float Tmp = _Min;
			Min = Max;
			Max = Tmp;
		}
		
		std::uniform_real_distribution<float> disF = std::uniform_real_distribution<float>(Min, Max);
		return disF(MT);
	}

	static float random_f(const float& _Max)
	{
		/*if (.0f > _Max)
		{
			BBY;
		}
		std::random_device tRD;
		std::mt19937 tMT(tRD());
		std::uniform_real_distribution<float> disF(.0f, _Max);
		return disF(tMT);*/

		float V = _Max;
		if (0 > V)
		{
			V *= -1.0f;
		}

		std::uniform_real_distribution<float> disF = std::uniform_real_distribution<float>(.0f, V);
		return disF(MT);
	}





	static int random_int()
	{
		return MT();
	}

	static int random_int(const int& _Min, const int& _Max)
	{
		/*if (_Min > _Max)
		{
			BBY;
		}
		std::random_device tRD;
		std::mt19937 tMT(tRD());
		std::uniform_real_distribution<int> disF(_Min, _Max);
		return disF(tMT);*/


		int Min = _Min;
		int Max = _Max;
		if (Min > Max)
		{
			int Tmp = Min;
			Min = Max;
			Max = Tmp;
		}

		std::uniform_int_distribution<int> disF = std::uniform_int_distribution<int>(Min, Max);
		return disF(MT);
	}

	static int random_int(const int& _Max)
	{

		/*if (0 > _Max)
		{
			BBY;
		}
		std::random_device tRD;
		std::mt19937 tMT(tRD());
		std::uniform_real_distribution<int> disF(0, _Max);
		return disF(tMT);*/


		int V = _Max;
		if (0 > V)
		{
			V *= -1;
		}

		std::uniform_int_distribution<int> disF = std::uniform_int_distribution<int>(0, V);
		return disF(MT);
	}

	static const bool random_bool(const float& _Value)
	{
		float V = _Value;
		std::bernoulli_distribution BernD = std::bernoulli_distribution(V);
		return BernD(MT);
	}
	
#pragma endregion

#pragma region Collision 2D
	static bool RectToRect(const KRect& _Left, const KRect& _Right)
	{ 
		if (_Left.left() > _Right.right())
		{
			return false;
		}

		if (_Left.right() < _Right.left())
		{
			return false;
		}

		if (_Left.top() < _Right.bottom())
		{
			return false;
		}

		if (_Left.bottom() > _Right.top())
		{
			return false;
		}

		return true; 
	}
	static bool RectToPoint(const KRect& _Left, const KPoint& _Right) 
	{
		if (_Left.left() > _Right.x)
		{
			return false;
		}

		if (_Left.right() < _Right.x)
		{
			return false;
		}

		if (_Left.top() < _Right.y)
		{
			return false;
		}

		if (_Left.bottom() > _Right.y)
		{
			return false;
		}

		return true;
	}

	static bool CirCleToCirCle(const KCircle& _Left, const KCircle& _Right)
	{
		if (_Left.Radius + _Right.Radius > (_Left.m_Pos - _Right.m_Pos).length())
		{
			return true;
		}
		return false;
	}
	static bool CirCleToPoint(const KCircle& _Left, const KPoint& _Right)
	{
		if (_Left.Radius > (_Left.m_Pos - _Right).length())
		{
			return true;
		}
		return false;
	}

	static KRect GKRect;
	static KRect GWRECT;
	static KCircle GCILCLE;
	static bool RectToCirCle(const KRect& _Left, const KCircle& _Right)
	{
		GKRect = _Left;
		GKRect.m_Size.y += _Right.diameter();
		GWRECT = _Left;
		GWRECT.m_Size.x += _Right.diameter();
		if (true == RectToPoint(GKRect, _Right.m_Pos))
		{
			return true;
		}

		if (true == RectToPoint(GWRECT, _Right.m_Pos))
		{
			return true;
		}

		KVector2 Rt = _Left.right_top();
		Rt -= _Left.m_Pos;
		GCILCLE = _Right;
		GCILCLE.m_Pos -= _Left.m_Pos;
		GCILCLE.m_Pos.x = fabsf(GCILCLE.m_Pos.x);
		GCILCLE.m_Pos.y = fabsf(GCILCLE.m_Pos.y);

		if (true == CirCleToPoint(GCILCLE, Rt))
		{
			return true;
		}

		return false;
	}
#pragma endregion


	static bool SphereToSphere(const DirectX::BoundingSphere& _Left, const DirectX::BoundingSphere& _Right)
	{
		return _Left.Intersects(_Right);
	}

	static bool SphereToRay(const DirectX::BoundingSphere& _Left, KVector _Ori, KVector _Dir, float& _dist)
	{
		float Tmp = _dist;
		bool TT = _Left.Intersects(_Ori, _Dir, _dist);
		return TT;
	}



	static bool OBBToOBB(const DirectX::BoundingOrientedBox& _Left, const DirectX::BoundingOrientedBox& _Right)
	{
		return _Left.Intersects(_Right);
	}

	static bool OBBToRay(const DirectX::BoundingOrientedBox& _Left, KVector _Ori, KVector _Dir, float& _dist)
	{
		float Tmp = _dist;
		bool TT = _Left.Intersects(_Ori, _Dir, _dist);
		return TT;
	}



	static bool PlaneToRay(const KMatrix& _Left, KVector _Ori, KVector _Dir, float& _dist)
	{
		bool TT = false;

		TT = DirectX::TriangleTests::Intersects(_Ori, _Dir, _Left.v1, _Left.v2, _Left.v3, _dist);
		if (false != TT)
		{
			return TT;
		}
	
		TT = DirectX::TriangleTests::Intersects(_Ori, _Dir, _Left.v3, _Left.v4, _Left.v2, _dist);
		if (false != TT)
		{
			return TT;
		}
		return TT;
	}



	static KVector Calc_ColPoint(const KVector& _Ori, const  KVector& _Dir, const float& _dist)
	{
		return KVector(_Dir * _dist + _Ori);
	}

	// Plane
	static KVector2 PostoUV2_XY(const KVector& _Src, KPtr<TransPosition> _Target, const KVector& _Expand = 1.0f);
	static KVector2 PostoUV2_XZ(const KVector& _Src, KPtr<TransPosition> _Target, const KVector& _Expand = 1.0f);
	
	// XY
	static KMatrix EdgeOfPlane_XY(KPtr<TransPosition> _Target);
	static KMatrix EdgeOfPlane_XZ(KPtr<TransPosition> _Target);



	static float DegToRad(const float& _Value)
	{
		return _Value * KPI / 180.0f;
	}

	static float RadToDeg(const float& _Value)
	{
		return _Value * 180.0f / KPI;
	}


public:
	KMath();
	~KMath();
};

