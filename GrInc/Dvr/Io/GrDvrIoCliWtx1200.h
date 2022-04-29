/*
	dvr io client wtx1200a keyboard

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoCliWtx1200
#define	_EPC_GrDvrIoCliWtx1200

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
	
//====================================================================
//constance

#define E_GrDvrIoCliWtx1200RcvBufSize		5

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliWtx1200 : public Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_RecvBuf[E_GrDvrIoCliWtx1200RcvBufSize];
		__u8	m_RecvIn;		//recive count
		__u8	m_RecvMax;	//packet size
		__u8	m_DirKeyIntv;
		__u8	m_TimerFz;		// zoom focus timer

		__u8	m_ChNum;			// channel number
		BOOL8	m_IsChIn;			// channel number input

		//local function
		void	LcParsPrtc( void );

	public:
    Cls_GrDvrIoCliWtx1200( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliWtx1200();

		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


