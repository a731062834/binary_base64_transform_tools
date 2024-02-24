
// Base64_Tools.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CBase64_ToolsApp:
// See Base64_Tools.cpp for the implementation of this class
//

class CBase64_ToolsApp : public CWinApp
{
public:
	CBase64_ToolsApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CBase64_ToolsApp theApp;