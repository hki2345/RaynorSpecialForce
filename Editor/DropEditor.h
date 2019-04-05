#pragma once
#include <functional>

class DropEditor : public CEdit
{

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg);

public:
	bool m_bStart;
	float* pValue;
	CDialogEx* Parent;
	int CurX;
	std::function<void()> m_ValueChangeFunc;

public:
	template<typename T>
	void ValueChangeFunc(T* _Obj, void(T::*_PTR)())
	{
		if (nullptr == _PTR) { return; }
		if (nullptr == _Obj) { return; }
		m_ValueChangeFunc = std::bind(_PTR, _Obj);
	}

public:
	DropEditor();
	~DropEditor();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

