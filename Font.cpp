// Font.cpp: implementation of the Font class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "renlib.h"
#include "Font.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace
{
	const int     defaultPointSize[] = { 6, 10, 12 };

	const bool    defaultItalic    = false;
	const bool    defaultBold      = false;
	const CString defaultFaceName  = "System";
}

Font::Font()
{
	setDefault(0);
}

Font::~Font()
{
}

void Font::setDefault(int boardSize)
{
	mPointSize = defaultPointSize[boardSize];
    mIsBold    = defaultBold;
    mIsItalic  = defaultItalic;
	mFaceName  = defaultFaceName;
}

void Font::getLogFont(LOGFONT& lf)
{
	memset(&lf, 0, sizeof(LOGFONT));
	lf.lfHeight = mPointSize * 10;
	lf.lfWeight = mIsBold ? 700 : 400;
	lf.lfItalic = mIsItalic ? TRUE : FALSE;
	strcpy (lf.lfFaceName, mFaceName);
}


int Font::getPointSize()
{
	return mPointSize;
}
   
void Font::setPointSize(int pointSize)
{
	mPointSize = pointSize;
}

bool Font::isBold()
{
	return mIsBold;
}

void Font::setBold(bool bold)
{
	mIsBold = bold;
}

bool Font::isItalic()
{
    return mIsItalic;
}

void Font::setItalic(bool italic)
{
	mIsItalic = italic;
}

CString Font::getFaceName()
{
    return mFaceName;
}

void Font::setFaceName(const CString& faceName)
{
	mFaceName = faceName;
}

CString Font::toString()
{
	CString fontString;
	fontString.Format("%d,%d,%d,%s", mPointSize,mIsBold,mIsItalic,mFaceName);
	return fontString;
}

void Font::parse(CString fontString, int boardSize)
{
	setDefault(boardSize);

	char seps[] = ",";
	char *token;

	token = strtok( fontString.GetBufferSetLength(fontString.GetLength()), seps );

	if (token != NULL)
	{
		mPointSize = atoi(token);
		token = strtok( NULL, seps );
	}

	if (token != NULL)
	{
		mIsBold = (atoi(token) == 1);
		token = strtok( NULL, seps );
	}

	if (token != NULL)
	{
		mIsItalic = (atoi(token) == 1);
		token = strtok( NULL, seps );
	}

	if (token != NULL)
	{
		mFaceName = token;
		token = strtok( NULL, seps );
	}
}
