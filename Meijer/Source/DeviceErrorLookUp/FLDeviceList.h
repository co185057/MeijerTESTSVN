// FLDeviceList.h: interface for the CFLDeviceList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FLDEVICELIST_H__46A98B5B_DE0A_4919_85FB_3E68D2CAB5E6__INCLUDED_)
#define AFX_FLDEVICELIST_H__46A98B5B_DE0A_4919_85FB_3E68D2CAB5E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class FL_DeviceClass
{
public:
	FL_DeviceClass() {
		m_nID = 0;
		m_sClassName=_T("");
		m_sErrorXML = _T("");
	}

	_bstr_t &GetErrorXML() {return m_sErrorXML;}
	void SetErrorXML(_bstr_t & sErrorXML) { m_sErrorXML = sErrorXML;}

	_bstr_t &GetClassName() {return m_sClassName;}
	void SetClassName(_bstr_t & sClassName) { m_sClassName = sClassName;}

	int GetClassID() { return m_nID;}
	void SetClassID(int nID) { m_nID = nID;}

	void reset()
	{
		m_nID = 0;
		m_sClassName=_T("");
		m_sErrorXML = _T("");
	}
private:
	int m_nID;
	_bstr_t m_sClassName ;
	_bstr_t m_sErrorXML;
};


typedef std::list<FL_DeviceClass> DEVICESLIST ;

class CFLDeviceList  
{
public:
	CFLDeviceList();
	virtual ~CFLDeviceList();
	
	bool LoadDeviceList(_bstr_t & sFileName);
	bool GetDeviceListFile(_bstr_t & sConfigFile);
	bool GetDeviceErrorXML(int DeviceClass, _bstr_t& sXML,  _bstr_t& sClassName);
	bool UnLoadDeviceList();
	_bstr_t GetDeviceErrorPath() { return _bstr_t(m_sFLDevErrPath);}
	_bstr_t GetImagePath(){ return _bstr_t(m_sFLImagePath);}
	_bstr_t GetConfigPath(){ return _bstr_t(m_sFLConfigPath);}	

private:
	bool ValidateDeviceErrorMappingXML(LPTSTR sXML, LPTSTR sXSDFile);
	TCHAR m_sFLDevErrPath[_MAX_PATH];
	TCHAR m_sFLImagePath[_MAX_PATH];
	TCHAR m_sFLConfigPath[_MAX_PATH];
	TCHAR m_sXSDFile[_MAX_PATH];

	bool m_bValidateXML;
	
	DEVICESLIST m_aDeviceList;   //list of device classes


};

#endif // !defined(AFX_FLDEVICELIST_H__46A98B5B_DE0A_4919_85FB_3E68D2CAB5E6__INCLUDED_)
