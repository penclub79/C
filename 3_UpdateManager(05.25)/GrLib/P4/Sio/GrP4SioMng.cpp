/*
 DVR serial I/O device manager

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <P4/Sio/GrP4SioMng.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>

#include <stdlib.h>

#include <P4/Sio/GrP4SioPiCj1000.h>
#include <P4/Sio/GrP4SioWtx1200.h>

//====================================================================
//local const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

Def_StrName	V_GrP4SioMngMdlNameTbl[E_GrP4SioModelMaxCnt]	=	
{
	{	"[None]"	},
	{	"Cj1000"	},
	{	"Wtx1200"	},
};

//====================================================================

//--------------------------------------------------------------------
Cls_GrP4SioMng::Cls_GrP4SioMng( Cls_GrTaskSvr* A_TaskSvr ):
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
		m_TaskSvr->RegistTimerClient( this, E_GrP4SioMngTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrP4SioMng::~Cls_GrP4SioMng()
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
BOOL8	Cls_GrP4SioMng::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;
		if ( m_MsgAloc == A_PtrMsg->Cmd )
		{
			switch( ((Ptr_GrP4SioMsgHd)A_PtrMsg)->Cmd )
			{
				case	E_GrP4SioMsgCmdSetup:
					ImmSetup( ((Ptr_GrP4SioMsgSetup)A_PtrMsg)->Model, ((Ptr_GrP4SioMsgSetup)A_PtrMsg)->Uart,
						((Ptr_GrP4SioMsgSetup)A_PtrMsg)->Id );
					break;
				case E_GrP4SioMsgCmdPiCmd:
					ImmPiCmd( ((Ptr_GrP4SioMsgPiCmd)A_PtrMsg)->PiCmd, ((Ptr_GrP4SioMsgPiCmd)A_PtrMsg)->Para0, ((Ptr_GrP4SioMsgPiCmd)A_PtrMsg)->Para1 );
					break;
			}

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::RtlTimer( __u32 A_Hndl )
{
		if ( NULL != m_ObjSio )
		{
			m_ObjSio->RtlTimer();
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::ImmSetup(	__u8 A_Model, __u8 A_Uart, __u8 A_Id )
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
				m_ObjSio	=	(Cls_GrP4SioPiBase*) new Cls_GrP4SioPiCj1000( (void*)this, A_Uart, A_Id );
				break;
			case 2:
				m_ObjSio	=	(Cls_GrP4SioPiBase*) new Cls_GrP4SioWtx1200( (void*)this, A_Uart, A_Id );
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::SafeSetup( __u8 A_Model, __u8 A_Uart, __u8 A_Id )
{
	// local -------------------
		St_GrP4SioMsgSetup	Tv_Msg;
	// code --------------------
		if ( E_GrP4SioModelMaxCnt > A_Model )
		{
			Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
			Tv_Msg.Hd.Cmd				=	E_GrP4SioMsgCmdSetup;
			Tv_Msg.Model				=	A_Model;
			Tv_Msg.Uart					=	A_Uart;
			Tv_Msg.Id						=	A_Id;

			m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::ModelNameGet( __u8 A_Model, WCHAR* A_StrRt )
{
	// local -------------------
	// code --------------------
		GrStrClear( A_StrRt );
		if ( E_GrP4SioModelMaxCnt > A_Model )
		{
			GrStrStrToWstr( A_StrRt, V_GrP4SioMngMdlNameTbl[A_Model] );
		}
}
//--------------------------------------------------------------------
__u8		Cls_GrP4SioMng::ModelCntGet( void )
{
	// local -------------------
	// code --------------------
		return	E_GrP4SioModelMaxCnt;
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::RtlExtCmd( __u32 A_Cmd, void* A_PtrPara )
{

}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::ImmPiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 )
{
		if ( NULL != m_ObjSio )
		{
			m_ObjSio->RtlPiCmd( A_Cmd, A_Para0, A_Para1 );
		}
}
//--------------------------------------------------------------------
void	Cls_GrP4SioMng::SafePiCmd( __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1 )
{
	// local -------------------
		St_GrP4SioMsgPiCmd	Tv_Msg;
	// code --------------------
		Tv_Msg.Hd.MsgHd.Cmd	=	m_MsgAloc;
		Tv_Msg.Hd.Cmd				=	E_GrP4SioMsgCmdPiCmd;
		Tv_Msg.PiCmd				=	A_Cmd;
		Tv_Msg.Para0				=	A_Para0;
		Tv_Msg.Para1				=	A_Para1;

		m_TaskSvr->MsgPost( (Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg) );
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP4SioMng::RtlBkgProc( void )
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
