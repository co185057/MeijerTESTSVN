using System;
using System.Text.RegularExpressions;

namespace CommonLibsNet
{
	/// <summary>
	/// Summary description for SignOnBuilder.
	/// </summary>
	public class SignOnBuilder
	{
		public SignOnBuilder(string userId)
		{
			this.userId = userId;
		}

		public SignOnBuilder(string userId, string userPwd)
		{
			this.userId = userId;
			this.userPwd = userPwd;
		}
		public SignOnBuilder(string userId, string userPwd, bool userIdScanned)
		{
			this.userId = userId;
			this.userPwd = userPwd;
			this.userIdScanned = userIdScanned;
		}
		public SignOnBuilder(string userId, string userPwd, bool userIdScanned, string biometData)
		{
			this.userId = userId;
			this.userPwd = userPwd;
			this.userIdScanned = userIdScanned;
			this.biometData = biometData;
		}

		public static SignOnBuilder FromString(string serialString)
		{
			string userId = "";
			string userPwd = "";
			bool userIdScanned = false;
			string biometData = "";

			if( serialString.StartsWith(TERMINATOR) )
			{
				// extract the username
				if( serialString.EndsWith(TERMINATOR) )
				{
					serialString = serialString.Substring(0, serialString.Length-TERMINATOR.Length);
				}

				string [] fields = Regex.Split(serialString,START);
				foreach(string field in fields)
				{
					int namePos = field.IndexOf(ASSIGN);
					if( namePos > -1 )
					{
						string name = field.Substring(0,namePos);
						string val = field.Substring(namePos+ASSIGN.Length,field.Length-(namePos + END.Length + ASSIGN.Length));
						switch(name)
						{
							case PARM_USERID: userId = val; break;
							case PARM_USERPWD: userPwd = val; break;
							case PARM_USERIDSCANNED: userIdScanned = (val == "1"); break;
							case PARM_BIOMETDATA: biometData = val; break;
						}
					}
				}
			}
			return new SignOnBuilder(userId, userPwd, userIdScanned, biometData);
		}

		public override string ToString()
		{
			string output = TERMINATOR;

			if( userId != "" )
			{
				output += START + PARM_USERID + ASSIGN + userId + END;
			}
			if( userPwd != "" )
			{
				output += START + PARM_USERPWD + ASSIGN + userPwd + END;
			}
			if( userIdScanned )
			{
				output += START + PARM_USERIDSCANNED + ASSIGN + (userIdScanned ? "1" : "0") + END;
			}
			if( biometData != "" )
			{
				output += START + PARM_BIOMETDATA + ASSIGN + biometData + END;
			}
			output+= TERMINATOR;

			return output;
		}

		public static bool IsLegacyPassword(string passwordString) 
		{ 
			return passwordString.StartsWith(TERMINATOR); 
		}

		public string UserId { get { return userId; } }
		public string UserPwd { get { return userPwd; } }
		public bool UserIdScanned { get { return userIdScanned; } }
		public string BiometData { get { return biometData; } }

		private const string TERMINATOR = "$$$";
		private const string START = "{{";
		private const string END = "}}";
		private const string ASSIGN = "::";
		private const string PARM_USERID = "UserId";
		private const string PARM_USERPWD = "UserPwd";
		private const string PARM_USERIDSCANNED = "UserIdScanned";
		private const string PARM_BIOMETDATA = "BiometData";

		private string userId = "";
		private string userPwd = "";
		private bool userIdScanned = false;
		private string biometData = "";
	}
}
