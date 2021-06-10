//////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE:  SMStateTB.h
//
//////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _SMSTATETB
#define _SMSTATETB
        public:
	        virtual USHORT TBGetKeyCode(const int nKey);
	        virtual long   TBGetKeyLabel(const int nKey,CString &csLabel);
	        virtual USHORT *TBEmulatorKeys(void);
	        virtual void   TBLoadAssistModeKeyboard(void);
	protected:
		static USHORT m_nAssistKeyVirtualKeyValue[200];				  // key Virtual Key Value


#endif
