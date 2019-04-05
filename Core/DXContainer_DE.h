#pragma once

#include "DXContainer.h"

#define TypeS(TYPE) public: \
	static UINT TypeSize() { \
		return (UINT)sizeof(TYPE); \
	}

#define MemS(TYPE) public: \
	static UINT MemberSize() { \
		return (UINT)sizeof(TYPE); \
	}

class COLORVTX
{
public:
	KVector4 Pos;
	KVector4 Color;

	TypeS(COLORVTX)
};


class VTX3D
{
public:
	KVector4 Pos;
	KVector2 Uv;
	KVector4 Color;
	KVector4 Normal;
	KVector4 Tangent;
	KVector4 BNormal;

	TypeS(VTX3D)
};

class TEXVTX
{
public:
	KVector4 Pos;
	KVector2 Uv;

	TypeS(TEXVTX)
};

class IDX16
{
public:
	WORD _1, _2, _3;
	
	MemS(WORD)

	IDX16() : _1(0), _2(0), _3(0)
	{

	}

	IDX16(WORD __1, WORD __2, WORD __3) : _1(__1), _2(__2), _3(__3)
	{
	}

	static DXGI_FORMAT FM() { return DXGI_FORMAT_R16_UINT; }
};


class IDX32
{
public:
	union
	{
		struct
		{
			DWORD _1, _2, _3;
		};

		DWORD p[3];
	};

	MemS(DWORD)

	IDX32() : _1(0), _2(0), _3(0)
	{

	}

	IDX32(DWORD __1, DWORD __2, DWORD __3) : _1(__1), _2(__2), _3(__3)
	{
	}

	static DXGI_FORMAT FM() { return DXGI_FORMAT_R32_UINT; }
};