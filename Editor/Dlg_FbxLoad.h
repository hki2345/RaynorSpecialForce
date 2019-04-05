#pragma once
#include "TabDlg.h"
#include "afxcmn.h"
#include "afxwin.h"



// Dlg_FbxLoad 대화 상자입니다.
class MeshContainer;
class Dlg_FbxLoad : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_FbxLoad)

public:
	Dlg_FbxLoad(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_FbxLoad();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FBXDLG };
#endif

private:
	CString pathName;
	;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.


	void Init_Dlg() override;
	void Hide_Dlg() override;
	void Update_RscTree();
	void Update_Path();
	void Update_Tex(const TEX_TYPE& _Value, const int& _Inx);
	

	std::wstring Show_DDSWindow();
public:
	unsigned int Load_ByThread(void* _Test);

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_RscList;
	KPtr<TheOne> m_CurOne;
	CButton m_TexBtn[4];
	CEdit	m_TexPath[4];

	afx_msg void OnBnClickedFbxloadbtn();
	afx_msg void OnLbnSelchangeFbxlist();
	afx_msg void OnBnClickedTokm2();



	afx_msg void OnBnClickedDiffbtn();
	afx_msg void OnBnClickedNormbtn();
	afx_msg void OnBnClickedSpecbtn();
	afx_msg void OnBnClickedEmisbtn();
	
	afx_msg void OnBnClickedListupbtn();
};
