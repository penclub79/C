/*
 GAUSE file system version 2 Backup

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <Dvr/Fs/V2/GrDvrFs2Bkup.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs2Bkup::Cls_GrDvrFs2Bkup( Cls_GrTaskSvr* A_TaskSvr, Cls_GrDvrFs2Mng* A_FsMng, Ptr_GrDvrFs2SeekCtrl A_PtrSeekCtrl, BOOL8 A_IsSafe ):
Cls_GrDvrFs2Dmux( A_TaskSvr, A_FsMng, A_PtrSeekCtrl, A_IsSafe )
{
	// local -------------------
		//__u8	Tv_WkIdx;
	// code --------------------
		// init
		m_IsBackup	=	FALSE;

		GrDumyZeroMem( m_ChTbl, sizeof(m_ChTbl) );

		// register
		// m_TaskSvr->RegistBkgClient( this );
}
//--------------------------------------------------------------------
Cls_GrDvrFs2Bkup::~Cls_GrDvrFs2Bkup()
{

		// unregister
		// m_TaskSvr->UnregistBkgClient( this );

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Bkup::LcPableReset( void )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrMaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].IsPable	=	FALSE;
		}
		
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs2Bkup::BkupRecDevTypeGet( void )
{
		return	m_DevType;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Bkup::IsBackup( void )
{
		return	m_IsBackup;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Bkup::BkupTimeProgress( void )
{
		return	m_BkupTimePrgs;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Bkup::BkupTimeStart( void )
{
		return	m_BkupTimeSt;
}
//--------------------------------------------------------------------
Def_GrTime	Cls_GrDvrFs2Bkup::BkupTimeEnd( void )
{
		return	m_BkupTimeEd;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Bkup::LcVdoChEnSet( __u16 A_Map )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
		__u8	Tv_EnChCnt;
	// code --------------------
		Tv_MaskCh		=	1;
		Tv_EnChCnt	=	0;
		for(Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++)
		{
			if ( 0 != (Tv_MaskCh & A_Map) )
			{
				m_ChTbl[Tv_Ch].IsVdoAble	=	TRUE;
				Tv_EnChCnt ++;
			}
			else
			{
				m_ChTbl[Tv_Ch].IsVdoAble	=	FALSE;
				m_ChTbl[Tv_Ch].IsPable		=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs2Bkup::LcAdoChEnSet( __u16 A_Map )
{
	// local -------------------
		__u8	Tv_Ch;
		__u16	Tv_MaskCh;
	// code --------------------
		Tv_MaskCh	=	1;
		for(Tv_Ch=0;Tv_Ch < E_GrDvrMaxChCnt;Tv_Ch++)
		{
			if ( 0 != (Tv_MaskCh & A_Map) )
			{
				m_ChTbl[Tv_Ch].IsAdoAble	=	TRUE;
			}
			else
			{
				m_ChTbl[Tv_Ch].IsAdoAble	=	FALSE;
			}
			// next
			Tv_MaskCh	=	Tv_MaskCh << 1;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Bkup::BkupStart( Def_GrTime A_TimeSt, Def_GrTime A_TimeEd, __u8 A_DevType, __u16 A_VdoChMap, __u16 A_AdoChMap )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgHd	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;

		// update device type
		m_DevType				=	A_DevType;
		m_BkupTimeSt		=	A_TimeSt;
		m_BkupTimeEd		=	A_TimeEd;

		m_BkupTimePrgs	=	0;

		LcVdoChEnSet( A_VdoChMap );
		LcAdoChEnSet( A_AdoChMap );

		// seek start position
		SeekByMinFw( m_BkupTimeSt );
		LcPableReset();
		// check status
		if ( E_GrDvrSeekStatNormal == m_PtrSeekCtrl->Stat )
		{
			LcPableReset();
			m_IsBackup	=	TRUE;
			Tv_Result	=	TRUE;

			// backup start message broadcast
			Tv_Msg.Cmd		=	E_GrTaskMsgBkupStart;
			GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Ptr_GrDvrAvHd	Cls_GrDvrFs2Bkup::BkupFrameGet( void )
{
	// local -------------------
		Ptr_GrDvrAvHd	Tv_Result;
		Ptr_GrDvrAvHd	Tv_PtrFrm;
	// code --------------------
		Tv_Result	=	NULL;

		// check now backup
		if ( m_IsBackup )
		{
			// get frame info
			if ( NULL != m_PtrFrm )
			{
				Tv_PtrFrm	=	(Ptr_GrDvrAvHd)m_PtrFrm;
				// check time
				if ( (E_GrDvrAvTypAdo >= Tv_PtrFrm->Type) || (E_GrDvrAvTypVdoB == Tv_PtrFrm->Type))
				{
					// check channel
					if ( E_GrDvrMaxChCnt > Tv_PtrFrm->Ch )
					{
						if ( (m_BkupTimeSt <= Tv_PtrFrm->TimeSec) && (m_BkupTimeEd >= Tv_PtrFrm->TimeSec) )
						{
							// check type
							if ( (E_GrDvrAvTypVdoP >= Tv_PtrFrm->Type) || (E_GrDvrAvTypVdoB == Tv_PtrFrm->Type) )
							{
								// check enable
								if ( m_ChTbl[Tv_PtrFrm->Ch].IsVdoAble )
								{
									// check p frame able
									if ( E_GrDvrAvTypVdoI == Tv_PtrFrm->Type )
									{
										Tv_Result	=	Tv_PtrFrm;
									}
									else if ( m_ChTbl[Tv_PtrFrm->Ch].IsPable )
									{
										Tv_Result	=	Tv_PtrFrm;
									}
								}
							}
							else 
							{
								// audio
								if ( m_ChTbl[Tv_PtrFrm->Ch].IsAdoAble )
								{
									Tv_Result	=	Tv_PtrFrm;
								}
							}
						}
					}
					else
					{
						DbgMsgPrint( "Cls_GrDvrFs2Bkup::BkupFrameGet - bad channel number %d \n", Tv_PtrFrm->Ch );
					}
				}
			}
			// check last frame time
			if ( NULL != Tv_Result )
			{
				if ( m_BkupTimePrgs < Tv_Result->TimeSec )
				{
					m_BkupTimePrgs	=	Tv_Result->TimeSec;
				}
			}
			
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrFs2Bkup::BkupGoNext( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgHd	Tv_Msg;
		Ptr_GrDvrAvHd	Tv_PtrFrm;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( m_IsBackup )
		{
			// next
			FrmNext();

			// check frame status
			if ( E_GrDvrSeekStatNormal == m_PtrSeekCtrl->Stat )
			{
				// get frame info
				if ( NULL != m_PtrFrm )
				{
					Tv_PtrFrm	=	(Ptr_GrDvrAvHd)m_PtrFrm;
					// check time
					if ( (E_GrDvrAvTypAdo >= Tv_PtrFrm->Type) || (E_GrDvrAvTypVdoB == Tv_PtrFrm->Type))
					{
						if ( m_BkupTimeEd < Tv_PtrFrm->TimeSec )
						{
							// check now progress time
							if ( (m_BkupTimePrgs + E_GrTimeSecOfMin) < m_BkupTimeEd )
							{
								// go next time
								m_BkupTimePrgs	=	m_BkupTimePrgs + E_GrTimeSecOfMin;
								SeekByMinFw( m_BkupTimePrgs );
								LcPableReset();
								if ( E_GrDvrSeekStatNormal != m_PtrSeekCtrl->Stat )
								{
									m_IsBackup	=	FALSE;
								}
							}
							else
							{
								// finish
								m_IsBackup	=	FALSE;
							}
						}
					}
				}
			}
			else
			{
				// stop mode
				m_IsBackup	=	FALSE;
			}

			// backup finish notify
			if ( !m_IsBackup )
			{
				Tv_Msg.Cmd		=	E_GrTaskMsgBkupEnd;
				GrTaskSvrMsgBroadCast( &Tv_Msg, sizeof(Tv_Msg) );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

