#pragma once
#include "TabDlg.h"
#include "afxwin.h"

#include "DropEditor.h"

// Dlg_Terrain 대화 상자입니다.

class Renderer_Terrain;
class SC2_Force;
class SC2_Camera;
class Force_Unit;
class Dlg_Terrain : public TabDlg
{
	DECLARE_DYNAMIC(Dlg_Terrain)

public:
	Dlg_Terrain(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_Terrain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TERRAINDLG };
#endif

private:
	bool m_bGrab;

	KPtr<SC2_Camera>				m_pCam;
	KPtr<Renderer_Terrain>			m_pTer;
	KPtr<Force_Unit>				m_GrabUnit;
	KPtr<Force_Unit>				m_SelectUnit;

	KPtr<SC2_Force>					m_CurForce;
	KPtr<Force_Unit>				m_CurPlayer;

	std::list<KPtr<Force_Unit>>		m_UComList;

	CEdit NameEdit[3];
	float TerSSEdit[4];
	CEdit TexName;
	CListBox UBoxList;
	CButton m_TerBtn;
	CButton m_PBTBtn[3];
	int		m_PBTCheck;
	CComboBox m_FocusCombo;

	DropEditor	m_PosDrop[3];
	float		UnitPosEdit[3];


	DropEditor	m_RotDrop[3];
	float		UnitRotEdit[3];

	CButton m_PlayEditBtn;


protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	KPtr<Force_Unit> Create_Unit();
	KPtr<Force_Unit> Cur_Unit();

	void Update_SelectInfo();
	void Update_SSPosFunc();
	void Update_SSRotFunc();


	void Update_Force();
	void Update_UnitList();
	void Update_Terrain();
	void Update_Grab();
	void Udpate_Delete();
	void Update_Color();
	void Update_Combo();

	void Update_StayCol(KCollision* _Left, KCollision* _Right);


public:
	void Init_Dlg() override;
	void Update_Dlg() override;

	void Grab_On()
	{
		m_bGrab = true;
	}

	void Grab_Off()
	{
		m_bGrab = false;
	}

	void Create_Grab(const wchar_t* _Name);

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedTerclear();
	afx_msg void OnBnClickedTerload();
	afx_msg void OnBnClickedTersave();
	afx_msg void OnBnClickedTercovertex();
	afx_msg void OnBnClickedStateclear();
	afx_msg void OnBnClickedStateload();
	afx_msg void OnBnClickedStatesave();


	afx_msg void OnEditSelChanged(UINT _Id);
	afx_msg void OnTerInfoSelChanged(UINT _Id);
	afx_msg void OnUnitPosSelChanged(UINT _Id);
	afx_msg void OnUnitRotSelChanged(UINT _Id);
	afx_msg void UnitPlayableBtnchange(UINT _Id);
	afx_msg void OnBnClickedStatereslist();
	afx_msg void OnBnClickedTereditbtn();
	afx_msg void OnBnClickedTersetplay();
	afx_msg void OnCbnSelchangeForcelist();
};
