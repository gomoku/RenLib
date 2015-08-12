// RenLibView.h : interface of the CRenLibView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENLIBVIEW_H__72EC9D4D_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
#define AFX_RENLIBVIEW_H__72EC9D4D_F85E_11D3_92A3_947F7AC2F525__INCLUDED_

#include "SearchComment.h"
#include "GCColorEdit.h"
#include "Font.h"
#include "ModeInformationI.h"
#include "DIBSectionLite.h"
#include "CISBitmap.h"
#include "webbrowser2.h"
#include <vector>

class Board;
class MoveNode;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
class CRenLibView : public CFormView, public ModeInformationI
{
protected: // create from serialization only
	CRenLibView();
	DECLARE_DYNCREATE(CRenLibView)

public:
	//{{AFX_DATA(CRenLibView)
	enum { IDD = IDD_RENLIB_FORM };
	CButton      m_Go;
	CListCtrl    m_Statistics;
	CGCColorEdit m_MultiLineComment;
	CTabCtrl	 m_Tab;
	CGCColorEdit m_OneLineComment;
	CComboBox	 m_Address;
	CWebBrowser2 m_Browser;
	//}}AFX_DATA

// Attributes
public:
	CRenLibDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRenLibView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRenLibView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CString	GetMouseCoord();
	CString	GetNextAutoBoardText();

	bool isReadOnly();

	bool handleKeyDown(UINT nChar, UINT lParam);
	void Invalidate(BOOL bErase = TRUE);

    void CheckVersionAndNews();

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRenLibView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnUpdateFileOpen(CCmdUI* pCmdUI);
	afx_msg void OnFileOpen();
	afx_msg void OnMoveBack();
	afx_msg void OnMoveDown();
	afx_msg void OnMoveUp();
	afx_msg void OnMoveForward();
	afx_msg void OnAppExit();
	afx_msg void OnFileSave();
	afx_msg void OnUpdateEditUpdateMode(CCmdUI* pCmdUI);
	afx_msg void OnEditUpdateMode();
	afx_msg void OnUpdateEditDeleteMove(CCmdUI* pCmdUI);
	afx_msg void OnEditDeleteMove();
	afx_msg void OnEditMark();
	afx_msg void OnUpdateEditMark(CCmdUI* pCmdUI);
	afx_msg void OnEditFindMark();
	afx_msg void OnUpdateEditFindMark(CCmdUI* pCmdUI);
	afx_msg void OnEditFindComment();
	afx_msg void OnUpdateEditFindComment(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveBack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveDown(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveForward(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMoveUp(CCmdUI* pCmdUI);
	afx_msg void OnViewSizeLarge();
	afx_msg void OnUpdateViewSizeLarge(CCmdUI* pCmdUI);
	afx_msg void OnViewSizeMedium();
	afx_msg void OnUpdateViewSizeMedium(CCmdUI* pCmdUI);
	afx_msg void OnViewSizeSmall();
	afx_msg void OnUpdateViewSizeSmall(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileNew(CCmdUI* pCmdUI);
	afx_msg void OnFileNew();
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnMove1stMove();
	afx_msg void OnUpdateMove1stMove(CCmdUI* pCmdUI);
	afx_msg void OnViewCoordinatesAo();
	afx_msg void OnUpdateViewCoordinatesAo(CCmdUI* pCmdUI);
	afx_msg void OnViewCoordinatesAp();
	afx_msg void OnUpdateViewCoordinatesAp(CCmdUI* pCmdUI);
	afx_msg void OnViewCoordinatesNone();
	afx_msg void OnUpdateViewCoordinatesNone(CCmdUI* pCmdUI);
	afx_msg void OnPosition0degrees();
	afx_msg void OnUpdatePosition0degrees(CCmdUI* pCmdUI);
	afx_msg void OnPosition180degrees();
	afx_msg void OnUpdatePosition180degrees(CCmdUI* pCmdUI);
	afx_msg void OnPosition270degrees();
	afx_msg void OnUpdatePosition270degrees(CCmdUI* pCmdUI);
	afx_msg void OnPosition90degrees();
	afx_msg void OnUpdatePosition90degrees(CCmdUI* pCmdUI);
	afx_msg void OnPositionReflectLeftRight();
	afx_msg void OnUpdatePositionReflectLeftRight(CCmdUI* pCmdUI);
	afx_msg void OnPositionReflectUpDown();
	afx_msg void OnUpdatePositionReflectUpDown(CCmdUI* pCmdUI);
	afx_msg void OnPositionTurnMinus90degrees();
	afx_msg void OnUpdatePositionTurnMinus90degrees(CCmdUI* pCmdUI);
	afx_msg void OnPositionTurnPlus90degrees();
	afx_msg void OnUpdatePositionTurnPlus90degrees(CCmdUI* pCmdUI);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnUpdateViewFontChooseFont(CCmdUI* pCmdUI);
	afx_msg void OnViewFontChooseFont();
	afx_msg void OnUpdateViewColorBoardChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardChooseColor();
	afx_msg void OnUpdateViewColorBoardDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardDefault();
	afx_msg void OnUpdateViewFontDefaultFont(CCmdUI* pCmdUI);
	afx_msg void OnViewFontDefaultFont();
	afx_msg void OnEditFindNext();
	afx_msg void OnUpdateEditFindNext(CCmdUI* pCmdUI);
	afx_msg void OnToolbarFindNext();
	afx_msg void OnUpdateToolbarFindNext(CCmdUI* pCmdUI);
	afx_msg void OnViewShowNumbers();
	afx_msg void OnUpdateViewShowNumbers(CCmdUI* pCmdUI);
	afx_msg void OnViewAutoPointMouse();
	afx_msg void OnUpdateViewAutoPointMouse(CCmdUI* pCmdUI);
	afx_msg void OnViewShowBoardText();
	afx_msg void OnUpdateViewShowBoardText(CCmdUI* pCmdUI);
	afx_msg void OnViewBoardTextExtraSpace();
	afx_msg void OnUpdateViewBoardTextExtraSpace(CCmdUI* pCmdUI);
	afx_msg void OnViewShowComments();
	afx_msg void OnUpdateViewShowComments(CCmdUI* pCmdUI);
	afx_msg void OnViewShowLastMove();
	afx_msg void OnUpdateViewShowLastMove(CCmdUI* pCmdUI);
	afx_msg void OnViewShowVariants();
	afx_msg void OnUpdateViewShowVariants(CCmdUI* pCmdUI);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnEditClearAllMarks();
	afx_msg void OnUpdateEditClearAllMarks(CCmdUI* pCmdUI);
	afx_msg void OnEditStartPosition();
	afx_msg void OnUpdateEditStartPosition(CCmdUI* pCmdUI);
	afx_msg void OnEditFindStartPosition();
	afx_msg void OnUpdateEditFindStartPosition(CCmdUI* pCmdUI);
	afx_msg void OnMoveLastMove();
	afx_msg void OnUpdateMoveLastMove(CCmdUI* pCmdUI);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnUpdateViewSizeDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewSizeDefault();
	afx_msg void OnEditFindMove();
	afx_msg void OnUpdateEditFindMove(CCmdUI* pCmdUI);
	afx_msg void OnViewCoordinatesSwapNumber();
	afx_msg void OnUpdateViewCoordinatesSwapNumber(CCmdUI* pCmdUI);
	afx_msg void OnViewShowForbiddenPoints();
	afx_msg void OnUpdateViewShowForbiddenPoints(CCmdUI* pCmdUI);
	afx_msg void OnEditFindPrevious();
	afx_msg void OnUpdateEditFindPrevious(CCmdUI* pCmdUI);
	afx_msg void OnToolbarFindPrevious();
	afx_msg void OnUpdateToolbarFindPrevious(CCmdUI* pCmdUI);
	afx_msg void OnEditFindSamePosition();
	afx_msg void OnUpdateEditFindSamePosition(CCmdUI* pCmdUI);
	afx_msg void OnEditFindSimilarPosition();
	afx_msg void OnUpdateEditFindSimilarPosition(CCmdUI* pCmdUI);
	afx_msg void OnViewAutoFindSamePosition();
	afx_msg void OnUpdateViewAutoFindSamePosition(CCmdUI* pCmdUI);
	afx_msg void OnViewShowAutoLinkVariants();
	afx_msg void OnUpdateViewShowAutoLinkVariants(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewColorVcfColorChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVcfColorChooseBlack();
	afx_msg void OnUpdateViewColorVcfColorDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVcfColorDefault();
	afx_msg void OnUpdateViewColorVcfColorChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVcfColorChooseWhite();
	afx_msg void OnUpdateViewColorVctColorChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVctColorChooseBlack();
	afx_msg void OnUpdateViewColorVctColorDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVctColorDefault();
	afx_msg void OnUpdateViewColorVctColorChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVctColorChooseWhite();
	afx_msg void OnUpdateViewColorAutoLinkVariantColorChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorAutoLinkVariantColorChooseBlack();
	afx_msg void OnUpdateViewColorAutoLinkVariantColorDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorAutoLinkVariantColorDefault();
	afx_msg void OnUpdateViewColorAutoLinkVariantColorChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorAutoLinkVariantColorChooseWhite();
	afx_msg void OnUpdateViewColorChangedOrderOfMovesVariantColorChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorChangedOrderOfMovesVariantColorChooseBlack();
	afx_msg void OnUpdateViewColorChangedOrderOfMovesVariantColorDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorChangedOrderOfMovesVariantColorDefault();
	afx_msg void OnUpdateViewColorChangedOrderOfMovesVariantColorChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorChangedOrderOfMovesVariantColorChooseWhite();
	afx_msg void OnViewShowChangedOrderOfMovesVariants();
	afx_msg void OnUpdateViewShowChangedOrderOfMovesVariants(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewColorLastMoveChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorLastMoveChooseColor();
	afx_msg void OnUpdateViewColorLastMoveDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorLastMoveDefault();
	afx_msg void OnUpdateViewColorStoneChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneChooseBlack();
	afx_msg void OnUpdateViewColorStoneChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneChooseWhite();
	afx_msg void OnUpdateViewColorStoneDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneDefault();
	afx_msg void OnUpdateViewColorVariantChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVariantChooseBlack();
	afx_msg void OnUpdateViewColorVariantChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVariantChooseWhite();
	afx_msg void OnUpdateViewColorVariantDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorVariantDefault();
	afx_msg void OnMovePass();
	afx_msg void OnUpdateMovePass(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewColorCommentBackground(CCmdUI* pCmdUI);
	afx_msg void OnViewColorCommentBackground();
	afx_msg void OnUpdateViewColorCommentDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorCommentDefault();
	afx_msg void OnUpdateViewColorCommentText(CCmdUI* pCmdUI);
	afx_msg void OnViewColorCommentText();
	afx_msg void OnUpdateViewColorCoordinateChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorCoordinateChooseColor();
	afx_msg void OnUpdateViewColorCoordinateDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorCoordinateDefault();
	afx_msg void OnUpdateViewColorBoardTextChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardTextChooseColor();
	afx_msg void OnUpdateViewColorBoardTextDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardTextDefault();
	afx_msg void OnUpdateViewColorStoneNumberChooseBlack(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneNumberChooseBlack();
	afx_msg void OnUpdateViewColorStoneNumberChooseWhite(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneNumberChooseWhite();
	afx_msg void OnUpdateViewColorStoneNumberDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorStoneNumberDefault();
	afx_msg void OnUpdateViewColorBackgroundChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBackgroundChooseColor();
	afx_msg void OnUpdateViewColorBackgroundDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBackgroundDefault();
	afx_msg void OnUpdateViewColorAllDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorAllDefault();
	afx_msg void OnUpdateViewColorForbiddenChooseColor(CCmdUI* pCmdUI);
	afx_msg void OnViewColorForbiddenChooseColor();
	afx_msg void OnUpdateViewColorForbiddenDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewColorForbiddenDefault();
	afx_msg void OnPaint();
	afx_msg void OnEditDeleteVariants();
	afx_msg void OnUpdateEditDeleteVariants(CCmdUI* pCmdUI);
	afx_msg void OnViewCoordinatesFollowRotationAndReflection();
	afx_msg void OnUpdateViewCoordinatesFollowRotationAndReflection(CCmdUI* pCmdUI);
	afx_msg void OnToolbarBack();
	afx_msg void OnUpdateToolbarBack(CCmdUI* pCmdUI);
	afx_msg void OnToolbarDown();
	afx_msg void OnUpdateToolbarDown(CCmdUI* pCmdUI);
	afx_msg void OnToolbarUp();
	afx_msg void OnUpdateToolbarUp(CCmdUI* pCmdUI);
	afx_msg void OnToolbarForward();
	afx_msg void OnUpdateToolbarForward(CCmdUI* pCmdUI);
	afx_msg void OnToolbar1stMove();
	afx_msg void OnUpdateToolbar1stMove(CCmdUI* pCmdUI);
	afx_msg void OnToolbarLastMove();
	afx_msg void OnUpdateToolbarLastMove(CCmdUI* pCmdUI);
	afx_msg void OnToolbarPass();
	afx_msg void OnUpdateToolbarPass(CCmdUI* pCmdUI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditFindVcf();
	afx_msg void OnUpdateFindVcf(CCmdUI* pCmdUI);
	afx_msg void OnEditFindVct();
	afx_msg void OnUpdateFindVct(CCmdUI* pCmdUI);
	afx_msg void OnEditFindStop();
	afx_msg void OnUpdateFindStop(CCmdUI* pCmdUI);
	afx_msg void OnFileSaveLibraryAs();
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnEditAutoEnterBoardText();
	afx_msg void OnUpdateEditAutoEnterBoardText(CCmdUI* pCmdUI);
	afx_msg void OnEditSetNextAutoBoardText();
	afx_msg void OnUpdateEditSetNextAutoBoardText(CCmdUI* pCmdUI);
	afx_msg void OnViewWin123();
	afx_msg void OnUpdateViewWin123(CCmdUI* pCmdUI);
	afx_msg void OnViewWinAbcLower();
	afx_msg void OnUpdateViewWinAbcLower(CCmdUI* pCmdUI);
	afx_msg void OnViewWinAbcUpper();
	afx_msg void OnUpdateViewWinAbcUpper(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewFontAllDefault(CCmdUI* pCmdUI);
	afx_msg void OnViewFontAllDefault();
	afx_msg void OnUpdateViewFontChooseFontBoardText(CCmdUI* pCmdUI);
	afx_msg void OnViewFontChooseFontBoardText();
	afx_msg void OnUpdateViewFontChooseFontComment(CCmdUI* pCmdUI);
	afx_msg void OnViewFontChooseFontComment();
	afx_msg void OnUpdateViewFontDefaultFontBoardText(CCmdUI* pCmdUI);
	afx_msg void OnViewFontDefaultFontBoardText();
	afx_msg void OnUpdateViewFontDefaultFontComment(CCmdUI* pCmdUI);
	afx_msg void OnViewFontDefaultFontComment();
	afx_msg void tabSelChange(NMHDR* pNotifyStruct, LRESULT* result);
	afx_msg void OnMergeCommentNext();
	afx_msg void OnMergeCommentPrevious();
	afx_msg void OnEditConvertToStandard();
	afx_msg void OnUpdateEditConvertToStandard(CCmdUI* pCmdUI);
	afx_msg void OnFileExportLibraryTextBoard();
	afx_msg void OnUpdateFileExportLibraryTextBoard(CCmdUI* pCmdUI);
	afx_msg void OnFileExportLibraryRenLibApplet();
	afx_msg void OnUpdateFileExportLibraryRenLibApplet(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOptionsEnvironmentAssociateLib(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateLib();
	afx_msg void OnUpdateOptionsEnvironmentAssociateWzq(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateWzq();
	afx_msg void OnUpdateOptionsEnvironmentAssociateBuf(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateBuf();
	afx_msg void OnUpdateOptionsEnvironmentAssociatePdb(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociatePdb();
	afx_msg void OnUpdateOptionsEnvironmentAssociatePos(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociatePos();
	afx_msg void OnUpdateOptionsEnvironmentAssociateRenjs(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateRenjs();
    afx_msg void OnUpdateOptionsEnvironmentAssociateSgf(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateSgf();
    afx_msg void OnUpdateOptionsEnvironmentAssociateBdt(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateBdt();	
    afx_msg void OnUpdateOptionsEnvironmentAssociateRdf(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateRdf();	
    afx_msg void OnUpdateOptionsEnvironmentAssociateAll(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEnvironmentAssociateAll();
	afx_msg void OnOneLineCommentUpdate();
	afx_msg void OnOneLineCommentSetFocus();
	afx_msg void OnOneLineCommentKillFocus();
	afx_msg void OnMultiLineCommentUpdate();
	afx_msg void OnMultiLineCommentSetFocus();
	afx_msg void OnMultiLineCommentKillFocus();
	afx_msg void OnAddressSetFocus();
	afx_msg void OnAddressKillFocus();
	afx_msg void OnAddressSelChange();
	afx_msg void OnOptionsEvaluatorRenjuClassRenjuSolver();
	afx_msg void OnUpdateOptionsEvaluatorRenjuClassRenjuSolver(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileMakeTournamentRenLibApplet(CCmdUI* pCmdUI);
	afx_msg void OnFileMakeTournamentRenLibApplet();
	afx_msg void OnUpdateViewColorBoardChooseBitmap(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardChooseBitmap();
	afx_msg void OnUpdateViewColorBoardDefaultBitmap(CCmdUI* pCmdUI);
	afx_msg void OnViewColorBoardDefaultBitmap();
	afx_msg void OnViewShowBoardBitmap();
	afx_msg void OnUpdateViewShowBoardBitmap(CCmdUI* pCmdUI);
	afx_msg void OnViewShowStoneBitmap();
	afx_msg void OnUpdateViewShowStoneBitmap(CCmdUI* pCmdUI);
	afx_msg void OnOptionsEvaluatorNone();
	afx_msg void OnUpdateOptionsEvaluatorNone(CCmdUI* pCmdUI);
	afx_msg void OnFileRead();
	afx_msg void OnUpdateFileRead(CCmdUI* pCmdUI);
	afx_msg void OnFileReadAll();
	afx_msg void OnUpdateFileReadAll(CCmdUI* pCmdUI);
	afx_msg void OnFileSavePositionAs();
	afx_msg void OnUpdateFileSavePositionAs(CCmdUI* pCmdUI);
	afx_msg void OnDocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnGo();
	afx_msg void OnUpdateBrowserOpenInDefaultBrowser(CCmdUI* pCmdUI);
	afx_msg void OnBrowserOpenInDefaultBrowser();
	afx_msg void OnUpdateBrowserOnlineRenjuclass(CCmdUI* pCmdUI);
	afx_msg void OnBrowserOnlineRenjuclass();
	afx_msg void OnUpdateBrowserRenjuNetPortal(CCmdUI* pCmdUI);
	afx_msg void OnBrowserRenjuNetPortal();
	afx_msg void OnUpdateBrowserRenLibHomepage1st(CCmdUI* pCmdUI);
	afx_msg void OnBrowserRenLibHomepage1st();
	afx_msg void OnUpdateBrowserRenLibHomepage2nd(CCmdUI* pCmdUI);
	afx_msg void OnBrowserRenLibHomepage2nd();
	afx_msg void OnBrowserRenLibNews();
	afx_msg void OnUpdateBrowserRenLibNews(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrowserRenLibUsersGuide(CCmdUI* pCmdUI);
	afx_msg void OnBrowserRenLibUsersGuide();
	afx_msg void OnUpdateBrowserRenjuInternationalFederation(CCmdUI* pCmdUI);
	afx_msg void OnBrowserRenjuInternationalFederation();
	afx_msg void OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel);
	afx_msg void OnUpdateBrowserForumOnlineRenjuclass(CCmdUI* pCmdUI);
	afx_msg void OnBrowserForumOnlineRenjuclass();
	afx_msg void OnUpdateBrowserStartpage(CCmdUI* pCmdUI);
	afx_msg void OnBrowserStartPage();
	afx_msg void OnUpdateBrowserUseCurrentPageAsStartpage(CCmdUI* pCmdUI);
	afx_msg void OnBrowserUseCurrentPageAsStartPage();
	afx_msg void OnBrowserOpenDiagrams();
	afx_msg void OnBrowserReadDiagrams();
	afx_msg void OnUpdateBrowserReadDiagrams(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBrowserOpenDiagrams(CCmdUI* pCmdUI);
	afx_msg void OnOpenWebPage();
	afx_msg void showCommentTab();
	afx_msg void OnNavigateComplete2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL);
	afx_msg void OnStatusTextChangeExplorer(LPCTSTR Text);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnUpdateFileExportLibraryGameCollection(CCmdUI* pCmdUI);
	afx_msg void OnFileExportLibraryGameCollection();
	afx_msg void OnOptionsAppletUrlLocal();
	afx_msg void OnUpdateOptionsAppletUrlLocal(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAppletUrlWeb();
	afx_msg void OnUpdateOptionsAppletUrlWeb(CCmdUI* pCmdUI);
	afx_msg void OnOptionsAppletGameCollectionShowBoard();
	afx_msg void OnUpdateOptionsAppletGameCollectionShowBoard(CCmdUI* pCmdUI);
	afx_msg void OnEditInsertPassMove();
	afx_msg void OnUpdateEditInsertPassMove(CCmdUI* pCmdUI);
	afx_msg void OnEditProblemMode();
	afx_msg void OnUpdateEditProblemMode(CCmdUI* pCmdUI);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	enum Coordinate
	{
		NONE,
		AO,
		AP
	};

	enum Position
	{
		DEGREE_0,
		DEGREE_90,
		DEGREE_180,
		DEGREE_270
	};

	enum BoardBitmapState
	{
		VALID_BITMAP,
		INVALID_BITMAP,
		DEFAULT_BITMAP,
		CHOOSE_BITMAP
	};

	enum TimerState
	{
		TIMER_VERSION_NEWS,
		TIMER_VCT_VCF
	};

private:
	bool          m_bInitialUpdate;

	CRect		  m_rectCircle;
	int			  m_BoardSize;
	Coordinate	  m_eCoordinate;
	bool		  m_bSwapRowNumber;
	bool		  m_bCoordinatesFollow;

	Position	  m_ePosition;
	bool		  m_bReflect_X;
	bool		  m_bReflect_Y;

	Position	  m_ePreviousPosition;
	bool		  m_bPreviousReflect_X;
	bool		  m_bPreviousReflect_Y;

	Font          m_Font[3];
	Font          m_FontComment[3];
	Font          m_FontBoardText[3];

	COLORREF	  m_BackgroundColor;
	COLORREF	  m_BoardColor;
	COLORREF	  m_BlackStoneColor;
	COLORREF	  m_WhiteStoneColor;
	COLORREF	  m_BlackVariantColor;
	COLORREF	  m_WhiteVariantColor;
	COLORREF	  m_LastMoveColor;
	COLORREF	  m_ForbiddenColor;
	COLORREF	  m_BlackVcfColor;
	COLORREF	  m_WhiteVcfColor;
	COLORREF	  m_BlackVctColor;
	COLORREF	  m_WhiteVctColor;
	COLORREF	  m_BlackALVColor;
	COLORREF	  m_WhiteALVColor;
	COLORREF	  m_BlackCMOColor;
	COLORREF	  m_WhiteCMOColor;
	COLORREF	  m_CommentTextColor;
	COLORREF	  m_CommentBkColor;
	COLORREF	  m_CoordTextColor;
	COLORREF	  m_BoardTextColor;
	COLORREF	  m_BlackNumberColor;
	COLORREF	  m_WhiteNumberColor;
	COLORREF	  m_CustColors[16];

	SearchComment m_SearchComment;
	int           m_SearchMoveNo;
	CString       m_strFindComment;

	bool		  m_bShowBoardBitmap;
	bool		  m_bShowStoneBitmap;
	bool		  m_bShowNumbers;
	bool		  m_bShowLastMove;
	bool		  m_bShowBoardText;
	bool		  m_bBoardTextExtraSpace;
	bool		  m_bShowComments;
	bool		  m_bShowVariants;
	bool		  m_bShowForbidden;
	bool		  m_bAutoPointMouse;
	bool		  m_bAutoFindSamePosition;
	bool		  m_bAutoEnterBoardText;
	bool		  m_bShowAutoLinkVariants;
	bool		  m_bShowChangedOrderOfMovesVariants;

	bool          m_bToolBarMoveCommand;

	CPoint        m_MouseCoord;

	CRect         m_rectBitmap;
	CDC*          m_pdcMemory;
	CBitmap*      m_pBitmap;

	CString       m_NextAutoBoardText;

	MoveNode*     m_LastMoveNode;

	Board::WinMarker m_eWinMarker;

	bool		  m_bOneLineCommentHasFocus;
	bool		  m_bMultiLineCommentHasFocus;
	bool		  m_bAddressHasFocus;

	CString       m_strOneLineComment;
	CString       m_strMultiLineComment;

	CString          m_strBoardBitmap;
	CDIBSectionLite  m_Board;
	BoardBitmapState m_BoardBitmapState;

	CCISBitmap      m_BlackStone;
	CCISBitmap      m_WhiteStone;

	int             m_StoneSize;

	std::vector<CString> mUrl;
	std::vector<CString> mCurrentUrl;
	CString              mStartPage;

    CTime      mStartTime;
	UINT       m_nTimer;
    TimerState mTimerState;

private:
	void	DrawBoard(CDC* pDC, CRenLibDoc* pDoc);
	void	DrawHelpMarker(CDC* pDC, CRenLibDoc* pDoc, int x, int y);
	void	DrawCircles(CDC* pDC, CRenLibDoc* pDoc);
	void	DrawOneCircle(CDC* pDC, CRenLibDoc* pDoc, int x, int y, COLORREF color, int number);
	void	DrawWin(CDC* pDC, int x, int y, const CString& strVcf, COLORREF color);
	void	DrawForbidden(CDC* pDC, CRenLibDoc* pDoc, int x, int y);
	void	DrawOneVariant(CDC* pDC, CRenLibDoc* pDoc, int x, int y, COLORREF color);
	void	DrawBoardText(CDC* pDC, CRenLibDoc* pDoc, const CString& strText, int x, int y, bool drawVariantMarker);
	void	DrawLastMoveMarker(CDC* pDC, CRenLibDoc* pDoc);
	void	DrawComment(CDC* pDC, CRenLibDoc* pDoc);
	void	AutopointMouse(CRenLibDoc* pDoc);
	void    UpdateNextAutoBoardText(CRenLibDoc* pDoc);
	CPoint	GetViewCoord(int x, int y);
	CPoint	GetInverseViewCoord(int x, int y);
	void	CalcMouseCoord(CPoint& point);
	CString RowImage(int row);
	CString ColImage(int col);
	int		defaultSize();
	void    SelectColor(COLORREF& color, COLORREF defaultColor);
	void	GetBoardView(Board& aBoard);
	void	CreateMemoryDeviceContext();
	void    UpdateScrollSizes();
	void	UpdateWinTrace();
	void	ChooseFont(Font& myFont);
	void	adjustSize();
	void	adjustControls();
	void	showTabControls();
	void	createTabControl();
	void	createListControl();
	void	readFile(CStringList& fileNameList);
	void	indicateConvertOrMerge(const CString& strTitle);
	void	loadBoardBitmap(const CString& strBoardBitmapPath);
	void	loadStoneBitmap();
	void	savePositionAsAsBmp(const CString& strFile);
	void    navigate(const CString& strUrl);
	void	initiateAddressUrl();
	void    OpenUrlInBrowser(const CString& strUrl);
	void	OpenUrlInBrowser(int index);
	void	SetPaneText(const CString& text);
};

#ifndef _DEBUG  // debug version in RenLibView.cpp
inline CRenLibDoc* CRenLibView::GetDocument()
   { return (CRenLibDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RENLIBVIEW_H__72EC9D4D_F85E_11D3_92A3_947F7AC2F525__INCLUDED_)
