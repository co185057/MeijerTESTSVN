using System;
using System.Collections;
using System.Xml;
using System.IO;
using RPSWNET;
using System.Text;

namespace LaunchPadNet
{
	/// <summary>
	/// Summary description for ConfigReader.
	///
	/// Read Executables and General Application information from LaunchPad configuration.
	/// Executable elements in here is link with buttons in LaunchPad PSX configuration
	/// </summary>
	public class ConfigReader
	{
    
		Executable m_ExecutableTemp=null;
		Hashtable m_Executables= new Hashtable();


		private App m_app = new App();
    
		public ConfigReader()
		{
			//
			// TODO: Add constructor logic here
			//              
		}
    
		//function must not return false, otherwise unload launchpad from the caller    
		public bool ReadXMLFile( String sXMLFile ) 
		{
			bool bResult = true;
      
			XmlTextReader xmlReader =null;
      
			try
			{
				xmlReader=new XmlTextReader(sXMLFile);                

				while( xmlReader.Read())
				{
					switch (xmlReader.NodeType)
					{       
             
						case XmlNodeType.Comment:
							break ; // Added for the sake of data capture not logging 
						case XmlNodeType.XmlDeclaration:
							break;  // Added for the sake of data capture not logging 
						case XmlNodeType.Whitespace:
							break;  // Added for the sake of data capture not logging 
						case XmlNodeType.Element:
							this.ProcessXMLElements( xmlReader);
							break;
						case XmlNodeType.EndElement:
							this.ProcessXMLEndElements( xmlReader);
							break;
						default:
							CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format("<Warning> [ConfigReader.ReadXML()] Unknown Node Type: {0}:{1}", xmlReader.Name, xmlReader.Value));
							break;
					}
        
				}      
			}
			catch( FileNotFoundException ex)
			{
				bResult = false;
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Error> [ConfigReader.ReadXML()] File IO Exception occurred: {0}", ex.Message));
			}
			catch( XmlException ex)
			{
				bResult = false;
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Error> [ConfigReader.ReadXML()] XML Exception occurred: {0}", ex.Message));
			}
			finally
			{             
				if( xmlReader != null)
					xmlReader.Close();
				CmDataCapture.Capture( CmDataCapture.MaskInfo, String.Format("<Info> [ConfigReader.ReadXML()] Finished Reading XMLFIle: {0}", sXMLFile));
			}
			return bResult;
		}
    
    
		public Hashtable GetExecutables()
		{     
			SetExecutableMain();
			return this.m_Executables;      
		}
    

		//Set property of executable to be main(IsMainExecutable=true)
		private void SetExecutableMain()
		{
			ICollection keys=this.m_Executables.Keys; 
      
			foreach(String key in keys)
			{
				if ( key.ToString().Equals( this.App.MainExecutable) || key.ToString().Equals( this.App.CompanionExecutable) )
				{     
					((Executable)this.m_Executables[key]).IsMainExecutable=true ;         
				}

			}
		}


		private void ProcessXMLElements( XmlTextReader xmlReader)
		{
			switch( xmlReader.Name)
			{       
				case "root":
					break;  // Added for the sake of data capture not logging         
				case "App":
					ProcessAppElement(xmlReader);
					break;
				case "Executable":          
					ProcessXMLExecElements( xmlReader);
					break;      
				case "ExecuteFunction":
					ProcessExecuteFunctionElements(xmlReader);
					break;      
				default:          
					CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format("<Warning> [ConfigReader.ProcessXMLElements] Unknown Start Element Name: {0}:{1}", xmlReader.Name, xmlReader.Value));
					break;
			}     
		}
    

		private void ProcessExecuteFunctionElements(XmlTextReader xmlReader)
		{
			if(m_ExecutableTemp == null)  //Invalid location of element , should be inner element of Executable
				return;

			String sName = xmlReader.GetAttribute("OnEvent");
			if( sName != null && !sName.Equals (""))
			{
            
				FunctionProcessor fnc = new FunctionProcessor(sName);                   
            
				for(int nIndex=0; nIndex<xmlReader.AttributeCount; nIndex++)
				{
					xmlReader.MoveToAttribute( nIndex);
					switch( xmlReader.Name)
					{
						case "OnEvent":                       
							break;
						case "ExecuteOn":
							if ( xmlReader.Value.ToLower().Equals("pre") || xmlReader.Value.ToLower().Equals("replaceactual") || xmlReader.Value.ToLower().Equals("post"))
							{
								fnc.ExecuteOn =xmlReader.Value; 
							}
							else //Invalid , disregard function
							{               
								CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("<Error> [ConfigReader.ProcessExecuteFunctionElements]  ExecuteOn value unknown:{0} ",xmlReader.Value));
								return;
							}               
							break;                              
						case "ExecutableName"://Recursion: error , cannot use own parent as executable
							if( !xmlReader.Value.Equals( m_ExecutableTemp.ExecutableName ))
								fnc.ExecuteableName =xmlReader.Value ;
							else
							{
								CmDataCapture.Capture( CmDataCapture.MaskError, String.Format ("<Error> [ConfigReader.ProcessExecuteFunctionElements] Recursion Error , cannot use own parent as executable:{0} ",xmlReader.Value));
								return;
							}               
							break;  
						case "Execute": 
							if ( xmlReader.Value.ToLower().Equals("start") || xmlReader.Value.ToLower().Equals("stop") || xmlReader.Value.ToLower().Equals("maximize") || xmlReader.Value.ToLower().Equals("minimize"))
								fnc.Execute =xmlReader.Value;
							else
							{
								CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format ("<Warning> [ConfigReader.ProcessExecuteFunctionElements] Execute value unknown :{0} using default value 'start'",xmlReader.Value));
								return;
							}
							break;

					}
				}
				this.m_ExecutableTemp.FunctionProcessor.Add(fnc);
			}
		}


		private void ProcessAppElement(XmlTextReader xmlReader)
		{
			for(int nIndex=0; nIndex<xmlReader.AttributeCount; nIndex++)
			{
				xmlReader.MoveToAttribute( nIndex);
				switch( xmlReader.Name)
				{
					case "MainExecutable":          
						this.m_app.MainExecutable  =xmlReader.Value ;
						break;
					case "MainContext":         
						this.m_app.MainContext  =xmlReader.Value ;
						break;           
					case "MainContextNotLoaded":          
						this.m_app.MainContextNotLoaded =xmlReader.Value ;
						break;      
					case "KeyLock":
						this.m_app.KeyLock  =xmlReader.Value ;
						break;                  
					case "LoadOnStartUp"://default :false
						if( xmlReader.Value.ToLower().Equals("true"))
							this.m_app.LoadOnStartUp   = true;                        
						break;
					case "UtilityButtonVisible"://default :false
						if( xmlReader.Value.ToLower().Equals("true"))
							this.m_app.UtilityButtonVisible=true;                        
						break;     
					case "InitialButtonState"://default :Primary
						if( xmlReader.Value.ToLower().Trim().Equals("primary") || xmlReader.Value.ToLower().Trim().Equals("secondary") )
							this.m_app.InitialButtonState   = xmlReader.Value.Trim();
						else
							CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format ("<Warning> [ConfigReader.ProcessAppElements] Unknown InitialButtonState value:{0}", xmlReader.Value));
						break;          
					case "LaunchPadSecurityOn"://default :false
						if( xmlReader.Value.ToLower().Equals("true"))
							this.m_app.LaunchPadSecurityOn  = true;                   
						break;              
					case "CompanionExecutable":
						this.m_app.CompanionExecutable =xmlReader.Value;
						break;
					case "CompanionContext":
						this.m_app.CompanionContext =xmlReader.Value;
						break;                    
					case "CompanionContextNotLoaded":
						this.m_app.CompanionContextNotLoaded =xmlReader.Value;
                        break;  
                    case "CoExecutable":
                        this.m_app.CoExecutable =xmlReader.Value;
                        break;
                    case "CoExecutableContext":
                        this.m_app.CoExecutableContext =xmlReader.Value;
                        break;                    
                    case "CoExecutableContextNotLoaded":
                        this.m_app.CoExecutableContextNotLoaded =xmlReader.Value;
                        break;  
					case "ForegroundWindowTitle":
						this.m_app.ForegroundWindowTitle =xmlReader.Value;
						break;     
					case "SecurityContext":
						this.m_app.SecurityContext =xmlReader.Value;
						break;          
					case "KillAppOnSwitch"://default :false
						if( xmlReader.Value.ToLower().Equals("true"))
							this.m_app.KillAppOnSwitch  = true;                   
						break;
                    case "MonitorCoExecutable"://default :false
                        if (xmlReader.Value.ToLower().Equals("true"))
                            this.m_app.MonitorCoExecutable = true;
                        break;
                    case "MonitorCompanionExecutable"://default :false
                        if (xmlReader.Value.ToLower().Equals("true"))
                            this.m_app.MonitorCompanionExecutable = true;
                        break;
					default:
						CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format ("<Warning> [ConfigReader.ProcessAppElements] Unknown Attribute Name: {0}:{1}", xmlReader.Name, xmlReader.Value));
						break;
				}
			}
      
		}

		private void ProcessXMLEndElements( XmlTextReader xmlReader)
		{
			switch( xmlReader.Name)
			{
        
				case "root":          
					break;// Added for the sake of data capture not logging     
				case "App":
					break;
				case "Executable":
					if( m_ExecutableTemp != null)
					{                     
						this.m_Executables.Add( m_ExecutableTemp.ExecutableName, m_ExecutableTemp);           
						m_ExecutableTemp = null;
					}
					break;    
				case "ExecuteFunction":
					break;
				default:
					CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format ("<Warning> [ConfigReader.ProcessXMLEndElements] Unknown End Element Name: {0}:{1}", xmlReader.Name, xmlReader.Value));
					break;
			}
      
		}

		private void ProcessXMLExecElements( XmlTextReader xmlReader)
		{
			if(m_ExecutableTemp == null) //if start of element; valid
			{
				String sName = xmlReader.GetAttribute("ExecutableName");
				if( sName != null && !sName.Equals (""))
				{
          
					Executable execTemp = new Executable(sName);                    
          
					for(int nIndex=0; nIndex<xmlReader.AttributeCount; nIndex++)
					{
						xmlReader.MoveToAttribute( nIndex);
						switch( xmlReader.Name)
						{
							case "ExecutableName" :
								// Do not process (Attribute was already used above)
								// Added for the sake of data capture not logging 
								break;
							case "WindowTitle":               
								execTemp.WindowTitle =xmlReader.Value;
								break;
							case "Filename":
								execTemp.Filename = xmlReader.Value;
								break;
							case "Arguments":
								execTemp.Arguments = xmlReader.Value;
								break;
							case "Path":
							{
								try
								{
									String sPath = ConfigData.ExpandPartitionVars(xmlReader.Value);

									//for review                  
									//if( sPath == "")
									//  sPath = SBIN_PATH;
                  
#if !WINCE // JZB_In
                  if ( -1 != sPath.IndexOf("{system}"))
                  {
                    sPath = sPath.Replace("{system}", Environment.SystemDirectory);
                  }
                  else if ( -1 != sPath.IndexOf("{windows}"))
#else
									if ( -1 != sPath.IndexOf("{windows}"))
#endif
									{
										StringBuilder sbWindowsPath = new StringBuilder(256);
										PInvoke.GetWindowsDirectory( sbWindowsPath, 256);
										sPath = sPath.Replace("{windows}", sbWindowsPath.ToString());
									}
									if(!sPath.EndsWith("\\") && !sPath.Equals(""))
										sPath = sPath + "\\";
									execTemp.Path = sPath;
								}
								catch( Exception ex)
								{
									CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Error> [ConfigReader.ProcessXMLExecMainElements()] Exception: {0}", ex.Message));
								}
								break;
							}
                
							case "Maximize": //default :true
								if( xmlReader.Value.ToLower().Equals("false"))
									execTemp.Maximize  = false;               
								break;

							case "MinimizeStart": //default :false
								if( xmlReader.Value.ToLower().Equals("true"))
									execTemp.MinimizeStart = true;
                                break;    
                                           
                            case "WaitForExit":		//Wait for Exit of this application prior to continuing...
                                if (xmlReader.Value.ToLower().Equals( "true" ))
                                    execTemp.WaitForExit = true;
                                break;

                            case "WaitForWindow":     //Wait for this application window to load prior to continuing...
                                if (xmlReader.Value.ToLower().Equals( "true" ))
                                    execTemp.WaitForWindow = true;
                                break; 
                             
							case "TopMost":		//If true, the executable window style be HWND_TOPMOST (if false HWND_TOP)
								if (xmlReader.Value.ToLower().Equals( "false" ))
									execTemp.TopMost = false;
								break;

							case "ExecuteOnlyOnce":		//Execute only one time (e.g. ADD.BAT)
								if (xmlReader.Value.ToLower().Equals( "true" ))
									execTemp.ExecuteOnlyOnce = true;
								break;

							case "StopWindow":
								execTemp.StopWindow = xmlReader.Value;
								break;

							case "StopUserMessage":
								execTemp.StopUserMessage = Convert.ToUInt32(xmlReader.Value);
								break;

                            case "NoRestartIfAlive":		//Don't restart executable if it's still alive...
                                if (xmlReader.Value.ToLower().Equals("true"))
                                    execTemp.NoRestartIfAlive = true;
                                break;

							default:
								CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format("<Warning> [ConfigReader.ProcessXMLExecMainElements()] Unknown ExecMain Attribute:{0} Value:{1}", xmlReader.Name, xmlReader.Value));
								break;
						}
					}
					m_ExecutableTemp = execTemp;
				}
				else
					CmDataCapture.Capture( CmDataCapture.MaskError, "<Error> [ConfigReader.ProcessXMLExecMainElements()] Error:  Required ExecMain ExecutableName Attribute Missing");
			}
			else
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "<Error> [ConfigReader.ProcessXMLExecMainElements()] m_ExecutableTemp was not null when it should have been.");
			}
      
		}
    
		//Check Main and Companion Executables if valid
		private void ValidateApp()
		{
			bool bValid=false;
			ICollection keys=this.m_Executables.Keys;       

			if ( this.m_app.MainExecutable!=null &&  !this.m_app.MainExecutable.Equals("") )
			{
				foreach(String key in keys)
				{
					if ( key.ToString().Equals( this.m_app.MainExecutable))
					{     
						bValid=true;
						break;
					}
				}       
			}
      
			if (bValid==false)
			{
				this.m_app.MainExecutable="" ;
			}
      
			bValid=false;
			if ( this.m_app.CompanionExecutable !=null &&  !this.m_app.CompanionExecutable.Equals("") )
			{
				foreach(String key in keys)
				{
					if ( key.ToString().Equals( this.m_app.CompanionExecutable ))
					{     
						bValid=true;
						break;
					}
				}       
			}
			if (bValid==false)
			{
				this.m_app.CompanionExecutable="" ;
			}
		}
    
		public App App
		{
			get
			{             
				ValidateApp();    
				return this.m_app;
			}
		}

  
	} // end of class
  
}
