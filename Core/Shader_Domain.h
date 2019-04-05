#pragma once
#include "KShader.h"
class Shader_Domain : public KShader
{

private:
	ID3D11DomainShader* m_pShader;

public:
	// �Լ� �̸� -> char�� �޴´�. GPU���� \
		// �ڿ� �ΰ��� UINT�� ���� ����
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateCB(KPtr<ConstBuffer> _Buf);

public:
	Shader_Domain();
	~Shader_Domain();
};

