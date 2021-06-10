/// 
/// The SHA256Managed class. The class SHA256Managed was
/// not available in Windows CE. Instead, this SHA256Managed class will be used
/// for Windows CE.
///
using System;
using System.Text;
using System.Runtime.InteropServices;
using RPSWNET;
namespace LaunchPadNet
{
	/// <summary>
	/// The Hash class. 
	/// </summary>
	public class SHA256Managed : LaunchPadNet.HashAlgorithm
	{
#if( DEBUG )
		public const string simpleHashDll = "SimpleHashUD.dll";
#else
		public const string simpleHashDll = "SimpleHashU.dll";
#endif
		[DllImport(simpleHashDll, CharSet=CharSet.Unicode, SetLastError=false)]
		public static extern void SHA256Hash(
			string plainText, byte[] hashedText);

		/// <summary>
		/// The class constructor.
		/// </summary>
		public SHA256Managed()
		{
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~SHA256Managed()
		{
		}
		
		/// <summary>
		/// Compute the hash array and return it.
		/// </summary>
		public override byte[] ComputeHash(
			byte[] buffer
			)
		{
			string plainText=Encoding.UTF8.GetString(buffer,0,buffer.Length);
			byte[] hashBytes;

			hashBytes = new byte[32];
			CmDataCapture.Capture( CmDataCapture.MaskError, "SHA256Managed::ComputeHash() - Before plainText = " + plainText );
			SHA256Hash(plainText, hashBytes);
			CmDataCapture.Capture( CmDataCapture.MaskError, "SHA256Managed::ComputeHash() - After " );

			return hashBytes;
		}
	}
}
