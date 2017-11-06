//////////////////////////////////////////////////////////////////////
//@ģ��		 �������������
//@�汾		 ��v1.0.0
//@����		 ��������
//@ʱ��		 ��2005-11-07
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LSHEET_H__B5A957A8_C638_4307_90BF_80135C75090F__INCLUDED_)
#define AFX_LSHEET_H__B5A957A8_C638_4307_90BF_80135C75090F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../Public/LSheetPublic.h"
#include "../Interface/LSheetInterface.h"

class SHEET_EXPORT_CLASS CLSheet :public ISheet, 
	public CLGrid,public IMouseEvent,public ISizeNotify
{
	friend class CLBook;
	friend class CLDrawerSheet;
private:
	void ShowToolTip(LPPOINT ppt,LPTSTR ptrMsg,BOOL bShow=TRUE);

	BOOL isCanMove(const RECT &rctSrc, const RECT &rctTo);
	void CopyRange(const RANGE& rangeSrc, const RANGE& rangeDst, BOOL bFillByRule=FALSE);
	void TrackRangeSel(LONG nRow,LONG nCol,BOOL bCombinRgn=TRUE);
	void ClearFlags();
	void DrawTrack(BOOL bHorz=TRUE,BOOL bMove=TRUE);
	void OuterHitest(HITEST &hitest, const POINT &pt, 
				 const CELL &cellBegin, const POINT &ptOffset);
	BOOL IsInDrawerObjectArea(HITEST& hitest,MOUSESTATUS& nstatus
		,const POINT& pt,BOOL& bError,BOOL bRaiseEvent=TRUE);
	BOOL isInClient(HITEST& hitest,const POINT& pt,const CELL& cellBegin,
		const POINT& ptOffset,BOOL& bError);
	BOOL isInFixedArea(HITEST& hitest,const POINT& pt,const CELL& cellBegin,
		const POINT& ptOffset,BOOL& bError);
	BOOL isInFreezeArea(HITEST& hitest,const POINT& pt,const CELL& cellBegin,
		const POINT& ptOffset,BOOL& bError);
	BOOL GetPosInfo(HITEST& hitest,const POINT& pt,
		const RANGE& range,const POINT& ptOffset,const RECT& rctClip);

	virtual BOOL call_function(const string& name,const tag_func_param_value_chain* param
		,tag_result_type type,tag_computer_result& ret,string& error,LPVOID pparam);
	virtual BOOL get_macro_define(const string& name,tag_computer_result& ret
		,string& error,LPVOID pparam);
	HITEST			m_hitest;//����������
	LPVOID			m_lpOldObj;
	LONG			m_oldID;
	
	IDirectDraw*	m_ptrDDraw;
	LPIDAREA		m_ptrArea;
	CLDrawItemMgrl	m_graphics;
	LPCHAIN		m_pFunChain;/*�����ص���*/
	LONG		m_nSheetLineStyle;
	
	BOOL		m_bRangeBorderDown;
	BOOL		m_bRangeCornerDown;
	BOOL		m_bResizeFixedCellOnly;//�Ƿ��������ڹ̶���Ԫʱ���ܸı����д�С
	BOOL		m_bCellAreaDown;
	BOOL		m_bRowColSelDown;
	BOOL		m_bSelectRow;
	BOOL		m_bSelectCol;
	BOOL		m_bGraphicsDown;
	 
	BOOL		m_bTrackColSize;
	BOOL		m_bTrackRowSize;//
	POINT		m_ptTrack,m_ptrOld_rc;
	
	LONG	m_nTrackOffset;
	RECT	m_rctTrack;
	LONG	m_oldPs;
	LONG	m_nRowColID;
	LONG	m_nSelMask;
	
	BOOL	m_bActive;
	RECT	m_rctAll;
	RECT	m_rctClient;
	RECT	m_rctTotal;
	POINT	m_posStart;
	POINT	m_posStartDraw;
	LPTSTR	m_ptrName;
	
	//������־
	
	LONG	m_nFillType;	//����������
	BOOL	m_bProtected;
	BOOL	m_bCanOperate;//�Ƿ��ܹ�������Sheet����
	LPTSTR	m_strPass;
	//��Ϣ�ص�����
	void OnHitest(HITEST& hitest,const POINT& pt);
	virtual LONG NotifyProc(LONG wMsg,LONG nCmd,WPARAM wParam,
		LPARAM lParam,BOOL& bCancel);
	virtual __inline CLDrawItemMgrl*	GetDrawItemsArr(){return &m_graphics;};
	__inline void SetDrawMgrlSizeNotify(){m_graphics.SetSizeNotify((ISizeNotify*)this);};
	__inline void SetDrawMgrlRedrawHelp(IRedrawHelp* ptrobj){m_graphics.SetRedrawHelp(ptrobj);};
public:
	virtual LONG GetMousePosition(const POINT& pt)
	{
		HITEST hitest;
		BOOL bError=FALSE;
		MOUSESTATUS nstatus;
		if(!IsInDrawerObjectArea(hitest,nstatus,pt,bError,FALSE))
			OnHitest(hitest,pt);
		return hitest.nHitest;
	}
	virtual BOOL GetPanRect(RANGE& range,POINT ptOffset,const SIZE& size,RECT& rct);
	virtual BOOL OnObjectMove(CELL& cellBegin,CELL& cellEnd,POINT& ptOffset,
		POINT& ptEndOffset,const SIZE& size,LONG cx,LONG cy,RECT& rctAll);
	virtual void OnObjectSize(CELL& cellEnd,POINT& ptEndOffset,LONG cx,LONG cy,RECT& rctAll);

	virtual BOOL OnSerial(CLSerial& ar);
	virtual __inline BOOL IsSelFixedRow(){return m_bSelectRow;};
	virtual __inline BOOL IsSelFixedCol(){return m_bSelectCol;};
	virtual BOOL IsResizeInFixedCell();
	virtual void IsResizeInFixedCell(BOOL bVal);
	
	virtual void SetDDrawObj(IDirectDraw* ptrObj);
	virtual void SetBeginPos(const POINT& pt);
	CLSheet();
	virtual ~CLSheet();
	virtual __inline ICallServ* GetCallServ(){return (ICallServ*)this;}
	virtual __inline IGrid* GetGrid(){return (IGrid*)this;};
	
	virtual LPTSTR GetSheetName();
	virtual void SetSheetName(LPTSTR ptrName);
	
	
	virtual void DeleteDrawItem(CLDrawItemBase* ptrItem);
	virtual BOOL BringToTop(CLDrawItemBase *ptrItem);
	virtual BOOL BringToBottom(CLDrawItemBase *ptrItem);
	virtual BOOL BringToBack(CLDrawItemBase* ptrItem);
	virtual BOOL BringToFront(CLDrawItemBase* ptrItem);
	virtual LONG AddDrawItem(CLDrawItemBase* ptrItem);
	virtual BOOL IsTopMost(CLDrawItemBase* ptrItem);
	virtual BOOL IsBottom(CLDrawItemBase* ptrItem);
	virtual CLDrawItemBase* GetTopMostItem();
	virtual CLDrawItemBase* GetBottomItem();
	virtual void SetFocusItem(CLDrawItemBase* ptrItem);
	virtual CLDrawItemBase* GetFocusItem();

	virtual LONG DefaultMessageProc(LONG wMsg,LONG nCmd,
		WPARAM wParam,LPARAM lParam,BOOL& bCancel);
	
	virtual BOOL SetMessageFilter(LPVOID ptrFun);
	virtual BOOL RemoveMessageFilter(LPVOID ptrFun);
	//���ص��¼�
	virtual void OnMouseMove(LONG idArea,MOUSESTATUS& nstatus);
	virtual void OnMouseOut(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnLButtonDown(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnLButtonUp(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnRButtonDown(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnRButtonUp(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnMButtonDown(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnMButtonUp(LONG idArea,const MOUSESTATUS& nstatus);
	virtual void OnMouseWheel(LONG idArea,const MOUSESTATUS& nstatus);
	virtual BOOL GetRect(LONG idArea,RECT& rct);
	virtual BOOL GetRgn(LONG idArea,HRGN& hrgn);
	virtual LPIDAREA GetAreaIDs();
	virtual BOOL IsActive();
	/////////////////////////////////////////////////////////////////////////
	virtual void IsActive(BOOL val);
	virtual LONG GetSheetLineStyle();
	virtual void SetSheetLineStyle(LONG val);
};

#endif // !defined(AFX_LSHEET_H__B5A957A8_C638_4307_90BF_80135C75090F__INCLUDED_)
