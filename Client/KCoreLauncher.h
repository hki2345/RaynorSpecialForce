#pragma once

// 내가 만든 프로젝트를 라이브러리로 하여 불러오려면
// 1. 그 프로젝트를 참조에 추가하고
// 2. 라이브러리 경로를 추가 -> 프로젝트 -> 속성 -> V/C++ 포함디렉터리에 ..\식으로
// 원하는 라이브러리의 경로를 추가 -> 물론 같은 솔루션 내에 있을 때에만 ..\식을 쓰는 것이겠다.
// 3. 마지막으로 라이브러리가 되길 원하는 프로젝트를 라이브러리화(?) 시켜야한다.
// -> 라이브러리 되고자 하는 프로젝트 -> 속성 -> 일반 -> 구성형식 -> 라이브러리 형식


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
