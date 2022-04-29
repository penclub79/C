/*
	DVR SIO plug-in WTX1200 keyboard

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrP3SioWtx1200
#define	_EPC_GrP3SioWtx1200

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <P3/Sio/GrP3SioBase.h>
#include <P3/GrP3Mng.h>

	
//====================================================================
//constance

#define E_GrP3SioWtx1200RcvBufSize		5

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrP3SioWtx1200 : public Cls_GrP3SioPiBase
{
	protected:
		/*
		__u8	m_VdoCh;
		__u8	m_QuadMode;
		__u8	m_QuadPage;
		*/

		Ptr_GrP3MngGvs	m_PtrGvs;

		__u8	m_RecvBuf[E_GrP3SioWtx1200RcvBufSize];
		__u8	m_RecvIn;		//receive count
		__u8	m_RecvMax;	//packet size
		__u8	m_DirKeyIntv;
		__u8	m_TimerFz;		// zoom focus timer

		__u8	m_ChNum;			// channel number
		BOOL8	m_IsChIn;			// channel number input

		//local function
		void	LcParsPrtc( void );
		void	LcProcPtzCtrl( void );

	public:
    Cls_GrP3SioWtx1200( void* A_SioMng, __u8 A_Uart, __u8 A_Id );
    virtual		~Cls_GrP3SioWtx1200();

		virtual	BOOL8	RtlBkgProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


