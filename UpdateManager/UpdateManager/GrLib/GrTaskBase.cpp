/*
	task base

*/

//====================================================================
//include
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDebug.h>
#include <GrTaskBase.h>
#include <GrDumyTool.h>
#include <GrTimeTool.h>
#include <GrProcess.h>
#include <GrTaskSvr.h>

#ifdef LINUX_APP
	#undef _XOPEN_SOURCE
	#define _XOPEN_SOURCE	500
	#include <signal.h>
	#include <stdlib.h>
	#include <stdio.h>
#endif

//====================================================================
//local const


//====================================================================
//local type

//====================================================================
//global var

St_GrTaskEnv	V_GrTaskEnv	=	
{0,};

//====================================================================
//functions


//--------------------------------------------------------------------
__s32	GrTaskSvrReg(void* A_Obj)
{
	// local -------------------
		__s32		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	-1;

		//check inited
		if(0 == V_GrTaskEnv.SvrRegCnt)
		{
			GrTaskSvrMngInit();
		}
		//crit
		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find empty slot
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
		{
			if(NULL == V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
			{
				V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj	=	A_Obj;
				Tv_Result		=	(__s32)Tv_WkIdx;
				V_GrTaskEnv.SvrRegCnt++;
				break;
			}
		}

		if(0 > Tv_Result)
		{
#ifdef LINUX_APP
			printf("[BUG!!] GrTaskSvrReg - task slot is full!\n");
#else
			DbgMsgPrint("[BUG!!] GrTaskSvrReg - task slot is full!\n");
#endif
		}

		//crit
		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);

		return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8	GrTaskSvrUnreg(void* A_Obj)
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//crit
		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find empty slot
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
		{
			if(A_Obj == V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
			{
				V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj			=	NULL;
				V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Fcc			=	0;
				V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].ThrdId	=	0;
				Tv_Result		=	TRUE;
				V_GrTaskEnv.SvrRegCnt --;
				break;
			}
		}

		//crit
		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);

		if(0 == V_GrTaskEnv.SvrRegCnt)
		{
			GrTaskSvrMngFinish();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrTaskSvrFindByThreadId(Def_GrPrcThrdId A_Id)
{
	// local -------------------
		void*	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
	//init
	Tv_Result		=	NULL;

	//find empty slot
	for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
	{
		if(NULL != V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
		{
			if(A_Id == V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].ThrdId)
			{
				Tv_Result		=	V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj;
				break;
			}
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrTaskSvrMngInit(void)
{
		GrDumyZeroMem(&V_GrTaskEnv, sizeof(V_GrTaskEnv));
		GrPrcCritInit(&V_GrTaskEnv.SvrCrit);
}
//--------------------------------------------------------------------
void	GrTaskSvrMngFinish(void)
{
		GrPrcCritDel(&V_GrTaskEnv.SvrCrit);
}
//--------------------------------------------------------------------
void*	GrTaskSvrGetByFcc(__u32 A_Fcc)
{
	// local -------------------
		void*	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//critical section
		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find empty slot
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
		{
			if(NULL != V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
			{
				if(A_Fcc == V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Fcc)
				{
					Tv_Result		=	V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj;
					break;
				}
			}
		}

		//critical section
		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void*	GrTaskSvrGetByThrdId(Def_GrPrcThrdId A_ThrdId)
{
	// local -------------------
		void*	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		//critical section
		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find empty slot
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
		{
			if(NULL != V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
			{
				if(A_ThrdId == V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].ThrdId)
				{
					Tv_Result		=	V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj;
					break;
				}
			}
		}

		//critical section
		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrTaskSvrStepDoThis(void)
{
	// local -------------------
		BOOL8		Tv_Result;
		Def_GrPrcThrdId	Tv_ThrdId;
		Cls_GrTaskSvr*	Tv_ObjTask;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		Tv_ThrdId		=	GrPrcThrdGetCurId();
		Tv_ObjTask	=	(Cls_GrTaskSvr*)GrTaskSvrFindByThreadId(Tv_ThrdId);
		if(NULL != Tv_ObjTask)
		{
			Tv_Result	=	Tv_ObjTask->DoStep();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	GrTaskSvrMsgCmdAlloc(void)
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_ByteIdx;
		__u32		Tv_BitIdx;
		__u8		Tv_BitMask;
	// code --------------------
		//init
		Tv_Result		=	0;

		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find
		for(Tv_ByteIdx=0; Tv_ByteIdx < E_GrTaskSvrMsgCliCmdTblSize; Tv_ByteIdx++)
		{
			if(0xFF != V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx])
			{
				//find bit
				Tv_BitMask	=	1;
				for(Tv_BitIdx=0; Tv_BitIdx < 8; Tv_BitIdx++)
				{
					//check found
					if(0 == (Tv_BitMask & V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx]))
					{
						//allocate
						V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx]	=	V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx] | Tv_BitMask;
						Tv_Result	=	E_GrTaskMsgCmdAlloc + (Tv_ByteIdx * 8) + Tv_BitIdx;
						break;
					}
					//next
					Tv_BitMask	=	Tv_BitMask << 1;
				}
			}
			//check found
			if(0 != Tv_Result)
			{
				break;
			}
		}

		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrTaskSvrMsgCmdFree(__u32 A_Cmd)
{
	// local -------------------
		__u32		Tv_ByteIdx;
		__u32		Tv_BitIdx;
		__u8		Tv_BitMask;
	// code --------------------
		//check able range
		if((E_GrTaskMsgCmdAlloc <= A_Cmd) && ((E_GrTaskMsgCmdAlloc + E_GrTaskSvrMaxMsgCliCmdCnt) > A_Cmd))
		{
			//critical section enter
			GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

			//release
			Tv_BitIdx		=	A_Cmd - E_GrTaskMsgCmdAlloc;
			Tv_ByteIdx	=	Tv_BitIdx >> 3;
			Tv_BitIdx		=	Tv_BitIdx & 0x7;
			Tv_BitMask	=	1 << Tv_BitIdx;
			V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx]	=	V_GrTaskEnv.CmdAlocTbl[Tv_ByteIdx] & ~Tv_BitMask;

			//crit leave
			GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);
		}
}
//--------------------------------------------------------------------
void	GrTaskSvrMsgBroadCast(Ptr_GrTaskMsgHd A_PtrMsg, __u32 A_Size, BOOL8 A_IsFast)
{
	// local -------------------
		__u32		Tv_WkIdx;
	// code --------------------
		//crit
		GrPrcCritEnter(&V_GrTaskEnv.SvrCrit);

		//find exist item
		for(Tv_WkIdx=0; Tv_WkIdx < E_GrTaskSvrMaxRegCnt; Tv_WkIdx++)
		{
			if(NULL != V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)
			{
				((Cls_GrTaskSvr*)V_GrTaskEnv.SvrRegTbl[Tv_WkIdx].Obj)->MsgPost(A_PtrMsg, A_Size, A_IsFast);
			}
		}

		//crit
		GrPrcCritLeave(&V_GrTaskEnv.SvrCrit);
}
//--------------------------------------------------------------------
void	GrTaskSvrMsgSysErrNotify(__u32 A_ErrCode, __u32 A_ParaD1, __u32 A_ParaD2)
{
	// local -------------------
		St_GrTaskMsgSysErr	Tv_Msg;
	// code --------------------
		Tv_Msg.MsgHd.Cmd	=	E_GrTaskMsgSysErr;
		Tv_Msg.ErrCode		=	A_ErrCode;
		Tv_Msg.ParaD1			=	A_ParaD1;
		Tv_Msg.ParaD2			=	A_ParaD2;
		GrTaskSvrMsgBroadCast((Ptr_GrTaskMsgHd)&Tv_Msg, sizeof(Tv_Msg));
}
//--------------------------------------------------------------------
__s32	GrTaskSvrGetUnstable(__u32* A_PtrRtFcc)
{
	// local -------------------
		__s32	Tv_Result;
		__u32	Tv_SvrIdx;
	// code --------------------
		Tv_Result	=	-1;

		for(Tv_SvrIdx = 0; Tv_SvrIdx < E_GrTaskSvrMaxRegCnt; Tv_SvrIdx++)
		{
			if(NULL != V_GrTaskEnv.SvrRegTbl[Tv_SvrIdx].Obj)
			{
				if(V_GrTaskEnv.SvrRegTbl[Tv_SvrIdx].PrgsCnt == V_GrTaskEnv.SvrRegTbl[Tv_SvrIdx].PrgsChk)
				{
					// found
					Tv_Result		=	(__s32)Tv_SvrIdx;
					*A_PtrRtFcc	=	V_GrTaskEnv.SvrRegTbl[Tv_SvrIdx].Fcc;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
