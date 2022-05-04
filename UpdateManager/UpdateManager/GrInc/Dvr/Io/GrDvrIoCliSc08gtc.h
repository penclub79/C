/*
	dvr io client wtx1200a keyboard

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrIoCliSc08gtc
#define	_EPC_GrDvrIoCliSc08gtc

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
#include <GrTimeTool.h>
	
//====================================================================
//constance

#define E_GrDvrIoCliSc08gtcDevFcc				Mac_GrMakeFcc('S','C','0','8')

//#define E_GrDvrIoCliSc08gtcUpdtPriod		(E_GrTimeSecOfHour * 10)	/* 1 hour */
#define E_GrDvrIoCliSc08gtcUpdtPriod		(E_GrTimeSecOfMin * 10)	/* 1 min */

#define E_GrDvrIoCliSc08gtcRcvBufSize		15

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliSc08gtc : public Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_RecvBuf[E_GrDvrIoCliSc08gtcRcvBufSize];
		__u8	m_RecvIn;		//receive count
		//BOOL8	m_IsReqSync;
		__u32	m_SyncTimer;
		__u32	m_checktime;
		__u32	m_checktimeb4;
		BOOL8	m_IsCheckGPS;

		//local function
		void	LcSyncTime( void );
		__u32	LcTimeGet( void );
	public:
    Cls_GrDvrIoCliSc08gtc( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliSc08gtc();

		virtual	void	UserAction( __u32 A_ActId );

		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
		virtual	BOOL8	RetrnGPS( void );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


