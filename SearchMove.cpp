// SearchMove.cpp : implementation file
//

#include "stdafx.h"
#include "RenLib.h"
#include "SearchMove.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SearchMove dialog


SearchMove::SearchMove(CWnd* pParent /*=NULL*/)
	: CDialog(SearchMove::IDD, pParent)
{
	//{{AFX_DATA_INIT(SearchMove)
	m_strMoveNo = _T("");
	//}}AFX_DATA_INIT
}


void SearchMove::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SearchMove)
	DDX_Text(pDX, IDC_MOVE_NO, m_strMoveNo);
	DDV_MaxChars(pDX, m_strMoveNo, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SearchMove, CDialog)
	//{{AFX_MSG_MAP(SearchMove)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SearchMove message handlers
