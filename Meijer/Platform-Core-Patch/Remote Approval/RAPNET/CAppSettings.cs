//
// CAppSettings.cs
//
// Change history:
// POS101386 Work Request:61233  Name: Saiprasad Srihasam Date: June 26, 2014
// POS100102 Work Request:61233  Name:Saiprasad Srihasam  Date:June 17, 2014
//
//////////////////////////////////////////////////////////////////////



using System;
using RPSWNET;

namespace RapNet
{
	/// <summary>
	/// Summary description for CAppSettings.
	/// </summary>
	public class CAppSettings
	{
		private string m_strStoreNumber;
		private string m_strDevWebServiceTemplate;
		private string m_strPrdWebServiceTemplate;
		private string m_strCertWebServiceTemplate;
		private string m_strEnvironmentType;
		private string []m_strCertStoreNumbers;	//POS100102
		private string m_strWebServiceTemplate; //POS101386

		/// <summary>
		/// Get property for Store number.
		/// </summary>
		public string propstrStoreNumber 
		{
			get
			{
				return m_strStoreNumber;
			}
		}

		public string propDevWebServiceTemplate
		{
			get
			{
				return m_strDevWebServiceTemplate;
			}
		}

		public string propCertWebServiceTemplate
		{
			get
			{
				return m_strCertWebServiceTemplate;
			}
		}

		
		/// <summary>
		/// Get property for PrdWebServiceTemplate
		/// </summary>

		public string propPrdWebServiceTemplate
		{
			get
			{
				return m_strPrdWebServiceTemplate;
			}
		}

		///<summary>
		///Get property for EnvironmentType.
		///</summary>
		


		public string propEnvironmentType
		{
			get
			{
				return m_strEnvironmentType;
			}
		}

//(+) POS100102
		///<summary>
		///Get Property for TestStoreNumbers
		///</summary>

		public string[] propCertStoreNumbers
		{
			get
			{
				return m_strCertStoreNumbers;
			}
		}

//(-) POS100102

// (+) POS101386
		public string propWebServiceTemplate
		{
			get
			{
				return m_strWebServiceTemplate;
			}
		}
// (-) POS101386

		/// <summary>
		/// Constructor
		/// </summary>
		public CAppSettings()
		{
            m_strPrdWebServiceTemplate = System.Configuration.ConfigurationSettings.AppSettings["PrdWebServiceTemplate"];
            m_strCertWebServiceTemplate = System.Configuration.ConfigurationSettings.AppSettings["CertWebServiceTemplate"];
            m_strDevWebServiceTemplate = System.Configuration.ConfigurationSettings.AppSettings["DevWebServiceTemplate"];
            string str = System.Configuration.ConfigurationSettings.AppSettings["LabStoreNumbers"]; // POS100102
			string strMachineName = System.Environment.MachineName;
            m_strWebServiceTemplate = System.Configuration.ConfigurationSettings.AppSettings["WebServiceTemplate"]; // POS101386
//(+) POS100102
			m_strStoreNumber = strMachineName.Substring(5, 4);
			string strTemp = m_strStoreNumber.TrimStart('0');
			m_strCertStoreNumbers = str.Split(',');
			string strTestStore;

			bool bMatch = false;
			for(int i=0;i<m_strCertStoreNumbers.Length; i++)
			{
				strTestStore = m_strCertStoreNumbers.GetValue(i).ToString();
				strTestStore = strTestStore.TrimStart(' ');
				strTestStore = strTestStore.TrimEnd(' ');
				strTestStore = strTestStore.TrimStart('0');

				if(strTemp == strTestStore)
				{
					bMatch = true;
					break;
				}
			}

			if(bMatch)
			{
				m_strEnvironmentType = "2";
			}
			else
			{
				m_strEnvironmentType = "1";
			}
//(-) POS100102
		}

	}
}
