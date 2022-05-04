/*
	Task server
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrTaskSvr
#define	_EPC_GrTaskSvr

//====================================================================
// option

//#define E_GrTaskSvrDbgApi			/* debuging api */
#define	E_GrTaskSvrSendPump			/* send pump style */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrProcess.h>
#include <GrTaskCli.h>
#include <GrBufTool.h>

//====================================================================
//constance

#define E_GrTaskSvrMsgBufSizeDflt				0x4000

#define E_GrTaskSvrMsgMaxSize						128

#define E_GrTaskSvrMaxCli								32
#define E_GrTaskSvrMaxTimer							16

#define	E_GrTaskSvrDfltMsgPump					2

#define E_GrTaskSvrPrioAlways						0				/* full load resource */
#define E_GrTaskSvrPrioGap0							1				/* task have rest 0ms at not work time */
#define E_GrTaskSvrPrioGap1							2				/* task have rest 10ms at not work time */
#define E_GrTaskSvrPrioGap2							3				/* task have rest 20ms at not work time */
#define E_GrTaskSvrPrioGap3							4				/* task have rest 30ms at not work time */
#define E_GrTaskSvrPrioTerm0						5				/* task have rest 0ms at any time slice */
#define E_GrTaskSvrPrioTerm1						6				/* task have rest 10ms at any time slice */
#define E_GrTaskSvrPrioTerm2						7				/* task have rest 20ms at any time slice */
#define E_GrTaskSvrPrioTerm3						8				/* task have rest 30ms at any time slice */
#define E_GrTaskSvrPrioQuat0						9				/* task have rest 20ms at every sec */
#define E_GrTaskSvrPrioQuat1						10			/* task have rest 250ms at every sec */
#define E_GrTaskSvrPrioQuat2						11			/* task have rest 500ms at every sec */
#define E_GrTaskSvrPrioQuat3						12			/* task have rest 750ms at every sec */


//====================================================================
//global macro


//====================================================================
//global type
typedef	struct St_GrTaskSvrTimerReg 
{
	__u32		AreaCnt;
	__u32		Priod;
	__u32		TickLast;
	Cls_GrTaskCli*	CliTbl[E_GrTaskSvrMaxCli];	
}	*Ptr_GrTaskSvrTimerReg;

/// message client register
typedef	struct St_GrTaskSvrMsgRegCli		
{
	St_GrTaskMsgHd	Hd;	
	__u32		Type;
	Cls_GrTaskCli*	ObjCli;
	__u32		Priod;
	__u32*	PtrRtHndl;
	BOOL8*		PtrRslt;
}	*Ptr_GrTaskSvrMsgRegCli;

/// message client unregister
typedef	struct St_GrTaskSvrMsgUnregCli
{
	St_GrTaskMsgHd	Hd;	
	__u32		Type;
	Cls_GrTaskCli*	ObjCli;
	__u32		Hndl;
	BOOL8*		PtrRslt;
}	*Ptr_GrTaskSvrMsgUnregCli;

//====================================================================
//class
class	Cls_GrTaskSvr
{
	private:
		// RTL
		BOOL8		m_IsAble;
		BOOL8		m_IsThrdRun;
		BOOL8		m_IsReqQuit;
		BOOL8		m_IsNewThrd;
		BOOL8		m_IsCliRun;
		St_GrPrcThrd	m_ThrdMng;
		St_GrPrcCritSct	m_Crit;			//critical section

		__s32		m_SvrId;						// task server id

		// priority
		__u32		m_Priority;
		__u32		m_SleepTime;

		// message 
		Ptr_GrBufVarQue	m_PtrMsgBuf;
		Cls_GrTaskCli*	m_MsgCliTbl[E_GrTaskSvrMaxCli];
		__u32		m_MsgCliAreaCnt;				//client register area count
		__u32		m_MsgPumpCnt;

		// background 
		Cls_GrTaskCli*	m_BkgCliTbl[E_GrTaskSvrMaxCli];
		__u32		m_BkgCliAreaCnt;

		// timer 
		St_GrTaskSvrTimerReg	m_TimerTbl[E_GrTaskSvrMaxTimer];
		__u32		m_TimerAreaCnt;

		// priority
		BOOL8		m_IsPrioRest;
		BOOL8		m_IsPrioGap;			// gap mode
		BOOL8		m_IsPrioQuat;
		__u32		m_PrioTick;
		__u32		m_PrioPriod;
		__u32		m_PrioBreak;			// break time

	protected:
		BOOL8	LcProcMsg( Ptr_GrTaskMsgHd A_PtrMsg );
		BOOL8	LcProcBkg( void );
		BOOL8	LcProcTimer( void );

		BOOL8	LcProcSvrMsg( Ptr_GrTaskMsgHd A_PtrMsg );		//server basic message process

		__u32	LcTimerIdxToHndl( __u32 A_PriodIdx, __u32 A_SlotIdx );
		void	LcTimerHndlToIdx( __u32 A_Hndl, __u32* A_PtrRtPriodIdx, __u32* A_PtrRtSlotIdx );
		void	LcPmsgRegCli( Ptr_GrTaskSvrMsgRegCli A_PtrMsg );
		void	LcPmsgUnregCli( Ptr_GrTaskSvrMsgUnregCli A_PtrMsg );
		BOOL8	LcRegCliMsg( Cls_GrTaskCli* A_Obj );
		BOOL8	LcUnregCliMsg( Cls_GrTaskCli* A_Obj );
		BOOL8	LcRegCliBkg( Cls_GrTaskCli* A_Obj );
		BOOL8	LcUnregCliBkg( Cls_GrTaskCli* A_Obj );
		BOOL8	LcRegCliTimer( Cls_GrTaskCli* A_Obj, __u32 A_Priod, __u32* A_PtrRtHndl );
		BOOL8	LcUnregCliTimer( __u32 A_Hndl );
		BOOL8	LcFindMsgTblEmpty( __u32* A_PtrRtIdx );
		void	LcMsgTblAreaUpdt( void );
		BOOL8	LcFindMsgTblByObj( Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx );
		BOOL8	LcFindBkgTblEmpty( __u32* A_PtrRtIdx );
		void	LcBkgTblAreaUpdt( void );
		BOOL8	LcFindBkgTblByObj( Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx );
		BOOL8	LcFindTmPdEmpty( __u32* A_PtrRtIdx );
		void	LcTmPdAreaUpdt( void );
		BOOL8	LcFindTmPdByPriod( __u32 A_Priod, __u32* A_PtrRtIdx );
		BOOL8	LcFindTmCliEmpty( __u32 A_PdIdx, __u32* A_PtrRtIdx );
		void	LcTmCliAreaUpdt( __u32 A_PdIdx );
		BOOL8	LcFindTmCliByObj( __u32 A_PdIdx, Cls_GrTaskCli* A_Obj, __u32* A_PtrRtIdx );

	public:
		Cls_GrTaskSvr( __u32 A_Fcc = E_GrTaskFccAny, BOOL8 A_IsNewThrd = TRUE, __u32 A_MsgBufSize = E_GrTaskSvrMsgBufSizeDflt );
		virtual	~Cls_GrTaskSvr();

		// information
		__u32	Fcc( void );
		Def_GrPrcThrdId	ThreadId( void );
		BOOL8	IsRun( void );		// now status run

		// maintenance
		BOOL8	DoStep( void );		//one setup process
		void	Run( void );			//task message loop with break time
		void	PrioritySet( __u32 A_Priority );
		void	ReqQuit( void );
		void	CliRunSet( BOOL8 A_IsRun );
		void	MsgPumpCntSet(__u32 A_Cnt);

		//message client register
		BOOL8	RegistMsgClient( Cls_GrTaskCli* A_Obj );
		BOOL8	UnregistMsgClient( Cls_GrTaskCli* A_Obj );

		//background client register
		BOOL8	RegistBkgClient( Cls_GrTaskCli* A_Obj );
		BOOL8	UnregistBkgClient( Cls_GrTaskCli* A_Obj );

		//timer client register
		BOOL8	RegistTimerClient( Cls_GrTaskCli* A_Obj, __u32 A_Priod, __u32* A_PtrRtHndl );
		BOOL8	UnregistTimerClient( __u32 A_Hndl );

		//message
#ifdef E_GrTaskSvrDbgApi
		BOOL8	MsgSend(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsDbg = FALSE);
#else
		BOOL8	MsgSend( Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size );
#endif
		BOOL8	MsgPost( Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsFast = FALSE,BOOL8 A_ShowMsg = TRUE );
		BOOL8	MsgExPost( __u32 A_Cmd , void* A_PtrData = NULL, __u32 A_SizeData = 0 );

		//runtime - do not run user
		void	RtlRun( void );
		BOOL8	RtlMsgPump(void);

};

//====================================================================
//global function

	
//====================================================================
//out Environment conditional
#endif
