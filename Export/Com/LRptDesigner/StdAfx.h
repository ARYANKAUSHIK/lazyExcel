// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__9DB0C2EA_9A8F_4258_82BD_6CE1242A64D5__INCLUDED_)
#define AFX_STDAFX_H__9DB0C2EA_9A8F_4258_82BD_6CE1242A64D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxsock.h>		// MFC socket extensions
static LOGFONT DEFAULT_LOGFONT={-12, 0, 0, 0, FW_NORMAL, 0, 0, 0, 134, 3, 2, 1, 2, _T("����")};
static CFont m_fontDefault;
extern CFont* m_pDefaultFont;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__9DB0C2EA_9A8F_4258_82BD_6CE1242A64D5__INCLUDED_)
