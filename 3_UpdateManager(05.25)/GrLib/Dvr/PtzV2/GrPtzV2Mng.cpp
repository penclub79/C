/*
 DVR PAN TILT

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <Dvr/PtzV2/GrPtzV2Mng.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <stdlib.h>

#include <Dvr/PtzV2/GrPtzV2PtPelcoD.h>
#include <Dvr/PtzV2/GrPtzV2PtPma200.h>
#include <Dvr/PtzV2/GrPtzV2PtVcl.h>
#include <Dvr/PtzV2/GrPtzV2PtDmax.h>

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
Cls_GrPtzV2Mng::Cls_GrPtzV2Mng( Cls_GrTaskSvr* A_TaskSvr, __u8 A_MaxChCnt ):
Cls_GrTaskCli()
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_TourTimer	=	0;
		m_MaxChCnt	=	A_MaxChCnt;
		GrDumyZeroMem( m_ChTbl , sizeof(m_ChTbl) );

		// protocol initialize
		LcPrtcBuild();

		// build default tour table
		for ( Tv_WkIdx=0;Tv_WkIdx < m_MaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].TourStanby	=	E_GrPtzV2TourStanbyDflt;
		}

		// none model make
		GrStrStrToWstr( m_StrModelNone, "[None]" );

		// channel table initialize


		//register task
		m_TaskSvr->RegistTimerClient( this, E_GrPtzV2AutoStopChkPeriod, &m_HndlTimer );
		m_TaskSvr->RegistMsgClient( this );

}
//--------------------------------------------------------------------
Cls_GrPtzV2Mng::~Cls_GrPtzV2Mng()
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//unregister timer
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release protocol object
		for ( Tv_WkIdx=0;Tv_WkIdx < m_ModelCnt;Tv_WkIdx++ )
		{
			delete	m_ObjPrtc[Tv_WkIdx];
			m_ObjPrtc[Tv_WkIdx]	=	NULL;
		}

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2Mng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrPtzV2MsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrPtzV2MsgCmdSetup:
					ImmSetup( ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->Model,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->Id, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->UartCh,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->BoudRate, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->DataBit,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->Parity, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->StopBit,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->SpdPan, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->SpdTilt,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->SpdZoom, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->SpdFocus,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->IsRvsPan, ((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->IsRvsTilt,
						((Ptr_GrPtzV2MsgSetup)A_PtrMsg)->IsHwTour );
					break;
				case	E_GrPtzV2MsgCmdDoPanTilt:
					ImmDoPanTilt( ((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->IsLeft,
						((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->IsRight, ((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->IsUp,
						((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->IsDown, ((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->SpdPan,
						((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->SpdTilt, ((Ptr_GrPtzV2MsgDoPanTilt)A_PtrMsg)->AutoStop );
					break;
				case	E_GrPtzV2MsgCmdDoZoom:
					ImmDoZoom( ((Ptr_GrPtzV2MsgDoZoom)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoZoom)A_PtrMsg)->IsWide,
						((Ptr_GrPtzV2MsgDoZoom)A_PtrMsg)->Speed, ((Ptr_GrPtzV2MsgDoZoom)A_PtrMsg)->AutoStop );
					break;
				case	E_GrPtzV2MsgCmdDoFocus:
					ImmDoFocus( ((Ptr_GrPtzV2MsgDoFocus)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoFocus)A_PtrMsg)->IsFar,
						((Ptr_GrPtzV2MsgDoFocus)A_PtrMsg)->Speed, ((Ptr_GrPtzV2MsgDoFocus)A_PtrMsg)->AutoStop );
					break;
				case	E_GrPtzV2MsgCmdDoIris:
					ImmDoIris( ((Ptr_GrPtzV2MsgDoIris)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoIris)A_PtrMsg)->IsOpen,
						((Ptr_GrPtzV2MsgDoIris)A_PtrMsg)->Speed, ((Ptr_GrPtzV2MsgDoIris)A_PtrMsg)->AutoStop );
					break;
				case	E_GrPtzV2MsgCmdDoStop:
					ImmDoStop( ((Ptr_GrPtzV2MsgDoStop)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoAutoFocus:
					ImmDoAutoFocus( ((Ptr_GrPtzV2MsgDoAutoFocus)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoMenuCall:
					ImmDoMenuCall( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoMenuExit:
					ImmDoMenuExit( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoPrsSet:
					ImmDoPresetSet( ((Ptr_GrPtzV2MsgDoPrsSet)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoPrsSet)A_PtrMsg)->Id );
					break;
				case	E_GrPtzV2MsgCmdDoPrsMove:
					ImmDoPresetMove( ((Ptr_GrPtzV2MsgDoPrsMove)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgDoPrsMove)A_PtrMsg)->Id,
						((Ptr_GrPtzV2MsgDoPrsMove)A_PtrMsg)->Speed );
					break;
				case	E_GrPtzV2MsgCmdIsMenuMode:
					*(((Ptr_GrPtzV2MsgIsMenuMode)A_PtrMsg)->PtrRslt)	=	ImmIsMenuAble( ((Ptr_GrPtzV2MsgIsMenuMode)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdTourRun:
					ImmTourRun( ((Ptr_GrPtzV2MsgTourRun)A_PtrMsg)->Ch, ((Ptr_GrPtzV2MsgTourRun)A_PtrMsg)->IsRun );
					break;
				case	E_GrPtzV2MsgCmdMenuOk:
					ImmDoMenuOk( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdMenuCancel:
					ImmDoMenuCancel( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdTourSave:
					ImmTourSave( ((Ptr_GrPtzV2MsgTourSave)A_PtrMsg)->Ch, 
						((Ptr_GrPtzV2MsgTourSave)A_PtrMsg)->PtrTourTbl, ((Ptr_GrPtzV2MsgTourSave)A_PtrMsg)->Cnt );
					break;
				case	E_GrPtzV2MsgCmdIdView:
					ImmExIdView( ((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->Model, ((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->Id,
						((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->UartCh, ((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->Baud,
						((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->DataBit, ((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->Parity,
						((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->StopBit, ((Ptr_GrPtzV2MsgIdView)A_PtrMsg)->IsOn );
					break;
				case	E_GrPtzV2MsgCmdIdSet:
					ImmExIdSet( ((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->Model, ((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->Id,
						((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->UartCh, ((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->Baud,
						((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->DataBit, ((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->Parity,
						((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->StopBit, ((Ptr_GrPtzV2MsgIdSet)A_PtrMsg)->IdNew );
					break;
				case	E_GrPtzV2MsgCmdDoMenuLeft:
					ImmDoMenuLeft( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoMenuRight:
					ImmDoMenuRight( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoMenuUp:
					ImmDoMenuUp( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrPtzV2MsgCmdDoMenuDown:
					ImmDoMenuDown( ((Ptr_GrPtzV2MsgDoMenuStd)A_PtrMsg)->Ch );
					break;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmSetup(	__u8 A_Ch, __u8 A_Model, __u8 A_Id, __u8 A_UartCh,
	__u32 A_BoudRate, __u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit,
	__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, 
	BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour )
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			// check model
			// reset
			GrDumyZeroMem( &m_ChTbl[A_Ch], sizeof(St_GrPtzV2ChMng) );
			if ( (0 < A_Model) && (A_Model <= m_ModelCnt) )
			{
				// update information
				m_ChTbl[A_Ch].Model		=	A_Model;
				m_ChTbl[A_Ch].Id			=	A_Id;
				m_ChTbl[A_Ch].Ch			=	A_Ch;
				m_ChTbl[A_Ch].UartCh	=	A_UartCh;
				m_ChTbl[A_Ch].Baud		=	A_BoudRate;
				m_ChTbl[A_Ch].DataBit	=	A_DataBit;
				m_ChTbl[A_Ch].Parity	=	A_Parity;
				m_ChTbl[A_Ch].StopBit	=	A_StopBit;
				m_ChTbl[A_Ch].SpdPan	=	A_SpdPan;
				m_ChTbl[A_Ch].SpdTilt	=	A_SpdTilt;
				m_ChTbl[A_Ch].SpdZoom	=	A_SpdZoom;
				m_ChTbl[A_Ch].SpdFocus	=	A_SpdFocus;
				m_ChTbl[A_Ch].IsRvsPan	=	A_IsRvsPan;
				m_ChTbl[A_Ch].IsRvsTilt	=	A_IsRvsTilt;
				m_ChTbl[A_Ch].IsHwTour	=	A_IsHwTour;

				// reset user data
				for ( Tv_WkIdx=0;Tv_WkIdx < E_GrPtzV2UserDataCnt;Tv_WkIdx++ )
				{
					m_ChTbl[A_Ch].UserData[Tv_WkIdx]	=	0;
				}
				// do init
				m_ObjPrtc[A_Model - 1]->Init( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmTourSave( __u8 A_Ch, Ptr_GrPtzV2TourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( (E_GrDvrMaxChCnt > A_Ch) && (0 != A_Cnt) && (E_GrPtzV2MaxTourCnt >= A_Cnt) )
		{
			//copy tour table
			GrDumyCopyMem( m_ChTbl[A_Ch].TourTbl, A_PtrTbl, sizeof(St_GrPtzV2TourItm) * (__u32)A_Cnt );
			m_ChTbl[A_Ch].TourCnt	=	A_Cnt;
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//update hardware touring
				if ( m_ChTbl[A_Ch].IsHwTour )
				{
					m_ObjPrtc[Tv_Model]->TourSet( &m_ChTbl[A_Ch] );
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::LcPrtcBuild( void )
{
	// local -------------------
	// code --------------------
		m_ModelCnt	=	0;
		// PELCO-D
		m_ObjPrtc[m_ModelCnt]	=	(Cls_GrPtzV2PtBase*)new Cls_GrPtzV2PtPelcoD();
		m_ModelCnt ++;
		// PMA-200
		m_ObjPrtc[m_ModelCnt]	=	(Cls_GrPtzV2PtBase*)new Cls_GrPtzV2PtPma200();
		m_ModelCnt ++;
		// VCL
		m_ObjPrtc[m_ModelCnt]	=	(Cls_GrPtzV2PtBase*)new Cls_GrPtzV2PtVcl();
		m_ModelCnt ++;
		// DMAX
		m_ObjPrtc[m_ModelCnt]	=	(Cls_GrPtzV2PtBase*)new Cls_GrPtzV2PtDmax();
		m_ModelCnt ++;

}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeSetup( __u8 A_Ch, __u8 A_Model, __u8 A_Id, __u8 A_UartCh,
	__u32 A_BoudRate, __u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit,
	__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, 
	BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour )
{
	// local -------------------
		St_GrPtzV2MsgSetup	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdSetup;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.Id						=	A_Id;
			Tv_Msg.UartCh				=	A_UartCh;
			Tv_Msg.BoudRate			=	A_BoudRate;
			Tv_Msg.DataBit			=	A_DataBit;
			Tv_Msg.Parity				=	A_Parity;
			Tv_Msg.StopBit			=	A_StopBit;
			Tv_Msg.SpdPan				=	A_SpdPan;
			Tv_Msg.SpdTilt			=	A_SpdTilt;
			Tv_Msg.SpdZoom			=	A_SpdZoom;
			Tv_Msg.SpdFocus			=	A_SpdFocus;
			Tv_Msg.IsRvsPan			=	A_IsRvsPan;
			Tv_Msg.IsRvsTilt		=	A_IsRvsTilt;
			Tv_Msg.IsHwTour			=	A_IsHwTour;

			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeTourSave( __u8 A_Ch, Ptr_GrPtzV2TourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
		St_GrPtzV2MsgTourSave	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdTourSave;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Cnt					=	A_Cnt;
			Tv_Msg.PtrTourTbl		=	A_PtrTbl;
			if ( 0 != A_Cnt )
			{
				//patch count
				if ( E_GrPtzV2MaxTourCnt < A_Cnt )
				{
					Tv_Msg.Cnt	=	E_GrPtzV2MaxTourCnt;
				}
				m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrPtzV2Mng::ModelNameGet( __u8 A_Model )
{
	// local -------------------
		__u8	Tv_Model;
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( 0 == A_Model )
		{
			Tv_Result	=	m_StrModelNone;
		}
		else
		if (A_Model <= m_ModelCnt)
		{
			Tv_Model	=	A_Model - 1;
			Tv_Result	=	m_ObjPrtc[Tv_Model]->ModelNameGet();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, 
																	 BOOL8 A_IsDown, __u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Model;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_SpdPan;
		__u8	Tv_SpdTilt;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//touring off
				m_ChTbl[A_Ch].IsTouring	=	FALSE;

				//set
				Tv_IsLeft		=	A_IsLeft;
				Tv_IsRight	=	A_IsRight;
				if ( m_ChTbl[A_Ch].IsRvsPan )
				{
					Tv_IsLeft		=	A_IsRight;
					Tv_IsRight	=	A_IsLeft;
				}
				Tv_IsUp			=	A_IsUp;
				Tv_IsDown		=	A_IsDown;
				if ( m_ChTbl[A_Ch].IsRvsTilt )
				{
					Tv_IsUp		=	A_IsDown;
					Tv_IsDown	=	A_IsUp;
				}
				Tv_SpdPan	=	A_SpdPan;
				if ( E_GrPtzV2PtMaxSpeed < A_SpdPan  )
				{
					Tv_SpdPan	=	m_ChTbl[A_Ch].SpdPan;
				}
				Tv_SpdTilt	=	A_SpdTilt;
				if ( E_GrPtzV2PtMaxSpeed < A_SpdTilt  )
				{
					Tv_SpdTilt	=	m_ChTbl[A_Ch].SpdTilt;
				}
				//update auto stop
				m_ChTbl[A_Ch].AutoStopCnt	=	A_AutoStop;
				// do protocol
				m_ObjPrtc[Tv_Model]->DoPanTilt( &m_ChTbl[A_Ch], Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, 
					Tv_SpdPan, Tv_SpdTilt );
#ifdef E_GrPtzV2DbgMsg
				DbgMsgPrint( "PTZ : ch %d ptz cmd\n", A_Ch );
#endif
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Model;
		__u8	Tv_Spd;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// run
				Tv_Spd	=	A_Speed;
				if ( E_GrPtzV2PtMaxSpeed < A_Speed )
				{
					Tv_Spd	=	m_ChTbl[A_Ch].SpdZoom;
				}
				//update auto stop
				m_ChTbl[A_Ch].AutoStopCnt	=	A_AutoStop;
				// run
				m_ObjPrtc[Tv_Model]->DoZoom( &m_ChTbl[A_Ch], A_IsWide, Tv_Spd );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Model;
		__u8	Tv_Spd;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				Tv_Spd	=	A_Speed;
				if ( E_GrPtzV2PtMaxSpeed < A_Speed )
				{
					Tv_Spd	=	m_ChTbl[A_Ch].SpdFocus;
				}
				//update auto stop
				m_ChTbl[A_Ch].AutoStopCnt	=	A_AutoStop;
				m_ObjPrtc[Tv_Model]->DoFocus( &m_ChTbl[A_Ch], A_IsFar, Tv_Spd );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_Model;
		__u8	Tv_Spd;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				Tv_Spd	=	A_Speed;
				if ( E_GrPtzV2PtMaxSpeed < A_Speed )
				{
					Tv_Spd	=	m_ChTbl[A_Ch].SpdFocus;
				}
				//update auto stop
				m_ChTbl[A_Ch].AutoStopCnt	=	A_AutoStop;
				// run
				m_ObjPrtc[Tv_Model]->DoIris( &m_ChTbl[A_Ch], A_IsOpen, Tv_Spd );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoStop( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ChTbl[A_Ch].AutoStopCnt	=	0;
				m_ObjPrtc[Tv_Model]->DoStop( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoAutoFocus( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ObjPrtc[Tv_Model]->DoAutoFocus( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuCall( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ObjPrtc[Tv_Model]->DoMenuCall( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuExit( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ObjPrtc[Tv_Model]->DoMenuExit( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuOk( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ObjPrtc[Tv_Model]->DoMenuOk( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuCancel( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//run
				m_ObjPrtc[Tv_Model]->DoMenuCancel( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoPresetSet( __u8 A_Ch, __u8 A_Id )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//tour standby
				LcTourStandby( A_Ch );
				//run
				m_ObjPrtc[Tv_Model]->DoPresetSet( &m_ChTbl[A_Ch], A_Id );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Model;
		__u8	Tv_Spd;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				//tour standby
				LcTourStandby( A_Ch );
				//run
				Tv_Spd	=	A_Speed;
				if ( E_GrPtzV2PtMaxSpeed < A_Speed )
				{
					Tv_Spd	=	m_ChTbl[A_Ch].SpdPan;
				}
				m_ObjPrtc[Tv_Model]->DoPresetMove( &m_ChTbl[A_Ch], A_Id, Tv_Spd );
			}
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2Mng::ImmIsMenuAble( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Model;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// check
				Tv_Result	=	m_ObjPrtc[Tv_Model]->IsMenuAble();
			}
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_Model;
		__u8	Tv_TourIdx;
		__u8	Tv_WkIdx;
	// code --------------------
		if ( m_HndlTimer == A_Hndl )
		{
			//update auto stop
			for ( Tv_Ch =0;Tv_Ch < m_MaxChCnt;Tv_Ch++ )
			{
				// check able model
				if ( 0 != m_ChTbl[Tv_Ch].Model )
				{
					// init
					Tv_Model	=	m_ChTbl[Tv_Ch].Model - 1;
					//check auto stop requested
					if ( 0 != m_ChTbl[Tv_Ch].AutoStopCnt )
					{
						//decrease counter
						m_ChTbl[Tv_Ch].AutoStopCnt --;
						//check time at can run
						if ( 0 == m_ChTbl[Tv_Ch].AutoStopCnt )
						{
							//do auto stop
							m_ObjPrtc[Tv_Model]->DoStop( &m_ChTbl[Tv_Ch] );
						}
					}
					//update tour
					if ( E_GrPtzV2TourTimePeriodCnt <= m_TourTimer )
					{
						//check run touring
						if ( m_ChTbl[Tv_Ch].IsTouring )
						{
							if ( 0 == m_ChTbl[Tv_Ch].TourStbyCnt )
							{
								//check soft ware touring
								if ( !m_ChTbl[Tv_Ch].IsHwTour )
								{
									//check tour update
									if ( 0 == m_ChTbl[Tv_Ch].TourTime )
									{
										//go preset now
										Tv_TourIdx	=	m_ChTbl[Tv_Ch].TourIdx;
										if ( 0 != m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain )
										{
											m_ObjPrtc[Tv_Model]->DoPresetMove( &m_ChTbl[Tv_Ch], m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].PresetId, m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Spd );
											//update time
											m_ChTbl[Tv_Ch].TourTime	=	m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain;
										}
										//go next index
										for ( Tv_WkIdx=0;Tv_WkIdx < E_GrPtzV2MaxTourCnt;Tv_WkIdx++ )
										{
											//go next
											Tv_TourIdx ++;
											if ( E_GrPtzV2MaxTourCnt <= Tv_TourIdx )
											{
												Tv_TourIdx	=	0;
											}
											//check able
											if ( 0 != m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain )
											{
												// found
												break;
											}
										}
										//check found
										if ( 0 != m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain )
										{
											// found
											m_ChTbl[Tv_Ch].TourIdx	=	Tv_TourIdx;
										}
										else
										{
											//not exist tour data
											m_ChTbl[Tv_Ch].IsTouring	=	FALSE;
										}
									}
									else
									{
										m_ChTbl[Tv_Ch].TourTime --;
									}
								}
							}
							else
							{
								//tour standby count decrease
								m_ChTbl[Tv_Ch].TourStbyCnt --;
								if ( (0 == m_ChTbl[Tv_Ch].TourStbyCnt) && m_ChTbl[Tv_Ch].IsHwTour )
								{
									//restore tour
									m_ObjPrtc[Tv_Model]->TourRun( &m_ChTbl[Tv_Ch] );
								}
							}
						}
						else
						{
							//reset tour standby value zero
							m_ChTbl[Tv_Ch].TourStbyCnt	=	0;
						}
					}
				}
			}
			//tour timer
			if ( E_GrPtzV2TourTimePeriodCnt <= m_TourTimer )
			{
				//reset tour time counter
				m_TourTimer	=	0;
			}
			m_TourTimer ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, 
																 BOOL8 A_IsDown, __u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		St_GrPtzV2MsgDoPanTilt	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):PT L:%d R:%d U:%d D:%d  SP:%d ST:%d\n", A_Ch, A_IsLeft, A_IsRight, A_IsUp, A_IsDown, A_SpdPan, A_SpdTilt );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoPanTilt;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsLeft				=	A_IsLeft;
			Tv_Msg.IsRight			=	A_IsRight;
			Tv_Msg.IsUp					=	A_IsUp;
			Tv_Msg.IsDown				=	A_IsDown;
			Tv_Msg.SpdPan				=	A_SpdPan;
			Tv_Msg.SpdTilt			=	A_SpdTilt;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrPtzV2MsgDoZoom	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):Z W:%d S:%d\n", A_Ch, A_IsWide, A_Speed );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoZoom;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsWide				=	A_IsWide;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrPtzV2MsgDoFocus	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):F F:%d S:%d\n", A_Ch, A_IsFar, A_Speed );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoFocus;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsFar				=	A_IsFar;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrPtzV2MsgDoIris	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):I O:%d S:%d\n", A_Ch, A_IsOpen, A_Speed );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoIris;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsOpen				=	A_IsOpen;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoStop( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoStop	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):STOP\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoStop;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoAutoFocus( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoAutoFocus	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):AF\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoAutoFocus;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuCall( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):MENU\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuCall;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuExit( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):EXIT\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuExit;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuOk( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):OK\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdMenuOk;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuCancel( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):CANCEL\n", A_Ch );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdMenuCancel;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoPresetSet( __u8 A_Ch, __u8 A_Id )
{
	// local -------------------
		St_GrPtzV2MsgDoPrsSet	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):PSET ID:%d\n", A_Ch, A_Id );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoPrsSet;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Id						=	A_Id;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		St_GrPtzV2MsgDoPrsMove	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):PMOVE ID:%d S:%d\n", A_Ch, A_Id, A_Speed );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoPrsMove;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Id						=	A_Id;
			Tv_Msg.Speed				=	A_Speed;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2Mng::SafeIsMenuAble( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrPtzV2MsgIsMenuMode	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdIsMenuMode;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.PtrRslt			=	&Tv_Result;
		
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrPtzV2Mng::IsTouring( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Result	=	m_ChTbl[A_Ch].IsTouring;
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeTouringRun( __u8 A_Ch, BOOL8 A_IsRun )
{
	// local -------------------
		St_GrPtzV2MsgTourRun	Tv_Msg;
	// code --------------------
#ifdef GR_P4_EDITOR
		DbgMsgPrint( "PTZ(%d):TOUR RUN:%d\n", A_Ch, A_IsRun );
#endif
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdTourRun;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsRun				=	A_IsRun;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmTourRun( __u8 A_Ch, BOOL8 A_IsRun )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// check
				if ( m_ChTbl[A_Ch].IsTouring != A_IsRun )
				{
					m_ChTbl[A_Ch].IsTouring	=	A_IsRun;
					m_ChTbl[A_Ch].TourIdx		=	0;
					m_ChTbl[A_Ch].TourTime	=	0;
					if ( m_ChTbl[A_Ch].IsHwTour )
					{
						if ( m_ChTbl[A_Ch].IsTouring )
						{
							m_ObjPrtc[Tv_Model]->TourRun( &m_ChTbl[A_Ch] );
						}
						else
						{
							m_ObjPrtc[Tv_Model]->TourStop( &m_ChTbl[A_Ch] );
						}
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmExIdView( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, __u8 A_Parity,
	__u8 A_StopBit, BOOL8 A_IsOn )
{
		if ( (0 != A_Model) && (A_Model <= m_ModelCnt) )
		{
			m_ObjPrtc[A_Model]->ExIdView( A_Id, A_UartCh, A_Baud, A_DataBit, A_Parity, A_StopBit, A_IsOn );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeExIdView( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
	__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, BOOL8 A_IsOn )
{
	// local -------------------
		St_GrPtzV2MsgIdView	Tv_Msg;
	// code --------------------
		if ( (0 != A_Model) && (A_Model <= m_ModelCnt) )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdIdView;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.Id						=	A_Id;
			Tv_Msg.UartCh				=	A_UartCh;
			Tv_Msg.Baud					=	A_Baud;
			Tv_Msg.DataBit			=	A_DataBit;
			Tv_Msg.Parity				=	A_Parity;
			Tv_Msg.StopBit			=	A_StopBit;
			Tv_Msg.IsOn					=	A_IsOn;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmExIdSet( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, __u8 A_Parity,
	__u8 A_StopBit, __u8 A_IdNew )
{
		if ( (0 != A_Model) && (A_Model <= m_ModelCnt) )
		{
			m_ObjPrtc[A_Model]->ExIdView( A_Id, A_UartCh, A_Baud, A_DataBit, A_Parity, A_StopBit, A_IdNew );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeExIdSet( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
	__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, __u8 A_IdNew )
{
	// local -------------------
		St_GrPtzV2MsgIdSet	Tv_Msg;
	// code --------------------
		if ( (0 != A_Model) && (A_Model <= m_ModelCnt) )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdIdSet;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.Id						=	A_Id;
			Tv_Msg.UartCh				=	A_UartCh;
			Tv_Msg.Baud					=	A_Baud;
			Tv_Msg.DataBit			=	A_DataBit;
			Tv_Msg.Parity				=	A_Parity;
			Tv_Msg.StopBit			=	A_StopBit;
			Tv_Msg.IdNew				=	A_IdNew;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::TourStanbyTimeSet( __u8 A_Ch, __u8 A_Sec )
{
		m_ChTbl[A_Ch].TourStanby	=	A_Sec;
}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2Mng::TourStanbyTimeGet( __u8 A_Ch )
{
		return	m_ChTbl[A_Ch].TourStanby;
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::LcTourStandby( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( 0 != m_ChTbl[A_Ch].Model )
		{
			Tv_Model	=	m_ChTbl[A_Ch].Model - 1;

			m_ChTbl[A_Ch].TourStbyCnt	=	m_ChTbl[A_Ch].TourStanby;
			m_ChTbl[A_Ch].TourIdx			=	0;		//reset touring
			m_ChTbl[A_Ch].TourTime		=	0;
			if ( m_ChTbl[A_Ch].IsTouring && m_ChTbl[A_Ch].IsHwTour && (0 == m_ChTbl[A_Ch].TourStbyCnt) )
			{
				//stop tour
				m_ObjPrtc[Tv_Model]->TourStop( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuLeft( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuLeft;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuRight( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuRight;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuUp( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuUp;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::SafeDoMenuDown( __u8 A_Ch )
{
	// local -------------------
		St_GrPtzV2MsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrPtzV2MsgCmdDoMenuDown;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuLeft( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// run
				m_ObjPrtc[Tv_Model]->DoMenuLeft( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuRight( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// run
				m_ObjPrtc[Tv_Model]->DoMenuRight( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuUp( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// run
				m_ObjPrtc[Tv_Model]->DoMenuUp( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::ImmDoMenuDown( __u8 A_Ch )
{
	// local -------------------
		__u8	Tv_Model;
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			if ( 0 != m_ChTbl[A_Ch].Model )
			{
				// init
				Tv_Model	=	m_ChTbl[A_Ch].Model - 1;
				// run
				m_ObjPrtc[Tv_Model]->DoMenuDown( &m_ChTbl[A_Ch] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrPtzV2Mng::MaxSpdToggle( __u8 A_Ch )
{
	// local -------------------
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			// check now speed
			if ( (E_GrPtzV2PtMaxSpeed == m_ChTbl[A_Ch].SpdPan) && (E_GrPtzV2PtMaxSpeed == m_ChTbl[A_Ch].SpdTilt) && (E_GrPtzV2PtMaxSpeed == m_ChTbl[A_Ch].SpdZoom) )
			{
				// normal speed
				m_ChTbl[A_Ch].SpdPan	=	2;
				m_ChTbl[A_Ch].SpdTilt	=	2;
				m_ChTbl[A_Ch].SpdZoom	=	2;
			}
			else
			{
				// max speed
				m_ChTbl[A_Ch].SpdPan	=	E_GrPtzV2PtMaxSpeed;
				m_ChTbl[A_Ch].SpdTilt	=	E_GrPtzV2PtMaxSpeed;
				m_ChTbl[A_Ch].SpdZoom	=	E_GrPtzV2PtMaxSpeed;
			}
		}
}
//--------------------------------------------------------------------
__u8	Cls_GrPtzV2Mng::ModelCntGet( void )
{
		return	m_ModelCnt;
}
//--------------------------------------------------------------------
