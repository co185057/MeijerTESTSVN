// PSAPI.h: interface for the PSAPI library.
//
// This class includes many process and module helper functions.  
// This class has evolved from its original implementation by
// Lee, JaeKil 1998-1999 (opensource) and fully supports all 
// features in both NT4 and up.  It is important that
// any new functionality also adheres to this requirement. 
// - SteveA (SA185011)
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CPSAPI.h"

namespace PSAPI
{
   namespace
   {
      typedef BOOL (WINAPI *GETMEMINFO)(
         HANDLE hProcess, 
         PPROCESS_MEMORY_COUNTERS ppsmemCounters,
         DWORD cb
         );

#ifdef	UNICODE
#define	GETMODULEFILENAME	GETMODULEFILENAMEW
#else
#define	GETMODULEFILENAME	GETMODULEFILENAMEA
#endif

      typedef BOOL (WINAPI *ENUMPROCESSES)(
         DWORD * lpidProcess,  // array to receive the process identifiers
         DWORD cb,             // size of the array
         DWORD * cbNeeded      // receives the number of bytes returned
         );

      typedef BOOL (WINAPI *ENUMPROCESSMODULES)(
         HANDLE hProcess,      // handle to the process
         HMODULE * lphModule,  // array to receive the module handles
         DWORD cb,             // size of the array
         LPDWORD lpcbNeeded    // receives the number of bytes returned
         );

      typedef DWORD (WINAPI *GETMODULEFILENAMEA)( 
         HANDLE hProcess,		// handle to the process
         HMODULE hModule,		// handle to the module
         LPSTR lpstrFileName,	// array to receive filename
         DWORD nSize			// size of filename array.
         );

      typedef DWORD (WINAPI *GETMODULEFILENAMEW)( 
         HANDLE hProcess,		// handle to the process
         HMODULE hModule,		// handle to the module
         LPWSTR lpstrFileName,	// array to receive filename
         DWORD nSize			// size of filename array.
         );

      typedef INT (WINAPI *VDMENUMTASKWOWEX)(
         DWORD dwProcessId,	// ID of NTVDM process
         TASKENUMPROCEX fp,	// address of our callback function
         LPARAM lparam		// anything we want to pass to the callback function.
         );

      //Windows NT Functions

      typedef DWORD (WINAPI *GETMODULEBASENAMEA)( 
         HANDLE hProcess,		// handle to the process
         HMODULE hModule,		// handle to the module
         LPTSTR lpstrFileName,	// array to receive base name of module
         DWORD nSize			// size of module name array.
         );

      typedef DWORD (WINAPI *GETMODULEBASENAMEW)( 
         HANDLE hProcess,		// handle to the process
         HMODULE hModule,		// handle to the module
         LPTSTR lpstrFileName,	// array to receive base name of module
         DWORD nSize			// size of module name array.
         );

#ifdef	UNICODE
#define	GETMODULEBASENAME	GETMODULEBASENAMEW
#else
#define	GETMODULEBASENAME	GETMODULEBASENAMEA
#endif

      // Win95 functions
      typedef HANDLE (WINAPI *CREATESNAPSHOT)(
         DWORD dwFlags, 
         DWORD th32ProcessID
         ); 

      typedef BOOL (WINAPI *THREADWALK)(
         HANDLE hSnapshot,
         LPTHREADENTRY32 lpte  
         );

      typedef BOOL (WINAPI *PROCESSWALK)(
         HANDLE hSnapshot,    
         LPPROCESSENTRY32 lppe
         );

      typedef BOOL (WINAPI *MODULEWALK)(
         HANDLE hSnapshot,
         LPMODULEENTRY32 lpme
         );


      bool	m_bNeedPSAPI;

      // Windows NT variables
      HMODULE	modPSAPI;
      HMODULE	modVDMDBG;

      ENUMPROCESSES       pfnEnumProcesses;
      GETMODULEFILENAME   pfnGetModuleFileName;
      ENUMPROCESSMODULES  pfnEnumProcessModules;  
      VDMENUMTASKWOWEX	pfnVDMEnumTaskWOWEx;
      GETMODULEBASENAME	pfnGetModuleBaseName;
      GETMEMINFO          pfnGetMemInfo;

      // Win2K + variables
      CREATESNAPSHOT pfnCreateToolhelp32Snapshot; 
      PROCESSWALK pfnProcess32First; 
      PROCESSWALK pfnProcess32Next; 
      THREADWALK pfnThread32First;
      THREADWALK pfnThread32Next;
      MODULEWALK pfnModule32First; 
      MODULEWALK pfnModule32Next; 

      BOOL OnDeviceDriver(LPVOID lpImageBase)
      {
         return TRUE;
      }

      BOOL OnProcess(LPCTSTR lpszFileName, DWORD ProcessID, PPROCESS_MEMORY_COUNTERS pMemCounters)
      {
         return TRUE;
      }

      BOOL OnModule(HMODULE hModule, LPCTSTR lpszModuleName, LPCTSTR lpszPathName)
      {
         return TRUE;
      }

      BOOL OnThread(DWORD ThreadID)
      {
         return TRUE;
      }

   }

   BOOL Load(void)
   {
      OSVERSIONINFO ovi;
      ovi.dwOSVersionInfoSize = sizeof(ovi);
      GetVersionEx(&ovi);

      // PSAPI is for Windows NT verison 4.0
      m_bNeedPSAPI = ovi.dwPlatformId == VER_PLATFORM_WIN32_NT && ovi.dwMajorVersion <= 4;

      if ( m_bNeedPSAPI )
      {
         modPSAPI = LoadLibrary(_T("PSAPI.DLL"));
         modVDMDBG = LoadLibrary(_T("VDMDBG.DLL"));

         if ( modPSAPI && modVDMDBG )
         {
            pfnVDMEnumTaskWOWEx   = (VDMENUMTASKWOWEX)GetProcAddress(modVDMDBG, "VDMEnumTaskWOWEx");
            pfnEnumProcesses      = (ENUMPROCESSES)GetProcAddress(modPSAPI, "EnumProcesses");
            pfnGetMemInfo         = (GETMEMINFO)GetProcAddress(modPSAPI, "GetProcessMemoryInfo");
#ifdef	UNICODE
            pfnGetModuleFileName  = (GETMODULEFILENAME)GetProcAddress(modPSAPI, "GetModuleFileNameExW");
            pfnGetModuleBaseName  = (GETMODULEBASENAME)GetProcAddress(modPSAPI, "GetModuleBaseNameW");
#else
            pfnGetModuleFileName  = (GETMODULEFILENAME)GetProcAddress(modPSAPI, "GetModuleFileNameExA");
            pfnGetModuleBaseName  = (GETMODULEBASENAME)GetProcAddress(modPSAPI, "GetModuleBaseNameA");
#endif
            pfnEnumProcessModules = (ENUMPROCESSMODULES)GetProcAddress(modPSAPI, "EnumProcessModules");

            if (
               pfnVDMEnumTaskWOWEx	&&
               pfnEnumProcesses		&& 
               pfnGetModuleFileName	&& 
               pfnGetModuleBaseName	&&
               pfnGetMemInfo       	&&
               pfnEnumProcessModules  
               )
            {
               return TRUE;	
            }
         }
      }
      else
      {
         HINSTANCE modKERNEL = GetModuleHandle(_T("KERNEL32.DLL"));
         pfnCreateToolhelp32Snapshot = (CREATESNAPSHOT)GetProcAddress(modKERNEL, "CreateToolhelp32Snapshot"); 
         pfnThread32First = (THREADWALK)GetProcAddress(modKERNEL, "Thread32First");
         pfnThread32Next = (THREADWALK)GetProcAddress(modKERNEL, "Thread32Next");

#ifdef UNICODE        
         pfnProcess32First = (PROCESSWALK)GetProcAddress(modKERNEL, "Process32FirstW"); 
         pfnProcess32Next  = (PROCESSWALK)GetProcAddress(modKERNEL, "Process32NextW"); 
         /*pfnThread32First = (THREADWALK)GetProcAddress(modKERNEL, "Thread32FirstW");
         pfnThread32Next = (THREADWALK)GetProcAddress(modKERNEL, "Thread32NextW");*/
         pfnModule32First = (MODULEWALK)GetProcAddress(modKERNEL, "Module32FirstW"); 
         pfnModule32Next  = (MODULEWALK)GetProcAddress(modKERNEL, "Module32NextW"); 
#else
         pfnProcess32First = (PROCESSWALK)GetProcAddress(modKERNEL, "Process32First"); 
         pfnProcess32Next  = (PROCESSWALK)GetProcAddress(modKERNEL, "Process32Next"); 
         /*pfnThread32First = (THREADWALK)GetProcAddress(modKERNEL, "Thread32First");
         pfnThread32Next = (THREADWALK)GetProcAddress(modKERNEL, "Thread32Next");*/
         pfnModule32First = (MODULEWALK)GetProcAddress(modKERNEL, "Module32First"); 
         pfnModule32Next  = (MODULEWALK)GetProcAddress(modKERNEL, "Module32Next"); 
#endif

         if (
            pfnCreateToolhelp32Snapshot   && 
            pfnThread32First              &&
            pfnThread32Next               &&
            pfnModule32First              &&
            pfnModule32Next               &&
            pfnProcess32First				&&
            pfnProcess32Next)
         {
            return TRUE;
         }
      }

      return FALSE;
   }

   void Unload()
   {
      ATLTRACE2(_T("Destroying PSAPI object...\n"));

      if ( m_bNeedPSAPI )
      {
         FreeLibrary((HINSTANCE)modVDMDBG);
         FreeLibrary((HINSTANCE)modPSAPI);
      }
   }

   BOOL EnumDeviceDrivers(void)
   {
      return FALSE;
   }

   BOOL EnumProcesses(void)
   {
      if ( m_bNeedPSAPI )
      {
         DWORD nProcessIDs[PSAPI_MAX_HANDLE_COUNT];
         DWORD nProcessNo;

         BOOL bSuccess = pfnEnumProcesses(nProcessIDs, sizeof(nProcessIDs), &nProcessNo);
         nProcessNo /= sizeof(nProcessIDs[0]);

         if ( !bSuccess )
         {
            return FALSE;
         }  

         for ( unsigned i=0; i<nProcessNo; i++)
         {
            HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, 
               FALSE, nProcessIDs[i]);

            HMODULE hModules[PSAPI_MAX_HANDLE_COUNT];
            DWORD nModuleNo;
            TCHAR szFileName[PSAPI_MAX_PATH];
            PROCESS_MEMORY_COUNTERS memCounters;
            ZeroMemory(&memCounters, sizeof(memCounters));

            pfnGetMemInfo(process, &memCounters, sizeof(memCounters));

            pfnEnumProcessModules(process, hModules, sizeof(hModules), &nModuleNo);

            nModuleNo /= sizeof(hModules[0]);

            if ( pfnGetModuleFileName(process, hModules[0], szFileName, sizeof(szFileName)) )
            {
               if ( OnProcess(szFileName, nProcessIDs[i], &memCounters) == FALSE )
               {
                  break;
               }

               pfnGetModuleBaseName(process, hModules[0], szFileName, sizeof(szFileName));

               if ( 0 == _tcsicmp(szFileName, _T("NTVDM.EXE")) )
               {
                  // We've got an NT VDM -- enumerate the processes it contains.
                  //pVDMEnumTaskWOWEx(nProcessIDs[i], show_task, (long)&disp);
               }
            }
            CloseHandle(process);
         }

         return TRUE;
      }
      else
      {
         PROCESSENTRY32 proc;
         proc.dwSize = sizeof(proc);

         HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
         /* Now that we have a snapshot of the system state, we simply
         * walk the list it represents by calling Process32First once,
         * then call Proces32Next repeatedly until we get to the end 
         * of the list.
         */
         pfnProcess32First(snapshot, &proc);
         if ( OnProcess(proc.szExeFile, proc.th32ProcessID, NULL) == FALSE )
         {
            CloseHandle(snapshot);
            return TRUE;
         }

         while ( pfnProcess32Next(snapshot, &proc) )
            if ( OnProcess(proc.szExeFile, proc.th32ProcessID, NULL) == FALSE )
               break;

         /* This should happen automatically when we terminate, but it never
         * hurts to clean up after ourselves.
         */
         CloseHandle(snapshot);

         return TRUE;
      }
   }

   BOOL EnumProcessModules(DWORD dwProcessId)
   {
      if ( m_bNeedPSAPI )
      {
         HMODULE hModules[PSAPI_MAX_HANDLE_COUNT];
         DWORD nModuleNo;

         HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
         if ( process == NULL )
            return FALSE;

         BOOL bSuccess = pfnEnumProcessModules(process, hModules, sizeof(hModules), &nModuleNo);
         nModuleNo /= sizeof(hModules[0]);

         if ( !bSuccess )
         {
            CloseHandle(process);
            return FALSE;
         }

         TCHAR szFileName[PSAPI_MAX_PATH];
         for ( unsigned i=0; i<nModuleNo; i++)
         {
            if ( pfnGetModuleFileName(process, hModules[i], szFileName, sizeof(szFileName)) == FALSE )
            {
               CloseHandle(process);
               return FALSE;
            }

            LPTSTR szSep = _tcsrchr(szFileName, '\\');
            if ( szSep )
               szSep++;

            if ( OnModule(hModules[i], szFileName, szSep ) == FALSE )
               break;
         }

         CloseHandle(process);
         return TRUE;
      }
      else
      {
         MODULEENTRY32 module;
         module.dwSize = sizeof(module);

         HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
         /* Now that we have a snapshot of the system state, we simply
         * walk the list it represents by calling Process32First once,
         * then call Proces32Next repeatedly until we get to the end 
         * of the list.
         */
         pfnModule32First(snapshot, &module);
         if ( OnModule(module.hModule, module.szExePath, module.szModule) == FALSE )
         {
            CloseHandle(snapshot);
            return TRUE;
         }

         while ( pfnModule32Next(snapshot, &module) )
            if ( OnModule(module.hModule, module.szExePath, module.szModule) == FALSE )
               break;

         /* This should happen automatically when we terminate, but it never
         * hurts to clean up after ourselves.
         */
         CloseHandle(snapshot);
         return TRUE;
      }
   }

   BOOL EnumProcessThreads(DWORD dwProcessId)
   {
      if ( m_bNeedPSAPI )
      {
         _ASSERT(FALSE); /// NOT IMPLEMENTED YET for NT 4.0
         return TRUE;
      }
      else
      {
         THREADENTRY32 thread;
         thread.dwSize = sizeof(thread);

         HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, dwProcessId);

         pfnThread32First(snapshot, &thread);

         if ( OnThread(thread.th32ThreadID) == FALSE )
         {
            CloseHandle(snapshot);
            return TRUE;
         }

         while ( pfnThread32Next(snapshot, &thread) )
         {
            if ( OnThread(thread.th32ThreadID) == FALSE )
            {
               break;
            }
         }

         /* This should happen automatically when we terminate, but it never
         * hurts to clean up after ourselves.
         */
         CloseHandle(snapshot);
         return TRUE;
      }
   }

   BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
   {   
      WINDOWLIST & wl = *(PWINDOWLIST)lParam;

      wl.push_back(hwnd);
      return TRUE;
   }


   HWND GetWindowByProcess(LPCTSTR szProcessName, LPCTSTR szWindowClass, LPCTSTR szWindowText, int iXpos, int iYpos)
   {
      DWORD dwProcessId = 0;
      PROCESSINFOLIST pil = GetProcessList();

      PROCESSINFOLIST::iterator iter;

      for( iter = pil.begin();  iter < pil.end(); iter++)
      {
         PROCESSINFO & pi = (PROCESSINFO &)*iter;

         if( !_tcsicmp(pi.szProcessName, szProcessName) )
         {
            dwProcessId = pi.dwProcessID;
            break;
         }
      }
      return GetWindowByProcess(dwProcessId, szWindowClass, szWindowText, iXpos, iYpos);
   }


   HWND GetWindowByProcess(DWORD dwProcessId, LPCTSTR szWindowClass, LPCTSTR szWindowText, int iXpos, int iYpos)
   {
      if( !dwProcessId ) return NULL;

      // we've got the process ID, enumerate all top level windows and look for a matching process ID

      WINDOWLIST wl;
      wl.clear();
      EnumWindows(EnumWindowsProc, (LPARAM)&wl);

      WINDOWLIST::iterator iter;
      HWND hWnd = NULL;
      BOOL bFound = FALSE;

      for(iter = wl.begin(); !bFound && (iter < wl.end()); iter++)
      {
         hWnd = *iter;
         DWORD dw = 0;
         DWORD dwThreadId = 0;
         dwThreadId = GetWindowThreadProcessId(hWnd, &dw);

         if( dwProcessId == dw )
         {
            bFound = TRUE;
         }
      }
#define WINBUF      1000

      if( bFound )
      {
         HWND hWndMatch = NULL;

         // Check if the primary window matches our query?
         if( szWindowClass )
         {
            TCHAR szClass[WINBUF];
            GetClassName(hWnd, szClass, WINBUF-1);
            if( !_tcsicmp(szWindowClass, szClass) )
            {
               // window class matches, check if window text is
               // specified as well, if it is it must match too.
               if( szWindowText )
               {
                  TCHAR szWindow[WINBUF];
                  GetWindowText(hWnd, szWindow, WINBUF-1);

                  if( !_tcsicmp(szWindowText, szWindow) )
                  {
                     // we've matched the primary window on class
                     // and window text
                     hWndMatch = hWnd;
                  }
                  else
                  {
                     // no Window text match!
                  }
               }
               else
               {
                  // we matched the primary window on class
                  hWndMatch = hWnd;
               }
            }
            else
            {
               // no class match!
            }
         }

         // If primary window doesn't match let's look through the children
         if( !hWndMatch )
         {
            hWndMatch = FindWindowEx(hWnd, NULL, szWindowClass, szWindowText);
         }

         //
         // We're running out of possibilities here.  Let's hope that the X and Y positions 
         // have been specified so that we can get a handle to a window using a hittest.
         //
         if( !hWndMatch && ((iXpos != -1) && (iYpos != -1)) )
         {
            WINDOWLIST wl;
            wl.clear();
            EnumChildWindows(hWnd, EnumWindowsProc, (LPARAM)&wl);

            WINDOWLIST::iterator iter;
            HWND hChildWnd = NULL;
            DWORD dwArea = 0;
            DWORD dwCandidateArea = 0;
            POINT pt = {iXpos, iYpos};

            for(iter = wl.begin(); (iter < wl.end()); iter++)
            {
               hChildWnd = *iter;

               RECT rect = { 0 };
               if( GetWindowRect(hChildWnd, &rect) )
               {
                  if( PtInRect(&rect, pt) )
                  {
                     // calculate the surface area
                     DWORD dwArea = (rect.bottom - rect.top)*(rect.right-rect.left);

                     // if we had a candidate compare and substitute if smaller
                     if( !dwCandidateArea || (dwArea < dwCandidateArea)  )
                     {
                        dwCandidateArea = dwArea;
                        hWndMatch = hChildWnd;
                     }
                  }
               }
            }

         }

         return hWndMatch;
      }
      return NULL;
   }

   PROCESSMODULEINFOLIST GetProcessModuleList(DWORD dwProcessId)
   {

      PROCESSMODULEINFOLIST pmil;

      pmil.clear();

      if ( m_bNeedPSAPI )
      {
         HMODULE hModules[PSAPI_MAX_HANDLE_COUNT];
         DWORD nModuleNo;

         HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwProcessId);
         if ( process == NULL )
            return pmil;

         BOOL bSuccess = pfnEnumProcessModules(process, hModules, sizeof(hModules), &nModuleNo);
         nModuleNo /= sizeof(hModules[0]);

         if ( !bSuccess )
         {
            CloseHandle(process);
            return pmil;
         }

         TCHAR szFileName[PSAPI_MAX_PATH];
         for ( unsigned i=0; i<nModuleNo; i++)
         {
            if ( pfnGetModuleFileName(process, hModules[i], szFileName, sizeof(szFileName)) == FALSE )
            {
               CloseHandle(process);
               return pmil;
            }

            LPTSTR szSep = _tcsrchr(szFileName, '\\');
            if ( szSep )
               szSep++;

            PROCESSMODULEINFO pmi = {0};
            pmi.hModule = hModules[i];
            _tcsncpy(pmi.szPathName, szFileName, PSAPI_MAX_PATH);
            _tcsncpy(pmi.szModuleName, szSep, PSAPI_MAX_PATH);
            pmil.push_back(pmi);
         }

         CloseHandle(process);
         return pmil;
      }
      else
      {
         MODULEENTRY32 module;
         module.dwSize = sizeof(module);

         HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessId);
         /* Now that we have a snapshot of the system state, we simply
         * walk the list it represents by calling Process32First once,
         * then call Proces32Next repeatedly until we get to the end 
         * of the list.
         */
         pfnModule32First(snapshot, &module);

         PROCESSMODULEINFO pmi = {0};
         pmi.hModule = module.hModule;
         _tcsncpy(pmi.szPathName, module.szExePath, PSAPI_MAX_PATH);
         _tcsncpy(pmi.szModuleName, module.szModule, PSAPI_MAX_PATH);
         pmil.push_back(pmi);

         while ( pfnModule32Next(snapshot, &module) )
         {
            PROCESSMODULEINFO pmi = {0};
            pmi.hModule = module.hModule;
            _tcsncpy(pmi.szPathName, module.szExePath, PSAPI_MAX_PATH);
            _tcsncpy(pmi.szModuleName, module.szModule, PSAPI_MAX_PATH);
            pmil.push_back(pmi);
         }


         /* This should happen automatically when we terminate, but it never
         * hurts to clean up after ourselves.
         */
         CloseHandle(snapshot);
         return pmil;
      }
   }

   PROCESSINFOLIST GetProcessList()
   {
      PROCESSINFOLIST pil;

      pil.clear();

      if ( m_bNeedPSAPI )
      {
         DWORD nProcessIDs[PSAPI_MAX_HANDLE_COUNT];
         DWORD nProcessNo;

         BOOL bSuccess = pfnEnumProcesses(nProcessIDs, sizeof(nProcessIDs), &nProcessNo);
         nProcessNo /= sizeof(nProcessIDs[0]);

         if ( !bSuccess )
         {
            return pil;
         }  

         for ( unsigned i=0; i<nProcessNo; i++)
         {
            HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, nProcessIDs[i]);

            HMODULE hModules[PSAPI_MAX_HANDLE_COUNT];
            DWORD nModuleNo;
            TCHAR szFileName[PSAPI_MAX_PATH];
            PROCESS_MEMORY_COUNTERS memCounters;
            ZeroMemory(&memCounters, sizeof(memCounters));

            pfnGetMemInfo(process, &memCounters, sizeof(memCounters));

            pfnEnumProcessModules(process, hModules, sizeof(hModules), &nModuleNo);

            nModuleNo /= sizeof(hModules[0]);

            if ( pfnGetModuleFileName(process, hModules[0], szFileName, sizeof(szFileName)) )
            {
               PROCESSINFO pi = { 0 };
               _tcsncpy(pi.szProcessName, szFileName, PSAPI_MAX_PATH);
               pi.dwProcessID = nProcessIDs[i];
               pil.push_back(pi);
            }
            CloseHandle(process);
         }

         return pil;
      }
      else
      {
         PROCESSENTRY32 proc;
         proc.dwSize = sizeof(proc);

         HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
         /* Now that we have a snapshot of the system state, we simply
         * walk the list it represents by calling Process32First once,
         * then call Proces32Next repeatedly until we get to the end 
         * of the list.
         */
         pfnProcess32First(snapshot, &proc);
         PROCESSINFO pi = { 0 };

         _tcsncpy(pi.szProcessName, proc.szExeFile, PSAPI_MAX_PATH);
         pi.dwProcessID = proc.th32ProcessID;
         pil.push_back(pi);

         while ( pfnProcess32Next(snapshot, &proc) )
         {
            _tcsncpy(pi.szProcessName, proc.szExeFile, PSAPI_MAX_PATH);
            pi.dwProcessID = proc.th32ProcessID;
            pil.push_back(pi);
         }

         /* This should happen automatically when we terminate, but it never
         * hurts to clean up after ourselves.
         */
         CloseHandle(snapshot);

         return pil;
      }
   }

   PROCESSMODULEINFO FindLoadedModuleInfo(DWORD dwProcessId, LPCTSTR szPartialModuleName)
   {
      PROCESSMODULEINFOLIST pmil = GetProcessModuleList(dwProcessId);
      PROCESSMODULEINFOLIST::iterator iter;
      PROCESSMODULEINFO pmiRet = { 0 };

      LPTSTR pszPartial = _tcslwr(_tcsdup(szPartialModuleName));
      LPTSTR pszModule = NULL;

      for( iter = pmil.begin(); iter < pmil.end(); iter++)
      {
         PROCESSMODULEINFO & pmi = (PROCESSMODULEINFO &)*iter;

         pszModule = _tcslwr(_tcsdup(pmi.szModuleName));
         if( _tcsstr(pszModule, pszPartial) )
         {
            // we found a match!
            pmiRet = pmi;
            break;
         }
         free(pszModule);
         pszModule = NULL;
      }

      if( pszPartial )
      {
         free(pszPartial);
         pszPartial = NULL;
      }
      if( pszModule )
      {
         free(pszModule);
         pszModule = NULL;
      }
      return pmiRet;
   }

   DWORD GetFirstMatchProcessId(LPCTSTR szProcessName)
   {
      PROCESSINFOLIST pil = GetProcessList();

      PROCESSINFOLIST::iterator iter;

      for( iter = pil.begin();  iter < pil.end(); iter++)
      {
         PROCESSINFO & pi = (PROCESSINFO &)*iter;

         if( !_tcsicmp(pi.szProcessName, szProcessName) )
         {
            return pi.dwProcessID;
         }
      }
      return 0;
   }

}
