
// UnityPro.h : UnityPro ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// UnityPro:
// �� Ŭ������ ������ ���ؼ��� UnityPro.cpp�� �����Ͻʽÿ�.
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


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
private:
	void Product_Cpp(const wchar_t* _Path, const wchar_t* _Name, const PROD_TYPE& _Type);


public:
	DECLARE_MESSAGE_MAP()
};

extern UnityPro theApp;
