using System;
using System.ComponentModel;
using System.Diagnostics;
using System.Threading;
using System.Text ;
using System.Collections;
using RPSWNET;
using CommonLibsNet;

namespace LaunchPadNet
{
	/// <summary>
	/// Summary description for Executable.
	/// </summary>
	public class Executable 
	{
		/// <summary>
		/// Executable constructor.
		/// </summary>
		public Executable( String sExecutableName)
		{     
			executableName = sExecutableName;
		}
  
		/// <summary>
		/// Get the executable name.
		/// </summary>
		public String ExecutableName
		{
			get
			{
				return executableName;
			}
		}
    
		/// <summary>
		/// Get/Set the executable window title string - use for traversing windows.
		/// </summary>
		public String WindowTitle
		{
			get
			{
				return windowTitle;    
			}
			set
			{
				windowTitle = value;
			}
		}

		/// <summary>
		/// Get/Set the maximize flag.
		/// </summary>
		public bool Maximize
		{
			get
			{
				return maximize;
			}
			set
			{
				maximize = value;
			}
		}
		
		/// <summary>
		/// Get/Set the minimize at start indicator. Used to get rid of DOS boxes and "ScotInputWindow".
		/// </summary>
		public bool MinimizeStart
		{
			get
			{
				return minimizeStart;
			}
			set
			{
				minimizeStart = value;
			}
		}

		/// <summary>
		/// Get/Set the main executable filename.
		/// </summary>
		public String Filename
		{
			get
			{
				return filename;
			}
			set
			{
				filename = value;
			}
		}

		/// <summary>
		/// Get/Set the main executable path.
		/// </summary>
		public String Path
		{
			get
			{
				return path;
			}
			set
			{
				path = value;
			}
		}

		/// <summary>
		/// Get/Set the executable arguments.
		/// </summary>
		public String Arguments
		{
			get
			{
				return arguments; 
			}
			set
			{
				arguments = value;
			}
		}
		
		/// <summary>
		/// Get/Set the windows window that is sent the stop message number.
		/// </summary>
		public String StopWindow
		{
			get
			{
				return stopWindow; 
			}
			set
			{
				stopWindow = value;
			}
		}

		/// <summary>
		/// Get/Set the windows message number is sent to the stop message window.
		/// </summary>
		public uint StopUserMessage
		{
			get
			{
				return stopUserMessage; 
			}
			set
			{
				stopUserMessage = value;
			}
		}

		/// <summary>
		/// Return the executable name.
		/// </summary>
		public override String ToString ()
		{
			return  ExecutableName ;  
		}

		/// <summary>
		/// Get/Set the main executable indicator.
		/// </summary>
		public bool IsMainExecutable
		{
			get
			{
				return isMainExecutable ;
			}
			set
			{
				isMainExecutable = value;
			}
		}   

		/// <summary>
		/// Get/Set the windows Process.
		/// </summary>
		public Process LocalProcess
		{
			get
			{
				return localProcess;
			}
			set
			{
				localProcess = value;
			}
		}

		/// <summary>
		/// Get/Set the FunctionProcess array.
		/// </summary>
		public ArrayList FunctionProcessor
		{
			get
			{
				return m_functionProcessor;
			}
			set
			{
				m_functionProcessor = value;
			}
		}

		/// <summary>
		/// Callback-Loop to check if executable is minimized. If so, restore to normal size.
		/// </summary>
		private void CheckMinimize()
		{
			while(LocalProcess != null && !LocalProcess.HasExited )
			{         
				try
				{
					PInvoke.EnumWindowsCallBack EnumWindowsFunc = new PInvoke.EnumWindowsCallBack(RestoreToNormalIfMinimize );
					PlatformEnumWindows(EnumWindowsFunc, 0 );              
				}
				catch ( Exception e )
				{
					CmDataCapture.Capture( CmDataCapture.MaskError, 
											String.Format("<Exception> [RestoreToNormalIfMinimize()] new Delegate: {0}",  e.Message ));                
				}
				Thread.Sleep(500);      
			}     
		}

		/// <summary>
		/// Does the executable have a window.
		/// </summary>
		private bool IsAWindow()
		{
			if (WindowTitle != null && !WindowTitle.Trim().Equals("") )
				return true;
			else
				return false;
		}

		/// <summary>
		/// Start the executable callback.
		/// </summary>
		private bool Start()
		{
			bool retVal = true;   
      
			// Some executable files (such as ADD.BAT) should only run once even if starting/stopping
			// the lane software. Thus, if this executable file should only run once and it has already
			// been run, do not run it again. Return a successful flag indicating things have been
			// processed as desired.
			if ( ExecuteOnlyOnce && HasBeenExecuted )
			{
				return retVal;
			}
			else
			{
				// Now we have executed this application.
				HasBeenExecuted = true;
			}

			Stop();
      
			int  processID = 0;
			Process[] foundProcesses = null;    
      
			//No need to find Windowless executables
			try
			{
				if ( IsAWindow())
				{
					//Get Possible Window Titles separated by commas from the config file
					String []windowTitles = WindowTitle.Split(',');

					for ( int count = 0;count<windowTitles.Length ;count++)
					{ 
						IntPtr hWnd = PInvoke.FindWindow(null,windowTitles[count]);
            
						if( hWnd != IntPtr.Zero)
						{             
#if	( !WINCE )
							WindowTitle = windowTitles[count];
#endif 
							PInvoke.GetWindowThreadProcessId( hWnd, out processID);                 
							//get First process, and use ProcessName property to get all Windows running
							Process firstProcess = Process.GetProcessById(processID);
							LocalProcess = firstProcess;
                      
							if ( firstProcess != null)
							{           
								foundProcesses = Process.GetProcessesByName(firstProcess.ProcessName);          
							}
							break;
						}
					}
					//close all open windows
					if( foundProcesses != null)
					{
						foreach(Process nextProcess in foundProcesses)
						{
							nextProcess.CloseMainWindow();        
						}
					}
				} 
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Exception> [Executable.startProcess()] Error finding processes: {0}",  ex.Message ));                
			}
        
			try
			{       
        
				//Since there might be some arguments included, just start a new process 
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("[Executable.Start()] - Starting {0}",  Filename ));                

				ProcessStartInfo startInfo = new ProcessStartInfo(Path + Filename);
				if( Arguments != null && !Arguments .Equals("") )
					startInfo.Arguments = Arguments;
				
				// If the config file option to start minimized is true, then set the option before starting.
				if (MinimizeStart)
				{
					startInfo.WindowStyle = ProcessWindowStyle.Minimized; //Fix Tar#327968
					//startInfo.WindowStyle = ProcessWindowStyle.Hidden;
				}

				Process process = new Process();
				process.StartInfo = startInfo;
        
				process.Start();
				LocalProcess  = process;
          
				if (IsAWindow())
				{
					try
					{
						process.WaitForInputIdle();                                 
						Thread.Sleep(1000);
						if (process.HasExited)
						{
							retVal = false;
							CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("Executable::Start() [Executable.startProcess()] Error starting process: {0}", Path + Filename ));               
						}
					}
					catch( Exception ex)
					{
						// This happens sometimes with batch files. It creates the following exception
						// "WaitForInputIdle failed.  This could be because the process does not have a graphical interface."
						// Add this try/catch to miminize the side effects.
						CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Exception> [Executable.startProcess()] Error WAITING for process: {0} - {1}", Path + Filename, ex.Message ));               
					}

#if	( WINCE )
#else
					if ( retVal && IsMainExecutable == false ) //allow minimize for main executable
					{         
						Thread tMinimize = new Thread( new ThreadStart(CheckMinimize));
						tMinimize.Start();
					}       
#endif
				} 

				// Let executable complete before continuing...
				if ( retVal && WaitForExit )
				{
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, String.Format("Let executable complete before continuing..." + ExecutableName ));                
					if ( LocalProcess != null && !LocalProcess.HasExited )
					{
						LocalProcess.WaitForExit();
					}
				}
			}
			catch( Exception ex)
			{         
				retVal = false;
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Exception> [Executable.startProcess()] Error starting process: {0} - {1}", Path + Filename, ex.Message ));               
			}
			return retVal;      
		}

		/// <summary>
		/// Stop the executable.
		/// </summary>
		private void Stop()
		{
			try
			{
				if ( LocalProcess != null )
				{
					if (LocalProcess.HasExited == false )
					{
						int elapsedMS = 0;		// Elapsed MilliSeconds to wait for a gentle shutdown...
						ConfigDataLaunchPad configData = ConfigDataLaunchPad.GetConfigData();

						if	( ( this.IsMainExecutable )
								&& ( this.Filename.ToLower().IndexOf( "rapnet" ) >= 0 ) )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "Sending alternative Stop to RapNet via RemoteMessage" );
							MyApplication.SendProcessMessage( ConfigDataLaunchPad.rapApplication, "operation=stop-app;" );
							if	( configData.ShutDownTimeout > 15000 )
								elapsedMS = configData.ShutDownTimeout - 15000;
						}

						// Request the process to close in a civil manner...
						//LocalProcess.CloseMainWindow();
						UIntPtr wParam = UIntPtr.Zero;
						IntPtr lParam = IntPtr.Zero;
						IntPtr hWnd = IntPtr.Zero;
						uint message = PInvoke.WM_CLOSE;

						// If the config file overrides the process window title, use the user defined window.
						if (!StopWindow.Equals(""))
						{
							hWnd = PInvoke.FindWindow(null,StopWindow);
						}
						else
						{
							hWnd = LocalProcess.MainWindowHandle;
						}

						// If the config file overrides WM_CLOSE, use the User defined message.
						if ( StopUserMessage != 0)
						{
							message = PInvoke.WM_USER + stopUserMessage;
						}

						//if ( PInvoke.PostMessage( LocalProcess.MainWindowHandle, PInvoke.WM_USER + 101, wParam, lParam) )
						if ( PInvoke.PostMessage(hWnd, message, wParam, lParam) )
						{
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "Stop() GENTLE: " + LocalProcess.ProcessName);
							while ( !LocalProcess.HasExited && elapsedMS < configData.ShutDownTimeout)	// 15000 ms is 15 seconds
							{
								Thread.Sleep(100);
								elapsedMS += 100;
							}
						}

						// No more Mr. Nice guy! Shut em down...
						if ( !LocalProcess.HasExited )
						{
							LocalProcess.Kill();
							CmDataCapture.Capture( CmDataCapture.MaskInfo, "Stop() HARD: LocalProcess.Kill(): " + LocalProcess.ProcessName);
						}
					}
				}
			}
			catch(Exception ex)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("<Exception> [Executable.Stop()] Exception stopping processes: {0}",  ex.Message ));                
			}
			LocalProcess = null; 
      
		}

		/// <summary>
		/// Maximize the executable callback.
		/// </summary>
		private bool MaximizeWindows(IntPtr hWnd, int iProcessIndex) 
		{     
			if ( LocalProcess == null || LocalProcess.HasExited || !IsAWindow())
			{         
				return false; //stop enumeration
			}   
  
			bool bNotFound = true;
			int processID = -1;

			PInvoke.GetWindowThreadProcessId( hWnd, out processID);

			if( processID == LocalProcess.Id )
			{   
				StringBuilder sbProcessText = new StringBuilder(256);

				PInvoke.GetWindowText( hWnd, sbProcessText, sbProcessText.Capacity);
        
				String []windowTitles = WindowTitle.Split(',');   

				for ( int count = 0;count<windowTitles.Length ;count++)
				{ 
					if (sbProcessText.ToString().EndsWith(windowTitles[count]) )
					{
#if	( !WINCE )
						WindowTitle = windowTitles[count];
#endif 

						IntPtr hParentWnd = IntPtr.Zero;

						// In the scotapp assist mode, sometimes the POS solution window is displayed on top
						// of the regular scotapp window. If that happens, and the keylock button is pressed
						// the customer will want to return to the POS window on the next button press. Save
						// the handle of that window so that the window can be redisplayed on the next button press.
						// After the button is pressed, restore the window pointer to zero.
						if (ForegroundWindowHandle != IntPtr.Zero)
						{
							hParentWnd = ForegroundWindowHandle;
						}
						else
						{
							hParentWnd = PInvoke.GetParent( hWnd );
						}

						if( hParentWnd == IntPtr.Zero)
							hParentWnd = hWnd;
	                    
						PInvoke.WINDOWPLACEMENT winPlace = new PInvoke.WINDOWPLACEMENT();
						winPlace.length = 44;               

						bool bResult = false;

#if	( !WINCE )
						bResult = PInvoke.GetWindowPlacement( hParentWnd, out winPlace);                        
#else
						// CE doesn't support GetWindowPlacement
						bResult = true;

						// We're not using these, just stop compile warnings 
						PInvoke.POINT point = new PInvoke.POINT();
						PInvoke.RECT  rect  = new PInvoke.RECT();
						winPlace.flags = 0;
						winPlace.ptMinPosition = point; 
						winPlace.ptMaxPosition = point; 
						winPlace.rcNormalPosition = rect; 
						// Default value 
						winPlace.showCmd = ( uint )PInvoke.ShowWindowFlags.SW_MINIMIZE;
#endif

						if((bResult) && (winPlace.showCmd != (uint)PInvoke.ShowWindowFlags.SW_SHOWMAXIMIZED ) )
						{         
							CmDataCapture.Capture( CmDataCapture.MaskError, String.Format("[Executable.Maximize()] Window[{0}] placement matched", 
								sbProcessText.ToString() ));      
							int nFlag;
							if( Maximize )
							{
								nFlag = (int) PInvoke.ShowWindowFlags.SW_SHOWMAXIMIZED;              
							}
							else
							{ 
								//restore normal window even if minimized
								nFlag = (int) PInvoke.ShowWindowFlags.SW_SHOWNORMAL;
							}
							PInvoke.ShowWindow(hParentWnd, nFlag);                                            
						}   

						IntPtr hWndInsertAfter = IntPtr.Zero;
						// If true, the executable window style be HWND_TOPMOST (if false HWND_TOP)
						hWndInsertAfter = (TopMost ? ((IntPtr) PInvoke.SetWindowPosInsertAfter.HWND_TOPMOST) :((IntPtr) PInvoke.SetWindowPosInsertAfter.HWND_TOP));
        
						PInvoke.SetForegroundWindow(hParentWnd);

						// In the scotapp assist mode, sometimes the POS solution window is displayed on top
						// of the regular scotapp window. If that happens, and the keylock button is pressed
						// the customer will want to return to the POS window on the next button press. Save
						// the handle of that window so that the window can be redisplayed on the next button press.
						// After the button is pressed, restore the window pointer to zero.
						if (ForegroundWindowHandle != IntPtr.Zero)
						{
							int nFlag= (int) PInvoke.ShowWindowFlags.SW_SHOW;	//SW_SHOWNORMAL;	// SW_SHOWNA;
							PInvoke.ShowWindow( ForegroundWindowHandle, nFlag);
							CmDataCapture.Capture( CmDataCapture.MaskError, "Executable.MaximizeWindows(): ForegroundWindowTitle - ForegroundWindowHandle CLEAR=\'0\'");
							ForegroundWindowHandle = IntPtr.Zero;
							break;
						}

#if ( !WINCE )
            			PInvoke.SetWindowPos(hParentWnd,hWndInsertAfter,0,0,0,0,(uint)(PInvoke.SetWindowPosFlags.SWP_NOMOVE|PInvoke.SetWindowPosFlags.SWP_NOSIZE|PInvoke.SetWindowPosFlags.SWP_SHOWWINDOW));                        
#else
						int windowWidth = 240;
						int windowHeight = 320;
						int newXPosition = 0;
						int newYPosition = 0;
						PInvoke.SetWindowPos(hParentWnd,hWndInsertAfter,newXPosition,newYPosition,windowWidth,windowHeight,(uint)(PInvoke.SetWindowPosFlags.SWP_SHOWWINDOW));
#endif
            
						PInvoke.SetFocus(hParentWnd);
						bNotFound = false;
						break;            
					}
				}
			}
			return bNotFound;
		}

		/// <summary>
		/// Minimize the executable callback.
		/// </summary>
		private bool MinimizeWindows(IntPtr hWnd, int iProcessIndex) 
		{     
			if ( LocalProcess == null || LocalProcess.HasExited || !IsAWindow())
			{ 
				return false; //stop enumeration
			}   
      
			bool bNotFound = true;
			int processID = -1;
          
			PInvoke.GetWindowThreadProcessId( hWnd, out processID);
      
			if( processID == LocalProcess.Id )
			{   
            
				StringBuilder sbProcessText = new StringBuilder(256);
				PInvoke.GetWindowText( hWnd, sbProcessText, sbProcessText.Capacity);
            
				String []windowTitles = WindowTitle.Split(',');   
				for ( int count = 0;count<windowTitles.Length ;count++)
				{ 
					if (sbProcessText.ToString().EndsWith(windowTitles[count]) )
					{
#if	( !WINCE )
						WindowTitle = windowTitles[count];
#endif 
						IntPtr hParentWnd = PInvoke.GetParent( hWnd);
						if( hParentWnd == IntPtr.Zero)
							hParentWnd = hWnd;
          
                    
						PInvoke.WINDOWPLACEMENT winPlace = new PInvoke.WINDOWPLACEMENT();
						winPlace.length = 44;

						bool bResult = false;

#if ( !WINCE )
         				bResult = PInvoke.GetWindowPlacement( hParentWnd, out winPlace);                        
#else
						// CE doesn't support GetWindowPlacement
						bResult = true;
						winPlace.showCmd = ( uint ) PInvoke.ShowWindowFlags.SW_NORMAL;
#endif

                        
						if(bResult && (winPlace.showCmd != (uint)PInvoke.ShowWindowFlags.SW_SHOWMINIMIZED))
						{
							//int nFlag = (int) PInvoke.ShowWindowFlags.SW_MINIMIZE; 
							int nFlag = (int) PInvoke.ShowWindowFlags.SW_HIDE; 
							PInvoke.ShowWindow( hParentWnd, nFlag);
              
						}   
                    
						bNotFound = false;
						break;
					}
				}
			}
			return bNotFound;
      
		}
    
		/// <summary>
		/// Return window to normal.
		/// </summary>
		private bool RestoreToNormalIfMinimize(IntPtr hWnd, int iProcessIndex) 
		{     
			if ( LocalProcess == null || LocalProcess.HasExited || !IsAWindow())
			{ 
				return false; //stop enumeration
			}   
      
			bool bNotFound = true;
			int processID = -1;
          
			PInvoke.GetWindowThreadProcessId( hWnd, out processID);
      
			if( processID == LocalProcess.Id )
			{   
            
				StringBuilder sbProcessText = new StringBuilder(256);
				PInvoke.GetWindowText( hWnd, sbProcessText, sbProcessText.Capacity);
            
				String []windowTitles = WindowTitle.Split(',');   
				for ( int count = 0;count<windowTitles.Length ;count++)
				{ 
					if (sbProcessText.ToString().EndsWith(windowTitles[count]) )
					{
#if	( !WINCE )
						WindowTitle = windowTitles[count];
#endif 
						IntPtr hParentWnd = PInvoke.GetParent( hWnd);
						if( hParentWnd == IntPtr.Zero)
							hParentWnd = hWnd;
                    
						PInvoke.WINDOWPLACEMENT winPlace = new PInvoke.WINDOWPLACEMENT();
						winPlace.length = 44;
                                  
						bool bResult = false;

#if ( !WINCE ) 
         				bResult = PInvoke.GetWindowPlacement( hParentWnd, out winPlace);                        
#else
						// CE doesn't support GetWindowPlacement
						bResult = true;
						winPlace.showCmd = ( int ) PInvoke.ShowWindowFlags.SW_NORMAL;
#endif

						if(bResult && (winPlace.showCmd == (uint)PInvoke.ShowWindowFlags.SW_SHOWMINIMIZED))
						{       
							int nFlag= (int) PInvoke.ShowWindowFlags.SW_RESTORE;             
							PInvoke.ShowWindow( hParentWnd, nFlag);       
              
						}
						if(bResult && (winPlace.showCmd == (uint)PInvoke.ShowWindowFlags.SW_NORMAL && Maximize ))
						{       
							int nFlag= (int) PInvoke.ShowWindowFlags.SW_MAXIMIZE;            
							PInvoke.ShowWindow( hParentWnd, nFlag);
						}
            
						bNotFound = false;
						break;
					}
				}
			}
			return bNotFound;
      
		}
        
		/// <summary>
		/// Minimize the executable.
		/// </summary>
		public void MinimizeWindow()
		{
			try
			{
				PInvoke.EnumWindowsCallBack EnumWindowsFunc = new PInvoke.EnumWindowsCallBack(MinimizeWindows);
				PlatformEnumWindows(EnumWindowsFunc, 0 );
			}
			catch ( Exception e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, 
										String.Format("<Exception> [MinimizeWindow()] new Delegate: {0}",  e.Message ));                
			}

		}

		
		/// <summary>
		/// Maximize the executable.
		/// </summary>
		public void MaximizeWindow()
		{
			try
			{
				PInvoke.EnumWindowsCallBack EnumWindowsFunc = new PInvoke.EnumWindowsCallBack(MaximizeWindows);
				PlatformEnumWindows(EnumWindowsFunc, 0 );  
			}
			catch ( Exception e )
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, 
					String.Format("<Exception> [MaximizeWindow()] new Delegate: {0}",  e.Message ));                
			}
		}

		/// <summary>
		/// Platform Neutral EnumWindows function
		/// </summary>
		/// <param name="EnumWindowsFunc">delegate to call back</param>
		/// <param name="valInt">Value to pass to delegate</param>
		private void PlatformEnumWindows( PInvoke.EnumWindowsCallBack EnumWindowsFunc, int valInt )
		{
#if	( !WINCE ) 
			PInvoke.EnumWindows( EnumWindowsFunc, valInt);  
#else
			EnumCEWindows( EnumWindowsFunc, valInt );
#endif
		}


		/// <summary>
		/// EnumCEWindows - Do an EnumWindows for CE.NET. 
		/// </summary>
		/// <param name="EnumWindowsFunc">CallBack function</param>
		/// <param name="valInt">Value to pass to callbacks in LParam</param>
		private void EnumCEWindows( PInvoke.EnumWindowsCallBack EnumWindowsFunc, int valInt )
		{
			// unsafe
			{
				IntPtr [] Handles = new IntPtr [ 2048 ];
				// fixed ( IntPtr * pHandles = Handles )
				{
					int	   iHandleCount = 2048;
					try
					{
						PInvoke.GetWindowHandles( Handles, ref iHandleCount );
					}
					catch ( Exception e )
					{
						CmDataCapture.Capture( CmDataCapture.MaskError, 
							String.Format( "<Exception> [PInvoke::EnumCEWindows - GetWindowHandles()] [{0}]", 
							e.Message ));
					}

					//CmDataCapture.Capture( CmDataCapture.MaskError, 
					//	String.Format( "GetWindowHandles found {0} handles", iHandleCount ));


					int i = 0;
					for( i = 0; i < iHandleCount; i++ )
					{
						try
						{
							bool bCallResult = true;
							bCallResult = EnumWindowsFunc( Handles[ i ], valInt );

							if	( bCallResult == false )
							{
								return;
							}
						}
						catch ( Exception e )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, 
								String.Format( "<Exception> [PInvoke::EnumCEWindows - Callback] [{0}] [{1}]", 
								e.Message, EnumWindowsFunc.GetType().Name.ToString() ));
						}
					}
				}
			}
		}

  
		/// <summary>
		/// Execute the task.
		/// </summary>
		public bool Execute(String function)
		{
			bool retVal=true;
			switch(function.ToLower())
			{
				case "minimize":
					if ( LocalProcess !=null) 
					{
						try
						{
							PInvoke.EnumWindowsCallBack EnumWindowsFunc = new PInvoke.EnumWindowsCallBack(MinimizeWindows);
							PlatformEnumWindows(EnumWindowsFunc, 0 );                            
						}
						catch (Exception e )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, 
								String.Format("<Exception> [MinimizeWindow2()] new Delegate: {0}",  e.Message ));                
						}
					}
					else
					{
						CmDataCapture.Capture( CmDataCapture.MaskError,"<Exception> [ExecutableMain.Execute()] Cannot Minimize null process.");
						retVal = false;
					}
					break;
        
				case "maximize": 
					if ( LocalProcess != null) 
					{
						try
						{
							PInvoke.EnumWindowsCallBack EnumWindowsFunc = new PInvoke.EnumWindowsCallBack(MaximizeWindows);
							PlatformEnumWindows(EnumWindowsFunc, 0 );                            
						}
						catch ( Exception e )
						{
							CmDataCapture.Capture( CmDataCapture.MaskError, 
								String.Format("<Exception> [MaximizeWindow2()] new Delegate: {0}",  e.Message ));                
						}

					}
					else
					{
						CmDataCapture.Capture( CmDataCapture.MaskError,"<Exception> [ExecutableMain.Execute()] Cannot Maximize null process.");
						retVal = false;
					}
					break;
        
				case "stop":
					Stop();
					break;
        
				case "start":         
					retVal = Start();
					break;
        
				default:
					CmDataCapture.Capture( CmDataCapture.MaskError,"<Exception> [ExecutableMain.Execute()] Unknown function to execute.");
					break;
			}
			return retVal;
		}
  
		/// <summary>
		/// Execute only one time (e.g. ADD.BAT)
		/// </summary>
		public bool ExecuteOnlyOnce
		{			
			get
			{
				return executeOnlyOnce;
			}			
			set
			{
				executeOnlyOnce = value;
			}
		}

		/// <summary>
		/// Used in conjunction with "ExecuteOnlyOnce". If the executable file has already run,
		/// set this boolean value to true.
		/// </summary>
		public bool HasBeenExecuted
		{			
			get
			{
				return hasBeenExecuted;
			}			
			set
			{
				hasBeenExecuted = value;
			}
		}

		/// <summary>
		/// Save the handle of the foreground window.
		/// </summary>
		public IntPtr ForegroundWindowHandle
		{			
			get
			{
				return foregroundWindowHandle;
			}			
			set
			{
				foregroundWindowHandle = value;
			}
		}

		/// <summary>
		/// Get/Set for topMost. If true, the executable window style be HWND_TOPMOST (if false HWND_TOP)
		/// </summary>
		public bool TopMost
		{			
			get
			{
				return topMost;
			}			
			set
			{
				topMost = value;
			}
		}

		/// <summary>
		/// Inidcates if LaunchPad should wait til this executable is complete before continuing (used for ADD.BAT)
		/// </summary>
		public bool WaitForExit
		{			
			get
			{
				return waitForExit;
			}			
			set
			{
				waitForExit = value;
			}
		}

		/// <summary>
		///	Executable name
		/// </summary>
		private String executableName;
		
		/// <summary>
		///	executable window title string - use for traversing windows
		/// </summary>
		private String windowTitle;
		
		/// <summary>
		///	Main executable filename
		/// </summary>
		private String filename = "";
		
		/// <summary>
		/// Main executable path
		/// </summary>
		private String path = ""; 
      		
		/// <summary>
		/// Executable arguments
		/// </summary>
		private String arguments;

		/// <summary>
		/// Executable override of the STOP Window (if empty, use the MainWindowTitle from the Process class)
		/// </summary>
		private String stopWindow = "";

		/// <summary>
		/// Executable override of the STOP User Message. (default to 0). If overridden WM_USER + override value is sent.
		///		If not overridden, WM_CLOSE is sent.
		/// </summary>
		private uint stopUserMessage = 0;

		/// <summary>
		/// Maximize flag
		/// </summary>
		private bool maximize = true;

		/// <summary>
		/// Minimize at start indicator. Used to get rid of DOS boxes and "ScotInputWindow".
		/// </summary>
		private bool minimizeStart = false; 

		/// <summary>
		/// Main executable indicator
		/// </summary>
		private bool isMainExecutable = false;

		/// <summary>
		/// Windows process
		/// </summary>
		private Process localProcess = null;

		/// <summary>
		/// FunctionProcess array
		/// </summary>
		private ArrayList m_functionProcessor = new ArrayList();

		/// <summary>
		/// Let executable complete before continuing...
		/// </summary>
		private bool waitForExit = false;

		/// <summary>
		/// If true, the executable window style be HWND_TOPMOST (if false HWND_TOP)
		/// </summary>
		private bool topMost = true;

		/// <summary>
		/// Execute only one time (e.g. ADD.BAT)
		/// </summary>
		private bool executeOnlyOnce = false;

		/// <summary>
		/// Used in conjunction with "ExecuteOnlyOnce". If the executable file has already run,
		/// set this boolean value to true.
		/// </summary>
		private bool hasBeenExecuted = false;

		/// <summary>
		/// Save the handle of the foreground window.
		/// </summary>
		private IntPtr foregroundWindowHandle = IntPtr.Zero;
	}//end class

}

