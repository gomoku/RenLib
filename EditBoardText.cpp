// EditBoardText.cpp : implementation file
//

#include "stdafx.h"
#include "renlib.h"
#include "EditBoardText.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditBoardText dialog


EditBoardText::EditBoardText(CWnd* pParent /*=NULL*/)
	: CDialog(EditBoardText::IDD, pParent)
{
	//{{AFX_DATA_INIT(EditBoardText)
	m_BoardText = _T("");
	//}}AFX_DATA_INIT
}


void EditBoardText::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditBoardText)
	DDX_Text(pDX, IDC_BOARD_TEXT, m_BoardText);
	DDV_MaxChars(pDX, m_BoardText, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditBoardText, CDialog)
	//{{AFX_MSG_MAP(EditBoardText)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditBoardText message handlers

int EditBoardText::DoModal() 
{
   CDialogTemplate dlt;
   int             nResult;

   // load dialog template
   if (!dlt.Load(MAKEINTRESOURCE(EditBoardText::IDD))) return -1;

   // set your own font, for example "Arial", 10 pts. 
   dlt.SetFont(m_strFaceName, 10);
	
   // get pointer to the modified dialog template
   LPSTR pdata = (LPSTR)GlobalLock(dlt.m_hTemplate);
	
   // let MFC know that you are using your own template
   m_lpszTemplateName = NULL;
   m_hDialogTemplate  = NULL;

   InitModalIndirect(pdata);

   // display dialog box
   nResult = CDialog::DoModal();

   // unlock memory object
   GlobalUnlock(dlt.m_hTemplate);

   return nResult;	
}
