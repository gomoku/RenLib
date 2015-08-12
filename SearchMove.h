#if !defined(AFX_SEARCH_MOVE_H__86B35E02_DF1B_11D4_92A3_D19AF614AD25__INCLUDED_)
#define AFX_SEARCH_MOVE_H__86B35E02_DF1B_11D4_92A3_D19AF614AD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchMove.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SearchMove dialog

class SearchMove : public CDialog
{
// Construction
public:
	SearchMove(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SearchMove)
	enum { IDD = IDD_SEARCH_MOVE };
	CString	m_strMoveNo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SearchMove)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SearchMove)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCH_MOVE_H__86B35E02_DF1B_11D4_92A3_D19AF614AD25__INCLUDED_)
