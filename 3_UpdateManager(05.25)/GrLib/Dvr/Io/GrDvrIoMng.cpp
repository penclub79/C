/*
 dvr pantilt

 */

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<GrDebug.h>
#include	<Dvr/Io/GrDvrIoMng.h>
#include	<GrDumyTool.h>
#include	<GrTimeTool.h>
#include	<GrStrTool.h>
#include  <DvrRtl.h>

#include	<stdlib.h>
#include	<Dvr/Io/GrDvrIoCliCj1000.h>
#include	<Dvr/Io/GrDvrIoCliWtx1200.h>
#include	<Dvr/Io/GrDvrIoCliSc08gtc.h>
#include	<Dvr/Io/GrDvrIoCliHtx5000.h>
#include	<Dvr/Io/GrDvrIoCliRTC.h>
#ifdef E_DvrRtlTestVersion_PTZCtrl
#include	<Dvr/Io/GrDvrIoCliCtrl.h>
#endif

//====================================================================
//local const
#define E_GrDvrIoTimerPriod					100

#define E_GrDvrIoMsgCmdSetup				0			/* setup : St_GrDvrIoMsgSetup */
#define E_GrDvrIoMsgCmdUserAction		1			/* user action : St_GrDvrIoMsgUserAction */
#define	E_GrDvrIoMsgCmdTimeSet			2			/* RTC Box Time set : St_GrDvrIoMsgTimeSet */
#define	E_GrDvrIoMsgCmdIsRtcBox			3			/* RTC Box exist?  : St_GrDvrIoMsgTimeSet */

//====================================================================
//local type

typedef	struct St_GrDvrIoMsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrDvrIoMsgHd;

typedef	struct St_GrDvrIoMsgSetup 
{
	St_GrDvrIoMsgHd	Hd;
	__u8	Model;
	__u8	Uart;
	__u8	Id;
}	*Ptr_GrDvrIoMsgSetup;

typedef struct St_GrDvrIoMsgUserAction
{
	St_GrDvrIoMsgHd	Hd;
	__u32	ActId;
}	*Ptr_GrDvrIoMsgUserAction;


typedef struct St_GrDvrIoMsgTimeSet
{
	St_GrDvrIoMsgHd	Hd;
	Def_GrTime Time;
}	*Ptr_GrDvrIoMsgTimeSet;

typedef struct St_GrDvrIoMsgIsRtcBox
{
	St_GrDvrIoMsgHd	Hd;
}	*Ptr_GrDvrIoMsgIsRtcBox;

//====================================================================
//local function

//====================================================================
//var
Def_WstrName	V_GrDvrIoMngModelNameTbl[E_GrDvrIoModelCnt]	=	
{
	{ L"[None]"	},
	{ L"[PSD-CJ1000]"	},
	{ L"[ITC-LDRM]"	},
	{ L"[SC-04GTC]"	},
	{ L"[HTX-5000]"	},
	{ L"[RTC-BOX]"	},
#ifdef E_DvrRtlTestVersion_PTZCtrl
	{ L"[Front-Ctrl]"	},
#endif
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrIoMng::Cls_GrDvrIoMng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_ObjIo		=	NULL;

		//regist task
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrDvrIoTimerPriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrDvrIoMng::~Cls_GrDvrIoMng()
{
	// local -------------------
	// code --------------------
		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release protocol object
		if ( NULL != m_ObjIo )
		{
			delete	m_ObjIo;
		}

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		//check current message
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch( ((Ptr_GrDvrIoMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrDvrIoMsgCmdSetup:
					LcSetup( ((Ptr_GrDvrIoMsgSetup)A_PtrMsg)->Model, ((Ptr_GrDvrIoMsgSetup)A_PtrMsg)->Uart,
						((Ptr_GrDvrIoMsgSetup)A_PtrMsg)->Id );
					break;
				case E_GrDvrIoMsgCmdUserAction:
					LcUserAction( ((Ptr_GrDvrIoMsgUserAction)A_PtrMsg)->ActId );
					break;
				case E_GrDvrIoMsgCmdTimeSet:
					LcTimeSet( ((Ptr_GrDvrIoMsgTimeSet)A_PtrMsg)->Time );
					break;
				case E_GrDvrIoMsgCmdIsRtcBox:
					LcCheckRtcBox();
				default:
					Tv_Result	=	FALSE;
					break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoMng::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_ObjIo )
		{
			Tv_Result	=	m_ObjIo->RtlProc();
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
WCHAR*	Cls_GrDvrIoMng::ModelNameGet( __u8 A_Model )
{
	// local -------------------
		WCHAR*	Tv_Result;
	// code --------------------
		Tv_Result	=	NULL;
		if ( E_GrDvrIoModelCnt > A_Model )
		{
			Tv_Result	=	V_GrDvrIoMngModelNameTbl[A_Model];
		}
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::Setup( __u8 A_Model, __u8 A_Uart, __u8 A_Id )
{
	// local -------------------
		St_GrDvrIoMsgSetup	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrIoMsgCmdSetup;
		Tv_Msg.Model				=	A_Model;
		Tv_Msg.Uart					=	A_Uart;
		Tv_Msg.Id						=	A_Id;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::LcSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id )
{
	// local -------------------
	// code --------------------
		//release object
		if ( NULL != m_ObjIo )
		{
			delete		m_ObjIo;
			m_ObjIo		=	NULL;
		}
		//create object
		switch( A_Model )
		{
			case	0:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliBase( A_Uart, A_Id );
				break;
			case	1:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliCj1000( A_Uart, A_Id );
				break;
			case	2:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliWtx1200( A_Uart, A_Id );
				break;
			case	3:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliSc08gtc( A_Uart, A_Id );
				break;
			case	4:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliHtx5000( A_Uart, A_Id );
				break;
			case	5:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliRTC( A_Uart, A_Id );
				break;
#ifdef E_DvrRtlTestVersion_PTZCtrl
			case	6:
				m_ObjIo	=	(Cls_GrDvrIoCliBase*)new Cls_GrDvrIoCliCtrl( A_Uart, A_Id );
				break;
#endif
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::RtlTimer( __u32 A_Hndl )
{
		if ( m_HndlTimer == A_Hndl )
		{
			if ( NULL != m_ObjIo )
			{
				m_ObjIo->RtlTimer();
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::UserAction( __u32 A_ActId )
{
	// local -------------------
		St_GrDvrIoMsgUserAction	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrIoMsgCmdUserAction;
		Tv_Msg.ActId				=	A_ActId;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::LcUserAction( __u32 A_ActId )
{
		if ( NULL != m_ObjIo )
		{
			m_ObjIo->UserAction( A_ActId );
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::TimeSet( Def_GrTime A_Time )
{
	// local -------------------
	St_GrDvrIoMsgTimeSet	Tv_Msg;
	// code --------------------
	Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
	Tv_Msg.Hd.Cmd				=	E_GrDvrIoMsgCmdTimeSet;
	Tv_Msg.Time					= A_Time;
	m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}//%%
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::LcTimeSet( Def_GrTime A_Time )
{	
	if ( NULL != m_ObjIo )
		m_ObjIo->TimeSet( A_Time );	
}//%%
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::LcCheckRtcBox( void )
{
	if ( NULL != m_ObjIo )
		m_ObjIo->CheckRtcBox();
}//^^%%
//--------------------------------------------------------------------
void	Cls_GrDvrIoMng::CheckRtcBox( void )
{
	// local -------------------
		St_GrDvrIoMsgIsRtcBox	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd = m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrDvrIoMsgCmdIsRtcBox;
		m_TaskSvr->MsgSend( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}//^^%%
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoMng::RetrnRtcBox( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result = FALSE;
		if ( NULL != m_ObjIo )
			Tv_Result = m_ObjIo->RetrnRtcBox();
		return Tv_Result;
}//^^%%
//--------------------------------------------------------------------
BOOL8	Cls_GrDvrIoMng::RetrnGPS( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result = FALSE;
		if ( NULL != m_ObjIo )
			Tv_Result = m_ObjIo->RetrnGPS();
		return Tv_Result;
}
//--------------------------------------------------------------------

