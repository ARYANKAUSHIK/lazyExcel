#ifndef __LTABLEAPI_H
	#define __LTABLEAPI_H
	#include "../../LComm/LGlobalParam.h"

	//�������ڲ���Ϣ����
	/////////////////////////////////////////////////////
	#define MSG_STYLECHANGE					1000	//��ʽ�ı�
	#define	TMSG_STYLECHANGE_BKCOLOR		1//������ɫ�ı�
	#define	TMSG_STYLECHANGE_BORDERCOLOR	2//�߿���ɫ�ı�
	#define	TMSG_STYLECHANGE_FONTCOLOR		3//�ı���ɫ�ı�
	#define	TMSG_STYLECHANGE_FONTNAME		4//
	#define	TMSG_STYLECHANGE_FONTSIZE		5
	#define	TMSG_STYLECHANGE_FONTBOLD		6
	#define	TMSG_STYLECHANGE_FONTITALIC		7
	#define	TMSG_STYLECHANGE_FONTUNDERLINE	8
	#define	TMSG_STYLECHANGE_TYLE			9
	#define TMSG_STYLECHANGE_FONTALIGN  	10

	#define MSG_CALLBACK(msg,command,wparam,lparam) \
		BOOL bCancel=FALSE;\
		NotifyProc(msg,command,wparam,lparam,bCancel);\
		if(bCancel)return
	/////////////////////////////////////////////////////
	#ifdef LZY_API
		#undef LZY_API
	#endif
	
	#ifdef LTABLELIB_EXPORTS
		#define TABLE_EXPORT_CLASS	__declspec(dllexport)
		#define LZY_API(type)  extern "C" type __declspec(dllexport)
	#else
		#define TABLE_EXPORT_CLASS	__declspec(dllimport)
		#define LZY_API(type)  extern "C" type  __declspec(dllimport)
	#endif
	#ifndef _LINK_DYN_TYPE_
		#ifndef LTABLELIB_EXPORTS
			#undef TABLE_EXPORT_CLASS
			#define TABLE_EXPORT_CLASS
		#endif
	#else
	#endif
#endif