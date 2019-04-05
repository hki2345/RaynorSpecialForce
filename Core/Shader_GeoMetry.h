#pragma once
#include "KShader.h"




class Shader_GeoMetry : public KShader
{
private:
	ID3D11GeometryShader* m_pShader;

public:
	// 함수 이름 -> char로 받는다. GPU에서
	// 뒤에 두개의 UINT는 버젼 정보
	bool Load(const char* _FuncName, const KUINT& _VH = 5, const KUINT& _VL = 0);
	bool Load(const char* _FuncName, D3D11_SO_DECLARATION_ENTRY* _Data, const KUINT& _ENDTRYCNT, const KUINT* pBufferStrides
	, const KUINT& BufCnt, const KUINT& _StreamRS, const KUINT& _VH = 5, const KUINT& _VL = 0);
	void Update() override;

private:
	void UpdateCB(KPtr<ConstBuffer> _Buf);

public:
	Shader_GeoMetry();
	~Shader_GeoMetry();
};

