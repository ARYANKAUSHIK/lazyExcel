#if !defined(AFX_TOOLBARCOMBOBOX_H__2258F600_8F5D_4CC6_B1FC_0DE5A9C0C97E__INCLUDED_)
#define AFX_TOOLBARCOMBOBOX_H__2258F600_8F5D_4CC6_B1FC_0DE5A9C0C97E__INCLUDED_

#include "LFlatComboBox.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ToolBarComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CToolBarComboBox window
class CToolBarComboBox : public CLFlatComboBox
{
// Construction
public:
	CToolBarComboBox();

// Attributes
public:
	/** ��¼���ַ��� */
	int m_nLimitText;
	/** ����ı��ִ� */
	void GetTheText(CString& str);
	/** �����ı��ִ� */
	void SetTheText(LPCTSTR lpszText,BOOL bMatchExact = FALSE);

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CToolBarComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	/** �������� */
	virtual ~CToolBarComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CToolBarComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.



/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLBARCOMBOBOX_H__2258F600_8F5D_4CC6_B1FC_0DE5A9C0C97E__INCLUDED_)
