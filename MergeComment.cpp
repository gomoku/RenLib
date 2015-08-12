// MergeComment.cpp : implementation file
//

#include "stdafx.h"
#include "renlib.h"
#include "MergeComment.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MergeComment dialog


namespace
{
	UINT getDialogId(MergeComment::Kind kind)
	{
		UINT id = 0;

		switch (kind)
		{
			case MergeComment::ONE_LINE_COMMENT:
				{
					id = IDD_MERGE_ONELINE_COMMENT;
					break;
				}
			case MergeComment::MULTI_LINE_COMMENT:
				{
					id = IDD_MERGE_MULTI_LINE_COMMENT;
					break;
				}
			case MergeComment::BOARD_TEXT:
				{
					id = IDD_MERGE_BOARD_TEXT;
					break;
				}
		}

		return id;
	}
}

MergeComment::MergeComment(const CString& currentComment, const CString& newComment, MergeComment::Kind kind)
: CDialog(getDialogId(kind), NULL),
  mKind(kind),
  mCurrentComment(currentComment),
  mNewComment(newComment),
  mSelection(CURRENT),
  mApplySelectionToAll(false)
{
	//{{AFX_DATA_INIT(MergeComment)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void MergeComment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MergeComment)
	DDX_Control(pDX, IDC_MERGE_ALL, mApplyToAll);
	DDX_Control(pDX, IDC_SELECT_CURRENT,  mSelectCurrent);
	DDX_Control(pDX, IDC_SELECT_NEW,      mSelectNew);
	DDX_Control(pDX, IDC_SELECT_COMBINED, mSelectCombined);
	DDX_Control(pDX, IDC_EDIT_NEW,        mNew);
	DDX_Control(pDX, IDC_EDIT_CURRENT,    mCurrent);
	DDX_Control(pDX, IDC_EDIT_COMBINED,   mCombinedComment);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MergeComment, CDialog)
	//{{AFX_MSG_MAP(MergeComment)
	ON_BN_CLICKED(IDC_SELECT_CURRENT,  OnCurrent)
	ON_BN_CLICKED(IDC_SELECT_NEW,      OnNew)
	ON_BN_CLICKED(IDC_SELECT_COMBINED, OnCombined)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MergeComment message handlers

BOOL MergeComment::OnInitDialog() 
{
	CDialog::OnInitDialog();

	mCurrent.SetWindowText(mCurrentComment);
	mNew.SetWindowText(mNewComment);

	CString strDelimiter(mKind == MULTI_LINE_COMMENT ? "\r\n" : " ");

	mCombinedComment.SetWindowText(mCurrentComment + strDelimiter + mNewComment);

	updateControls();
		
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

//----------------------------------------------------------------------------

void MergeComment::OnCurrent()
{
	mSelection = CURRENT;
	updateControls();
}

//----------------------------------------------------------------------------

void MergeComment::OnNew()
{
	mSelection = NEW;
	updateControls();
}

//----------------------------------------------------------------------------

void MergeComment::OnCombined()
{
	mSelection = COMBINED;
	updateControls();
}

//----------------------------------------------------------------------------

void MergeComment::updateControls()
{
	mSelectCurrent.SetCheck(mSelection == CURRENT);
	mSelectNew.SetCheck(mSelection == NEW);
	mSelectCombined.SetCheck(mSelection == COMBINED);

	mCurrent.EnableWindow(mSelectCurrent.GetCheck());
	mNew.EnableWindow(mSelectNew.GetCheck());
	mCombinedComment.EnableWindow(mSelectCombined.GetCheck());
}

//----------------------------------------------------------------------------

CString MergeComment::getMergedComment()
{
	return mCurrentComment;
}

//----------------------------------------------------------------------------

MergeComment::Selection
MergeComment::getSelection()
{
	return mSelection;
}

//----------------------------------------------------------------------------

bool MergeComment::getApplySelectionToAll()
{
	return mApplySelectionToAll;
}

//----------------------------------------------------------------------------

void MergeComment::OnOK() 
{
	CDialog::OnOK();

	if (mCurrent.IsWindowEnabled())
	{
		mCurrent.GetWindowText(mCurrentComment);
	}
	else if (mNew.IsWindowEnabled())
	{
		mNew.GetWindowText(mCurrentComment);
	}
	else if (mCombinedComment.IsWindowEnabled())
	{
		mCombinedComment.GetWindowText(mCurrentComment);
	}

    if (mApplyToAll.GetCheck() == 1)
    {
        mApplySelectionToAll = true;
    }
}
