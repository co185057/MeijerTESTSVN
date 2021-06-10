#pragma once

class CSafeArrayVariant
{
public:
	CSafeArrayVariant(void);
	virtual ~CSafeArrayVariant(void);

	SAFEARRAY		*m_psa;	
	SAFEARRAYBOUND	m_rgsabound[1];
    VARIANT			m_var;

	void SetBuffer(UCHAR* pBuffer, ULONG ulnBytes);
	VARIANT &GetVariant();
	void SetVariant(VARIANT &var);
	void GetBufferFromVariant(const VARIANT &var, UCHAR* pBuffer, ULONG& ulnBytes);
	void GetBuffer(UCHAR* pBuffer, ULONG& ulnBytes);

private:
	CSafeArrayVariant (CSafeArrayVariant &);
	CSafeArrayVariant & operator =(const CSafeArrayVariant &);
};
