#if !defined(AFX_AUTO_BOARD_TEXT_H__47FED263_ECD1_11D5_92A3_0000E89F396C__INCLUDED_)
#define AFX_AUTO_BOARD_TEXT_H__47FED263_ECD1_11D5_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoBoardText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// AutoBoardText dialog

class AutoBoardText : public CDialog
{
// Construction
public:
	AutoBoardText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(AutoBoardText)
	enum { IDD = IDD_AUTO_BOARD_TEXT };
	CString	m_NextAutoBoardText;
	//}}AFX_DATA
	CString	m_strFaceName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(AutoBoardText)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(AutoBoardText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTO_BOARD_TEXT_H__47FED263_ECD1_11D5_92A3_0000E89F396C__INCLUDED_)
