// Registry.cpp: implementation of the Registry class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Registry.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace
{
	const CString strOptions("Options");
	const CString strView("View");
	const CString strWindow("Window");
	const CString strInternal("Internal");

	const CString strEntry[] =
	{
		// Int
		"BackgroundColor",                      //BackgroundColor
		"BlackAutoLinkVariantColor",            //BlackALVColor
		"BlackChangedOrderOfMovesVariantColor", //BlackCMOColor
		"BlackNumberColor",                     //BlackNumberColor
		"BlackStoneColor",                      //BlackStoneColor
		"BlackVariantColor",                    //BlackVariantColor
		"BlackVcfColor",                        //BlackVcfColor
		"BlackVctColor",                        //BlackVctColor
		"Color",                                //BoardColor
		"BoardSize",                            //BoardSize
		"BoardTextColor",                       //BoardTextColor
		"CommentBkColor",                       //CommentBkColor
		"CommentTextColor",                     //CommentTextColor
		"Coordinates",                          //Coordinates
		"CoordTextColor",                       //CoordTextColor
		"CustColors_0",                         //CustColors_0
		"CustColors_1",                         //CustColors_1
		"CustColors_2",                         //CustColors_2
		"CustColors_3",                         //CustColors_3
		"CustColors_4",                         //CustColors_4
		"CustColors_5",                         //CustColors_5
		"CustColors_6",                         //CustColors_6
		"CustColors_7",                         //CustColors_7
		"CustColors_8",                         //CustColors_8
		"CustColors_9",                         //CustColors_9
		"CustColors_10",                        //CustColors_10
		"CustColors_11",                        //CustColors_11
		"CustColors_12",                        //CustColors_12
		"CustColors_13",                        //CustColors_13
		"CustColors_14",                        //CustColors_14
		"CustColors_15",                        //CustColors_15
		"Evaluator",                            //Evaluator
		"ForbiddenColor",                       //ForbiddenColor
		"LastMoveColor",                        //LastMoveColor
		"WinMarker",                            //WinMarker
		"WhiteAutoLinkVariantColor",            //WhiteALVColor
		"WhiteChangedOrderOfMovesVariantColor", //WhiteCMOColor
		"WhiteNumberColor",                     //WhiteNumberColor
		"WhiteStoneColor",                      //WhiteStoneColor
		"WhiteVariantColor",                    //WhiteVariantColor
		"WhiteVcfColor",                        //WhiteVcfColor
		"WhiteVctColor",                        //WhiteVctColor
		"RegisteredVersion",                    //RegisteredVersion
		"NewsDate",                             //NewsDate
		
        // Bool
		"AutoFindSamePosition",                 //AutoFindSamePosition
		"AutoPointMouse",                       //AutoPointMouse
		"BoardTextExtraSpace",                  //BoardTextExtraSpace
		"CoordinatesFollow",                    //CoordinatesFollow
		"IsZoomed",                             //IsZoomed
		"ShowAutoLinkVariants",                 //ShowAutoLinkVariants
		"ShowBoardBitmap",                      //ShowBoardBitmap
		"ShowBoardText",                        //ShowBoardText
		"ShowChangedOrderOfMovesVariants",      //ShowChangedOrderOfMovesVariants
		"ShowComments",                         //ShowComments
		"ShowForbidden",                        //ShowForbidden
		"ShowLastMove",                         //ShowLastMove
		"ShowNumbers",                          //ShowNumbers
		"ShowStoneBitmap",                      //ShowStoneBitmap
		"ShowVariants",                         //ShowVariants
		"SwapRowNumber",                        //SwapRowNumber

        // String
		"BoardBitmap",                          //BoardBitmap
		"Font0",                                //FontSmall
		"Font1",                                //FontMedium
		"Font2",                                //FontLarge
		"FontBoardText0",                       //FontBoardTextSmall
		"FontBoardText1",                       //FontBoardText2
		"FontBoardText2",                       //FontBoardText3
		"FontComment0",                         //FontCommentSmall
		"FontComment1",                         //FontComment1
		"FontComment2",                         //FontComment2
		"StartPage"                             //StartPage
	};
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Registry::Registry()
: mApp(AfxGetApp())
{
}

Registry::~Registry()
{
}

UINT Registry::getInt(RegistryKey key, int nDefault)
{
	return mApp->GetProfileInt(getSection(key), getEntry(key), nDefault);
}

void Registry::setInt(RegistryKey key, int nValue)
{
	mApp->WriteProfileInt(getSection(key), getEntry(key), nValue);
}

bool Registry::getBool(RegistryKey key, int bDefault)
{
	return mApp->GetProfileInt(getSection(key), getEntry(key), bDefault) == 1;
}

void Registry::setBool(RegistryKey key, int bValue)
{
	mApp->WriteProfileInt(getSection(key), getEntry(key), bValue);
}

CString Registry::getString(RegistryKey key, const CString& strDefault)
{
	return mApp->GetProfileString(getSection(key), getEntry(key), strDefault);
}

void Registry::setString(RegistryKey key, const CString& strValue)
{
	mApp->WriteProfileString(getSection(key), getEntry(key), strValue);
}

CString Registry::getSection(RegistryKey key)
{
	CString strSection;

	switch (key)
	{
		case IsZoomed:
		{
			strSection = strWindow;
			break;
		}

		case Evaluator:
		{
			strSection = strOptions;
			break;
		}

		case RegisteredVersion:
		case NewsDate:
		{
			strSection = strInternal;
			break;
		}

		case BackgroundColor:
		case BlackALVColor:
		case BlackCMOColor:
		case BlackNumberColor:
		case BlackStoneColor:
		case BlackVariantColor:
		case BlackVcfColor:
		case BlackVctColor:
		case BoardColor:
		case BoardSize:
		case BoardTextColor:
		case CommentBkColor:
		case CommentTextColor:
		case Coordinates:
		case CoordTextColor:
		case CustColors_0:
		case CustColors_1:
		case CustColors_2:
		case CustColors_3:
		case CustColors_4:
		case CustColors_5:
		case CustColors_6:
		case CustColors_7:
		case CustColors_8:
		case CustColors_9:
		case CustColors_10:
		case CustColors_11:
		case CustColors_12:
		case CustColors_13:
		case CustColors_14:
		case CustColors_15:
		case ForbiddenColor:
		case LastMoveColor:
		case WinMarker:
		case WhiteALVColor:
		case WhiteCMOColor:
		case WhiteNumberColor:
		case WhiteStoneColor:
		case WhiteVariantColor:
		case WhiteVcfColor:
		case WhiteVctColor:
		case AutoFindSamePosition:
		case AutoPointMouse:
		case BoardTextExtraSpace:
		case CoordinatesFollow:
		case ShowAutoLinkVariants:
		case ShowBoardBitmap:
		case ShowBoardText:
		case ShowChangedOrderOfMovesVariants:
		case ShowComments:
		case ShowForbidden:
		case ShowLastMove:
		case ShowNumbers:
		case ShowStoneBitmap:
		case ShowVariants:
		case SwapRowNumber:
		case BoardBitmap:
		case FontSmall:
		case FontMedium:
		case FontLarge:
		case FontBoardTextSmall:
		case FontBoardTextMedium:
		case FontBoardTextLarge:
		case FontCommentSmall:
		case FontCommentMedium:
		case FontCommentLarge:
		case StartPage:
		{
			strSection = strView;
			break;
		}

		default:
		{
			ASSERT(FALSE);
		}
	}

	return strSection;
}

CString Registry::getEntry(RegistryKey key)
{
	return strEntry[key];
}
