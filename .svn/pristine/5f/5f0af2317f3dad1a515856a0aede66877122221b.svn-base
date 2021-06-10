// JScript source code

// test the COM objects

var obj;
var genobj;


function tbConnect_ReceiveTBGenObj( objgen )
{
   WScript.Echo(objgen.xml);
//   WScript.Echo(objgen.GetProperty("ItemDescription"));
}

try
{
	// create objects and use functions that start with "tbConnect_" as events
	obj = WScript.CreateObject("tbControls.tbConnect", "tbConnect_"); 
	genobj = WScript.CreateObject("tbControls.TBGenObj");
	
	genobj.IsXMLOutput=true;
	genobj.SetProperty("ItemDescription", "my Item");
}
catch(e)
{
	WScript.Echo("Failed to create object");
	WScript.Echo(e);
}

try
{
    obj.SubscribeTo("A0");
	obj.Connect();

	genobj.MsgId=0xA0;	
	obj.Send(genobj);
	WScript.Echo("Sent.");
}
catch(e)
{
	WScript.Echo("Error");
}

