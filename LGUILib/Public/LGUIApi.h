#ifndef _LGUIAPI_H_INCLUDE_
	#define _LGUIAPI_H_INCLUDE_
	#include "../../LComm/LGlobalParam.h"

	#ifdef LZY_API
		#undef LZY_API
	#endif
	//DRAWER�����ڲ���Ϣ����
	/////////////////////////////////////////////////////
	#define MSG_ONDRAWER					1003	//��ͼ��Ϣ
	#define	TMSG_DRAW_CELL					1//���Ƶ�Ԫ��
	#define	TMSG_DRAW_SCROLLBAR				2//���ƹ�����
	#define	TMSG_DRAW_TABLEITEM				3//����TAB��
	#define	TMSG_DRAW_TABLEBUTTOM			4//
	#ifdef LGUILIB_EXPORTS
		#define GUI_EXPORT_CLASS	__declspec(dllexport)
		#define LZY_API(type)  extern "C" type __declspec(dllexport)
	#else
		#define GUI_EXPORT_CLASS	__declspec(dllimport)
		#define LZY_API(type)  extern "C" type __declspec(dllimport)
	#endif
	#ifndef _LINK_DYN_TYPE_
		#ifndef LGUILIB_EXPORTS
			#undef GUI_EXPORT_CLASS
			#define GUI_EXPORT_CLASS
		#endif
	#else
		#pragma comment(lib,"LSheetLib.lib")
		#pragma comment(lib,"LGridLib.lib")
		#pragma comment(lib,"LTableLib.lib")
	#endif

#endif