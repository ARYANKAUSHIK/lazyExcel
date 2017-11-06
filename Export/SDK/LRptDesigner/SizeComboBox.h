#if !defined(AFX_SIZECOMBOBOX_H__908610CF_5AB8_466D_8D42_30647521D496__INCLUDED_)
#define AFX_SIZECOMBOBOX_H__908610CF_5AB8_466D_8D42_30647521D496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SizeComboBox.h : header file
//
#include "ToolBarComboBox.h"
/////////////////////////////////////////////////////////////////////////////
// CSizeComboBox window

class CSizeComboBox : public CToolBarComboBox
{
public:
	/** �����Сѡ�������캯�� */
	CSizeComboBox();

// Attributes
public:

	int m_nLogVert;
	int m_nTwipsLast;

// Operations
public:
	/** �����С�о� */
	void EnumFontSizes(CDC& dc, LPCTSTR pFontName);
	/** ��С�ͺ��оٻص����� */
	static BOOL FAR PASCAL EnumSizeCallBack(LOGFONT FAR* lplf, 
		LPNEWTEXTMETRIC lpntm,int FontType, LPVOID lpv);
	/** ��Twipsת��Ϊ���ִ� */
	void TwipsToPointString(LPTSTR lpszBuf, int nTwips);
	/** ����twips�Ĵ�С */
	void SetTwipSize(int nSize);
	/** ��ȡtwips�Ĵ�С */
	int GetTwipSize();
	/** ����һ����С�ͺ� */
	void InsertSize(int nSize);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSizeComboBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	/** �������� */
	virtual ~CSizeComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSizeComboBox)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIZECOMBOBOX_H__908610CF_5AB8_466D_8D42_30647521D496__INCLUDED_)
