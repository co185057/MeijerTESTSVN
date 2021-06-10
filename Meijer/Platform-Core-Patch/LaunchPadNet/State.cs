using System;
using PsxNet;
using CommonLibsNet;

namespace LaunchPadNet
{
	/// <summary>
	/// The state base class.
	/// </summary>
	internal abstract class State : IDisposable
	{
		/// <summary>
		/// The class constructor.
		/// </summary>
		/// <param name="paramPsx">
		/// SharedPsx instance used to connect.
		/// </param>
		public State( SharedPsx paramPsx )
		{
			myPsx = paramPsx;
			if( null != myPsx.Scanner )
			{
				myPsx.Scanner.ScannerEvent += new ScannerEventHandler( State_ScannerEvent );
			}
		}

		/// <summary>
		/// The class destructor.
		/// </summary>
		~State()
		{
			Dispose();
		}

		/// <summary>
		/// Releases the resources used by the State instance.
		/// </summary>
		public virtual void Dispose()
		{
			if( null != myPsx.Scanner )
			{
				myPsx.Scanner.ScannerEvent -= new ScannerEventHandler( State_ScannerEvent );
			}
			GC.SuppressFinalize( this );
		}
		
		/// <summary>
		/// State Psx event handler.
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A PsxEventArgs that contains the event data.
		/// </param>
		/// <returns>
		/// Return True if we handle the event; otherwise, it should return False.
		/// </returns>
		public abstract bool State_PsxEvent( object sender, PsxEventArgs e );		

		/// <summary>
		/// Occurs when Scanner fires an event.
		/// </summary>
		/// <param name="sender">
		/// The source of the event.
		/// </param>		
		/// <param name="e">
		/// A ScannerEventArgs that contains the event data.
		/// </param>
		protected abstract void State_ScannerEvent( object sender, ScannerEventArgs e );

		/// <summary>
		/// Psx instance.
		/// </summary>
		protected SharedPsx myPsx;
	}
}
