#pragma once

// ���� ���� ������Ʈ�� ���̺귯���� �Ͽ� �ҷ�������
// 1. �� ������Ʈ�� ������ �߰��ϰ�
// 2. ���̺귯�� ��θ� �߰� -> ������Ʈ -> �Ӽ� -> V/C++ ���Ե��͸��� ..\������
// ���ϴ� ���̺귯���� ��θ� �߰� -> ���� ���� �ַ�� ���� ���� ������ ..\���� ���� ���̰ڴ�.
// 3. ���������� ���̺귯���� �Ǳ� ���ϴ� ������Ʈ�� ���̺귯��ȭ(?) ���Ѿ��Ѵ�.
// -> ���̺귯�� �ǰ��� �ϴ� ������Ʈ -> �Ӽ� -> �Ϲ� -> �������� -> ���̺귯�� ����


#include <KCore.h>

class KCoreLauncher : public KCore::Launcher
{
public:
	void Build() override;

public:
	KCoreLauncher();
	~KCoreLauncher();
};

CORESTART(KCoreLauncher, L"RaynorSpecialForce", true);
