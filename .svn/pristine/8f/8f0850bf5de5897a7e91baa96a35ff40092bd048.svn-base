using System;
using System.Runtime.InteropServices;

namespace DataPumpClient
{
	[Serializable]
	public struct MESG_HDR
	{
		public uint seq;
		public short type;
		public short subtype;
		public short svalue;
		public short version;
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.I1, SizeConst = 20)]
		public char [] computername;
	}

	[Serializable]
	public struct MESG_BODY
	{
		public int msgId;
		[MarshalAs(UnmanagedType.ByValArray, ArraySubType = UnmanagedType.I1, SizeConst = DpcMesg.MESG_SIZE)]
		public char [] text;
	}

	[Serializable]
	public struct MESG_DPC
	{
		public MESG_HDR hdr;
		public MESG_BODY msg;
	}

	public class DpcMesg
	{
		public const short MESG_SIZE = 250;

		public static MESG_DPC dpc_mesg;
		public static MESG_HDR msg_header;
		public static MESG_BODY msg_body;

		public static void InitStructs()
		{
			// Create Message Header struct
			msg_header.computername = new char [20];

			char[] compName = Environment.MachineName.ToCharArray();
			Array.Copy(compName, msg_header.computername, compName.Length); 

			// Create Message Body struct
			msg_body.text = new char[250];

			// Create the DPC Mesg struct
			dpc_mesg.hdr = msg_header;
			dpc_mesg.msg = msg_body;
		}

		public static byte[] Serialize()
		{
			// Get the byte size of a MyDataStruct structure if it is to be
			// marshaled to unmanaged memory.
			Int32 nSizeMesg_DPC = Marshal.SizeOf(typeof(MESG_DPC));

			// Allocate a byte array to contain the bytes of the unmanaged version
			// of the structure.
			byte[] byteArrayMesgDpcStruct = new byte[nSizeMesg_DPC];
			
			// Allocate a GCHandle to pin the byteArrayMesgDpcStruct array
			// in memory in order to obtain its pointer.
			GCHandle gch = GCHandle.Alloc(byteArrayMesgDpcStruct, GCHandleType.Pinned);
			
			// Obtain a pointer to the byteArrayMyDataStruct array in memory.
			IntPtr pbyteArrayMesgDpcStruct = gch.AddrOfPinnedObject();
			
			// Copy all bytes from the managed MyDataStruct structure into
			// the byte array.
			Marshal.StructureToPtr(dpc_mesg, pbyteArrayMesgDpcStruct, false);
			
			// Unpin the byteArrayMyDataStruct array in memory.
			gch.Free();
			
			// Return the byte array.
			// It contains the serialized bytes of the MyDataStruct structure.
			
			return byteArrayMesgDpcStruct;
		}

		public static void SetData(short type, string mesg)
		{
			dpc_mesg.hdr.type = type;

			Array.Clear(dpc_mesg.msg.text, '\0', dpc_mesg.msg.text.Length);

			char [] msg = mesg.ToCharArray();
			Array.Copy(msg, dpc_mesg.msg.text, msg.Length); 
		}
		
	}
}
