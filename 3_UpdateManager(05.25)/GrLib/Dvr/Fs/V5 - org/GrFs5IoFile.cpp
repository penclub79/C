/*
 GAUSE DVR file system version 2 manager

 */

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrStrTool.h>
#include <GrError.h>
#include <GrFileTool.h>

#include	<Dvr/Fs/V5/GrFs5IoFile.h>

#include	<stdlib.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

extern	St_GrFs5Env	V_GrFs5Env;

//====================================================================

//--------------------------------------------------------------------
Cls_GrFs5IoFile::Cls_GrFs5IoFile(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt) :
Cls_GrFs5IoBase(A_TaskSvr)
{
	// local -------------------
	// code --------------------
		//init
		m_MsgMng		=	GrTaskSvrMsgCmdAlloc();


		//register task
		//m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrFs5IoFileTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrFs5IoFile::~Cls_GrFs5IoFile()
{
		// request finish

		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		//m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgMng );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFs5IoFile::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrTaskMsgRecTimeMapDone	Tv_MsgTmDone;
	// code --------------------
		Tv_Result	=	FALSE;
		// check input message
		if ( m_MsgMng == A_PtrMsg->Cmd )
		{
			Tv_Result	=	TRUE;
			switch ( ((Ptr_GrFs5IoFileMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrFs5IoFileMsgInit:
					break;
				case E_GrFs5IoFileMsgFinish:
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		else
		{
			Tv_Result	=	Cls_GrFs5IoBase::RtlMsgProc(A_PtrMsg);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFs5IoFile::RtlTimer( __u32 A_Hndl )
{
		// update tick
		V_GrFs5Env.Tick100ms ++;



}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFs5IoFile::ImmFileOpen(char* A_StrFn)
{
	// local -------------------
	// code --------------------
		// check exist file
		if(!GrFileIsExist(A_StrFn))
		{
			return	E_GrErrNotFound;
		}

		// do close


		// load time data

		return	E_GrErrNone;
}
//--------------------------------------------------------------------

