// FontComboBox.h: interface for the CFontComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FONTCOMBOBOX_H__65F201C6_81F1_4DBB_961A_9FB59515267F__INCLUDED_)
#define AFX_FONTCOMBOBOX_H__65F201C6_81F1_4DBB_961A_9FB59515267F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ToolBarComboBox.h"
#define PRINTER_FONT 0x0100
#define TT_FONT	0x0200
#define DEVICE_FONT 0x0400
/**
 *	������������
 *	���������������
 */
class CFontDesc
{
public:
	CFontDesc(LPCTSTR lpszName, LPCTSTR lpszScript, BYTE nCharSet, 
		BYTE nPitchAndFamily, DWORD dwFlags);
	/** �������� */
	CString m_strName;
	/** �ű� */
	CString m_strScript;
	/** �ַ� */
	BYTE m_nCharSet;
	/** ��� */
	BYTE m_nPitchAndFamily;
	DWORD m_dwFlags;
};

class CFontComboBox : public CToolBarComboBox  
{
public:
	CFontComboBox();
public:
	/** ��������ӳ����� */
	CPtrArray m_arrayFontDesc;
	/** ����߶� */
	static int m_nFontHeight;
	/** �������� */
	CBitmap m_bmFontType;
public:
	/** ����ϵ�м��� */
	void EnumFontFamiliesEx(CDC& dc, BYTE nCharSet = DEFAULT_CHARSET);
	/** �������� */
	void AddFont(ENUMLOGFONT* pelf, DWORD dwType, LPCTSTR lpszScript = NULL);
	/** ����ƥ�� */
	void MatchFont(LPCTSTR lpszName, BYTE nCharSet);
	void EmptyContents();

	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBack(
		ENUMLOGFONT* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamPrinterCallBack(
		ENUMLOGFONT* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);
	static BOOL CALLBACK AFX_EXPORT EnumFamPrinterCallBackEx(
		ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
		LPVOID pThis);
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontComboBox)
	public:
	/** ���������� */
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	/** ������ߴ� */
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	/** ������Ƚ� */
	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	/** �������� */
	virtual ~CFontComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFontComboBox)
	/** ע����Ϣ */
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_FONTCOMBOBOX_H__65F201C6_81F1_4DBB_961A_9FB59515267F__INCLUDED_)
