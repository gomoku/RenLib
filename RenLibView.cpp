// RenLibView.cpp : implementation of the CRenLibView class
//

#include "stdafx.h"
#include "RenLib.h"
#include "Utils.h"
#include "EditBoardText.h"
#include "AutoBoardText.h"
#include "SearchComment.h"
#include "SearchMove.h"
#include "RenLibDoc.h"
#include "RenLibView.h"
#include "ProgressWnd.h"
#include "IO.H"
#include "MainFrm.h"
#include "Renjuclass.h"
#include "FileInfo.h"
#include "Registry.h"
#include "RenJS.h"
#include "RenLibApplet.h"
#include "RenLibAppletDialog.h"
#include "HyperLink.h"
#include "Bdt.h"
#include "Rdf.h"
#include "StringEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace
{
	const CPoint Center(8, 8);

	const int cSizeSmall  =  0;
	const int cSizeMedium =  1;
	const int cSizeLarge  =  2;

	const int xOffset = 30;
	const int yOffset = 30;

	const int cOneLineCommentBottom = 30;

	const int cAddressHeight = 25;

	const int cRight = 28;

	enum
	{
		TAB_COMMENT    = 0,
		TAB_STATISTICS = 1,
		TAB_BROWSER    = 2
	};

	const int cNoOfColumns = 2;

	const CString strDefaultNextAutoBoardText("A");
	
	const CString strDefaultBoardBitmap("Wood1.bmp");

	enum
	{
		URL_RENJU_NET_PORTAL               = 0,
		URL_RENJU_INTERNATIONAL_FEDERATION = 1,
		URL_ONLINE_RENJU_CLASS             = 2,
		URL_FORUM_ONLINE_RENJU_CLASS       = 3,
		URL_RENLIB_HOMEPAGE_1ST            = 4,
		URL_RENLIB_HOMEPAGE_2ND            = 5,
        URL_RENLIB_NEWS                    = 6,
		URL_RENLIB_USERS_GUIDE             = 7,
		URL_START_PAGE                     = 8
	};

	//------------------------------------------------------------------------

	const COLORREF cWhite                   = RGB(255, 255, 255);
	const COLORREF cBlack                   = RGB(  0,   0,   0);
	const COLORREF cRed                     = RGB(255,   0,   0);
	const COLORREF cDarkYellow              = RGB(190, 190,   0);
	const COLORREF cBlue                    = RGB(  0,   0, 255);
	const COLORREF cDarkGreen               = RGB(  0, 128,   0);
	const COLORREF cGray                    = RGB(225, 225, 225);
	const COLORREF cGreen                   = RGB(  0, 255,   0);
	const COLORREF cLightBlue               = RGB(  0, 255, 255);
	const COLORREF cLightGreen              = RGB(164, 255, 164);

	const COLORREF defaultBackgroundColor   = cGray;
	const COLORREF defaultBoardColor        = cDarkYellow;

	const COLORREF defaultBlackStoneColor   = cBlack;
	const COLORREF defaultWhiteStoneColor   = cWhite;

	const COLORREF defaultBlackVariantColor = cBlack;
	const COLORREF defaultWhiteVariantColor = cWhite;

	const COLORREF defaultLastMoveColor     = cDarkYellow;
	const COLORREF defaultForbiddenColor    = cRed;

	const COLORREF defaultBlackVcfColor     = cBlue;
	const COLORREF defaultWhiteVcfColor     = cDarkGreen;

	const COLORREF defaultBlackVctColor     = cBlue;
	const COLORREF defaultWhiteVctColor     = cDarkGreen;

	const COLORREF defaultBlackALVColor     = cBlue;
	const COLORREF defaultWhiteALVColor     = cGreen;

	const COLORREF defaultBlackCMOColor     = cLightBlue;
	const COLORREF defaultWhiteCMOColor     = cLightGreen;

	const COLORREF defaultCommentTextColor  = cBlack;
	const COLORREF defaultCommentBkColor    = cWhite;
	const COLORREF defaultCoordTextColor    = cBlack;
	const COLORREF defaultBoardTextColor    = cBlack;
	const COLORREF defaultBlackNumberColor  = cWhite;
	const COLORREF defaultWhiteNumberColor  = cBlack;

    const UINT cTimerId         =    1;
    const UINT cTimeVersionNews = 5000;
    const UINT cTimeVcfVct      =  333;
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibView

IMPLEMENT_DYNCREATE(CRenLibView, CFormView)

BEGIN_MESSAGE_MAP(CRenLibView, CFormView)
	//{{AFX_MSG_MAP(CRenLibView)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(ID_FILE_OPEN, OnUpdateFileOpen)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_MOVE_BACK, OnMoveBack)
	ON_COMMAND(ID_MOVE_DOWN, OnMoveDown)
	ON_COMMAND(ID_MOVE_UP, OnMoveUp)
	ON_COMMAND(ID_MOVE_FORWARD, OnMoveForward)
	ON_COMMAND(ID_APP_EXIT, OnAppExit)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UPDATE_MODE, OnUpdateEditUpdateMode)
	ON_COMMAND(ID_EDIT_UPDATE_MODE, OnEditUpdateMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_MOVE, OnUpdateEditDeleteMove)
	ON_COMMAND(ID_EDIT_DELETE_MOVE, OnEditDeleteMove)
	ON_COMMAND(ID_EDIT_MARK, OnEditMark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_MARK, OnUpdateEditMark)
	ON_COMMAND(ID_EDIT_FIND_MARK, OnEditFindMark)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_MARK, OnUpdateEditFindMark)
	ON_COMMAND(ID_EDIT_FIND_COMMENT, OnEditFindComment)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_COMMENT, OnUpdateEditFindComment)
	ON_UPDATE_COMMAND_UI(ID_MOVE_BACK, OnUpdateMoveBack)
	ON_UPDATE_COMMAND_UI(ID_MOVE_DOWN, OnUpdateMoveDown)
	ON_UPDATE_COMMAND_UI(ID_MOVE_FORWARD, OnUpdateMoveForward)
	ON_UPDATE_COMMAND_UI(ID_MOVE_UP, OnUpdateMoveUp)
	ON_COMMAND(ID_VIEW_SIZE_LARGE, OnViewSizeLarge)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIZE_LARGE, OnUpdateViewSizeLarge)
	ON_COMMAND(ID_VIEW_SIZE_MEDIUM, OnViewSizeMedium)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIZE_MEDIUM, OnUpdateViewSizeMedium)
	ON_COMMAND(ID_VIEW_SIZE_SMALL, OnViewSizeSmall)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIZE_SMALL, OnUpdateViewSizeSmall)
	ON_UPDATE_COMMAND_UI(ID_FILE_NEW, OnUpdateFileNew)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(ID_MOVE_1ST_MOVE, OnMove1stMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE_1ST_MOVE, OnUpdateMove1stMove)
	ON_COMMAND(ID_VIEW_COORDINATES_AO, OnViewCoordinatesAo)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATES_AO, OnUpdateViewCoordinatesAo)
	ON_COMMAND(ID_VIEW_COORDINATES_AP, OnViewCoordinatesAp)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATES_AP, OnUpdateViewCoordinatesAp)
	ON_COMMAND(ID_VIEW_COORDINATES_NONE, OnViewCoordinatesNone)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATES_NONE, OnUpdateViewCoordinatesNone)
	ON_COMMAND(ID_POSITION_0_DEGREES, OnPosition0degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_0_DEGREES, OnUpdatePosition0degrees)
	ON_COMMAND(ID_POSITION_180_DEGREES, OnPosition180degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_180_DEGREES, OnUpdatePosition180degrees)
	ON_COMMAND(ID_POSITION_270_DEGREES, OnPosition270degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_270_DEGREES, OnUpdatePosition270degrees)
	ON_COMMAND(ID_POSITION_90_DEGREES, OnPosition90degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_90_DEGREES, OnUpdatePosition90degrees)
	ON_COMMAND(ID_POSITION_REFLECT_LEFT_RIGHT, OnPositionReflectLeftRight)
	ON_UPDATE_COMMAND_UI(ID_POSITION_REFLECT_LEFT_RIGHT, OnUpdatePositionReflectLeftRight)
	ON_COMMAND(ID_POSITION_REFLECT_UP_DOWN, OnPositionReflectUpDown)
	ON_UPDATE_COMMAND_UI(ID_POSITION_REFLECT_UP_DOWN, OnUpdatePositionReflectUpDown)
	ON_COMMAND(ID_POSITION_TURN_MINUS_90_DEGREES, OnPositionTurnMinus90degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_TURN_MINUS_90_DEGREES, OnUpdatePositionTurnMinus90degrees)
	ON_COMMAND(ID_POSITION_TURN_PLUS_90_DEGREES, OnPositionTurnPlus90degrees)
	ON_UPDATE_COMMAND_UI(ID_POSITION_TURN_PLUS_90_DEGREES, OnUpdatePositionTurnPlus90degrees)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_CHOOSE_FONT, OnUpdateViewFontChooseFont)
	ON_COMMAND(ID_VIEW_FONT_CHOOSE_FONT, OnViewFontChooseFont)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_CHOOSE_COLOR, OnUpdateViewColorBoardChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_CHOOSE_COLOR, OnViewColorBoardChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_DEFAULT, OnUpdateViewColorBoardDefault)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_DEFAULT, OnViewColorBoardDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_DEFAULT_FONT, OnUpdateViewFontDefaultFont)
	ON_COMMAND(ID_VIEW_FONT_DEFAULT_FONT, OnViewFontDefaultFont)
	ON_COMMAND(ID_EDIT_FIND_NEXT, OnEditFindNext)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_NEXT, OnUpdateEditFindNext)
	ON_COMMAND(ID_TOOLBAR_FIND_NEXT, OnToolbarFindNext)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_FIND_NEXT, OnUpdateToolbarFindNext)
	ON_COMMAND(ID_VIEW_SHOW_NUMBERS, OnViewShowNumbers)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_NUMBERS, OnUpdateViewShowNumbers)
	ON_COMMAND(ID_VIEW_AUTO_POINT_MOUSE, OnViewAutoPointMouse)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AUTO_POINT_MOUSE, OnUpdateViewAutoPointMouse)
	ON_COMMAND(ID_VIEW_BOARD_TEXT_SHOW, OnViewShowBoardText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOARD_TEXT_SHOW, OnUpdateViewShowBoardText)
	ON_COMMAND(ID_VIEW_BOARD_TEXT_EXTRA_SPACE, OnViewBoardTextExtraSpace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_BOARD_TEXT_EXTRA_SPACE, OnUpdateViewBoardTextExtraSpace)
	ON_COMMAND(ID_VIEW_SHOW_COMMENTS, OnViewShowComments)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_COMMENTS, OnUpdateViewShowComments)
	ON_COMMAND(ID_VIEW_SHOW_LAST_MOVE, OnViewShowLastMove)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_LAST_MOVE, OnUpdateViewShowLastMove)
	ON_COMMAND(ID_VIEW_SHOW_VARIANTS, OnViewShowVariants)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_VARIANTS, OnUpdateViewShowVariants)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_EDIT_CLEAR_ALL_MARKS, OnEditClearAllMarks)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CLEAR_ALL_MARKS, OnUpdateEditClearAllMarks)
	ON_COMMAND(ID_EDIT_START_POSITION, OnEditStartPosition)
	ON_UPDATE_COMMAND_UI(ID_EDIT_START_POSITION, OnUpdateEditStartPosition)
	ON_COMMAND(ID_EDIT_FIND_START_POSITION, OnEditFindStartPosition)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_START_POSITION, OnUpdateEditFindStartPosition)
	ON_COMMAND(ID_MOVE_LAST_MOVE, OnMoveLastMove)
	ON_UPDATE_COMMAND_UI(ID_MOVE_LAST_MOVE, OnUpdateMoveLastMove)
	ON_WM_MOUSEMOVE()
	ON_UPDATE_COMMAND_UI(ID_VIEW_SIZE_DEFAULT, OnUpdateViewSizeDefault)
	ON_COMMAND(ID_VIEW_SIZE_DEFAULT, OnViewSizeDefault)
	ON_COMMAND(ID_EDIT_FIND_MOVE, OnEditFindMove)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_MOVE, OnUpdateEditFindMove)
	ON_COMMAND(ID_VIEW_COORDINATES_SWAP_NUMBER, OnViewCoordinatesSwapNumber)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATES_SWAP_NUMBER, OnUpdateViewCoordinatesSwapNumber)
	ON_COMMAND(ID_VIEW_SHOW_FORBIDDEN_POINTS, OnViewShowForbiddenPoints)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_FORBIDDEN_POINTS, OnUpdateViewShowForbiddenPoints)
	ON_COMMAND(ID_EDIT_FIND_PREVIOUS, OnEditFindPrevious)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_PREVIOUS, OnUpdateEditFindPrevious)
	ON_COMMAND(ID_TOOLBAR_FIND_PREVIOUS, OnToolbarFindPrevious)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_FIND_PREVIOUS, OnUpdateToolbarFindPrevious)
	ON_COMMAND(ID_EDIT_FIND_SAME_POSITION, OnEditFindSamePosition)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_SAME_POSITION, OnUpdateEditFindSamePosition)
	ON_COMMAND(ID_EDIT_FIND_SIMILAR_POSITION, OnEditFindSimilarPosition)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_SIMILAR_POSITION, OnUpdateEditFindSimilarPosition)
	ON_COMMAND(ID_VIEW_AUTO_FIND_SAME_POSITION, OnViewAutoFindSamePosition)
	ON_UPDATE_COMMAND_UI(ID_VIEW_AUTO_FIND_SAME_POSITION, OnUpdateViewAutoFindSamePosition)
	ON_COMMAND(ID_VIEW_SHOW_AUTO_LINK_VARIANTS, OnViewShowAutoLinkVariants)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_AUTO_LINK_VARIANTS, OnUpdateViewShowAutoLinkVariants)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCF_COLOR_CHOOSE_BLACK, OnUpdateViewColorVcfColorChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_VCF_COLOR_CHOOSE_BLACK, OnViewColorVcfColorChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCF_COLOR_DEFAULT, OnUpdateViewColorVcfColorDefault)
	ON_COMMAND(ID_VIEW_COLOR_VCF_COLOR_DEFAULT, OnViewColorVcfColorDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCF_COLOR_CHOOSE_WHITE, OnUpdateViewColorVcfColorChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_VCF_COLOR_CHOOSE_WHITE, OnViewColorVcfColorChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCT_COLOR_CHOOSE_BLACK, OnUpdateViewColorVctColorChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_VCT_COLOR_CHOOSE_BLACK, OnViewColorVctColorChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCT_COLOR_DEFAULT, OnUpdateViewColorVctColorDefault)
	ON_COMMAND(ID_VIEW_COLOR_VCT_COLOR_DEFAULT, OnViewColorVctColorDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VCT_COLOR_CHOOSE_WHITE, OnUpdateViewColorVctColorChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_VCT_COLOR_CHOOSE_WHITE, OnViewColorVctColorChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_CHOOSE_BLACK, OnUpdateViewColorAutoLinkVariantColorChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_CHOOSE_BLACK, OnViewColorAutoLinkVariantColorChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_DEFAULT, OnUpdateViewColorAutoLinkVariantColorDefault)
	ON_COMMAND(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_DEFAULT, OnViewColorAutoLinkVariantColorDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_CHOOSE_WHITE, OnUpdateViewColorAutoLinkVariantColorChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_AUTO_LINK_VARIANT_COLOR_CHOOSE_WHITE, OnViewColorAutoLinkVariantColorChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_CHOOSE_BLACK, OnUpdateViewColorChangedOrderOfMovesVariantColorChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_CHOOSE_BLACK, OnViewColorChangedOrderOfMovesVariantColorChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_DEFAULT, OnUpdateViewColorChangedOrderOfMovesVariantColorDefault)
	ON_COMMAND(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_DEFAULT, OnViewColorChangedOrderOfMovesVariantColorDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_CHOOSE_WHITE, OnUpdateViewColorChangedOrderOfMovesVariantColorChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_CHANGED_ORDER_OF_MOVES_VARIANT_COLOR_CHOOSE_WHITE, OnViewColorChangedOrderOfMovesVariantColorChooseWhite)
	ON_COMMAND(ID_VIEW_SHOW_CHANGED_ORDER_OF_MOVES_VARIANTS, OnViewShowChangedOrderOfMovesVariants)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_CHANGED_ORDER_OF_MOVES_VARIANTS, OnUpdateViewShowChangedOrderOfMovesVariants)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_LAST_MOVE_CHOOSE_COLOR, OnUpdateViewColorLastMoveChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_LAST_MOVE_CHOOSE_COLOR, OnViewColorLastMoveChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_LAST_MOVE_DEFAULT, OnUpdateViewColorLastMoveDefault)
	ON_COMMAND(ID_VIEW_COLOR_LAST_MOVE_DEFAULT, OnViewColorLastMoveDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_CHOOSE_BLACK, OnUpdateViewColorStoneChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_STONE_CHOOSE_BLACK, OnViewColorStoneChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_CHOOSE_WHITE, OnUpdateViewColorStoneChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_STONE_CHOOSE_WHITE, OnViewColorStoneChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_DEFAULT, OnUpdateViewColorStoneDefault)
	ON_COMMAND(ID_VIEW_COLOR_STONE_DEFAULT, OnViewColorStoneDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VARIANT_CHOOSE_BLACK, OnUpdateViewColorVariantChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_VARIANT_CHOOSE_BLACK, OnViewColorVariantChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VARIANT_CHOOSE_WHITE, OnUpdateViewColorVariantChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_VARIANT_CHOOSE_WHITE, OnViewColorVariantChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_VARIANT_DEFAULT, OnUpdateViewColorVariantDefault)
	ON_COMMAND(ID_VIEW_COLOR_VARIANT_DEFAULT, OnViewColorVariantDefault)
	ON_COMMAND(ID_MOVE_PASS, OnMovePass)
	ON_UPDATE_COMMAND_UI(ID_MOVE_PASS, OnUpdateMovePass)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_COMMENT_BACKGROUND, OnUpdateViewColorCommentBackground)
	ON_COMMAND(ID_VIEW_COLOR_COMMENT_BACKGROUND, OnViewColorCommentBackground)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_COMMENT_DEFAULT, OnUpdateViewColorCommentDefault)
	ON_COMMAND(ID_VIEW_COLOR_COMMENT_DEFAULT, OnViewColorCommentDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_COMMENT_TEXT, OnUpdateViewColorCommentText)
	ON_COMMAND(ID_VIEW_COLOR_COMMENT_TEXT, OnViewColorCommentText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_COORDINATE_CHOOSE_COLOR, OnUpdateViewColorCoordinateChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_COORDINATE_CHOOSE_COLOR, OnViewColorCoordinateChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_COORDINATE_DEFAULT, OnUpdateViewColorCoordinateDefault)
	ON_COMMAND(ID_VIEW_COLOR_COORDINATE_DEFAULT, OnViewColorCoordinateDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_TEXT_CHOOSE_COLOR, OnUpdateViewColorBoardTextChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_TEXT_CHOOSE_COLOR, OnViewColorBoardTextChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_TEXT_DEFAULT, OnUpdateViewColorBoardTextDefault)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_TEXT_DEFAULT, OnViewColorBoardTextDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_NUMBER_CHOOSE_BLACK, OnUpdateViewColorStoneNumberChooseBlack)
	ON_COMMAND(ID_VIEW_COLOR_STONE_NUMBER_CHOOSE_BLACK, OnViewColorStoneNumberChooseBlack)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_NUMBER_CHOOSE_WHITE, OnUpdateViewColorStoneNumberChooseWhite)
	ON_COMMAND(ID_VIEW_COLOR_STONE_NUMBER_CHOOSE_WHITE, OnViewColorStoneNumberChooseWhite)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_STONE_NUMBER_DEFAULT, OnUpdateViewColorStoneNumberDefault)
	ON_COMMAND(ID_VIEW_COLOR_STONE_NUMBER_DEFAULT, OnViewColorStoneNumberDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BACKGROUND_CHOOSE_COLOR, OnUpdateViewColorBackgroundChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_BACKGROUND_CHOOSE_COLOR, OnViewColorBackgroundChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BACKGROUND_DEFAULT, OnUpdateViewColorBackgroundDefault)
	ON_COMMAND(ID_VIEW_COLOR_BACKGROUND_DEFAULT, OnViewColorBackgroundDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_ALL_DEFAULT, OnUpdateViewColorAllDefault)
	ON_COMMAND(ID_VIEW_COLOR_ALL_DEFAULT, OnViewColorAllDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_FORBIDDEN_CHOOSE_COLOR, OnUpdateViewColorForbiddenChooseColor)
	ON_COMMAND(ID_VIEW_COLOR_FORBIDDEN_CHOOSE_COLOR, OnViewColorForbiddenChooseColor)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_FORBIDDEN_DEFAULT, OnUpdateViewColorForbiddenDefault)
	ON_COMMAND(ID_VIEW_COLOR_FORBIDDEN_DEFAULT, OnViewColorForbiddenDefault)
	ON_WM_PAINT()
	ON_COMMAND(ID_EDIT_DELETE_VARIANTS, OnEditDeleteVariants)
	ON_UPDATE_COMMAND_UI(ID_EDIT_DELETE_VARIANTS, OnUpdateEditDeleteVariants)
	ON_COMMAND(ID_VIEW_COORDINATES_FOLLOW_ROTATION_AND_REFLECTION, OnViewCoordinatesFollowRotationAndReflection)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COORDINATES_FOLLOW_ROTATION_AND_REFLECTION, OnUpdateViewCoordinatesFollowRotationAndReflection)
	ON_COMMAND(ID_TOOLBAR_BACK, OnToolbarBack)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_BACK, OnUpdateToolbarBack)
	ON_COMMAND(ID_TOOLBAR_DOWN, OnToolbarDown)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_DOWN, OnUpdateToolbarDown)
	ON_COMMAND(ID_TOOLBAR_UP, OnToolbarUp)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_UP, OnUpdateToolbarUp)
	ON_COMMAND(ID_TOOLBAR_FORWARD, OnToolbarForward)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_FORWARD, OnUpdateToolbarForward)
	ON_COMMAND(ID_TOOLBAR_1ST_MOVE, OnToolbar1stMove)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_1ST_MOVE, OnUpdateToolbar1stMove)
	ON_COMMAND(ID_TOOLBAR_LAST_MOVE, OnToolbarLastMove)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_LAST_MOVE, OnUpdateToolbarLastMove)
	ON_COMMAND(ID_TOOLBAR_PASS, OnToolbarPass)
	ON_UPDATE_COMMAND_UI(ID_TOOLBAR_PASS, OnUpdateToolbarPass)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_FIND_VCF, OnEditFindVcf)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_VCF, OnUpdateFindVcf)
	ON_COMMAND(ID_EDIT_FIND_VCT, OnEditFindVct)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_VCT, OnUpdateFindVct)
	ON_COMMAND(ID_EDIT_FIND_STOP, OnEditFindStop)
	ON_UPDATE_COMMAND_UI(ID_EDIT_FIND_STOP, OnUpdateFindStop)
	ON_COMMAND(ID_FILE_SAVE_LIBRARY_AS, OnFileSaveLibraryAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_LIBRARY_AS, OnUpdateFileSaveAs)
	ON_COMMAND(ID_EDIT_AUTO_ENTER_BOARD_TEXT, OnEditAutoEnterBoardText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_AUTO_ENTER_BOARD_TEXT, OnUpdateEditAutoEnterBoardText)
	ON_COMMAND(ID_EDIT_SET_NEXT_AUTO_BOARD_TEXT, OnEditSetNextAutoBoardText)
	ON_UPDATE_COMMAND_UI(ID_EDIT_SET_NEXT_AUTO_BOARD_TEXT, OnUpdateEditSetNextAutoBoardText)
	ON_COMMAND(ID_VIEW_WIN_123, OnViewWin123)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIN_123, OnUpdateViewWin123)
	ON_COMMAND(ID_VIEW_WIN_ABC_LOWER, OnViewWinAbcLower)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIN_ABC_LOWER, OnUpdateViewWinAbcLower)
	ON_COMMAND(ID_VIEW_WIN_ABC_UPPER, OnViewWinAbcUpper)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WIN_ABC_UPPER, OnUpdateViewWinAbcUpper)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_ALL_DEFAULT, OnUpdateViewFontAllDefault)
	ON_COMMAND(ID_VIEW_FONT_ALL_DEFAULT, OnViewFontAllDefault)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_CHOOSE_FONT_BOARD_TEXT, OnUpdateViewFontChooseFontBoardText)
	ON_COMMAND(ID_VIEW_FONT_CHOOSE_FONT_BOARD_TEXT, OnViewFontChooseFontBoardText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_CHOOSE_FONT_COMMENT, OnUpdateViewFontChooseFontComment)
	ON_COMMAND(ID_VIEW_FONT_CHOOSE_FONT_COMMENT, OnViewFontChooseFontComment)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_DEFAULT_FONT_BOARD_TEXT, OnUpdateViewFontDefaultFontBoardText)
	ON_COMMAND(ID_VIEW_FONT_DEFAULT_FONT_BOARD_TEXT, OnViewFontDefaultFontBoardText)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FONT_DEFAULT_FONT_COMMENT, OnUpdateViewFontDefaultFontComment)
	ON_COMMAND(ID_VIEW_FONT_DEFAULT_FONT_COMMENT, OnViewFontDefaultFontComment)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, tabSelChange)
	ON_COMMAND(ID_MERGE_COMMENT_NEXT, OnMergeCommentNext)
	ON_COMMAND(ID_MERGE_COMMENT_PREVIOUS, OnMergeCommentPrevious)
	ON_COMMAND(ID_EDIT_CONVERT_TO_STANDARD, OnEditConvertToStandard)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CONVERT_TO_STANDARD, OnUpdateEditConvertToStandard)
	ON_COMMAND(ID_FILE_EXPORT_LIBRARY_TEXT_BOARD, OnFileExportLibraryTextBoard)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_LIBRARY_TEXT_BOARD, OnUpdateFileExportLibraryTextBoard)
	ON_COMMAND(ID_FILE_EXPORT_LIBRARY_RENLIB_APPLET, OnFileExportLibraryRenLibApplet)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_LIBRARY_RENLIB_APPLET, OnUpdateFileExportLibraryRenLibApplet)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_LIB, OnUpdateOptionsEnvironmentAssociateLib)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_LIB, OnOptionsEnvironmentAssociateLib)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_WZQ, OnUpdateOptionsEnvironmentAssociateWzq)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_WZQ, OnOptionsEnvironmentAssociateWzq)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_BUF, OnUpdateOptionsEnvironmentAssociateBuf)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_BUF, OnOptionsEnvironmentAssociateBuf)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_PDB, OnUpdateOptionsEnvironmentAssociatePdb)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_PDB, OnOptionsEnvironmentAssociatePdb)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_POS, OnUpdateOptionsEnvironmentAssociatePos)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_POS, OnOptionsEnvironmentAssociatePos)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_RENJS, OnUpdateOptionsEnvironmentAssociateRenjs)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_RENJS, OnOptionsEnvironmentAssociateRenjs)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_SGF, OnUpdateOptionsEnvironmentAssociateSgf)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_SGF, OnOptionsEnvironmentAssociateSgf)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_BDT, OnUpdateOptionsEnvironmentAssociateBdt)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_BDT, OnOptionsEnvironmentAssociateBdt)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_RDF, OnUpdateOptionsEnvironmentAssociateRdf)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_RDF, OnOptionsEnvironmentAssociateRdf)
    ON_UPDATE_COMMAND_UI(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_ALL, OnUpdateOptionsEnvironmentAssociateAll)
	ON_COMMAND(ID_OPTIONS_ENVIRONMENT_ASSOCIATE_ALL, OnOptionsEnvironmentAssociateAll)
	ON_EN_CHANGE(IDC_ONE_LINE_COMMENT, OnOneLineCommentUpdate)
	ON_EN_SETFOCUS(IDC_ONE_LINE_COMMENT, OnOneLineCommentSetFocus)
	ON_EN_KILLFOCUS(IDC_ONE_LINE_COMMENT, OnOneLineCommentKillFocus)
	ON_EN_CHANGE(IDC_MULTI_LINE_COMMENT, OnMultiLineCommentUpdate)
	ON_EN_SETFOCUS(IDC_MULTI_LINE_COMMENT, OnMultiLineCommentSetFocus)
	ON_EN_KILLFOCUS(IDC_MULTI_LINE_COMMENT, OnMultiLineCommentKillFocus)
	ON_CBN_SETFOCUS(IDC_ADDRESS, OnAddressSetFocus)
	ON_CBN_KILLFOCUS(IDC_ADDRESS, OnAddressKillFocus)
	ON_CBN_SELCHANGE(IDC_ADDRESS, OnAddressSelChange)
	ON_COMMAND(ID_OPTIONS_EVALUATOR_RENJU_CLASS_RENJU_SOLVER, OnOptionsEvaluatorRenjuClassRenjuSolver)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_EVALUATOR_RENJU_CLASS_RENJU_SOLVER, OnUpdateOptionsEvaluatorRenjuClassRenjuSolver)
	ON_UPDATE_COMMAND_UI(ID_FILE_MAKE_TOURNAMENT_RENLIB_APPLET, OnUpdateFileMakeTournamentRenLibApplet)
	ON_COMMAND(ID_FILE_MAKE_TOURNAMENT_RENLIB_APPLET, OnFileMakeTournamentRenLibApplet)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_CHOOSE_BITMAP, OnUpdateViewColorBoardChooseBitmap)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_CHOOSE_BITMAP, OnViewColorBoardChooseBitmap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_COLOR_BOARD_DEFAULT_BITMAP, OnUpdateViewColorBoardDefaultBitmap)
	ON_COMMAND(ID_VIEW_COLOR_BOARD_DEFAULT_BITMAP, OnViewColorBoardDefaultBitmap)
	ON_COMMAND(ID_VIEW_SHOW_BOARD_BITMAP, OnViewShowBoardBitmap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_BOARD_BITMAP, OnUpdateViewShowBoardBitmap)
	ON_COMMAND(ID_VIEW_SHOW_STONE_BITMAP, OnViewShowStoneBitmap)
	ON_UPDATE_COMMAND_UI(ID_VIEW_SHOW_STONE_BITMAP, OnUpdateViewShowStoneBitmap)
	ON_COMMAND(ID_OPTIONS_EVALUATOR_NONE, OnOptionsEvaluatorNone)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_EVALUATOR_NONE, OnUpdateOptionsEvaluatorNone)
	ON_COMMAND(ID_FILE_READ, OnFileRead)
	ON_UPDATE_COMMAND_UI(ID_FILE_READ, OnUpdateFileRead)
	ON_COMMAND(ID_FILE_READ_ALL, OnFileReadAll)
	ON_UPDATE_COMMAND_UI(ID_FILE_READ_ALL, OnUpdateFileReadAll)
	ON_COMMAND(ID_FILE_SAVE_POSITION_AS, OnFileSavePositionAs)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE_POSITION_AS, OnUpdateFileSavePositionAs)
	ON_BN_CLICKED(IDC_GO, OnGo)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_OPEN_IN_DEFAULT_BROWSER, OnUpdateBrowserOpenInDefaultBrowser)
	ON_COMMAND(ID_BROWSER_OPEN_IN_DEFAULT_BROWSER, OnBrowserOpenInDefaultBrowser)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_ONLINE_RENJU_CLASS, OnUpdateBrowserOnlineRenjuclass)
	ON_COMMAND(ID_BROWSER_ONLINE_RENJU_CLASS, OnBrowserOnlineRenjuclass)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RENJU_NET_PORTAL, OnUpdateBrowserRenjuNetPortal)
	ON_COMMAND(ID_BROWSER_RENJU_NET_PORTAL, OnBrowserRenjuNetPortal)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RENLIB_HOMEPAGE_1ST, OnUpdateBrowserRenLibHomepage1st)
	ON_COMMAND(ID_BROWSER_RENLIB_HOMEPAGE_1ST, OnBrowserRenLibHomepage1st)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RENLIB_HOMEPAGE_2ND, OnUpdateBrowserRenLibHomepage2nd)
	ON_COMMAND(ID_BROWSER_RENLIB_HOMEPAGE_2ND, OnBrowserRenLibHomepage2nd)
	ON_COMMAND(ID_BROWSER_RENLIB_NEWS, OnBrowserRenLibNews)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RENLIB_NEWS, OnUpdateBrowserRenLibNews)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RENLIB_USERS_GUIDE, OnUpdateBrowserRenLibUsersGuide)
	ON_COMMAND(ID_BROWSER_RENLIB_USERS_GUIDE, OnBrowserRenLibUsersGuide)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_RIF, OnUpdateBrowserRenjuInternationalFederation)
	ON_COMMAND(ID_BROWSER_RIF, OnBrowserRenjuInternationalFederation)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_FORUM_ONLINE_RENJU_CLASS, OnUpdateBrowserForumOnlineRenjuclass)
	ON_COMMAND(ID_BROWSER_FORUM_ONLINE_RENJU_CLASS, OnBrowserForumOnlineRenjuclass)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_START_PAGE, OnUpdateBrowserStartpage)
	ON_COMMAND(ID_BROWSER_START_PAGE, OnBrowserStartPage)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_USE_CURRENT_PAGE_AS_START_PAGE, OnUpdateBrowserUseCurrentPageAsStartpage)
	ON_COMMAND(ID_BROWSER_USE_CURRENT_PAGE_AS_START_PAGE, OnBrowserUseCurrentPageAsStartPage)
	ON_COMMAND(ID_BROWSER_OPEN_DIAGRAMS, OnBrowserOpenDiagrams)
	ON_COMMAND(ID_BROWSER_READ_DIAGRAMS, OnBrowserReadDiagrams)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_READ_DIAGRAMS, OnUpdateBrowserReadDiagrams)
	ON_UPDATE_COMMAND_UI(ID_BROWSER_OPEN_DIAGRAMS, OnUpdateBrowserOpenDiagrams)
	ON_COMMAND(ID_OPEN_WEB_PAGE, OnOpenWebPage)
	ON_COMMAND(ID_SHOW_COMMENT_TAB, showCommentTab)
	ON_WM_TIMER()
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORT_LIBRARY_GAME_COLLECTION, OnUpdateFileExportLibraryGameCollection)
	ON_COMMAND(ID_FILE_EXPORT_LIBRARY_GAME_COLLECTION, OnFileExportLibraryGameCollection)
	ON_COMMAND(ID_EDIT_INSERT_PASS_MOVE, OnEditInsertPassMove)
	ON_UPDATE_COMMAND_UI(ID_EDIT_INSERT_PASS_MOVE, OnUpdateEditInsertPassMove)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_EDIT_PROBLEM_MODE, OnEditProblemMode)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PROBLEM_MODE, OnUpdateEditProblemMode)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CRenLibView, CFormView)
    //{{AFX_EVENTSINK_MAP(CRenLibView)
	ON_EVENT(CRenLibView, IDC_EXPLORER, 259 /* DocumentComplete  */, OnDocumentCompleteExplorer, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CRenLibView, IDC_EXPLORER, 250 /* BeforeNavigate2   */, OnBeforeNavigate2Explorer, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CRenLibView, IDC_EXPLORER, 252 /* NavigateComplete2 */, OnNavigateComplete2Explorer, VTS_DISPATCH VTS_PVARIANT)
	ON_EVENT(CRenLibView, IDC_EXPLORER, 102 /* StatusTextChange  */, OnStatusTextChangeExplorer, VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRenLibView construction/destruction

CRenLibView::CRenLibView()
:	CFormView(CRenLibView::IDD),
	m_bInitialUpdate(false),
	m_MouseCoord(0,0),
	m_SearchMoveNo(0),
	m_ePosition(DEGREE_0),
	m_bReflect_X(false),
	m_bReflect_Y(false),
	m_bToolBarMoveCommand(false),
	m_bAutoEnterBoardText(true),
	m_NextAutoBoardText(strDefaultNextAutoBoardText),
	m_LastMoveNode(0),
	m_bOneLineCommentHasFocus(false),
	m_bMultiLineCommentHasFocus(false),
	m_bAddressHasFocus(false),
	m_strOneLineComment("a"),
	m_strMultiLineComment("b"),
	m_BoardBitmapState(DEFAULT_BITMAP)
{
	Registry reg;

	m_BoardSize             = reg.getInt(Registry::BoardSize, defaultSize());
	loadStoneBitmap();

	m_eCoordinate           = (Coordinate) reg.getInt(Registry::Coordinates, AO);

    m_bSwapRowNumber        = reg.getBool(Registry::SwapRowNumber,        false);
	
	m_bCoordinatesFollow    = reg.getBool(Registry::CoordinatesFollow,    false);

	m_bShowBoardBitmap      = reg.getBool(Registry::ShowBoardBitmap,      true);
	
	m_bShowStoneBitmap      = reg.getBool(Registry::ShowStoneBitmap,      true);
	
	m_bShowNumbers          = reg.getBool(Registry::ShowNumbers,          true);
	
	m_bShowLastMove         = reg.getBool(Registry::ShowLastMove,         true);
	
	m_bShowBoardText        = reg.getBool(Registry::ShowBoardText,        true);
	
	m_bBoardTextExtraSpace  = reg.getBool(Registry::BoardTextExtraSpace,  true);
	
	m_eWinMarker            = (Board::WinMarker) reg.getInt(Registry::WinMarker, Board::NUMBER);
	
	m_bShowComments         = reg.getBool(Registry::ShowComments,         true);
	
	m_bShowVariants         = reg.getBool(Registry::ShowVariants,         true);
	
	m_bShowForbidden        = reg.getBool(Registry::ShowForbidden,        true);
	
	m_bAutoPointMouse       = reg.getBool(Registry::AutoPointMouse,       true);

	m_bAutoFindSamePosition = reg.getBool(Registry::AutoFindSamePosition, true);

	m_bShowAutoLinkVariants = reg.getBool(Registry::ShowAutoLinkVariants, true);

	m_bShowChangedOrderOfMovesVariants = 
		                      reg.getBool(Registry::ShowChangedOrderOfMovesVariants, true);

	m_BackgroundColor   = reg.getInt(Registry::BackgroundColor,   defaultBackgroundColor);

	m_BoardColor        = reg.getInt(Registry::BoardColor,        defaultBoardColor);
	
	m_BlackStoneColor   = reg.getInt(Registry::BlackStoneColor,   defaultBlackStoneColor);
	
	m_WhiteStoneColor   = reg.getInt(Registry::WhiteStoneColor,   defaultWhiteStoneColor);
	
	m_BlackVariantColor = reg.getInt(Registry::BlackVariantColor, defaultBlackVariantColor);
	
	m_WhiteVariantColor = reg.getInt(Registry::WhiteVariantColor, defaultWhiteVariantColor);
	
	m_LastMoveColor     = reg.getInt(Registry::LastMoveColor,     defaultLastMoveColor);
	
	m_ForbiddenColor    = reg.getInt(Registry::ForbiddenColor,    defaultForbiddenColor);
	
	m_BlackVcfColor     = reg.getInt(Registry::BlackVcfColor,     defaultBlackVcfColor);
	
	m_WhiteVctColor     = reg.getInt(Registry::WhiteVctColor,     defaultWhiteVctColor);
	
	m_BlackVcfColor     = reg.getInt(Registry::BlackVcfColor,     defaultBlackVcfColor);
	
	m_WhiteVctColor     = reg.getInt(Registry::WhiteVctColor,     defaultWhiteVctColor);
	
	m_BlackALVColor     = reg.getInt(Registry::BlackALVColor,     defaultBlackALVColor);
	
	m_WhiteALVColor     = reg.getInt(Registry::WhiteALVColor,     defaultWhiteALVColor);
	
	m_BlackCMOColor     = reg.getInt(Registry::BlackCMOColor,     defaultBlackCMOColor);
	
	m_WhiteCMOColor     = reg.getInt(Registry::WhiteCMOColor,     defaultWhiteCMOColor);
	
	m_CommentTextColor  = reg.getInt(Registry::CommentTextColor,  defaultCommentTextColor);
	
	m_CommentBkColor    = reg.getInt(Registry::CommentBkColor,    defaultCommentBkColor);
	
	m_CoordTextColor    = reg.getInt(Registry::CoordTextColor,    defaultCoordTextColor);
	
	m_BoardTextColor    = reg.getInt(Registry::BoardTextColor,    defaultBoardTextColor);
	
	m_BlackNumberColor  = reg.getInt(Registry::BlackNumberColor,  defaultBlackNumberColor);
	
	m_WhiteNumberColor  = reg.getInt(Registry::WhiteNumberColor,  defaultWhiteNumberColor);

	m_CustColors[ 0] = reg.getInt(Registry::CustColors_0,  cWhite);
	
	m_CustColors[ 1] = reg.getInt(Registry::CustColors_1,  cWhite);
	
	m_CustColors[ 2] = reg.getInt(Registry::CustColors_2,  cWhite);
	
	m_CustColors[ 3] = reg.getInt(Registry::CustColors_3,  cWhite);
	
	m_CustColors[ 4] = reg.getInt(Registry::CustColors_4,  cWhite);
	
	m_CustColors[ 5] = reg.getInt(Registry::CustColors_5,  cWhite);
	
	m_CustColors[ 6] = reg.getInt(Registry::CustColors_6,  cWhite);
	
	m_CustColors[ 7] = reg.getInt(Registry::CustColors_7,  cWhite);
	
	m_CustColors[ 8] = reg.getInt(Registry::CustColors_8,  cWhite);
	
	m_CustColors[ 9] = reg.getInt(Registry::CustColors_9,  cWhite);
	
	m_CustColors[10] = reg.getInt(Registry::CustColors_10, cWhite);
	
	m_CustColors[11] = reg.getInt(Registry::CustColors_11, cWhite);
	
	m_CustColors[12] = reg.getInt(Registry::CustColors_12, cWhite);
	
	m_CustColors[13] = reg.getInt(Registry::CustColors_13, cWhite);
	
	m_CustColors[14] = reg.getInt(Registry::CustColors_14, cWhite);
	
	m_CustColors[15] = reg.getInt(Registry::CustColors_15, cWhite);

	m_Font[cSizeSmall].parse          (reg.getString(Registry::FontSmall,           ""), cSizeSmall);
	m_Font[cSizeMedium].parse         (reg.getString(Registry::FontMedium,          ""), cSizeMedium);
	m_Font[cSizeLarge].parse          (reg.getString(Registry::FontLarge,           ""), cSizeLarge);
	
	m_FontComment[cSizeSmall].parse   (reg.getString(Registry::FontCommentSmall,    ""), cSizeSmall);
	m_FontComment[cSizeMedium].parse  (reg.getString(Registry::FontCommentMedium,   ""), cSizeMedium);
	m_FontComment[cSizeLarge].parse   (reg.getString(Registry::FontCommentLarge,    ""), cSizeLarge);

	m_FontBoardText[cSizeSmall].parse (reg.getString(Registry::FontBoardTextSmall,  ""), cSizeSmall);
	m_FontBoardText[cSizeMedium].parse(reg.getString(Registry::FontBoardTextMedium, ""), cSizeMedium);
	m_FontBoardText[cSizeLarge].parse (reg.getString(Registry::FontBoardTextLarge,  ""), cSizeLarge);

	mStartPage = reg.getString(Registry::StartPage, Utils::GetString(IDS_URL_RENJU_NET_PORTAL));

	m_pdcMemory = 0;
	m_pBitmap   = 0;

	m_strBoardBitmap = reg.getString(Registry::BoardBitmap, Utils::getBitmapDirectory() + strDefaultBoardBitmap);
	loadBoardBitmap(m_strBoardBitmap);
}

void CRenLibView::loadBoardBitmap(const CString& strBoardBitmapPath)
{
	if (Utils::exists(strBoardBitmapPath) &&
		m_Board.Load(strBoardBitmapPath))
	{
		m_BoardBitmapState = VALID_BITMAP;
	}
	else if (m_BoardBitmapState == VALID_BITMAP)
	{
		m_BoardBitmapState = INVALID_BITMAP;
	}

	if (m_BoardBitmapState != VALID_BITMAP)
	{
		m_Board.SetBitmap(IDB_INVALID_BITMAP);
	}
}

void CRenLibView::loadStoneBitmap()
{
	m_BlackStone.Detach();
	m_WhiteStone.Detach();

	if (m_BoardSize == cSizeSmall)
	{
		m_BlackStone.LoadBitmap(IDB_BLACK_SMALL);
		m_WhiteStone.LoadBitmap(IDB_WHITE_SMALL);
	}
	else if (m_BoardSize == cSizeMedium)
	{
		m_BlackStone.LoadBitmap(IDB_BLACK_MEDIUM);
		m_WhiteStone.LoadBitmap(IDB_WHITE_MEDIUM);
	}
	else // (m_BoardSize == cSizeLarge)
	{
		m_BlackStone.LoadBitmap(IDB_BLACK_LARGE);
		m_WhiteStone.LoadBitmap(IDB_WHITE_LARGE);
	}

	m_StoneSize = m_BlackStone.Width();
}

void CRenLibView::Invalidate(BOOL bErase)
{
	CRect repaintRect(m_rectBitmap);
	repaintRect.top += cOneLineCommentBottom;

	InvalidateRect(&repaintRect, bErase);
}

void CRenLibView::adjustControls()
{
	if (m_bInitialUpdate)
	{
		CClientDC dc(this);
		OnPrepareDC(&dc);

		CRect ClientRect;
		GetClientRect(&ClientRect);

		dc.DPtoLP(ClientRect);

		const int oneLineCommentLeft   = xOffset;
		const int oneLineCommentTop    = 5;
		const int oneLineCommentRight  = xOffset + m_StoneSize * 25;
		const int oneLineCommentBottom = cOneLineCommentBottom;

		CRect oneLineCommentRect
				( oneLineCommentLeft,
                  oneLineCommentTop,
                  __max(ClientRect.right, oneLineCommentRight),
                  oneLineCommentBottom);

		m_OneLineComment.MoveWindow(&oneLineCommentRect);

		const int tabBoxLeft   = xOffset + m_StoneSize * 17;
		const int tabBoxTop    = yOffset;
		const int tabBoxRight  = xOffset + m_StoneSize * cRight;
		const int tabBoxBottom = yOffset + m_StoneSize * 17;

		CRect tabBoxRect(tabBoxLeft,
						 tabBoxTop,
						 __max(ClientRect.right, tabBoxRight),
						 __max(ClientRect.bottom, tabBoxBottom));

		// Set position of tab control
		m_Tab.MoveWindow(&tabBoxRect);

		// Get client area of tab control
		CRect tabBoxClientRect(tabBoxRect);
		m_Tab.AdjustRect(FALSE, &tabBoxClientRect);

		CRect goRect(tabBoxClientRect);
		goRect.left   = goRect.right - cAddressHeight * 0.9;
		goRect.bottom = goRect.top   + cAddressHeight * 0.9;

		CRect addressRect(tabBoxClientRect);
		addressRect.bottom = addressRect.top + cAddressHeight * m_Address.GetCount();
		addressRect.right -= goRect.Width();

		CRect browserRect(tabBoxClientRect);
		browserRect.top = addressRect.top + cAddressHeight;

		// Set position of the controls
		m_MultiLineComment.MoveWindow(&tabBoxClientRect);
		m_Statistics.MoveWindow(&tabBoxClientRect);
		m_Address.MoveWindow(&addressRect);
		m_Go.MoveWindow(&goRect);

		if (IsWindow(m_Browser.GetSafeHwnd()))
		{
			m_Browser.MoveWindow(&browserRect);
		}

		CRect listBoxRect;
		m_Statistics.GetClientRect(&listBoxRect);

		for (int i = 0; i < cNoOfColumns; i++)
		{
			m_Statistics.SetColumnWidth(i, listBoxRect.Width() / cNoOfColumns);
		}

		// Redraw tab box
		InvalidateRect(&tabBoxClientRect);

		// Get client area of tab control
		tabBoxRect.bottom = tabBoxClientRect.top;
		m_Tab.MoveWindow(&tabBoxRect);
	}
}

CRenLibView::~CRenLibView()
{
	delete m_pdcMemory;
	delete m_pBitmap;
}

void CRenLibView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRenLibView)
	DDX_Control(pDX, IDC_GO,                 m_Go);
	DDX_Control(pDX, IDC_STATISTICS,         m_Statistics);
	DDX_Control(pDX, IDC_MULTI_LINE_COMMENT, m_MultiLineComment);
	DDX_Control(pDX, IDC_TAB,                m_Tab);
	DDX_Control(pDX, IDC_ONE_LINE_COMMENT,   m_OneLineComment);
	DDX_Control(pDX, IDC_ADDRESS,            m_Address);
	DDX_Control(pDX, IDC_EXPLORER,           m_Browser);
	//}}AFX_DATA_MAP
}

BOOL CRenLibView::PreCreateWindow(CREATESTRUCT& cs)
{
	// Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CRenLibView::SetPaneText(const CString& text)
{
	CMainFrame* pFrame = (CMainFrame*) AfxGetApp()->GetMainWnd();
	pFrame->SetPaneText(text);
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibView drawing

CString CRenLibView::RowImage(int row)
{
	CString image;

	if (m_bCoordinatesFollow)
	{
		bool falling = !m_bReflect_Y;

		switch (m_ePosition)
		{
			case DEGREE_0:
				image =  Utils::YCoordinateImage(row, falling != m_bSwapRowNumber);
				break;

			case DEGREE_90:
				image =  Utils::XCoordinateImage(row, (m_eCoordinate == AO), !falling);
				break;

			case DEGREE_180:
				image =  Utils::YCoordinateImage(row, !(falling != m_bSwapRowNumber));
				break;

			case DEGREE_270:
				image =  Utils::XCoordinateImage(row, (m_eCoordinate == AO), falling);
				break;
		}
	}
	else
	{
		image = Utils::YCoordinateImage(row, !m_bSwapRowNumber);
	}

	return image;
}

CString CRenLibView::ColImage(int col)
{
	CString image;

	if (m_bCoordinatesFollow)
	{
		bool falling = m_bReflect_X;

		switch (m_ePosition)
		{
			case DEGREE_0:
				image =  Utils::XCoordinateImage(col, (m_eCoordinate == AO), falling);
				break;

			case DEGREE_90:
				image =  Utils::YCoordinateImage(col, falling != m_bSwapRowNumber);
				break;

			case DEGREE_180:
				image =  Utils::XCoordinateImage(col, (m_eCoordinate == AO), !falling);
				break;

			case DEGREE_270:
				image =  Utils::YCoordinateImage(col, !(falling != m_bSwapRowNumber));
				break;
		}
	}
	else
	{
		image =  Utils::XCoordinateImage(col, (m_eCoordinate == AO), false);
	}

	return image;
}

void CRenLibView::DrawBoard(CDC* pDC, CRenLibDoc* pDoc)
{
	int x;
	int y;

	//
	// Background Color
	//
	CPen nullpen(PS_NULL, 0, cBlack);
	CPen* pPen = pDC->SelectObject(&nullpen);

	CBrush backgroundBrush(m_BackgroundColor);
	pDC->SelectObject(&backgroundBrush);

	CRect rect(m_rectBitmap);	
	pDC->Rectangle(rect);
	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->SelectObject(pPen);

	//
	// Draw Board Image
	//
	x = xOffset;
	y = yOffset;

	const int cBoardSize = 16 * m_StoneSize;

	const int xEnd = xOffset + cBoardSize;
	const int yEnd = yOffset + cBoardSize;

	if (m_bShowBoardBitmap)
	{
		CSize imageSize = m_Board.GetSize();

		do
		{
			CSize drawSize(imageSize);

			if ((x + drawSize.cx) >= xEnd)
			{
				drawSize.cx = xEnd - x;
			}

			if ((y + drawSize.cy) >= yEnd)
			{
				drawSize.cy = yEnd - y;
			}

			m_Board.Draw(pDC, CPoint(x,y), drawSize);

			x += imageSize.cx;

			if (x >= xEnd)
			{
				x = xOffset;
				y += imageSize.cy;
			}

		}
		while (y <= yEnd);
	}
	else
	{
		CBrush myBrush(m_BoardColor);
		pDC->SelectObject(&myBrush);
		pDC->Rectangle(CRect(x, y, xEnd, yEnd));
		pDC->SelectStockObject(BLACK_BRUSH);
	}

	//
	// Draw Board Lines
	//
	for (int row = 1; row <= 15; row++)
	{
		x = xOffset + m_StoneSize;
		y = yOffset + m_StoneSize * row;

		pDC->MoveTo(x, y);
		pDC->LineTo(x + m_StoneSize * 14, y);
	}

	for (int col = 1; col <= 15; col++)
	{
		x = xOffset + m_StoneSize * col;
		y = yOffset + m_StoneSize;
		pDC->MoveTo(x, y);
		pDC->LineTo(x, y + m_StoneSize * 14);
	}

	//
	// Draw Help Markers
	//
	DrawHelpMarker(pDC, pDoc,  8,  8);
	DrawHelpMarker(pDC, pDoc,  4,  4);
	DrawHelpMarker(pDC, pDoc,  4, 12);
	DrawHelpMarker(pDC, pDoc, 12,  4);
	DrawHelpMarker(pDC, pDoc, 12, 12);

	//
	// Draw Coordinates
	//
	if (m_eCoordinate != NONE)
	{
		pDC->SetTextColor(m_CoordTextColor);
		pDC->SetBkMode(TRANSPARENT);

		for (row = 15; row >= 1; row--)
		{
			CRect Rect( 0,       yOffset - m_StoneSize/2 + m_StoneSize * row,
						xOffset, yOffset + m_StoneSize/2 + m_StoneSize * row);

			pDC->DrawText(RowImage(row), &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		for (col = 1; col <= 15; col++)
		{
			CRect Rect( xOffset - m_StoneSize/2 + m_StoneSize * col, yOffset + m_StoneSize * 16,
						xOffset + m_StoneSize/2 + m_StoneSize * col, yOffset + m_StoneSize * 17);

			pDC->DrawText(ColImage(col), &Rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		}

		pDC->SetTextColor(cBlack);
		pDC->SetBkColor(cWhite);
		pDC->SetBkMode(OPAQUE);
	}
}

void CRenLibView::DrawHelpMarker(CDC* pDC, CRenLibDoc* pDoc, int x, int y)
{
	CBrush myBrush(cBlack);
	pDC->SelectObject(&myBrush);

	m_rectCircle = CRect( xOffset - 2 + m_StoneSize * x, yOffset - 2 + m_StoneSize * y,
						  xOffset + 2 + m_StoneSize * x, yOffset + 2 + m_StoneSize * y);

	pDC->Rectangle(m_rectCircle);

	pDC->SelectStockObject(BLACK_BRUSH);
}

void CRenLibView::DrawOneVariant(CDC* pDC, CRenLibDoc* pDoc, int x, int y, COLORREF color)
{
	if (m_bShowVariants && (!m_bShowBoardText || pDoc->GetBoard()->GetText(CPoint(x,y)).IsEmpty()))
	{
		CBrush myBrush(color);
		pDC->SelectObject(&myBrush);

		m_rectCircle = CRect( xOffset - m_StoneSize/6 + m_StoneSize * x, yOffset - m_StoneSize/6 + m_StoneSize * y,
							  xOffset + m_StoneSize/6 + m_StoneSize * x, yOffset + m_StoneSize/6 + m_StoneSize * y);

		pDC->Ellipse(m_rectCircle);

		pDC->SelectStockObject(BLACK_BRUSH);
	}
}

void CRenLibView::DrawBoardText(CDC* pDC, CRenLibDoc* pDoc, const CString& strText, int x, int y, bool drawVariantMarker)
{
	if (m_bShowBoardText)
	{
		if (!strText.IsEmpty())
		{
			m_rectCircle = CRect( xOffset - m_StoneSize/2 + m_StoneSize * x, yOffset - m_StoneSize/2 + m_StoneSize * y,
								  xOffset + m_StoneSize/2 + m_StoneSize * x, yOffset + m_StoneSize/2 + m_StoneSize * y);

			if (m_bBoardTextExtraSpace && !m_bShowBoardBitmap)
			{
				pDC->FillSolidRect(m_rectCircle, m_BoardColor);
			}

			pDC->SetTextColor(m_BoardTextColor);
			pDC->SetBkColor(m_BoardColor);
			pDC->SetBkMode(TRANSPARENT);

			LOGFONT lf;
			m_FontBoardText[m_BoardSize].getLogFont(lf);

			if (drawVariantMarker)
			{
				lf.lfUnderline = true;
			}

			CFont Font;
			Font.CreatePointFontIndirect(&lf, pDC);

			CFont* pOldFont = pDC->SelectObject(&Font);

			m_rectCircle.left  -= m_StoneSize/2;
			m_rectCircle.right += m_StoneSize/2;

			pDC->DrawText(strText, &m_rectCircle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

			pDC->SelectObject(pOldFont);
			Font.DeleteObject();

			pDC->SetTextColor(cBlack);
			pDC->SetBkColor(cWhite);
			pDC->SetBkMode(OPAQUE);
		}
	}
}

void CRenLibView::DrawLastMoveMarker(CDC* pDC, CRenLibDoc* pDoc)
{
	CPoint LastMove(pDoc->GetBoard()->GetLastMove());

	LastMove = GetViewCoord(LastMove.x, LastMove.y);

	if (LastMove.x && LastMove.y)
	{
		CBrush myBrush(m_LastMoveColor);
		pDC->SelectObject(&myBrush);
		pDC->Rectangle(CRect(xOffset - m_StoneSize/10 + m_StoneSize * LastMove.x,
			                 yOffset - m_StoneSize/10 + m_StoneSize * LastMove.y,
							 xOffset + m_StoneSize/10 + m_StoneSize * LastMove.x,
							 yOffset + m_StoneSize/10 + m_StoneSize * LastMove.y));

		pDC->SelectStockObject(BLACK_BRUSH);
	}
}

void CRenLibView::DrawOneCircle(CDC* pDC, CRenLibDoc* pDoc, int x, int y, COLORREF color, int number)
{
	const bool isLastMove = (m_bShowLastMove && pDoc->GetCurrentMove() == number);

	m_rectCircle = CRect( xOffset - m_StoneSize/2 + m_StoneSize * x,
		                  yOffset - m_StoneSize/2 + m_StoneSize * y,
						  xOffset + m_StoneSize/2 + m_StoneSize * x,
						  yOffset + m_StoneSize/2 + m_StoneSize * y);

	if (m_bShowStoneBitmap)
	{
		const int xPos = xOffset - m_StoneSize/2 + m_StoneSize * x;
		const int yPos = yOffset - m_StoneSize/2 + m_StoneSize * y;

		if (number & 1)
		{
			m_BlackStone.DrawTransparent(pDC, xPos, yPos, cRed);
		}
		else
		{
			m_WhiteStone.DrawTransparent(pDC, xPos, yPos, cRed);
		}
	}
	else
	{
		CBrush myBrush(color);
		pDC->SelectObject(&myBrush);
		pDC->Ellipse(m_rectCircle);
		pDC->SelectStockObject(BLACK_BRUSH);
	}

	//
	// Last move marker
	//
	if (isLastMove)
	{
		//
		// Last Move
		//
		CRenLibDoc* pDoc = GetDocument();
		DrawLastMoveMarker(pDC, pDoc);
	}

	//
	// Numbers
	//
	if (m_bShowNumbers)
	{
		CString strNum;
		strNum.Format("%d", number);

		if (color == m_BlackStoneColor)
		{
			pDC->SetTextColor(m_BlackNumberColor);

			if (isLastMove)
			{
				pDC->SetBkColor(m_LastMoveColor);
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
			}
		}
		else
		{
			pDC->SetTextColor(m_WhiteNumberColor);

			if (isLastMove)
			{
				pDC->SetBkColor(m_LastMoveColor);
			}
			else
			{
				pDC->SetBkMode(TRANSPARENT);
			}
		}

		pDC->DrawText(strNum, &m_rectCircle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		pDC->SetBkMode(OPAQUE);
		pDC->SetTextColor(cBlack);
		pDC->SetBkColor(cWhite);
	}
}

void CRenLibView::DrawWin(CDC* pDC, int x, int y, const CString& strVcf, COLORREF color)
{
	CPen penvcf(PS_SOLID, 1, color);
	CPen* pPen = pDC->SelectObject(&penvcf);

	pDC->SetBkMode(TRANSPARENT);

	CBrush myBrush(m_BoardColor);
	pDC->SelectObject(&myBrush);

	m_rectCircle = CRect( xOffset - m_StoneSize/2 + m_StoneSize * x, yOffset - m_StoneSize/2 + m_StoneSize * y,
						  xOffset + m_StoneSize/2 + m_StoneSize * x, yOffset + m_StoneSize/2 + m_StoneSize * y);

	CPoint startEndPoint(m_rectCircle.right, m_rectCircle.CenterPoint().y);
	pDC->Arc(m_rectCircle, startEndPoint, startEndPoint);

	pDC->SelectStockObject(BLACK_BRUSH);
	pDC->SelectObject(pPen);


	pDC->SetTextColor(color);
	pDC->SetBkColor(m_BoardColor);

	pDC->DrawText(strVcf, &m_rectCircle, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

	pDC->SetBkMode(OPAQUE);
	pDC->SetTextColor(cBlack);
	pDC->SetBkColor(cWhite);
}

void CRenLibView::DrawForbidden(CDC* pDC, CRenLibDoc* pDoc, int x, int y)
{
	if (m_bShowForbidden)
	{
		m_rectCircle = CRect( xOffset - m_StoneSize/4 + m_StoneSize * x, yOffset - m_StoneSize/4 + m_StoneSize * y,
							  xOffset + m_StoneSize/4 + m_StoneSize * x, yOffset + m_StoneSize/4 + m_StoneSize * y);

		CPen penred(PS_SOLID, 2, m_ForbiddenColor);
		CPen* pPen = pDC->SelectObject(&penred);

		pDC->MoveTo(m_rectCircle.left,  m_rectCircle.bottom);
		pDC->LineTo(m_rectCircle.right, m_rectCircle.top);

		pDC->MoveTo(m_rectCircle.left,  m_rectCircle.top);
		pDC->LineTo(m_rectCircle.right, m_rectCircle.bottom);

		pDC->SelectObject(pPen);
	}
}

CPoint CRenLibView::GetViewCoord(int x, int y)
{
	CPoint ViewCoord;

	switch (m_ePosition)
	{
		case DEGREE_0:
			ViewCoord = CPoint(x-8, y-8);
			break;

		case DEGREE_90:
			ViewCoord = CPoint(8-y, x-8);
			break;

		case DEGREE_180:
			ViewCoord = CPoint(8-x, 8-y);
			break;

		case DEGREE_270:
			ViewCoord = CPoint(y-8, 8-x);
			break;
	}

	ViewCoord += CPoint(8,8);

	if (m_bReflect_Y)
	{
		ViewCoord.y = 16 - ViewCoord.y;
	}

	if (m_bReflect_X)
	{
		ViewCoord.x = 16 - ViewCoord.x;
	}

	return ViewCoord;
}

CPoint CRenLibView::GetInverseViewCoord(int x, int y)
{
	CPoint ViewCoord;

	if (m_bReflect_Y)
	{
		y = 16 - y;
	}

	if (m_bReflect_X)
	{
		x = 16 - x;
	}

	switch (m_ePosition)
	{
		case DEGREE_0:
			ViewCoord = CPoint(x-8, y-8);
			break;

		case DEGREE_90:
			ViewCoord = CPoint(y-8, 8-x);
			break;

		case DEGREE_180:
			ViewCoord = CPoint(8-x, 8-y);
			break;

		case DEGREE_270:
			ViewCoord = CPoint(8-y, x-8);
			break;
	}

	ViewCoord += CPoint(8,8);

	return ViewCoord;
}

void CRenLibView::GetBoardView(Board& aBoard)
{
	CRenLibDoc* pDoc = GetDocument();
	Board::BoardMarker marker;

	aBoard.Clear();

	for (int x=1; x<=15; x++)
	{
		for (int y=1; y<=15; y++)
		{
			marker = pDoc->GetBoard()->Get(CPoint(x,y));

			if (marker == Board::BLACK_MOVE || marker == Board::WHITE_MOVE)
			{
				CPoint ViewCoord(GetViewCoord(x,y));
 				aBoard.Set(ViewCoord, marker);
			}
		}
	}
}

void CRenLibView::DrawCircles(CDC* pDC, CRenLibDoc* pDoc)
{
	Board* board = pDoc->GetBoard();
	Board::BoardMarker marker;

	//
	// Draw Circles or Forbidden marker
	//
	for (int x=1; x<=15; x++)
	{
		for (int y=1; y<=15; y++)
		{
			const CPoint Pos(x,y);
			CPoint ViewCoord(GetViewCoord(x,y));

			marker = board->Get(Pos);

			int number = board->GetNumber(Pos);

			switch (marker)
			{
				case Board::BLACK_MOVE:
					DrawOneCircle(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_BlackStoneColor, number);
					break;

				case Board::WHITE_MOVE:
					DrawOneCircle(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_WhiteStoneColor, number);
					break;

				case Board::BLACK_VARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_BlackVariantColor);
					break;

				case Board::WHITE_VARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_WhiteVariantColor);
					break;

				case Board::BLACK_AUTOLINKVARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_BlackALVColor);
					break;

				case Board::WHITE_AUTOLINKVARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_WhiteALVColor);
					break;

				case Board::BLACK_MOVEORDERVARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_BlackCMOColor);
					break;

				case Board::WHITE_MOVEORDERVARIANT:
					DrawOneVariant(pDC, pDoc, ViewCoord.x,  ViewCoord.y, m_WhiteCMOColor);
					break;

				default:
					break;
			}
		}
	}

	//
	// Draw Forbidden and Board Text
	//
	for (x=1; x<=15; x++)
	{
		for (int y=1; y<=15; y++)
		{
			const CPoint Pos(x,y);
			CPoint ViewCoord(GetViewCoord(x,y));

			marker = board->Get(Pos);
			bool drawVariantMarker = m_bShowVariants && marker;

			if (board->isForbidden(Pos))
			{
				DrawForbidden(pDC, pDoc, ViewCoord.x,  ViewCoord.y);
			}

			CString strText(board->GetText(Pos));
			DrawBoardText(pDC, pDoc, strText, ViewCoord.x,  ViewCoord.y, drawVariantMarker);
		}
	}

	//
	// Draw VCF
	//
    if (pDoc->IsWinPosition(CRenLibDoc::VCF))
	{
		COLORREF color = (pDoc->GetCurrentMove() & 1) ? m_WhiteVcfColor : m_BlackVcfColor;

		for (x=1; x<=15; x++)
		{
			for (int y=1; y<=15; y++)
			{
				const CPoint Pos(x,y);
				CPoint ViewCoord(GetViewCoord(x,y));

				CString strWin = board->GetWin(Pos);

				if (!strWin.IsEmpty())
				{
					DrawWin(pDC, ViewCoord.x, ViewCoord.y, strWin, color);
				}
			}
		}
	}

    //
	// Draw VCT
	//
    if (pDoc->IsWinPosition(CRenLibDoc::VCT))
	{
		COLORREF color = (pDoc->GetCurrentMove() & 1) ? m_WhiteVctColor : m_BlackVctColor;

		for (x=1; x<=15; x++)
		{
			for (int y=1; y<=15; y++)
			{
				const CPoint Pos(x,y);
				CPoint ViewCoord(GetViewCoord(x,y));

				CString strWin = board->GetWin(Pos);

				if (!strWin.IsEmpty())
				{
					DrawWin(pDC, ViewCoord.x, ViewCoord.y, strWin, color);
				}
			}
		}
	}
}

void CRenLibView::DrawComment(CDC* pDC, CRenLibDoc* pDoc)
{
	bool doUpdateOneLineComment   = false;
	bool doUpdateMultiLineComment = false;

	if (m_LastMoveNode != pDoc->GetCurrentMoveNode())
	{
		doUpdateOneLineComment   = true;
		doUpdateMultiLineComment = true;
	}

	//
	// One Line Comment and Multi Line Comment
	//
	CString strOneLineComment;
	CString strMultiLineComment;

	if (m_bShowComments || pDoc->isUpdateMode())
	{
		strOneLineComment   = pDoc->getOneLineComment();
		strMultiLineComment = pDoc->getMultiLineComment();
	}

	LOGFONT lf;
	m_FontComment[m_BoardSize].getLogFont(lf);

	if (m_strOneLineComment != strOneLineComment)
	{
		m_strOneLineComment = strOneLineComment;
		doUpdateOneLineComment = true;
	}

	if (m_strMultiLineComment != strMultiLineComment)
	{
		m_strMultiLineComment = strMultiLineComment;
		doUpdateMultiLineComment = true;
	}

	m_OneLineComment.SetPointFontIndirect(&lf);
	m_OneLineComment.SetTextColor(m_CommentTextColor);
	m_OneLineComment.SetBackgroundColor(m_CommentBkColor);

	if (doUpdateOneLineComment)
	{
		m_OneLineComment.SetWindowText(strOneLineComment);
	}

	m_MultiLineComment.SetPointFontIndirect(&lf);
	m_MultiLineComment.SetTextColor(m_CommentTextColor);
	m_MultiLineComment.SetBackgroundColor(m_CommentBkColor);

	if (doUpdateMultiLineComment)
	{
		m_MultiLineComment.SetWindowText(strMultiLineComment);
	}

	m_Tab.SetFocus();
}

void CRenLibView::AutopointMouse(CRenLibDoc* pDoc)
{
	if (m_bAutoPointMouse && !m_bToolBarMoveCommand)
	{
		CPoint PreviousVariant = pDoc->GetBoard()->GetPreviousVariant();

		if (PreviousVariant.x > 0 && PreviousVariant.y > 0)
		{
			PreviousVariant = GetViewCoord(PreviousVariant.x, PreviousVariant.y);

			int x = PreviousVariant.x;
			int y = PreviousVariant.y;

			m_rectCircle = CRect( xOffset - m_StoneSize/6 + m_StoneSize * x, yOffset - m_StoneSize/6 + m_StoneSize * y,
								  xOffset + m_StoneSize/6 + m_StoneSize * x, yOffset + m_StoneSize/6 + m_StoneSize * y);

			CClientDC dc(this);
			OnPrepareDC(&dc);
			dc.LPtoDP(m_rectCircle);
			CPoint pos ((m_rectCircle.left + m_rectCircle.right) / 2,
						(m_rectCircle.top  + m_rectCircle.bottom) / 2);
			ClientToScreen(&pos);
			SetCursorPos(pos.x, pos.y);
		}
	}

	pDoc->GetBoard()->SetPreviousVariant(CPoint(0, 0)); // Disable autopoint
	m_bToolBarMoveCommand = false;
}

void CRenLibView::UpdateNextAutoBoardText(CRenLibDoc* pDoc)
{
	if (m_bAutoEnterBoardText)
	{
		m_NextAutoBoardText = pDoc->GetHighestAutoBoardText();

		if (m_NextAutoBoardText.IsEmpty())
		{
			m_NextAutoBoardText = strDefaultNextAutoBoardText;
		}
		else
		{
			Utils::Increment(m_NextAutoBoardText);
		}
	}
}

void CRenLibView::OnDraw(CDC* pDC)
{
	CRenLibDoc* pDoc = GetDocument();

	CFont Font;

	LOGFONT lf;
	m_Font[m_BoardSize].getLogFont(lf);

	VERIFY(Font.CreatePointFontIndirect(&lf, pDC));

	CFont* pOldFont = pDC->SelectObject(&Font);

	pDoc->SetTitle();

	pDoc->UpdateBoard(m_bShowAutoLinkVariants, m_bShowChangedOrderOfMovesVariants);
	
	DrawBoard(pDC, pDoc);
	DrawCircles(pDC, pDoc);
	DrawComment(pDC, pDoc);
	AutopointMouse(pDoc);

	pDC->SelectObject(pOldFont);
	Font.DeleteObject();

	if (m_LastMoveNode != pDoc->GetCurrentMoveNode())
	{
		UpdateNextAutoBoardText(pDoc);

		m_LastMoveNode = pDoc->GetCurrentMoveNode();
	}

	if (m_BoardBitmapState == DEFAULT_BITMAP)
	{
		PostMessage(WM_COMMAND, ID_VIEW_COLOR_BOARD_DEFAULT_BITMAP);
		m_BoardBitmapState = CHOOSE_BITMAP;
	}
	else if (m_BoardBitmapState == CHOOSE_BITMAP)
	{
		PostMessage(WM_COMMAND, ID_VIEW_COLOR_BOARD_CHOOSE_BITMAP);
		m_BoardBitmapState = INVALID_BITMAP;
	}
}

void CRenLibView::UpdateScrollSizes()
{
	const int sizeTotalX = xOffset + m_StoneSize * cRight;
	const int sizeTotalY = yOffset + m_StoneSize * 17;

	CSize sizeTotal(sizeTotalX, sizeTotalY);
	CSize sizePage (sizeTotal.cx, sizeTotal.cy);
	CSize sizeLine (sizeTotal.cx / 50, sizeTotal.cy / 50);
	SetScrollSizes(MM_TEXT, sizeTotal, sizePage, sizeLine);

	CreateMemoryDeviceContext();

	GetParentFrame()->RecalcLayout();
	ResizeParentToFit(FALSE);
}

int CRenLibView::defaultSize()
{
	const int cScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	if (cScreenHeight <= 600)
	{
		return cSizeSmall;
	}
	else if (cScreenHeight <= 864)
	{
		return cSizeMedium;
	}
	else
	{
		return cSizeLarge;
	}
}

void CRenLibView::adjustSize()
{
	adjustControls();
}

void CRenLibView::OnInitialUpdate()
{
	m_bInitialUpdate = true;

	CFormView::OnInitialUpdate();

	UpdateScrollSizes();

	if (TabCtrl_GetItemCount(m_Tab) == 0)
	{
		createTabControl();
		createListControl();
		adjustSize();
	}

	indicateConvertOrMerge(Utils::GetString(IDS_CAP_OPEN));

    mTimerState = TIMER_VERSION_NEWS;
	m_nTimer = SetTimer(cTimerId, cTimeVersionNews, 0);
}

void CRenLibView::createTabControl()
{
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;

	CString strTab;

	strTab = Utils::GetString(IDS_TAB_COMMENT);
	tcItem.pszText = (LPSTR) strTab.GetBufferSetLength(strTab.GetLength());
	m_Tab.InsertItem(TAB_COMMENT, &tcItem);

	strTab = Utils::GetString(IDS_TAB_STATISTICS);
	tcItem.pszText = (LPSTR) strTab.GetBufferSetLength(strTab.GetLength());
	m_Tab.InsertItem(TAB_STATISTICS, &tcItem);

	strTab = Utils::GetString(IDS_TAB_BROWSER);
	tcItem.pszText = (LPSTR) strTab.GetBufferSetLength(strTab.GetLength());
	m_Tab.InsertItem(TAB_BROWSER, &tcItem);

	// Show comment
	showCommentTab();

	m_OneLineComment.SetModeInformation(this);
	m_MultiLineComment.SetModeInformation(this);

	initiateAddressUrl();
	navigate(mStartPage);
}

void CRenLibView::createListControl()
{
	CRect rect;
	m_Statistics.GetClientRect(&rect);

	for (int i = 0; i < cNoOfColumns; i++)
	{
		m_Statistics.SetColumnWidth(i, rect.Width() / cNoOfColumns);
	}

	const UINT cIds[cNoOfColumns] =
	{
		IDS_TAB_COMMENT, 
		IDS_TAB_STATISTICS
	};

	CString header[cNoOfColumns];

	for (i = 0; i < cNoOfColumns; i++)
	{
		header[i].LoadString(cIds[i]);
		m_Statistics.InsertColumn(i, header[i], LVCFMT_LEFT, rect.Width() / cNoOfColumns, i);
	}
}

void CRenLibView::CreateMemoryDeviceContext()
{
	if (m_bInitialUpdate)
	{
		ScrollToPosition(CPoint(0,0));

		if (m_pdcMemory != 0)
		{
			delete m_pdcMemory;
			delete m_pBitmap;
		}

		m_pdcMemory = new CDC;
		m_pBitmap   = new CBitmap;

		// creates the memory device context and the bitmap
		if (m_pdcMemory->GetSafeHdc() == NULL)
		{
			CRenLibDoc* pDoc = GetDocument();

			CClientDC dc(this);
			OnPrepareDC(&dc);

			CWnd::GetClientRect(&m_rectBitmap);

			dc.DPtoLP(m_rectBitmap);

			m_rectBitmap.right  = xOffset + m_StoneSize * 17;
			m_rectBitmap.bottom =
				__max(m_rectBitmap.bottom, yOffset + m_StoneSize * 17),

			dc.LPtoDP(m_rectBitmap);

			m_pdcMemory->CreateCompatibleDC(&dc);
			m_pdcMemory->SetMapMode(MM_TEXT);

			// makes bitmap same size as display window
			m_pBitmap->CreateCompatibleBitmap(&dc, m_rectBitmap.right, m_rectBitmap.bottom);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// CRenLibView diagnostics

#ifdef _DEBUG
void CRenLibView::AssertValid() const
{
	CFormView::AssertValid();
}

void CRenLibView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CRenLibDoc* CRenLibView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRenLibDoc)));
	return (CRenLibDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRenLibView message handlers

bool CRenLibView::handleKeyDown(UINT nChar, UINT lParam)
{
    if (GetDocument()->isFindActive())
    {
        return true;
    }

	bool isHandled     = false;
    bool isControlDown = (GetKeyState(VK_CONTROL) & 0x80) != 0;

    SHORT functionKeyState =
        GetKeyState(VK_F1) |
        GetKeyState(VK_F2) |
        GetKeyState(VK_F3) |
        GetKeyState(VK_F4) |
        GetKeyState(VK_F5) |
        GetKeyState(VK_F6) |
        GetKeyState(VK_F7) |
        GetKeyState(VK_F8) |
        GetKeyState(VK_F9) |
        GetKeyState(VK_F10)|
        GetKeyState(VK_F11)|
        GetKeyState(VK_F12);

	if ((functionKeyState & 0x80) != 0)
    {
        return isHandled;
    }

	if (m_bAddressHasFocus && nChar == VK_RETURN)
	{
		// User hit return in Browser address field
		// Interpret as click on Go button
		PostMessage(WM_COMMAND, IDC_GO);
		isHandled = true;
	}
	else
	if (!isControlDown && 
		!m_bMultiLineCommentHasFocus && 
		!m_bOneLineCommentHasFocus && 
		!m_bAddressHasFocus &&
		m_Tab.GetCurSel() != TAB_BROWSER)
	{
		switch (nChar)
		{
			case '1':
			case VK_HOME:
			{
				OnMove1stMove();
				isHandled = true;
				break;
			}

			case 'l':
			case 'L':
			case VK_END:
			{
				OnMoveLastMove();
				isHandled = true;
				break;
			}

			case 'u':
			case 'U':
			case VK_UP:
			case VK_PRIOR:
			{
				OnMoveUp();
				isHandled = true;
				break;
			}

			case 'd':
			case 'D':
			case VK_DOWN:
			case VK_NEXT:
			{
				OnMoveDown();
				isHandled = true;
				break;
			}

			case 'b':
			case 'B':
			case VK_LEFT:
			{
				OnMoveBack();
				isHandled = true;
				break;
			}

			case 'f':
			case 'F':
			case VK_RIGHT:
			{
				OnMoveForward();
				isHandled = true;
				break;
			}

			case VK_DELETE:
			case VK_BACK:
			{
				OnEditDeleteMove();
				isHandled = true;
				break;
			}

			default:
			{
				break;
			}
		}
	}

	return isHandled;
}

void CRenLibView::CalcMouseCoord(CPoint& point)
{
	CClientDC dc(this);
	OnPrepareDC(&dc);
	CRect rectDevice(xOffset + m_StoneSize/2,                   // Left
		             yOffset + m_StoneSize/2,                   // Top
		             xOffset + m_StoneSize/2 + 15*m_StoneSize,  // Right
					 yOffset + m_StoneSize/2 + 15*m_StoneSize); // Bottom

	dc.LPtoDP(rectDevice);

	if (rectDevice.PtInRect(point))
	{
		CRect WindowRect;
		AfxGetApp()->m_pMainWnd->GetWindowRect(&WindowRect);

		point.x -= rectDevice.left;
		point.y -= rectDevice.top;

		double dx = (rectDevice.right - rectDevice.left) / 15;
		double dy = (rectDevice.bottom - rectDevice.top) / 15;
		
		int x = (int)(point.x / dx + 1.0);
		if (x > 15) x = 15;

		int y = (int)(point.y / dy + 1.0);
		if (y > 15) y = 15;

		m_MouseCoord = CPoint(x,y);
	}
	else
	{
		m_MouseCoord = (0,0);
	}
}

void CRenLibView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	CalcMouseCoord(point);
	m_Tab.SetFocus();

	if (m_MouseCoord != CPoint(0,0))
	{
		const CPoint Coord = GetInverseViewCoord(m_MouseCoord.x, m_MouseCoord.y);

		if (nFlags & MK_CONTROL)
		{
			if (pDoc->isUpdateMode())
			{
				CString boardText = pDoc->GetBoardText(Coord);

				if (m_bAutoEnterBoardText && boardText.IsEmpty())
				{
					pDoc->BoardTextCommand(Coord, m_NextAutoBoardText);
					Invalidate(FALSE);

					Utils::Increment(m_NextAutoBoardText);
				}
				else
				{
					EditBoardText BoardText;
					BoardText.m_BoardText = boardText;
					BoardText.m_strFaceName = m_FontBoardText[m_BoardSize].getFaceName();

					if (BoardText.DoModal() == IDOK)
					{
						pDoc->BoardTextCommand(Coord, BoardText.m_BoardText);
						Invalidate(FALSE);
					}
				}
			}
		}
		else if (!(nFlags & MK_SHIFT))
		{
			const int currentMove = pDoc->GetCurrentMove();

			// Get board position before click
			Board currentBoard;
			GetBoardView(currentBoard);

			bool hasChangedBranch;
			pDoc->ButtonCommand(Coord, m_bShowVariants, hasChangedBranch);

			if (hasChangedBranch)
			{
				const int nextMove = pDoc->GetCurrentMove();

				if (nextMove > currentMove)
				{
					if (nextMove & 1)
					{
						currentBoard.Set(m_MouseCoord, Board::BLACK_MOVE); 
					}
					else
					{
						currentBoard.Set(m_MouseCoord, Board::WHITE_MOVE); 
					}
				}

				//
				// Save current view attributes
				//
				m_ePreviousPosition  = m_ePosition;
				m_bPreviousReflect_X = m_bReflect_X;
				m_bPreviousReflect_Y = m_bReflect_Y;
				
				//
				// Board position after click
				//
				Board nextBoard;
				bool found = false;
				
				for (int n=1; n <= 8; n++)
				{
					if (n == 5)
					{
						if (m_bReflect_X)
						{
							OnPositionReflectLeftRight();
						}
						else
						{
							OnPositionReflectUpDown();
						}
					}
					
					pDoc->UpdateBoard(false, false);
					GetBoardView(nextBoard);
					
					if (currentBoard.isEqual(nextBoard))
					{
						// Views are same
						found = true;
						break;
					}
					
					OnPositionTurnPlus90degrees();
				}
				
				if (!found)
				{
					//
					// Set to previous view attributes
					// eg. when clicking on no variant in display mode
					//
					m_ePosition  = m_ePreviousPosition;
					m_bReflect_X = m_bPreviousReflect_X;
					m_bReflect_Y = m_bPreviousReflect_Y;
				}
			}

			if (m_bAutoFindSamePosition)
			{
				pDoc->autoFindSamePositionCommand();
			}
		}

		Invalidate(FALSE);
	}
}

void CRenLibView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CalcMouseCoord(point);
	CFormView::OnMouseMove(nFlags, point);
}

void CRenLibView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	CalcMouseCoord(point);
	m_Tab.SetFocus();

    if (pDoc->isProblemMode())
    {
        if (m_MouseCoord != CPoint(0,0))
	    {
		    const CPoint Coord = GetInverseViewCoord(m_MouseCoord.x, m_MouseCoord.y);
		    pDoc->RButtonCommand(Coord);
        }
    }
    else
    {
	    OnMoveBack();
    }

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileOpen(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnFileOpen()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	CString strFile;

	if (Utils::OpenFileDialog(strFile))
	{
		pDoc->OnOpenDocument(strFile);
	}

	Invalidate(FALSE);
}

void CRenLibView::OnMoveBack() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->BackCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnMoveDown() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->DownCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnMoveUp() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->UpCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnMoveForward() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->ForwardCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnAppExit() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->OnCloseDocument();
}

void CRenLibView::OnFileSave() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->SaveCommand(false);
	Invalidate(FALSE);
}

void CRenLibView::OnFileSaveLibraryAs() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->SaveCommand(false, true);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
    CRenLibDoc* pDoc = GetDocument();
    pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnUpdateFileSave(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsModified());
}

void CRenLibView::OnUpdateEditUpdateMode(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc = GetDocument();

	if (!pDoc->isFindActive() && pDoc->GetCurrentMove() >= 0)
	{
		if (pDoc->isUpdateMode() || pDoc->isProblemMode())
		{
			pCmdUI->SetText(Utils::GetString(IDS_MENU_DISPLAY_MODE));
			pCmdUI->SetCheck(true);
		}
		else
		{
			pCmdUI->SetText(Utils::GetString(IDS_MENU_UPDATE_MODE));
			pCmdUI->SetCheck(false);
		}
		pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CRenLibView::OnEditUpdateMode()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();

	if (pDoc->isDisplayMode())
	{
        pDoc->SetMode(CRenLibDoc::UPDATE_MODE);
		m_Tab.SetFocus();
	}
    else
	{
	    pDoc->SetMode(CRenLibDoc::DISPLAY_MODE);
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditProblemMode(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();

	if (!pDoc->isFindActive() && pDoc->GetCurrentMove() >= 0 && 
        !pDoc->isProblemMode() && !pDoc->hasVariants())
	{
		if (pDoc->isProblemMode())
		{
			pCmdUI->SetCheck(true);
		}
		else
		{
			pCmdUI->SetCheck(false);
		}

        pCmdUI->Enable(true);
	}
	else
	{
		pCmdUI->Enable(false);
	}
}

void CRenLibView::OnEditProblemMode() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	pDoc->SetMode(CRenLibDoc::PROBLEM_MODE);

	if (m_bShowNumbers)
    {
        OnViewShowNumbers();
    }

	if (m_bShowLastMove)
    {
        OnViewShowLastMove();
    }

    Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditDeleteMove(CCmdUI* pCmdUI) 
{	
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnEditDeleteMove() 
{	
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->DeleteCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditDeleteVariants(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->NoOfVariantMoves() > 0);
}

void CRenLibView::OnEditDeleteVariants() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->DeleteVariantsCommand();

	Invalidate(FALSE);
}

void CRenLibView::OnEditMark() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->MarkCommand();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditMark(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() >= 0);
	pCmdUI->SetCheck(pDoc->IsMark());
}

void CRenLibView::OnEditFindMark() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->FindMarkCommand())
	{
		pDoc->FindNextCommand();
	}
	else
	{
		Utils::Beep();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindMark(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnEditFindComment() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	m_SearchComment.m_strComment  = m_strFindComment;
	m_SearchComment.m_strFaceName = m_FontComment[m_BoardSize].getFaceName();
	m_SearchComment.mFindOption   = 0;

	if (m_SearchComment.DoModal() == IDOK)
	{
		m_strFindComment = m_SearchComment.m_strComment;

		if (m_SearchComment.mFindOption == 0)
		{
			if (pDoc->FindCommentCommand(m_SearchComment.m_strComment))
			{
				pDoc->FindNextCommand();
			}
			else
			{
				Utils::Beep();
			}
		}
		else
		{
			if (pDoc->FindBoardTextCommand(m_SearchComment.m_strComment))
			{
				pDoc->FindNextCommand();
			}
			else
			{
				Utils::Beep();
			}
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindComment(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnEditFindMove() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	bool Done = false;

	do
	{
		SearchMove MoveDialog;

		if (m_SearchMoveNo >= 1 && m_SearchMoveNo <= 225)
		{
			MoveDialog.m_strMoveNo.Format("%d", m_SearchMoveNo);
		}

		if (MoveDialog.DoModal() == IDOK)
		{
			if (MoveDialog.m_strMoveNo.IsEmpty())
			{
				m_SearchMoveNo = 999;
			}
			else
			{
				if (Utils::StringToInteger(MoveDialog.m_strMoveNo, m_SearchMoveNo))
				{
					Done = (m_SearchMoveNo >= 1 && m_SearchMoveNo <= 225);
				}
				else
				{
					m_SearchMoveNo = 0;
				}
			}

			if (m_SearchMoveNo >= 1)
			{
				if (m_SearchMoveNo > pDoc->GetCurrentMove())
				{
					if (pDoc->FindMoveCommand(m_SearchMoveNo))
					{
						pDoc->FindNextCommand();
					}
					else
					{
						Utils::Beep();
					}
					break;
				}
				else
				{
					while (m_SearchMoveNo < pDoc->GetCurrentMove())
					{
						pDoc->BackCommand();
					};
				}
			}
		}
		else
		{
			break;
		}
	} while (!Done);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindMove(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnUpdateMoveBack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnUpdateMoveDown(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsDownCommand());
}

void CRenLibView::OnUpdateMoveForward(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsForwardCommand());
}

void CRenLibView::OnUpdateMoveUp(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnViewSizeLarge() 
{
	m_BoardSize = cSizeLarge;
	loadStoneBitmap();
	UpdateScrollSizes();
	adjustSize();
	Invalidate(TRUE);
}

void CRenLibView::OnUpdateViewSizeLarge(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_BoardSize == cSizeLarge);
}

void CRenLibView::OnViewSizeMedium() 
{
	m_BoardSize = cSizeMedium;
	loadStoneBitmap();
	UpdateScrollSizes();
	adjustSize();
	Invalidate(TRUE);
}

void CRenLibView::OnUpdateViewSizeMedium(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_BoardSize == cSizeMedium);
}

void CRenLibView::OnViewSizeSmall() 
{
	m_BoardSize = cSizeSmall;
	loadStoneBitmap();
	UpdateScrollSizes();
	adjustSize();
	Invalidate(TRUE);
}

void CRenLibView::OnUpdateViewSizeSmall(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_BoardSize == cSizeSmall);
}

void CRenLibView::OnUpdateViewSizeDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewSizeDefault() 
{
	m_BoardSize = defaultSize();
	loadStoneBitmap();
	adjustSize();
	UpdateScrollSizes();
	Invalidate(TRUE);
}

void CRenLibView::OnUpdateFileNew(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnFileNew() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->SaveCommand(true))
	{
		pDoc->NewLibraryCommand();
		
		bool hasChangedBranch;
		pDoc->ButtonCommand(Center, false, hasChangedBranch);

		Invalidate(FALSE);
	}
}

void CRenLibView::OnMove1stMove() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->FirstCommand();

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateMove1stMove(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::readFile(CStringList& fileNameList)
{	
	CString strCaption(Utils::GetString(IDS_MSG_READING_FILE));

	CRenLibDoc* pDoc = GetDocument();
	pDoc->initMergeComment();
	
	CString strFile;
	int nTotalFiles = fileNameList.GetCount();
	
	if (nTotalFiles)
	{
		CProgressWnd wndProgress(this, Utils::GetString(IDS_CAP_READING_FILES, nTotalFiles));
		
		wndProgress.GoModal();
		wndProgress.SetRange(0,nTotalFiles);
		Invalidate(FALSE);
		
		POSITION pos = fileNameList.GetHeadPosition();

		int n = 0;

		while (pos != NULL)
		{
			strFile = fileNameList.GetNext(pos);
			
			CString strText;
			strText.Format("%s : %d\n%s", Utils::GetString(IDS_MSG_READING_FILE), ++n, strFile);
			wndProgress.SetText(strText);

			bool success = true;

			if (Utils::IsExtensionLib(strFile))
			{
				if (!pDoc->MergeLibraryCommand(strFile))
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionTxt(strFile))
			{
                Game aGame;
				const bool multipleGames = true;

                if (aGame.open(strFile, multipleGames))
                {
                    while (true)
                    {
                        Game::Result result = aGame.addGame();

		                if (result == Game::VALID)
		                {
                            pDoc->AddGame(aGame);
		                }
		                else if (result == Game::INVALID_KIND)
		                {
                            break;
		                }
                    }

                    aGame.close();
                }
		        else
		        {
					success = false;
				}
			}
			else if (Utils::IsExtensionWzq(strFile))
			{
				Wzq	aWzq;

				if (aWzq.OpenFile(strFile))
				{
					pDoc->AddWzq(aWzq);
				}
				else
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionBuf(strFile))
			{
				Buf	aBuf;

				if (aBuf.OpenFile(strFile))
				{
					pDoc->AddBuf(aBuf);
				}
				else
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionPdb(strFile))
			{
				Pdb aPdb;

				if (aPdb.OpenFile(strFile))
				{
					pDoc->AddPdb(aPdb);
				}
				else
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionPos(strFile))
			{
				Pos aPos;

				if (aPos.Open(strFile))
				{
					pDoc->AddPos(aPos);
				}
				else
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionRenjs(strFile))
			{
				RenJS renjs;

				if (renjs.addGame(strFile) == RenJS::VALID)
				{
					success = TRUE;
					pDoc->AddRenJS(renjs);
				}
				else
				{
					success = false;
				}
			}
			else if (Utils::IsExtensionSgf(strFile))
			{
				Sgf	aSgf;
				const bool multipleGames = true;

				if (aSgf.OpenFile(strFile, multipleGames))
				{
					pDoc->AddSgf(aSgf);
				}
				else
				{
					success = false;
				}
			}
	        else if (Utils::IsExtensionBdt(strFile))
	        {
		        Bdt	aBdt;

		        if (aBdt.OpenFile(strFile))
		        {
			        pDoc->AddBdt(aBdt);
		        }
				else
				{
					success = false;
				}
	        }
	        else if (Utils::IsExtensionRdf(strFile))
	        {
		        Rdf	aRdf;

		        if (aRdf.OpenFile(strFile))
		        {
			        pDoc->AddRdf(aRdf);
		        }
				else
				{
					success = false;
				}
	        }

			if (!success)
			{
				CString strMessage(Utils::GetString(IDS_MSG_OPEN_FILE, strFile));
				strMessage += "\n\n";
				strMessage += Utils::GetString(IDS_MSG_OK_CANCEL);
				
				if (Utils::ShowMessage(
						strMessage, strCaption, MB_ICONERROR + MB_OKCANCEL) == IDCANCEL)
				{
					break;
				}
			}

			wndProgress.StepIt();
			wndProgress.PeekAndPump();

			if (wndProgress.Cancelled())
			{
				CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
				Utils::ShowMessage(strMessage, strCaption, MB_ICONWARNING);
				break;
			}
		}
	}

	indicateConvertOrMerge(strCaption);

	Invalidate(FALSE);
}

void CRenLibView::OnViewCoordinatesAo() 
{
	m_eCoordinate = AO;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewCoordinatesAo(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_eCoordinate == AO);
}

void CRenLibView::OnViewCoordinatesAp() 
{
	m_eCoordinate = AP;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewCoordinatesAp(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_eCoordinate == AP);
}

void CRenLibView::OnViewCoordinatesNone()
{
	m_eCoordinate = NONE;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewCoordinatesNone(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_eCoordinate == NONE);
}

void CRenLibView::PostNcDestroy()
{
	Registry reg;

	reg.setInt(Registry::BoardSize, m_BoardSize);

	reg.setInt (Registry::Coordinates,          m_eCoordinate);
	reg.setBool(Registry::SwapRowNumber,        m_bSwapRowNumber);
	reg.setBool(Registry::CoordinatesFollow,    m_bCoordinatesFollow);
	reg.setBool(Registry::ShowBoardBitmap,      m_bShowBoardBitmap);
	reg.setBool(Registry::ShowStoneBitmap,      m_bShowStoneBitmap);
	reg.setBool(Registry::ShowNumbers,          m_bShowNumbers);
	reg.setBool(Registry::ShowLastMove,         m_bShowLastMove);
	reg.setBool(Registry::ShowBoardText,        m_bShowBoardText);
	reg.setBool(Registry::BoardTextExtraSpace,  m_bBoardTextExtraSpace);
	reg.setBool(Registry::ShowComments,         m_bShowComments);
	reg.setBool(Registry::ShowVariants,         m_bShowVariants);
	reg.setBool(Registry::ShowForbidden,        m_bShowForbidden);
	reg.setBool(Registry::AutoPointMouse,       m_bAutoPointMouse);
	reg.setBool(Registry::AutoFindSamePosition, m_bAutoFindSamePosition);
	reg.setBool(Registry::ShowAutoLinkVariants, m_bShowAutoLinkVariants);

	reg.setBool(Registry::ShowChangedOrderOfMovesVariants, m_bShowChangedOrderOfMovesVariants);

	reg.setInt(Registry::BackgroundColor,      m_BackgroundColor);
	reg.setInt(Registry::BoardColor,           m_BoardColor);
	reg.setInt(Registry::BlackStoneColor,      m_BlackStoneColor);
	reg.setInt(Registry::WhiteStoneColor,      m_WhiteStoneColor);
	reg.setInt(Registry::BlackVariantColor,    m_BlackVariantColor);
	reg.setInt(Registry::WhiteVariantColor,    m_WhiteVariantColor);
	reg.setInt(Registry::LastMoveColor,        m_LastMoveColor);
	reg.setInt(Registry::ForbiddenColor,       m_ForbiddenColor);
	reg.setInt(Registry::BlackVcfColor,        m_BlackVcfColor);
	reg.setInt(Registry::WhiteVcfColor,        m_WhiteVcfColor);
	reg.setInt(Registry::BlackVctColor,        m_BlackVctColor);
	reg.setInt(Registry::WhiteVctColor,        m_WhiteVctColor);
	reg.setInt(Registry::BlackALVColor,        m_BlackALVColor);
	reg.setInt(Registry::WhiteALVColor,        m_WhiteALVColor);
	reg.setInt(Registry::BlackCMOColor,        m_BlackCMOColor);
	reg.setInt(Registry::WhiteCMOColor,        m_WhiteCMOColor);
	reg.setInt(Registry::CommentTextColor,     m_CommentTextColor);
	reg.setInt(Registry::CommentBkColor,       m_CommentBkColor);
	reg.setInt(Registry::CoordTextColor,       m_CoordTextColor);
	reg.setInt(Registry::BoardTextColor,       m_BoardTextColor);
	reg.setInt(Registry::BlackNumberColor,     m_BlackNumberColor);
	reg.setInt(Registry::WhiteNumberColor,     m_WhiteNumberColor);
	reg.setInt(Registry::WinMarker,            m_eWinMarker);
	reg.setInt(Registry::CustColors_0,         m_CustColors[ 0]);
	reg.setInt(Registry::CustColors_1,         m_CustColors[ 1]);
	reg.setInt(Registry::CustColors_2,         m_CustColors[ 2]);
	reg.setInt(Registry::CustColors_3,         m_CustColors[ 3]);
	reg.setInt(Registry::CustColors_4,         m_CustColors[ 4]);
	reg.setInt(Registry::CustColors_5,         m_CustColors[ 5]);
	reg.setInt(Registry::CustColors_6,         m_CustColors[ 6]);
	reg.setInt(Registry::CustColors_7,         m_CustColors[ 7]);
	reg.setInt(Registry::CustColors_8,         m_CustColors[ 8]);
	reg.setInt(Registry::CustColors_9,         m_CustColors[ 9]);
	reg.setInt(Registry::CustColors_10,        m_CustColors[10]);
	reg.setInt(Registry::CustColors_11,        m_CustColors[11]);
	reg.setInt(Registry::CustColors_12,        m_CustColors[12]);
	reg.setInt(Registry::CustColors_13,        m_CustColors[13]);
	reg.setInt(Registry::CustColors_14,        m_CustColors[14]);
	reg.setInt(Registry::CustColors_15,        m_CustColors[15]);

	reg.setString(Registry::FontSmall,           m_Font[cSizeSmall].toString());
	reg.setString(Registry::FontMedium,          m_Font[cSizeMedium].toString());
	reg.setString(Registry::FontLarge,           m_Font[cSizeLarge].toString());
	
	reg.setString(Registry::FontCommentSmall,    m_FontComment[cSizeSmall].toString());
	reg.setString(Registry::FontCommentMedium,   m_FontComment[cSizeMedium].toString());
	reg.setString(Registry::FontCommentLarge,    m_FontComment[cSizeLarge].toString());

	reg.setString(Registry::FontBoardTextSmall,  m_FontBoardText[cSizeSmall].toString());
	reg.setString(Registry::FontBoardTextMedium, m_FontBoardText[cSizeMedium].toString());
	reg.setString(Registry::FontBoardTextLarge,  m_FontBoardText[cSizeLarge].toString());

	reg.setString(Registry::StartPage, mStartPage);
	reg.setString(Registry::BoardBitmap, m_strBoardBitmap);

	CFormView::PostNcDestroy();
}

void CRenLibView::OnPosition0degrees() 
{
	m_ePosition = DEGREE_0;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePosition0degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_ePosition == DEGREE_0);
}

void CRenLibView::OnPosition90degrees() 
{
	m_ePosition = DEGREE_90;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePosition90degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_ePosition == DEGREE_90);
}

void CRenLibView::OnPosition180degrees() 
{
	m_ePosition = DEGREE_180;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePosition180degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_ePosition == DEGREE_180);
}

void CRenLibView::OnPosition270degrees() 
{
	m_ePosition = DEGREE_270;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePosition270degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_ePosition == DEGREE_270);
}

void CRenLibView::OnPositionTurnPlus90degrees() 
{
	switch (m_ePosition)
	{
		case DEGREE_0:
			m_ePosition = DEGREE_90;
			break;

		case DEGREE_90:
			m_ePosition = DEGREE_180;
			break;

		case DEGREE_180:
			m_ePosition = DEGREE_270;
			break;

		case DEGREE_270:
			m_ePosition = DEGREE_0;
			break;
	}
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePositionTurnPlus90degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnPositionTurnMinus90degrees()
{
	switch (m_ePosition)
	{
		case DEGREE_0:
			m_ePosition = DEGREE_270;
			break;

		case DEGREE_90:
			m_ePosition = DEGREE_0;
			break;

		case DEGREE_180:
			m_ePosition = DEGREE_90;
			break;

		case DEGREE_270:
			m_ePosition = DEGREE_180;
			break;
	}
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePositionTurnMinus90degrees(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnPositionReflectLeftRight() 
{
	m_bReflect_X = !m_bReflect_X;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePositionReflectLeftRight(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bReflect_X);
}

void CRenLibView::OnPositionReflectUpDown() 
{
	m_bReflect_Y = !m_bReflect_Y;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdatePositionReflectUpDown(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bReflect_Y);
}

void CRenLibView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
}

void CRenLibView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CFormView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(FALSE);
}

void CRenLibView::ChooseFont(Font& myFont) 
{
    CClientDC dc(this);
    long lPixelsy = dc.GetDeviceCaps(LOGPIXELSY);

	LOGFONT lf;
	myFont.getLogFont(lf);

    lf.lfHeight = -MulDiv(myFont.getPointSize(), lPixelsy, 72);

	CFontDialog dlg(&lf, CF_SCREENFONTS, NULL, this);

	if (dlg.DoModal() == IDOK)
	{
		myFont.setPointSize(dlg.GetSize() / 10);
		myFont.setBold(!!dlg.IsBold());
		myFont.setItalic(!!dlg.IsItalic());
		myFont.setFaceName(dlg.GetFaceName());
	}
}

void CRenLibView::OnUpdateViewFontChooseFont(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontChooseFont() 
{
	ChooseFont(m_Font[m_BoardSize]);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontDefaultFont(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontDefaultFont() 
{
	m_Font[m_BoardSize].setDefault(m_BoardSize);
	adjustSize();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontChooseFontBoardText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontChooseFontBoardText() 
{
	ChooseFont(m_FontBoardText[m_BoardSize]);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontDefaultFontBoardText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontDefaultFontBoardText() 
{
	m_FontBoardText[m_BoardSize].setDefault(m_BoardSize);
	adjustSize();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontChooseFontComment(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontChooseFontComment() 
{
	ChooseFont(m_FontComment[m_BoardSize]);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontDefaultFontComment(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontDefaultFontComment() 
{
	m_FontComment[m_BoardSize].setDefault(m_BoardSize);
	adjustSize();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewFontAllDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewFontAllDefault() 
{
	OnViewFontDefaultFont();
	OnViewFontDefaultFontBoardText();
	OnViewFontDefaultFontComment();
}

void CRenLibView::SelectColor(COLORREF& color, COLORREF defaultColor)
{
	Utils::SelectColor(color, defaultColor, &m_CustColors[0]);
}

void CRenLibView::OnUpdateViewColorBoardChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardChooseColor() 
{
	SelectColor(m_BoardColor, defaultBoardColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorBoardDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardDefault() 
{
	m_BoardColor = defaultBoardColor;
	Invalidate(FALSE);
}

void CRenLibView::OnEditFindNext() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->IsFindInProgress())
	{
		if (pDoc->FindNextCommand())
		{
			Invalidate(FALSE);
			UpdateWindow();

			if (pDoc->isMergeComment())
			{
				PostMessage(WM_COMMAND, ID_MERGE_COMMENT_NEXT);
			}
		}
		else
		{
			Utils::Beep();
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindNext(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsFindInProgress());
}

void CRenLibView::OnToolbarFindNext() 
{
	m_bToolBarMoveCommand = true;
	OnEditFindNext();
}

void CRenLibView::OnUpdateToolbarFindNext(CCmdUI* pCmdUI) 
{
	OnUpdateEditFindNext(pCmdUI);
}

void CRenLibView::OnEditFindPrevious() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->IsFindInProgress())
	{
		if (pDoc->FindPreviousCommand())
		{
			Invalidate(FALSE);
			UpdateWindow();

			if (pDoc->isMergeComment())
			{
				PostMessage(WM_COMMAND, ID_MERGE_COMMENT_PREVIOUS);
			}
		}
		else
		{
			Utils::Beep();
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindPrevious(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsFindInProgress());
}

void CRenLibView::OnToolbarFindPrevious() 
{
	m_bToolBarMoveCommand = true;
	OnEditFindPrevious();
}

void CRenLibView::OnUpdateToolbarFindPrevious(CCmdUI* pCmdUI) 
{
	OnUpdateEditFindPrevious(pCmdUI);
}

void CRenLibView::OnViewShowBoardBitmap() 
{
	m_bShowBoardBitmap = !m_bShowBoardBitmap;
	m_bBoardTextExtraSpace = false;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowBoardBitmap(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(m_bShowBoardBitmap);
}

void CRenLibView::OnViewShowStoneBitmap() 
{
	m_bShowStoneBitmap = !m_bShowStoneBitmap;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowStoneBitmap(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowStoneBitmap);
}

void CRenLibView::OnViewShowNumbers() 
{
	m_bShowNumbers = !m_bShowNumbers;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowNumbers(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowNumbers);
}

void CRenLibView::OnViewShowBoardText() 
{
	m_bShowBoardText = !m_bShowBoardText;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowBoardText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowBoardText);
}

void CRenLibView::OnViewBoardTextExtraSpace() 
{
	m_bBoardTextExtraSpace = !m_bBoardTextExtraSpace;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewBoardTextExtraSpace(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0 && m_bShowBoardText && !m_bShowBoardBitmap);
	pCmdUI->SetCheck(m_bBoardTextExtraSpace);
}

void CRenLibView::OnViewShowComments() 
{
	m_bShowComments = !m_bShowComments;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowComments(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowComments);
}

void CRenLibView::OnViewShowLastMove() 
{
	m_bShowLastMove = !m_bShowLastMove;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowLastMove(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowLastMove);
}

void CRenLibView::OnViewShowVariants() 
{
	m_bShowVariants = !m_bShowVariants;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowVariants(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowVariants);
}

void CRenLibView::OnViewAutoPointMouse() 
{
	m_bAutoPointMouse = !m_bAutoPointMouse;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewAutoPointMouse(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bAutoPointMouse);
}


BOOL CRenLibView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	Invalidate(FALSE);
	
	return CFormView::OnMouseWheel(nFlags, zDelta, pt);
}

void CRenLibView::OnEditClearAllMarks() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->ClearAllMarksCommand();
}

void CRenLibView::OnUpdateEditClearAllMarks(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnEditStartPosition() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	pDoc->StartPositionCommand();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditStartPosition(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() >= 0);

	if (pDoc->IsStartPosition())
	{
		pCmdUI->SetText(Utils::GetString(IDS_MENU_CLEAR_START_POSITION));
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetText(Utils::GetString(IDS_MENU_SET_START_POSITION));
		pCmdUI->SetCheck(FALSE);
	}
}

void CRenLibView::OnEditFindStartPosition() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->FindStartPositionCommand(true))
	{
		if (m_bAutoFindSamePosition)
		{
			pDoc->autoFindSamePositionCommand();
		}
	}
	else
	{
		Utils::Beep();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindStartPosition(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnMoveLastMove() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	while (pDoc->IsForwardCommand())
	{
		pDoc->ForwardCommand();
	}

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateMoveLastMove(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->IsForwardCommand());
}

CString	CRenLibView::GetMouseCoord()
{
	if (m_eCoordinate != NONE && m_MouseCoord != CPoint(0,0))
	{
		return " " + ColImage(m_MouseCoord.x) + RowImage(m_MouseCoord.y);
	}

	return "";
}

CString	CRenLibView::GetNextAutoBoardText()
{
	CRenLibDoc* pDoc = GetDocument();

	if (m_bAutoEnterBoardText && pDoc->isUpdateMode())
	{
		return " " + m_NextAutoBoardText;
	}

	return "";
}

void CRenLibView::OnViewCoordinatesSwapNumber() 
{
	m_bSwapRowNumber = !m_bSwapRowNumber;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewCoordinatesSwapNumber(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   m_eCoordinate != NONE);
	pCmdUI->SetCheck(m_bSwapRowNumber);
}


void CRenLibView::OnViewCoordinatesFollowRotationAndReflection() 
{
	m_bCoordinatesFollow = !m_bCoordinatesFollow;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewCoordinatesFollowRotationAndReflection(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   m_eCoordinate != NONE);
	pCmdUI->SetCheck(m_bCoordinatesFollow);
}

void CRenLibView::OnViewShowForbiddenPoints() 
{
	m_bShowForbidden = !m_bShowForbidden;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowForbiddenPoints(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0 &&
		           pDoc->GetBoard()->canShowForbiddenPoints());
	pCmdUI->SetCheck(m_bShowForbidden);
}


void CRenLibView::OnEditFindSamePosition() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->FindSamePositionCommand())
	{
		pDoc->FindNextCommand();
	}
	else
	{
		Utils::Beep();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindSamePosition(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 1);
}

void CRenLibView::OnEditFindSimilarPosition() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	if (pDoc->FindSimilarPositionCommand())
	{
		pDoc->FindNextCommand();
	}
	else
	{
		Utils::Beep();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditFindSimilarPosition(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 1);
}

void CRenLibView::OnViewAutoFindSamePosition() 
{
	CRenLibDoc* pDoc = GetDocument();

	m_bAutoFindSamePosition = !m_bAutoFindSamePosition;

	if (m_bAutoFindSamePosition)
	{
		pDoc->autoFindSamePositionCommand();
	}
	else
	{
		pDoc->ClearAutoFindSamePositionList();
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewAutoFindSamePosition(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bAutoFindSamePosition);
}

void CRenLibView::OnViewShowAutoLinkVariants() 
{
	m_bShowAutoLinkVariants = !m_bShowAutoLinkVariants;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowAutoLinkVariants(CCmdUI* pCmdUI) 
{	
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowAutoLinkVariants);
}

void CRenLibView::OnUpdateViewColorVcfColorChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVcfColorChooseBlack() 
{
	SelectColor(m_BlackVcfColor, defaultBlackVcfColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVcfColorChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVcfColorChooseWhite() 
{
	SelectColor(m_WhiteVcfColor, defaultWhiteVcfColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorAutoLinkVariantColorChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorAutoLinkVariantColorChooseBlack() 
{
	SelectColor(m_BlackALVColor, defaultBlackALVColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorAutoLinkVariantColorChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorAutoLinkVariantColorChooseWhite() 
{
	SelectColor(m_WhiteALVColor, defaultWhiteALVColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVctColorChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVctColorChooseBlack() 
{
	SelectColor(m_BlackVctColor, defaultBlackVctColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVctColorChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVctColorChooseWhite() 
{
	SelectColor(m_WhiteVctColor, defaultWhiteVctColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVcfColorDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVcfColorDefault() 
{
	m_BlackVcfColor  = defaultBlackVcfColor;
	m_WhiteVcfColor  = defaultWhiteVcfColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVctColorDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVctColorDefault() 
{
	m_BlackVctColor  = defaultBlackVctColor;
	m_WhiteVctColor  = defaultWhiteVctColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorAutoLinkVariantColorDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorAutoLinkVariantColorDefault() 
{
	m_BlackALVColor  = defaultBlackALVColor;
	m_WhiteALVColor  = defaultWhiteALVColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorChangedOrderOfMovesVariantColorChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorChangedOrderOfMovesVariantColorChooseBlack() 
{
	SelectColor(m_BlackCMOColor, defaultBlackCMOColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorChangedOrderOfMovesVariantColorChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorChangedOrderOfMovesVariantColorChooseWhite() 
{
	SelectColor(m_WhiteCMOColor, defaultWhiteCMOColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorChangedOrderOfMovesVariantColorDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorChangedOrderOfMovesVariantColorDefault() 
{
	m_BlackCMOColor  = defaultBlackCMOColor;
	m_WhiteCMOColor  = defaultWhiteCMOColor;
	Invalidate(FALSE);
}

void CRenLibView::OnViewShowChangedOrderOfMovesVariants() 
{
	m_bShowChangedOrderOfMovesVariants = !m_bShowChangedOrderOfMovesVariants;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewShowChangedOrderOfMovesVariants(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bShowChangedOrderOfMovesVariants);
}

void CRenLibView::OnUpdateViewColorLastMoveChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorLastMoveChooseColor() 
{
	SelectColor(m_LastMoveColor, defaultLastMoveColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorLastMoveDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorLastMoveDefault() 
{
	m_LastMoveColor = defaultLastMoveColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneChooseBlack() 
{
	SelectColor(m_BlackStoneColor, defaultBlackStoneColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneChooseWhite() 
{
	SelectColor(m_WhiteStoneColor, defaultWhiteStoneColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneDefault() 
{
	m_BlackStoneColor = defaultBlackStoneColor;
	m_WhiteStoneColor = defaultWhiteStoneColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVariantChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVariantChooseBlack() 
{
	SelectColor(m_BlackVariantColor, defaultBlackVariantColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVariantChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVariantChooseWhite() 
{
	SelectColor(m_WhiteVariantColor, defaultWhiteVariantColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorVariantDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorVariantDefault() 
{
	m_BlackVariantColor = defaultBlackVariantColor;
	m_WhiteVariantColor = defaultWhiteVariantColor;
	Invalidate(FALSE);
}

void CRenLibView::OnMovePass() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->DisplayPassMove();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateMovePass(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() && pDoc->hasPassVariant());
}

void CRenLibView::OnUpdateViewColorCommentText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorCommentText() 
{
	SelectColor(m_CommentTextColor, defaultCommentTextColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorCommentBackground(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorCommentBackground() 
{
	SelectColor(m_CommentBkColor, defaultCommentBkColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorCommentDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorCommentDefault() 
{
	m_CommentTextColor = defaultCommentTextColor;
	m_CommentBkColor   = defaultCommentBkColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorCoordinateChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorCoordinateChooseColor() 
{
	SelectColor(m_CoordTextColor, defaultCoordTextColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorCoordinateDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorCoordinateDefault() 
{
	m_CoordTextColor = defaultCoordTextColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorBoardTextChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardTextChooseColor() 
{
	SelectColor(m_BoardTextColor, defaultBoardTextColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorBoardTextDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardTextDefault() 
{
	m_BoardTextColor = defaultBoardTextColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneNumberChooseBlack(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneNumberChooseBlack() 
{
	SelectColor(m_BlackNumberColor, defaultBlackNumberColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneNumberChooseWhite(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneNumberChooseWhite() 
{
	SelectColor(m_WhiteNumberColor, defaultWhiteNumberColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorStoneNumberDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorStoneNumberDefault() 
{
	m_BlackNumberColor = defaultBlackNumberColor;
	m_WhiteNumberColor = defaultWhiteNumberColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorBackgroundChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBackgroundChooseColor() 
{
	SelectColor(m_BackgroundColor, defaultBackgroundColor);
	CView::Invalidate();
}

void CRenLibView::OnUpdateViewColorBackgroundDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBackgroundDefault() 
{
	m_BackgroundColor = defaultBackgroundColor;
	CView::Invalidate();
}

void CRenLibView::OnUpdateViewColorForbiddenChooseColor(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorForbiddenChooseColor() 
{
	SelectColor(m_ForbiddenColor, defaultForbiddenColor);
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorForbiddenDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorForbiddenDefault() 
{
	m_ForbiddenColor = defaultForbiddenColor;
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewColorAllDefault(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorAllDefault() 
{
	OnViewColorBackgroundDefault();
	OnViewColorBoardDefault();
	OnViewColorStoneDefault();
	OnViewColorVariantDefault();
	OnViewColorLastMoveDefault();
	OnViewColorForbiddenDefault();
	OnViewColorAutoLinkVariantColorDefault();
	OnViewColorChangedOrderOfMovesVariantColorDefault();
	OnViewColorCommentDefault();
	OnViewColorCoordinateDefault();
	OnViewColorBoardTextDefault();
	OnViewColorStoneNumberDefault();
}

void CRenLibView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnPrepareDC(&dc);
	CRect rectUpdate;
	dc.GetClipBox(&rectUpdate);
	CBitmap* pOldBitmap = m_pdcMemory->SelectObject(m_pBitmap);
	m_pdcMemory->SelectClipRgn(NULL);
	m_pdcMemory->IntersectClipRect(&rectUpdate);
	CBrush backgroundBrush((COLORREF) ::GetSysColor(COLOR_WINDOW));
	CBrush* pOldBrush = m_pdcMemory->SelectObject(&backgroundBrush);
	m_pdcMemory->PatBlt(rectUpdate.left, rectUpdate.top,
	                    rectUpdate.Width(), rectUpdate.Height(),
	                    PATCOPY);
	OnDraw(m_pdcMemory);

	rectUpdate.top = 0;

	dc.BitBlt(rectUpdate.left, rectUpdate.top,
	          rectUpdate.Width(), rectUpdate.Height(),
	          m_pdcMemory, rectUpdate.left, rectUpdate.top,
	          SRCCOPY);
	m_pdcMemory->SelectObject(pOldBitmap);
	m_pdcMemory->SelectObject(pOldBrush);
}

void CRenLibView::OnToolbarBack()
{
	m_bToolBarMoveCommand = true;
	OnMoveBack();
}

void CRenLibView::OnToolbarDown()
{
	m_bToolBarMoveCommand = true;
	OnMoveDown();
}

void CRenLibView::OnToolbarUp()
{
	m_bToolBarMoveCommand = true;
	OnMoveUp();
}

void CRenLibView::OnToolbarForward()
{
	m_bToolBarMoveCommand = true;
	OnMoveForward();
}

void CRenLibView::OnToolbar1stMove()
{
	m_bToolBarMoveCommand = true;
	OnMove1stMove();
}

void CRenLibView::OnToolbarLastMove()
{
	m_bToolBarMoveCommand = true;
	OnMoveLastMove();
}

void CRenLibView::OnToolbarPass()
{
	m_bToolBarMoveCommand = true;
	OnMovePass();
}

void CRenLibView::OnUpdateToolbarBack(CCmdUI* pCmdUI)
{
	OnUpdateMoveBack(pCmdUI);
}

void CRenLibView::OnUpdateToolbarDown(CCmdUI* pCmdUI)
{
	OnUpdateMoveDown(pCmdUI);
}

void CRenLibView::OnUpdateToolbarUp(CCmdUI* pCmdUI)
{
	OnUpdateMoveUp(pCmdUI);
}

void CRenLibView::OnUpdateToolbarForward(CCmdUI* pCmdUI)
{
	OnUpdateMoveForward(pCmdUI);
}

void CRenLibView::OnUpdateToolbar1stMove(CCmdUI* pCmdUI)
{
	OnUpdateMove1stMove(pCmdUI);
}

void CRenLibView::OnUpdateToolbarLastMove(CCmdUI* pCmdUI)
{
	OnUpdateMoveLastMove(pCmdUI);
}

void CRenLibView::OnUpdateToolbarPass(CCmdUI* pCmdUI)
{
	OnUpdateMovePass(pCmdUI);
}

void CRenLibView::OnSize(UINT nType, int cx, int cy) 
{
	if (nType == SIZE_RESTORED)
	{
		CreateMemoryDeviceContext();
	}

	CFormView::OnSize(nType, cx, cy);	

	adjustControls();
}

void CRenLibView::UpdateWinTrace()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->GetBoard()->updateWinTrace(m_eWinMarker);
}

void CRenLibView::OnEditFindVcf()
{
	CWaitCursor cursor;
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

    mStartTime  = CTime::GetCurrentTime();
    mTimerState = TIMER_VCT_VCF;
    m_nTimer    = SetTimer(cTimerId, cTimeVcfVct, 0);

    pDoc->findWin(CRenLibDoc::VCF);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFindVcf(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();

	pCmdUI->Enable(pDoc->GetCurrentMove() > 0 &&
		           !pDoc->hasPassMove() &&
		           pDoc->GetBoard()->canFindVcf());

    if (pDoc->GetBoard()->isFindVcfActive() ||
        pDoc->IsWinPosition(CRenLibDoc::VCF))
	{
		pCmdUI->SetText(Utils::GetString(IDS_MENU_HIDE_VCF));
        pCmdUI->SetCheck(TRUE);
	}
	else
	{
        pCmdUI->SetText(Utils::GetString(IDS_MENU_FIND_VCF));
        pCmdUI->SetCheck(FALSE);
	}
}

void CRenLibView::OnEditFindVct()
{
	CWaitCursor cursor;
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

    mStartTime  = CTime::GetCurrentTime();
    mTimerState = TIMER_VCT_VCF;
    m_nTimer    = SetTimer(cTimerId, cTimeVcfVct, 0);

    pDoc->findWin(CRenLibDoc::VCT);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFindVct(CCmdUI* pCmdUI)
{
	CRenLibDoc* pDoc = GetDocument();

	pCmdUI->Enable(pDoc->GetCurrentMove() > 0 &&
		           !pDoc->hasPassMove() &&
		           pDoc->GetBoard()->canFindVct());

	if (pDoc->GetBoard()->isFindVctActive() ||
        pDoc->IsWinPosition(CRenLibDoc::VCT))
	{
		pCmdUI->SetText(Utils::GetString(IDS_MENU_HIDE_VCT));
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetText(Utils::GetString(IDS_MENU_FIND_VCT));
		pCmdUI->SetCheck(FALSE);
	}
}

void CRenLibView::OnEditFindStop()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->GetBoard()->stopFind();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFindStop(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(pDoc->isFindActive());
}

void CRenLibView::OnEditAutoEnterBoardText() 
{
	m_bAutoEnterBoardText = !m_bAutoEnterBoardText;

	if (m_bAutoEnterBoardText)
	{
		CRenLibDoc* pDoc = GetDocument();
		UpdateNextAutoBoardText(pDoc);
	}
}

void CRenLibView::OnUpdateEditAutoEnterBoardText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_bAutoEnterBoardText);
}

void CRenLibView::OnEditSetNextAutoBoardText() 
{
	CRenLibDoc* pDoc = GetDocument();
	AutoBoardText autoBoardText;
	autoBoardText.m_NextAutoBoardText = m_NextAutoBoardText;
	autoBoardText.m_strFaceName       = m_FontBoardText[m_BoardSize].getFaceName();

	int length = autoBoardText.m_NextAutoBoardText.GetLength();

	if (autoBoardText.DoModal() == IDOK && (length == 1 || length == 2 ))
	{
		m_NextAutoBoardText  = autoBoardText.m_NextAutoBoardText;
		m_bAutoEnterBoardText = true;
	}
	else
	{
		m_bAutoEnterBoardText = false;
	}
}

void CRenLibView::OnUpdateEditSetNextAutoBoardText(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() && pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnViewWin123() 
{
	m_eWinMarker = Board::NUMBER;
	UpdateWinTrace();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewWin123(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_eWinMarker == Board::NUMBER);
}

void CRenLibView::OnViewWinAbcUpper() 
{
	m_eWinMarker = Board::UPPER;
	UpdateWinTrace();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewWinAbcUpper(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_eWinMarker == Board::UPPER);
}

void CRenLibView::OnViewWinAbcLower() 
{
	m_eWinMarker = Board::LOWER;
	UpdateWinTrace();
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateViewWinAbcLower(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
	pCmdUI->SetCheck(m_eWinMarker == Board::LOWER);
}

void CRenLibView::tabSelChange(NMHDR* pNotifyStruct, LRESULT* result)
{
	showTabControls();

	*result = 0;
}

void CRenLibView::showTabControls()
{
	const int curSel = m_Tab.GetCurSel();

	m_MultiLineComment.ShowWindow(curSel == TAB_COMMENT    ? SW_SHOWNORMAL : SW_HIDE);
	m_Statistics.ShowWindow      (curSel == TAB_STATISTICS ? SW_SHOWNORMAL : SW_HIDE);
	m_Address.ShowWindow         (curSel == TAB_BROWSER    ? SW_SHOWNORMAL : SW_HIDE);
	m_Go.ShowWindow              (curSel == TAB_BROWSER    ? SW_SHOWNORMAL : SW_HIDE);
}

void CRenLibView::OnMergeCommentNext()
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->mergeComment())
	{
		PostMessage(WM_COMMAND, ID_EDIT_FIND_NEXT);
	}
}

void CRenLibView::OnMergeCommentPrevious()
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->mergeComment())
	{
		PostMessage(WM_COMMAND, ID_EDIT_FIND_PREVIOUS);
	}
}

void CRenLibView::OnEditConvertToStandard() 
{
	CRenLibDoc* pDoc = GetDocument();

	pDoc->initMergeComment(true);

	pDoc->ConvertToStandardCommand();

	if (pDoc->hasMergeComment())
	{
		CString strMessage(Utils::GetString(IDS_MSG_MERGE_COMMENTS));
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_CONVERT_TO_STANDARD), MB_ICONINFORMATION);
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateEditConvertToStandard(CCmdUI* pCmdUI) 
{	
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() &&
		           pDoc->GetCurrentMove() >= 0 &&
				   pDoc->checkConvertToStandard());
}

void CRenLibView::OnEditInsertPassMove() 
{
	CRenLibDoc* pDoc = GetDocument();
    pDoc->InsertPassMove();
}

void CRenLibView::OnUpdateEditInsertPassMove(CCmdUI* pCmdUI) 
{	
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode() &&
		           pDoc->GetCurrentMove() >= 0);
}

void CRenLibView::OnFileExportLibraryTextBoard() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	
	CString strCaption(Utils::GetString(IDS_SAVE_BOARD_TEXT_DIRECTORY));

	CString strDirectory(Utils::SelectDirectory(strCaption));
	
	bool doSave = false;
	
	if (!strDirectory.IsEmpty())
	{
		CFileInfoArray fia;
		
		fia.AddDir
			(strDirectory,              // Directory
			 "RenLib*.txt",             // Filemask
			 FALSE,                     // Recurse subdirs
			 CFileInfoArray::AP_NOSORT, // Sort
			 FALSE                      // Do not add array entries for directories (only for files)
			);
		
		if (fia.GetSize() == 0)
		{
			doSave = true;
		}
		else
		{
			int choice =
				Utils::ShowMessage(Utils::GetString(IDS_MSG_EXPORT_TEXT_BOARD),
                                   Utils::GetString(IDS_CAP_EXPORT_TEXT_BOARD),
                                   MB_YESNO + MB_ICONQUESTION);

			doSave = (choice == IDYES);
		}
	}

	if (doSave)
	{
		pDoc->ExportTextBoardCommand(strDirectory);
	}
	
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileExportLibraryTextBoard(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnFileExportLibraryRenLibApplet() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();
	
	CString strCaption(Utils::GetString(IDS_SAVE_APPLET_HTML_DIRECTORY));

	CString strDirectory(Utils::SelectDirectory(strCaption));
	
	bool doSave = false;
	
	if (!strDirectory.IsEmpty())
	{
		CFileInfoArray fia;
		
		fia.AddDir
			(strDirectory,              // Directory
			 "RenLib*.htm",             // Filemask
			 FALSE,                     // Recurse subdirs
			 CFileInfoArray::AP_NOSORT, // Sort
			 FALSE                      // Do not add array entries for directories (only for files)
			);
		
		if (fia.GetSize() == 0)
		{
			doSave = true;
		}
		else
		{
			int choice =
				Utils::ShowMessage(Utils::GetString(IDS_MSG_EXPORT_RENLIB_APPLET),
                                   Utils::GetString(IDS_CAP_EXPORT_RENLIB_APPLET),
                                   MB_YESNO + MB_ICONQUESTION);

			doSave = (choice == IDYES);
		}
	}

	if (doSave)
	{
		CString strFile;

		if (pDoc->ExportRenLibAppletCommand(strDirectory, strFile, &m_CustColors[0]))
		{
			OpenUrlInBrowser(strFile);
		}
	}
	
	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileExportLibraryRenLibApplet(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::OnFileExportLibraryGameCollection() 
{
	CRenLibDoc* pDoc = GetDocument();
	CString strFile;
	Utils::FileType fileType = Utils::GAME_COLLECTION;

	if (Utils::SaveGameCollectionDialog(strFile, fileType) &&
		fileType == Utils::GAME_COLLECTION)
	{
		if (pDoc->ExportGameCollectionCommand(strFile, &m_CustColors[0]))
		{
			OpenUrlInBrowser(strFile);
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileExportLibraryGameCollection(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
    pCmdUI->Enable(!pDoc->isFindActive() && pDoc->GetCurrentMove() > 0);
}

void CRenLibView::indicateConvertOrMerge(const CString& strTitle)
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->GetCurrentMove() >= 0)
	{
		if (pDoc->checkConvertToStandard() &&
			Utils::ShowMessage(Utils::GetString(IDS_MSG_CONVERT_TO_STANDARD),
			                   strTitle,
							   MB_YESNO + MB_ICONQUESTION) == IDYES)
		{
			pDoc->SetMode(CRenLibDoc::UPDATE_MODE);
			PostMessage(WM_COMMAND, ID_EDIT_CONVERT_TO_STANDARD);
		}		
		else if (pDoc->hasMergeComment())
		{
			CString strMessage(Utils::GetString(IDS_MSG_MERGE_COMMENTS));
			Utils::ShowMessage(strMessage, strTitle, MB_ICONINFORMATION);
		}
	}
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateLib(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateLib()
{
	m_Tab.SetFocus();
	Utils::associateLib();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_LIB),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateWzq(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateWzq() 
{
	m_Tab.SetFocus();
	Utils::associateWzq();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_WZQ),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateBuf(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateBuf() 
{
	m_Tab.SetFocus();
	Utils::associateBuf();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_BUF),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociatePdb(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociatePdb() 
{
	m_Tab.SetFocus();
	Utils::associatePdb();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_PDB),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociatePos(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociatePos() 
{
	m_Tab.SetFocus();
	Utils::associatePos();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_POS),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateRenjs(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateRenjs() 
{
	m_Tab.SetFocus();
	Utils::associateRenjs();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_RENJS),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateSgf(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateSgf() 
{
	m_Tab.SetFocus();
	Utils::associateSgf();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_SGF),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateBdt(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateBdt() 
{
	m_Tab.SetFocus();
	Utils::associateBdt();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_BDT),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateRdf(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateRdf() 
{
	m_Tab.SetFocus();
	Utils::associateRdf();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_RDF),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnUpdateOptionsEnvironmentAssociateAll(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnOptionsEnvironmentAssociateAll() 
{
	m_Tab.SetFocus();
	Utils::associateLib();
	Utils::associateWzq();
	Utils::associateBuf();
	Utils::associatePdb();
	Utils::associatePos();
	Utils::associateRenjs();
	Utils::associateSgf();
	Utils::associateBdt();
	Utils::associateRdf();

	Utils::ShowMessage(Utils::GetString(IDS_MSG_RENLIB_PATH,
                       Utils::getRenLibPath()),
                       Utils::GetString(IDS_CAP_ASSOCIATE_ALL),
                       MB_ICONINFORMATION);
}

void CRenLibView::OnOneLineCommentUpdate()
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->isUpdateMode())
	{
		CString strOneLine;
		m_OneLineComment.GetWindowText(strOneLine);
		pDoc->SetOneLineComment(strOneLine);
	}
}

void CRenLibView::OnOneLineCommentSetFocus()
{
	m_bOneLineCommentHasFocus = true;
}

void CRenLibView::OnOneLineCommentKillFocus()
{
	m_bOneLineCommentHasFocus = false;
}

void CRenLibView::OnMultiLineCommentUpdate()
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->isUpdateMode())
	{
		CString strMultiLine;
		m_MultiLineComment.GetWindowText(strMultiLine);
		pDoc->SetMultiLineComment(strMultiLine);
	}
}

void CRenLibView::OnMultiLineCommentSetFocus()
{
	m_bMultiLineCommentHasFocus = true;
}

void CRenLibView::OnMultiLineCommentKillFocus()
{
	m_bMultiLineCommentHasFocus = false;
}

void CRenLibView::OnAddressSetFocus()
{
	m_bAddressHasFocus = true;
}

void CRenLibView::OnAddressKillFocus()
{
	m_bAddressHasFocus = false;
}

void CRenLibView::OnAddressSelChange()
{
	int curSel = m_Address.GetCurSel();

	if (curSel != CB_ERR && curSel < mUrl.size())
	{
		navigate(mUrl[curSel]);
	}
}

bool CRenLibView::isReadOnly()
{
	CRenLibDoc* pDoc = GetDocument();
	return (pDoc->isDisplayMode());
}

void CRenLibView::OnOptionsEvaluatorNone() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->GetBoard()->setEvaluatorSelection(Board::NONE);
	m_bShowForbidden = false;

	pDoc->GetBoard()->ClearWin();
	pDoc->SetIsWinPosition(CRenLibDoc::VCF, false);
	pDoc->SetIsWinPosition(CRenLibDoc::VCT, false);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateOptionsEvaluatorNone(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
	pCmdUI->SetCheck(pDoc->GetBoard()->getEvaluatorSelection() == Board::NONE);
}

void CRenLibView::OnOptionsEvaluatorRenjuClassRenjuSolver()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->GetBoard()->setEvaluatorSelection(Board::RENJU_CLASS_RENJU_SOLVER);
	
	pDoc->GetBoard()->ClearWin();
	pDoc->SetIsWinPosition(CRenLibDoc::VCF, false);
	pDoc->SetIsWinPosition(CRenLibDoc::VCT, false);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateOptionsEvaluatorRenjuClassRenjuSolver(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());

    pCmdUI->SetCheck(
        pDoc->GetBoard()->getEvaluatorSelection() == Board::ORIGINAL ||
        pDoc->GetBoard()->getEvaluatorSelection() == Board::RENJU_CLASS_RENJU_SOLVER);
}

void CRenLibView::OnUpdateFileMakeTournamentRenLibApplet(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnFileMakeTournamentRenLibApplet() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	CString strCaption(Utils::GetString(IDS_SAVE_APPLET_HTML_DIRECTORY));

	CString strDirectory(Utils::SelectDirectory(strCaption));

	if (!strDirectory.IsEmpty())
	{
		CString strFile;
		CFileInfoArray fia;

		fia.AddDir
		  (strDirectory,                     // Directory
		   "*.lib",                          // Filemask
		   FALSE,                            // Do not recurse subdirs
		   CFileInfoArray::AP_SORTASCENDING, // Sort ascending order
		   FALSE                             // Do not add array entries for directories (only for files)
		);

		int nTotalFiles = fia.GetSize();
		CString strCaption(Utils::GetString(IDS_CAP_READING_FILES, nTotalFiles));
		CStringList fileNameList;

		if (nTotalFiles == 0)
		{
			CString strMessage(Utils::GetString(IDS_NO_FILES, strDirectory));
			Utils::ShowMessage(strMessage, strCaption, MB_ICONERROR);
			return;
		}
		else
		{
			RenLibApplet::Information info;

			RenLibApplet renlibApplet(info);

			if (!renlibApplet.getAppletInformation(
					strDirectory, info, &m_CustColors[0]))
			{
				return; // User clicked Cancel in RenLibApplet dialogue
			}

			for (int i=0; i < nTotalFiles; i++)
			{
				fileNameList.AddTail(fia[i].GetFilePath());
				fileNameList.AddTail(fia[i].GetFileName());
			}

			CProgressWnd wndProgress(this, strCaption);
			
			wndProgress.GoModal();
			wndProgress.SetRange(0,nTotalFiles);
			Invalidate(FALSE);
			
			POSITION pos = fileNameList.GetHeadPosition();
			CStringList strList;

			while (pos != NULL)
			{
				strFile = fileNameList.GetNext(pos);
				
				CString strText(Utils::GetString(IDS_MSG_READING_FILE));
				strText += "\n" + strFile;
				wndProgress.SetText(strText);
				
				pDoc->OnOpenDocument(strFile);
				Invalidate(FALSE);

				strFile.Replace(".lib",".htm");

				CString strComment;
				pDoc->MakeRenLibAppletCommand(strFile, strComment, info);

				Utils::trim(strComment);

				if (strComment.IsEmpty())
				{
					strComment = "<b style=color:#ff0000>Missing comment</b>";
				}

				strFile = fileNameList.GetNext(pos);
				strFile.Replace(".lib",".htm");
				strList.AddTail(strFile);
				strList.AddTail(strComment);

				wndProgress.StepIt();
				wndProgress.PeekAndPump();

				if (wndProgress.Cancelled())
				{
					CString strMessage(Utils::GetString(IDS_MSG_CANCELLED));
					Utils::ShowMessage(strMessage, strCaption, MB_ICONWARNING);
					break;
				}
			}

			if (renlibApplet.CreateTournamentGameFile(
				    strDirectory, strList, info.mTitleGameWebPage, strCaption, strFile))
			{
				OpenUrlInBrowser(strFile);
			}
		}

		Invalidate(FALSE);
	}
}

void CRenLibView::OnUpdateViewColorBoardChooseBitmap(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardChooseBitmap() 
{
	if (Utils::OpenBmpDialog(m_strBoardBitmap, Utils::getBitmapDirectory()))
	{
		loadBoardBitmap(m_strBoardBitmap);
		Invalidate(FALSE);
	}
}

void CRenLibView::OnUpdateViewColorBoardDefaultBitmap(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnViewColorBoardDefaultBitmap() 
{
	m_strBoardBitmap = Utils::getBitmapDirectory() + strDefaultBoardBitmap;
	loadBoardBitmap(m_strBoardBitmap);
	Invalidate(FALSE);
}


void CRenLibView::OnFileRead() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	CStringList fileNameList;
	Utils::OpenMultiFileDialog(fileNameList);

	readFile(fileNameList);

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileRead(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode());
}

void CRenLibView::OnFileReadAll()
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	CString strCaption(Utils::GetString(ID_FILE_READ_ALL));

	CString strDirectory(Utils::SelectDirectory(strCaption));

	if (!strDirectory.IsEmpty())
	{
		pDoc->initMergeComment();

		CString strFile;

		CFileInfoArray fia;

		fia.AddDir
		  (strDirectory,              // Directory
		   "*.*",                     // Filemask
		   TRUE,                      // Recurse subdirs
		   CFileInfoArray::AP_NOSORT, // Sort
		   FALSE                      // Do not add array entries for directories (only for files)
		);

		CStringList fileNameList;

		int nTotalFiles = fia.GetSize();

		for (int i=0; i < nTotalFiles; i++)
		{
			CString strFile(fia[i].GetFilePath());

			if (Utils::IsExtensionAccepted(strFile))
			{
				fileNameList.AddTail(strFile);
			}
		}

		if (fileNameList.GetCount() == 0)
		{
			CString strMessage(Utils::GetString(IDS_NO_FILES, strDirectory));
			Utils::ShowMessage(strMessage, strCaption, MB_ICONERROR);
			return;
		}
		else
		{
			readFile(fileNameList);
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileReadAll(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->isUpdateMode());
}

void CRenLibView::OnFileSavePositionAs() 
{
	CRenLibDoc* pDoc = GetDocument();
	pDoc->EmptyPaneText();
	m_Tab.SetFocus();

	CString strFile;
	Utils::FileType fileType;

	if (Utils::SavePositionDialog(strFile, fileType))
	{
		switch (fileType)
		{
			case Utils::RENLIB_LIBRARY:
			{
				pDoc->SaveBranchCommand(strFile);
				break;
			}
			
			case Utils::TEXT_GAME:
			{
				pDoc->TextBoardCommand(strFile);
				break;
			}
			
			case Utils::POS:
			{
				pDoc->SavePosCommand(strFile);
				break;
			}
			
			case Utils::RENLIB_APPLET:
			{
                RenLibApplet::Information info;

				info.mTitleAppletWebPage = pDoc->getOneLineComment();

				if (info.mTitleAppletWebPage.IsEmpty())
				{
					info.mTitleAppletWebPage = "RenLib Applet";
				}

                RenLibAppletDialog renLibAppletDialog(true, false, &m_CustColors[0]);

                renLibAppletDialog.setInfo(info);

                std::vector<CString> oneLineComments;
	            pDoc->getOneLineComments(oneLineComments);

                renLibAppletDialog.setOneLineComments(oneLineComments);

				if (renLibAppletDialog.DoModal() == IDOK)
				{
                    renLibAppletDialog.getInfo(info);

					CString strComment;

					if (pDoc->MakeRenLibAppletCommand(strFile, strComment, info))
					{
						OpenUrlInBrowser(strFile);
					}
				}

				break;
			}
			
			case Utils::RENARTIST_APPLET:
			{
				if (pDoc->MakeRenartistAppletCommand(strFile))
				{
					OpenUrlInBrowser(strFile);
				}

				break;
			}

			case Utils::RENJU_CLASS_DIAGRAM:
			{
				if (pDoc->RenjuclassDiagramCommand(strFile))
				{
					OpenUrlInBrowser(strFile);
				}

				break;
			}

			case Utils::BMP:
			{
				savePositionAsAsBmp(strFile);
				break;
			}
			
			default:
			{
				break;
			}
		}
	}

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateFileSavePositionAs(CCmdUI* pCmdUI) 
{
    CRenLibDoc* pDoc = GetDocument();
    pCmdUI->Enable(!pDoc->isFindActive() &&
                   pDoc->GetCurrentMove() > 0);
}

void CRenLibView::savePositionAsAsBmp(const CString& strFile)
{
	CRect mainRect;

	AfxGetMainWnd()->GetWindowRect(&mainRect);

	AfxGetMainWnd()->
		MoveWindow( 0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	CreateMemoryDeviceContext();

	Invalidate(FALSE);
	UpdateWindow();

	CDIBSectionLite* pDib;

	pDib = new CDIBSectionLite;

	if (pDib->SetBitmap((HBITMAP)*m_pBitmap))
	{
		pDib->Save(strFile);
	}

	delete pDib;

	AfxGetMainWnd()->MoveWindow(&mainRect);

	CreateMemoryDeviceContext();
}

void CRenLibView::OnStatusTextChangeExplorer(LPCTSTR Text) 
{
	if (m_Tab.GetCurSel() == TAB_BROWSER)
	{
		SetPaneText(Text);
	}
}

void CRenLibView::OnBeforeNavigate2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL, VARIANT FAR* Flags, VARIANT FAR* TargetFrameName, VARIANT FAR* PostData, VARIANT FAR* Headers, BOOL FAR* Cancel) 
{
	mCurrentUrl.clear();
}

void CRenLibView::OnNavigateComplete2Explorer(LPDISPATCH pDisp, VARIANT FAR* URL) 
{
	CString strUrl(URL->bstrVal);

	Utils::trim(strUrl);

	if (!strUrl.IsEmpty())
	{
		mCurrentUrl.push_back(strUrl);
	}
}

void CRenLibView::OnDocumentCompleteExplorer(LPDISPATCH pDisp, VARIANT FAR* URL)
{
	CString strUrl(URL->bstrVal);

	Utils::trim(strUrl);

	if (!strUrl.IsEmpty())
	{
		m_Address.SetWindowText(strUrl);
		m_Address.SetEditSel(-1,0);
		m_Tab.SetFocus();
	}
}

void CRenLibView::navigate(const CString& strUrl)
{
	m_Tab.SetFocus();

	COleVariant vtEmpty;
	m_Browser.Navigate(strUrl, &vtEmpty, &vtEmpty, &vtEmpty, &vtEmpty);

	m_Address.SetWindowText(strUrl);
	m_Address.SetEditSel(-1,0);
	m_Tab.SetFocus();
}

void CRenLibView::OnGo() 
{
	CString strUrl;
	m_Address.GetWindowText(strUrl);
	navigate(strUrl);
}

void CRenLibView::OnUpdateBrowserOpenInDefaultBrowser(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserOpenInDefaultBrowser() 
{
	CHyperLink::GotoURL(m_Browser.GetLocationURL());
}

void CRenLibView::initiateAddressUrl()
{
    // Initiate address combo control
	m_Address.AddString(Utils::GetString(IDS_NAME_RENJU_NET_PORTAL));
	mUrl.push_back(Utils::GetString(IDS_URL_RENJU_NET_PORTAL));

	m_Address.AddString(Utils::GetString(IDS_NAME_RENJU_INTERNATIONAL_FEDERATION));
	mUrl.push_back(Utils::GetString(IDS_URL_RENJU_INTERNATIONAL_FEDERATION));

	m_Address.AddString(Utils::GetString(IDS_NAME_ONLINE_RENJU_CLASS));
	mUrl.push_back(Utils::GetString(IDS_URL_ONLINE_RENJU_CLASS));

	m_Address.AddString(Utils::GetString(IDS_NAME_FORUM_ONLINE_RENJU_CLASS));
	mUrl.push_back(Utils::GetString(IDS_URL_FORUM_ONLINE_RENJU_CLASS));

	m_Address.AddString(Utils::GetString(IDS_NAME_RENLIB_HOMEPAGE_1ST));
	mUrl.push_back(Utils::GetString(IDS_URL_RENLIB_HOMEPAGE_1ST));

	m_Address.AddString(Utils::GetString(IDS_NAME_RENLIB_HOMEPAGE_2ND));
	mUrl.push_back(Utils::GetString(IDS_URL_RENLIB_HOMEPAGE_2ND));

	m_Address.AddString(Utils::GetString(IDS_NAME_RENLIB_NEWS));
	mUrl.push_back(mStartPage);

	m_Address.AddString(Utils::GetString(IDS_NAME_RENLIB_USERS_GUIDE));
	mUrl.push_back(Utils::getRenLibDirectory() + "RenLibUsersGuide.htm");

	m_Address.AddString(Utils::GetString(IDS_NAME_START_PAGE));
	mUrl.push_back(mStartPage);
}

void CRenLibView::OpenUrlInBrowser(const CString& strUrl)
{
	m_Tab.SetCurSel(TAB_BROWSER);
	showTabControls();
	navigate(strUrl);
}

void CRenLibView::OpenUrlInBrowser(int index)
{
	if (index < mUrl.size())
	{
		OpenUrlInBrowser(mUrl[index]);
	}
}

void CRenLibView::OnUpdateBrowserRenjuNetPortal(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenjuNetPortal() 
{
	OpenUrlInBrowser(URL_RENJU_NET_PORTAL);
}

void CRenLibView::OnUpdateBrowserRenjuInternationalFederation(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenjuInternationalFederation() 
{
	OpenUrlInBrowser(URL_RENJU_INTERNATIONAL_FEDERATION);
}

void CRenLibView::OnUpdateBrowserOnlineRenjuclass(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserOnlineRenjuclass() 
{
	OpenUrlInBrowser(URL_ONLINE_RENJU_CLASS);
}

void CRenLibView::OnUpdateBrowserForumOnlineRenjuclass(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserForumOnlineRenjuclass() 
{
	OpenUrlInBrowser(URL_FORUM_ONLINE_RENJU_CLASS);
}

void CRenLibView::OnUpdateBrowserRenLibHomepage1st(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenLibHomepage1st() 
{
	OpenUrlInBrowser(URL_RENLIB_HOMEPAGE_1ST);
}

void CRenLibView::OnUpdateBrowserRenLibHomepage2nd(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenLibHomepage2nd() 
{
	OpenUrlInBrowser(URL_RENLIB_HOMEPAGE_2ND);
}


void CRenLibView::OnUpdateBrowserRenLibNews(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenLibNews()
{
	OpenUrlInBrowser(URL_RENLIB_NEWS);
}

void CRenLibView::OnUpdateBrowserRenLibUsersGuide(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserRenLibUsersGuide() 
{
	OpenUrlInBrowser(URL_RENLIB_USERS_GUIDE);
}

void CRenLibView::OnUpdateBrowserStartpage(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserStartPage()
{
	OpenUrlInBrowser(URL_START_PAGE);
}

void CRenLibView::OnUpdateBrowserUseCurrentPageAsStartpage(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive());
}

void CRenLibView::OnBrowserUseCurrentPageAsStartPage()
{
	mStartPage = m_Browser.GetLocationURL();
	mUrl[URL_START_PAGE] = mStartPage;
}

void CRenLibView::OnBrowserOpenDiagrams() 
{
	CRenLibDoc* pDoc = GetDocument();

	if (pDoc->SaveCommand(true))
	{
		pDoc->NewLibraryCommand();

		pDoc->initMergeComment();

		CStringEx strAllPages(Utils::ReadWebPage(mCurrentUrl));

		while (!strAllPages.IsEmpty())
		{
			Game aGame;
			const bool multipleGames = true;
			Game::Result result = aGame.addWebGame(strAllPages);

			if (result == Game::VALID)
			{
				pDoc->AddGame(aGame);
			}
		}

		indicateConvertOrMerge(Utils::GetString(IDS_CAP_WEB_PAGE));

		showCommentTab();

		Invalidate(FALSE);
	}
}

void CRenLibView::OnUpdateBrowserOpenDiagrams(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();
	pCmdUI->Enable(!pDoc->isFindActive() &&
                   !mCurrentUrl.empty());
}

void CRenLibView::OnBrowserReadDiagrams() 
{
	CRenLibDoc* pDoc = GetDocument();

	pDoc->initMergeComment();

	CString strCaption(Utils::GetString(IDS_CAP_WEB_PAGE));

	CStringEx strAllPages(Utils::ReadWebPage(mCurrentUrl));

	while (!strAllPages.IsEmpty())
	{
		Game aGame;
		const bool multipleGames = true;
		Game::Result result = aGame.addWebGame(strAllPages);

		if (result == Game::VALID)
		{
			pDoc->AddGame(aGame);
		}
	}

	indicateConvertOrMerge(Utils::GetString(IDS_CAP_WEB_PAGE));

	showCommentTab();

	Invalidate(FALSE);
}

void CRenLibView::OnUpdateBrowserReadDiagrams(CCmdUI* pCmdUI) 
{
	CRenLibDoc* pDoc = GetDocument();

	pCmdUI->Enable(
		!pDoc->isFindActive() &&
        pDoc->isUpdateMode() && !mCurrentUrl.empty());
}

void CRenLibView::OnOpenWebPage()
{
	CRenLibDoc* pDoc = GetDocument();
	OpenUrlInBrowser(pDoc->getLastFile());
}

void CRenLibView::showCommentTab()
{
	m_Tab.SetCurSel(TAB_COMMENT);
	showTabControls();
}

void CRenLibView::CheckVersionAndNews()
{
    CRenLibDoc* pDoc = GetDocument();

	Registry reg;

	const int Version = pDoc->getVersion();

	int RegisteredVersion = reg.getInt(Registry::RegisteredVersion, 0);

	if (RegisteredVersion < Version)
	{
		CString strRequest;
		long    intValue;

		strRequest.Format(
			"http://www.lemes.se/cgi-bin/renlib/counter.php?version=%d", Version);

		CString strResponse(Utils::ReadWebPage(strRequest));
		Utils::trim(strResponse);

		if (Utils::StringToInt(strResponse, intValue))
		{
			reg.setInt(Registry::RegisteredVersion, Version);
		}
	}

    int NewsDate = reg.getInt(Registry::NewsDate, 0);

	if (NewsDate < Version)
	{
		NewsDate = Version;
	}

	const CString strNewsPage("http://www.renju.se/renlib/news.dat");
	long  intValue;
	const char SEPS[] = "|";

	CString strResponse(Utils::ReadWebPage(strNewsPage));
	Utils::trim(strResponse);

	if (!strResponse.IsEmpty())
	{
		CString strNewsDate(strtok(strResponse.GetBuffer(1), SEPS));
		CString strUrl(strtok(NULL, SEPS));

		if (!strUrl.IsEmpty())
		{
            mUrl[URL_RENLIB_NEWS] = strUrl;

            if (Utils::StringToInt(strNewsDate, intValue) && NewsDate < intValue)
		    {
			    reg.setInt(Registry::NewsDate, intValue);

			    OnBrowserRenLibNews();
		    }
        }
	}
}

UINT TimerThread(LPVOID pParam)
{
    ((CRenLibView*)pParam)->CheckVersionAndNews();

    return 0; // thread completed successfully
}

void CRenLibView::OnTimer(UINT nIDEvent)
{
	CFormView::OnTimer(nIDEvent);
	KillTimer(m_nTimer);

    switch (mTimerState)
    {
        case TIMER_VERSION_NEWS:
        {
            AfxBeginThread(TimerThread, this);
            break;
        }

        case TIMER_VCT_VCF:
        {
            CTime mEndTime = CTime::GetCurrentTime();

            CTimeSpan elapsedTime = mEndTime - mStartTime;

            CString timeString(elapsedTime.Format("%H:%M:%S "));

            CRenLibDoc* pDoc = GetDocument();

            if (pDoc->checkFindWin(m_eWinMarker, timeString))
            {
                mTimerState = TIMER_VCT_VCF;
                m_nTimer    = SetTimer(cTimerId, cTimeVcfVct, 0);
            }
            else
            {
                Invalidate(FALSE);
            }

            break;
        }
    }
}
