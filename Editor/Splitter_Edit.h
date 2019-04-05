#pragma once

class Splitter_Edit : public CSplitterWnd
{
private:
	bool m_bLock;

public:
	void Lock() { m_bLock = true; }
	void UnLock() { m_bLock = false; }

	int GetBoderSizeY() { return  m_cyBorder; }

public:
	Splitter_Edit();
	~Splitter_Edit();

private:
	BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLButtonDown(UINT nFlages, CPoint point);
	afx_msg void OnMouseMove(UINT nFlages, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* _pWnd, UINT _nHitTest, UINT _Msg);
};

