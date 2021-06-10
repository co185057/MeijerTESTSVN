using System;
using System.Net;
using System.Net.Sockets;
using System.Text;
using RPSWNET;

namespace DataPumpClient
{

	/// <summary>
	/// Summary description for DPUdpClient.
	/// </summary>
	public class DPUdpClient : IDisposable
	{
		public DPUdpClient(string IPAddress, int port)
		{
			_ipAdrs = IPAddress;
			_port = port;

			CmDataCapture.Capture( CmDataCapture.MaskError, 
				String.Format("DataPumpClient : IP = [{0}], Port = [{1}]", _ipAdrs, _port));
		}

		public bool Init()
		{
			bool bRet = false;

			try
			{
				// Create UDP Client
				_sock = new Socket(AddressFamily.InterNetwork, SocketType.Dgram, ProtocolType.Udp);
				IPAddress dataPumpSrvrIP = IPAddress.Parse(_ipAdrs);
				ep = new IPEndPoint(dataPumpSrvrIP, _port);

				// Initializing DPC Message structures.
				DpcMesg.InitStructs();

				bRet = true;
			}
			catch(Exception e)
			{
				CmDataCapture.Capture( CmDataCapture.MaskError, "DPUdpClient::Init() - EXCEPTION : " + e.Message );
				_sock = null;
			}

			return bRet;
		}

		public void SendMesg(short type, string mesg)
		{
			lock (sendLock)
			{
				if (null != _sock && mesg.Length > 0 && mesg.Length < DpcMesg.MESG_SIZE)
				{	
					CmDataCapture.Capture( CmDataCapture.MaskExtensive, "Send => " + mesg );
					DpcMesg.SetData(type, mesg);
					byte[] sendbuf = DpcMesg.Serialize();
					//_sock.SendTo(sendbuf, ep);

					// Send reduced length of the mesg since most of the space will contain 
					// zeros.
					int len = (sendbuf.Length - (DpcMesg.MESG_SIZE - mesg.Length + 1));
					_sock.SendTo(sendbuf, len , SocketFlags.None, ep);
				}
				else
				{
					CmDataCapture.Capture( CmDataCapture.MaskWarning, String.Format("Unable to send mesg: {0}", mesg));
				}
			}
		}

		public void Dispose()
		{
			CmDataCapture.Capture( CmDataCapture.MaskInfo, "DPUdpClient::Dispose()");
			if (null != _sock)
			{
				_sock.Close();
				_sock = null;
			}
		}

		private Socket _sock = null;
		
		private IPEndPoint ep;

		private int _port = -1;
		
		private string _ipAdrs = String.Empty;

		private Object sendLock = new Object();

		
	}
}
