/*
 file system version 1

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Fs/V1/GrDvrFs1Mng.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<Dvr/Fs/V1/GrDvrFs1Dsk.h>

//====================================================================
//local const

#define E_GrDvrFs1MngCmdFormat			0x00
#define E_GrDvrFs1MngCmdTmaRead			0x01
#define E_GrDvrFs1MngCmdRecStat			0x02		/* record status get St_GrDvrFs1MngMsgRecStat */
#define E_GrDvrFs1MngCmdSetup				0x03		/* record status get St_GrDvrFs1MngMsgSetup */
#define E_GrDvrFs1MngCmdUcaWrite		0x04		/* user configuration area write : St_GrDvrFs1MngMsgUcaRw */
#define E_GrDvrFs1MngCmdUcaRead			0x05		/* user configuration area read : St_GrDvrFs1MngMsgUcaRw */
#define E_GrDvrFs1MngCmdUwaWrite		0x06		/* user work area write : St_GrDvrFs1MngMsgUwaRw */
#define E_GrDvrFs1MngCmdUwaRead			0x07		/* user work area read : St_GrDvrFs1MngMsgUwaRw */
#define E_GrDvrFs1MngCmdTimeRng			0x08		/* record time range get : St_GrDvrFs1MngMsgTimeRng */
#define E_GrDvrFs1MngCmdLogAdd			0x09		/* log add : St_GrDvrFs1MngMsgLogAdd */
#define E_GrDvrFs1MngCmdLogGet			0x0A		/* log get : St_GrDvrFs1MngMsgLogGet */
#define E_GrDvrFs1MngCmdHndlBegin		0x0B		/* begin handle get : St_GrDvrFs1MngMsgHndlGet */
#define E_GrDvrFs1MngCmdHndlEnd			0x0C		/* end handle get : St_GrDvrFs1MngMsgHndlGet */
#define E_GrDvrFs1MngCmdHndlMinute	0x0D		/* handle by minute : St_GrDvrFs1MngMsgHndlByTime */
#define E_GrDvrFs1MngCmdReadNormal	0x0E		/* read normal : St_GrDvrFs1MngMsgReadPara */
#define E_GrDvrFs1MngCmdReadBack		0x0F		/* read back : St_GrDvrFs1MngMsgReadPara */
#define E_GrDvrFs1MngCmdHndlFindFw	0x10		/* handle by near minute : St_GrDvrFs1MngMsgHndlByRng */
#define E_GrDvrFs1MngCmdAtdSet			0x11		/* auto delete set : St_GrDvrFs1MngMsgAtdSet */
#define E_GrDvrFs1MngCmdTempGet			0x12		/* temperature get : St_GrDvrFs1MngMsgTempGet */
#define E_GrDvrFs1MngCmdHndlFindBw	0x13		/* handle by near minute dir backward : St_GrDvrFs1MngMsgHndlByRng */
#define E_GrDvrFs1MngCmdDataSize		0x14		/* calculate recorded data size : St_GrDvrFs1MngMsgDataSize */
#define E_GrDvrFs1MngCmdVssGet			0x15		/* vss data get : St_GrDvrFs1MngMsgVssGet */
#define E_GrDvrFs1MngCmdVssPop			0x16		/* vss queue pop : St_GrDvrFs1MngMsgBase */
#define E_GrDvrFs1MngCmdVssAdd			0x17		/* vss item add : St_GrDvrFs1MngMsgVssAdd */
#define E_GrDvrFs1MngCmdVssReset		0x18		/* vss queue reset : St_GrDvrFs1MngMsgBase */
#define E_GrDvrFs1MngCmdMainAllErr	0x19		/* main disk all error : St_GrDvrFs1MngMsgMainAllErr */
#define E_GrDvrFs1MngCmdReadFw			0x1A		/* read forward : St_GrDvrFs1MngMsgDvCtl */
#define E_GrDvrFs1MngCmdReadBw			0x1B		/* read backward : St_GrDvrFs1MngMsgDvCtl */
#define E_GrDvrFs1MngCmdReadBegin		0x1C		/* read begin : St_GrDvrFs1MngMsgDvCtl */
#define E_GrDvrFs1MngCmdReadEnd			0x1D		/* read end : St_GrDvrFs1MngMsgDvCtl */
#define E_GrDvrFs1MngCmdReadMinFw		0x1E		/* read minute forward : St_GrDvrFs1MngMsgDvCtlTm */
#define E_GrDvrFs1MngCmdReadMinBw		0x1F		/* read minute backward : St_GrDvrFs1MngMsgDvCtlTm */
#define E_GrDvrFs1MngCmdReadHndlFw	0x20		/* read handle forward : St_GrDvrFs1MngMsgDvCtlHd */
#define E_GrDvrFs1MngCmdReadHndlBw	0x21		/* read handle backward : St_GrDvrFs1MngMsgDvCtlHd */


//====================================================================
//local type

typedef	struct St_GrDvrFs1MngMsgBase 
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrDvrFs1MngMsgBase;

typedef	struct St_GrDvrFs1MngMsgFormat 
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DskIdx;
	BOOL8	IsWithoutLog;
	BOOL8*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgFormat;

typedef	struct St_GrDvrFs1MngMsgTmaRead
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8 DevType;
	Def_GrTime Time;
	void* PtrBuf;
	__u32 Cnt;
	BOOL8*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgTmaRead;

typedef	struct St_GrDvrFs1MngMsgRecStat
{
	St_GrDvrFs1MngMsgBase	Hd;
	void*	PtrBuf;
}	*Ptr_GrDvrFs1MngMsgRecStat;

typedef	struct St_GrDvrFs1MngMsgSetup 
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8*		PtrDskTbl;
	__u8		DiskCnt;
	BOOL8		IsNormalOvwt;
	BOOL8		IsMirrorOvwt;
}	*Ptr_GrDvrFs1MngMsgSetup;

typedef	struct St_GrDvrFs1MngMsgUcaRw 
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u32		Pos;
	void*		PtrBuf;
	__u32		Size;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFs1MngMsgUcaRw;

typedef	struct St_GrDvrFs1MngMsgUwaRw 
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u64		Pos;
	void*		PtrBuf;
	__u32		Size;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFs1MngMsgUwaRw;

typedef	struct St_GrDvrFs1MngMsgTimeRng
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8		DevType;
	Def_GrTime*	PtrTimeSt;
	Def_GrTime*	PtrTimeEd;
	BOOL8*		PtrRt;
}	*Ptr_GrDvrFs1MngMsgTimeRng;

typedef	struct St_GrDvrFs1MngMsgLogAdd
{
	St_GrDvrFs1MngMsgBase	Hd;
	St_GrDvrLogItm	Log;
}	*Ptr_GrDvrFs1MngMsgLogAdd;

typedef	struct St_GrDvrFs1MngMsgLogGet
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u32	EvtMask;
	__u32	ChMask;
	Ptr_GrDvrLogItm	PtrRtLog;
	__u32	MaxCnt;
	__u32*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgLogGet;

typedef	struct St_GrDvrFs1MngMsgHndlGet
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	__u64*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgHndlGet;

typedef	struct St_GrDvrFs1MngMsgHndlByTime
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	Time;
	__u64*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgHndlByTime;

typedef	struct St_GrDvrFs1MngMsgReadPara
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	__u64 Hndl;
	void* PtrBuf;
	__u32 Size;
	__u32* PtrRtSize;
	__u64* PtrRtCrntHndl;
	__u8*	PtrRslt;
}	*Ptr_GrDvrFs1MngMsgReadPara;

typedef	struct St_GrDvrFs1MngMsgHndlByRng
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgHndlByRng;

typedef	struct St_GrDvrFs1MngMsgAtdSet
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u32	DayCnt;
}	*Ptr_GrDvrFs1MngMsgAtdSet;

typedef	struct St_GrDvrFs1MngMsgTempGet
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DskIdx;
	__u8*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgTempGet;

typedef	struct St_GrDvrFs1MngMsgDataSize
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	__u16	ChMap;
	Def_GrTime	TimeSt;
	Def_GrTime	TimeEd;
	__u64*	PtrRt;
}	*Ptr_GrDvrFs1MngMsgDataSize;

typedef	struct St_GrDvrFs1MngMsgVssGet
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u64*	PtrRtHndlRec;		//record handle
	Def_GrTime*	PtrRtTime;	//armed time
	__u8*	PtrRtCh;
	__u8*	PtrRtCmd;
	BOOL8*	PtrRtIsArmed;
	__u8*	PtrRtEtty;
	__u8*	PtrRtSeq;
	__u8*	PtrRtFrmCnt;
	BOOL8*	PtrRtRslt;
}	*Ptr_GrDvrFs1MngMsgVssGet;

typedef	struct	St_GrDvrFs1MngMsgVssAdd
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u64	Hndl;				//record handle
	Def_GrTime	Time;	//armed time
	__u8	Ch;
	__u8	Cmd;
	BOOL8	IsArmed;
	__u8	Etty;
	__u8	Seq;
	__u8	FrmCnt;
}	*Ptr_GrDvrFs1MngMsgVssAdd;

typedef	struct	St_GrDvrFs1MngMsgMainAllErr
{
	St_GrDvrFs1MngMsgBase	Hd;
	BOOL8*	PtrRtRslt;
}	*Ptr_GrDvrFs1MngMsgMainAllErr;

typedef struct St_GrDvrFs1MngMsgDvCtl
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Ptr_GrDvrFs1SeekCtrl	PtrCtrl;
}	*Ptr_GrDvrFs1MngMsgDvCtl;

typedef struct St_GrDvrFs1MngMsgDvCtlTm
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Ptr_GrDvrFs1SeekCtrl	PtrCtrl;
	Def_GrTime	Time;
}	*Ptr_GrDvrFs1MngMsgDvCtlTm;

typedef struct St_GrDvrFs1MngMsgDvCtlHd
{
	St_GrDvrFs1MngMsgBase	Hd;
	__u8	DevType;
	Ptr_GrDvrFs1SeekCtrl	PtrCtrl;
	__u64	Hndl;
}	*Ptr_GrDvrFs1MngMsgDvCtlHd;


//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs1Mng::Cls_GrDvrFs1Mng( Cls_GrTaskSvr* A_TaskSvr, __u32 A_SysId, __u32 A_FccStrm, Def_GrTime A_TimeBase, __u32 A_TmaCnt, 
																	BOOL8 A_IsVssUse, BOOL8 A_IsInstReplay, __u64 A_UwaSize, __u32 A_VssSize ):
Cls_GrTaskCli()
{
	// local -------------------
		__u8	Tv_Wkidx;
	// code --------------------

		//check struct size current
		if ( sizeof(St_GrDvrFs1Mis) > E_GrDvrFs1MisSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad mis structur size = %d \n", sizeof(St_GrDvrFs1Mis) );
		}
		if ( sizeof(St_GrDvrFs1Mca) > E_GrDvrFs1McaSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad mca structur size = %d \n", sizeof(St_GrDvrFs1Mca) );
		}
		if ( sizeof(St_GrDvrFs1Rca) > E_GrDvrFs1RcaItmSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad rca structur size = %d \n", sizeof(St_GrDvrFs1Rca) );
		}
		if ( sizeof(St_GrDvrRecMinThumb) != E_GrDvrRecMinThumbItemSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad tma structur size = %d \n", sizeof(St_GrDvrRecMinThumb) );
		}
		if ( sizeof(St_GrDvrLogItm) != E_GrDvrFs1ElaItmSize )
		{
			DbgMsgPrint( "dvr fs ver 1::bad ela structur size = %d \n", sizeof(St_GrDvrLogItm) );
		}
		/*
		if ( sizeof(St_GrDvrFs1VssItm) != 32 )
		{
			DbgMsgPrint( "dvr fs ver 1::bad vss structur size = %d \n", sizeof(St_GrDvrFs1VssItm) );
		}
		*/

		//init
		m_TaskSvr		=	A_TaskSvr;
		m_MsgAloc		=	GrTaskSvrMsgCmdAlloc();

		//setup data
		GrDumyZeroMem( &m_DskCfg, sizeof(m_DskCfg) );
		m_DskCfg.SysId			=	A_SysId;
		m_DskCfg.FccStrm		=	A_FccStrm;
		m_DskCfg.TimeBase		=	A_TimeBase;
		m_DskCfg.IdaSize		=	0;
		if ( A_IsInstReplay )
		{
			m_DskCfg.IdaSize	=	E_GrDvrFs1RdaAlignUnit;
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
		for ( Tv_Wkidx=0;E_GrDvrFs1CfgDskModeCnt > Tv_Wkidx;Tv_Wkidx++ )
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
		m_TaskSvr->RegistTimerClient( this, E_GrDvrFs1FlushPeriod, &m_HndlTimer );

}
//--------------------------------------------------------------------
Cls_GrDvrFs1Mng::~Cls_GrDvrFs1Mng()
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
void	Cls_GrDvrFs1Mng::LcFreeAllObject( void )
{
	// local -------------------
		__u8	Tv_Wkidx;
	// code --------------------
		//stop all group
		for ( Tv_Wkidx=0;E_GrDvrFs1CfgDskModeCnt > Tv_Wkidx;Tv_Wkidx++ )
		{
			if ( NULL != m_GrpTbl[Tv_Wkidx] )
			{
				delete	m_GrpTbl[Tv_Wkidx];
				m_GrpTbl[Tv_Wkidx]	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::LcSetup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt )
{
	// local -------------------
		__u8	Tv_Cnt;
		__u8	Tv_Wkidx;
		BOOL8	Tv_IsOk;
		__u32	Tv_DskMode;
		BOOL8	Tv_IsOvrt[E_GrDvrFs1CfgDskModeCnt];
		Cls_GrDvrFs1Dsk*	Tv_ObjDsk;
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
			if ( E_GrDvrFs1GrpMaxDiskCnt < Tv_Cnt )
			{
				Tv_Cnt	=	E_GrDvrFs1GrpMaxDiskCnt;
			}
			Tv_IsOvrt[E_GrDvrFs1CfgDskNormal]		=	A_IsNormalOvwt;
			Tv_IsOvrt[E_GrDvrFs1CfgDskMirror]		=	A_IsMirrorOvwt;
			Tv_IsOvrt[E_GrDvrFs1CfgDskHalt]			=	FALSE;
			for ( Tv_Wkidx=0;Tv_Cnt > Tv_Wkidx;Tv_Wkidx++ )
			{
				//check mode
				Tv_DskMode	=	A_PtrDskTbl[Tv_Wkidx];
				if ( E_GrDvrFs1CfgDskNone > Tv_DskMode )
				{
					//create disk
					Tv_ObjDsk	=	(Cls_GrDvrFs1Dsk*)new Cls_GrDvrFs1Dsk( Tv_Wkidx, &m_DskCfg, Tv_IsOvrt[Tv_DskMode], (void*)m_TmpBuf );
					//check exist disk
					if ( 0 != Tv_ObjDsk->DiskSizeGet() )
					{
						//patch disk mode
						if ( !Tv_ObjDsk->IsFormatted() )
						{
							m_NotFmtMap	=	m_NotFmtMap | (1 << Tv_Wkidx);
							Tv_DskMode	=	E_GrDvrFs1CfgDskHalt;
						}
						//check exist group
						if ( NULL == m_GrpTbl[Tv_DskMode] )
						{
							//create group
							m_GrpTbl[Tv_DskMode]	=	(Cls_GrDvrFs1Grp*)new Cls_GrDvrFs1Grp( (__u8)Tv_DskMode, Tv_IsOvrt[Tv_DskMode], m_GrpBuf );
						}
						//add disk
						m_GrpTbl[Tv_DskMode]->DiskAdd( Tv_ObjDsk );
						if ( E_GrDvrFs1CfgDskHalt > Tv_DskMode )
						{
							//mirror or normal hdd exist
							m_IsRecAble	=	TRUE;
						}
					}
					else
					{
						// miss map
						if ( E_GrDvrFs1CfgDskHalt > Tv_DskMode )
						{
							m_MissMap	=	m_MissMap | ( 1 << Tv_Wkidx );
						}
						//release disk
						delete	Tv_ObjDsk;
					}
				}
			}
			//setup finished , init
			for ( Tv_Wkidx=0;Tv_Wkidx <= E_GrDvrFs1CfgDskMirror;Tv_Wkidx++ )
			{
				//check exist
				if ( NULL != m_GrpTbl[Tv_Wkidx] )
				{
					m_GrpTbl[Tv_Wkidx]->DiskInit();
				}
			}
			//check vss able
			if ( m_IsVssUse && (NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal]) )
			{
				if ( m_VssSize <= m_GrpTbl[E_GrDvrFs1CfgDskNormal]->UwaSizeGet() )
				{
					m_IsVssAble	=	TRUE;
					//load vss data
					Tv_IsOk		=	FALSE;
					if ( m_GrpTbl[E_GrDvrFs1CfgDskNormal]->IsVssSaved() )
					{
						if ( m_GrpTbl[E_GrDvrFs1CfgDskNormal]->UwaRead( 0, (void*)m_PtrVssQue, m_VssSize ) )
						{
							DbgMsgPrint( "Cls_GrDvrFs1Mng::LcSetup - vss data loaded\n" );
							//check data able
							Tv_BufSize	=	m_VssSize - (m_VssSize % sizeof(St_GrDvrFs1VssItm));
							if ( (Tv_BufSize == (__u32)m_PtrVssQue->BufSize) && (sizeof(St_GrDvrFs1VssItm) == m_PtrVssQue->SizeItem) &&
								(m_VssSize > (__u32)m_PtrVssQue->InPos) && (m_VssSize > (__u32)m_PtrVssQue->OutPos)
								)
							{
								Tv_IsOk	=	TRUE;
							}
						}
						else
						{
							DbgMsgPrint( "Cls_GrDvrFs1Mng::LcSetup - vss data load failure\n" );
						}
					}
					if ( !Tv_IsOk )
					{
						//reset queue
						GrBufQueInit( m_PtrVssQue, sizeof(St_GrDvrFs1VssItm), m_VssSize );
						DbgMsgPrint( "Cls_GrDvrFs1Mng::LcSetup - vss data reset.\n" );
					}
				}
			}
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::PacketReserve( __u32 A_Size, __u64* A_PtrRtHndlNrm, __u64* A_PtrRtHndlMir )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		*A_PtrRtHndlNrm	=	E_GrDvrRecHndlNone;
		*A_PtrRtHndlMir	=	E_GrDvrRecHndlNone;
		//normal
		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal] )
		{
			*A_PtrRtHndlNrm	=	m_GrpTbl[E_GrDvrFs1CfgDskNormal]->PacketReserve( A_Size );
			if ( E_GrDvrRecHndlNone != *A_PtrRtHndlNrm )
			{
				Tv_Result	=	TRUE;
			}
		}
		//mirror
		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskMirror] )
		{
			*A_PtrRtHndlMir	=	m_GrpTbl[E_GrDvrFs1CfgDskMirror]->PacketReserve( A_Size );
			if ( E_GrDvrRecHndlNone != *A_PtrRtHndlMir )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::PacketAdd( Ptr_GrDvrAvHd A_PtrPkt, __u64* A_PtrRtHndl )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//normal
		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal] )
		{
			if ( m_GrpTbl[E_GrDvrFs1CfgDskNormal]->PacketAdd( A_PtrPkt, A_PtrRtHndl ) )
			{
				m_IsReced	=	TRUE;
				m_IsWdgTick	=	TRUE;
				Tv_Result	=	TRUE;
			}
			else
			{
				if ( m_GrpTbl[E_GrDvrFs1CfgDskNormal]->IsDiskFull() )
				{
					m_IsDiskFull	=	TRUE;
				}
			}
		}
		//mirror
		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskMirror] )
		{
			if ( m_GrpTbl[E_GrDvrFs1CfgDskMirror]->PacketAdd( A_PtrPkt, NULL ) )
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
void	Cls_GrDvrFs1Mng::RtlTimer( __u32 A_Hndl )
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
			for ( Tv_WkIdx=0;E_GrDvrFs1CfgDskModeCnt > Tv_WkIdx;Tv_WkIdx++ )
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
BOOL8	Cls_GrDvrFs1Mng::LcFormat( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_WkIdx=0;E_GrDvrFs1CfgDskModeCnt > Tv_WkIdx;Tv_WkIdx++ )
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
BOOL8	Cls_GrDvrFs1Mng::LcTmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
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
					if ( E_GrDvrFs1TmaTmpCnt < Tv_ReadCnt )
					{
						Tv_ReadCnt	=	E_GrDvrFs1TmaTmpCnt;
					}

					if ( m_GrpTbl[A_DevType]->TmaReadCmb( Tv_Time, Tv_PtrBuf, Tv_ReadCnt ) )
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
__u64	Cls_GrDvrFs1Mng::RecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

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
__u64	Cls_GrDvrFs1Mng::FindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		//read current date
		Tv_IsEnd		=	FALSE;
		Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin);
		while ( LcTmaRead( A_DevType, Tv_TimeMin, (void*)m_TmaTmp, E_GrDvrFs1TmaTmpCnt ) )
		{
			//find current time	
			for ( Tv_TmaIdx=0;Tv_TmaIdx < E_GrDvrFs1TmaTmpCnt;Tv_TmaIdx++ )
			{
				//check find current
				if ( E_GrDvrRecHndlNone != m_TmaTmp[Tv_TmaIdx].HndlRec )
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
__u64	Cls_GrDvrFs1Mng::FindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeMin;
		__u32	Tv_TmaIdx;
		BOOL8	Tv_IsEnd;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		//read current date
		Tv_IsEnd		=	FALSE;
		Tv_TimeMin	=	A_TimeSt - (A_TimeSt % E_GrTimeSecOfMin) - E_GrTimeSecOfDay + E_GrTimeSecOfMin;
		while ( LcTmaRead( A_DevType, Tv_TimeMin, (void*)m_TmaTmp, E_GrDvrFs1TmaTmpCnt ) )
		{
			//find current time	
			Tv_TmaIdx	=	E_GrDvrFs1TmaTmpCnt;
			do 
			{
				//index
				Tv_TmaIdx --;
				//check find current
				if ( E_GrDvrRecHndlNone != m_TmaTmp[Tv_TmaIdx].HndlRec )
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
BOOL8	Cls_GrDvrFs1Mng::DirectWrite( __u8 A_DevType, __u64 A_Hndl, void* A_PtrBuf, __u32 A_Size )
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
__u64	Cls_GrDvrFs1Mng::RecBeginHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		Def_GrTime	Tv_TimeSt;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

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
__u64	Cls_GrDvrFs1Mng::RecEndHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

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
void	Cls_GrDvrFs1Mng::LcEvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
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
__u32	Cls_GrDvrFs1Mng::LcEvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
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
BOOL8	Cls_GrDvrFs1Mng::LcUcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_TypeIdx;
	// code --------------------
		Tv_Result	=	FALSE;
		for ( Tv_TypeIdx=0;Tv_TypeIdx < E_GrDvrFs1CfgDskModeCnt;Tv_TypeIdx++ )
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
BOOL8	Cls_GrDvrFs1Mng::LcUcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_TypeIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		for ( Tv_TypeIdx=0;Tv_TypeIdx < E_GrDvrFs1CfgDskModeCnt;Tv_TypeIdx++ )
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
BOOL8	Cls_GrDvrFs1Mng::LcUwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs1Mng::LcUwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
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
__u8	Cls_GrDvrFs1Mng::LcTempGet( __u8 A_DskIdx )
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
BOOL8	Cls_GrDvrFs1Mng::WbaWrite( void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs1Mng::WbaRead( void* A_PtrBuf, __u32 A_Size )
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
BOOL8	Cls_GrDvrFs1Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch( ((Ptr_GrDvrFs1MngMsgBase)A_PtrMsg)->Cmd )
			{
				case	E_GrDvrFs1MngCmdFormat:
					*(((Ptr_GrDvrFs1MngMsgFormat)A_PtrMsg)->PtrRt)	=	LcFormat( ((Ptr_GrDvrFs1MngMsgFormat)A_PtrMsg)->DskIdx,
											((Ptr_GrDvrFs1MngMsgFormat)A_PtrMsg)->IsWithoutLog );
					break;
				case	E_GrDvrFs1MngCmdTmaRead:
					*(((Ptr_GrDvrFs1MngMsgTmaRead)A_PtrMsg)->PtrRt)	=	LcTmaRead( ((Ptr_GrDvrFs1MngMsgTmaRead)A_PtrMsg)->DevType,
											((Ptr_GrDvrFs1MngMsgTmaRead)A_PtrMsg)->Time, ((Ptr_GrDvrFs1MngMsgTmaRead)A_PtrMsg)->PtrBuf,
											((Ptr_GrDvrFs1MngMsgTmaRead)A_PtrMsg)->Cnt );
					break;
				case	E_GrDvrFs1MngCmdRecStat:
					LcRecStatusGet( (Ptr_GrDvrFs1RecStat)(((Ptr_GrDvrFs1MngMsgRecStat)A_PtrMsg)->PtrBuf) );
					break;
				case	E_GrDvrFs1MngCmdSetup:
					LcSetup( ((Ptr_GrDvrFs1MngMsgSetup)A_PtrMsg)->PtrDskTbl, ((Ptr_GrDvrFs1MngMsgSetup)A_PtrMsg)->DiskCnt,
										((Ptr_GrDvrFs1MngMsgSetup)A_PtrMsg)->IsNormalOvwt, ((Ptr_GrDvrFs1MngMsgSetup)A_PtrMsg)->IsMirrorOvwt	);
					break;
				case	E_GrDvrFs1MngCmdUcaWrite:
					*(((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->PtrRt)	=	LcUcaWrite( ((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->Pos,
												((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFs1MngCmdUcaRead:
					*(((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->PtrRt)	=	LcUcaRead( ((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->Pos,
												((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFs1MngMsgUcaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFs1MngCmdUwaWrite:
					*(((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->PtrRt)	=	LcUwaWrite( ((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->Pos,
																			((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFs1MngCmdUwaRead:
					*(((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->PtrRt)	=	LcUwaRead( ((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->Pos,
																			((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->PtrBuf, ((Ptr_GrDvrFs1MngMsgUwaRw)A_PtrMsg)->Size );
					break;
				case	E_GrDvrFs1MngCmdTimeRng:
					*(((Ptr_GrDvrFs1MngMsgTimeRng)A_PtrMsg)->PtrRt)	=	RecTimeRangeGet( ((Ptr_GrDvrFs1MngMsgTimeRng)A_PtrMsg)->DevType,
																			((Ptr_GrDvrFs1MngMsgTimeRng)A_PtrMsg)->PtrTimeSt, ((Ptr_GrDvrFs1MngMsgTimeRng)A_PtrMsg)->PtrTimeEd );
					break;
				case	E_GrDvrFs1MngCmdLogAdd:
					LcEvtLogAdd( &((Ptr_GrDvrFs1MngMsgLogAdd)A_PtrMsg)->Log );
					break;
				case	E_GrDvrFs1MngCmdLogGet:
					*(((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->PtrRt)	=	LcEvtLogGet( ((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->DevType,
																			((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->TimeEd,
																			((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->EvtMask, ((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->ChMask,
																			((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->PtrRtLog,	((Ptr_GrDvrFs1MngMsgLogGet)A_PtrMsg)->MaxCnt );
					break;
				case	E_GrDvrFs1MngCmdHndlBegin:
					*(((Ptr_GrDvrFs1MngMsgHndlGet)A_PtrMsg)->PtrRt)	=	RecBeginHndlGet( ((Ptr_GrDvrFs1MngMsgHndlGet)A_PtrMsg)->DevType );
					break;
				case	E_GrDvrFs1MngCmdHndlEnd:
					*(((Ptr_GrDvrFs1MngMsgHndlGet)A_PtrMsg)->PtrRt)	=	RecEndHndlGet( ((Ptr_GrDvrFs1MngMsgHndlGet)A_PtrMsg)->DevType );
					break;
				case	E_GrDvrFs1MngCmdHndlMinute:
					*(((Ptr_GrDvrFs1MngMsgHndlByTime)A_PtrMsg)->PtrRt)	=	RecHndlByTimeMin( ((Ptr_GrDvrFs1MngMsgHndlByTime)A_PtrMsg)->DevType,
																													((Ptr_GrDvrFs1MngMsgHndlByTime)A_PtrMsg)->Time );
					break;
				case	E_GrDvrFs1MngCmdHndlFindFw:
					*(((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->PtrRt)	=	FindHndlByMinFw( ((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->DevType,
						((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFs1MngCmdAtdSet:
					LcAutoDelTimeSet( ((Ptr_GrDvrFs1MngMsgAtdSet)A_PtrMsg)->DayCnt );
					break;
				case	E_GrDvrFs1MngCmdTempGet:
					*(((Ptr_GrDvrFs1MngMsgTempGet)A_PtrMsg)->PtrRt)	=	LcTempGet( ((Ptr_GrDvrFs1MngMsgTempGet)A_PtrMsg)->DskIdx );
					break;
				case	E_GrDvrFs1MngCmdHndlFindBw:
					*(((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->PtrRt)	=	FindHndlByMinBw( ((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->DevType,
						((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs1MngMsgHndlByRng)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFs1MngCmdDataSize:
					*(((Ptr_GrDvrFs1MngMsgDataSize)A_PtrMsg)->PtrRt)	=	CalcDataSize( ((Ptr_GrDvrFs1MngMsgDataSize)A_PtrMsg)->DevType,
						((Ptr_GrDvrFs1MngMsgDataSize)A_PtrMsg)->ChMap, ((Ptr_GrDvrFs1MngMsgDataSize)A_PtrMsg)->TimeSt, ((Ptr_GrDvrFs1MngMsgDataSize)A_PtrMsg)->TimeEd );
					break;
				case	E_GrDvrFs1MngCmdVssGet:
					*(((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtRslt)	=	LcVssItemGet( 
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtHndlRec, 
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtTime,
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtCh, ((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtCmd,
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtIsArmed, 
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtEtty,
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtSeq,
						((Ptr_GrDvrFs1MngMsgVssGet)A_PtrMsg)->PtrRtFrmCnt );
					break;
				case	E_GrDvrFs1MngCmdVssPop:
					LcVssItemPop();
					break;
				case	E_GrDvrFs1MngCmdVssAdd:
					VssItemAdd( ((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Hndl, ((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Time,
						((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Ch, ((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Cmd,
						((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->IsArmed, ((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Etty,
						((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->Seq, ((Ptr_GrDvrFs1MngMsgVssAdd)A_PtrMsg)->FrmCnt );
					break;
				case E_GrDvrFs1MngCmdVssReset:
					LcVssReset();
					break;
				case	E_GrDvrFs1MngCmdMainAllErr:
					*(((Ptr_GrDvrFs1MngMsgMainAllErr)A_PtrMsg)->PtrRtRslt)	=	IsMainDskAllErr();
					break;
				case E_GrDvrFs1MngCmdReadFw:
					ImmReadForward( ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs1MngCmdReadBw:
					ImmReadBackward( ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs1MngCmdReadBegin:
					ImmReadBegin( ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs1MngCmdReadEnd:
					ImmReadEnd( ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtl)A_PtrMsg)->PtrCtrl );
					break;
				case E_GrDvrFs1MngCmdReadMinFw:
					ImmReadByMinFw( ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->PtrCtrl, ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs1MngCmdReadMinBw:
					ImmReadByMinBw( ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->PtrCtrl, ((Ptr_GrDvrFs1MngMsgDvCtlTm)A_PtrMsg)->Time );
					break;
				case E_GrDvrFs1MngCmdReadHndlFw:
					ImmReadByHndlFw( ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->PtrCtrl, ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->Hndl );
					break;
				case E_GrDvrFs1MngCmdReadHndlBw:
					ImmReadByHndlBw( ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->DevType, ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->PtrCtrl, ((Ptr_GrDvrFs1MngMsgDvCtlHd)A_PtrMsg)->Hndl );
					break;
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::Format( __u8 A_DskIdx, BOOL8 A_IsWithoutLog )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgFormat	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdFormat;
		Tv_Msg.DskIdx					=	A_DskIdx;
		Tv_Msg.IsWithoutLog		=	A_IsWithoutLog;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::TmaRead( __u8 A_DevType, Def_GrTime A_Time, void* A_PtrBuf, __u32 A_Cnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgTmaRead	Tv_Msg;
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
			Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdTmaRead;
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
								Tv_PtrThumb->HndlRec	=	E_GrDvrRecHndlNone;
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
void	Cls_GrDvrFs1Mng::LcRecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl )
{
	// local -------------------
		__u8	Tv_DevType;
	// code --------------------
		//check device type
		for ( Tv_DevType=0;Tv_DevType < E_GrDvrFs1CfgDskModeCnt;Tv_DevType++ )
		{
			if ( NULL != m_GrpTbl[Tv_DevType] )
			{
				m_GrpTbl[Tv_DevType]->RecStatusGet( A_PtrStatTbl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::RecStatusGet( Ptr_GrDvrFs1RecStat A_PtrStatTbl )
{
	// local -------------------
		St_GrDvrFs1MngMsgRecStat	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdRecStat;
		Tv_Msg.PtrBuf					=	(void*)A_PtrStatTbl;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::Setup( __u8* A_PtrDskTbl, __u8 A_Cnt, BOOL8 A_IsNormalOvwt, BOOL8 A_IsMirrorOvwt )
{
	// local -------------------
		St_GrDvrFs1MngMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdSetup;
		Tv_Msg.PtrDskTbl			=	A_PtrDskTbl;
		Tv_Msg.DiskCnt				=	A_Cnt;
		Tv_Msg.IsNormalOvwt		=	A_IsNormalOvwt;
		Tv_Msg.IsMirrorOvwt		=	A_IsMirrorOvwt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::UcaWrite( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgUcaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdUcaWrite;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::UcaRead( __u32 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgUcaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdUcaRead;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::DirvePathSet( __u8 A_Idx, __s8* A_StrPath )
{
		if ( E_GrDvrFs1MaxDriveCnt > A_Idx )
		{
			GrStrCopy( m_DskCfg.DevFnTbl[A_Idx], A_StrPath );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::UwaWrite( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgUwaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdUwaWrite;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::UwaRead( __u64 A_Pos, void* A_PtrBuf, __u32 A_Size )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgUwaRw	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd		=	m_MsgAloc;
		Tv_Msg.Hd.Cmd					=	E_GrDvrFs1MngCmdUwaRead;
		Tv_Msg.Pos						=	A_Pos;
		Tv_Msg.PtrBuf					=	A_PtrBuf;
		Tv_Msg.Size						=	A_Size;
		Tv_Msg.PtrRt					=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::UwaDiskPosGet( void )
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		Tv_Result	=	0;

		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFs1CfgDskNormal]->UwaDiskPosGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Mng::UwaDiskIndexGet( void )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0;
		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFs1CfgDskNormal]->UwaDiskIndexGet();
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::RecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
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
BOOL8	Cls_GrDvrFs1Mng::SafeRecTimeRangeGet( __u8 A_DevType, Def_GrTime* A_PtrRtTimeSt, Def_GrTime* A_PtrRtTimeEd )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgTimeRng	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdTimeRng;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrTimeSt		=	A_PtrRtTimeSt;
		Tv_Msg.PtrTimeEd		=	A_PtrRtTimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::EvtLogAdd( Ptr_GrDvrLogItm A_PtrItem )
{
	// local -------------------
		St_GrDvrFs1MngMsgLogAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdLogAdd;
		Tv_Msg.Log					=	*A_PtrItem;
		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Mng::EvtLogGet( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u32 A_EvtMask, __u32 A_ChMask, Ptr_GrDvrLogItm A_PtrRt, __u32 A_MaxCnt )
{
	// local -------------------
		__u32		Tv_Result;
		St_GrDvrFs1MngMsgLogGet	Tv_Msg;
	// code --------------------
		Tv_Result						=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdLogGet;
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
__u64	Cls_GrDvrFs1Mng::SafeRecBeginHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgHndlGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdHndlBegin;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::SafeRecEndHndlGet( __u8 A_DevType )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgHndlGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdHndlEnd;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::SafeRecHndlByTimeMin( __u8 A_DevType, Def_GrTime A_Time )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgHndlByTime	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdHndlMinute;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.Time					=	A_Time;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::SafeFindHndlByMinFw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgHndlByRng	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdHndlFindFw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::SafeFindHndlByMinBw( __u8 A_DevType, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgHndlByRng	Tv_Msg;
	// code --------------------
		Tv_Result	=	E_GrDvrRecHndlNone;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdHndlFindBw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrDvrFs1Mng::FccStreamGet( void )
{
		return	m_DskCfg.FccStrm;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::LcAutoDelTimeUpdt( void )
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
Def_GrTime	Cls_GrDvrFs1Mng::AutoDelLimTimeGet( void )
{
		return	m_AtdLimTime;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::LcAutoDelTimeSet( __u32 A_DayCnt )
{
	// local -------------------
	// code --------------------
		m_AutoDelTime		=	A_DayCnt * E_GrTimeSecOfDay;
		LcAutoDelTimeUpdt();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::AutoDelTimeSet( __u32 A_DayCnt )
{
	// local -------------------
		St_GrDvrFs1MngMsgAtdSet	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdAtdSet;
		Tv_Msg.DayCnt				=	A_DayCnt;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Mng::TemperatureGet( __u8 A_DskIdx )
{
	// local -------------------
		__u8	Tv_Result;
		St_GrDvrFs1MngMsgTempGet	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdTempGet;
		Tv_Msg.DskIdx				=	A_DskIdx;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::IsDiskFull()
{
		return	m_IsDiskFull;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::IsRecorded( BOOL8 A_IsReset )
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
BOOL8	Cls_GrDvrFs1Mng::IsWdgChk( BOOL8 A_IsReset )
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
BOOL8	Cls_GrDvrFs1Mng::IsRecAble( void )
{
		return	m_IsRecAble;
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs1Mng::CalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
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
__u64	Cls_GrDvrFs1Mng::SafeCalcDataSize( __u8 A_DevType, __u16 A_ChMap, Def_GrTime A_TimeSt, Def_GrTime A_TimeEd )
{
	// local -------------------
		__u64	Tv_Result;
		St_GrDvrFs1MngMsgDataSize	Tv_Msg;
	// code --------------------
		Tv_Result	=	0;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdDataSize;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.ChMap				=	A_ChMap;
		Tv_Msg.TimeSt				=	A_TimeSt;
		Tv_Msg.TimeEd				=	A_TimeEd;
		Tv_Msg.PtrRt				=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::IsVssAble( void )
{
		return	m_IsVssAble;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::VssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, 
																	__u8	A_Etty, __u8 A_Seq, __u8 A_FrmCnt )
{
	// local -------------------
		Ptr_GrDvrFs1VssItm	Tv_PtrItm;
		__u32	Tv_Pos;
	// code --------------------
		Tv_PtrItm	=	(Ptr_GrDvrFs1VssItm)GrBufQueGetInPtr( m_PtrVssQue );
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
			LcUwaWrite( (__u64)Tv_Pos, (void*)Tv_PtrItm, E_GrDvrFs1VssItmSize );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeVssItemAdd( __u64	A_Hndl, Def_GrTime A_Time, __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, 
	__u8	A_Etty,	__u8 A_Seq, __u8 A_FrmCnt )
{
	// local -------------------
		St_GrDvrFs1MngMsgVssAdd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdVssAdd;
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
BOOL8	Cls_GrDvrFs1Mng::LcVssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, 
																		__u8* A_PtrRtCmd, BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, 
																		__u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		Ptr_GrDvrFs1VssItm	Tv_PtrItm;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_PtrItm	=	(Ptr_GrDvrFs1VssItm)GrBufQueGetOutPtr( m_PtrVssQue );
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
void	Cls_GrDvrFs1Mng::LcVssItemPop( void )
{
	// local -------------------
		Ptr_GrDvrFs1VssItm	Tv_PtrItm;
	// code --------------------
		Tv_PtrItm	=	(Ptr_GrDvrFs1VssItm)GrBufQueGetOutPtr( m_PtrVssQue );
		if ( NULL != Tv_PtrItm )
		{
			GrBufQueOut( m_PtrVssQue );
			//save contorl area
			LcUwaWrite( 0, (void*)m_PtrVssQue, sizeof(St_GrBufQue) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::VssItemGet( __u64* A_PtrRtHndl, Def_GrTime* A_PtrRtTime, __u8* A_PtrRtCh, 
																	__u8* A_PtrRtCmd, BOOL8* A_PtrRtIsArmed, __u8*	A_PtrRtEtty, 
																	__u8* A_PtrRtSeq, __u8* A_PtrRtFrmCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgVssGet	Tv_Msg;
	// code --------------------
		Tv_Result		=	FALSE;

		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdVssGet;
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
void	Cls_GrDvrFs1Mng::VssItemPop( void )
{
	// local -------------------
		St_GrDvrFs1MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDvrFs1MngCmdVssPop;
		
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::LcVssReset( void )
{
		GrBufQueInit( m_PtrVssQue, sizeof(St_GrDvrFs1VssItm), m_VssSize );
		//save contorl area
		LcUwaWrite( 0, (void*)m_PtrVssQue, sizeof(St_GrBufQue) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::VssReset( void )
{
	// local -------------------
		St_GrDvrFs1MngMsgBase	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Cmd				=	E_GrDvrFs1MngCmdVssReset;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::IsMainDskAllErr( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	TRUE;

		if ( NULL != m_GrpTbl[E_GrDvrFs1CfgDskNormal] )
		{
			Tv_Result	=	m_GrpTbl[E_GrDvrFs1CfgDskNormal]->IsAllError();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs1Mng::IsSafeMainDskAllErr( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrFs1MngMsgMainAllErr	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdMainAllErr;
		Tv_Msg.PtrRtRslt		=	&Tv_Result;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		return	Tv_Result;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Mng::MissMapGet( void )
{
		return	m_MissMap;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs1Mng::NotFormatMapGet( void )
{
		return	m_NotFmtMap;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadForward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				m_GrpTbl[A_DevType]->ReadForward( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadBackward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				m_GrpTbl[A_DevType]->ReadBackward( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadBegin( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		if ( 0 != m_AutoDelTime )
		{
			ImmReadByMinFw( A_DevType, A_PtrCtrl, m_AtdLimTime );
		}
		else
		{
			A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
			//check device type
			if ( E_GrDvrRecDevMirror >= A_DevType )
			{
				if ( NULL != m_GrpTbl[A_DevType] )
				{
					m_GrpTbl[A_DevType]->ReadBegin( A_PtrCtrl );
				}
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadEnd( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		//check device type
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				m_GrpTbl[A_DevType]->ReadEnd( A_PtrCtrl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		Def_GrTime	Tv_TimeSt;
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				Tv_TimeSt	=	A_Time;
				if ( 0 != m_AutoDelTime )
				{
					if ( Tv_TimeSt < m_AtdLimTime )
					{
						Tv_TimeSt	=	m_AtdLimTime;
					}
				}
				m_GrpTbl[A_DevType]->ReadByTimeFw( A_PtrCtrl, Tv_TimeSt );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				if ( 0 != m_AutoDelTime )
				{
					if ( A_Time > m_AtdLimTime )
					{
						m_GrpTbl[A_DevType]->ReadByTimeBw( A_PtrCtrl, A_Time, m_AtdLimTime );
					}
				}
				else
				{
					m_GrpTbl[A_DevType]->ReadByTimeBw( A_PtrCtrl, A_Time, 0 );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
	// code --------------------
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatEnd;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				m_GrpTbl[A_DevType]->ReadByHndlFw( A_PtrCtrl, A_Hndl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::ImmReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
		A_PtrCtrl->Stat	=	E_GrDvrSeekStatBegin;
		if ( E_GrDvrRecDevMirror >= A_DevType )
		{
			if ( NULL != m_GrpTbl[A_DevType] )
			{
				m_GrpTbl[A_DevType]->ReadByHndlBw( A_PtrCtrl, A_Hndl );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadForward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtl	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadFw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadBackward( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtl	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadBw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadBegin( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtl	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadBegin;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadEnd( __u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtl	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadEnd;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadByMinFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtlTm	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadMinFw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;
		Tv_Msg.Time					=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadByMinBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, Def_GrTime A_Time )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtlTm	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadMinBw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;
		Tv_Msg.Time					=	A_Time;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadByHndlFw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtlHd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadHndlFw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;
		Tv_Msg.Hndl					=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs1Mng::SafeReadByHndlBw(__u8 A_DevType, Ptr_GrDvrFs1SeekCtrl A_PtrCtrl, __u64 A_Hndl )
{
	// local -------------------
		St_GrDvrFs1MngMsgDvCtlHd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrFs1MngCmdReadHndlBw;
		Tv_Msg.DevType			=	A_DevType;
		Tv_Msg.PtrCtrl			=	A_PtrCtrl;
		Tv_Msg.Hndl					=	A_Hndl;

		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
