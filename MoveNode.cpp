// MoveNode.cpp: implementation of the MoveNode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RenLib.h"
#include "MoveNode.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------
// const
//--------------------------------------------------------------------

namespace
{
	//
	// Extension bytes
	//
	const BOARD_TEXT  = 0x000100;

	//
	// Low byte
	//
	const DOWN        = 0x000080;
	const RIGHT       = 0x000040;
	const OLD_COMMENT = 0x000020;
	const MARK        = 0x000010;
	const COMMENT     = 0x000008;
	const START       = 0x000004;
	const NO_MOVE     = 0x000002;
	const EXTENSION   = 0x000001;

	const MASK        = 0xFFFF3F;

	const CPoint NullPoint(0, 0);

	bool isValid(CPoint Pos)
	{
		return
			Pos == NullPoint || (Pos.x >= 1 && Pos.x <= 15 && Pos.y >= 1 && Pos.y <= 15);
	}
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MoveNode::MoveNode() :
	mPos(NullPoint),
	mInfo(0),
	mMatch(0),
	mDown(0),
	mRight(0)
{
}

MoveNode::MoveNode(MoveNode& node) :
	mPos(node.mPos),
	mInfo(node.mInfo),
	mMatch(0),
	mDown(0),
	mRight(0)
{
	if (!isValid(mPos))
	{
		//
		// ERROR checking code
		//
		CString strMessage;
		strMessage += Utils::GetString(IDS_MSG_POSITION, mPos);
		strMessage += "\n\n";
		strMessage += Utils::GetString(IDS_MSG_REPORT);
		strMessage += "\n\n";
		strMessage += Utils::GetString(IDS_MSG_NOTE);
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_MAKE), MB_ICONERROR);

		setPos(CPoint(1,1));
		setIsMark(true);
	}
}

MoveNode::MoveNode(CPoint Pos) :
	mPos(Pos),
	mInfo(0),
	mMatch(0),
	mDown(0),
	mRight(0)
{
	if (!isValid(mPos))
	{
		//
		// ERROR checking code
		//
		CString strMessage;
		strMessage += Utils::GetString(IDS_MSG_POSITION, mPos);
		strMessage += "\n\n";
		strMessage += Utils::GetString(IDS_MSG_REPORT);
		strMessage += "\n\n";
		strMessage += Utils::GetString(IDS_MSG_NOTE);
		Utils::ShowMessage(strMessage, Utils::GetString(IDS_CAP_MAKE), MB_ICONERROR);

		setPos(CPoint(1,1));
		setIsMark(true);
	}
}

MoveNode::~MoveNode()
{
}

void MoveNode::checkExtension()
{
	setIsExtension((mInfo & 0xFFFF00) != 0);
}

bool MoveNode::isValue(UINT bitValue) const
{
	return Utils::bit_is_one(bitValue, mInfo);
}

void MoveNode::setIsValue(bool value, UINT bitValue)
{
	if (value)
	{
		Utils::set_bit(bitValue, mInfo);
	}
	else
	{
		Utils::clear_bit(bitValue, mInfo);
	}
}

void MoveNode::setPos(CPoint pos)
{
	mPos = pos;
	setIsMove(true);
}

CPoint MoveNode::getPos() const
{
	return mPos;
}

void MoveNode::setPosInfo(BYTE pos, BYTE info)
{
	mPos  = Utils::PosToPoint(pos);
	mInfo = (mInfo & 0xFFFF00) | info;
}

void MoveNode::getPosInfo(BYTE& pos, BYTE& info) const
{
	pos  = Utils::PointToPos(mPos);
	info = BYTE(mInfo & 0xFF);
}

void MoveNode::setExtendedInfo(BYTE info2, BYTE info1)
{
	mInfo &= 0xFF;
	mInfo |= ((info2 << 8) | info1) << 8;
}

void MoveNode::getExtendedInfo(BYTE& info2, BYTE& info1) const
{
	UINT info = mInfo >> 8;
	info1 = BYTE(info & 0xFF);

	info >>= 8;
	info2 = BYTE(info & 0xFF);
}

void MoveNode::clearInformation()
{
	mInfo = 0;
}

bool MoveNode::isInformation() const
{
	return (mInfo & MASK) != 0;
}

bool MoveNode::isDown() const
{
	return isValue(DOWN);
}

void MoveNode::setIsDown(bool value)
{
	setIsValue(value, DOWN);
}

bool MoveNode::isRight() const
{
	return isValue(RIGHT);
}

void MoveNode::setIsRight(bool value)
{
	setIsValue(value, RIGHT);
}

bool MoveNode::isOldComment() const
{
	return isValue(OLD_COMMENT);
}

bool MoveNode::isNewComment() const
{
	return isValue(COMMENT);
}

void MoveNode::setIsNewComment(bool value)
{
	setIsValue(value, COMMENT);
	setIsValue(false, OLD_COMMENT);
}

bool MoveNode::isMark() const
{
	return isValue(MARK);
}

void MoveNode::setIsMark(bool value)
{
	setIsValue(value, MARK);
}

bool MoveNode::isStart() const
{
	return isValue(START);
}

void MoveNode::setIsStart(bool value)
{
	setIsValue(value, START);
}

bool MoveNode::isMove() const
{
	return !isValue(NO_MOVE);
}

bool MoveNode::isPassMove() const
{
    bool result = false;

    if (isMove())
    {
        result = (mPos.x == 0 && mPos.y == 0);
    }

    return result;
}

void MoveNode::setIsMove(bool value)
{
	setIsValue(!value, NO_MOVE);
}

bool MoveNode::isExtension() const
{
	return isValue(EXTENSION);
}

void MoveNode::setIsExtension(bool value)
{
	setIsValue(value, EXTENSION);
}

void MoveNode::setMatch(BYTE match)
{
	mMatch = match;
}

BYTE MoveNode::getMatch() const
{
	return mMatch;
}

void MoveNode::setDown(MoveNode* node)
{
	mDown = node;
}

MoveNode* MoveNode::getDown() const
{
	return mDown;
}

void MoveNode::setRight(MoveNode* node)
{
	mRight = node;
}

MoveNode* MoveNode::getRight() const
{
	return mRight;
}

bool MoveNode::isOneLineComment() const
{
	return !mOneLineComment.IsEmpty();
}

void MoveNode::setOneLineComment(const CString& comment)
{
	mOneLineComment = comment;
	setIsNewComment(isOneLineComment() || isMultiLineComment());
}

CString MoveNode::getOneLineComment() const
{
	return mOneLineComment;
}

bool MoveNode::isMultiLineComment() const
{
	return !mMultiLineComment.IsEmpty();

}

void MoveNode::setMultiLineComment(const CString& comment)
{
	mMultiLineComment = comment;
	setIsNewComment(isOneLineComment() || isMultiLineComment());
}

CString MoveNode::getMultiLineComment() const
{
	return mMultiLineComment;
}

void MoveNode::setIsBoardText(bool value)
{
	setIsValue(value, BOARD_TEXT);
	checkExtension();
}

bool MoveNode::isBoardText() const
{
	return isValue(BOARD_TEXT);
}

void MoveNode::setBoardText(const CString& text)
{
	mBoardText = text;
	setIsBoardText(!mBoardText.IsEmpty());
}

CString MoveNode::getBoardText() const
{
	return mBoardText;
}
