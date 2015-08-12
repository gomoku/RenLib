// RenJS.cpp: implementation of the RenJS class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenJS.h"
#include "Utils.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace
{
	const CenterPos = 8 + 7*16; // Center Intersection
}

//------------------------------------------------------------------------

RenJS::RenJS()
: mResult(VALID)
{
}

//------------------------------------------------------------------------

bool RenJS::hasMoreGames()
{
	Node* node = getNode(0);

	VERIFY(node != 0);

	return node->hasMoreChildren();
}

//------------------------------------------------------------------------

RenJS::Node*
RenJS::nextGame()
{
	Node* node = getNode(0);

	VERIFY(node != 0);

	return node->nextChild();
}

//------------------------------------------------------------------------

RenJS::Node*
RenJS::getNode(int id)
{
	int left  = 0;
	int right = mMoves.size();
	int index;

	while (true)
	{
		index = (left + right) / 2;

		if (id == mMoves[index].getId())
		{
			return &mMoves[index];
		}
		else if (id < mMoves[index].getId())
		{
			right = index;
		}
		else if (id > mMoves[index].getId())
		{
			left = index;
		}
		else
		{
			return 0;
		}
	}
}

//------------------------------------------------------------------------

RenJS::Result
RenJS::CheckMoves()
{
	if (mMoves.size() < 5)
	{
		return LESS_THAN_5_MOVES;
	}

	if (!hasMoreGames())
	{
		return EMPTY_ERROR;
	}

	return VALID;
}

//------------------------------------------------------------------------

RenJS::Result
RenJS::addGame(const CString& strFile)
{
	CFileException e;

	if (!mGameFile.Open(strFile, CFile::modeRead, &e))
	{
		mResult = OPEN_FILE_ERROR;
	}
	else
	{
		try
		{
			parseGame();

			if (mMoves.size() == 0)
			{
				mResult = EMPTY_ERROR;
			}
		}
		catch (Exception e)
		{
			mResult = e.getErrorCause();
		}

		mGameFile.Close();
	}

	if (mResult == VALID)
	{
		mResult = CheckMoves();
	}

	return mResult;
}

//------------------------------------------------------------------------

CString RenJS::getFilePath()
{
	return mFilePath;
}

//------------------------------------------------------------------------

void RenJS::parseGame()
{
	mFilePath = mGameFile.GetFilePath();

	CString strLine;

	if (!mGameFile.ReadString(strLine))
	{
		throw Exception(READ_FILE_ERROR);
	}

	Utils::trim(strLine);

	int left  = strLine.Find("[");
	int right = strLine.Find("]", left);

	while (left < right)
	{
		CString strMove(strLine.Mid(left + 1, right - left - 1));

		parseMove(strMove);

		left  = strLine.Find("[", right);
		right = strLine.Find("]", left);
	}
}

//------------------------------------------------------------------------

void RenJS::parseMove(const CString& strMove)
{
	Node node(this);

	int left  = 0;
	int right = strMove.Find(";");

	while (left < right)
	{
		CString strParam(strMove.Mid(left, right - left));

		parseParam(strParam, node);
		
		left  = right + 1;
		right = strMove.Find(";", left);
	}

	mMoves.push_back(node);
}

//------------------------------------------------------------------------

void RenJS::parseParam(const CString& strParam, RenJS::Node& node)
{
	int index = strParam.Find("=");

	CString strKey(strParam.Left(index));
	CString strValue(strParam.Right(strParam.GetLength() - 1 - index));

	if (strKey == "i")
	{
		parseId(strValue, node);
	}
	else if (strKey == "p")
	{
		parseCoordinate(strValue, node);
	}
	else if (strKey == "m")
	{
		parseNumber(strValue, node);
	}
	else if (strKey == "f")
	{
		parseParent(strValue, node);
	}
	else if (strKey == "c")
	{
		parseChildren(strValue, node);
	}
	else if (strKey == "co")
	{
		parseComment(strValue, node);
	}
	else if (strKey == "k")
	{
		parseBoardText(strValue, node);
	}
}

//------------------------------------------------------------------------

void RenJS::parseId(const CString& strData, RenJS::Node& node)
{
	int id;
	
	if (Utils::StringToInteger(strData, id))
	{
		node.setId(id);
	}
	else
	{
		throw Exception(ID_ERROR);
	}
}

//------------------------------------------------------------------------

void RenJS::parseCoordinate(const CString& strData, RenJS::Node& node)
{
	CPoint move;
	
	if (extractCoordinate(strData, move))
	{
		node.setPos(Utils::PointToPos(move));
	}
	else
	{
		throw Exception(COORDINATE_ERROR);
	}
}

//------------------------------------------------------------------------

void RenJS::parseNumber(const CString& strData, RenJS::Node& node)
{
	int number;
	
	if (Utils::StringToInteger(strData, number))
	{
		node.setNumber(number);
	}
	else
	{
		throw Exception(NUMBER_ERROR);
	}
}

//------------------------------------------------------------------------

void RenJS::parseParent(const CString& strData, RenJS::Node& node)
{
	int parent;
	
	if (Utils::StringToInteger(strData, parent))
	{
		node.setParent(parent);
	}
	else
	{
		throw Exception(PARENT_ERROR);
	}
}

//------------------------------------------------------------------------

void RenJS::parseChildren(const CString& strData, RenJS::Node& node)
{
	int left  = 0;

	if (strData[left] == ',')
	{
		left++;
	}

	int right = strData.Find(",", left);

	while (left < right)
	{
		CString strChild(strData.Mid(left, right - left));

		if (!strChild.IsEmpty())
		{
			parseChild(strChild, node);
		}
		
		left  = right + 1;
		right = strData.Find(",", left);
	}
}

//------------------------------------------------------------------------

void RenJS::parseChild(const CString& strData, RenJS::Node& node)
{
	int child;
	
	if (Utils::StringToInteger(strData, child))
	{
		node.addChild(child);
	}
	else
	{
		throw Exception(CHILD_ERROR);
	}
}

//------------------------------------------------------------------------

void RenJS::parseComment(const CString& strData, RenJS::Node& node)
{
	int left  = strData.Find('\1');
	int right = strData.Find('\2', left);

	CString strOneLineComment(strData.Mid(left + 1, right - left - 1));
	node.setOneLineComment(strOneLineComment);

	left  = right;
	right = strData.Find('\3', left);

	CString strMultiLineComment(strData.Mid(left + 1, right - left - 1));
	node.setMultiLineComment(strMultiLineComment);
}

//------------------------------------------------------------------------

void RenJS::parseBoardText(const CString& strData, RenJS::Node& node)
{
	int left  = strData.Find('\4');
	int right = strData.Find('\5', left);

	CString strBoardText(strData.Mid(left + 1, right - left - 1));
	node.setBoardText(strBoardText);
}

//------------------------------------------------------------------------

bool RenJS::extractCoordinate(const CString& strData, CPoint& move)
{
	if (strData.GetLength() >= 2 && strData.GetLength() <= 3)
	{
		if (strData[0] >= 'A' && strData[0] <= 'O')
		{
			move.x = strData[0] - 'A' + 1;
		}
		else
		{
			return false;
		}

		CString strNumber(strData);
		strNumber.Delete(0);

		int number;

		if (Utils::StringToInteger(strNumber, number) && number >= 1 && number <= 15)
		{
			move.y = 16 - number;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

//------------------------------------------------------------------------

RenJS::Node::Node(RenJS* renjs)
: mId(0),
  mPos(0),
  mNumber(0),
  mParent(0),
  mIndex(0),
  mRenjs(renjs)
{
}

//------------------------------------------------------------------------

int RenJS::Node::getId()
{
	return mId;
}

//------------------------------------------------------------------------

void RenJS::Node::setId(int id)
{
	mId = id;
}

//------------------------------------------------------------------------

BYTE RenJS::Node::getPos()
{
	return mPos;
}

//------------------------------------------------------------------------

void RenJS::Node::setPos(BYTE pos)
{
	mPos = pos;
}

//------------------------------------------------------------------------

int RenJS::Node::getNumber()
{
	return mNumber;
}

//------------------------------------------------------------------------

void RenJS::Node::setNumber(int number)
{
	mNumber = number;
}

//------------------------------------------------------------------------

int RenJS::Node::getParent()
{
	return mParent;
}

//------------------------------------------------------------------------

void RenJS::Node::setParent(int parent)
{
	mParent = parent;
}
		
//------------------------------------------------------------------------

bool RenJS::Node::hasMoreChildren()
{
	return mIndex < mChildren.size();
}

//------------------------------------------------------------------------

RenJS::Node*
RenJS::Node::nextChild()
{
	VERIFY(hasMoreChildren());

	return mRenjs->getNode(mChildren[mIndex++]);
}

//------------------------------------------------------------------------

void RenJS::Node::addChild(int child)
{
	mChildren.push_back(child);
}
		
//------------------------------------------------------------------------

CString RenJS::Node::getOneLineComment()
{
	return mOneLineComment;
}

//------------------------------------------------------------------------

void RenJS::Node::setOneLineComment(const CString& comment)
{
	mOneLineComment = comment;
}

//------------------------------------------------------------------------

CString RenJS::Node::getMultiLineComment()
{
	return mMultiLineComment;
}

//------------------------------------------------------------------------

void RenJS::Node::setMultiLineComment(const CString& comment)
{
	mMultiLineComment = comment;
}

//------------------------------------------------------------------------

CString RenJS::Node::getBoardText()
{
	return mBoardText;
}

//------------------------------------------------------------------------

void RenJS::Node::setBoardText(const CString& boardText)
{
	mBoardText = boardText;
}

//------------------------------------------------------------------------

RenJS::Exception::Exception(Result errorCause)
: mErrorCause(errorCause)
{
}

//------------------------------------------------------------------------

RenJS::Result
RenJS::Exception::getErrorCause()
{
	return mErrorCause;
}

