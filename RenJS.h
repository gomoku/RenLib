// RenJS.h: interface for the RenJS class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RENJS_H__17E181E4_21CE_4989_A988_DCCA7631C0E7__INCLUDED_)
#define AFX_RENJS_H__17E181E4_21CE_4989_A988_DCCA7631C0E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class RenJS  
{
public:
	enum Result
	{
		VALID,
		EMPTY_ERROR,
		OPEN_FILE_ERROR,
		READ_FILE_ERROR,
		LESS_THAN_5_MOVES,
		ID_ERROR,
		COORDINATE_ERROR,
		NUMBER_ERROR,
		PARENT_ERROR,
		CHILD_ERROR
	};

	class Node
	{
	public:
		Node(RenJS* renjs);

		int getId();
		void setId(int id);

		BYTE getPos();
		void setPos(BYTE pos);

		int getNumber();
		void setNumber(int number);
		
		int getParent();
		void setParent(int parent);
		
		bool hasMoreChildren();
		Node* nextChild();
		void addChild(int child);
		
		CString getOneLineComment();
		void setOneLineComment(const CString& comment);

		CString getMultiLineComment();
		void setMultiLineComment(const CString& comment);

		CString getBoardText();
		void setBoardText(const CString& boardText);

	private:
		int              mId;
		BYTE             mPos;
		int              mNumber;
		int              mParent;
		std::vector<int> mChildren;
		int              mIndex;
		CString          mOneLineComment;
		CString          mMultiLineComment;
		CString          mBoardText;
		RenJS*           mRenjs;
	};

public:
	RenJS();

	Result addGame(const CString& strFile);

	bool  hasMoreGames();
	Node* nextGame();

	CString getFilePath();

	Result CheckMoves();

	Node* getNode(int id);

private:
	void parseGame();
	void parseMove(const CString& strMove);
	void parseParam(const CString& strParam, Node& node);
	void parseId(const CString& strData, Node& node);
	void parseCoordinate(const CString& strData, Node& node);
	void parseNumber(const CString& strData, Node& node);
	void parseParent(const CString& strData, Node& node);
	void parseChildren(const CString& strData, Node& node);
	void parseChild(const CString& strData, Node& node);
	void parseComment(const CString& strData, Node& node);
	void parseOneLineComment(const CString& strData, Node& node);
	void parseMultiLineComment(const CString& strData, Node& node);
	void parseBoardText(const CString& strData, Node& node);
	bool extractCoordinate(const CString& strData, CPoint& Move);

private:
	CStdioFile        mGameFile;
	std::vector<Node> mMoves;
	int               mIndex;
	Result            mResult;
	CString           mFilePath;

private:
	class Exception
	{
	public:
		Exception(Result errorCause);

		Result getErrorCause();

	private:
		Result mErrorCause;
	};
};

#endif // !defined(AFX_RENJS_H__17E181E4_21CE_4989_A988_DCCA7631C0E7__INCLUDED_)
