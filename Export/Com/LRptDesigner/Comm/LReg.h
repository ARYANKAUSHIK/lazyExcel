#if !defined _LREG_H
#define _LREG_H

class CLReg : public CObject
{
public:
	CLReg(HKEY hRootKey = HKEY_LOCAL_MACHINE); //���캯������Ĭ�ϲ���
	virtual ~CLReg();

public:
	void SetRootKey(HKEY root);
	BOOL VerifyKey (LPCTSTR pszPath); 
	BOOL VerifyValue (LPCTSTR pszValue);

	BOOL CreateKey (LPCTSTR pszPath);
	void Close();

	BOOL DeleteValue (LPCTSTR pszValue);
	BOOL DeleteKey (LPCTSTR pszPath);

	BOOL SetValue (LPCTSTR pszKey, int iVal);
	BOOL SetValue (LPCTSTR pszKey, DWORD dwVal);
	BOOL SetValue (LPCTSTR pszKey, LPCTSTR pszVal);

	BOOL GetValue (LPCTSTR pszKey, int& iVal);
	BOOL GetValue (LPCTSTR pszKey, DWORD& dwVal);
	BOOL GetValue (LPCTSTR pszKey, CString& sVal);

	BOOL IsEqual(LPCTSTR pszValue,int nn);
	BOOL IsEqual(LPCTSTR pszValue,DWORD dw);
	BOOL IsEqual(LPCTSTR pszValue,LPCTSTR lpsz);

protected:	
	HKEY 	m_hSubKey;    //����򿪵��Ӽ����
	HKEY    m_hRootKey;   //����������
};

#endif
