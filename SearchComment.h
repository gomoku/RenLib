#if !defined(AFX_SEARCH_COMMENT_H__9FE3DCE5_179A_11D4_92A3_B960D61F6125__INCLUDED_)
#define AFX_SEARCH_COMMENT_H__9FE3DCE5_179A_11D4_92A3_B960D61F6125__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchComment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// SearchComment dialog

class SearchComment : public CDialog
{
// Construction
public:
	SearchComment(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SearchComment)
	enum { IDD = IDD_SEARCH_COMMENT };
	CString	m_strComment;
	int		mFindOption;
	//}}AFX_DATA
	CString  m_strFaceName;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SearchComment)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SearchComment)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCH_COMMENT_H__9FE3DCE5_179A_11D4_92A3_B960D61F6125__INCLUDED_)
