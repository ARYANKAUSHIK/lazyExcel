#ifndef _LGUILINK_H_INCLUDE_
#define _LGUILINK_H_INCLUDE_
#include "../../LSheetLib/Public/LSheetLink.h"
//Gridʵ���ļ�
#ifdef _WIN32
	#include "../../OSFace/WIN32/GDI/LPrintDC.cpp"
#else
	#include "../../OSFace/Linux/GDI/LPrintDC.cpp"
	//��֧�ֵ�ƽ̨
#endif
#include "../../LSheetLib/Src/LSheet.cpp"
#endif