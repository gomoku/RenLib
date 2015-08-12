// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__72EC9D49_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
#define AFX_MAINFRM_H__72EC9D49_F85E_11D3_92A3_947F7AC2F525__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:
	void SetPaneText(const CString& text);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CToolBar    m_wndFileBar;
	CToolBar    m_wndMoveBar;
	CToolBar    m_wndPositionBar;
	CToolBar    m_wndEditBar;
	CToolBar    m_wndFindBar;
	CStatusBar	m_wndStatusBar;

private:
	static bool    sIsPaneTextAllowed;
	static CString sPaneText;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateVar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMark(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMouse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateText(CCmdUI* pCmdUI);
	afx_msg void OnFileBar();
	afx_msg void OnMoveBar();
	afx_msg void OnUpdateFileBar(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveBar(CCmdUI* pCmdUI);
	afx_msg void OnPositionBar();
	afx_msg void OnUpdatePositionBar(CCmdUI* pCmdUI);
	afx_msg void OnEditBar();
	afx_msg void OnUpdateEditBar(CCmdUI* pCmdUI);
	afx_msg void OnFindBar();
	afx_msg void OnUpdateFindBar(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf);
	BOOL isVisible(const CToolBar& Bar);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__72EC9D49_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
