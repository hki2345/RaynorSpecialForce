
// UnityPro.h : UnityPro 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.


// UnityPro:
// 이 클래스의 구현에 대해서는 UnityPro.cpp을 참조하십시오.
//

class UnityPro : public CWinApp
{
private:
	enum PROD_TYPE
	{
		PT_CLIENT = 0,
		PT_EDITOR
	};

public:
	UnityPro();


// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 구현입니다.
private:
	void Product_Cpp(const wchar_t* _Path, const wchar_t* _Name, const PROD_TYPE& _Type);


public:
	DECLARE_MESSAGE_MAP()
};

extern UnityPro theApp;
