#if !defined(AFX_EDIT_BOARD_TEXT_H__879CA622_E2A1_11D5_92A3_0000E89F396C__INCLUDED_)
#define AFX_EDIT_BOARD_TEXT_H__879CA622_E2A1_11D5_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditBoardText.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// EditBoardText dialog

class EditBoardText : public CDialog
{
// Construction
public:
	EditBoardText(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(EditBoardText)
	enum { IDD = IDD_EDIT_BOARD_TEXT };
	CString	m_BoardText;
	//}}AFX_DATA
	CString  m_strFaceName;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EditBoardText)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(EditBoardText)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDIT_BOARD_TEXT_H__879CA622_E2A1_11D5_92A3_0000E89F396C__INCLUDED_)
