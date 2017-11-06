#include "stdafx.h"
#include "lreg.h"

/*================================================================ 
* ��������    CLReg
* ������      (HKEY hRootKey) 
* ˵����      ������캯��������������ʹ��Ĭ�ϵĲ�����m_hRootKey����ʼ��
			  ΪHKEY_LOCAL_MACHINE, ������в����� m_hRootKeyΪָ����ֵ
================================================================*/ 
CLReg::CLReg(HKEY hRootKey) 
{
	m_hRootKey = hRootKey; 
}

CLReg::~CLReg() //�����������йرմ�ע�����
{
	Close();
}



/*================================================================ 
* ��������    VerifyKey
* ������      (HKEY hRootKey, LPCTSTR pszPath) 
* ��������:   �жϸ�����·���Ƿ���� (���д򿪵Ĺ���)
			  �����һ������ΪNULL����ʹ��Ĭ�ϵĸ�����
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::VerifyKey (LPCTSTR pszPath)
{
 	LONG ReturnValue = ::RegOpenKeyEx (m_hRootKey, pszPath, 0L,
										KEY_ALL_ACCESS, &m_hSubKey);

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}


/*================================================================ 
* ��������    VerifyValue
* ������      (LPCTSTR pszValue)
* ��������:   �жϸ�����ֵ�Ƿ���� �����ȵ���VerifyKey��Ȼ����ʹ�øú�����
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::VerifyValue (LPCTSTR pszValue)
{
	LONG lReturn = ::RegQueryValueEx(m_hSubKey, pszValue, NULL,
		NULL, NULL, NULL);

	if(lReturn == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}

/*================================================================ 
* ��������    VerifyValue
* ������      (LPCTSTR pszValue)
* ��������:   �ж�ָ���ļ����Ƿ����ĳ��ֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::IsEqual(LPCTSTR pszValue,int nn)
{
	int nTemp;
	this->GetValue(pszValue,nTemp);
	if(nTemp==nn)
		return TRUE;
	return FALSE;
}
BOOL CLReg::IsEqual(LPCTSTR pszValue,DWORD dw)
{
	DWORD dwTemp;
	this->GetValue(pszValue,dwTemp);
	if(dwTemp==dw)
		return TRUE;
	return FALSE;
}
BOOL CLReg::IsEqual(LPCTSTR pszValue,LPCTSTR lpsz)
{
	CString str;
	this->GetValue(pszValue,str);
	if(str.CompareNoCase(lpsz)==0)
		return TRUE;
	return FALSE;
}


/*================================================================ 
* ��������    CreateKey
* ������      (HKEY hRootKey, LPCTSTR pszPath)
* ��������:   ����·��
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::CreateKey (LPCTSTR pszPath)
{
	DWORD dw;

	LONG ReturnValue = ::RegCreateKeyEx (m_hRootKey, pszPath, 0L, NULL,
							REG_OPTION_VOLATILE, KEY_ALL_ACCESS, NULL, 
							&m_hSubKey, &dw);

	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;

	return FALSE;
}



/*================================================================ 
* ��������    SetValue
* ������      (LPCTSTR lpszKeyName, int iVal)
* ��������:   д������ֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::SetValue (LPCTSTR lpszKeyName, int iVal)
{
	DWORD dwValue;

	dwValue = (DWORD)iVal;
	LONG ReturnValue = ::RegSetValueEx(m_hSubKey, lpszKeyName, 0L, REG_DWORD,
		(CONST unsigned char*) &dwValue, sizeof(DWORD));


	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}

/*================================================================ 
* ��������    SetValue
* ������      (LPCTSTR lpszKeyName, DWORD dwVal)
* ��������:   д��DWORDֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::SetValue (LPCTSTR lpszKeyName, DWORD dwVal)
{
	return ::RegSetValueEx (m_hSubKey, lpszKeyName, 0L, REG_DWORD,
		(CONST unsigned char*) &dwVal, sizeof(DWORD));
}


/*================================================================ 
* ��������    SetValue
* ������      (LPCTSTR lpszKeyName, LPCTSTR pszData)
* ��������:   д���ַ���ֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::SetValue (LPCTSTR lpszKeyName, LPCTSTR pszData)
{

	LONG ReturnValue = ::RegSetValueEx (m_hSubKey, lpszKeyName, 0L, REG_SZ,
		(CONST unsigned char*) pszData, strlen(pszData) + 1);


	if(ReturnValue == ERROR_SUCCESS)
		return TRUE;
	
	return FALSE;
}


/*================================================================ 
* ��������    GetValue
* ������      (LPCTSTR lpszKeyName, int& iVal) ��2������ͨ�����ô��ݣ������ں������޸�ʵ��
* ��������:   ��ȡ����
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::GetValue(LPCTSTR lpszKeyName, int& iVal)
{

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = ::RegQueryValueEx (m_hSubKey, (LPSTR) lpszKeyName, NULL,
		&dwType, (unsigned char *) &dwDest, &dwSize);

	if(lReturn == ERROR_SUCCESS)
	{
		iVal = (int)dwDest;
		return TRUE;
	}

	return FALSE;
}


/*================================================================ 
* ��������    GetValue
* ������      (LPCTSTR lpszKeyName, DWORD& dwVal) ��2������ͨ�����ô��ݣ������ں������޸�ʵ��
* ��������:   ��ȡDWORDֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::GetValue (LPCTSTR lpszKeyName, DWORD& dwVal)
{

	DWORD dwType;
	DWORD dwSize = sizeof (DWORD);
	DWORD dwDest;

	LONG lReturn = ::RegQueryValueEx (m_hSubKey, (LPSTR) lpszKeyName, NULL, 
		&dwType, (unsigned char *) &dwDest, &dwSize);


	if(lReturn == ERROR_SUCCESS)
	{
		dwVal = dwDest;
		return TRUE;
	}

	return FALSE;
}


/*================================================================ 
* ��������    GetValue
* ������      (LPCTSTR lpszKeyName, CString& sVal) ��2������ͨ�����ô��ݣ������ں������޸�ʵ��
* ��������:   ��ȡ�ַ���ֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::GetValue (LPCTSTR lpszKeyName, CString& sVal)
{

	DWORD dwType;
	DWORD dwSize = 200;
	char  szString[255];

	LONG lReturn = ::RegQueryValueEx (m_hSubKey, (LPSTR) lpszKeyName, NULL,
		&dwType, (unsigned char *) szString, &dwSize);


	if(lReturn == ERROR_SUCCESS)
	{
		sVal = szString;
		return TRUE;
	}

	return FALSE;
}



/*================================================================ 
* ��������    DeleteValue
* ������      (LPCTSTR pszValue) 
* ��������:   ɾ��ֵ
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::DeleteValue (LPCTSTR pszValue)
{
	if(::RegDeleteValue(m_hSubKey, pszValue)== ERROR_SUCCESS)		
		return TRUE;
	else
		return FALSE;
}

/*================================================================ 
* ��������    DeleteKey
* ������      (HKEY hRootKey, LPCTSTR pszPath) 
* ��������:   ɾ��·��
* ����ֵ��    BOOL
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
BOOL CLReg::DeleteKey (LPCTSTR pszPath)
{

	if(::RegDeleteKey(m_hRootKey, pszPath) == ERROR_SUCCESS)
		return TRUE;
	else
		return FALSE;
}


/*================================================================ 
* ��������    Close
* ������      
* ��������:   �ر�ע���
* ����ֵ��    void
* �� �ߣ�     ������ 2005-09-12
================================================================*/ 
 void CLReg::Close()
{
	if (m_hSubKey)
	{
		::RegCloseKey (m_hSubKey);
		m_hSubKey = NULL;
	}
}


void CLReg::SetRootKey(HKEY root)
{
	m_hRootKey=root;
}
