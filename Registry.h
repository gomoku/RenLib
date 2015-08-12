// Registry.h: interface for the Registry class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REGISTRY_H__F6128891_1475_43EC_A2FE_C768ABC5C820__INCLUDED_)
#define AFX_REGISTRY_H__F6128891_1475_43EC_A2FE_C768ABC5C820__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class Registry  
{
public:

	enum RegistryKey
	{
		// Int
		BackgroundColor,
		BlackALVColor,
		BlackCMOColor,
		BlackNumberColor,
		BlackStoneColor,
		BlackVariantColor,
		BlackVcfColor,
		BlackVctColor,
		BoardColor,
		BoardSize,
		BoardTextColor,
		CommentBkColor,
		CommentTextColor,
		Coordinates,
		CoordTextColor,
		CustColors_0,
		CustColors_1,
		CustColors_2,
		CustColors_3,
		CustColors_4,
		CustColors_5,
		CustColors_6,
		CustColors_7,
		CustColors_8,
		CustColors_9,
		CustColors_10,
		CustColors_11,
		CustColors_12,
		CustColors_13,
		CustColors_14,
		CustColors_15,
		Evaluator,
		ForbiddenColor,
		LastMoveColor,
		WinMarker,
		WhiteALVColor,
		WhiteCMOColor,
		WhiteNumberColor,
		WhiteStoneColor,
		WhiteVariantColor,
		WhiteVcfColor,
		WhiteVctColor,
		RegisteredVersion,
		NewsDate,
		
		// Bool
		AutoFindSamePosition,
		AutoPointMouse,
		BoardTextExtraSpace,
		CoordinatesFollow,
		IsZoomed,
		ShowAutoLinkVariants,
		ShowBoardBitmap,
		ShowBoardText,
		ShowChangedOrderOfMovesVariants,
		ShowComments,
		ShowForbidden,
		ShowLastMove,
		ShowNumbers,
		ShowStoneBitmap,
		ShowVariants,
		SwapRowNumber,

		// String
		BoardBitmap,
		FontSmall,
		FontMedium,
		FontLarge,
		FontBoardTextSmall,
		FontBoardTextMedium,
		FontBoardTextLarge,
		FontCommentSmall,
		FontCommentMedium,
		FontCommentLarge,
		StartPage
	};

public:
	Registry();
	~Registry();

	UINT getInt(RegistryKey key, int nDefault);
	void setInt(RegistryKey key, int nValue);

	bool getBool(RegistryKey key, int bDefault);
	void setBool(RegistryKey key, int bValue);

	CString getString(RegistryKey key, const CString& strDefault);
	void    setString(RegistryKey key, const CString& strValue);

private:
	CString getSection(RegistryKey key);
	CString getEntry(RegistryKey key);

private:
	CWinApp* mApp;
};

#endif // !defined(AFX_REGISTRY_H__F6128891_1475_43EC_A2FE_C768ABC5C820__INCLUDED_)
