#pragma once

#include "KMacro.h"

#include <d3d11.h> // �������
#include <d3dcompiler.h>// ���̴��� ��������

#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")
#pragma comment(lib, "dxguid")

#define KPI 3.14159265358979323846264338327950288f


class HMATHVAR
{
public:
	static const float DTOR;
	static const float RTOD;

public:
	// 0.0000001
	static bool EpSilonCheckf(float _Value) 
	{
		if (fabsf(_Value) < 0.0000001f)
		{
			return true;
		}

		return false;
	}

	static float EpSilonCutf(float& _Value)
	{
		if (fabsf(_Value) < 0.0000001f)
		{
			_Value = 0.0f;
		}

		return _Value;
	}

private:
	HMATHVAR() {}
	~HMATHVAR() {}
};

class BaseColor
{
public:
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;

};


// �浹ü���� ��
class int_compare
{
public:
	union 
	{
		struct 
		{
			int X;
			int Y;
		};

		struct
		{
			int Left;
			int Right;
		};

		__int64 m_i64;
	};

public:
	operator __int64() 
	{
		return *((__int64*)&X);
	}

public:
	int_compare(int _X, int  _Y) : X(_X), Y(_Y) {}
	int_compare(__int64 _Value) : m_i64(_Value) {  }
	~int_compare() {}
};


class KVector2
{

public:
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT2 m_XMVec2;
		float s[2];

		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
		struct
		{
			float x;
			float y;
		};

		struct
		{
			float m1;
			float m2;
		};

		struct
		{
			int ix;
			int iy;
		};

		__int64 m_i64;
	};

public:
	KVector2() : m_XMVec2(.0f, .0f) {}
	KVector2(__int64 _Value) : m_i64(_Value) {}
	KVector2(float _Value) : m_XMVec2(_Value, _Value) {}
	KVector2(float _x, float _y) : m_XMVec2(_x, _y) {}
	KVector2(int _x, int _y) : ix(_x), iy(_y) {}
	KVector2(const KVector2& _Other) : m_XMVec2(_Other.x, _Other.y) {}

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
	KVector2(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat2(&m_XMVec2, _Other); }


public:
	static const KVector2 Zero;
	static const KVector2 Left;
	static const KVector2 Right;
	static const KVector2 Up;
	static const KVector2 Down;


	void set_vector2(const float& _X, const float& _Y) { x = _X; y = _Y; }
	float x_part() const { return x * .5f; }
	float y_part() const { return y * .5f; }

	KVector2 IntToFloat() const
	{
		return KVector2((float)ix, (float)iy);
	}

	KVector2 FloatToInt() const
	{
		return KVector2(ix, iy);
	}

	KVector2 operator + (const KVector2& _Other)
	{
		return{ x + _Other.x, y + _Other.y };
	}

	KVector2& operator = (const KVector2& _Value)
	{
		m_XMVec2 = _Value.m_XMVec2;
		return *this;
	}


	KVector2 operator + (const KVector2& _Value) const
	{
		return KVector2(x + _Value.x, y + _Value.y);
	}
	KVector2 operator * (const KVector2& _Value) const
	{
		return KVector2(x * _Value.x, y * _Value.y);
	}
	KVector2 operator - (const KVector2& _Value) const
	{
		return KVector2(x - _Value.x, y - _Value.y);
	}
	KVector2 operator / (const KVector2& _Value) const
	{
		return KVector2(x / _Value.x, y / _Value.y);
	}

	KVector2 operator + (const float& _Value) const
	{
		return KVector2(x + _Value, y + _Value);
	}
	KVector2 operator * (const float& _Value)const
	{
		return KVector2(x * _Value, y * _Value);
	}
	KVector2 operator - (const float& _Value)const
	{
		return KVector2(x - _Value, y - _Value);
	}
	KVector2 operator / (const float& _Value)const
	{
		return KVector2(x / _Value, y / _Value);
	}



	KVector2& operator += (const KVector2& _Value)
	{
		x += _Value.x;
		y += _Value.y;

		return *this;
	}
	KVector2& operator -= (const KVector2& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;

		return *this;
	}
	KVector2& operator *= (const KVector2& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;

		return *this;
	}
	KVector2& operator /= (const KVector2& _Value)
	{
		x /= _Value.x;
		y /= _Value.y;

		return *this;
	}

	bool operator == (const KVector2& _Value)
	{
		return (x == _Value.x && y == _Value.y);
	}
	bool operator != (const KVector2& _Value)
	{
		return (x != _Value.x || y != _Value.y);
	}


	bool operator == (const KVector2& _Value) const
	{
		return (x == _Value.x && y == _Value.y);
	}
	bool operator != (const KVector2& _Value) const
	{
		return (x != _Value.x || y != _Value.y);
	}
	bool operator >= (const KVector2& _Value) const
	{
		return (x >= _Value.x && y >= _Value.y);
	}
	bool operator <= (const KVector2& _Value) const
	{
		return (x <= _Value.x && y <= _Value.y);
	}
	bool operator > (const KVector2& _Value) const
	{
		return (x > _Value.x && y > _Value.y);
	}
	bool operator < (const KVector2& _Value) const
	{
		return (x < _Value.x && y < _Value.y);
	}



	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat2(&m_XMVec2);
	}

	operator __int64() const
	{
		return m_i64;
	}


	float length() const
	{
		return sqrtf(x*x + y*y);
	}

	float distance() const
	{
		return sqrtf((x) * (x)+(y) * (y));
	}

	float distance(const KVector2& _Value) const
	{
		return sqrtf((x - _Value.x) * (x - _Value.x) + (y - _Value.y) * (y - _Value.y));
	}

	void normalize()
	{
		*this = DirectX::XMVector2Normalize(*this);
	}

	const float dot(const KVector2& _Left, const KVector2& _Right)
	{
		KVector2 Value = DirectX::XMVector2Dot(_Left, _Right);
		return Value.x;
	}

	float dot(const KVector2& _Other) const
	{
		return x * _Other.x + y * _Other.y;
	}

	KVector2& y_reverse()
	{
		y *= -1.0f;
		return *this;
	}

	float x_ratio() const
	{
		if (0 > y)
		{
			return .0f;
		}

		return x / y;
	}
	float y_ratio() const
	{
		if (0 > x)
		{
			return .0f;
		}

		return y / x;
	}


};
typedef KVector2 KPoint;




class KVector3
{
public:
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT3 m_XMVec3;

		float s[3];

		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
		struct
		{
			KVector2 m_Pos;
			float Radius;
		};

		struct
		{
			float x;
			float y;
			float z;
		};

		struct
		{
			float m1;
			float m2;
			float m3;
		};

		struct
		{
			KVector2 m_Vec2;
			float Value;
		};
	};

public:
	KVector3() : m_XMVec3(.0f, .0f, .0f) {}
	KVector3(float _Value) : m_XMVec3(_Value, _Value, _Value) {}
	KVector3(float _x, float _y, float _z) : m_XMVec3(_x, _y, _z) {}
	KVector3(const KVector2& _Other, float _z) : m_XMVec3(_Other.x, _Other.y, _z) {}
	KVector3(const KVector3& _Other) : m_XMVec3(_Other.x, _Other.y, _Other.z) {}

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
	KVector3(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat3(&m_XMVec3, _Other); }


public:
	static const KVector3 Zero;
	static const KVector3 Left;
	static const KVector3 Right;
	static const KVector3 Up;
	static const KVector3 Down;
	static const KVector3 Forword;
	static const KVector3 Back;
	static const KVector3 One; // ��������������


	void set_vector3(const float& _X, const float& _Y, const float& _Z) { x = _X; y = _Y; z = _Z; }
	float x_part() { return x * .5f; }
	float y_part() { return y * .5f; }
	float z_part() { return z * .5f; }

	KVector3 operator + (const KVector3& _Other)
	{
		return{ x + _Other.x, y + _Other.y , z + _Other.z };
	}

	KVector3& operator = (const KVector3& _Value)
	{
		m_XMVec3 = _Value.m_XMVec3;
		return *this;
	}

	KVector3& operator = (const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat3(&m_XMVec3, _Value);
		return *this;
	}
	KVector3 operator + (const KVector3& _Value) const
	{
		return KVector3(x + _Value.x, y + _Value.y, z + _Value.z);
	}
	KVector3 operator * (const KVector3& _Value) const
	{
		return KVector3(x * _Value.x, y * _Value.y, z * _Value.z);
	}
	KVector3 operator - (const KVector3& _Value) const
	{
		return KVector3(x - _Value.x, y - _Value.y, z - _Value.z);
	}
	KVector3 operator / (const KVector3& _Value) const
	{
		return KVector3(x / _Value.x, y / _Value.y, z / _Value.z);
	}

	KVector3 operator + (const float& _Value) const
	{
		return KVector3(x + _Value, y + _Value, z + _Value);
	}
	KVector3 operator * (const float& _Value)const
	{
		return KVector3(x * _Value, y * _Value, z * _Value);
	}
	KVector3 operator - (const float& _Value)const
	{
		return KVector3(x - _Value, y - _Value, z - _Value);
	}
	KVector3 operator / (const float& _Value)const
	{
		return KVector3(x / _Value, y / _Value, z / _Value);
	}



	KVector3& operator += (const KVector3& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		z += _Value.z;

		return *this;
	}
	KVector3& operator -= (const KVector3& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;
		z -= _Value.z;

		return *this;
	}
	KVector3& operator *= (const KVector3& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;
		z *= _Value.z;

		return *this;
	}
	KVector3& operator /= (const KVector3& _Value)
	{
		x /= _Value.x;
		y /= _Value.y;
		z /= _Value.z;

		return *this;
	}

	bool operator == (const KVector3& _Value) const
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z);
	}
	bool operator != (const KVector3& _Value) const
	{
		return (x != _Value.x && y != _Value.y && z != _Value.z);
	}


	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat3(&m_XMVec3);
	}

	const KVector3 dot(const KVector3& _Left, const KVector3& _Right)
	{
		KVector3 Value = DirectX::XMVector3Dot(_Left, _Right);
		return Value.x;
	}

	float dot(const KVector3& _Other) const
	{
		KVector3 Value = DirectX::XMVector3Dot(*this, _Other);
		return Value.x;
	}

	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector2& _Left, const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(_Left, _Right);
	}

	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector2& _Right) const
	{
		return DirectX::XMVector3Cross(*this, _Right);
	}


	// �翬�� ������ ��������.����������������
	KVector3 cross(const KVector3& _Right) const
	{
		KVector3 Temp = *this * KPI / 180;
		return DirectX::XMVector3Cross(Temp, _Right);
	}

	// 3���� ���� �Ÿ� ���� -- �������� ���� -> 3���� �� ��ǥ������ �Ÿ�
	float length() const
	{
		return sqrtf(x*x + y*y + z*z);
	}

	float diameter() const
	{
		return Radius * 2.0f;
	}

	KVector3& normalize()
	{
		return *this = DirectX::XMVector3Normalize(*this);
	}


	void Absolute()
	{
		x = fabsf(x); y = fabsf(y); z = fabsf(z);
	}
};
typedef KVector3 KCircle;






#define KRGBA(r, g, b, a) ( (COLORREF)(((BYTE)(r) | ((COLORREF)((BYTE)(g))<<8)) | ((COLORREF)(BYTE)(b))<<16 | ((COLORREF)(BYTE)(a))<<24 )
class KVector4
{
public:
	// �̺κ��� ��� ����� ���͸� �����ϸ鼭 ���Դ� �����Ӥ���
	// �̸� ���� ����ü -> ������� �ٷ� �߰�--- ����ü�� ���� �ƶ�
	union
	{
		// ���̷�ƮX ���� Vector3������ �����ϴ� ���� ������
		// �ۿ��ϴ� ���ʹ� �� ����(x, y, z, w)�� ��ȯ�Ѵ�.
		// ���� �� ���͸� ����� ������ �ִ� ����
		DirectX::XMFLOAT4 m_XMVec4;
		DirectX::XMFLOAT3 m_XMVec3;
		KVector2	m_Vec2;
		KVector3	m_Vec3;

		float s[4];


		// �̸����� ����ü -> ������� �ٷ� �߰��ؼ� ���ڰ��� ���ٰ���
		// �̳�Ŭ���� ������ ����ü�� �ƴ�
		struct
		{
			float x;
			float y;
			float z;
			float w;
		};

		struct
		{
			float m1;
			float m2;
			float m3;
			float m4;
		};

		struct
		{
			KVector2 m_Pos;
			KVector2 m_Size;
		};

		struct
		{
			KVector2 m_Front;
			KVector2 m_Back;
		};

		struct
		{
			float r;
			float g;
			float b;
			float a;
		};

		struct
		{
			int ir;
			int ig;
			int ib;
			int ia;
		};


		struct
		{
			unsigned int uint_r;
			unsigned int uint_g;
			unsigned int uint_b;
			unsigned int uint_a;
		};
	};


public:
	void Set_Color(
		unsigned int _uir,
		unsigned int _uig,
		unsigned int _uib,
		unsigned int _uia
	)
	{
		uint_r = _uir;
		uint_g = _uig;
		uint_b = _uib;
		uint_a = _uia;
	}

public:
	static const KVector4 Left;
	static const KVector4 Right;
	static const KVector4 Up;
	static const KVector4 Down;
	static const KVector4 Forword;
	static const KVector4 Back;
	static const KVector4 Zero;
	static const KVector4 One; // ��������������


	static const KVector4 Color_None;
	static const KVector4 White;
	static const KVector4 Black;
	static const KVector4 Red;
	static const KVector4 Green;
	static const KVector4 Blue;
	static const KVector4 Yellow;
	static const KVector4 Majenta;
	static const KVector4 Cyan;
	static const KVector4 Gray;
	static const KVector4 Brown;
	static const KVector4 Orange;


public:
	KVector4() : m_XMVec4(.0f, .0f, .0f, .0f) {}
	KVector4(float _Value) : m_XMVec4(_Value, _Value, _Value, _Value) {}
	KVector4(float _x, float _y) : m_XMVec4(_x, _y, .0f, .0f) {}
	KVector4(float _x, float _y, float _z) : m_XMVec4(_x, _y, _z, .0f) {}
	KVector4(float _x, float _y, float _z, float _w) : m_XMVec4(_x, _y, _z, _w) {}
	KVector4(float _x, float _y, const KVector2& _Vec) : m_Front(_x, _y), m_Back(_Vec) {}
	KVector4(const KVector4& _Other) : m_XMVec4(_Other.x, _Other.y, _Other.z, _Other.w) {}
	KVector4(const KVector2& _Other1, const KVector2& _Other2) : m_Front(_Other1), m_Back(_Other2) {}
	KVector4(const KVector3& _Other) : m_XMVec4(_Other.x, _Other.y, _Other.z, .0f) {}
	KVector4(const KVector2& _Other) : m_XMVec4(_Other.x, _Other.y, .0f, .0f) {}
	KVector4(const KVector2& _Other, const float& _V1) : m_XMVec4(_Other.x, _Other.y, _V1, .0f) {}
	KVector4(const KVector2& _Other, const float& _V1, const float& _V2) : m_XMVec4(_Other.x, _Other.y, _V1, _V2) {}
	KVector4(COLORREF& _Other)
	{
		BYTE* TT = (BYTE*)&_Other;

		r = ((float)*(TT + 0)) / 255.0f;
		g = ((float)*(TT + 1)) / 255.0f;
		b = ((float)*(TT + 2)) / 255.0f;
		a = ((float)*(TT + 3)) / 255.0f;
	}

	// ���Ϳ��� �׶���� �÷� 4���� ��Ʋ����� �� ���� ������ ���� �� �� �ְ� ���� �ƿ� ����ܰ迡�� ���� ����
	KVector4(const DirectX::XMVECTOR& _Other) { DirectX::XMStoreFloat4(&m_XMVec4, _Other); }


public:
	float distance() const
	{
		return sqrtf((x) * (x)+(y) * (y)+(z) * (z)+(w) * (w));
	}

	float distance(const KVector4& _Other) const
	{
		return sqrtf(
			(_Other.x - x) * (_Other.x - x)+
			(_Other.y - y) * (_Other.y - y)+
			(_Other.z - z) * (_Other.z - z)+
			(_Other.w - w) * (_Other.w - w));
	}

	static KVector4 cross3D(const KVector4& _Left, const KVector4& _Right)
	{
		KVector4 Temp = DirectX::XMVector3Cross(_Left, _Right);
		return Temp;
	}

	KVector4 cross3D(const KVector4& _Other)
	{
		KVector4 Temp = DirectX::XMVector3Cross(*this, _Other);
		return Temp;
	}

	KVector4 ElipseCut()
	{
		if (fabsf(x) < .00000001f)
		{
			x = .0f;
		}

		if (fabsf(y) < .00000001f)
		{
			y = .0f;
		}

		if (fabsf(z) < .00000001f)
		{
			z = .0f;
		}

		return KVector4(x, y, z);
	}

	float left() const { return m_Pos.x - m_Size.x_part(); }
	float right() const { return m_Pos.x + m_Size.x_part(); }
	float bottom() const { return m_Pos.y - m_Size.y_part(); }
	float top() const { return m_Pos.y + m_Size.y_part(); }

	KVector2 left_top() const { return{ left(), top() }; }
	KVector2 left_bottom() const { return{ left(), bottom() }; }
	KVector2 right_top() const { return{ right(), top() }; }
	KVector2 right_bottom() const { return{ right(), bottom() }; }

	float x_part() { return x * .5f; }
	float y_part() { return y * .5f; }
	float z_part() { return z * .5f; }
	float w_part() { return w * .5f; }

	UINT color_to_reverse255() const
	{
		return KRGBA((UINT)(r * 255), (UINT)(g * 255), (UINT)(b * 255), (UINT)(a * 255)));
	}

	KVector4& operator = (const KVector4& _Value)
	{
		m_XMVec4 = _Value.m_XMVec4;
		return *this;
	}

	KVector4& operator = (const KVector3& _Value)
	{
		m_Vec3 = _Value;
		return *this;
	}

	KVector4& operator = (const DirectX::XMVECTOR& _Value)
	{
		DirectX::XMStoreFloat4(&m_XMVec4, _Value);
		return *this;
	}

	KVector4& operator = (const float& _Value)
	{
		m_XMVec4 = { _Value, _Value, _Value, _Value };
		return *this;
	}

	KVector4 operator -()
	{
		return KVector4(-x, -y, -z, -w);
	}


	KVector4 operator + (const KVector4& _Value) const
	{
		return KVector4(x + _Value.x, y + _Value.y, z + _Value.z, w + _Value.w);
	}
	KVector4 operator * (const KVector4& _Value) const
	{
		return KVector4(x * _Value.x, y * _Value.y, z * _Value.z, w * _Value.w);
	}
	KVector4 operator - (const KVector4& _Value) const
	{
		return KVector4(x - _Value.x, y - _Value.y, z - _Value.z, w - _Value.w);
	}
	KVector4 operator / (const KVector4& _Value) const
	{
		KVector4 temp;

		if (0 != _Value.x) { temp.x = x / _Value.x; }
		else { temp.x; }
		if (0 != _Value.y) { temp.y = y / _Value.y; }
		else { temp.y; }
		if (0 != _Value.z) { temp.z = z / _Value.z; }
		else { temp.z; }
		if (0 != _Value.w) { temp.w = w / _Value.w; }
		else { temp.w; }

		return temp;
	}

	KVector4 operator + (const KVector2& _Value) const
	{
		return KVector4(x + _Value.x, y + _Value.y, z, w);
	}
	KVector4 operator - (const KVector2& _Value) const
	{
		return KVector4(x - _Value.x, y - _Value.y, z, w);
	}
	KVector4 operator / (const KVector2& _Value) const
	{
		KVector4 temp;

		if (0 != _Value.x) { temp.x = x / _Value.x; }
		else { temp.x; }
		if (0 != _Value.y) { temp.y = y / _Value.y; }
		else { temp.y; }

		return temp;
	}

	KVector4 operator + (const float& _Value) const
	{
		return KVector4(x + _Value, y + _Value, z + _Value);
	}
	KVector4 operator * (const float& _Value)const
	{
		return KVector4(x * _Value, y * _Value, z * _Value);
	}
	KVector4 operator - (const float& _Value)const
	{
		return KVector4(x - _Value, y - _Value, z - _Value);
	}
	KVector4 operator / (const float& _Value)const
	{
		return KVector4(x / _Value, y / _Value, z / _Value);
	}



	KVector4& operator += (const float& _Value)
	{
		x += _Value;
		y += _Value;
		z += _Value;
		w += _Value;

		return *this;
	}
	KVector4& operator -= (const float& _Value)
	{
		x -= _Value;
		y -= _Value;
		z -= _Value;
		w -= _Value;

		return *this;
	}
	KVector4& operator *= (const float& _Value)
	{
		x *= _Value;
		y *= _Value;
		z *= _Value;
		w *= _Value;

		return *this;
	}
	KVector4& operator /= (const float& _Value)
	{
		{ x /= _Value; }
		{ y /= _Value; }
		{ z /= _Value; }
		{ w /= _Value; }

		return *this;
	}


	KVector4& operator += (const KVector4& _Value)
	{
		x += _Value.x;
		y += _Value.y;
		z += _Value.z;
		w += _Value.w;

		return *this;
	}
	KVector4& operator -= (const KVector4& _Value)
	{
		x -= _Value.x;
		y -= _Value.y;
		z -= _Value.z;
		w -= _Value.w;

		return *this;
	}
	KVector4& operator *= (const KVector4& _Value)
	{
		x *= _Value.x;
		y *= _Value.y;
		z *= _Value.z;
		w *= _Value.w;

		return *this;
	}
	KVector4& operator /= (const KVector4& _Value)
	{
		{ x /= _Value.x; }
		{ y /= _Value.y; }
		{ z /= _Value.z; }
		{ w /= _Value.w; }

		return *this;
	}

	bool operator == (const KVector4& _Value) const
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z && w == _Value.w);
	}
	bool operator != (const KVector4& _Value) const
	{
		return (x != _Value.x || y != _Value.y || z != _Value.z || w != _Value.w);
	}
	bool operator >= (const KVector4& _Value) const
	{
		return (x == _Value.x && y == _Value.y && z == _Value.z && w == _Value.w);
	}
	bool operator <= (const KVector4& _Value) const
	{
		return (x != _Value.x || y != _Value.y || z != _Value.z || w != _Value.w);
	}

	operator KVector2() const
	{
		return{ m_XMVec4.x, m_XMVec4.y };
	}

	operator DirectX::XMVECTOR() const
	{
		return DirectX::XMLoadFloat4(&m_XMVec4);
	}

	KVector4 Compute_Quarternion()
	{
		KVector4 Temp = DirectX::XMQuaternionRotationRollPitchYaw(x, y, z);
		return Temp;
	}


public:
	static KVector4 Dot(const KVector4& _Left, const KVector4& _Right)
	{
		KVector4 Value = DirectX::XMVector4Dot(_Left, _Right);
		return Value.x;
	}

	float Dot(const KVector4& _Other) const
	{
		KVector4 Value = DirectX::XMVector4Dot(*this, _Other);
		return Value.x;
	}

	KVector4 Normalize()
	{
		*this = DirectX::XMVector4Normalize(*this);
		return *this;
	}

	void NormalizeVec3()
	{
		*this = DirectX::XMVector3Normalize(*this);
		w = .0f;
	}
	void NormalizeVec2()
	{
		*this = DirectX::XMVector2Normalize(*this);
		z = .0f;
		w = .0f;
	}

	KVector3 Vec3lize()
	{
		return KVector3(x, y, z);
	}
};

typedef KVector4 KColor;
typedef KVector4 KRect;
typedef KVector4 KVector;

struct ANIEFFCB
{
	KVector m_Color;
	KVector2 m_Start;
	KVector2 m_Size;
};


class KMatrix
{
public:
	union
	{
		DirectX::XMFLOAT4X4 m_Mat;

		struct 
		{
			KVector4 v1;
			KVector4 v2;
			KVector4 v3;
			KVector4 v4;
		};

		float m[4][4];


		float mrray[16];
	};

public:
	KMatrix() : v1(KVector::Zero), v2(KVector::Zero), v3(KVector::Zero), v4(KVector::Zero)
	{
	}

	
	KMatrix(const KVector& _V1, const KVector& _V2, const KVector& _V3, const KVector& _V4)
	{
		v1 = _V1;
		v2 = _V2;
		v3 = _V3;
		v4 = _V4;
	}


	KMatrix(const KMatrix& _Other)
	{
		memcpy_s(this, sizeof(KMatrix),  &_Other, sizeof(KMatrix));
	}

	KMatrix(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
	}

public:
	operator DirectX::XMMATRIX() const
	{
		return DirectX::XMLoadFloat4x4(&m_Mat);
	}

	KMatrix& operator=(const KMatrix& _Other)
	{
		memcpy_s(this, sizeof(KMatrix), &_Other, sizeof(KMatrix));
		return *this;
	}


	KMatrix& operator=(const DirectX::XMMATRIX& _Other)
	{
		DirectX::XMStoreFloat4x4(&m_Mat, _Other);
		return *this;
	}

	KMatrix operator*(const DirectX::XMMATRIX& _Other) const
	{
		return DirectX::XMMatrixMultiply(*this, _Other);
	}
	KMatrix operator*(const float& _Other) const
	{
		KMatrix TT;

		for (int i = 0; i < 16; i++)
		{
			TT.mrray[i] *= _Other;
		}

		return TT;
	}
	KMatrix operator-(const KMatrix& _Other) const
	{
		KMatrix TT;

		for (int i = 0; i < 16; i++)
		{
			TT.mrray[i] -= _Other.mrray[i];
		}

		return TT;
	}
public:
	KMatrix& Identity() 
	{
		*this = DirectX::XMMatrixIdentity();
		return *this;
	}

	void RotX(float _Value)
	{
		*this = DirectX::XMMatrixRotationX(_Value);
		return;
	}

	void RotY(float _Value)
	{
		*this = DirectX::XMMatrixRotationY(_Value);
		return;
	}

	void RotZ(float _Value)
	{
		*this = DirectX::XMMatrixRotationZ(_Value);
		return;
	}

	void Scale(const KVector4& _Scale)
	{
		*this = DirectX::XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z);
		return;
	}

	void Translate(const KVector4& _Pos)
	{
		*this = DirectX::XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);
		return;
	}

	void Transpose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return;
	}

	const KMatrix& RTranspose()
	{
		*this = DirectX::XMMatrixTranspose(*this);
		return *this;
	}

	KMatrix RVTranspose() const
	{
		KMatrix Mat = *this;
		Mat = DirectX::XMMatrixTranspose(Mat);
		return Mat;
	}

	void ViewAtLH(const KVector4& _EyePos, const KVector4& _LookPos, const KVector4& _UpDir)
	{
		*this = DirectX::XMMatrixLookAtLH(_EyePos, _LookPos, _UpDir);
		return;
	}

	void ViewToLH(const KVector4& _EyePos, const KVector4& _LookDir, const KVector4& _UpDir)
	{
		*this = DirectX::XMMatrixLookToLH(_EyePos, _LookDir, _UpDir);
		return;
	}

	// ��������
	// Near ������� �Ÿ�
	// _Far ������� �Ÿ���
	void PersLH(float _Fov, float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixPerspectiveFovLH(_Fov, _Width/ _Height, _Near, _Far);
		return;
	}

	void OrthLH(float _Width, float _Height, float _Near, float _Far)
	{
		*this = DirectX::XMMatrixOrthographicLH(_Width, _Height, _Near, _Far);
		return;
	}

	// ��ġ�Ӽ��� ����ǰ�
	KVector4 MulVecOne(const KVector4& _Vec) const
	{
		KMatrix CalMat = *this;
		return DirectX::XMVector3TransformCoord(_Vec, CalMat);
	}

	// ��ġ�Ӽ��� ������� �ʴ´�.
	KVector4 MulVecZero(const KVector4& _Vec) const
	{
		KMatrix CalMat;
		memcpy_s(&CalMat, sizeof(KMatrix), this, sizeof(KMatrix));
		return DirectX::XMVector3TransformNormal(_Vec, CalMat);
	}

	KMatrix& InverseRef()
	{
		*this = DirectX::XMMatrixInverse(nullptr, *this);
		return *this;
	}

	KMatrix InverseValue()
	{
		KMatrix TMX = *this;
		return TMX.InverseRef();
	}
};

class VS_Format
{
public:
	static KUINT Size_Format(DXGI_FORMAT _Fm);
};

class MatrixContainer 
{
public:
	KMatrix m_W;
	KMatrix m_V;
	KMatrix m_P;
	KMatrix m_WV;
	KMatrix m_VP;
	KMatrix m_WVP;
	KVector m_CamPos;

public:
	void RTrans() {
		m_W.RTranspose();
		m_V.RTranspose();
		m_P.RTranspose();
		m_WV.RTranspose();
		m_VP.RTranspose();
		m_WVP.RTranspose();
	}
};

// 3D�׷��� ī���� �������� �����޶�� �����ϴ� ��.
enum SHTYPE
{
	ST_NONE,
	ST_VS,
	ST_HUL,
	ST_DOM,
	ST_GEO,
	ST_PS,
};

