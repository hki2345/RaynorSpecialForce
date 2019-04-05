#pragma once
#include "TabDlg.h"
#include "afxwin.h"



// Dlg_KM3Loader 대화 상자입니다.

class Dlg_KM3Loader : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_KM3Loader)

public:
	Dlg_KM3Loader(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_KM3Loader();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KM3DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void Init_Dlg() override;
	void Hide_Dlg() override;
	void Update_RscTree();

	// 스레드
	unsigned int Load_KM3T(void* _args);


	// Animation
	void Init_AniName();
	void Init_AniClip();

	void Update_Combo(KPtr<Changer_Animation> _Value);
	void Update_Clip(KPtr<Changer_Animation::Ani_Clip> _Value);

	// Texture
	void Init_Path();
	void Update_Tex(const TEX_TYPE& _Value, const int& _Inx);
	std::wstring Show_DDSWindow();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedKm3load();
	afx_msg void OnLbnSelchangeKm3list();

	// 단발 애니
	int m_AniInx;

	// List
	int m_PrevSel;
	CListBox m_KM3List;
	KPtr<TheOne> m_CurOne;


	std::list<KPtr<TheOne>> m_OList;
	std::list<KPtr<TheOne>>::iterator m_SO;
	std::list<KPtr<TheOne>>::iterator m_EO;

	// Aniclip
	CEdit		m_CAName;
	CComboBox	m_ClipCombo;
	CEdit		m_ClipInfo[3];
	CButton		m_ClipAdd;
	CButton		m_ClipDelete;

	// Tex
	CButton m_TexBtn[4];
	CEdit	m_TexPath[4];

	afx_msg void OnBnClickedDiffbtnk();
	afx_msg void OnBnClickedNormbtnk();
	afx_msg void OnBnClickedSpecbtnk();
	afx_msg void OnBnClickedEmisbtnk();
	afx_msg void OnEnChangeKm3acstart();
	afx_msg void OnEnChangeKm3acend();
	afx_msg void OnEnChangeKm3acname();
	afx_msg void OnBnClickedKm3acaddbtn();
	afx_msg void OnBnClickedKm3acdelbtn();
	afx_msg void OnCbnSelchangeKm3accombo();
	afx_msg void OnBnClickedKm3acssave();
	afx_msg void OnBnClickedKm3acasave();
	afx_msg void OnBnClickedKm3resetbtn();
	afx_msg void OnBnClickedKm3playbtn();
	afx_msg void OnBnClickedKm3paubtn();
	afx_msg void OnBnClickedKm3prevbtn();
	afx_msg void OnBnClickedKm3nextbtn();
};
