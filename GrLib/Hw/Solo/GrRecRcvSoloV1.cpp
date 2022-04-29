/*
	solo record recive

*/

//====================================================================
//include
#include	<Hw/Solo/GrRecRcvSoloV1.h>
#include	<GrTimeTool.h>
#include	<GrDebug.h>
#include	<GrDumyTool.h>

//====================================================================
//local const

#define E_GrRecRcvSoloV1DfltPrefrmMs		6000

#define E_GrRecRcvSoloV1VssHeadCnt			2
#define E_GrRecRcvSoloV1VssTailCnt			8

//#define E_GrRecRcvSoloV1MaxSizeMode			

//====================================================================
//local type


//====================================================================
//global var


//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrRecRcvSoloV1::Cls_GrRecRcvSoloV1( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDevSolo* A_DevSolo, 
																	 Cls_GrDvrFs1Mng* A_FsMng, BOOL8 A_IsUseVss ):
Cls_GrTaskCli()
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		m_TaskSvr		=	A_TaskSvr;
		m_ObjDev		=	A_DevSolo;
		m_FsMng			=	A_FsMng;
		m_IsVssUse	=	A_IsUseVss;

		m_FccStrm		=	m_FsMng->FccStreamGet();

		m_TimeSecLast		=	0;
		m_TimeMsecLast	=	0;
		m_PrefrmTick		=	GrTimeMsToTick( E_GrRecRcvSoloV1DfltPrefrmMs );

		m_PtrNewest			=	NULL;
		m_IsProced			=	FALSE;
		m_VdoChCnt			=	m_ObjDev->VdoChCnt();

		m_AdoChCnt			=	m_ObjDev->AdoChCnt();
		m_AdoCpAdd			=	(m_AdoChCnt - 1) * E_SoloAdoChUnitSize;
		m_AdoRecMap			=	0xFFFF;

		GrDumyZeroMem( m_VssTbl, sizeof(m_VssTbl) );

		m_MtnMapSize		=	0;
		if ( m_ObjDev->IsMtnMapGet() )
		{
			m_MtnMapSize		=	E_SoloEncMtnMapSize;
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrRecRcvMtnPluginCnt;Tv_WkIdx++ )
		{
			m_MtnPlugTbl[Tv_WkIdx]	=	NULL;
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			m_VdoAbleTbl[Tv_WkIdx]		=	FALSE;
			m_VopIdTbl[Tv_WkIdx]			=	0;
			m_EvtTbl[Tv_WkIdx]				=	0;
			m_RtlFrmSizeTbl[Tv_WkIdx]	=	0;
			m_VdoFrmSizeTbl[Tv_WkIdx]	=	0;
			m_SeqIdTbl[Tv_WkIdx]			=	0;
		}

		//clear packet buffer
		m_AdoSyncIdx	=	0;
		GrDumyZeroMem( (void*)m_AdoSyncTbl, sizeof(m_AdoSyncTbl)  );

		//get record queue
		m_PtrRecQue	=	(Ptr_GrBufRefQue)m_ObjDev->RecBufPtrGet();
		if ( NULL == m_PtrRecQue )
		{
			DbgMsgPrint( "Cls_GrRecRcvSoloV1:: Solo object need run DevConfg!\n" );
		}
		else
		{
			// calculate buffer limite
			m_BufLimite	=	(m_PtrRecQue->BufSize >> 1) + (m_PtrRecQue->BufSize >> 2);
			DbgMsgPrint( "Cls_GrRecRcvSoloV1:: Preframe buffer limite = %d!\n", m_BufLimite );
		}

		//check header size
		if ( E_SoloVencIdxTblSize < sizeof(St_GrDvrAvHd) )
		{
			DbgMsgPrint( "Cls_GrRecRcvSoloV1:: video frame header bigger than p2m pad = %d !\n", sizeof(St_GrDvrAvHd) );
		}
		if ( 0 != (0x07 & sizeof(St_GrRecRcvSoloV1AdoPkt) ) )
		{
			DbgMsgPrint( "Cls_GrRecRcvSoloV1:: audio packet not aligned = %d !\n", sizeof(St_GrRecRcvSoloV1AdoPkt) );
		}

		//vss reserve queu
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			GrBufQueInit( (Ptr_GrBufQue)m_VssQue[Tv_WkIdx], sizeof(St_GrRecRcvSoloV1VssRsv), E_GrRecRcvVssRsvBufSize );
		}

		//regist background
		m_TaskSvr->RegistTimerClient( this, 2000, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrRecRcvSoloV1::~Cls_GrRecRcvSoloV1()
{

		//unregist background
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistBkgClient( this );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrRecRcvSoloV1::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		//process newest
		if ( LcProcNewestFrame() )
		{
			Tv_Result	=	TRUE;
		}

		//check size over
		LcProcLastFrame();

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u32	Tv_WkIdx;
		BOOL8	Tv_IsEnc;
	// code --------------------
		// check watch dog
		if ( 0 == m_ObjDev->VencWatchdogCount() )
		{
			// check encode channel use
			Tv_IsEnc	=	FALSE;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_VdoChCnt;Tv_WkIdx++ )
			{
				if ( m_ObjDev->IsEncChAble( (__u8)Tv_WkIdx ) )
				{
					//watchdog reset
					Tv_IsEnc	=	TRUE;
					break;
				}
			}
			if ( Tv_IsEnc )
			{
				//watchdog
				m_ObjDev->CapBtwReset();
				DbgMsgPrint( "Cls_GrRecRcvSoloV1::RtlTimer - capture freeze, reset capture!\n" );
			}
			else
			{
				DbgMsgPrint( "Cls_GrRecRcvSoloV1::RtlTimer - capture channel not exist.\n" );
			}
		}
		m_ObjDev->VencWatchdogReset();		//watchdog counter reset
		// debug
		// DbgMsgPrint( "Last cap time = %8X : %8X\n", m_TimeSecLast, m_TimeMsecLast );
		
		// frame size table reset
#ifndef E_GrRecRcvSoloV1MaxSizeMode
		for ( Tv_WkIdx=0;Tv_WkIdx < m_VdoChCnt;Tv_WkIdx++ )
		{
			m_VdoFrmSizeTbl[Tv_WkIdx]	=	m_RtlFrmSizeTbl[Tv_WkIdx];
			m_RtlFrmSizeTbl[Tv_WkIdx]	=	0;
		}
#endif
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::LcProcMtnPlugin( void* A_PtrFrm )
{
	// local -------------------
		__u32*	Tv_PtrHwHd;
		__u8		Tv_Res;
		__u8		Tv_Ch;
		__u8		Tv_Vop;
		St_GrRecRcvV1MtnCbMsg	Tv_MtnCbMsg;
		__u32		Tv_WkIdx;
	// code --------------------
		//process motion map
		if ( 0 != m_MtnMapSize )
		{
			//get information
			Tv_PtrHwHd	=	(__u32*)A_PtrFrm;
			Tv_Ch				=	(__u8)((Tv_PtrHwHd[E_GrDrvSoloVencStat0BkIdx] >> 24) & 0x0F);
			Tv_Vop			=	(__u8)((Tv_PtrHwHd[E_GrDrvSoloVencStat0BkIdx] >> 22) & 0x03);
			//get resolution
			switch( 0xFFFF & Tv_PtrHwHd[E_GrDrvSoloVencStat1BkIdx] )
			{
				case	0x2C1E:
					Tv_Res	=	E_GrDvrVdoHighRes;		//ntsc high
					break;
				case	0x2C0F:
					Tv_Res	=	E_GrDvrVdoMidRes;		//ntsc mid
					break;
				case	0x160F:
					Tv_Res	=	E_GrDvrVdoLowRes;		//ntsc low
					break;
				case	0x2C24:
					Tv_Res	=	E_GrDvrVdoHighRes | E_GrDvrVdoResPal;		//pal high
					break;
				case	0x2C12:
					Tv_Res	=	E_GrDvrVdoMidRes | E_GrDvrVdoResPal;		//pal mid
					break;
				case	0x1612:
					Tv_Res	=	E_GrDvrVdoLowRes | E_GrDvrVdoResPal;		//pal low
					break;
				default:
					Tv_Res	=	E_GrDvrVdoResBad;
					break;
			}

			//check i frame
			if ( E_GrDvrVencGopTypeI != Tv_Vop )
			{
				Tv_MtnCbMsg.Hd.Cmd	=	E_GrRecRcvMtnCallbackCmd;
				Tv_MtnCbMsg.Ch			=	Tv_Ch;
				Tv_MtnCbMsg.Scale		=	Tv_Res;
				Tv_MtnCbMsg.PtrMap	=	(void*)( (__u32)A_PtrFrm + E_SoloVencIdxTblSize );
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrRecRcvMtnPluginCnt;Tv_WkIdx++ )
				{
					if ( NULL != m_MtnPlugTbl[Tv_WkIdx] )
					{
						//do callback
						m_MtnPlugTbl[Tv_WkIdx]->RtlCallback( (Ptr_GrTaskMsgHd)&Tv_MtnCbMsg );
					}
				}
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrRecRcvSoloV1::LcProcVdoFrame( void* A_PtrFrm, BOOL8 A_IsPreFlush )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32*	Tv_PtrHwHd;
		__u8		Tv_Res;
		__u32		Tv_CodeSize;
		__u8		Tv_Ch;
		__u8		Tv_Vop;
		__u8		Tv_Speed;
		__u8		Tv_Evt;
		__u32		Tv_WaterMark;
		Ptr_GrDvrAvHd	Tv_PtrFrm;	//frame header
		__u32		Tv_PktSize;
		__u32*	Tv_PtrTail;
		__u64*	Tv_PtrHndl;
		__u8		Tv_Idx;
		BOOL8		Tv_IsAdd;		//packet added
		Ptr_GrRecRcvSoloV1VssRsv	Tv_PtrVssItm;
	// code --------------------
		Tv_Result		=	TRUE;
		Tv_PtrHwHd	=	(__u32*)A_PtrFrm;
		//get event
		Tv_Evt			=	(__u8)(Tv_PtrHwHd[E_GrDrvSoloVencEvtBkIdx] & 0xFF);		//get event
		Tv_Ch				=	(__u8)((Tv_PtrHwHd[E_GrDrvSoloVencStat0BkIdx] >> 24) & 0x0F);

		//get information
		Tv_Vop				=	(__u8)((Tv_PtrHwHd[E_GrDrvSoloVencStat0BkIdx] >> 22) & 0x03);
		Tv_Speed			=	(__u8)((Tv_PtrHwHd[4] >> 20) & 0xFF);
		//get resolution
		switch( 0xFFFF & Tv_PtrHwHd[E_GrDrvSoloVencStat1BkIdx] )
		{
			case	0x2C1E:
				Tv_Res	=	E_GrDvrVdoHighRes;		//ntsc high
				Tv_Speed ++;
				break;
			case	0x2C0F:
				Tv_Res	=	E_GrDvrVdoMidRes;		//ntsc mid
				break;
			case	0x160F:
				Tv_Res	=	E_GrDvrVdoLowRes;		//ntsc low
				break;
			case	0x2C24:
				Tv_Res	=	E_GrDvrVdoHighRes | E_GrDvrVdoResPal;		//pal high
				Tv_Speed ++;
				break;
			case	0x2C12:
				Tv_Res	=	E_GrDvrVdoMidRes | E_GrDvrVdoResPal;		//pal mid
				break;
			case	0x1612:
				Tv_Res	=	E_GrDvrVdoLowRes | E_GrDvrVdoResPal;		//pal low
				break;
			default:
				Tv_Res	=	E_GrDvrVdoResBad;
				break;
		}
		//patch speed
		if ( 0 == Tv_Speed )
		{
			Tv_Speed ++;
		}

		//check and update event
		m_EvtTbl[Tv_Ch]	=	Tv_Evt;

		//get code size
		Tv_CodeSize		=	Tv_PtrHwHd[E_GrDrvSoloVencStat0BkIdx] & 0x0FFFFF;
		//check current frame
		Tv_IsAdd	=	FALSE;
		if ( (E_GrDvrVdoResBad != Tv_Res) && ( (E_GrDvrVencGopTypeI == Tv_Vop) || m_VdoAbleTbl[Tv_Ch] ) )
		{
			Tv_Result	=	FALSE;
			//check write able
			if ( (0 != Tv_Evt) || A_IsPreFlush )
			{
				// gop id
				if ( E_GrDvrVencGopTypeI == Tv_Vop )
				{
					m_VopIdTbl[Tv_Ch] ++;
				}
				//sequence id
				m_SeqIdTbl[Tv_Ch] ++;
				//get more info
				m_TimeSecLast		=	GrTimeSysTimeToTime( Tv_PtrHwHd[5] , &m_IsSmLast );
				m_TimeMsecLast	=	Tv_PtrHwHd[6] >> 10;	//convert usec to msec
				Tv_WaterMark		=	Tv_PtrHwHd[8];

				//make header
				Tv_PtrFrm	=	(Ptr_GrDvrAvHd)( (__u32)A_PtrFrm + E_SoloVencIdxTblSize + m_MtnMapSize - sizeof(St_GrDvrAvHd) );
				Tv_PtrFrm->TimeSec		=	m_TimeSecLast;
				Tv_PtrFrm->TimeMsec		=	m_TimeMsecLast;
				Tv_PtrFrm->IsSm				=	(BOOL8)m_IsSmLast;
				Tv_PtrFrm->Ch					=	Tv_Ch;
				Tv_PtrFrm->Type				=	Tv_Vop;
				Tv_PtrFrm->Evt				=	Tv_Evt;
				Tv_PtrFrm->SkipCnt		=	Tv_Speed;
				Tv_PtrFrm->Res				=	Tv_Res;
				Tv_PtrFrm->VopId			=	m_VopIdTbl[Tv_Ch];
				Tv_PtrFrm->SeqId			=	m_SeqIdTbl[Tv_Ch];
				Tv_PtrFrm->DataSize		=	Tv_CodeSize;
				Tv_PtrFrm->Wtmk				=	Tv_WaterMark;
				Tv_PtrFrm->IsNosig		=	FALSE;
				Tv_PtrFrm->Fcc				=	m_FccStrm;

				//make stream tail
				Tv_PktSize	=	E_GrDvrFs1StrmAlignMask & (Tv_CodeSize + sizeof(St_GrDvrAvHd) + E_GrDvrRecTailMarkSize + E_GrDvrFs1StrmAlignSize - 1 );
				Tv_PtrTail	=	(__u32*)( (__u32)Tv_PtrFrm +  Tv_PktSize - E_GrDvrRecTailMarkSize );
				*Tv_PtrTail	=	E_GrDvrRecTailMarkId | Tv_PktSize;

				//debug - update frame size
#ifndef E_GrRecRcvSoloV1MaxSizeMode
				if ( 0 == m_RtlFrmSizeTbl[Tv_Ch] )
				{
					m_RtlFrmSizeTbl[Tv_Ch]	=	Tv_PtrFrm->DataSize;
				}
				else
				{
					m_RtlFrmSizeTbl[Tv_Ch]	=	(m_RtlFrmSizeTbl[Tv_Ch] + Tv_PtrFrm->DataSize) >> 1;
				}
#else
				if ( Tv_PtrFrm->DataSize > m_VdoFrmSizeTbl[Tv_Ch] )
				{
					m_VdoFrmSizeTbl[Tv_Ch]	=	Tv_PtrFrm->DataSize;
				}
#endif
				//vss
				Tv_PtrHndl	=	NULL;
				if ( m_IsVssUse )
				{
					//add pframe count
					Tv_Idx	=	m_VssTbl[Tv_Ch].IhndlIdx;
					m_VssTbl[Tv_Ch].Time	=	m_TimeSecLast;	//update last time
					if ( E_GrDvrVencGopTypeI == Tv_Vop )
					{
						Tv_Idx	=	Tv_Idx ^ 1;
						m_VssTbl[Tv_Ch].IhndlIdx	=	Tv_Idx;
						m_VssTbl[Tv_Ch].His[Tv_Idx].PfrmCnt	=	0;
						Tv_PtrHndl	=	&m_VssTbl[Tv_Ch].His[Tv_Idx].Hndl;
					}
					else
					{
						m_VssTbl[Tv_Ch].His[Tv_Idx].PfrmCnt ++;
					}
				}
				//write packet and update success
				Tv_IsAdd	=	(BOOL8)m_FsMng->PacketAdd( Tv_PtrFrm, Tv_PtrHndl );
				//vss process frame count
				if ( m_IsVssUse && Tv_IsAdd )
				{
					//check exist reserve
					Tv_PtrVssItm	=	(Ptr_GrRecRcvSoloV1VssRsv)GrBufQueGetOutPtr( (Ptr_GrBufQue)m_VssQue[Tv_Ch] );
					if ( NULL != Tv_PtrVssItm )
					{
						//update process count
						(Tv_PtrVssItm->ProcCnt) ++;
						if ( Tv_PtrVssItm->ProcCnt >= Tv_PtrVssItm->FrmCnt )
						{
							//add item
							m_FsMng->VssItemAdd( Tv_PtrVssItm->Hndl, Tv_PtrVssItm->Time, Tv_Ch,
								Tv_PtrVssItm->Cmd, Tv_PtrVssItm->IsArmed, Tv_PtrVssItm->Etty,
								Tv_PtrVssItm->Seq, Tv_PtrVssItm->FrmCnt );
							// release reserve
							GrBufQueOut( (Ptr_GrBufQue)m_VssQue[Tv_Ch] );
						}
					}
				}
				//success
				Tv_Result		=	TRUE;
			}
		}
		//update packet added
		m_VdoAbleTbl[Tv_Ch]	=	Tv_IsAdd;

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::LcProcAdoFrame( void* A_PtrFrm, BOOL8 A_IsPreFlush )
{
	// local -------------------
		__u8		Tv_Ch;
		void*		Tv_PtrCol;
		__u32*	Tv_PtrSr;
		__u32*	Tv_PtrTg;
		__u32		Tv_ElePos;
		__u32		Tv_CpPos;
		__u32		Tv_MaskCh;
		__s32			Tv_SyncIdx;
	// code --------------------
		//check current time
		if ( 0 != m_TimeSecLast )
		{
			Tv_SyncIdx	=	m_AdoSyncIdx - (E_GrRecRcvSoloV1SyncCnt >> 1);
			if ( 0 > Tv_SyncIdx )
			{
				Tv_SyncIdx	=	Tv_SyncIdx + E_GrRecRcvSoloV1SyncCnt;
			}
			
			Tv_PtrCol	=	(void*)( (__u32)A_PtrFrm + E_GrDrvSoloP2mAvHeadSize);
			Tv_MaskCh	=	1;
			for ( Tv_Ch=0;Tv_Ch < m_AdoChCnt;Tv_Ch++ )
			{
				//packet reserved
				m_AdoSyncTbl[Tv_SyncIdx].ChTbl[Tv_Ch].HndlNrm	=	E_GrDvrRecHndlNone;
				m_AdoSyncTbl[Tv_SyncIdx].ChTbl[Tv_Ch].HndlMir	=	E_GrDvrRecHndlNone;
				//check record able
				if ( 0 != (Tv_MaskCh & m_AdoRecMap) )
				{
					//check schedule
					if ( (0 != m_EvtTbl[Tv_Ch]) || A_IsPreFlush )
					{
						//packet reserved
						m_AdoSyncTbl[Tv_SyncIdx].TimeSec	=	m_TimeSecLast;
						m_AdoSyncTbl[Tv_SyncIdx].TimeMsec	=	m_TimeMsecLast;
						m_AdoSyncTbl[Tv_SyncIdx].IsSm			=	m_IsSmLast;	
						m_FsMng->PacketReserve( sizeof(St_GrRecRcvSoloV1AdoPkt), 
																		&m_AdoSyncTbl[Tv_SyncIdx].ChTbl[Tv_Ch].HndlNrm, &m_AdoSyncTbl[Tv_SyncIdx].ChTbl[Tv_Ch].HndlMir );
					}
				}

				//write packet
				if ( (E_GrDvrRecHndlNone != m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlNrm) || (E_GrDvrRecHndlNone != m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlMir) )
				{
					//make packet
					m_AdoPktBuf.TailMark	=	E_GrDvrRecTailMarkId | sizeof(St_GrRecRcvSoloV1AdoPkt);	//tail
					//set header
					m_AdoPktBuf.Hd.Fcc			=	m_FccStrm;
					m_AdoPktBuf.Hd.Ch				=	Tv_Ch;
					m_AdoPktBuf.Hd.Type			=	E_GrDvrAvTypAdo;
					m_AdoPktBuf.Hd.Evt			=	m_EvtTbl[Tv_Ch];
					m_AdoPktBuf.Hd.IsSm			=	(BOOL8)m_AdoSyncTbl[m_AdoSyncIdx].IsSm;
					m_AdoPktBuf.Hd.TimeSec	=	m_AdoSyncTbl[m_AdoSyncIdx].TimeSec;
					m_AdoPktBuf.Hd.TimeMsec	=	m_AdoSyncTbl[m_AdoSyncIdx].TimeMsec;
					m_AdoPktBuf.Hd.DataSize	=	E_GrDrvSoloAdoClustSize;
					m_AdoPktBuf.Hd.IsNosig	=	FALSE;

					//set data
					Tv_PtrSr	=	(__u32*)Tv_PtrCol;
					Tv_PtrTg	=	(__u32*)m_AdoPktBuf.Data;
					for ( Tv_ElePos=E_GrDrvSoloAdoClustElemCnt;0 != Tv_ElePos;Tv_ElePos-- )
					{
						//dumy copy pos
						for ( Tv_CpPos = (E_SoloAdoChUnitSize >> 2);0 != Tv_CpPos;Tv_CpPos-- )
						{
							*Tv_PtrTg	=	*Tv_PtrSr;
							//next
							Tv_PtrSr ++;
							Tv_PtrTg ++;
						}
						//next
						Tv_PtrSr	=	(__u32*)( (__u32)Tv_PtrSr + m_AdoCpAdd );
					}

#ifndef LINUX_APP
					// emulate debug
					m_AdoPktBuf.Data[0]	=	'A';
					m_AdoPktBuf.Data[1]	=	'D';
					m_AdoPktBuf.Data[2]	=	'O';
					m_AdoPktBuf.Data[3]	=	' ';
					m_AdoPktBuf.Data[4]	=	'D';
					m_AdoPktBuf.Data[5]	=	'A';
					m_AdoPktBuf.Data[6]	=	'T';
					m_AdoPktBuf.Data[7]	=	'A';
#endif

					//write normal
					if ( E_GrDvrRecHndlNone != m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlNrm )
					{
						m_FsMng->DirectWrite( E_GrDvrRecDevNoraml, m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlNrm, &m_AdoPktBuf, sizeof(m_AdoPktBuf) );
					}
					//write mirror
					if ( E_GrDvrRecHndlNone != m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlNrm )
					{
						m_FsMng->DirectWrite( E_GrDvrRecDevMirror, m_AdoSyncTbl[m_AdoSyncIdx].ChTbl[Tv_Ch].HndlMir, &m_AdoPktBuf, sizeof(m_AdoPktBuf) );
					}
				}
				
				//next
				Tv_PtrCol	=	(void*)((__u32)Tv_PtrCol + E_SoloAdoChUnitSize);
				Tv_MaskCh	=	Tv_MaskCh << 1;
			}
			//update info
			m_AdoSyncIdx ++;
			if ( E_GrRecRcvSoloV1SyncCnt <= m_AdoSyncIdx )
			{
				m_AdoSyncIdx	=	0;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::PreframeTimeSet( __u32 A_Sec )
{
		m_PrefrmTick	=	GrTimeMsToTick( (A_Sec + 1) * 1000 );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrRecRcvSoloV1::LcProcNewestFrame( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32*	Tv_PtrFrm;
		__u8	Tv_Ch;
	// code --------------------
		Tv_Result	=	FALSE;

		//get now frame
		Tv_PtrFrm	=	(__u32*)m_PtrNewest;
		if ( NULL == m_PtrNewest )
		{
			//get last frame
			Tv_PtrFrm		=	(__u32*)GrBufRqGetRefPtr( m_PtrRecQue );
			m_PtrNewest	=	(void*)Tv_PtrFrm;
			m_IsProced	=	FALSE;
		}
		else if ( m_IsProced )
		{
			//get next frame
			Tv_PtrFrm	=	(__u32*)GrBufRqGetRefPtr( m_PtrRecQue );
			if ( NULL != Tv_PtrFrm )
			{
				if ( (__u32)m_PtrNewest != (__u32)Tv_PtrFrm )
				{
					m_PtrNewest	=	Tv_PtrFrm;
					m_IsProced	=	FALSE;
				}
				else
				{
					GrBufRqRefNext( m_PtrRecQue );
					Tv_PtrFrm	=	NULL;
				}
			}
		}
		
		//check processable
		if ( NULL != Tv_PtrFrm )
		{
			Tv_Result	=	TRUE;
			//chekc video frame
			if ( E_GrDrvSoloP2mVdoFcc == *Tv_PtrFrm )
			{
				//video frame
				//motion process
				LcProcMtnPlugin( (void*)Tv_PtrFrm );
				//check record attribute
				if ( 0 != Tv_PtrFrm[E_GrDrvSoloVencEvtBkIdx] )
				{
					//check exist event
					if ( 0 != (E_GrDvrRecAtbEvtMask & Tv_PtrFrm[E_GrDrvSoloVencEvtBkIdx]) )
					{
						//flush preframe
						//LcFlushPreframes();
						// update attribute
						Tv_Ch				=	(__u8)((Tv_PtrFrm[E_GrDrvSoloVencStat0BkIdx] >> 24) & 0x0F);
						LcMarkEvtPreframes( Tv_Ch, Tv_PtrFrm[E_GrDrvSoloVencEvtBkIdx] & 0xFF );
					}
				}
				//new frame processed
				m_IsProced	=	TRUE;
			}
			else if ( E_GrDrvSoloP2mAdoFcc == *Tv_PtrFrm )
			{
				//update next
				m_IsProced	=	TRUE;
			}
			else if ( E_GrDrvSoloP2mBadFcc == *Tv_PtrFrm )
			{
				//bad frame - skip now frame
				//update next
				m_IsProced	=	TRUE;
			}
			else
			{
				//now transfer
				Tv_Result	=	FALSE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::LcFlushPreframes( void )
{
	// local -------------------
		__u32*	Tv_PtrFrm;
	// code --------------------
		if ( 0 != m_PrefrmTick )
		{
			//flush
			Tv_PtrFrm	=	(__u32*)GrBufRqGetOutPtr( m_PtrRecQue );
			while ( NULL != Tv_PtrFrm )
			{
				if ( E_GrDrvSoloP2mVdoFcc == *Tv_PtrFrm )
				{
					//video frame
					LcProcVdoFrame( (void*)Tv_PtrFrm, TRUE );
				}
				else if ( E_GrDrvSoloP2mAdoFcc == *Tv_PtrFrm )
				{
					//audio frame
					LcProcAdoFrame( (void*)Tv_PtrFrm, TRUE );
				}
				else if ( E_GrDrvSoloP2mBadFcc != *Tv_PtrFrm )
				{
					//not work
					break;
				}
				//next
				GrBufRqOut( m_PtrRecQue );
				Tv_PtrFrm	=	(__u32*)GrBufRqGetOutPtr( m_PtrRecQue );
			}
			//DbgMsgPrint("prflush\n");
		}
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::LcProcLastFrame( void )
{
	// local -------------------
		BOOL8		Tv_IsRun;
		__u32*	Tv_PtrFrm;
		__u32		Tv_UseSize;
		__u32		Tv_TickNow;
		__u32		Tv_TryCnt;
	// code --------------------
		Tv_IsRun	=	TRUE;
		Tv_TryCnt	=	1;
		while ( Tv_IsRun && (0 != Tv_TryCnt) )
		{
			Tv_IsRun	=	FALSE;
			//get last frame
			Tv_PtrFrm	=	(__u32*)GrBufRqGetOutPtr( m_PtrRecQue );
			if ( NULL != Tv_PtrFrm )
			{
				if ( E_GrDrvSoloP2mVdoFcc == *Tv_PtrFrm )
				{
					//video frame
					Tv_IsRun	=	LcProcVdoFrame( (void*)Tv_PtrFrm, FALSE );
					if ( !Tv_IsRun )
					{
						//no preframe
						if ( 0 == m_PrefrmTick )
						{
							Tv_IsRun	=	TRUE;
						}
						else
						{
							//check size over
							Tv_UseSize	=	GrBufRqGetUseSize( m_PtrRecQue );
							if (Tv_UseSize > m_BufLimite)
							{
								Tv_IsRun	=	TRUE;
								// DbgMsgPrint("szov - %d\n", m_BufLimite);
							}
							else
							{
								//check over time
								Tv_TickNow	=	GrTimeGetTick();
								if (m_PrefrmTick < (Tv_TickNow - Tv_PtrFrm[E_GrDrvSoloVencTickIdx]))
								{
									Tv_IsRun	=	TRUE;
									// DbgMsgPrint("tmov\n");
								}
							}	//if (Tv_UseSize > m_BufLimite)
						}	//if ( 0 == m_PrefrmTick )
					}	//if ( !Tv_Result )
				}
				else if ( E_GrDrvSoloP2mAdoFcc == *Tv_PtrFrm )
				{
					//audio frame
					LcProcAdoFrame( (void*)Tv_PtrFrm, FALSE );
					Tv_IsRun	=	TRUE;
				}
				else if ( E_GrDrvSoloP2mBadFcc == *Tv_PtrFrm )
				{
					//bad frame
					Tv_IsRun	=	TRUE;
				}

				if ( Tv_IsRun )
				{
					//next
					GrBufRqOut( m_PtrRecQue );
				}
			}
			// next
			Tv_TryCnt --;
		}

}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::AdoRecMapSet( __u32 A_Map )
{
		m_AdoRecMap		=	A_Map;
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::MotionPluginSet( __u8 A_Idx, Cls_GrTaskCli* A_ObjPlugin )
{
		if ( E_GrRecRcvMtnPluginCnt > A_Idx )
		{
			m_MtnPlugTbl[A_Idx]	=	A_ObjPlugin;
		}
}
//--------------------------------------------------------------------
__u32	Cls_GrRecRcvSoloV1::VdoFrmAverageSizeGet( __u8 A_Ch )
{
		return	m_VdoFrmSizeTbl[A_Ch];
}
//--------------------------------------------------------------------
BOOL8	Cls_GrRecRcvSoloV1::VssAdd( __u8 A_Ch, __u8 A_Cmd, BOOL8 A_IsArmed, __u8 A_Etty, __u8 A_CstFrmCnt )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Idx;
		//__u8	Tv_PvIdx;
		__u64	Tv_Hndl;
		__u8	Tv_FrmCnt;
		__u32	Tv_Mili;
		Ptr_GrRecRcvSoloV1VssRsv	Tv_PtrItm;
	// code --------------------
		Tv_Result	=	FALSE;

		//change queue type
		//check nosignal
		if ( 0 == (0x04 & A_Etty) )
		{
			//allocate queue
			Tv_PtrItm	=	(Ptr_GrRecRcvSoloV1VssRsv)GrBufQueGetInPtr( (Ptr_GrBufQue)m_VssQue[A_Ch] );
			if ( NULL != Tv_PtrItm )
			{
				//check i frame position
				Tv_Idx	=	m_VssTbl[A_Ch].IhndlIdx;
				Tv_Hndl	=	m_VssTbl[A_Ch].His[Tv_Idx].Hndl;
				Tv_FrmCnt	=	m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt;
				if ( E_GrRecRcvSoloV1VssHeadCnt > m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt )
				{
					Tv_Idx	=	Tv_Idx ^ 1;
					Tv_FrmCnt	=	Tv_FrmCnt + m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt;
					Tv_Hndl		=	m_VssTbl[A_Ch].His[Tv_Idx].Hndl;
				}
				if ( E_GrDvrRecHndlNone != Tv_Hndl )
				{
					//DbgMsgPrint( "vss add - preframe = %d, seq = %d\n", Tv_FrmCnt, m_SeqIdTbl[A_Ch] );
					//reserving vss
					Tv_PtrItm->Hndl			=	Tv_Hndl;
					Tv_PtrItm->Time			=	m_VssTbl[A_Ch].Time;
					Tv_PtrItm->Cmd			=	A_Cmd;
					Tv_PtrItm->IsArmed	=	A_IsArmed;
					Tv_PtrItm->Etty			=	A_Etty;
					Tv_PtrItm->Seq			=	m_SeqIdTbl[A_Ch];
					Tv_PtrItm->FrmCnt		=	Tv_FrmCnt + E_GrRecRcvSoloV1VssTailCnt;
					if ( 0 != A_CstFrmCnt )
					{
						Tv_PtrItm->FrmCnt	=	A_CstFrmCnt;
					}
					Tv_PtrItm->ProcCnt	=	Tv_FrmCnt;

					//queue in
					GrBufQueIn( (Ptr_GrBufQue)m_VssQue[A_Ch] );
				}
				else
				{
					DbgMsgPrint( "Cls_GrRecRcvSoloV1::VssAdd - not enough pre-frame.\n" );
				}
			}
			else
			{
				DbgMsgPrint( "Cls_GrRecRcvSoloV1::VssAdd - not enough reserve queue.\n" );
			}
		}
		else
		{
			//nosignal
			Tv_Idx	=	m_VssTbl[A_Ch].IhndlIdx;
			Tv_Hndl	=	m_VssTbl[A_Ch].His[Tv_Idx].Hndl;
			Tv_FrmCnt	=	m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt;
			// reset
			m_VssTbl[A_Ch].Time	=	GrTimeSysTimeGet( &Tv_Mili );		//time
			m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt	=	0;
			m_VssTbl[A_Ch].His[Tv_Idx].Hndl			=	E_GrDvrRecHndlNone;
			//check privous able
			Tv_Idx	=	Tv_Idx ^ 1;
			if ( (E_GrDvrRecHndlNone != Tv_Hndl) && (E_GrDvrRecHndlNone != m_VssTbl[A_Ch].His[Tv_Idx].Hndl) )
			{
				Tv_FrmCnt	=	Tv_FrmCnt + m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt;
				Tv_Hndl		=	m_VssTbl[A_Ch].His[Tv_Idx].Hndl;
				// reset
				m_VssTbl[A_Ch].His[Tv_Idx].PfrmCnt	=	0;
				m_VssTbl[A_Ch].His[Tv_Idx].Hndl			=	E_GrDvrRecHndlNone;
			}
			Tv_FrmCnt ++;
			if ( E_GrDvrRecHndlNone == Tv_Hndl )
			{
				Tv_FrmCnt	=	0;
			}
			m_FsMng->SafeVssItemAdd( Tv_Hndl, m_VssTbl[A_Ch].Time, A_Ch, A_Cmd, A_IsArmed, A_Etty,
				m_SeqIdTbl[A_Ch], Tv_FrmCnt );
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrRecRcvSoloV1::LcMarkEvtPreframes( __u8 A_Ch, __u32 A_Evt )
{
	// local -------------------
		__u32*	Tv_PtrFrm;
		__u8		Tv_Ch;
	// code --------------------
		// init scan
		GrBufRqScanReset( m_PtrRecQue );

		Tv_PtrFrm	=	(__u32*)GrBufRqGetScanPtr( m_PtrRecQue );
		while ( NULL != Tv_PtrFrm )
		{
			//chekc video frame
			if ( E_GrDrvSoloP2mVdoFcc == *Tv_PtrFrm )
			{
				//video frame
				// check channel number
				Tv_Ch				=	(__u8)((Tv_PtrFrm[E_GrDrvSoloVencStat0BkIdx] >> 24) & 0x0F);
				if ( A_Ch == Tv_Ch )
				{
					//update attribute
					Tv_PtrFrm[E_GrDrvSoloVencEvtBkIdx]	=	Tv_PtrFrm[E_GrDrvSoloVencEvtBkIdx] | A_Evt;
				}
			}

			// next
			if ( !GrBufRqScanNext( m_PtrRecQue ) )
			{
				break;
			}
			Tv_PtrFrm	=	(__u32*)GrBufRqGetScanPtr( m_PtrRecQue );
		}

}
//--------------------------------------------------------------------

