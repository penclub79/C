/*
 dvr pantilt

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Ptz/GrDvrPtzMng.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>

#include	<Dvr/Ptz/GrDvrPtzPtElmo.h>
#include	<Dvr/Ptz/GrDvrPtzPtPelcoD.h>
#include	<Dvr/Ptz/GrDvrPtzPtPelcoP.h>
#include	<Dvr/Ptz/GrDvrPtzPtPwtech.h>
#include	<Dvr/Ptz/GrDvrPtzPtSams.h>
#include	<Dvr/Ptz/GrDvrPtzPtDmax.h>
#include	<Dvr/Ptz/GrDvrPtzPtC100hm.h>
#include	<Dvr/Ptz/GrDvrPtzPtNuvico.h>
#include	<Dvr/Ptz/GrDvrPtzPtAp8777z.h>
#include	<Dvr/Ptz/GrDvrPtzPtC201n.h>
#include	<Dvr/Ptz/GrDvrPtzPtOrx.h>
#include	<Dvr/Ptz/GrDvrPtzPtLilin2.h>
#include	<Dvr/Ptz/GrDvrPtzPtHoneywell.h>
#include	<Dvr/Ptz/GrDvrPtzPtSamt.h>
#include	<Dvr/Ptz/GrDvrPtzPtTsd300.h>
#include	<Dvr/Ptz/GrDvrPtzPtFastraxII.h>
#include	<Dvr/Ptz/GrDvrPtzPtFastraxIIE.h>
#include	<Dvr/Ptz/GrDvrPtzPtUTCFixed.h>//!!new112-5-6
#include	<Dvr/Ptz/GrDvrPtzPtUTCDmax.h>//!!new112-5-6
#include	<Dvr/Ptz/GrDvrPtzPtUTCPelcoD.h>//!!new112-5-6

//====================================================================
//local const

#define E_GrDvrPtzAutoStopChkPeriod		100

#define E_GrDvrPtzTourTimePeriodCnt		10

#define E_GrDvrPtzTourStanbyDflt			3

//command
#define E_GrDvrPtzMsgCmdSetup					0		/* setup : St_GrDvrPtzMsgSetup */
#define E_GrDvrPtzMsgCmdDoPanTilt			1		/* pantilt : St_GrDvrPtzMsgDoPanTilt */
#define E_GrDvrPtzMsgCmdDoZoom				2		/* zoom : St_GrDvrPtzMsgDoZoom */
#define E_GrDvrPtzMsgCmdDoFocus				3		/* focus : St_GrDvrPtzMsgDoFocus */
#define E_GrDvrPtzMsgCmdDoStop				4		/* stop ptz : St_GrDvrPtzMsgDoStop */
#define E_GrDvrPtzMsgCmdDoAutoFocus		5		/* auto focus : St_GrDvrPtzMsgDoAutoFocus */
#define E_GrDvrPtzMsgCmdDoMenuCall		6		/* menu call : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoMenuExit		7		/* menu exit : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoPrsSet			8		/* preset set : St_GrDvrPtzMsgDoPrsSet */
#define E_GrDvrPtzMsgCmdDoPrsMove			9		/* preset move : St_GrDvrPtzMsgDoPrsMove */
#define E_GrDvrPtzMsgCmdIsMenuMode		10	/* check in menu mode : St_GrDvrPtzMsgIsMenuMode */
#define E_GrDvrPtzMsgCmdTourRun				11	/* set tour run : St_GrDvrPtzMsgTourRun */
#define E_GrDvrPtzMsgCmdMenuOk				12	/* menu ok : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdMenuCancel		13	/* menu right : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdTourSave			14	/* tour save : St_GrDvrPtzMsgTourSave */
#define E_GrDvrPtzMsgCmdIdView				15	/* id view : St_GrDvrPtzMsgIdView */
#define E_GrDvrPtzMsgCmdIdSet					16	/* id set : St_GrDvrPtzMsgIdSet */
#define E_GrDvrPtzMsgCmdDoMenuLeft		17	/* menu left : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoMenuRight		18	/* menu right : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoMenuUp			19	/* menu up : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoMenuDown		20	/* menu down : St_GrDvrPtzMsgDoMenuStd */
#define E_GrDvrPtzMsgCmdDoIris				21	/* Iris : St_GrDvrPtzMsgDoIris */


//====================================================================
//local type

typedef	struct St_GrDvrPtzMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrDvrPtzMsgHd;

typedef	struct St_GrDvrPtzMsgSetup 
{
	St_GrDvrPtzMsgHd	Hd;
	__u8 Ch;
	__u8 Model;
	__u8 UartCh;
	__u8 Id;
	__u8 SpdPan;
	__u8 SpdTilt;
	__u8 SpdZoom;
	__u8 SpdFocus;
	BOOL8 IsRvsPan;
	BOOL8 IsRvsTilt;
	BOOL8	IsHwTour;

}	*Ptr_GrDvrPtzMsgSetup;

typedef	struct St_GrDvrPtzMsgTourSave
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	__u8	Cnt;		//tour table count
	Ptr_GrDvrPtzTourItm PtrTourTbl;
}	*Ptr_GrDvrPtzMsgTourSave;

typedef	struct St_GrDvrPtzMsgDoPanTilt
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8 IsLeft;
	BOOL8 IsRight;
	BOOL8 IsUp;
	BOOL8 IsDown;
	__u8	SpdPan;
	__u8	SpdTilt;
	__u8	AutoStop;
}	*Ptr_GrDvrPtzMsgDoPanTilt;

typedef	struct St_GrDvrPtzMsgDoZoom 
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8 IsWide;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrDvrPtzMsgDoZoom;

typedef	struct St_GrDvrPtzMsgDoFocus
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8 IsFar;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrDvrPtzMsgDoFocus;

typedef	struct St_GrDvrPtzMsgDoIris
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8 IsOpen;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrDvrPtzMsgDoIris;

typedef	struct St_GrDvrPtzMsgDoStop
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrDvrPtzMsgDoStop;

typedef	struct St_GrDvrPtzMsgDoAutoFocus
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrDvrPtzMsgDoAutoFocus;

typedef	struct St_GrDvrPtzMsgDoMenuStd
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrDvrPtzMsgDoMenuStd;

typedef	struct St_GrDvrPtzMsgDoPrsSet
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	__u8	Id;
}	*Ptr_GrDvrPtzMsgDoPrsSet;

typedef	struct St_GrDvrPtzMsgDoPrsMove
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	__u8	Id;
	__u8	Speed;
}	*Ptr_GrDvrPtzMsgDoPrsMove;

typedef	struct St_GrDvrPtzMsgIsMenuMode
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8*	PtrRslt;
}	*Ptr_GrDvrPtzMsgIsMenuMode;

typedef	struct St_GrDvrPtzMsgTourRun
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8	IsRun;
}	*Ptr_GrDvrPtzMsgTourRun;

typedef	struct St_GrDvrPtzMsgIdView
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	BOOL8	IsOn;
}	*Ptr_GrDvrPtzMsgIdView;

typedef	struct St_GrDvrPtzMsgIdSet
{
	St_GrDvrPtzMsgHd	Hd;
	__u8	Ch;
	__u8	IdNew;
}	*Ptr_GrDvrPtzMsgIdSet;

//====================================================================
//local function

//====================================================================
//var

Def_WstrName	V_GrDvrPtzMngModelNameTbl[E_GrDvrPtzMaxModelCnt]	=	
{
		{ L"[None]"	},
		{ L"ELMO-400C"	},
		{ L"PelcoD"	},
		{ L"PMA-200HF"	},
		{ L"SAMSUNG-S"	},
		{ L"D-max"	},
		{ L"LVC-C100hm/C200hm"	},
		{ L"Nuvico-EZ"	},
		{ L"PelcoP"	},
		{ L"AP-8777Z"	},
		{ L"LC201N"	},
		{ L"ORX-1000"	},
		{ L"Lilin V2"	},
		{ L"HSDN-S"	},
		{ L"SAMSUNG-T"	},
		{ L"SRX-100B"	},
		{ L"FD-1200SD"	},
		{ L"Fastrax-2"	},
		{ L"Fastrax-2E"	},
		{ L"UTC-Fixed"	},
		{ L"UTC-Dmax"	},
		{ L"UTC-PelcoD"	},
		{ L"VPD100/120"	},
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrPtzMng::Cls_GrDvrPtzMng( Cls_GrTaskSvr* A_TaskSvr, __u8 A_MaxChCnt ):
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

		for ( Tv_WkIdx=0;Tv_WkIdx < m_MaxChCnt;Tv_WkIdx++ )
		{
			m_ChTbl[Tv_WkIdx].TourStanby	=	E_GrDvrPtzTourStanbyDflt;
			m_ChTbl[Tv_WkIdx].ObjPrtc	=	(Cls_GrDvrPtzPtBase*) new Cls_GrDvrPtzPtBase( 0, 0, 0 );
		}

		//regist task
		m_TaskSvr->RegistTimerClient( this, E_GrDvrPtzAutoStopChkPeriod, &m_HndlTimer );
		m_TaskSvr->RegistMsgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrPtzMng::~Cls_GrDvrPtzMng()
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		//unregister timer
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release protocol object
		for ( Tv_WkIdx=0;Tv_WkIdx < m_MaxChCnt;Tv_WkIdx++ )
		{
			delete	m_ChTbl[Tv_WkIdx].ObjPrtc;
		}

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrDvrPtzMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrDvrPtzMsgCmdSetup:
					LcSetup( ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->Model,
						((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->UartCh, ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->Id, 
						((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->SpdPan, ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->SpdTilt,
						((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->SpdZoom, ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->SpdFocus,
						((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->IsRvsPan, ((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->IsRvsTilt,
						((Ptr_GrDvrPtzMsgSetup)A_PtrMsg)->IsHwTour );
					break;
				case	E_GrDvrPtzMsgCmdDoPanTilt:
					LcDoPanTilt( ((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->IsLeft,
						((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->IsRight, ((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->IsUp,
						((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->IsDown, ((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->SpdPan,
						((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->SpdTilt, ((Ptr_GrDvrPtzMsgDoPanTilt)A_PtrMsg)->AutoStop );
					break;
				case	E_GrDvrPtzMsgCmdDoZoom:
					LcDoZoom( ((Ptr_GrDvrPtzMsgDoZoom)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoZoom)A_PtrMsg)->IsWide,
						((Ptr_GrDvrPtzMsgDoZoom)A_PtrMsg)->Speed, ((Ptr_GrDvrPtzMsgDoZoom)A_PtrMsg)->AutoStop );
					break;
				case	E_GrDvrPtzMsgCmdDoFocus:
					LcDoFocus( ((Ptr_GrDvrPtzMsgDoFocus)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoFocus)A_PtrMsg)->IsFar,
						((Ptr_GrDvrPtzMsgDoFocus)A_PtrMsg)->Speed, ((Ptr_GrDvrPtzMsgDoFocus)A_PtrMsg)->AutoStop );
					break;
				case	E_GrDvrPtzMsgCmdDoIris:
					LcDoIris( ((Ptr_GrDvrPtzMsgDoIris)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoIris)A_PtrMsg)->IsOpen,
						((Ptr_GrDvrPtzMsgDoIris)A_PtrMsg)->Speed, ((Ptr_GrDvrPtzMsgDoIris)A_PtrMsg)->AutoStop );
					break;
				case	E_GrDvrPtzMsgCmdDoStop:
					LcDoStop( ((Ptr_GrDvrPtzMsgDoStop)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoAutoFocus:
					LcDoAutoFocus( ((Ptr_GrDvrPtzMsgDoAutoFocus)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuCall:
					LcDoMenuCall( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuExit:
					LcDoMenuExit( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoPrsSet:
					LcDoPresetSet( ((Ptr_GrDvrPtzMsgDoPrsSet)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoPrsSet)A_PtrMsg)->Id );
					break;
				case	E_GrDvrPtzMsgCmdDoPrsMove:
					LcDoPresetMove( ((Ptr_GrDvrPtzMsgDoPrsMove)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgDoPrsMove)A_PtrMsg)->Id,
						((Ptr_GrDvrPtzMsgDoPrsMove)A_PtrMsg)->Speed );
					break;
				case	E_GrDvrPtzMsgCmdIsMenuMode:
					*(((Ptr_GrDvrPtzMsgIsMenuMode)A_PtrMsg)->PtrRslt)	=	LcIsMenuAble( ((Ptr_GrDvrPtzMsgIsMenuMode)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdTourRun:
					LcTourRun( ((Ptr_GrDvrPtzMsgTourRun)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgTourRun)A_PtrMsg)->IsRun );
					break;
				case	E_GrDvrPtzMsgCmdMenuOk:
					LcDoMenuOk( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdMenuCancel:
					LcDoMenuCancel( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdTourSave:
					LcTourSave( ((Ptr_GrDvrPtzMsgTourSave)A_PtrMsg)->Ch, 
						((Ptr_GrDvrPtzMsgTourSave)A_PtrMsg)->PtrTourTbl, ((Ptr_GrDvrPtzMsgTourSave)A_PtrMsg)->Cnt );
					break;
				case	E_GrDvrPtzMsgCmdIdView:
					LcIdView( ((Ptr_GrDvrPtzMsgIdView)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgIdView)A_PtrMsg)->IsOn );
					break;
				case	E_GrDvrPtzMsgCmdIdSet:
					LcIdSet( ((Ptr_GrDvrPtzMsgIdSet)A_PtrMsg)->Ch, ((Ptr_GrDvrPtzMsgIdSet)A_PtrMsg)->IdNew );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuLeft:
					LcDoMenuLeft( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuRight:
					LcDoMenuRight( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuUp:
					LcDoMenuUp( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
				case	E_GrDvrPtzMsgCmdDoMenuDown:
					LcDoMenuDown( ((Ptr_GrDvrPtzMsgDoMenuStd)A_PtrMsg)->Ch );
					break;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcSetup(	__u8 A_Ch, __u8 A_Model, __u8 A_UartCh, __u8 A_Id, __u8 A_SpdPan, 
															__u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, BOOL8 A_IsRvsPan, 
															BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour )
{
	// local -------------------
		BOOL8	Tv_IsObjChg;
		__u8	Tv_Model;
	// code --------------------
		//patch model value
		Tv_Model	=	A_Model;
		if ( E_GrDvrPtzMaxModelCnt <= A_Model )
		{
			Tv_Model	=	0;	
		}

		//check need change object
		Tv_IsObjChg	=	FALSE;
		if ( m_ChTbl[A_Ch].Model != Tv_Model )
		{
			Tv_IsObjChg	=	TRUE;
		}
		else
		{
			if ( 0 != Tv_Model )
			{
				if ( (A_Id != m_ChTbl[A_Ch].Id ) || (A_UartCh != m_ChTbl[A_Ch].UartCh) )
				{
					Tv_IsObjChg	=	TRUE;
				}
			}
		}
		//check do object change
		if ( Tv_IsObjChg )
		{
			//release old object
			delete	m_ChTbl[A_Ch].ObjPrtc;
			m_ChTbl[A_Ch].ObjPrtc	=	LcPrtcCreate( Tv_Model, A_UartCh, A_Id, A_Ch );
		}
		//update
		m_ChTbl[A_Ch].Model			=	Tv_Model;
		m_ChTbl[A_Ch].UartCh		=	A_UartCh;
		m_ChTbl[A_Ch].Id				=	A_Id;

		//etc configuration
		m_ChTbl[A_Ch].SpdPan		=	A_SpdPan;
		m_ChTbl[A_Ch].SpdTilt		=	A_SpdTilt;
		m_ChTbl[A_Ch].SpdZoom		=	A_SpdZoom;
		m_ChTbl[A_Ch].SpdFocus	=	A_SpdFocus;
		m_ChTbl[A_Ch].IsRvsPan	=	A_IsRvsPan;
		m_ChTbl[A_Ch].IsRvsTilt	=	A_IsRvsTilt;
		m_ChTbl[A_Ch].IsHwTour	=	A_IsHwTour;

		//reset value
		m_ChTbl[A_Ch].AutoStopCnt	=	0;
		m_ChTbl[A_Ch].IsTouring		=	FALSE;
		m_ChTbl[A_Ch].TourIdx			=	0;
		m_ChTbl[A_Ch].TourTime		=	0;

		//zero table
		GrDumyZeroMem( m_ChTbl[A_Ch].TourTbl, sizeof(St_GrDvrPtzTourItm) * E_GrDvrPtzMaxTourCnt );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcTourSave( __u8 A_Ch, Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
	// code --------------------
		//copy tour table
		GrDumyCopyMem( m_ChTbl[A_Ch].TourTbl, A_PtrTbl, sizeof(St_GrDvrPtzTourItm) * (__u32)A_Cnt );
		//updaet hardware touring
		if ( m_ChTbl[A_Ch].IsHwTour )
		{
			m_ChTbl[A_Ch].ObjPrtc->TourSet( m_ChTbl[A_Ch].TourTbl, A_Cnt );
		}
}
//--------------------------------------------------------------------
Cls_GrDvrPtzPtBase*	Cls_GrDvrPtzMng::LcPrtcCreate( __u8 A_Model, BOOL8 A_UartCh, BOOL8 A_Id, __u8 A_Ch )
{
	// local -------------------
		Cls_GrDvrPtzPtBase*	Tv_Result;
	// code --------------------
		//create object
		switch( A_Model )
		{
			case	1:
				//elmo 400c
				Tv_Result	=	(Cls_GrDvrPtzPtElmo*) new Cls_GrDvrPtzPtElmo( A_UartCh, A_Id, A_Ch );
				break;
			case	2:
				//pelco-d
				Tv_Result	=	(Cls_GrDvrPtzPtPelcoD*) new Cls_GrDvrPtzPtPelcoD( A_UartCh, A_Id, A_Ch );
				break;
			case	3:
				//pma200hf
				Tv_Result	=	(Cls_GrDvrPtzPtPwtech*) new Cls_GrDvrPtzPtPwtech( A_UartCh, A_Id, A_Ch );
				break;
			case	4:
				//samsung - s
				Tv_Result	=	(Cls_GrDvrPtzPtSams*) new Cls_GrDvrPtzPtSams( A_UartCh, A_Id, A_Ch );
				break;
			case	5:
				//d-max
				Tv_Result	=	(Cls_GrDvrPtzPtDmax*) new Cls_GrDvrPtzPtDmax( A_UartCh, A_Id, A_Ch );
				break;
			case	6:
				//lvc c100hm
				Tv_Result	=	(Cls_GrDvrPtzPtC100hm*) new Cls_GrDvrPtzPtC100hm( A_UartCh, A_Id, A_Ch );
				break;
			case	7:
				Tv_Result	=	(Cls_GrDvrPtzPtNuvico*) new Cls_GrDvrPtzPtNuvico( A_UartCh, A_Id, A_Ch );
				break;
			case	8:
				Tv_Result	=	(Cls_GrDvrPtzPtPelcoP*) new Cls_GrDvrPtzPtPelcoP( A_UartCh, A_Id, A_Ch );
				break;
			case	9:
				Tv_Result	=	(Cls_GrDvrPtzPtAp8777z*) new Cls_GrDvrPtzPtAp8777z( A_UartCh, A_Id, A_Ch );
				break;
			case	10:
				Tv_Result	=	(Cls_GrDvrPtzPtC201n*) new Cls_GrDvrPtzPtC201n( A_UartCh, A_Id, A_Ch );
				break;
			case 11:
				Tv_Result	=	(Cls_GrDvrPtzPtOrx*) new Cls_GrDvrPtzPtOrx( A_UartCh, A_Id, A_Ch );
				break;
			case 12:
				Tv_Result	=	(Cls_GrDvrPtzPtLilin2*) new Cls_GrDvrPtzPtLilin2( A_UartCh, A_Id, A_Ch );
				break;
			case 13:
				Tv_Result	=	(Cls_GrDvrPtzPtHoneywell*) new Cls_GrDvrPtzPtHoneywell( A_UartCh, A_Id, A_Ch );
				break;
			case 14:
				Tv_Result	=	(Cls_GrDvrPtzPtSamt*) new Cls_GrDvrPtzPtSamt( A_UartCh, A_Id, A_Ch, FALSE );
				break;
			case 15:
				Tv_Result	=	(Cls_GrDvrPtzPtSamt*) new Cls_GrDvrPtzPtSamt( A_UartCh, A_Id, A_Ch, TRUE );
				break;
			case 16:
				Tv_Result	=	(Cls_GrDvrPtzPtTsd300*) new Cls_GrDvrPtzPtTsd300( A_UartCh, A_Id, A_Ch );
				break;
			case 17://!!new112-4
				Tv_Result	=	(Cls_GrDvrPtzPtFastraxII*) new Cls_GrDvrPtzPtFastraxII( A_UartCh, A_Id, A_Ch );
				break;	
			case 18://!!new112-4
				Tv_Result	=	(Cls_GrDvrPtzPtFastraxIIE*) new Cls_GrDvrPtzPtFastraxIIE( A_UartCh, A_Id, A_Ch );
				break;
			case 19://!!new112-5-6
				Tv_Result	=	(Cls_GrDvrPtzPtUTCFixed*) new Cls_GrDvrPtzPtUTCFixed( A_UartCh, A_Id, A_Ch );
				break;
			case 20://!!new112-5-6
				Tv_Result	=	(Cls_GrDvrPtzPtUTCDmax*) new Cls_GrDvrPtzPtUTCDmax( A_UartCh, A_Id, A_Ch );
				break;
			case 21://!!new112-5-6
				Tv_Result	=	(Cls_GrDvrPtzPtUTCPelcoD*) new Cls_GrDvrPtzPtUTCPelcoD( A_UartCh, A_Id, A_Ch );
				break;
			case 22://!!new119-1
				Tv_Result	=	(Cls_GrDvrPtzPtAp8777z*) new Cls_GrDvrPtzPtAp8777z( A_UartCh, A_Id, A_Ch );
				break;
			default:
				//none
				Tv_Result	=	(Cls_GrDvrPtzPtBase*) new Cls_GrDvrPtzPtBase( 0, 0, 0 );
				break;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::Setup( __u8 A_Ch, __u8 A_Model, __u8 A_UartCh, __u8 A_Id, __u8 A_SpdPan, 
													__u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, BOOL8 A_IsRvsPan, 
													BOOL8 A_IsRvsTilt,  BOOL8 A_IsHwTour )
{
	// local -------------------
		St_GrDvrPtzMsgSetup	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdSetup;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.UartCh				=	A_UartCh;
			Tv_Msg.Id						=	A_Id;
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
void	Cls_GrDvrPtzMng::TourSave( __u8 A_Ch, Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt )
{
	// local -------------------
		St_GrDvrPtzMsgTourSave	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdTourSave;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Cnt					=	A_Cnt;
			Tv_Msg.PtrTourTbl		=	A_PtrTbl;
			if ( 0 != A_Cnt )
			{
				//patch count
				if ( E_GrDvrPtzMaxTourCnt < A_Cnt )
				{
					Tv_Msg.Cnt	=	E_GrDvrPtzMaxTourCnt;
				}
				m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
			}
		}
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrDvrPtzMng::ModelNameGet( __u8 A_Model )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;

		if ( E_GrDvrPtzMaxModelCnt > A_Model )
		{
			Tv_Result	=	V_GrDvrPtzMngModelNameTbl[A_Model];
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, 
																	 BOOL8 A_IsDown, __u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_AutoStop;
		BOOL8	Tv_IsLeft;
		BOOL8	Tv_IsRight;
		BOOL8	Tv_IsUp;
		BOOL8	Tv_IsDown;
		__u8	Tv_SpdPan;
		__u8	Tv_SpdTilt;
	// code --------------------
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
		if ( E_GrDvrPtzPtMaxSpeed < A_SpdPan  )
		{
			Tv_SpdPan	=	m_ChTbl[A_Ch].SpdPan;
		}
		Tv_SpdTilt	=	A_SpdTilt;
		if ( E_GrDvrPtzPtMaxSpeed < A_SpdTilt  )
		{
			Tv_SpdTilt	=	m_ChTbl[A_Ch].SpdTilt;
		}
		Tv_AutoStop	=	m_ChTbl[A_Ch].ObjPrtc->DoPanTilt( Tv_IsLeft, Tv_IsRight, Tv_IsUp, Tv_IsDown, 
			Tv_SpdPan, Tv_SpdTilt, A_AutoStop );
		//update auto stop
		m_ChTbl[A_Ch].AutoStopCnt	=	Tv_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_AutoStop;
		__u8	Tv_Spd;
	// code --------------------
		//run
		Tv_Spd	=	A_Speed;
		if ( E_GrDvrPtzPtMaxSpeed < A_Speed )
		{
			Tv_Spd	=	m_ChTbl[A_Ch].SpdZoom;
		}
		Tv_AutoStop	=	m_ChTbl[A_Ch].ObjPrtc->DoZoom( A_IsWide, Tv_Spd, A_AutoStop );
		//update auto stop
		m_ChTbl[A_Ch].AutoStopCnt	=	Tv_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_AutoStop;
		__u8	Tv_Spd;
	// code --------------------
		//run
		Tv_Spd	=	A_Speed;
		if ( E_GrDvrPtzPtMaxSpeed < A_Speed )
		{
			Tv_Spd	=	m_ChTbl[A_Ch].SpdFocus;
		}
		Tv_AutoStop	=	m_ChTbl[A_Ch].ObjPrtc->DoFocus( A_IsFar, Tv_Spd, A_AutoStop );
		//update auto stop
		m_ChTbl[A_Ch].AutoStopCnt	=	Tv_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		__u8	Tv_AutoStop;
		__u8	Tv_Spd;
	// code --------------------
		//run
		Tv_Spd	=	A_Speed;
		if ( E_GrDvrPtzPtMaxSpeed < A_Speed )
		{
			Tv_Spd	=	m_ChTbl[A_Ch].SpdFocus;
		}
		Tv_AutoStop	=	m_ChTbl[A_Ch].ObjPrtc->DoIris( A_IsOpen, Tv_Spd, A_AutoStop );
		//update auto stop
		m_ChTbl[A_Ch].AutoStopCnt	=	Tv_AutoStop;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoStop( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoStop();
		m_ChTbl[A_Ch].AutoStopCnt	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoAutoFocus( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoAutoFocus();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuCall( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoMenuCall();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuExit( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoMenuExit();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuOk( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoMenuOk();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuCancel( __u8 A_Ch )
{
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoMenuCancel();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoPresetSet( __u8 A_Ch, __u8 A_Id )
{
		//tour stanby
		LcTourStanby( A_Ch );
		//run
		m_ChTbl[A_Ch].ObjPrtc->DoPresetSet( A_Id );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		__u8	Tv_Spd;
	// code --------------------
		//tour stanby
		LcTourStanby( A_Ch );
		//run
		Tv_Spd	=	A_Speed;
		if ( E_GrDvrPtzPtMaxSpeed < A_Speed )
		{
			Tv_Spd	=	m_ChTbl[A_Ch].SpdPan;
		}
		m_ChTbl[A_Ch].ObjPrtc->DoPresetMove( A_Id, Tv_Spd );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzMng::LcIsMenuAble( __u8 A_Ch )
{
		return	m_ChTbl[A_Ch].ObjPrtc->IsMenuAble();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::RtlTimer( __u32 A_Hndl )
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_TourIdx;
		__u8	Tv_WkIdx;
	// code --------------------
		if ( m_HndlTimer == A_Hndl )
		{
			//update auto stop
			for ( Tv_Ch =0;Tv_Ch < m_MaxChCnt;Tv_Ch++ )
			{
				//check auto stop requested
				if ( 0 != m_ChTbl[Tv_Ch].AutoStopCnt )
				{
					//decrease counter
					m_ChTbl[Tv_Ch].AutoStopCnt --;
					//check time at runable
					if ( 0 == m_ChTbl[Tv_Ch].AutoStopCnt )
					{
						//do auto stop
						m_ChTbl[Tv_Ch].ObjPrtc->DoStop();
					}
				}
				//update tour
				if ( E_GrDvrPtzTourTimePeriodCnt <= m_TourTimer )
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
										m_ChTbl[Tv_Ch].ObjPrtc->DoPresetMove( m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].PresetId, m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Spd );
										//update time
										m_ChTbl[Tv_Ch].TourTime	=	m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain;
									}
									//go next index
									for ( Tv_WkIdx=0;Tv_WkIdx < E_GrDvrPtzMaxTourCnt;Tv_WkIdx++ )
									{
										//go next
										Tv_TourIdx ++;
										if ( E_GrDvrPtzMaxTourCnt <= Tv_TourIdx )
										{
											Tv_TourIdx	=	0;
										}
										//check able
										if ( 0 != m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain )
										{
											//finded
											break;
										}
									}
									//check finded
									if ( 0 != m_ChTbl[Tv_Ch].TourTbl[Tv_TourIdx].Maintain )
									{
										//finded
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
							//tour stanby count decrease
							m_ChTbl[Tv_Ch].TourStbyCnt --;
							if ( (0 == m_ChTbl[Tv_Ch].TourStbyCnt) && m_ChTbl[Tv_Ch].IsHwTour )
							{
								//restore tour
								m_ChTbl[Tv_Ch].ObjPrtc->TourRun();
							}
						}
					}
					else
					{
						//reset tour stanby value zero
						m_ChTbl[Tv_Ch].TourStbyCnt	=	0;
					}
				}
				//protocol timer
				m_ChTbl[Tv_Ch].ObjPrtc->DoTimer();
			}
			//tour timer
			if ( E_GrDvrPtzTourTimePeriodCnt <= m_TourTimer )
			{
				//reset tour time counter
				m_TourTimer	=	0;
			}
			m_TourTimer ++;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, 
																 BOOL8 A_IsDown, __u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop )
{
	// local -------------------
		St_GrDvrPtzMsgDoPanTilt	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoPanTilt;
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
void	Cls_GrDvrPtzMng::DoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrDvrPtzMsgDoZoom	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoZoom;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsWide				=	A_IsWide;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrDvrPtzMsgDoFocus	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoFocus;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsFar				=	A_IsFar;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop )
{
	// local -------------------
		St_GrDvrPtzMsgDoIris	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoIris;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsOpen				=	A_IsOpen;
			Tv_Msg.Speed				=	A_Speed;
			Tv_Msg.AutoStop			=	A_AutoStop;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoStop( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoStop	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoStop;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoAutoFocus( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoAutoFocus	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoAutoFocus;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuCall( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuCall;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuExit( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuExit;
			Tv_Msg.Ch						=	A_Ch;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuOk( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdMenuOk;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuCancel( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdMenuCancel;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoPresetSet( __u8 A_Ch, __u8 A_Id )
{
	// local -------------------
		St_GrDvrPtzMsgDoPrsSet	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoPrsSet;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Id						=	A_Id;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed )
{
	// local -------------------
		St_GrDvrPtzMsgDoPrsMove	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoPrsMove;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.Id						=	A_Id;
			Tv_Msg.Speed				=	A_Speed;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzMng::IsMenuAble( __u8 A_Ch )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrDvrPtzMsgIsMenuMode	Tv_Msg;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdIsMenuMode;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.PtrRslt			=	&Tv_Result;
		
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrPtzMng::IsTouring( __u8 A_Ch )
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
void	Cls_GrDvrPtzMng::TouringRun( __u8 A_Ch, BOOL8 A_IsRun )
{
	// local -------------------
		St_GrDvrPtzMsgTourRun	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdTourRun;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsRun				=	A_IsRun;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcTourRun( __u8 A_Ch, BOOL8 A_IsRun )
{
	// local -------------------
	// code --------------------
		if ( m_ChTbl[A_Ch].IsTouring != A_IsRun )
		{
			m_ChTbl[A_Ch].IsTouring	=	A_IsRun;
			m_ChTbl[A_Ch].TourIdx		=	0;
			m_ChTbl[A_Ch].TourTime	=	0;
			if ( m_ChTbl[A_Ch].IsHwTour )
			{
				if ( m_ChTbl[A_Ch].IsTouring )
				{
					m_ChTbl[A_Ch].ObjPrtc->TourRun();
				}
				else
				{
					m_ChTbl[A_Ch].ObjPrtc->TourStop();
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcIdView( __u8 A_Ch, BOOL8 A_IsOn )
{
		m_ChTbl[A_Ch].ObjPrtc->ExIdView( A_IsOn );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::ExIdView( __u8 A_Ch, BOOL8 A_IsOn )
{
	// local -------------------
		St_GrDvrPtzMsgIdView	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdIdView;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IsOn					=	A_IsOn;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcIdSet( __u8 A_Ch, __u8 A_IdNew )
{
		m_ChTbl[A_Ch].ObjPrtc->ExIdSet( A_IdNew );
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::ExIdSet( __u8 A_Ch, __u8 A_IdNew )
{
	// local -------------------
		St_GrDvrPtzMsgIdSet	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdIdSet;
			Tv_Msg.Ch						=	A_Ch;
			Tv_Msg.IdNew				=	A_IdNew;
			m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::TourStanbyTimeSet( __u8 A_Ch, __u8 A_Sec )
{
		m_ChTbl[A_Ch].TourStanby	=	A_Sec;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrPtzMng::TourStanbyTimeGet( __u8 A_Ch )
{
		return	m_ChTbl[A_Ch].TourStanby;
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcTourStanby( __u8 A_Ch )
{
		m_ChTbl[A_Ch].TourStbyCnt	=	m_ChTbl[A_Ch].TourStanby;
		m_ChTbl[A_Ch].TourIdx			=	0;		//reset touring
		m_ChTbl[A_Ch].TourTime		=	0;
		if ( m_ChTbl[A_Ch].IsTouring && m_ChTbl[A_Ch].IsHwTour && (0 == m_ChTbl[A_Ch].TourStbyCnt) )
		{
			//stop tour
			m_ChTbl[A_Ch].ObjPrtc->TourStop();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuLeft( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuLeft;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuRight( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuRight;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuUp( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuUp;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::DoMenuDown( __u8 A_Ch )
{
	// local -------------------
		St_GrDvrPtzMsgDoMenuStd	Tv_Msg;
	// code --------------------
		if ( m_MaxChCnt > A_Ch )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrDvrPtzMsgCmdDoMenuDown;
			Tv_Msg.Ch						=	A_Ch;
			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuLeft( __u8 A_Ch )
{
		m_ChTbl[A_Ch].ObjPrtc->DoMenuLeft();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuRight( __u8 A_Ch )
{
		m_ChTbl[A_Ch].ObjPrtc->DoMenuRight();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuUp( __u8 A_Ch )
{
		m_ChTbl[A_Ch].ObjPrtc->DoMenuUp();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::LcDoMenuDown( __u8 A_Ch )
{
		m_ChTbl[A_Ch].ObjPrtc->DoMenuDown();
}
//--------------------------------------------------------------------
void	Cls_GrDvrPtzMng::MaxSpdToggle( __u8 A_Ch )
{
	// local -------------------
	// code --------------------
		if ( E_GrDvrMaxChCnt > A_Ch )
		{
			// check now speed
			if ( (E_GrDvrPtzPtMaxSpeed == m_ChTbl[A_Ch].SpdPan) && (E_GrDvrPtzPtMaxSpeed == m_ChTbl[A_Ch].SpdTilt) && (E_GrDvrPtzPtMaxSpeed == m_ChTbl[A_Ch].SpdZoom) )
			{
				// normal speed
				m_ChTbl[A_Ch].SpdPan	=	2;
				m_ChTbl[A_Ch].SpdTilt	=	2;
				m_ChTbl[A_Ch].SpdZoom	=	2;
			}
			else
			{
				// max speed
				m_ChTbl[A_Ch].SpdPan	=	E_GrDvrPtzPtMaxSpeed;
				m_ChTbl[A_Ch].SpdTilt	=	E_GrDvrPtzPtMaxSpeed;
				m_ChTbl[A_Ch].SpdZoom	=	E_GrDvrPtzPtMaxSpeed;
			}
		}
}
//--------------------------------------------------------------------

