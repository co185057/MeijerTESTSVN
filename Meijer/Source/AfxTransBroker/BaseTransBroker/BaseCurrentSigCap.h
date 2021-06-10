///////////////////////////////////////////////////////////////////////////
///
/// \file BaseCurrentSigCap.h
/// \brief interface for the CBaseCurrentSigCap class.
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////////
#if !defined(AFX_BaseCurrentSigCap_H__INCLUDED_)
#define AFX_BaseCurrentSigCap_H__INCLUDED_

//////////////////////////////////////////////////////////////////////
///
/// CBaseCurrentSigCap is an implementaion of CBaseCurrentSigCap
///
///	@author: FastLane POSI
///
//////////////////////////////////////////////////////////////////////
class CBaseCurrentSigCap
{
public:
		//////////////////////////////////////////////////////////////////////
		///
		/// Destructor
		///
		//////////////////////////////////////////////////////////////////////
    virtual ~CBaseCurrentSigCap();

protected:
		//////////////////////////////////////////////////////////////////////
		///
		/// Constructor
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentSigCap();

private:
		//////////////////////////////////////////////////////////////////////
		///
		/// Hide copy const
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentSigCap(CBaseCurrentSigCap &);                    // hide copy const

		//////////////////////////////////////////////////////////////////////
		///
		/// Hide assignment
		///
		//////////////////////////////////////////////////////////////////////
    CBaseCurrentSigCap & operator =(const CBaseCurrentSigCap &); // hide assignment
};

#endif 
