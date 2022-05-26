/*
	dvr io client CJ1000 keyboard

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoCliCj1000
#define	_EPC_GrDvrIoCliCj1000

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
	
//====================================================================
//constance

#define E_GrDvrIoCliCj1000RcvBufSize		8

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliCj1000 : public Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_RecvBuf[E_GrDvrIoCliCj1000RcvBufSize];
		__u8	m_RecvIn;		//recive count
		__u8	m_RecvMax;	//packet size
		__u8	m_DirKeyIntv;

		//local function
		void	LcParsPrtc( void );
		void	LcProcQuadKey( __u8 A_Num );

	public:
    Cls_GrDvrIoCliCj1000( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliCj1000();

		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


