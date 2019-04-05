#pragma once
#include "afxcmn.h"
#include <vector>
#include <State.h>

#include <Core_Class.h>


#include "Edit_SUpdater.h"
#include "TabDlg.h"


// Dlg_MainMenu 대화 상자입니다.

class Dlg_MainMenu : public CDialogEx
{
	DECLARE_DYNAMIC(Dlg_MainMenu)

public:
	Dlg_MainMenu(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~Dlg_MainMenu();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MENUDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

public:
	CTabCtrl TabMenu;

	size_t m_CurIndex;

	//SceneDlg m_SceneDlg;
	//SpriteDlg m_SpriteDlg;

private:
	std::vector<TabDlg*> m_VecDlg;

	template<typename T>
	void CreateDlg(UINT _DlgId, CString _ItemName, bool _IsSceneCreate = true) 
	{
		UpdateData(TRUE);

		RECT Rc;
		TabMenu.GetClientRect(&Rc);
		TabMenu.InsertItem((int)m_VecDlg.size(), _ItemName);


		// Dlg를 만들떄 변수가 거짓으로 들어 오면
		// 해당 탭은 씬을 만들어주지 않는다. -> 업데이터 자체가 없다.
		if (true == _IsSceneCreate)
		{
			KPtr<State> m_Scene = Core_Class::MainSceneMgr().Find_State(_ItemName.GetString());
			if (nullptr == m_Scene)
			{
				m_Scene = Core_Class::MainSceneMgr().Create_State(_ItemName.GetString());
				m_Scene->CreateUpdater<Edit_SUpdater>();
				KPtr<TheOne> Cam = m_Scene->Create_One();
				KPtr<Camera> CamCom = Cam->Add_Component<Camera>();
				CamCom->Insert_LayerData(0);
			}
		}

		T* m_pDlg = new T();

		m_pDlg->StateName = _ItemName;
		m_pDlg->Create(_DlgId, this);
		m_pDlg->SetBackgroundColor(RGB(255, 255, 255), TRUE);
		m_pDlg->SetWindowPos(nullptr, 36, 18, Rc.right - 36, Rc.bottom - 18, SWP_NOZORDER);
		m_pDlg->ShowWindow(SW_HIDE);
		m_pDlg->m_StateChange = _IsSceneCreate;

		m_VecDlg.push_back(m_pDlg);

		UpdateData(FALSE);
	}

	void Show_Dlg(size_t _Index);

public:
	TabDlg* Find_Dlg(const wchar_t* _Name);

	void Update_Dlg();

public:
	afx_msg void OnTcnSelchangeMenutab(NMHDR *pNMHDR, LRESULT *pResult);

};

