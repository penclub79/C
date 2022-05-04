/*
	dvr external enviroment sensor

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrExtEsen
#define	_EPC_GrDvrExtEsen

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <GrTaskCli.h>
	
//====================================================================
//constance
#define E_GrDvrExtEsenRecvBufSize			0x10

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrExtEsenChItm 
{
	BOOL8	IsUse;			//use
	//BOOL8	IsReqed;		//requested
	//__u8	PassTime;		//pass time from request
	BOOL8	IsRcv;			//recive
	BOOL8	IsErr;			//error recived
	__u16	ValInt;			//revived value integer part
	__u8	ValMin;			//revived value minor part
}	*Ptr_GrDvrExtEsenChItm;


//====================================================================
//class

class	Cls_GrDvrExtEsen	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		St_GrDvrExtEsenChItm	m_ChTbl[E_GrDvrMaxChCnt];
		__u8		m_UartCh;
		BOOL8		m_IsOn;
		__u8		m_ChkCh;			//check channel
		BOOL8		m_IsReqed;		//requested
		__u8		m_TimeOut;
		BOOL8		m_IsChged;		//data changed

		__u32		m_RecvBuf[E_GrDvrExtEsenRecvBufSize >> 2];
		__u8		m_RecvSize;
		__u8		m_RecvMax;

	protected:

		//local
		void	LcSetup( __u8 A_UartCh, __u16 A_ChMap );
		void	LcRequest( __u8 A_Ch );
		BOOL8	LcProcRecv( void );
		void	LcUpdtValue( void );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	BOOL8	RtlBkgProc( void );
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrDvrExtEsen( Cls_GrTaskSvr* A_TaskSvr );
    virtual		~Cls_GrDvrExtEsen();

		void	Setup( __u8 A_UartCh, __u16 A_ChMap );
		BOOL8	ValueGet( __u8 A_Ch, __u16* A_PtrRtInt, __u8* A_PtrRtMin, BOOL8* A_PtrRtErr );
		BOOL8	IsChanged( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


