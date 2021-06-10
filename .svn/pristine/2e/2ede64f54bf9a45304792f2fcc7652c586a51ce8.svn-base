using System;
using System.Data;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Reboot
{
	/// <summary>
	/// Summary description for Class1.
	/// </summary>
	class Class1
	{
		const string coreDll = "coredll.dll";
		
		// Control Code flags
		public const uint FILE_DEVICE_HAL = 0x00000101;
		public const uint FILE_DEVICE_CONSOLE = 0x00000102;
		public const uint FILE_DEVICE_PSL = 0x00000103;
		public const uint METHOD_BUFFERED = 0;
		public const uint METHOD_IN_DIRECT = 1;
		public const uint METHOD_OUT_DIRECT = 2;
		public const uint METHOD_NEITHER = 3;
		public const uint FILE_ANY_ACCESS = 0;
		public const uint FILE_READ_ACCESS = 0x0001;
		public const uint FILE_WRITE_ACCESS = 0x0002;

		/// <summary>
		/// Create a control code from the specified device, function, method, and
		/// access flags.
		/// </summary>
		/// <param name="DeviceType">Device type flag</param>
		/// <param name="Function">Function number</param>
		/// <param name="Method">Method flag</param>
		/// <param name="Access">Access flag</param>
		/// <returns>A control code based on the specified parameters</returns>
		public static uint CTL_CODE(uint DeviceType, uint Function, uint Method, uint Access)
		{
			return ((DeviceType << 16) | (Access << 14) | (Function << 2) | Method);
		}

		/// <summary>
		/// This function provides the kernel with a generic I/O control for
		/// carrying out I/O operations.
		/// </summary>
		/// <param name="dwIoControlCode">I/O control code, which should support the
		/// OAL I/O controls. For a list of these I/O controls, see Supported
		/// OAL APIs.</param>
		/// <param name="lpInBuf">Pointer to the input buffer.</param>
		/// <param name="nInBufSize">Size, in bytes, of lpInBuf.</param>
		/// <param name="lpOutBuf">Pointer to the output buffer.</param>
		/// <param name="nOutBufSize">Maximum number of bytes that can be returned in
		/// lpOutBuf.</param>
		/// <param name="lpBytesReturned">Address of a DWORD that receives the size,
		/// in bytes, of the data returned.</param>
		/// <returns>TRUE indicates success; FALSE indicates failure.</returns>
		[DllImport( coreDll )]
		public extern static uint KernelIoControl
		(
			uint dwIoControlCode,
			IntPtr lpInBuf,
			uint nInBufSize,
			IntPtr lpOutBuf,
			uint nOutBufSize,
			ref uint lpBytesReturned
		);

		/// <summary>
		/// The main entry point for the application.
		/// </summary>
		static void Main(string[] args)
		{
			/*
			System.Windows.Forms.DialogResult r = System.Windows.Forms.MessageBox.Show
			(
				"Are you sure?",
				"Reboot",
				System.Windows.Forms.MessageBoxButtons.YesNo,
				System.Windows.Forms.MessageBoxIcon.Question,
				System.Windows.Forms.MessageBoxDefaultButton.Button2
			);
			*/
			// if (r == System.Windows.Forms.DialogResult.Yes)
			{
				uint bytesReturned = 0;
				uint IOCTL_HAL_REBOOT = CTL_CODE(FILE_DEVICE_HAL, 15, METHOD_BUFFERED, FILE_ANY_ACCESS);
				KernelIoControl(IOCTL_HAL_REBOOT, IntPtr.Zero, 0, IntPtr.Zero, 0, ref bytesReturned);
			}
		}
	}
}
