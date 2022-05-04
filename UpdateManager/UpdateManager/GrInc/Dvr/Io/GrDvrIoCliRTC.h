/*
	dvr io RTC BOX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrDvrIoCliRTC
#define	_EPC_GrDvrIoCliRTC

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Io/GrDvrIoCliBase.h>
#include <GrTimeTool.h>
	
//====================================================================
//constance

#define E_GrDvrIoCliRTCDevFcc				Mac_GrMakeFcc('E','R','T','C')

#define E_GrDvrIoCliRTCUpdtPriod		(E_GrTimeSecOfMin * 10)	/* 1 min */

#define E_GrDvrIoCliRTCRcvBufSize		8

//====================================================================
//global macro

//====================================================================
//global type


//====================================================================
//class

class	Cls_GrDvrIoCliRTC : public Cls_GrDvrIoCliBase
{
	protected:
		__u8	m_RecvBuf[E_GrDvrIoCliRTCRcvBufSize];
		__u8	m_RecvIn;		//receive count
		//BOOL8	m_IsReqSync;
		__u32	m_checktime;
		__u32	m_checktimeb4;
		__u32	m_SyncTimer;
		__u8	m_UartCh;
		BOOL8	m_IsCheckRtcBox;
		
		//local function
		void	LcSyncTime( void );
		__u32	LcTimeGet( void );
		__u8	EBCDIC( __u8 A_Code );		
	public:
    Cls_GrDvrIoCliRTC( __u8 A_UartCh, __u8 A_Id );
    virtual		~Cls_GrDvrIoCliRTC();
		
		virtual	BOOL8	RtlProc( void );
		virtual	void	RtlTimer( void );		// 1/10 sec timer
		virtual	void	TimeSet( Def_GrTime A_Time );
		virtual	void	CheckRtcBox( void );
		virtual	BOOL8  RetrnRtcBox( void );
};	

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


