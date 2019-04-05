// Dlg_TransPosition.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "KEditor.h"
#include "Dlg_TransPosition.h"
#include "afxdialogex.h"


// Dlg_TransPosition 대화 상자입니다.

IMPLEMENT_DYNAMIC(Dlg_TransPosition, ComDlg)

Dlg_TransPosition::Dlg_TransPosition(CWnd* pParent /*=NULL*/)
	: ComDlg(IDD_DLGTRANS, pParent), m_pTrans(nullptr)
{
	Y_SIZE = 180;
}

Dlg_TransPosition::~Dlg_TransPosition()
{
}

void Dlg_TransPosition::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	int StartId = IDC_POSX;

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			DDX_Control(pDX, StartId, ArrMatValue[Y][X]);
			DDX_Text(pDX, StartId, TransValue.m[Y][X]);
			++StartId;
		}
	}


	StartId = IDC_POSX2;

	// NameXYZ[0][0].ONSET

	for (size_t Y = 0; Y < 3; Y++)
	{
		for (size_t X = 0; X < 3; X++)
		{
			NameXYZ[Y][X].pValue = &TransValue.m[Y][X];
			NameXYZ[Y][X].Parent = this;
			NameXYZ[Y][X].ValueChangeFunc(this, &Dlg_TransPosition::Update_Trans);
			DDX_Control(pDX, StartId, NameXYZ[Y][X]);

			switch (X)
			{
			case 0:
				NameXYZ[Y][X].SetWindowTextW(L"X");
				break;
			case 1:
				NameXYZ[Y][X].SetWindowTextW(L"Y");
				break;
			case 2:
				NameXYZ[Y][X].SetWindowTextW(L"Z");
				break;
			default:
				break;
			}

			++StartId;
		}
	}
}


BEGIN_MESSAGE_MAP(Dlg_TransPosition, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_POSX, IDC_SCALZ, &Dlg_TransPosition::OnEnFocusTransValue)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_POSX, IDC_SCALZ, &Dlg_TransPosition::OnEnChangeTransValue)
END_MESSAGE_MAP()


// Dlg_TransPosition 메시지 처리기입니다.


void Dlg_TransPosition::OnEnFocusTransValue(UINT _Id) {
	int a = 0;
}

void Dlg_TransPosition::OnEnChangeTransValue(UINT _Id)
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// ComDlg::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	int Index = _Id - IDC_POSX;
	int IndexX = Index % 3;
	int IndexY = Index / 3;

	bool Check = false;

	// 짱나게 0 일때 데이터 최신화를 하면 커서가 앞으로 이동함 ㅋㅋㅋㅋ
	if (0.0f == TransValue.m[IndexY][IndexX])
	{
		Check = true;
		// ArrMatValue[IndexY][IndexX].SetSel();
	}


	UpdateData(TRUE);
	UpdateData(FALSE);

	if (true == Check)
	{
		ArrMatValue[IndexY][IndexX].SetSel(1, 1);
	}
	Update_Trans();

	// 
	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void Dlg_TransPosition::SettingComponent(Component* _pCom) 
{
	if (_pCom->TypeInfo() != &typeid(TransPosition))
	{
		return;
	}

	m_pTrans = (TransPosition*)_pCom;

	UpdateData(TRUE);


	KVector3 Temp;
	Temp = m_pTrans->pos_local().m_Vec3;
	memcpy(&TransValue.v1, &Temp, sizeof(KVector3));
	Temp = m_pTrans->rotate_local().m_Vec3;
	memcpy(&TransValue.v2, &Temp, sizeof(KVector3));
	Temp = m_pTrans->scale_local().m_Vec3;
	memcpy(&TransValue.v3, &Temp, sizeof(KVector3));

	UpdateData(FALSE);
}

void Dlg_TransPosition::Update_Trans() 
{

	m_pTrans->pos_local(TransValue.v1);
	m_pTrans->rotate_local(TransValue.v2);
	m_pTrans->scale_local(TransValue.v3);
}