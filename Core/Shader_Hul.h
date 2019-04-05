#pragma once
#include "KShader.h"
class Shader_Hul :
	public KShader
{
private:
	ID3D11HullShader* m_pShader;

public:
	// 함수 이름 -> char로 받는다. GPU에서 \
		// 뒤에 두개의 UINT는 버젼 정보
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateCB(KPtr<ConstBuffer> _Buf);

public:
	Shader_Hul();
	~Shader_Hul();
};

