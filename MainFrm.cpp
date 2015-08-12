// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "RenLib.h"
#include "Utils.h"

#include "RenLibDoc.h"
#include "RenLibView.h"

#include "MainFrm.h"
#include "Registry.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

namespace
{
	const CString strSectionGeneral("General");
}

bool    CMainFrame::sIsPaneTextAllowed = false;
CString CMainFrame::sPaneText = "";


IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDW_FILE_BAR, OnFileBar)
	ON_COMMAND(IDW_MOVE_BAR, OnMoveBar)
	ON_UPDATE_COMMAND_UI(IDW_FILE_BAR, OnUpdateFileBar)
	ON_UPDATE_COMMAND_UI(IDW_MOVE_BAR, OnUpdateMoveBar)
	ON_COMMAND(IDW_POSITION_BAR, OnPositionBar)
	ON_UPDATE_COMMAND_UI(IDW_POSITION_BAR, OnUpdatePositionBar)
	ON_COMMAND(IDW_EDIT_BAR, OnEditBar)
	ON_UPDATE_COMMAND_UI(IDW_EDIT_BAR, OnUpdateEditBar)
	ON_COMMAND(IDW_FIND_BAR, OnFindBar)
	ON_UPDATE_COMMAND_UI(IDW_FIND_BAR, OnUpdateFindBar)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TEXT, OnUpdateText)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MOUSE,OnUpdateMouse)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MARK, OnUpdateMark)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_VAR,  OnUpdateVar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MOVE, OnUpdateMove)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_MODE, OnUpdateMode)
END_MESSAGE_MAP()

// toolbar buttons - IDs are command buttons
namespace
{
	static UINT BASED_CODE FileButtons[] =
	{
		// same order as in the bitmap 'toolbarfile.bmp'
		ID_FILE_NEW,
		ID_FILE_OPEN,
		ID_FILE_SAVE
	};

	static UINT BASED_CODE MoveButtons[] =
	{
		// same order as in the bitmap 'toolbarmove.bmp'
		ID_TOOLBAR_1ST_MOVE,
		ID_TOOLBAR_UP,
		ID_TOOLBAR_BACK,
		ID_TOOLBAR_FORWARD,
		ID_TOOLBAR_DOWN,
		ID_TOOLBAR_LAST_MOVE,
		ID_SEPARATOR,
		ID_TOOLBAR_PASS
	};

	static UINT BASED_CODE PositionButtons[] =
	{
		// same order as in the bitmap 'toolbarposition.bmp'
		ID_POSITION_0_DEGREES,
		ID_POSITION_90_DEGREES,
		ID_POSITION_180_DEGREES,
		ID_POSITION_270_DEGREES,
			ID_SEPARATOR,
		ID_POSITION_TURN_PLUS_90_DEGREES,
		ID_POSITION_TURN_MINUS_90_DEGREES,
			ID_SEPARATOR,
		ID_POSITION_REFLECT_LEFT_RIGHT,
		ID_POSITION_REFLECT_UP_DOWN
	};

	static UINT BASED_CODE EditButtons[] =
	{
		// same order as in the bitmap 'toolbaredit.bmp'
		ID_EDIT_UPDATE_MODE,
		ID_EDIT_START_POSITION,
		ID_EDIT_MARK
	};

	static UINT BASED_CODE FindButtons[] =
	{
		// same order as in the bitmap 'toolbarfind.bmp'
			ID_EDIT_FIND_COMMENT,
			ID_EDIT_FIND_MOVE,
			ID_EDIT_FIND_START_POSITION,
			ID_EDIT_FIND_MARK,
				ID_SEPARATOR,
			ID_EDIT_FIND_SAME_POSITION,
			ID_EDIT_FIND_SIMILAR_POSITION,
				ID_SEPARATOR,
			ID_TOOLBAR_FIND_PREVIOUS,
			ID_TOOLBAR_FIND_NEXT,
				ID_SEPARATOR,
			ID_EDIT_FIND_VCF,
			ID_EDIT_FIND_VCT,
			ID_EDIT_FIND_STOP
	};

	static UINT indicators[] =
	{
		ID_SEPARATOR,           // status line indicator
		ID_INDICATOR_TEXT,
		ID_INDICATOR_MOUSE,
		ID_INDICATOR_MARK,
		ID_INDICATOR_MOVE,
		ID_INDICATOR_VAR,
		ID_INDICATOR_MODE
	};
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndFileBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDW_FILE_BAR) ||
		!m_wndFileBar.LoadBitmap(IDR_TOOLBAR_FILE) ||
		!m_wndFileBar.SetButtons(FileButtons, sizeof(FileButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar file\n");
		return -1;      // fail to create
	}

	if (!m_wndEditBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDW_EDIT_BAR) ||
		!m_wndEditBar.LoadBitmap(IDR_TOOLBAR_EDIT) ||
		!m_wndEditBar.SetButtons(EditButtons, sizeof(EditButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar edit\n");
		return -1;      // fail to create
	}

	if (!m_wndFindBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDW_FIND_BAR) ||
		!m_wndFindBar.LoadBitmap(IDR_TOOLBAR_FIND) ||
		!m_wndFindBar.SetButtons(FindButtons, sizeof(FindButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar find\n");
		return -1;      // fail to create
	}

	if (!m_wndMoveBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDW_MOVE_BAR) ||
		!m_wndMoveBar.LoadBitmap(IDR_TOOLBAR_MOVE) ||
		!m_wndMoveBar.SetButtons(MoveButtons, sizeof(MoveButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar file\n");
		return -1;      // fail to create
	}

	if (!m_wndPositionBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_SIZE_DYNAMIC |
			CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY, IDW_POSITION_BAR) ||
		!m_wndPositionBar.LoadBitmap(IDR_TOOLBAR_POSITION) ||
		!m_wndPositionBar.SetButtons(PositionButtons, sizeof(PositionButtons)/sizeof(UINT)))
	{
		TRACE0("Failed to create toolbar position\n");
		return -1;      // fail to create
	}

	m_wndFileBar.SetWindowText(_T("File"));
	m_wndFileBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndEditBar.SetWindowText(_T("Edit"));
	m_wndEditBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndFindBar.SetWindowText(_T("Find"));
	m_wndFindBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndMoveBar.SetWindowText(_T("Move"));
	m_wndMoveBar.EnableDocking(CBRS_ALIGN_ANY);

	m_wndPositionBar.SetWindowText(_T("Position"));
	m_wndPositionBar.EnableDocking(CBRS_ALIGN_ANY);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndFileBar,AFX_IDW_DOCKBAR_TOP);

	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_wndFileBar,AFX_IDW_DOCKBAR_TOP);
	DockControlBarLeftOf(&m_wndEditBar,&m_wndFileBar);
	DockControlBarLeftOf(&m_wndMoveBar,&m_wndEditBar);
	DockControlBarLeftOf(&m_wndPositionBar,&m_wndMoveBar);
	DockControlBarLeftOf(&m_wndFindBar,&m_wndPositionBar);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	LoadBarState(strSectionGeneral);

	return 0;
}

void CMainFrame::DockControlBarLeftOf(CToolBar* Bar,CToolBar* LeftOf)
{
	CRect rect;
	DWORD dw;
	UINT n;

	// get MFC to adjust the dimensions of all docked ToolBars
	// so that GetWindowRect will be accurate
	RecalcLayout();
	LeftOf->GetWindowRect(&rect);
	rect.OffsetRect(1,0);
	dw=LeftOf->GetBarStyle();
	n = 0;
	n = (dw&CBRS_ALIGN_TOP           ) ? AFX_IDW_DOCKBAR_TOP    : n;
	n = (dw&CBRS_ALIGN_BOTTOM && n==0) ? AFX_IDW_DOCKBAR_BOTTOM : n;
	n = (dw&CBRS_ALIGN_LEFT   && n==0) ? AFX_IDW_DOCKBAR_LEFT   : n;
	n = (dw&CBRS_ALIGN_RIGHT  && n==0) ? AFX_IDW_DOCKBAR_RIGHT  : n;

	// When we take the default parameters on rect, DockControlBar will dock
	// each Toolbar on a seperate line.  By calculating a rectangle, we in effect
	// are simulating a Toolbar being dragged to that location and docked.
	DockControlBar(Bar,n,&rect);
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.x  = 0;
	cs.y  = 0;
	cs.cx = int (::GetSystemMetrics(SM_CXSCREEN) * 0.90);
	cs.cy = int (::GetSystemMetrics(SM_CYSCREEN) * 0.95);

    // Call the base-class version
    return CFrameWnd::PreCreateWindow(cs); 
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnUpdateMode(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();

	CRenLibDoc* pDoc = (CRenLibDoc*)GetActiveDocument();

	if (pDoc->isProblemMode())
	{
		pCmdUI->SetText(Utils::GetString(IDS_INFO_PROBLEM_MODE));
	}
	else if (pDoc->isUpdateMode())
	{
		pCmdUI->SetText(Utils::GetString(IDS_INFO_UPDATE_MODE));
	}
	else
	{
		pCmdUI->SetText(Utils::GetString(IDS_INFO_DISPLAY_MODE));
	}

	if (!sIsPaneTextAllowed)
	{
		sIsPaneTextAllowed = true;

		if (!sPaneText.IsEmpty())
		{
			SetPaneText(sPaneText);
		}
	}
}

void CMainFrame::OnUpdateMove(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();

	CRenLibDoc* pDoc = (CRenLibDoc*)GetActiveDocument();
	CString strMove;
	int currentMove = pDoc->GetCurrentMove();

	if (currentMove >= 0)
	{
		strMove = Utils::GetString(IDS_INFO_MOVE, currentMove);
	}

	pCmdUI->SetText(Utils::Center(strMove, 11));
}

void CMainFrame::OnUpdateVar(CCmdUI* pCmdUI)
{
	pCmdUI->Enable();

	CRenLibDoc* pDoc = (CRenLibDoc*)GetActiveDocument();
	CString strVar;

	if (pDoc->GetCurrentMove() >= 0)
	{
		strVar.Format("%d / %d", pDoc->NoOfVariantMoves(), pDoc->NoOfMovesInBranch());
	}

	pCmdUI->SetText(Utils::Center(strVar, 15));
}

void CMainFrame::OnUpdateMark(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc = (CRenLibDoc*)GetActiveDocument();
	if (pDoc->IsStartPosition())
	{
		pCmdUI->Enable();
		pCmdUI->SetText(Utils::GetString(IDS_INFO_START));
	}
	else if (pDoc->IsMark())
	{
		pCmdUI->Enable();
		pCmdUI->SetText(Utils::GetString(IDS_INFO_MARK));
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CMainFrame::OnUpdateMouse(CCmdUI* pCmdUI)
{
	CRenLibView* pView = (CRenLibView*)GetActiveView();

	pCmdUI->Enable();
	pCmdUI->SetText(pView->GetMouseCoord());
}

void CMainFrame::OnUpdateText(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc   = (CRenLibDoc*)GetActiveDocument();
	CRenLibView* pView = (CRenLibView*)GetActiveView();
	CString strText;

	pCmdUI->Enable();

	if (pDoc->GetCurrentMove() >= 0)
	{
		strText = pView->GetNextAutoBoardText();
	}

	pCmdUI->SetText(Utils::Center(strText, 5));
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	bool isHandled = false;

	if (pMsg->message == WM_RBUTTONDOWN)
	{
		CWnd* pWnd = CWnd::FromHandlePermanent(pMsg->hwnd);
		CControlBar* pBar = DYNAMIC_DOWNCAST(CControlBar, pWnd);

		if (pBar != NULL)
		{
			CMenu Menu;
			CPoint pt;

			pt.x = LOWORD(pMsg->lParam);
			pt.y = HIWORD(pMsg->lParam);
			pBar->ClientToScreen(&pt);

			if (Menu.LoadMenu(IDR_TOOLBAR_MENU))
			{
				CMenu* pSubMenu = Menu.GetSubMenu(0);

				if (pSubMenu!=NULL)
				{
					pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,pt.x,pt.y,this);
				}
			}
		}
	}
	else if (pMsg->message == WM_KEYDOWN)
	{
		CRenLibView* pView = (CRenLibView*)GetActiveView();
		isHandled = pView->handleKeyDown(pMsg->wParam, pMsg->lParam);
	}

	if (isHandled)
	{
		return TRUE;
	}
	else
	{
		return CFrameWnd::PreTranslateMessage(pMsg);
	}
}

void CMainFrame::OnFileBar() 
{
	ShowControlBar(&m_wndFileBar, !isVisible(m_wndFileBar), FALSE);
}

void CMainFrame::OnUpdateFileBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(isVisible(m_wndFileBar));
}

void CMainFrame::OnMoveBar() 
{
	ShowControlBar(&m_wndMoveBar, !isVisible(m_wndMoveBar), FALSE);
}

void CMainFrame::OnUpdateMoveBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(isVisible(m_wndMoveBar));
}

void CMainFrame::OnPositionBar() 
{
	ShowControlBar(&m_wndPositionBar, !isVisible(m_wndPositionBar), FALSE);
}

void CMainFrame::OnUpdatePositionBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(isVisible(m_wndPositionBar));
}

void CMainFrame::OnEditBar() 
{
	ShowControlBar(&m_wndEditBar, !isVisible(m_wndEditBar), FALSE);
}

void CMainFrame::OnUpdateEditBar(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(isVisible(m_wndEditBar));
}

void CMainFrame::OnFindBar() 
{
	ShowControlBar(&m_wndFindBar, !isVisible(m_wndFindBar), FALSE);
}

void CMainFrame::OnUpdateFindBar(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(isVisible(m_wndFindBar));
}

BOOL CMainFrame::isVisible(const CToolBar& Bar)
{
	return ((Bar.GetStyle() & WS_VISIBLE) != 0) ? TRUE : FALSE;
}

void CMainFrame::OnClose() 
{
	SaveBarState(strSectionGeneral);

	Registry reg;
	reg.setInt(Registry::IsZoomed, IsZoomed());
	
	CFrameWnd::OnClose();
}

void CMainFrame::SetPaneText(const CString& text)
{
	if (sIsPaneTextAllowed)
	{
		m_wndStatusBar.SetPaneText(0, text);
	}
	else
	{
		sPaneText = text;
	}
}
