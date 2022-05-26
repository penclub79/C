/*
 DVR serial I/O device manager

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <P3/Sio/GrP3SioMng.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <stdlib.h>

#include <P3/Sio/GrP3SioPiCj1000.h>
#include <P3/Sio/GrP3SioWtx1200.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

Def_StrName	V_GrP3SioMngMdlNameTbl[E_GrP3SioModelMaxCnt]	=	
{
	{	"[None]"	},
	{	"Cj1000"	},
	{	"Wtx1200"	},
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrP3SioMng::Cls_GrP3SioMng( Cls_GrTaskSvr* A_TaskSvr ):
Cls_GrTaskCli()
{
	// local -------------------
		//__u8	Tv_WkIdx;
	// code --------------------
		m_TaskSvr	=	A_TaskSvr;
		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		//init
		m_ObjSio	=	NULL;

		//register task
		m_TaskSvr->RegistTimerClient( this, E_GrP3SioMngTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrP3SioMng::~Cls_GrP3SioMng()
{
	// local -------------------
		//__u8	Tv_WkIdx;
	// code --------------------
		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );
		m_TaskSvr->UnregistMsgClient( this );

		//release object
		if ( NULL != m_ObjSio )
		{
			delete	m_ObjSio;
		}

		GrTaskSvrMsgCmdFree( m_MsgAloc );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3SioMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrP3SioMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrP3SioMsgCmdSetup:
					ImmSetup( ((Ptr_GrP3SioMsgSetup)A_PtrMsg)->Model, ((Ptr_GrP3SioMsgSetup)A_PtrMsg)->Uart,
						((Ptr_GrP3SioMsgSetup)A_PtrMsg)->Id );
					break;
				case E_GrP3SioMsgCmdPiCmd:
					ImmPiCmd( ((Ptr_GrP3SioMsgPiCmd)A_PtrMsg)->PiCmd, ((Ptr_GrP3SioMsgPiCmd)A_PtrMsg)->Para0, ((Ptr_GrP3SioMsgPiCmd)A_PtrMsg)->Para1 );
					break;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::RtlTimer( __u32 A_Hndl )
{
		if ( NULL != m_ObjSio )
		{
			m_ObjSio->RtlTimer();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::ImmSetup(	__u8 A_Model, __u8 A_Uart, __u8 A_Id )
{
	// local -------------------
	// code --------------------
		// delete previous object
		if ( NULL != m_ObjSio )
		{
			delete	m_ObjSio;
			m_ObjSio	=	NULL;
		}

		switch ( A_Model )
		{
			case 1:
				m_ObjSio	=	(Cls_GrP3SioPiBase*) new Cls_GrP3SioPiCj1000( (void*)this, A_Uart, A_Id );
				break;
			case 2:
				m_ObjSio	=	(Cls_GrP3SioPiBase*) new Cls_GrP3SioWtx1200( (void*)this, A_Uart, A_Id );
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::SafeSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id )
{
	// local -------------------
		St_GrP3SioMsgSetup	Tv_Msg;
	// code --------------------
		if ( E_GrP3SioModelMaxCnt > A_Model )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP3SioMsgCmdSetup;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.Uart					=	A_Uart;
			Tv_Msg.Id						=	A_Id;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::ModelNameGet( __u8 A_Model, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		GrStrClear( A_StrRt );
		if ( E_GrP3SioModelMaxCnt > A_Model )
		{
			GrStrStrToWstr( A_StrRt, V_GrP3SioMngMdlNameTbl[A_Model] );
		}
}
//--------------------------------------------------------------------
__u8		Cls_GrP3SioMng::ModelCntGet( void )
{
	// local -------------------
	// code --------------------
		return	E_GrP3SioModelMaxCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::RtlExtCmd( __u32 A_Cmd, void* A_PtrPara )
{

}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::ImmPiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 )
{
		if ( NULL != m_ObjSio )
		{
			m_ObjSio->RtlPiCmd( A_Cmd, A_Para0, A_Para1 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP3SioMng::SafePiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 )
{
	// local -------------------
		St_GrP3SioMsgPiCmd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP3SioMsgCmdPiCmd;
		Tv_Msg.PiCmd				=	A_Cmd;
		Tv_Msg.Para0				=	A_Para0;
		Tv_Msg.Para1				=	A_Para1;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP3SioMng::RtlBkgProc( void )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( NULL != m_ObjSio )
		{
			if ( m_ObjSio->RtlBkgProc() )
			{
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
