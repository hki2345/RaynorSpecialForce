#pragma once
#include "KShader.h"

// �ȼ��� ���ؽ��� ���� �и��Ǿ� �ִµ�
// ���߿� ���� �ҋ� ������ �϶��
class Shader_Pixel : public KShader
{
private:
	ID3D11PixelShader* m_pShader;

public:
	// �Լ� �̸� -> char�� �޴´�. GPU���� \
	// �ڿ� �ΰ��� UINT�� ���� ����
	bool Load(const char* _FuncName, UINT _VH = 5, UINT _VL = 0);
	void Update() override;

private:
	void UpdateCB(KPtr<ConstBuffer> _Buf);

public:
	Shader_Pixel();
	~Shader_Pixel();
};

