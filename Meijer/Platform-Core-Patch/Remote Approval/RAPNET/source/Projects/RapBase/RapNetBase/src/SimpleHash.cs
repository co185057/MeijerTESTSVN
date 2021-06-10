using System;
using System.Text;
using System.Security.Cryptography;
using RPSWNET;

/// <summary>
/// This class generates and compares hashes using MD5, SHA1, SHA256, SHA384, 
/// and SHA512 hashing algorithms. To verify another plain text value against the given hash,
/// compute a new hash from the plain text.
/// </summary>
namespace RapNet
{
	/// <summary>
	/// Configuration data singleton used by the application.
	/// </summary>
	public class SimpleHash
	{
		/// <summary>
		/// Generates a hash for the given plain text value and returns a
		/// base16-encoded result.
		/// </summary>
		/// <param name="plainText">
		/// Plaintext value to be hashed. The function does not check whether
		/// this parameter is null.
		/// </param>
		/// <param name="hashAlgorithm">
		/// Name of the hash algorithm. Allowed values are: "MD5", "SHA1",
		/// "SHA256", "SHA384", and "SHA512" (if any other value is specified
		/// MD5 hashing algorithm will be used). This value is case-insensitive.
		/// </param>
		/// <returns>
		/// Hash value formatted as a base16-encoded string.
		/// </returns>
		public static string ComputeHash(string plainText, string hashAlgorithm)
		{
			string hashValue = "";
			// Convert plain text into a byte array.
			byte[] plainTextBytes = Encoding.UTF8.GetBytes(plainText);
			// Because we support multiple hashing algorithms, we must define
			// hash object as a common (abstract) base class. We will specify the
			// actual hashing algorithm class later during object creation.
			HashAlgorithm hash;
			// Make sure hashing algorithm name is specified.
            if (hashAlgorithm == null)
            {
                hashAlgorithm = "";
            }        
			// Initialize appropriate hashing algorithm class.
			switch (hashAlgorithm.ToUpper())
			{
#if (WINCE)
                default:
                    hash = new SHA1Managed();
                    break;
#else
				case "SHA1":
					hash = new SHA1Managed();
					break;
				case "MD5":
					hash = new MD5CryptoServiceProvider();
					break;
				case "SHA384":
					hash = new SHA384Managed();
					break;
				case "SHA512":
					hash = new SHA512Managed();
					break;
                default:
					hash = new SHA256Managed();
					break;
#endif                
			}
			// Compute hash value of our plain text 
			byte[] hashBytes = hash.ComputeHash(plainTextBytes);
			// Base 16 encoding...
			for (int j = 0; j < hashBytes.Length; j++) 
			{
				for (int i = 1; i >= 0; i--) 
				{
					char ch = "0123456789ABCDEF"[((hashBytes[j] >> i * 4) & 0xF)];
					hashValue += ch;
				}
			}
			// Return the result.
			return hashValue;
		}

		/// <summary>
		/// Compares a hash of the specified plain text value to a given hash
		/// value. Plain text is hashed. 
		/// </summary>
		/// <param name="plainText">
		/// Plain text to be verified against the specified hash. The function
		/// does not check whether this parameter is null.
		/// </param>
		/// <param name="hashAlgorithm">
		/// Name of the hash algorithm. Allowed values are: "MD5", "SHA1", 
		/// "SHA256", "SHA384", and "SHA512" (if any other value is specified,
		/// MD5 hashing algorithm will be used). This value is case-insensitive.
		/// </param>
		/// <param name="hashValue">
		/// Base16-encoded hash value produced by ComputeHash function. 
		/// </param>
		/// <returns>
		/// If computed hash mathes the specified hash the function the return
		/// value is true; otherwise, the function returns false.
		/// </returns>
		public static bool VerifyHash(string plainText, string hashAlgorithm, string hashValue)
		{
			try
			{			
				// Compute a new hash string.
				string expectedHashString = ComputeHash(plainText, hashAlgorithm);

				// If the computed hash matches the specified hash,
				// the plain text value must be correct.
				return (hashValue == expectedHashString);
			}
			catch (Exception e)
			{
				CmDataCapture.Capture(CmDataCapture.MaskError, "VerifyHash: EXCEPTION " + e.Message);
				return false;
			}
		}
	}
}






