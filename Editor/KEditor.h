
// KEditor.h : AR14TOOL ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// KEditorApp:
// �� Ŭ������ ������ ���ؼ��� KEditor.cpp�� �����Ͻʽÿ�.
//

class KEditorApp : public CWinApp
{
public:
	KEditorApp();

public:
	void MyCurSor(HCURSOR _hCur);

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnIdle(LONG lCount);
};

extern KEditorApp theApp;
