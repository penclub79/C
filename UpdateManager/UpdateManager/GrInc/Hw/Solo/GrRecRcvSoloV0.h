/*
	solo record data recive
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrRecRcvSoloV0
#define	_EPC_GrRecRcvSoloV0

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>

#include	<Dvr/GrDvrBase.h>
#include	<GrTaskSvr.h>
#include	<GrTaskCli.h>
#include	<Hw/Solo/GrDevSolo.h>
#include	<GrBufTool.h>

#include	<Dvr/Fs/V1Old/GrDvrFsoDsk.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoMng.h>

//====================================================================
//constance
#define E_GrRecRcvSoloV0SyncCnt				5

#define E_GrRecRcvIposHisCnt				2		/* i frame history */

#define E_GrRecRcvVssRsvBufSize			0x800		/* vss reserve queue buffer size */

#define E_GrRecRcvMtnPluginCnt			2
#define E_GrRecRcvMtnCallbackCmd		Mac_GrMakeFcc('G','M','N','T')			/* motion callback command */


//====================================================================
//global macro


//====================================================================
//global type

typedef	struct  St_GrRecRcvSoloV0AdoPkt
{
	St_GrDvrAvHd	Hd;
	__u8		Data[E_GrDrvSoloAdoClustSize];
	__u32		_rsvd[7];		//32 byte align pad
	__u32		TailMark;		//reverse tail mark
} *Ptr_GrRecRcvSoloV0AdoPkt;

typedef	struct St_GrRecRcvSoloV0AdoSyncCh
{
	__u64		HndlNrm;
	__u64		HndlMir;
}	*Ptr_GrRecRcvSoloV0AdoSyncCh;

typedef	struct St_GrRecRcvSoloV0AdoSyncItm
{
	Def_GrTime	TimeSec;
	__u32		TimeMsec;
	BOOL8		IsSm;				//summer time
	St_GrRecRcvSoloV0AdoSyncCh	ChTbl[E_SoloMaxChCnt];
}	*Ptr_GrRecRcvSoloV0AdoSyncItm;

typedef	struct St_GrRecRcvSoloV0VssRsv 
{
	__u64	Hndl;
	Def_GrTime Time;
	__u8	Cmd;
	BOOL8	IsArmed;
	__u8	Etty;
	__u8	Seq;
	__u8	FrmCnt;
	__u8	ProcCnt;		//process frame count
	__u8	_rsvd[2];
}	*Ptr_GrRecRcvSoloV0VssRsv;

typedef	struct St_GrRecRcvSoloV0VssHis 
{
	__u64	Hndl;			// i frame handle
	__u8	PfrmCnt;	// p frame count
}	*Ptr_GrRecRcvSoloV0VssHis;

typedef struct St_GrRecRcvSoloV0VssCh
{
	__u8	IhndlIdx;		// i frame handle index
	Def_GrTime	Time;	// last time
	St_GrRecRcvSoloV0VssHis	His[E_GrRecRcvIposHisCnt];
	//St_GrRecRcvSoloV0VssRsv	Rsv;		//reserved
}	*Ptr_GrRecRcvSoloV0VssCh;

typedef	struct St_GrRecRcvMtnCbMsg
{
	St_GrTaskMsgHd	Hd;
	__u8	Ch;
	__u8	Scale;
	void*	PtrMap;		//motion map
}	*Ptr_GrRecRcvMtnCbMsg;

//====================================================================
//class

class Cls_GrRecRcvSoloV0	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*		m_TaskSvr;
		Cls_GrDevSolo*		m_ObjDev;

		__u32		m_HndlTimer;
		Cls_GrDvrFsoMng*	m_FsMng;
		__u32		m_FccStrm;

		Ptr_GrBufRefQue		m_PtrRecQue;		//record queue
		Def_GrTime	m_TimeSecLast;			//last sec
		__u32		m_TimeMsecLast;			//last msec
		BOOL8		m_IsSmLast;					//last time summer time

		__u32		m_VdoChCnt;
		BOOL8		m_VdoAbleTbl[E_GrDvrMaxChCnt];		//video able table
		__u8		m_VopIdTbl[E_GrDvrMaxChCnt];			//vop id table
		__u8		m_SeqIdTbl[E_GrDvrMaxChCnt];			//sequence id table
		__u8		m_EvtTbl[E_GrDvrMaxChCnt];				//last event

		BOOL8		m_IsProced;				//last frame is processed
		void*		m_PtrNewest;			//last worked newest frame

		__u32		m_PrefrmTick;				//preframe time tick unit
		__u32		m_BufLimite;				//limite size

		__u32		m_AdoChCnt;					//audio channel count
		__u32		m_AdoCpAdd;					//audio copy add
		__u32		m_AdoRecMap;				//audio record map
		St_GrRecRcvSoloV0AdoPkt	m_AdoPktBuf;
		St_GrRecRcvSoloV0AdoSyncItm		m_AdoSyncTbl[E_GrRecRcvSoloV0SyncCnt];		//audio sync table
		__s32			m_AdoSyncIdx;		//sync index

		__u32		m_MtnMapSize;					//hardware motion map size
		Cls_GrTaskCli*	m_MtnPlugTbl[E_GrRecRcvMtnPluginCnt];		//motion map plugin table

		//test
		__u32		m_RtlFrmSizeTbl[E_GrDvrMaxChCnt];
		__u32		m_VdoFrmSizeTbl[E_GrDvrMaxChCnt];

		//vss
		BOOL8		m_IsVssUse;	
		St_GrRecRcvSoloV0VssCh	m_VssTbl[E_GrDvrMaxChCnt];		//vss
		__u8		m_VssQue[E_GrDvrMaxChCnt][E_GrRecRcvVssRsvBufSize];					//vss reserve queue

	protected:
		//local
		BOOL8	LcProcVdoFrame( void* A_PtrFrm, BOOL8 A_IsPreFlush );
		void	LcProcAdoFrame( void* A_PtrFrm, BOOL8 A_IsPreFlush );
		BOOL8	LcProcNewestFrame( void );	//process newest frame
		void	LcFlushPreframes( void );
		void	LcProcLastFrame( void );			//lastest frame operation
		void	LcProcMtnPlugin( void* A_PtrFrm );
		void	LcMarkEvtPreframes( __u8 A_Ch, __u32 A_Evt );

		//inherited
		virtual	void	RtlTimer( __u32 A_Hndl );
		virtual	BOOL8	RtlBkgProc( void );											/* return = background processed */
	public:
		Cls_GrRecRcvSoloV0( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, Cls_GrDvrFsoMng* A_FsMng,
			BOOL8 A_IsUseVss	);
		virtual	~Cls_GrRecRcvSoloV0();

		//control
		void		PreframeTimeSet( __u32 A_Sec );
		void		AdoRecMapSet( __u32 A_Map );
		void		MotionPluginSet( __u8 A_Idx, Cls_GrTaskCli* A_ObjPlugin );
		__u32		VdoFrmAverageSizeGet( __u8 A_Ch );
		BOOL8		VssAdd( __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, __u8 A_Etty, __u8 A_CstFrmCnt );
};


//====================================================================
//global function
	
//====================================================================
//out Enviroment conditional
#endif
