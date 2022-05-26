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

#include	<Dvr/Fs/Nvt/GrFsNvtIoFile.h>

#include	<stdlib.h>

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

extern	St_GrFsNvtEnv	V_GrFsNvtEnv;

//====================================================================

//--------------------------------------------------------------------
Cls_GrFsNvtIoFile::Cls_GrFsNvtIoFile(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt) :
Cls_GrFsNvtIoBase(A_TaskSvr)
{
	// local -------------------
	// code --------------------
		//init
		m_MsgMng		=	GrTaskSvrMsgCmdAlloc();


		//register task
		//m_TaskSvr->RegistMsgClient( this );
		m_TaskSvr->RegistTimerClient( this, E_GrFsNvtIoFileTimerPeriod, &m_HndlTimer );
		m_TaskSvr->RegistBkgClient( this );

}
//--------------------------------------------------------------------
Cls_GrFsNvtIoFile::~Cls_GrFsNvtIoFile()
{
		// request finish

		//unregister timer
		m_TaskSvr->UnregistBkgClient( this );
		//m_TaskSvr->UnregistMsgClient( this );
		m_TaskSvr->UnregistTimerClient( m_HndlTimer );


		GrTaskSvrMsgCmdFree( m_MsgMng );

}
//--------------------------------------------------------------------
BOOL8	Cls_GrFsNvtIoFile::RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg )
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
			switch ( ((Ptr_GrFsNvtIoFileMsgBase)A_PtrMsg)->Cmd )
			{
				case E_GrFsNvtIoFileMsgInit:
					break;
				case E_GrFsNvtIoFileMsgFinish:
					break;

				default:
					Tv_Result	=	FALSE;
					break;
			}
			
		}
		else
		{
			Tv_Result	=	Cls_GrFsNvtIoBase::RtlMsgProc(A_PtrMsg);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrFsNvtIoFile::RtlTimer( __u32 A_Hndl )
{
		// update tick
		V_GrFsNvtEnv.Tick100ms ++;



}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrFsNvtIoFile::ImmFileOpen(char* A_StrFn)
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

