//////////////////////////////////////////////////////////////////////////////////////////////////
//
//	Class Name:		DMMessageExt
//
//  Filename:		DMMessageExtension.h
//
//  Description:	This class encapulates the current SCOT Message Sending feature.
//
//	Message System:	SCOT Platform's Paging System (Currently)
//
//  Author:			John Z. Bush
//
//	Date:			March 23, 1999
//
//	Copyright:		NCR Corporation (c) 1999
//
//////////////////////////////////////////////////////////////////////////////

#ifndef _DMMessageExt
#define _DMMessageExt

class DMMessageExt
{
public:
	// Constructor
	DMMessageExt();
    // Destructor
    ~DMMessageExt(); //tar 229591
    // Availability of Message System Function
	bool IsAvailable();
	// Message System's Send Message Function
	bool Send( LPCTSTR Message);
    // Set Maximum Counter value for reinitializing the availiblity of the paging system    // TAR 117805
    void SetReinitializeMaximum( int MaxCount) {iReinitializeMax = MaxCount;}               // TAR 117805
private:
	bool bPagerInstalled, bInitialized;
    int  iReinitializeCount, iReinitializeMax;                                              // TAR 117805
	DMMessageExt(const DMMessageExt&);				// Blocking Definition
	DMMessageExt& operator= (const DMMessageExt&);	// Blocking Definition
};

#endif