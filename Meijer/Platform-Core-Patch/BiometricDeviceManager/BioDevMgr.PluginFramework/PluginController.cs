using System;
using System.Collections.Generic;
using System.Text;

namespace BioDevMgr
{
    /// <summary>
    /// Summary description for PluginController.
    /// </summary>
    public abstract class PluginController : Plugin
    {
        protected PluginController()
		{
			//
			// prevent instantiation 
			//
		}
        
        protected override void OnPluginEvent(PluginEventArgs e)
		{
			// Do any Connector specific processing here.

			// Call the base class event invocation method.
			base.OnPluginEvent(e);
		}


    }
}
