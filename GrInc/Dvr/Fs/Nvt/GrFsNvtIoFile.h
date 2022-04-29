/*
GAUSE DVR file system v5 file

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrFsNvtIoFile
#define	_EPC_GrFsNvtIoFile

//====================================================================
// option

//#define	E_GrFsNvtIoFileChkLogSeek

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrBufTool.h>
#include <GrTaskSvr.h>
#include <GrTaskCli.h>
#include <GrTimeTool.h>
#include <GrError.h>

#include <Dvr/Fs/Nvt/GrFsNvtBase.h>
#include <Dvr/Fs/Nvt/GrFsNvtHdd.h>
#include <Dvr/Fs/Nvt/GrFsNvtRgrp.h>

//====================================================================
//constance

#define E_GrFsNvtIoFileTimerPeriod							100

// message command
#define E_GrFsNvtIoFileMsgInit									1		/* initialize */
#define E_GrFsNvtIoFileMsgFinish								2		/* initialize */

//====================================================================
//global macro

//====================================================================
//global type


// message
typedef	struct St_GrFsNvtIoFileMsgBase
{
	St_GrTaskMsgHd	MsgHd;
	__u32		Cmd;
}	*Ptr_GrFsNvtIoFileMsgBase;


//====================================================================
//class

class	Cls_GrFsNvtIoFile : public	Cls_GrFsNvtIoBase
{
	private:
		__u32	m_MsgMng;
		__u32	m_HndlTimer;


	protected:
		//local


		//event
		virtual	BOOL8	RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg);		/* return = message processed */
		virtual	void	RtlTimer(__u32 A_Hndl);
		//virtual	BOOL8	RtlBkgProc(void);											/* return = background processed */

	public:
		Cls_GrFsNvtIoFile(Cls_GrTaskSvr* A_TaskSvr, __u32 A_CchMinCnt, __u32 A_CchDataCnt);
		virtual		~Cls_GrFsNvtIoFile();

		// control
		Def_GrErrCode	ImmFileOpen(char* A_StrFn);

		// io base
#if 0
		virtual	Def_GrErrCode	ImmDevOpen(__u8 A_SubType, Ptr_GrFsNvtIoDevInfo A_PtrRtInfo);
		virtual	Def_GrErrCode	ImmDevClose(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);

		virtual	Def_GrErrCode	ImmReadFwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		virtual	Def_GrErrCode	ImmReadBwNext(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
		virtual	Def_GrErrCode	ImmReadFwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);
		virtual	Def_GrErrCode	ImmReadBwByTime(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, Def_GrTime A_Time);

		virtual	Def_GrErrCode	ImmReadFwByHndl(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl, __u64 A_Hndl);
		virtual	Def_GrErrCode	ImmReadFwStart(__u8 A_SubType, Ptr_GrFsNvtSeekCtrl A_PtrCtrl);
#endif

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


