#if !defined(AFX_MERGE_COMMENT_H__F6B9D182_F304_11D6_92A3_0000E89F396C__INCLUDED_)
#define AFX_MERGE_COMMENT_H__F6B9D182_F304_11D6_92A3_0000E89F396C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MergeComment.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// MergeComment dialog

class MergeComment : public CDialog
{
// Construction
public:
	enum Kind
	{
		ONE_LINE_COMMENT,
		MULTI_LINE_COMMENT,
		BOARD_TEXT
	};

	enum Selection
	{
		CURRENT,
		NEW,
		COMBINED
	};

public:
	MergeComment(const CString& currentComment, const CString& newComment, Kind kind);

	CString   getMergedComment();
    Selection getSelection();
    bool      getApplySelectionToAll();

// Dialog Data
	//{{AFX_DATA(MergeComment)
	enum { IDD = IDD_MERGE_ONELINE_COMMENT };
	CButton	mSelectCurrent;
	CButton	mSelectNew;
	CButton	mSelectCombined;
	CEdit	mNew;
	CEdit	mCurrent;
	CEdit	mCombinedComment;
	CButton	mApplyToAll;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MergeComment)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MergeComment)
	virtual BOOL OnInitDialog();
	afx_msg void OnCurrent();
	afx_msg void OnNew();
	afx_msg void OnCombined();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	Kind      mKind;
	CString   mCurrentComment;
	CString   mNewComment;
	Selection mSelection;
    bool      mApplySelectionToAll;

private:
	void updateControls();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MERGE_COMMENT_H__F6B9D182_F304_11D6_92A3_0000E89F396C__INCLUDED_)
