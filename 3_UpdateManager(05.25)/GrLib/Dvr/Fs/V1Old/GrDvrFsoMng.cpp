/*
 file system version 1

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoMng.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<Dvr/Fs/V1Old/GrDvrFsoDsk.h>

//====================================================================
//local const
#define E_GrDvrFsoMngCmdFormat			0x00
#define E_GrDvrFsoMngCmdTmaRead			0x01
#define E_GrDvrFsoMngCmdRecStat			0x02		/* record status get St_GrDvrFsoMngMsgRecStat */
#define E_GrDvrFsoMngCmdSetup				0x03		/* record status get St_GrDvrFsoMngMsgSetup */
#define E_GrDvrFsoMngCmdUcaWrite		0x04		/* user configuration area write : St_GrDvrFsoMngMsgUcaRw */
#define E_GrDvrFsoMngCmdUcaRead			0x05		/* user configuration area read : St_GrDvrFsoMngMsgUcaRw */
#define E_GrDvrFsoMngCmdUwaWrite		0x06		/* user work area write : St_GrDvrFsoMngMsgUwaRw */
#define E_GrDvrFsoMngCmdUwaRead			0x07		/* user work area read : St_GrDvrFsoMngMsgUwaRw */
#define E_GrDvrFsoMngCmdTimeRng			0x08		/* record time range get : St_GrDvrFsoMngMsgTimeRng */
#define E_GrDvrFsoMngCmdLogAdd			0x09		/* log add : St_GrDvrFsoMngMsgLogAdd */
#define E_GrDvrFsoMngCmdLogGet			0x0A		/* log get : St_GrDvrFsoMngMsgLogGet */
#define E_GrDvrFsoMngCmdHndlBegin		0x0B		/* begin handle get : St_GrDvrFsoMngMsgHndlGet */
#define E_GrDvrFsoMngCmdHndlEnd			0x0C		/* end handle get : St_GrDvrFsoMngMsgHndlGet */
#define E_GrDvrFsoMngCmdHndlMinute	0x0D		/* handle by minute : St_GrDvrFsoMngMsgHndlByTime */
#define E_GrDvrFsoMngCmdReadNormal	0x0E		/* read normal : St_GrDvrFsoMngMsgReadPara */
#define E_GrDvrFsoMngCmdReadBack		0x0F		/* read back : St_GrDvrFsoMngMsgReadPara */
#define E_GrDvrFsoMngCmdHndlFindFw	0x10		/* handle by near minute : St_GrDvrFsoMngMsgHndlByRng */
#define E_GrDvrFsoMngCmdAtdSet			0x11		/* auto delete set : St_GrDvrFsoMngMsgAtdSet */
#define E_GrDvrFsoMngCmdTempGet			0x12		/* temperature get : St_GrDvrFsoMngMsgTempGet */
#define E_GrDvrFsoMngCmdHndlFindBw	0x13		/* handle by near minute dir backward : St_GrDvrFsoMngMsgHndlByRng */
#define E_GrDvrFsoMngCmdDataSize		0x14		/* calculate recorded data size : St_GrDvrFsoMngMsgDataSize */
#define E_GrDvrFsoMngCmdVssGet			0x15		/* vss data get : St_GrDvrFsoMngMsgVssGet */
#define E_GrDvrFsoMngCmdVssPop			0x16		/* vss queue pop : St_GrDvrFsoMngMsgBase */
#define E_GrDvrFsoMngCmdVssAdd			0x17		/* vss item add : St_GrDvrFsoMngMsgVssAdd */
#define E_GrDvrFsoMngCmdVssReset		0x18		/* vss queue reset : St_GrDvrFsoMngMsgBase */
#define E_GrDvrFsoMngCmdMainAllErr	0x19		/* main disk all error : St_GrDvrFsoMngMsgMainAllErr */
#define E_GrDvrFsoMngCmdDayMapGet		0x1A		/* main disk all error : St_GrDvrFsoMngMsgDayMapGet */
#define E_GrDvrFsoMngCmdReadPrev		0x1B		/* read previous : St_GrDvrFsoMngMsgReadPara */

//====================================================================
//local type

typedef	struct St_GrDvrFsoMngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFsoMngMsgBase;

typedef	struct St_GrDvrFsoMngMsgFormat 
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DskIdx;
	BOOL8	IsWithoutLog;
	BOOL8*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgFormat;

typedef	struct St_GrDvrFsoMngMsgTmaRead
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8 DevType;
	Def_GrTime Time;
	void* PtrBuf;
	__u32 Cnt;
	BOOL8*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgTmaRead;

typedef	struct St_GrDvrFsoMngMsgRecStat
{
	St_GrDvrFsoMngMsgBase	Hd;
	void*	PtrBuf;
}	*Ptr_GrDvrFsoMngMsgRecStat;

typedef	struct St_GrDvrFsoMngMsgSetup 
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8*		PtrDskTbl;
	__u8		DiskCnt;
	BOOL8		IsNormalOvwt;
	BOOL8		IsMirrorOvwt;
}	*Ptr_GrDvrFsoMngMsgSetup;

typedef	struct St_GrDvrFsoMngMsgUcaRw 
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u32		Pos;
	void*		PtrBuf;
	__u32		Size;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFsoMngMsgUcaRw;

typedef	struct St_GrDvrFsoMngMsgUwaRw 
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u64		Pos;
	void*		PtrBuf;
	__u32		Size;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFsoMngMsgUwaRw;

typedef	struct St_GrDvrFsoMngMsgTimeRng
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8		DevType;
	Def_GrTime*	PtrTimeSt;
	Def_GrTime*	PtrTimeEd;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFsoMngMsgTimeRng;

typedef	struct St_GrDvrFsoMngMsgLogAdd
{
	St_GrDvrFsoMngMsgBase	Hd;
	St_GrDvrLogItm	Log;
}	*Ptr_GrDvrFsoMngMsgLogAdd;

typedef	struct St_GrDvrFsoMngMsgLogGet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u32	EvtMask;
	__u32	ChMask;
	Ptr_GrDvrLogItm	PtrRtLog;
	__u32	MaxCnt;
	__u32*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgLogGet;

typedef	struct St_GrDvrFsoMngMsgHndlGet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	__u64*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgHndlGet;

typedef	struct St_GrDvrFsoMngMsgHndlByTime
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	Time;
	__u64*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgHndlByTime;

typedef	struct St_GrDvrFsoMngMsgReadPara
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	__u64 Hndl;
	void* PtrBuf;
	__u32 Size;
	__u32* PtrRtSize;
	__u64* PtrRtCrntHndl;
	__u8*	PtrRslt;
}	*Ptr_GrDvrFsoMngMsgReadPara;

typedef	struct St_GrDvrFsoMngMsgHndlByRng
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgHndlByRng;

typedef	struct St_GrDvrFsoMngMsgAtdSet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u32	DayCnt;
}	*Ptr_GrDvrFsoMngMsgAtdSet;

typedef	struct St_GrDvrFsoMngMsgTempGet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DskIdx;
	__u8*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgTempGet;

typedef	struct St_GrDvrFsoMngMsgDataSize
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u8	DevType;
	__u16	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRt;
}	*Ptr_GrDvrFsoMngMsgDataSize;

typedef	struct St_GrDvrFsoMngMsgVssGet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u64*	PtrRtHndlRec;		//record handle
	Def_GrTime*	PtrRtTime;	//armed time
	__u8*	PtrRtCh;
	__u8*	PtrRtCmd;
	BOOL8*	PtrRtIsArmed;
	__u8*	PtrRtEtty;
	__u8*	PtrRtSeq;
	__u8*	PtrRtFrmCnt;
	BOOL8*	PtrRtRslt;
}	*Ptr_GrDvrFsoMngMsgVssGet;

typedef	struct	St_GrDvrFsoMngMsgVssAdd
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u64	Hndl;				//record handle
	Def_GrTime	Time;	//armed time
	__u8	Ch;
	__u8	Cmd;
	BOOL8	IsArmed;
	__u8	Etty;
	__u8	Seq;
	__u8	FrmCnt;
}	*Ptr_GrDvrFsoMngMsgVssAdd;

typedef	struct	St_GrDvrFsoMngMsgMainAllErr
{
	St_GrDvrFsoMngMsgBase	Hd;
	BOOL8*	PtrRtRslt;
}	*Ptr_GrDvrFsoMngMsgMainAllErr;

typedef	struct	St_GrDvrFsoMngMsgDayMapGet
{
	St_GrDvrFsoMngMsgBase	Hd;
	__u32*	PtrRtRslt;
	__u8		DevType;
	Def_GrTime	TimeDay;
	__u32		DayCnt;
}	*Ptr_GrDvrFsoMngMsgDayMapGet;


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFsoMng::Cls_GrDvrFsoMng( Cls_GrTaskSvr* A_TaskSvr, __u32 A_SysId, __u32 A_FccStrm, __u32 A_TmaCnt, 
																	BOOL8 A_IsVssUse, BOOL8 A_IsInstReplay, __u64 A_UwaSize, __u32 A_VssSize ):
Cls_GrTaskCli()
{
	// local -------------------
		__u8	Tv_Wkidx;
	// code --------------------

		//check struct size current
		if ( sizeof(St_GrDvrFsoMis) > E_GrDvrFsoMisSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad mis structur size = %d \n", sizeof(St_GrDvrFsoMis) );
		}
		if ( sizeof(St_GrDvrFsoMca) > E_GrDvrFsoMcaSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad mca structur size = %d \n", sizeof(St_GrDvrFsoMca) );
		}
		if ( sizeof(St_GrDvrFsoRca) > E_GrDvrFsoRcaItmSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad rca structur size = %d \n", sizeof(St_GrDvrFsoRca) );
		}
		if ( sizeof(St_GrDvrRecMinThumb) != E_GrDvrRecMinThumbItemSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad tma structur size = %d \n", sizeof(St_GrDvrRecMinThumb) );
		}
		if ( sizeof(St_GrDvrLogItm) != E_GrDvrFsoElaItmSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad ela structur size = %d \n", sizeof(St_GrDvrLogItm) );
		}
		/*
		if ( sizeof(St_GrDvrFsoVssItm) != 32 )
		{
			DbgMsgPrint( "dvr fs ver 1::bad vss structur size = %d \n", sizeof(St_GrDvrFsoVssItm) );
		}
		*/

		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		//setup data
		GrDumyZeroMem( &m_DskCfg, sizeof(m_DskCfg) );
		m_DskCfg.SysId			=	A_SysId;
		m_DskCfg.FccStrm		=	A_FccStrm;
		//m_DskCfg.TimeBase		=	A_TimeBase;
		m_DskCfg.IdaSize		=	0;
		if ( A_IsInstReplay )
		{
			m_DskCfg.IdaSize	=	E_GrDvrFsoRdaAlignUnit;
		}
		m_DskCfg.UwaSize	=	A_UwaSize;
		m_VssSize					=	A_VssSize;
		m_AutoDelTime			=	0;
		m_AtdLimTime			=	0;
		m_IsDiskFull			=	FALSE;
		m_IsReced					=	FALSE;
		m_IsRecAble				=	FALSE;
		m_IsVssAble				=	FALSE;
		m_IsWdgTick				=	FALSE;
		m_MissMap					=	0;
		m_NotFmtMap				=	0;

		//reset object
		for ( Tv_Wkidx=0;E_GrDvrFsoCfgDskModeCnt > Tv_Wkidx;Tv_Wkidx++ )
		{
			m_GrpTbl[Tv_Wkidx]	=	NULL;
		}
		
		m_IsVssUse		=	FALSE;
		m_PtrVssQue		=	NULL;
		if ( A_IsVssUse )
		{
			m_IsVssUse		=	TRUE;
			m_PtrVssQue		=	(Ptr_GrBufQue)malloc( m_VssSize );
		}

		//regist task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFsoFlushPeriod, &m_HndlTimer );

}
//--------------------------------------------------------------------
Cls_GrDvrFsoMng::~Cls_GrDvrFsoMng()
{
		//unregister timer
		m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );

		LcFreeAllObject();

		if ( NULL != m_PtrVssQue )
		{
			free( m_PtrVssQue );
		}

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcFreeAllObject( void )
{
	// local -------------------
		__u8	Tv_Wkidx;
	// code --------------------
		//stop all group
		for ( Tv_Wkidx=0;E_GrDvrFsoCfgDskModeCnt > Tv_Wkidx;Tv_Wkidx++ )
		{
			if ( NULL != m_GrpTbl[Tv_Wkidx] )
			{
				delete	m_GrpTbl[Tv_Wkidx];
				m_GrpTbl[Tv_Wkidx]	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcSetup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt )
{
	// local -------------------
		__u8	Tv_Cnt;
		__u8	Tv_Wkidx;
		BOOL8	Tv_IsOk;
		__u32	Tv_DskMode;
		BOOL8	Tv_IsOvrt[E_GrDvrFsoCfgDskModeCnt];
		Cls_GrDvrFsoDsk*	Tv_ObjDsk;
		__u32	Tv_BufSize;
	// code --------------------

		m_IsDiskFull	=	FALSE;		//reset status
		m_IsRecAble		=	FALSE;
		m_IsVssAble		=	FALSE;
		m_MissMap			=	0;
		m_NotFmtMap		=	0;
		//release all object
		LcFreeAllObject();

		//check exist hdd list
		if ( 0 != A_Cnt )
		{
			//build table
			Tv_Cnt	=	A_Cnt;
			if ( E_GrDvrFsoGrpMaxDiskCnt < Tv_Cnt )
			{
				Tv_Cnt	=	E_GrDvrFsoGrpMaxDiskCnt;
			}
			Tv_IsOvrt[E_GrDvrFsoCfgDskNormal]		=	A_IsNormalOvwt;
			Tv_IsOvrt[E_GrDvrFsoCfgDskMirror]		=	A_IsMirrorOvwt;
			Tv_IsOvrt[E_GrDvrFsoCfgDskHalt]			=	FALSE;
			for ( Tv_Wkidx=0;Tv_Cnt > Tv_Wkidx;Tv_Wkidx++ )
			{
				//check mode
				Tv_DskMode	=	A_PtrDskTbl[Tv_Wkidx];
				if ( E_GrDvrFsoCfgDskNone > Tv_DskMode )
				{
					//create disk
					Tv_ObjDsk	=	(Cls_GrDvrFsoDsk*)new Cls_GrDvrFsoDsk( Tv_Wkidx, &m_DskCfg, Tv_IsOvrt[Tv_DskMode], (void*)m_LogRdBuf );
					//check exist disk
					if ( 0 != Tv_ObjDsk->DiskSizeGet() )
					{
						//patch disk mode
						if ( !Tv_ObjDsk->IsFormatted() )
						{
							m_NotFmtMap	=	m_NotFmtMap | (1 << Tv_Wkidx);
							Tv_DskMode	=	E_GrDvrFsoCfgDskHalt;
						}
						//check exist group
						if ( NULL == m_GrpTbl[Tv_DskMode] )
						{
							//create group
							m_GrpTbl[Tv_DskMode]	=	(Cls_GrDvrFsoGrp*)new Cls_GrDvrFsoGrp( (__u8)Tv_DskMode, Tv_IsOvrt[Tv_DskMode] );
						}
						//add disk
						m_GrpTbl[Tv_DskMode]->DiskAdd( Tv_ObjDsk );
						if ( E_GrDvrFsoCfgDskHalt > Tv_DskMode )
						{
							//mirror or normal hdd exist
							m_IsRecAble	=	TRUE;
						}
					}
					else
					{
						// miss map
						if ( E_GrDvrFsoCfgDskHalt > Tv_DskMode )
						{
							m_MissMap	=	m_MissMap | ( 1 << Tv_Wkidx );
						}
						//release disk
						delete	Tv_ObjDsk;
					}
				}
			}
			//setup finished , init
			for ( Tv_Wkidx=0;Tv_Wkidx <= E_GrDvrFsoCfgDskMirror;Tv_Wkidx++ )
			{
				//check exist
				if ( NULL != m_GrpTbl[Tv_Wkidx] )
				{
					m_GrpTbl[Tv_Wkidx]->DiskInit();
				}
			}
			//check vss able
			if ( m_IsVssUse && (NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal]) )
			{
				if ( m_VssSize <= m_GrpTbl[E_GrDvrFsoCfgDskNormal]->UwaSizeGet() )
				{
					m_IsVssAble	=	TRUE;
					//load vss data
					Tv_IsOk		=	FALSE;
					if ( m_GrpTbl[E_GrDvrFsoCfgDskNormal]->IsVssSaved() )
					{
						if ( m_GrpTbl[E_GrDvrFsoCfgDskNormal]->UwaRead( 0, (void*)m_PtrVssQue, m_VssSize ) )
						{
							DbgMsgPrint( "Cls_GrDvrFsoMng::LcSetup - vss data loaded\n" );
							//check data able
							Tv_BufSize	=	m_VssSize - (m_VssSize % sizeof(St_GrDvrFsoVssItm));
							if ( (Tv_BufSize == (__u32)m_PtrVssQue->BufSize) && (sizeof(St_GrDvrFsoVssItm) == m_PtrVssQue->SizeItem) &&
								(m_VssSize > (__u32)m_PtrVssQue->InPos) && (m_VssSize > (__u32)m_PtrVssQue->OutPos)
								)
							{
								Tv_IsOk	=	TRUE;
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFsoMng::LcSetup - vss data load failure\n" );
						}
					}
					if ( !Tv_IsOk )
					{
						//reset queue
						GrBufQueInit( m_PtrVssQue, sizeof(St_GrDvrFsoVssItm), m_VssSize );
						DbgMsgPrint( "Cls_GrDvrFsoMng::LcSetup - vss data reset.\n" );
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::PacketReserve( __u32 A_Size, __u64* A_PtrRtHndlNrm, __u64* A_PtrRtHndlMir )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtHndlNrm	=	E_GrDvrFsoHndlNone;
		*A_PtrRtHndlMir	=	E_GrDvrFsoHndlNone;
		//normal
		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal] )
		{
			*A_PtrRtHndlNrm	=	m_GrpTbl[E_GrDvrFsoCfgDskNormal]->PacketReserve( A_Size );
			if ( E_GrDvrFsoHndlNone != *A_PtrRtHndlNrm )
			{
				Tv_Result	=	TRUE;
			}
		}
		//mirror
		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskMirror] )
		{
			*A_PtrRtHndlMir	=	m_GrpTbl[E_GrDvrFsoCfgDskMirror]->PacketReserve( A_Size );
			if ( E_GrDvrFsoHndlNone != *A_PtrRtHndlMir )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//normal
		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal] )
		{
			if ( m_GrpTbl[E_GrDvrFsoCfgDskNormal]->PacketAdd( A_PtrPkt, A_PtrRtHndl ) )
			{
				m_IsReced	=	TRUE;
				m_IsWdgTick	=	TRUE;
				Tv_Result	=	TRUE;
			}
			else
			{
				if ( m_GrpTbl[E_GrDvrFsoCfgDskNormal]->IsDiskFull() )
				{
					m_IsDiskFull	=	TRUE;
				}
			}
		}
		//mirror
		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskMirror] )
		{
			if ( m_GrpTbl[E_GrDvrFsoCfgDskMirror]->PacketAdd( A_PtrPkt, NULL ) )
			{
				m_IsWdgTick	=	TRUE;
				Tv_Result	=	TRUE;
			}
		}

		//update recordable
		m_IsRecAble	=	(BOOL8)Tv_Result;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//check flush timer
		if ( m_HndlTimer == A_Hndl )
		{
			//update limite timer
			LcAutoDelTimeUpdt();
			//try flush
			for ( Tv_WkIdx=0;E_GrDvrFsoCfgDskModeCnt > Tv_WkIdx;Tv_WkIdx++ )
			{
				if ( NULL != m_GrpTbl[Tv_WkIdx] )
				{
					m_GrpTbl[Tv_WkIdx]->BufSync();
					m_GrpTbl[Tv_WkIdx]->TemperatureUpdt();	//block temprature update
				}
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcFormat( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_WkIdx=0;E_GrDvrFsoCfgDskModeCnt > Tv_WkIdx;Tv_WkIdx++ )
		{
			if ( NULL != m_GrpTbl[Tv_WkIdx] )
			{
				if ( m_GrpTbl[Tv_WkIdx]->Format( A_DskIdx, A_IsWithoutLog ) )
				{
					//success
					Tv_Result	=	TRUE;
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcTmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LstCnt;
		__u32	Tv_ReadCnt;
		Def_GrTime	Tv_Time;
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_Result	=	FALSE;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				//clear buffer
				GrDumyZeroMem( A_PtrBuf, A_Cnt << E_GrDvrRecMinThumbSizeShft );
				Tv_LstCnt	=	A_Cnt;
				Tv_Time		=	A_Time;
				Tv_PtrBuf	=	A_PtrBuf;
				while ( 0 != Tv_LstCnt )
				{
					Tv_ReadCnt	=	Tv_LstCnt;
					if ( E_GrDvrFsoTmaTmpCnt < Tv_ReadCnt )
					{
						Tv_ReadCnt	=	E_GrDvrFsoTmaTmpCnt;
					}

					if ( m_GrpTbl[A_DevType]->TmaReadCmb( Tv_Time, Tv_PtrBuf, Tv_ReadCnt, (void*)m_TmaRcch ) )
					{
						Tv_Result	=	TRUE;
					}

					//next
					Tv_LstCnt	=	Tv_LstCnt - Tv_ReadCnt;
					Tv_Time		=	Tv_Time + (Tv_ReadCnt * E_GrTimeSecOfMin);
					Tv_PtrBuf	=	(void*)( (__u32)Tv_PtrBuf + (Tv_ReadCnt << E_GrDvrRecMinThumbSizeShft) );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::RecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//read current date
		Tv_TimeMin	=	A_Time - (A_Time % E_GrTimeSecOfMin);
		if ( LcTmaRead( A_DevType, Tv_TimeMin, (void*)m_TmaTmp, 1 ) )
		{
			//check exist
			Tv_Result	=	m_TmaTmp[0].HndlRec;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::FindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//read current date
		Tv_IsEnd		=	FALSE;
		Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
		while ( LcTmaRead( A_DevType, Tv_TimeMin, (void*)m_TmaTmp, E_GrDvrFsoTmaTmpCnt ) )
		{
			//find current time	
			for ( Tv_TmaIdx=0;Tv_TmaIdx < E_GrDvrFsoTmaTmpCnt;Tv_TmaIdx++ )
			{
				//check find current
				if ( E_GrDvrFsoHndlNone != m_TmaTmp[Tv_TmaIdx].HndlRec )
				{
					//finded
					Tv_Result	=	m_TmaTmp[Tv_TmaIdx].HndlRec;
					Tv_IsEnd	=	TRUE;
					break;
				}
				//next
				Tv_TimeMin	=	Tv_TimeMin + E_GrTimeSecOfMin;
				//check time over
				if ( Tv_TimeMin > A_TimeEd  )
				{
					Tv_IsEnd	=	TRUE;
					break;
				}
			}
			if ( Tv_IsEnd )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::FindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//read current date
		Tv_IsEnd		=	FALSE;
		Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin) - E_GrTimeSecOfDay + E_GrTimeSecOfMin;
		while ( LcTmaRead( A_DevType, Tv_TimeMin, (void*)m_TmaTmp, E_GrDvrFsoTmaTmpCnt ) )
		{
			//find current time	
			Tv_TmaIdx	=	E_GrDvrFsoTmaTmpCnt;
			do 
			{
				//index
				Tv_TmaIdx --;
				//check find current
				if ( E_GrDvrFsoHndlNone != m_TmaTmp[Tv_TmaIdx].HndlRec )
				{
					//finded
					Tv_Result	=	m_TmaTmp[Tv_TmaIdx].HndlRec;
					Tv_IsEnd	=	TRUE;
					break;
				}
				//next
				Tv_TimeMin	=	Tv_TimeMin - E_GrTimeSecOfMin;
				//check time over
				if ( Tv_TimeMin < A_TimeEd  )
				{
					Tv_IsEnd	=	TRUE;
					break;
				}
			} while( 0 != Tv_TmaIdx );
			if ( Tv_IsEnd )
			{
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::DirectWrite( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->DirectWrite( A_Hndl, A_PtrBuf, A_Size );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::ReadNormal( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, 
																	__u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->ReadNormal( A_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::ReadBack( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, 
																__u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->ReadBack( A_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::ReadPrev( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->ReadPrev( A_Hndl, A_PtrBuf, A_Size, A_PtrRtSize, A_PtrRtCrntHndl );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::RecBeginHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeSt;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				//check auto delete
				if ( 0 != m_AtdLimTime )
				{
					//check last time
					if ( RecTimeRangeGet( A_DevType, &Tv_TimeSt, NULL ) )
					{
						if ( Tv_TimeSt < m_AtdLimTime )
						{
							Tv_Result	=	FindHndlByMinFw( A_DevType, m_AtdLimTime, 0xFFFFFFFF );
						}
						else
						{
							Tv_Result	=	m_GrpTbl[A_DevType]->RecBeginHndlGet();
						}
					}
				}
				else
				{
					Tv_Result	=	m_GrpTbl[A_DevType]->RecBeginHndlGet();
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::RecEndHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->RecEndHndlGet();
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType <= E_GrDvrRecDevMirror;Tv_DevType++ )
		{
			if ( NULL != m_GrpTbl[Tv_DevType] )
			{
				m_GrpTbl[Tv_DevType]->EvtLogAdd( A_PtrItem );
			}
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoMng::LcEvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
{
	// local -------------------
		__u32	Tv_Result;
		/*
		__u32	Tv_TgIdx;
		Def_GrTime	Tv_TimeMk;
		__u32	Tv_MkIdx;					//mark index
		__u32	Tv_FindIdx;
		St_GrDvrLogItm	Tv_TmpLog;
		__u32*	Tv_PtrTmp;
		__u32*	Tv_PtrSr;
		__u32*	Tv_PtrTg;
		*/
	// code --------------------
		Tv_Result	=	0;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->EvtLogGet( A_TimeSt, A_TimeEd, A_EvtMask, A_ChMask, A_PtrRt, A_MaxCnt );
				//sort
				/*
				if ( 0 != Tv_Result )
				{
					Tv_PtrTmp	=	(__u32*)&Tv_TmpLog;
					for ( Tv_TgIdx=0;Tv_TgIdx < Tv_Result;Tv_TgIdx++ )
					{
						//check finished
						if ( 2 <= (Tv_Result - Tv_TgIdx) )
						{
							//find bigest time
							Tv_TimeMk		=	A_PtrRt[Tv_TgIdx].Time;
							Tv_MkIdx		=	Tv_TgIdx;
							for ( Tv_FindIdx= Tv_TgIdx + 1;Tv_FindIdx < Tv_Result;Tv_FindIdx++ )
							{
								if ( Tv_TimeMk < A_PtrRt[Tv_FindIdx].Time )
								{
									//change mark
									Tv_MkIdx	=	Tv_FindIdx;
									Tv_TimeMk	=	A_PtrRt[Tv_FindIdx].Time;
								}
							}
							//check finded
							if ( Tv_TgIdx != Tv_MkIdx )
							{
								//tg to tmp
								Tv_PtrSr	=	(__u32*)&A_PtrRt[Tv_TgIdx];
								Tv_PtrTmp[0]	=	Tv_PtrSr[0];
								Tv_PtrTmp[1]	=	Tv_PtrSr[1];
								Tv_PtrTmp[2]	=	Tv_PtrSr[2];
								Tv_PtrTmp[3]	=	Tv_PtrSr[3];
								//mk to tg
								Tv_PtrTg	=	(__u32*)&A_PtrRt[Tv_MkIdx];
								Tv_PtrSr[0]	=	Tv_PtrTg[0];
								Tv_PtrSr[1]	=	Tv_PtrTg[1];
								Tv_PtrSr[2]	=	Tv_PtrTg[2];
								Tv_PtrSr[3]	=	Tv_PtrTg[3];
								//tmp to mk
								Tv_PtrTg[0]	=	Tv_PtrTmp[0];
								Tv_PtrTg[1]	=	Tv_PtrTmp[1];
								Tv_PtrTg[2]	=	Tv_PtrTmp[2];
								Tv_PtrTg[3]	=	Tv_PtrTmp[3];
							}
						}
					}
				}
				*/
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_TypeIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_TypeIdx=0;Tv_TypeIdx < E_GrDvrFsoCfgDskModeCnt;Tv_TypeIdx++ )
		{
			if ( NULL != m_GrpTbl[Tv_TypeIdx] )
			{
				Tv_Result	=	m_GrpTbl[Tv_TypeIdx]->UcaWrite( A_Pos, A_PtrBuf, A_Size );
				if ( Tv_Result )
				{
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_TypeIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		for ( Tv_TypeIdx=0;Tv_TypeIdx < E_GrDvrFsoCfgDskModeCnt;Tv_TypeIdx++ )
		{
			if ( NULL != m_GrpTbl[Tv_TypeIdx] )
			{
				Tv_Result	=	m_GrpTbl[Tv_TypeIdx]->UcaRead( A_Pos, A_PtrBuf, A_Size );
				if ( Tv_Result )
				{
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcUwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsVssAble && (NULL != m_GrpTbl[E_GrDvrRecDevNoraml]) )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrRecDevNoraml]->UwaWrite( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcUwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_IsVssAble && (NULL != m_GrpTbl[E_GrDvrRecDevNoraml]) )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrRecDevNoraml]->UwaRead( A_Pos, A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::LcTempGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
		__u8	Tv_DevType;
	// code --------------------
		Tv_Result	=	0;
		//check device type
		for ( Tv_DevType=0;Tv_DevType <= E_GrDvrRecDevMirror;Tv_DevType++ )
		{
			if ( NULL != m_GrpTbl[Tv_DevType] )
			{
				Tv_Result	=	m_GrpTbl[Tv_DevType]->TemperatureGet( A_DskIdx );
				if ( 0 != Tv_Result )
				{
					break;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::WbaWrite( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_GrpTbl[E_GrDvrRecDevNoraml] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrRecDevNoraml]->WbaWrite( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::WbaRead( void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( NULL != m_GrpTbl[E_GrDvrRecDevNoraml] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrRecDevNoraml]->WbaRead( A_PtrBuf, A_Size );
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch( ((Ptr_GrDvrFsoMngMsgBase)A_PtrMsg)->Cmd )
			{
				case	E_GrDvrFsoMngCmdFormat:
					*(((Ptr_GrDvrFsoMngMsgFormat)A_PtrMsg)->PtrRt)	=	LcFormat( ((Ptr_GrDvrFsoMngMsgFormat)A_PtrMsg)->DskIdx,
											((Ptr_GrDvrFsoMngMsgFormat)A_PtrMsg)->IsWithoutLog );
					break;
				case	E_GrDvrFsoMngCmdTmaRead:
					*(((Ptr_GrDvrFsoMngMsgTmaRead)A_PtrMsg)->PtrRt)	=	LcTmaRead( ((Ptr_GrDvrFsoMngMsgTmaRead)A_PtrMsg)->DevType,
											((Ptr_GrDvrFsoMngMsgTmaRead)A_PtrMsg)->Time, ((Ptr_GrDvrFsoMngMsgTmaRead)A_PtrMsg)->PtrBuf,
											((Ptr_GrDvrFsoMngMsgTmaRead)A_PtrMsg)->Cnt );
					break;
				case	E_GrDvrFsoMngCmdRecStat:
					LcRecStatusGet( (Ptr_GrDvrFsoRecStat)(((Ptr_GrDvrFsoMngMsgRecStat)A_PtrMsg)->PtrBuf) );
					break;
				case	E_GrDvrFsoMngCmdSetup:
					LcSetup( ((Ptr_GrDvrFsoMngMsgSetup)A_PtrMsg)->PtrDskTbl, ((Ptr_GrDvrFsoMngMsgSetup)A_PtrMsg)->DiskCnt,
										((Ptr_GrDvrFsoMngMsgSetup)A_PtrMsg)->IsNormalOvwt, ((Ptr_GrDvrFsoMngMsgSetup)A_PtrMsg)->IsMirrorOvwt	);
					break;
				case	E_GrDvrFsoMngCmdUcaWrite:
					*(((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->PtrRt)	=	LcUcaWrite( ((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->Pos,
												((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFsoMngCmdUcaRead:
					*(((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->PtrRt)	=	LcUcaRead( ((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->Pos,
												((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgUcaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFsoMngCmdUwaWrite:
					*(((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->PtrRt)	=	LcUwaWrite( ((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->Pos,
																			((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFsoMngCmdUwaRead:
					*(((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->PtrRt)	=	LcUwaRead( ((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->Pos,
																			((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgUwaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFsoMngCmdTimeRng:
					*(((Ptr_GrDvrFsoMngMsgTimeRng)A_PtrMsg)->PtrRt)	=	RecTimeRangeGet( ((Ptr_GrDvrFsoMngMsgTimeRng)A_PtrMsg)->DevType,
																			((Ptr_GrDvrFsoMngMsgTimeRng)A_PtrMsg)->PtrTimeSt, ((Ptr_GrDvrFsoMngMsgTimeRng)A_PtrMsg)->PtrTimeEd );
					break;
				case	E_GrDvrFsoMngCmdLogAdd:
					LcEvtLogAdd( &((Ptr_GrDvrFsoMngMsgLogAdd)A_PtrMsg)->Log );
					break;
				case	E_GrDvrFsoMngCmdLogGet:
					*(((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->PtrRt)	=	LcEvtLogGet( ((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->DevType,
																			((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->TimeEd,
																			((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->EvtMask, ((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->ChMask,
																			((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->PtrRtLog,	((Ptr_GrDvrFsoMngMsgLogGet)A_PtrMsg)->MaxCnt );
					break;
				case	E_GrDvrFsoMngCmdHndlBegin:
					*(((Ptr_GrDvrFsoMngMsgHndlGet)A_PtrMsg)->PtrRt)	=	RecBeginHndlGet( ((Ptr_GrDvrFsoMngMsgHndlGet)A_PtrMsg)->DevType );
					break;
				case	E_GrDvrFsoMngCmdHndlEnd:
					*(((Ptr_GrDvrFsoMngMsgHndlGet)A_PtrMsg)->PtrRt)	=	RecEndHndlGet( ((Ptr_GrDvrFsoMngMsgHndlGet)A_PtrMsg)->DevType );
					break;
				case	E_GrDvrFsoMngCmdHndlMinute:
					*(((Ptr_GrDvrFsoMngMsgHndlByTime)A_PtrMsg)->PtrRt)	=	RecHndlByTimeMin( ((Ptr_GrDvrFsoMngMsgHndlByTime)A_PtrMsg)->DevType,
																													((Ptr_GrDvrFsoMngMsgHndlByTime)A_PtrMsg)->Time );
					break;
				case	E_GrDvrFsoMngCmdReadNormal:
					*(((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRslt)	=	ReadNormal(
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->DevType,	((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Size,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtSize, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtCrntHndl );
					break;
				case	E_GrDvrFsoMngCmdReadBack:
					*(((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRslt)	=	ReadBack(
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->DevType,	((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Size,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtSize, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtCrntHndl );
					break;
				case  E_GrDvrFsoMngCmdReadPrev:
					*(((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRslt)	=	ReadPrev(
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->DevType,	((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Hndl,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->Size,
						((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtSize, ((Ptr_GrDvrFsoMngMsgReadPara)A_PtrMsg)->PtrRtCrntHndl );
					break;
				case	E_GrDvrFsoMngCmdHndlFindFw:
					*(((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->PtrRt)	=	FindHndlByMinFw( ((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->DevType,
						((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFsoMngCmdAtdSet:
					LcAutoDelTimeSet( ((Ptr_GrDvrFsoMngMsgAtdSet)A_PtrMsg)->DayCnt );
					break;
				case	E_GrDvrFsoMngCmdTempGet:
					*(((Ptr_GrDvrFsoMngMsgTempGet)A_PtrMsg)->PtrRt)	=	LcTempGet( ((Ptr_GrDvrFsoMngMsgTempGet)A_PtrMsg)->DskIdx );
					break;
				case	E_GrDvrFsoMngCmdHndlFindBw:
					*(((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->PtrRt)	=	FindHndlByMinBw( ((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->DevType,
						((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFsoMngMsgHndlByRng)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFsoMngCmdDataSize:
					*(((Ptr_GrDvrFsoMngMsgDataSize)A_PtrMsg)->PtrRt)	=	CalcDataSize( ((Ptr_GrDvrFsoMngMsgDataSize)A_PtrMsg)->DevType,
						((Ptr_GrDvrFsoMngMsgDataSize)A_PtrMsg)->ChMap, ((Ptr_GrDvrFsoMngMsgDataSize)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFsoMngMsgDataSize)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFsoMngCmdVssGet:
					*(((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtRslt)	=	LcVssItemGet( 
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtHndlRec, 
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtTime,
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtCh, ((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtCmd,
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtIsArmed, 
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtEtty,
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtSeq,
						((Ptr_GrDvrFsoMngMsgVssGet)A_PtrMsg)->PtrRtFrmCnt );
					break;
				case	E_GrDvrFsoMngCmdVssPop:
					LcVssItemPop();
					break;
				case	E_GrDvrFsoMngCmdVssAdd:
					VssItemAdd( ((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Hndl, ((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Time,
						((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Ch, ((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Cmd,
						((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->IsArmed, ((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Etty,
						((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->Seq, ((Ptr_GrDvrFsoMngMsgVssAdd)A_PtrMsg)->FrmCnt );
					break;
				case E_GrDvrFsoMngCmdVssReset:
					LcVssReset();
					break;
				case	E_GrDvrFsoMngCmdMainAllErr:
					*(((Ptr_GrDvrFsoMngMsgMainAllErr)A_PtrMsg)->PtrRtRslt)	=	IsMainDskAllErr();
					break;
				case E_GrDvrFsoMngCmdDayMapGet:
					*(((Ptr_GrDvrFsoMngMsgDayMapGet)A_PtrMsg)->PtrRtRslt)	=	ImmDayMapGet( ((Ptr_GrDvrFsoMngMsgDayMapGet)A_PtrMsg)->DevType, ((Ptr_GrDvrFsoMngMsgDayMapGet)A_PtrMsg)->TimeDay,
						((Ptr_GrDvrFsoMngMsgDayMapGet)A_PtrMsg)->DayCnt );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdFormat;
		Tv_Msg.DskIdx					=	A_DskIdx;
		Tv_Msg.IsWithoutLog		=	A_IsWithoutLog;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::SafeTmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgTmaRead	Tv_Msg;
		Def_GrTime	Tv_TimeEd;
		BOOL8	Tv_IsRead;
		Def_GrTime	Tv_AtdLimTime;
		Ptr_GrDvrRecMinThumb	Tv_PtrThumb;
		Def_GrTime	Tv_TmWk;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_AtdLimTime	=	m_AtdLimTime;

		//check autodelete range
		Tv_IsRead	=	TRUE;
		if ( 0 != Tv_AtdLimTime )
		{
			//calculate last time
			Tv_TimeEd	=	A_Time + (E_GrTimeSecOfMin * A_Cnt);
			if ( Tv_TimeEd <= Tv_AtdLimTime )
			{
				//direct run
				Tv_IsRead	=	FALSE;
				GrDumyZeroMem( A_PtrBuf, A_Cnt << E_GrDvrRecMinThumbSizeShft );
				Tv_Result	=	TRUE;
			}
		}
		
		if ( Tv_IsRead )
		{
			Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
			Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdTmaRead;
			Tv_Msg.DevType				=	A_DevType;
			Tv_Msg.Time						=	A_Time;
			Tv_Msg.PtrBuf					=	A_PtrBuf;
			Tv_Msg.Cnt						=	A_Cnt;
			Tv_Msg.PtrRt		=	&Tv_Result;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

			//patch auto delete
			if ( Tv_Result )
			{
				if ( 0 != Tv_AtdLimTime )
				{
					if ( A_Time < Tv_AtdLimTime  )
					{
						Tv_TmWk			=	A_Time;
						Tv_PtrThumb	=	(Ptr_GrDvrRecMinThumb)A_PtrBuf;
						for ( Tv_WkIdx=0;Tv_WkIdx < A_Cnt;Tv_WkIdx++ )
						{
							//clear
							if ( Tv_TmWk < Tv_AtdLimTime )
							{
								Tv_PtrThumb->HndlRec	=	E_GrDvrFsoHndlNone;
							}
							//next
							Tv_PtrThumb	++;
							Tv_TmWk		=	Tv_TmWk + E_GrTimeSecOfMin;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcRecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStatTbl )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFsoCfgDskModeCnt;Tv_DevType++ )
		{
			if ( NULL != m_GrpTbl[Tv_DevType] )
			{
				m_GrpTbl[Tv_DevType]->RecStatusGet( A_PtrStatTbl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::RecStatusGet( Ptr_GrDvrFsoRecStat A_PtrStatTbl )
{
	// local -------------------
		St_GrDvrFsoMngMsgRecStat	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdRecStat;
		Tv_Msg.PtrBuf					=	(void*)A_PtrStatTbl;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::Setup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt )
{
	// local -------------------
		St_GrDvrFsoMngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdSetup;
		Tv_Msg.PtrDskTbl			=	A_PtrDskTbl;
		Tv_Msg.DiskCnt				=	A_Cnt;
		Tv_Msg.IsNormalOvwt		=	A_IsNormalOvwt;
		Tv_Msg.IsMirrorOvwt		=	A_IsMirrorOvwt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgUcaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdUcaWrite;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgUcaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdUcaRead;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::DirvePathSet( __u8 A_Idx, __s8* A_StrPath )
{
		if ( E_GrDvrFsoMaxDriveCnt > A_Idx )
		{
			GrStrCopy( m_DskCfg.DevFnTbl[A_Idx], A_StrPath );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgUwaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdUwaWrite;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgUwaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFsoMngCmdUwaRead;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::UwaDiskPosGet( void )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFsoCfgDskNormal]->UwaDiskPosGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::UwaDiskIndexGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFsoCfgDskNormal]->UwaDiskIndexGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::RecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		//check device type
		if (E_GrDvrRecDevMirror >= A_DevType)
		{
			//check not need start
			if ( NULL != A_PtrRtTimeSt )
			{
				if ( NULL != m_GrpTbl[A_DevType] )
				{
					*A_PtrRtTimeSt	=	m_GrpTbl[A_DevType]->RecTimeBeginGet();
					if ( 0 != (*A_PtrRtTimeSt) )
					{
						Tv_Result	=	TRUE;
						//check auto delete
						if ( 0 != m_AtdLimTime )
						{
							//check start time
							if ( (*A_PtrRtTimeSt) < m_AtdLimTime  )
							{
								*A_PtrRtTimeSt	=	m_AtdLimTime;
							}
						}
					}
				}
			}
			//get last direct
			if (NULL != A_PtrRtTimeEd)
			{
				if ( NULL != m_GrpTbl[A_DevType] )
				{
					*A_PtrRtTimeEd	=	m_GrpTbl[A_DevType]->RecTimeEndGet();
					if ( 0 != (*A_PtrRtTimeEd) )
					{
						Tv_Result	=	TRUE;
						//check auto delete
						if ( 0 != m_AtdLimTime )
						{
							//check end time
							if ( (*A_PtrRtTimeEd) < m_AtdLimTime )
							{
								*A_PtrRtTimeEd	=	0;
								Tv_Result	=	FALSE;
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::SafeRecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgTimeRng	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdTimeRng;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrTimeSt		=	A_PtrRtTimeSt;
		Tv_Msg.PtrTimeEd		=	A_PtrRtTimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		St_GrDvrFsoMngMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdLogAdd;
		Tv_Msg.Log					=	*A_PtrItem;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoMng::EvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
{
	// local -------------------
		__u32		Tv_Result;
		St_GrDvrFsoMngMsgLogGet	Tv_Msg;
	// code --------------------
		Tv_Result						=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdLogGet;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.EvtMask			=	A_EvtMask;
		Tv_Msg.ChMask				=	A_ChMask;
		Tv_Msg.PtrRtLog			=	A_PtrRt;
		Tv_Msg.MaxCnt				=	A_MaxCnt;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeRecBeginHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgHndlGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdHndlBegin;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeRecEndHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgHndlGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdHndlEnd;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeRecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgHndlByTime	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrFsoHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdHndlMinute;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.Time					=	A_Time;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::SafeReadNormal( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, 
																			__u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFsoMngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdReadNormal;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.Size					=	A_Size;
		Tv_Msg.PtrRtSize		=	A_PtrRtSize;
		Tv_Msg.PtrRtCrntHndl	=	A_PtrRtCrntHndl;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::SafeReadBack( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, 
																		__u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFsoMngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdReadBack;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.Size					=	A_Size;
		Tv_Msg.PtrRtSize		=	A_PtrRtSize;
		Tv_Msg.PtrRtCrntHndl	=	A_PtrRtCrntHndl;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::SafeReadPrev( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size, __u32* A_PtrRtSize, __u64* A_PtrRtCrntHndl )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFsoMngMsgReadPara	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdReadPrev;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.PtrBuf				=	A_PtrBuf;
		Tv_Msg.Size					=	A_Size;
		Tv_Msg.PtrRtSize		=	A_PtrRtSize;
		Tv_Msg.PtrRtCrntHndl	=	A_PtrRtCrntHndl;
		Tv_Msg.PtrRslt			=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeFindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgHndlByRng	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdHndlFindFw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeFindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgHndlByRng	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecErrHndlBad;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdHndlFindBw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoMng::FccStreamGet( void )
{
		return	m_DskCfg.FccStrm;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcAutoDelTimeUpdt( void )
{
	// local -------------------
		Def_GrTime	Tv_Time;
		__u32				Tv_Mili;
		BOOL8				Tv_IsSm;
	// code --------------------
		if ( 0 != m_AutoDelTime )
		{
			Tv_Time	=	GrTimeGet( &Tv_Mili, &Tv_IsSm );
			m_AtdLimTime	=	Tv_Time - m_AutoDelTime;
		}
		else
		{
			//no limited time
			m_AtdLimTime	=	0;
		}
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFsoMng::AutoDelLimTimeGet( void )
{
		return	m_AtdLimTime;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcAutoDelTimeSet( __u32 A_DayCnt )
{
	// local -------------------
	// code --------------------
		m_AutoDelTime		=	A_DayCnt * E_GrTimeSecOfDay;
		LcAutoDelTimeUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::AutoDelTimeSet( __u32 A_DayCnt )
{
	// local -------------------
		St_GrDvrFsoMngMsgAtdSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdAtdSet;
		Tv_Msg.DayCnt				=	A_DayCnt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::TemperatureGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFsoMngMsgTempGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdTempGet;
		Tv_Msg.DskIdx				=	A_DskIdx;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsDiskFull()
{
		return	m_IsDiskFull;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsRecorded( BOOL8 A_IsReset )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	m_IsReced;
		if ( A_IsReset )
		{
			m_IsReced	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsWdgChk( BOOL8 A_IsReset )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	m_IsWdgTick;
		if ( A_IsReset )
		{
			m_IsWdgTick	=	FALSE;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsRecAble( void )
{
		return	m_IsRecAble;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::CalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->CalcDataSize( A_ChMap, A_TimeSt, A_TimeEd, m_TmaTmp );
				if ( 0 != Tv_Result )
				{
					//patch error term
					Tv_Result	=	Tv_Result + 60000000;
				}
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFsoMng::SafeCalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFsoMngMsgDataSize	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdDataSize;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.ChMap				=	A_ChMap;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsVssAble( void )
{
		return	m_IsVssAble;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::VssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, 
																	__u8	A_Etty, __u8 A_Seq, __u8 A_FrmCnt )
{
	// local -------------------
		Ptr_GrDvrFsoVssItm	Tv_PtrItm;
		__u32	Tv_Pos;
	// code --------------------
		Tv_PtrItm	=	(Ptr_GrDvrFsoVssItm)GrBufQueGetInPtr( m_PtrVssQue );
		if ( NULL != Tv_PtrItm )
		{
			//add data
			Tv_PtrItm->HndlRec	=	A_Hndl;
			Tv_PtrItm->Time			=	A_Time;
			Tv_PtrItm->Ch				=	A_Ch;
			Tv_PtrItm->Cmd			=	A_Cmd;
			Tv_PtrItm->IsArmed	=	A_IsArmed;
			Tv_PtrItm->Etty			=	A_Etty;
			Tv_PtrItm->Seq			=	A_Seq;
			Tv_PtrItm->FrmCnt		=	A_FrmCnt;
			GrBufQueIn( m_PtrVssQue );		//data in

			//save contorl area
			LcUwaWrite( 0, (void*)m_PtrVssQue, sizeof(St_GrBufQue) );
			//save item area
			Tv_Pos		=	(__u32)Tv_PtrItm - (__u32)m_PtrVssQue;
			LcUwaWrite( (__u64)Tv_Pos, (void*)Tv_PtrItm, E_GrDvrFsoVssItmSize );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::SafeVssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, 
	__u8	A_Etty,	__u8 A_Seq, __u8 A_FrmCnt )
{
	// local -------------------
		St_GrDvrFsoMngMsgVssAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdVssAdd;
		Tv_Msg.Hndl					=	A_Hndl;
		Tv_Msg.Time					=	A_Time;
		Tv_Msg.Ch						=	A_Ch;
		Tv_Msg.Cmd					=	A_Cmd;
		Tv_Msg.IsArmed			=	A_IsArmed;
		Tv_Msg.Etty					=	A_Etty;
		Tv_Msg.Seq					=	A_Seq;
		Tv_Msg.FrmCnt				=	A_FrmCnt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::LcVssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, 
																		__u8* A_PtrRtCmd, BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, 
																		__u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFsoVssItm	Tv_PtrItm;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrItm	=	(Ptr_GrDvrFsoVssItm)GrBufQueGetOutPtr( m_PtrVssQue );
		if ( NULL != Tv_PtrItm )
		{
			*A_PtrRtHndl		=	Tv_PtrItm->HndlRec;
			*A_PtrRtTime		=	Tv_PtrItm->Time;
			*A_PtrRtCh			=	Tv_PtrItm->Ch;
			*A_PtrRtCmd			=	Tv_PtrItm->Cmd;
			*A_PtrRtIsArmed	=	Tv_PtrItm->IsArmed;
			*A_PtrRtEtty		=	Tv_PtrItm->Etty;
			*A_PtrRtSeq			=	Tv_PtrItm->Seq;
			*A_PtrRtFrmCnt	=	Tv_PtrItm->FrmCnt;

			Tv_Result				=	TRUE;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcVssItemPop( void )
{
	// local -------------------
		Ptr_GrDvrFsoVssItm	Tv_PtrItm;
	// code --------------------
		Tv_PtrItm	=	(Ptr_GrDvrFsoVssItm)GrBufQueGetOutPtr( m_PtrVssQue );
		if ( NULL != Tv_PtrItm )
		{
			GrBufQueOut( m_PtrVssQue );
			//save contorl area
			LcUwaWrite( 0, (void*)m_PtrVssQue, sizeof(St_GrBufQue) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::VssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, 
																	__u8* A_PtrRtCmd, BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, 
																	__u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgVssGet	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdVssGet;
		Tv_Msg.PtrRtHndlRec	=	A_PtrRtHndl;
		Tv_Msg.PtrRtTime		=	A_PtrRtTime;
		Tv_Msg.PtrRtCh			=	A_PtrRtCh;
		Tv_Msg.PtrRtCmd			=	A_PtrRtCmd;
		Tv_Msg.PtrRtIsArmed	=	A_PtrRtIsArmed;
		Tv_Msg.PtrRtEtty		=	A_PtrRtEtty;
		Tv_Msg.PtrRtSeq			=	A_PtrRtSeq;
		Tv_Msg.PtrRtFrmCnt	=	A_PtrRtFrmCnt;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::VssItemPop( void )
{
	// local -------------------
		St_GrDvrFsoMngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDvrFsoMngCmdVssPop;
		
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::LcVssReset( void )
{
		GrBufQueInit( m_PtrVssQue, sizeof(St_GrDvrFsoVssItm), m_VssSize );
		//save contorl area
		LcUwaWrite( 0, (void*)m_PtrVssQue, sizeof(St_GrBufQue) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFsoMng::VssReset( void )
{
	// local -------------------
		St_GrDvrFsoMngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDvrFsoMngCmdVssReset;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsMainDskAllErr( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		if ( NULL != m_GrpTbl[E_GrDvrFsoCfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFsoCfgDskNormal]->IsAllError();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFsoMng::IsSafeMainDskAllErr( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFsoMngMsgMainAllErr	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdMainAllErr;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::MissMapGet( void )
{
		return	m_MissMap;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFsoMng::NotFormatMapGet( void )
{
		return	m_NotFmtMap;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoMng::ImmDayMapGet( __u8 A_DevType, Def_GrTime A_TimeDay, __u32 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_Result	=	m_GrpTbl[A_DevType]->DayMapGet( A_TimeDay, A_DayCnt );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFsoMng::SafeDayMapGet( __u8 A_DevType, Def_GrTime A_TimeDay, __u32 A_DayCnt )
{
	// local -------------------
		__u32	Tv_Result;
		St_GrDvrFsoMngMsgDayMapGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFsoMngCmdDayMapGet;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeDay			=	A_TimeDay;
		Tv_Msg.DayCnt				=	A_DayCnt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
