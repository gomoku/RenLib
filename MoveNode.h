// MoveNode.h: interface for the MoveNode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MOVE_NODE_H__990E8D64_0BDE_11D4_92A3_CEA74A1A6D25__INCLUDED_)
#define AFX_MOVE_NODE_H__990E8D64_0BDE_11D4_92A3_CEA74A1A6D25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class MoveNode
{
public:
	MoveNode();
	explicit MoveNode(MoveNode& node);
	explicit MoveNode(CPoint Pos);

	virtual ~MoveNode();

	void setPosInfo(BYTE pos, BYTE info);
	void getPosInfo(BYTE& pos, BYTE& info) const;

	void setExtendedInfo(BYTE info2, BYTE info1);
	void getExtendedInfo(BYTE& info2, BYTE& info1) const;

	//
	// Position
	//
	void setPos(CPoint pos);
	CPoint getPos() const;

	//
	// Information
	//
	void clearInformation();
	bool isInformation() const;

	bool isDown() const;
	void setIsDown(bool value);

	bool isRight() const;
	void setIsRight(bool value);

	bool isOldComment() const;
	bool isNewComment() const;

	bool isMark() const;
	void setIsMark(bool value);

	bool isStart() const;
	void setIsStart(bool value);

	bool isMove() const;
	bool isPassMove() const;
	void setIsMove(bool value);

	bool isExtension() const;
	void setIsExtension(bool value);

	void setMatch(BYTE match);
	BYTE getMatch() const;

	//
	// Instance data
	//
	void setDown(MoveNode* node);
	MoveNode* getDown() const;

	void setRight(MoveNode* node);
	MoveNode* getRight() const;

	bool isOneLineComment() const;
	void setOneLineComment(const CString& comment);
	CString getOneLineComment() const;

	bool isMultiLineComment() const;
	void setMultiLineComment(const CString& comment);
	CString getMultiLineComment() const;

	bool isBoardText() const;
	void setBoardText(const CString& text);
	CString getBoardText() const;

private:
	void setIsNewComment(bool value);
	void setIsBoardText(bool value);
	void checkExtension();
	bool isValue(UINT bitValue) const;
	void setIsValue(bool value, UINT bitValue);

private:
	CPoint		mPos;              // position, (x,y)
	UINT		mInfo;             // information

	BYTE		mMatch;            // position search
	
	CString		mOneLineComment;   // one line comment
	CString		mMultiLineComment; // multi line comment
	CString		mBoardText;        // board text

	MoveNode*	mDown;             // next level
	MoveNode*	mRight;            // same level
};

#endif // !defined(AFX_MOVE_NODE_H__990E8D64_0BDE_11D4_92A3_CEA74A1A6D25__INCLUDED_)