//link with wininet.lib

#pragma once

#include <tchar.h>
#include <windows.h>
#include <wininet.h>

/*
	custom errorcodes:
	-1: bad url...
*/

class CAmHttpSocket
{
public:
	int GetPageStatusCode(); //get the HTTP statuscode for the last received page
	TCHAR* GetHeaders(const TCHAR *url); //return a pointer th the headers from an url
	CAmHttpSocket();
	~CAmHttpSocket();
	char* GetPage(const TCHAR *url, bool Post = false, const char *PostData = NULL, int PostDataLength = -1); //get a page, if post is false, HTTP GET is used othervise HTTP POST is used. if PostDataLength is -1 the data must be NULL terminated...

  void setAgent (CString strAgentName)
    { m_strAgentName = strAgentName; }

protected:
	bool PostUrl(const TCHAR *url, const char *PostData, int PostDataLength = -1); //open a page using http post
	TCHAR* GetHeaderLine(TCHAR *s); //get a specific line from the headers
	bool OpenUrl(const TCHAR *url); //open a page using http get
	HINTERNET hIO, hIS, hCO;
	char *ReceivedData; //the internal databuffer
	TCHAR *Headers; //the internal headerbuffer
	int LastError; //internal statuscode...

  CString m_strAgentName;   // agent name
};