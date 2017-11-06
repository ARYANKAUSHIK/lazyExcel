#ifndef _LSHEETAPI_H_
	#define _LSHEETAPI_H_
	#include "../../LComm/LGlobalParam.h"

	#ifdef LZY_API
		#undef LZY_API
	#endif
	//SHEET�����ڲ���Ϣ����
	/////////////////////////////////////////////////////
	#define MSG_ONSHEET					1002	//��ʽ�ı�
	#define	TMSG_SHEET_REPLACERANGE			1//���򸲸�
	#define	TMSG_SHEET_FILLRANGE			2//�������
	#define	TMSG_SHEET_CLICK				3//��굥����Ϣ
	
	#ifdef LSHEETLIB_EXPORTS
		#define SHEET_EXPORT_CLASS	__declspec(dllexport)
		#define LZY_API(type)  extern "C" type __declspec(dllexport)
	#else
		#define SHEET_EXPORT_CLASS	__declspec(dllimport)
		#define LZY_API(type)  extern "C" type __declspec(dllimport)
	#endif
	#ifndef _LINK_DYN_TYPE_
		#ifndef LSHEETLIB_EXPORTS
			#undef SHEET_EXPORT_CLASS
			#define SHEET_EXPORT_CLASS
		#endif
	#else
		#pragma comment(lib,"LGridLib.lib")
		#pragma comment(lib,"LTableLib.lib")
	#endif
#endif