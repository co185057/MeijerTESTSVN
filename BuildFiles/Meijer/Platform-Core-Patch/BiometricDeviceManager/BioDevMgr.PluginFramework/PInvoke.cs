using System;
using System.Text;
using System.Collections;
using System.Runtime.InteropServices;
using RPSWNET;

namespace BioDevMgr
{
    /// <summary>
    /// PInvoke definitions used by the application.
    /// </summary>
    public class PInvoke
    {
        public const uint HKEY_LOCAL_MACHINE = 0x80000002;
        public const uint INFINITE = 0xFFFFFFFF;
        public const uint KEY_READ = 0x00020019;
        public const uint MAX_PATH = 260;
        public const uint REG_DWORD = 4;
        public const uint REG_SZ = 1;
        public const uint WAIT_OBJECT_0 = 0;
        public const uint WM_QUIT = 0x00000012;

        public enum PeekMessageOption
        {
            PM_NOREMOVE = 0x0000,
            PM_REMOVE = 0x0001,
            PM_NOYIELD = 0x0002
        };

        // Used by MsgWaitForMultipleObjects
        public enum WakeMask
        {
            QS_ALLEVENTS = 0x04BF,
            QS_ALLINPUT = 0x04FF,
            QS_ALLPOSTMESSAGE = 0x0100,
            QS_HOTKEY = 0x0080,
            QS_INPUT = 0x407,
            QS_KEY = 0x0001,
            QS_MOUSE = 0x0006,
            QS_MOUSEBUTTON = 0x0004,
            QS_MOUSEMOVE = 0x0002,
            QS_PAINT = 0x0020,
            QS_POSTMESSAGE = 0x0008,
            QS_RAWINPUT = 0x0400,
            QS_SENDMESSAGE = 0x0040,
            QS_TIMER = 0x0010
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct POINT
        {
            public Int32 x;
            public Int32 y;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct MSG
        {
            public Int32 hwnd;
            public Int32 message;
            public Int32 wParam;
            public Int32 lParam;
            public Int32 time;
            public POINT pt;
        };

        [StructLayout(LayoutKind.Sequential)]
        public struct MEMORYSTATUS
        {
            public int Length;
            public int MemoryLoad;
            public UIntPtr TotalPhysical;
            public UIntPtr AvailablePhysical;
            public UIntPtr TotalPageFile;
            public UIntPtr AvailablePageFile;
            public UIntPtr TotalVirtual;
            public UIntPtr AvailableVirtual;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SYSTEMTIME
        {
            public short Year;
            public short Month;
            public short DayOfWeek;
            public short Day;
            public short Hour;
            public short Minute;
            public short Second;
            public short Milliseconds;
        }

        [StructLayout(LayoutKind.Sequential)]
        public struct SECURITY_ATTRIBUTES
        {
            public uint nLength;
            public IntPtr lpSecurityDescriptor;
            public bool bInheritHandle;
        }

        private const string kernelApiDll = "kernel32.dll";
        private const string regApiDll = "advapi32.dll";
        private const string userDll = "user32.dll";
        private const string winsockDll = "ws2_32.dll";

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern IntPtr FindWindow(string lpClassName, string lpWindowName);

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern uint MsgWaitForMultipleObjects(uint numHandles, IntPtr[] handleArrays, bool waitAll, uint timeOut, WakeMask wakeMask);

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool PeekMessage(ref MSG lpMsg, Int32 hwnd, Int32 wMsgFilterMin, Int32 wMsgFilterMax, PeekMessageOption wRemoveMsg);

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool TranslateMessage(ref MSG lpMsg);

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern Int32 DispatchMessage(ref MSG lpMsg);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool FreeLibrary(IntPtr hModule);

        // Don't force the CharSet.Unicode for GetProcAddress; it will not find entrypoint in DLL
        [DllImport(kernelApiDll, SetLastError = false)]
        public static extern IntPtr GetProcAddress(IntPtr hModule, string procedureName);

        [DllImport(winsockDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int gethostname(byte[] host, int len);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern void GlobalMemoryStatus(out MEMORYSTATUS lpBuffer);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern IntPtr LoadLibrary(string lpFileName);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern IntPtr CreateEventW(IntPtr lpEventAttributes, bool bManualReset, bool bInitialState, string lpName);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool SetEvent(IntPtr hEvent);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern uint WaitForMultipleObjects(uint numHandles, IntPtr[] handleArrays, bool waitAll, uint timeOut);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool CloseHandle(IntPtr hObject);

        [DllImport(regApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int RegCloseKey(uint hkey);

        [DllImport(regApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int RegOpenKeyExW(uint hkey, string subKey, uint options, uint sam, ref uint phkResult);

        [DllImport(regApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int RegQueryValueExW(uint hkey, string valueName, int reserved, ref uint type, byte[] data, ref uint len);

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool SetLocalTime(ref SYSTEMTIME time);

        [DllImport(userDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern bool ShowWindow(IntPtr hWnd, int nCmdShow);

        [DllImport(winsockDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern int WSACleanup();

        [DllImport(kernelApiDll, CharSet = CharSet.Unicode, SetLastError = false)]
        public static extern uint GetCurrentProcess();
    }
}
