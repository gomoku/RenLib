# Microsoft Developer Studio Project File - Name="RenLib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=RenLib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "RenLib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "RenLib.mak" CFG="RenLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "RenLib - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "RenLib - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""$/RenLib", BAAAAAAA"
# PROP Scc_LocalPath "."
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "RenLib - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Imagehlp.lib ForbiddenPointFinder.lib Ws2_32.lib bestmove.lib /nologo /subsystem:windows /map /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"libcmtd.lib" /nodefaultlib:"msvcrtd.lib" /libpath:"External\Release"
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "RenLib - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x41d /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x41d /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ForbiddenPointFinder.lib Ws2_32.lib bestmove.lib /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"libc.lib" /nodefaultlib:"libcmt.lib" /nodefaultlib:"msvcrt.lib" /nodefaultlib:"libcd.lib" /nodefaultlib:"msvcrtd.lib" /pdbtype:sept /libpath:"External\Debug"

!ENDIF 

# Begin Target

# Name "RenLib - Win32 Release"
# Name "RenLib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AmHttpSocket.cpp
# End Source File
# Begin Source File

SOURCE=.\AutoBoardText.cpp
# End Source File
# Begin Source File

SOURCE=.\Bdt.cpp
# End Source File
# Begin Source File

SOURCE=.\Board.cpp
# End Source File
# Begin Source File

SOURCE=.\Buf.cpp
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.cpp
# End Source File
# Begin Source File

SOURCE=.\CISBitmap.cpp
# End Source File
# Begin Source File

SOURCE=.\DIBSectionLite.cpp
# End Source File
# Begin Source File

SOURCE=.\EditBoardText.cpp
# End Source File
# Begin Source File

SOURCE=.\FileInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Font.cpp
# End Source File
# Begin Source File

SOURCE=.\Game.cpp
# End Source File
# Begin Source File

SOURCE=.\GCColorEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\HyperLink.cpp
# End Source File
# Begin Source File

SOURCE=.\IntReg.cpp
# End Source File
# Begin Source File

SOURCE=.\LibraryFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MergeComment.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveList.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveNode.cpp
# End Source File
# Begin Source File

SOURCE=.\Pdb.cpp
# End Source File
# Begin Source File

SOURCE=.\Pos.cpp
# End Source File
# Begin Source File

SOURCE=.\Position.cpp
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.cpp
# End Source File
# Begin Source File

SOURCE=.\Rdf.cpp
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\Renartist.cpp
# End Source File
# Begin Source File

SOURCE=.\RenJS.cpp
# End Source File
# Begin Source File

SOURCE=.\Renjuclass.cpp
# End Source File
# Begin Source File

SOURCE=.\RenLib.cpp
# End Source File
# Begin Source File

SOURCE=.\RenLib.rc
# End Source File
# Begin Source File

SOURCE=.\RenLibApplet.cpp
# End Source File
# Begin Source File

SOURCE=.\RenLibAppletDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\RenLibDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\RenLibView.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchComment.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchItem.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchList.cpp
# End Source File
# Begin Source File

SOURCE=.\SearchMove.cpp
# End Source File
# Begin Source File

SOURCE=.\Sgf.cpp
# End Source File
# Begin Source File

SOURCE=.\Stack.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StringEx.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBoard.cpp
# End Source File
# Begin Source File

SOURCE=.\TextBoxDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Tree.cpp
# End Source File
# Begin Source File

SOURCE=.\Utils.cpp
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.cpp
# End Source File
# Begin Source File

SOURCE=.\Wzq.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AmHttpSocket.h
# End Source File
# Begin Source File

SOURCE=.\AutoBoardText.h
# End Source File
# Begin Source File

SOURCE=.\Bdt.h
# End Source File
# Begin Source File

SOURCE=.\External\bestmove.h
# End Source File
# Begin Source File

SOURCE=.\Board.h
# End Source File
# Begin Source File

SOURCE=.\Buf.h
# End Source File
# Begin Source File

SOURCE=.\cgfiltyp.h
# End Source File
# Begin Source File

SOURCE=.\CISBitmap.h
# End Source File
# Begin Source File

SOURCE=.\DIBSectionLite.h
# End Source File
# Begin Source File

SOURCE=.\EditBoardText.h
# End Source File
# Begin Source File

SOURCE=.\FileInfo.h
# End Source File
# Begin Source File

SOURCE=.\FolderDlg.h
# End Source File
# Begin Source File

SOURCE=.\Font.h
# End Source File
# Begin Source File

SOURCE=.\FontDialog.h
# End Source File
# Begin Source File

SOURCE=.\External\ForbiddenPointFinder.h
# End Source File
# Begin Source File

SOURCE=.\Game.h
# End Source File
# Begin Source File

SOURCE=.\GCColorEdit.h
# End Source File
# Begin Source File

SOURCE=.\HyperLink.h
# End Source File
# Begin Source File

SOURCE=.\IntReg.h
# End Source File
# Begin Source File

SOURCE=.\LibraryFile.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MergeComment.h
# End Source File
# Begin Source File

SOURCE=.\ModeInformationI.h
# End Source File
# Begin Source File

SOURCE=.\MoveList.h
# End Source File
# Begin Source File

SOURCE=.\MoveNode.h
# End Source File
# Begin Source File

SOURCE=.\Pdb.h
# End Source File
# Begin Source File

SOURCE=.\Pos.h
# End Source File
# Begin Source File

SOURCE=.\Position.h
# End Source File
# Begin Source File

SOURCE=.\ProgressWnd.h
# End Source File
# Begin Source File

SOURCE=.\Rdf.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
# End Source File
# Begin Source File

SOURCE=.\Renartist.h
# End Source File
# Begin Source File

SOURCE=.\RenJS.h
# End Source File
# Begin Source File

SOURCE=.\Renjuclass.h
# End Source File
# Begin Source File

SOURCE=.\RenLib.h
# End Source File
# Begin Source File

SOURCE=.\RenLibApplet.h
# End Source File
# Begin Source File

SOURCE=.\RenLibAppletDialog.h
# End Source File
# Begin Source File

SOURCE=.\RenLibDoc.h
# End Source File
# Begin Source File

SOURCE=.\RenLibView.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SearchComment.h
# End Source File
# Begin Source File

SOURCE=.\SearchItem.h
# End Source File
# Begin Source File

SOURCE=.\SearchList.h
# End Source File
# Begin Source File

SOURCE=.\SearchMove.h
# End Source File
# Begin Source File

SOURCE=.\Sgf.h
# End Source File
# Begin Source File

SOURCE=.\Stack.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\StringEx.h
# End Source File
# Begin Source File

SOURCE=.\TextBoard.h
# End Source File
# Begin Source File

SOURCE=.\TextBoxDlg.h
# End Source File
# Begin Source File

SOURCE=.\Tree.h
# End Source File
# Begin Source File

SOURCE=.\Utils.h
# End Source File
# Begin Source File

SOURCE=.\webbrowser2.h
# End Source File
# Begin Source File

SOURCE=.\Wzq.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\hand.cur
# End Source File
# Begin Source File

SOURCE=.\res\InvalidBitmap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\RenLib.ico
# End Source File
# Begin Source File

SOURCE=.\res\RenLib.rc2
# End Source File
# Begin Source File

SOURCE=.\res\RenLibDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\stoneBlackLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoneBlackSmall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoneBlackSmallMedium.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoneWhiteLarge.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoneWhiteMedium.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stoneWhiteSmall.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbaredit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarfile.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarfind.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarmove.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbarposition.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\RenLibSuggestions.html
# End Source File
# Begin Source File

SOURCE=.\RenLibUsersGuide.htm
# End Source File
# Begin Source File

SOURCE=.\TODO.txt
# End Source File
# End Target
# End Project
