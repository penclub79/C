

//====================================================================
// uses

#include <Win\MfcDock\GrMfcDockBase.h>
#include <Win\MfcDock\GrMfcDockMng.h>
#include <Win\MfcDock\GrMfcDockPaneBase.h>

#include <afxwinappex.h>

#include <GrDumyTool.h>
#include <GrBufTool.h>
#include <GrStrTool.h>

//====================================================================
//const


//====================================================================
//--------------------------------------------------------------------
//global var

St_GrMfcDockEnv	V_GrMfcDockEnv	={NULL,NULL,};

//--------------------------------------------------------------------
void	DbgMsgPrint(__in_z __format_string const char * A_StrFmt, ...)
{
	// local -------------------
		Def_StrLong		Tv_StrMsg;
		Def_WstrLong	Tv_StrUni;
		va_list		Tv_Ap;

		DWORD		Tv_Len;
		WCHAR*	Tv_StrQue;
	// code --------------------
		// check initialized
		if((NULL != V_GrMfcDockEnv.TaskMng) || (NULL != V_GrMfcDockEnv.ObjMng))
		{
			va_start(Tv_Ap, A_StrFmt);
			vsprintf(Tv_StrMsg, A_StrFmt, Tv_Ap);
			va_end(Tv_Ap);
			GrStrStrToWstr(Tv_StrUni, Tv_StrMsg);

			// check length
			Tv_Len = GrStrWlen(Tv_StrUni);
			// allocate queue
			GrPrcCritEnter(&V_GrMfcDockEnv.DmsgCrit);

			Tv_StrQue = (WCHAR*)GrBufVqGetInPtr((struct St_GrBufVarQue*)V_GrMfcDockEnv.DmsgQue, (Tv_Len + 1) * 2);
			if(NULL != Tv_StrQue)
			{
				GrStrWcopy(Tv_StrQue, Tv_StrUni);
				GrBufVqIn((struct St_GrBufVarQue*)V_GrMfcDockEnv.DmsgQue);
			}

			GrPrcCritLeave(&V_GrMfcDockEnv.DmsgCrit);
		}
}
//--------------------------------------------------------------------
Def_GrErrCode	GrMfcDockInit(Cls_GrTaskSvr* A_TaskSvr, __u32 A_Version)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		BOOL8	Tv_IsVerOk;
		CRegKey	Tv_Rgst;
		DWORD	Tv_ValVer;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		// check ui version
		if(0 != A_Version)
		{
			// init
			Tv_IsVerOk	=	TRUE;
			// get registry key
			if(ERROR_SUCCESS == Tv_Rgst.Open(AfxGetApp()->GetAppRegistryKey(), NULL))
			{
				// get ui version
				Tv_ValVer	=	0;
				if(ERROR_SUCCESS == Tv_Rgst.QueryDWORDValue(L"GrMfcDockVer", Tv_ValVer))
				{
					// check
					if((__u32)Tv_ValVer != A_Version)
					{
						Tv_IsVerOk	=	FALSE;
					}
				}
				else
				{
					// UI version update
					Tv_Rgst.SetDWORDValue(L"GrMfcDockVer", (DWORD)A_Version);
				}

				if(!Tv_IsVerOk)
				{
					// reset ui work space
					Tv_Rgst.DeleteSubKey(L"Workspace");
					// UI version update
					Tv_Rgst.SetDWORDValue(L"GrMfcDockVer", (DWORD)A_Version);
				}

				Tv_Rgst.Close();
			}

		}

		if(NULL == V_GrMfcDockEnv.TaskMng)
		{
			// reset
			GrDumyZeroMem(&V_GrMfcDockEnv, sizeof(V_GrMfcDockEnv));

			V_GrMfcDockEnv.TaskMng	=	A_TaskSvr;

			V_GrMfcDockEnv.ObjMng		=	(Cls_GrTaskCli*) new Cls_GrMfcDockMng(V_GrMfcDockEnv.TaskMng);
			if(NULL != V_GrMfcDockEnv.ObjMng)
			{
				Tv_Result	=	E_GrErrNone;
			}

			// init
			if(E_GrErrNone == Tv_Result)
			{
				// debug buffer 
				GrBufVqInit((Ptr_GrBufVarQue)V_GrMfcDockEnv.DmsgQue, E_GrMfcDockDbgMsgBufSize, 16);
				GrPrcCritInit(&V_GrMfcDockEnv.DmsgCrit);

			}

			// check faile
			if(E_GrErrNone != Tv_Result)
			{
				if(NULL != V_GrMfcDockEnv.ObjMng)
				{
					delete	V_GrMfcDockEnv.ObjMng;
					V_GrMfcDockEnv.ObjMng	=	NULL;
				}

				if(NULL != V_GrMfcDockEnv.TaskMng)
				{
					delete	V_GrMfcDockEnv.TaskMng;
					V_GrMfcDockEnv.TaskMng	=	NULL;
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrMfcDockFinish(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if((NULL != V_GrMfcDockEnv.TaskMng) || (NULL != V_GrMfcDockEnv.ObjMng))
		{
			// release critcal section
			GrPrcCritDel(&V_GrMfcDockEnv.DmsgCrit);
			
			// release object
			if(NULL != V_GrMfcDockEnv.ObjMng)
			{
				delete	V_GrMfcDockEnv.ObjMng;
				V_GrMfcDockEnv.ObjMng	=	NULL;
			}

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrMfcDockTaskRun(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if((NULL != V_GrMfcDockEnv.TaskMng) && (NULL != V_GrMfcDockEnv.ObjMng))
		{
			V_GrMfcDockEnv.TaskMng->Run();
			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
CMFCToolBarButton*	GrMfcDockToolbarBtnGetByCmd(CMFCToolBar* A_ObjToolbar, UINT A_Cmd)
{
	// local -------------------
		CMFCToolBarButton*	Tv_Result;
		int	Tv_Cnt;
		int	Tv_WkIdx;
	// code --------------------
		Tv_Result	=	NULL;

		Tv_Cnt	=	A_ObjToolbar->GetCount();
		if(0 != Tv_Cnt)
		{
			for(Tv_WkIdx=0; Tv_WkIdx < Tv_Cnt; Tv_WkIdx++)
			{
				if(A_Cmd == A_ObjToolbar->GetItemID(Tv_WkIdx))
				{
					Tv_Result	=	A_ObjToolbar->GetButton(Tv_WkIdx);
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	GrMfcDockPaneRegist(CDockablePane* A_Pane)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBufFull;

		if(E_GrMfcDockPaneMaxCnt > V_GrMfcDockEnv.PaneCnt)
		{
			
			// insert
			V_GrMfcDockEnv.PaneTbl[V_GrMfcDockEnv.PaneCnt]	=	A_Pane;

			V_GrMfcDockEnv.PaneCnt ++;

			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
CDockablePane*	GrMfcDockPaneGetByPaneId(__u32 A_PaneId)
{
	// local -------------------
		CDockablePane*	Tv_Result;
		__u32	Tv_PneIdx;
	// code --------------------
		// init
		Tv_Result	=	NULL;

		if(0 != V_GrMfcDockEnv.PaneCnt)
		{
			for(Tv_PneIdx = 0; Tv_PneIdx < V_GrMfcDockEnv.PaneCnt; Tv_PneIdx++)
			{
				if( A_PaneId == ((Cls_GrMfcDockPaneBase*)V_GrMfcDockEnv.PaneTbl[Tv_PneIdx])->PaneIdGet() )
				{
					Tv_Result	=	V_GrMfcDockEnv.PaneTbl[Tv_PneIdx];
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrMfcDockPaneMsgPost(__u32 A_OwnerId, __u32 A_TargetId, __u32 A_Cmd, __u32 A_Para0, __u32 A_Para1, __u32 A_Para2)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(NULL != V_GrMfcDockEnv.ObjMng)
		{
			Tv_Result	=	((Cls_GrMfcDockMng*)V_GrMfcDockEnv.ObjMng)->PaneMsgPost(A_OwnerId, A_TargetId, A_Cmd, A_Para0, A_Para1, A_Para2);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	GrMfcDockReqQuit(void)
{
	// local -------------------
	// code --------------------
		if(NULL != V_GrMfcDockEnv.MainFrm)
		{
			PostMessage(V_GrMfcDockEnv.MainFrm->m_hWnd, WM_CLOSE, 0, 0);
		}
}
//--------------------------------------------------------------------
BOOL8	GrMfcDockIsReady(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result		=	FALSE;

		if (E_GrMfcDockPaneStatReady == V_GrMfcDockEnv.Stat)
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

