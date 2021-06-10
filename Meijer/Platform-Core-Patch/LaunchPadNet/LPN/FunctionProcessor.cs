using System;

namespace LaunchPadNet
{
	/// <summary>
	/// Summary description for Function.
	/// </summary>
	public class FunctionProcessor
	{
		private String m_sOnEvent="";
		private String m_sExecuteOn="";
		private String m_sExecutableName="";
		private String m_sExecute="Start";
		
		public FunctionProcessor(String name)
		{
			this.m_sOnEvent =name;
		}
		
		public String OnEvent
		{			
			get
			{
				return m_sOnEvent;
			}			
			set
			{
				m_sOnEvent = value;
			}
		}
		
		public String ExecuteOn
		{			
			get
			{
				return m_sExecuteOn;
			}			
			set
			{
				m_sExecuteOn = value;
			}
		}

		public String Execute
		{			
			get
			{
				return m_sExecute;
			}			
			set
			{
				m_sExecute = value;
			}
		}

		public String ExecuteableName
		{			
			get
			{
				return m_sExecutableName;
			}			
			set
			{
				m_sExecutableName = value;
			}
		}
	}
}
