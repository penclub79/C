/*
	DVR SIO plug-in CJ1000 keyboard

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrP3SioPiCj1000
#define	_EPC_GrP3SioPiCj1000

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <P3/Sio/GrP3SioBase.h>
#include <P3/GrP3Mng.h>

	
//====================================================================
//constance

#define E_GrP3SioPiCj1000RcvBufSize		8

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrP3SioPiCj1000 : public Cls_GrP3SioPiBase
{
	protected:
		__u8	m_VdoCh;
		__u8	m_QuadMode;
		__u8	m_QuadPage;

		Ptr_GrP3MngGvs	m_PtrGvs;

		__u8	m_RecvBuf[E_GrP3SioPiCj1000RcvBufSize];
		__u8	m_RecvIn;		//receive count
		__u8	m_RecvMax;	//packet size
		__u8	m_DirKeyIntv;

		//local function
		void	LcParsPrtc( void );
		void	LcProcPtzCtrl( void );
		void	LcProcQuadKey( __u8 A_Num );

	public:
    Cls_GrP3SioPiCj1000( void* A_SioMng, __u8 A_Uart, __u8 A_Id );
    virtual		~Cls_GrP3SioPiCj1000();

		virtual	BOOL8	RtlBkgProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


