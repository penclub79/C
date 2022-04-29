/*
	Platform 5 develop manager

*/

//--------------------------------------------------------------------
// uses

#include <P5/Dvl/GrP5DvlMng.h>

#include <P5/GrP5Base.h>
#include <P5/Dvl/GrP5DvlBase.h>

#include <FontV3/GrFontV3DrawBase.h>
#include <FontV3/Dib2/GrFontV3Dib2Base.h>

#include <P5/GrP5SkinBase.h>
#include <P5/GrP5ConBase.h>

#include <GrStrTool.h>
#include <GrImgBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrGdibTool.h>
#include <GrFileCtrl.h>
#include <GrFileTool.h>

#include <GrLayerFile.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5Env	V_GrP5Env;
extern	St_GrP5DvlEnv	V_GrP5DvlEnv;
extern	St_GrP5SkinCfg	V_GrP5SkinCfg;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5DvlMng::Cls_GrP5DvlMng(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrP5Mng(A_TaskSvr)
{
	// local -------------------
	// code --------------------

		// init
		DvlnitPrj();
		DvlInitText();

		// init override
		m_PtrUiInfoBasic		=	&V_GrP5DvlEnv.PrjCtrl.UiInfoBasic;
		m_PtrVkbdCfg				=	&V_GrP5DvlEnv.PrjCtrl.VkbdCfg;
}
//--------------------------------------------------------------------
Cls_GrP5DvlMng::~Cls_GrP5DvlMng(void)
{
		DvlScptFreeAll();
		DvlImageFreeAll();
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlnitPrj(void)
{
	// local -------------------
	// code --------------------

		// global DVL variable reset
		V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScpt	=	E_GrP5ScriptFcc;
		V_GrP5DvlEnv.PrjCtrl.UiInfoBasic.FccScr		=	E_GrFccARGB8888;

		V_GrP5DvlEnv.PrjCtrl.UiTest.ResX					=	E_GrP5DvlTestEnvDfltResX;
		V_GrP5DvlEnv.PrjCtrl.UiTest.ResY					=	E_GrP5DvlTestEnvDfltResY;


}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlScptFreeAll(void)
{
	// local -------------------
		__u32	Tv_Wkidx;
	// code --------------------
		// releasse window script
		for( Tv_Wkidx = 0; Tv_Wkidx < E_GrP5ScptWinMaxCnt; Tv_Wkidx++)
		{
			if(NULL != m_ScptWinTbl[Tv_Wkidx])
			{
				free(m_ScptWinTbl[Tv_Wkidx]);
				m_ScptWinTbl[Tv_Wkidx]	=	NULL;
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlWindowScriptCreate(__u16 A_ScptId, __u16 A_ClassId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptIdx;
		__u32	Tv_ScptSize;
		St_GrP5RelRect	Tv_RelRect;
	// code --------------------
		Tv_Result		=	FALSE;

		// check script id
		if ((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			Tv_ScptIdx	=	A_ScptId - E_GrP5ScptIdWinBase;
			// check exist
			if (NULL == m_ScptWinTbl[Tv_ScptIdx])
			{
				// default rect
				GrDumyZeroMem(&Tv_RelRect, sizeof(Tv_RelRect));
				Tv_RelRect.Right.Val	=	E_GrP5DvlMngDfltWinSizeX;
				Tv_RelRect.Bottom.Val	=	E_GrP5DvlMngDfltWinSizeY;
				// get script size
				Tv_ScptSize	=	GrP5DvlScptWindowBuild(NULL, A_ScptId, 0, &Tv_RelRect, A_ClassId);
				// allocate script
				if (0 != Tv_ScptSize)
				{
					m_ScptWinTbl[Tv_ScptIdx]	=	(Ptr_GrP5ScptObjHead)malloc(Tv_ScptSize);
					// build
					GrP5DvlScptWindowBuild(m_ScptWinTbl[Tv_ScptIdx], A_ScptId, 0, &Tv_RelRect, A_ClassId);
					// success
					Tv_Result		=	TRUE;
				}
				
			}
			else
			{
				DbgMsgPrint("Cls_GrP5DvlMng::DvlWindowCreate - already scprit (%d) data exist.\n", A_ScptId);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlWindowScriptDelete(__u16 A_ScptId)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptIdx;
	// code --------------------
		Tv_Result		=	FALSE;

		// check script id
		if((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
		{
			Tv_ScptIdx	=	A_ScptId - E_GrP5ScptIdWinBase;
			// check exist
			if(NULL != m_ScptWinTbl[Tv_ScptIdx])
			{
				// delete script id
				GrP5DvlScptSlaveIdFree(m_ScptWinTbl[Tv_ScptIdx], m_ScptWinTbl[Tv_ScptIdx]->TotalSize);

				// release buffer
				free(m_ScptWinTbl[Tv_ScptIdx]);
				m_ScptWinTbl[Tv_ScptIdx]	=	NULL;
				Tv_Result	=	TRUE;
			}
			else
			{
				DbgMsgPrint("Cls_GrP5DvlMng::DvlWindowScriptDelete - scprit (%d) data not exist.\n", A_ScptId);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlWindowScriptSet(__u16 A_ScptId, void* A_PtrScpt, __u32 A_ScptSize)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ScptIdx;
	// code --------------------
		Tv_Result		=	FALSE;

		// check size
		if(0 != A_ScptSize)
		{
			// check script id
			if((E_GrP5ScptIdWinBase <= A_ScptId) && ((E_GrP5ScptIdWinBase + E_GrP5ScptWinMaxCnt) > A_ScptId))
			{
				Tv_ScptIdx	=	A_ScptId - E_GrP5ScptIdWinBase;
				// check exist
				if(NULL != m_ScptWinTbl[Tv_ScptIdx])
				{
					free(m_ScptWinTbl[Tv_ScptIdx]);
					m_ScptWinTbl[Tv_ScptIdx]	=	NULL;
				}
				// reallocate
				m_ScptWinTbl[Tv_ScptIdx]	=	(Ptr_GrP5ScptObjHead)malloc(A_ScptSize);
				if(NULL != m_ScptWinTbl[Tv_ScptIdx])
				{
					// copy contents
					GrDumyCopyMem(m_ScptWinTbl[Tv_ScptIdx], A_PtrScpt, A_ScptSize);
					Tv_Result	=	TRUE;
				}

			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5DvlMng::DvlWindowScriptSet - bad scprit (%d) data size.\n", A_ScptId);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlFontScriptSet(__u8 A_FontId, void* A_PtrScpt, __u32 A_Size, BOOL8 A_IsAutoScptFree)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check font id
		if(E_GrP5ScptFontMaxCnt > A_FontId)
		{
			// release old font
			if(NULL != m_FontTbl[A_FontId])
			{
				delete	m_FontTbl[A_FontId];
				m_FontTbl[A_FontId]	=	NULL;
			}

			m_FontTbl[A_FontId]	=	GrFontV3DrawCreateByScript(A_PtrScpt, A_Size, A_IsAutoScptFree);
			if(NULL != m_FontTbl[A_FontId])
			{
				Tv_Result	=	TRUE;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlFontDelete(__u8 A_FontId)
{
	// local -------------------
	// code --------------------
		// check font id
		if(E_GrP5ScptFontMaxCnt <= A_FontId)
		{
			return	FALSE;
		}

		// check exist font
		if(NULL == m_FontTbl[A_FontId])
		{
			return	FALSE;
		}

		// release font
		delete	m_FontTbl[A_FontId];
		m_FontTbl[A_FontId]	=	NULL;

		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlFontScriptSave(__u8 A_FontId, WCHAR* A_StrFn)
{
	// local -------------------
		Cls_GrFontV3DrawBase*	Tv_Font;
		void*	Tv_PtrScpt;
		__u32	Tv_ScptSize;
	// code --------------------
		// check font id
		if(E_GrP5ScptFontMaxCnt <= A_FontId)
		{
			return	FALSE;
		}

		// check exist font
		if(NULL == m_FontTbl[A_FontId])
		{
			return	FALSE;
		}

		Tv_Font	=	m_FontTbl[A_FontId];

		// delete old file
		GrFileDelete(A_StrFn, TRUE);

		// get script
		Tv_PtrScpt	=	Tv_Font->ScriptGet(&Tv_ScptSize);

		if(Tv_ScptSize != GrFileCtrlWrite(A_StrFn, TRUE, Tv_PtrScpt, Tv_ScptSize, TRUE))
		{
			return	FALSE;
		}
		
		return	TRUE;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlImageSet(__u16 A_ImgId, Cls_GrGdib* A_ObjGdib)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ImgIdx;
		__u32	Tv_GdibSize;
		Ptr_GrGdib	Tv_PtrGdib;
	// code --------------------
		Tv_Result	=	FALSE;

		if((E_GrP5ImgIdBase <= A_ImgId) && ((E_GrP5ImgIdBase + E_GrP5ScptImgMaxCnt) > A_ImgId))
		{
			Tv_ImgIdx	=	A_ImgId - E_GrP5ImgIdBase;
			if(NULL != m_ImgScptTbl[Tv_ImgIdx])
			{
				free(m_ImgScptTbl[Tv_ImgIdx]);
				m_ImgScptTbl[Tv_ImgIdx]	=	NULL;
			}

			// create new gdib
			if(A_ObjGdib->IsPalleteUse())
			{
				Tv_GdibSize	=	GrGdibCalcSize(A_ObjGdib->GetDibPitch(), A_ObjGdib->GetResY(), A_ObjGdib->GetPixelBit());
			}
			else
			{
				Tv_GdibSize	=	GrGdibCalcSize(A_ObjGdib->GetDibPitch(), A_ObjGdib->GetResY());
			}
			// allocate buffer
			Tv_PtrGdib	=	(Ptr_GrGdib)malloc(Tv_GdibSize);
			if(NULL != Tv_PtrGdib)
			{
				A_ObjGdib->SaveToMem(Tv_PtrGdib);
				m_ImgScptTbl[Tv_ImgIdx]	=	Tv_PtrGdib;
			}

			delete	A_ObjGdib;
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlImageSet(__u16 A_ImgId, Ptr_GrGdib A_PtrGdib)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_ImgIdx;
	// code --------------------
		Tv_Result	=	FALSE;

		if ((E_GrP5ImgIdBase <= A_ImgId) && ((E_GrP5ImgIdBase + E_GrP5ScptImgMaxCnt) > A_ImgId))
		{
			Tv_ImgIdx	=	A_ImgId - E_GrP5ImgIdBase;
			if (NULL != m_ImgScptTbl[Tv_ImgIdx])
			{
				free(m_ImgScptTbl[Tv_ImgIdx]);
				m_ImgScptTbl[Tv_ImgIdx]	=	NULL;
			}
			m_ImgScptTbl[Tv_ImgIdx]	=	A_PtrGdib;

			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlInitText(void)
{
	// local -------------------
		__u8	Tv_LngIdx;
		__u16	Tv_TxtIdx;
	// code --------------------
		// dedicated text
		for ( Tv_LngIdx = 0; Tv_LngIdx < E_GrP5ScptLanguageMaxCnt; Tv_LngIdx++)
		{
			for ( Tv_TxtIdx = 0; Tv_TxtIdx < E_GrP5TxtDedicatedCnt; Tv_TxtIdx++)
			{
				m_TxtFixTbl[Tv_LngIdx][Tv_TxtIdx]	=	V_GrP5DvlEnv.PrjCtrl.StrTxtFixTbl[Tv_LngIdx][Tv_TxtIdx];
			}
		}

		// user text
		for (Tv_LngIdx = 0; Tv_LngIdx < E_GrP5ScptLanguageMaxCnt; Tv_LngIdx++)
		{
			for (Tv_TxtIdx = 0; Tv_TxtIdx < E_GrP5ScptTxtUserMaxCnt; Tv_TxtIdx++)
			{
				m_TxtUserTbl[Tv_LngIdx][Tv_TxtIdx]	=	V_GrP5DvlEnv.PrjCtrl.StrTxtUserTbl[Tv_LngIdx][Tv_TxtIdx];
			}
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjSaveFont(Cls_GrLayerFileSave* A_ObjLf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_FntIdx;
		__u32	Tv_ScptSize;
		void*	Tv_PtrScpt;
	// code --------------------
		Tv_Result	=	TRUE;

		for(Tv_FntIdx = 0; Tv_FntIdx < E_GrP5ScptFontMaxCnt; Tv_FntIdx++)
		{
			if(NULL != m_FontTbl[Tv_FntIdx])
			{
				Tv_PtrScpt	=	m_FontTbl[Tv_FntIdx]->ScriptGet(&Tv_ScptSize);
				if(NULL != Tv_PtrScpt)
				{
					// patch script size
					Tv_ScptSize	=	Tv_ScptSize + ((4 - (Tv_ScptSize & 3)) & 3);
					// write font
					if(E_GrErrNone != A_ObjLf->DataAdd(E_GrP5DataIdFont, Tv_PtrScpt, Tv_ScptSize, Tv_FntIdx, 0))
					{
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjSaveImage(Cls_GrLayerFileSave* A_ObjLf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WkIdx;
		__u16	Tv_ImgId;
		__u32	Tv_GdibSize;
	// code --------------------
		Tv_Result	=	TRUE;

		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptImgMaxCnt; Tv_WkIdx++)
		{
			if(NULL != m_ImgScptTbl[Tv_WkIdx])
			{
				Tv_ImgId	=	Tv_WkIdx + E_GrP5ImgIdBase;
				Tv_GdibSize	=	m_ImgScptTbl[Tv_WkIdx]->Size;
				Tv_GdibSize	=	Tv_GdibSize + ((4 - (3 & Tv_GdibSize)) & 3);
				if(E_GrErrNone != A_ObjLf->DataAdd(E_GrP5DataIdImage, m_ImgScptTbl[Tv_WkIdx], Tv_GdibSize, Tv_ImgId, 0))
				{
					Tv_Result	=	FALSE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjSaveUiScript(Cls_GrLayerFileSave* A_ObjLf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_WkIdx;
		__u16	Tv_ScptId;
	// code --------------------
		Tv_Result	=	TRUE;

		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptWinMaxCnt; Tv_WkIdx++)
		{
			if(NULL != m_ScptWinTbl[Tv_WkIdx])
			{
				Tv_ScptId		=	E_GrP5ScptIdWinBase + Tv_WkIdx;
				if(E_GrErrNone != A_ObjLf->DataAdd(E_GrP5DataIdWinScript, m_ScptWinTbl[Tv_WkIdx], m_ScptWinTbl[Tv_WkIdx]->TotalSize, Tv_ScptId, 0))
				{
					Tv_Result	=	FALSE;
					break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjSaveText(Cls_GrLayerFileSave* A_ObjLf)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_LngIdx;
		__u32	Tv_WkIdx;
		__u32	Tv_TxtId;
		__u32	Tv_Len;
	// code --------------------
		Tv_Result	=	TRUE;

		// dedicate text
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5TxtDedicatedCnt; Tv_WkIdx++)
		{
			Tv_TxtId	=	Tv_WkIdx + E_GrP5TxtIdFixBase;
			for( Tv_LngIdx = 0; Tv_LngIdx < E_GrP5ScptLanguageMaxCnt; Tv_LngIdx++)
			{
				Tv_Len	=	GrStrWlen(V_GrP5DvlEnv.PrjCtrl.StrTxtFixTbl[Tv_LngIdx][Tv_WkIdx]);
				if(0 != Tv_Len)
				{
					Tv_Len ++;	// add zero terminate
					if(0 != (1 & Tv_Len))
					{
						// align 4 byte
						Tv_Len ++;
					}
					if(E_GrErrNone != A_ObjLf->DataAdd(E_GrP5DataIdText, V_GrP5DvlEnv.PrjCtrl.StrTxtFixTbl[Tv_LngIdx][Tv_WkIdx], 
						(Tv_Len * 2), Tv_TxtId, Tv_LngIdx))
					{
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}

		// user text
		for(Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptTxtUserMaxCnt; Tv_WkIdx++)
		{
			Tv_TxtId	=	Tv_WkIdx + E_GrP5TxtIdUserBase;
			for(Tv_LngIdx = 0; Tv_LngIdx < E_GrP5ScptLanguageMaxCnt; Tv_LngIdx++)
			{
				Tv_Len	=	GrStrWlen(V_GrP5DvlEnv.PrjCtrl.StrTxtUserTbl[Tv_LngIdx][Tv_WkIdx]);
				if(0 != Tv_Len)
				{
					Tv_Len ++;	// add zero terminate
					if(0 != (1 & Tv_Len))
					{
						// align 4 byte
						Tv_Len ++;
					}
					if(E_GrErrNone != A_ObjLf->DataAdd(E_GrP5DataIdText, V_GrP5DvlEnv.PrjCtrl.StrTxtUserTbl[Tv_LngIdx][Tv_WkIdx],
						(Tv_Len * 2), Tv_TxtId, Tv_LngIdx))
					{
						Tv_Result	=	FALSE;
						break;
					}
				}
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjSave(WCHAR* A_StrFn, BOOL8 A_IsAddPrjInfo)
{
	// local -------------------
		BOOL8	Tv_Result;
		Cls_GrLayerFileSave*	Tv_Lf;
	// code --------------------
		// init
		Tv_Result	=	FALSE;
		// create object
		Tv_Lf	=	(Cls_GrLayerFileSave*)new Cls_GrLayerFileSave(TRUE);
		if(NULL != Tv_Lf)
		{
			m_PtrUiInfoBasic->FccScpt	=	E_GrP5ScriptFcc;
			if(E_GrErrNone == Tv_Lf->Open(A_StrFn, m_PtrUiInfoBasic, sizeof(St_GrP5ScptUiInfoBasic)))
			{
				while(TRUE)
				{
					if (A_IsAddPrjInfo)
					{
						// window info
						if (E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DvlDataIdWinInfo, V_GrP5DvlEnv.PrjCtrl.WinInfoTbl, sizeof(St_GrP5DvlWinInfo)* E_GrP5ScptWinMaxCnt, 0, 0))
						{
							break;
						}

						// slave id map
						if (E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DvlDataIdSlaveIdMap, V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap, E_GrP5DvlScptSlaveMapSize, 0, 0))
						{
							break;
						}

						// ui test environment
						if (E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DvlDataIdUiTest, &V_GrP5DvlEnv.PrjCtrl.UiTest, sizeof(St_GrP5DvlUiTestCtrl), 0, 0))
						{
							break;
						}

						// work environment
						if (E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DvlDataIdWorkEnv, &V_GrP5DvlEnv.PrjCtrl.WorkEnv, sizeof(St_GrP5DvlWorkEnv), 0, 0))
						{
							break;
						}

					}

					// skin configuration
					if (E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DataIdSkinCfg, &V_GrP5SkinCfg, sizeof(V_GrP5SkinCfg), 0, 0))
					{
						break;
					}

					// virtial keyboard configuration
					if(E_GrErrNone != Tv_Lf->DataAdd(E_GrP5DataIdVkbdCfg, &V_GrP5DvlEnv.PrjCtrl.VkbdCfg, sizeof(St_GrP5VkbdCfg), 0, 0))
					{
						break;
					}

					// ui script
					if (!DvlPrjSaveUiScript(Tv_Lf))
					{
						break;
					}

					// text write
					if(!DvlPrjSaveText(Tv_Lf))
					{
						break;
					}

					// image 
					if (!DvlPrjSaveImage(Tv_Lf))
					{
						break;
					}

					// font
					if(!DvlPrjSaveFont(Tv_Lf))
					{
						break;
					}

					// finish
					Tv_Result	=	TRUE;
					break;
				}
				
			}
			delete	Tv_Lf;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjLoadFont(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_FontId, __u32 A_Size)
{
	// local -------------------
		void*	Tv_PtrBuf;
		BOOL8	Tv_IsOk;
		__u32	Tv_PtcSize;
	// code --------------------
		Tv_IsOk		=	FALSE;
		Tv_PtcSize	=	A_Size + ((4 - (A_Size & 3)) & 3);
		Tv_PtrBuf	=	(void*)malloc(Tv_PtcSize);
		if(NULL != Tv_PtrBuf)
		{
			if(E_GrErrNone == A_ObjLf->LayerDataLoad(Tv_PtrBuf,A_Size))
			{
				Tv_IsOk	=	DvlFontScriptSet((__u8)A_FontId, Tv_PtrBuf, A_Size, TRUE);
			}

			if(!Tv_IsOk)
			{
				free(Tv_PtrBuf);
			}

		}
		else
		{
			DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadFont - buffer allocate fail\n");
			m_DvlIsLoadOk	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjLoadImage(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_ImgId, __u32 A_Size)
{
	// local -------------------
		Ptr_GrGdib	Tv_PtrGdib;
		BOOL8	Tv_IsOk;
	// code --------------------
		Tv_PtrGdib	=	(Ptr_GrGdib)malloc(A_Size);
		if(NULL != Tv_PtrGdib)
		{
			Tv_IsOk	=	FALSE;
			if(E_GrErrNone == A_ObjLf->LayerDataLoad(Tv_PtrGdib, A_Size))
			{
				// check GDIB
				if(E_GrGdibFcc == Tv_PtrGdib->FccGdib)
				{
					// assign
					Tv_IsOk	=	DvlImageSet((__u16)A_ImgId, Tv_PtrGdib);
				}
			}
			if(!Tv_IsOk)
			{
				free(Tv_PtrGdib);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadImage - buffer allocate fail\n");
			m_DvlIsLoadOk	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjLoadWinScript(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_ScptId, __u32 A_Size)
{
	// local -------------------
		void*	Tv_PtrBuf;
	// code --------------------
		Tv_PtrBuf	=	(void*)malloc(A_Size);
		if(NULL != Tv_PtrBuf)
		{
			if(E_GrErrNone == A_ObjLf->LayerDataLoad(Tv_PtrBuf, A_Size))
			{
				if(!DvlWindowScriptSet((__u16)A_ScptId, Tv_PtrBuf, A_Size))
				{
					DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadWinScript - Script %d regist fail\n", A_ScptId);
				}
			}

			free(Tv_PtrBuf);
		}
		else
		{
			DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadWinScript - buffer allocate fail\n");
			m_DvlIsLoadOk	=	FALSE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjLoadWinInfo(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_Size)
{
	// local -------------------
	// code --------------------
		A_ObjLf->LayerDataLoad(V_GrP5DvlEnv.PrjCtrl.WinInfoTbl, sizeof(St_GrP5DvlWinInfo)* E_GrP5ScptWinMaxCnt);
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjLoadText(Cls_GrLayerFileLoad* A_ObjLf, __u32 A_Size, __u32 A_TextId, __u32 A_Language)
{
	// local -------------------
		WCHAR*	Tv_StrTxt;
	// code --------------------
		// check language
		if(E_GrP5ScptLanguageMaxCnt > A_Language)
		{
			Tv_StrTxt	=	GrP5TextGet((__u16)A_TextId, (__u8)A_Language);
			if(NULL != Tv_StrTxt)
			{
				A_ObjLf->LayerDataLoad(Tv_StrTxt, ((E_GrP5DvlTextMaxLen + 1) * 2));
			}
			else
			{
				DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadText - bad text id (%d)\n", A_TextId);
				A_ObjLf->LayerDataSkip();
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoadText - bad language id (%d)\n", A_Language);
			A_ObjLf->LayerDataSkip();
		}
		
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlMng::DvlPrjLoad(WCHAR* A_StrFn)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u16	Tv_DataId;
		__u32	Tv_Size;
		__u32	Tv_Para0;
		__u32	Tv_Para1;
		Cls_GrLayerFileLoad*	Tv_Lf;
	// code --------------------
		Tv_Result		=	FALSE;

		// reset project
		DvlPrjReset();

		m_DvlIsLoadOk	=	TRUE;
		// create load class
		Tv_Lf	=	(Cls_GrLayerFileLoad*)new Cls_GrLayerFileLoad(TRUE);
		if(NULL != Tv_Lf)
		{
			if(E_GrErrNone == Tv_Lf->Open(A_StrFn, m_PtrUiInfoBasic, sizeof(St_GrP5ScptUiInfoBasic), &Tv_Size))
			{
				// load
				while(m_DvlIsLoadOk && (E_GrErrNone == Tv_Lf->LayerInfoGet(&Tv_DataId, &Tv_Size, &Tv_Para0, &Tv_Para1)))
				{
					// read data
					switch(Tv_DataId)
					{
						case E_GrP5DataIdFont:
							DvlPrjLoadFont(Tv_Lf, Tv_Para0,Tv_Size);
							break;
						case E_GrP5DataIdImage:
							DvlPrjLoadImage(Tv_Lf, Tv_Para0, Tv_Size);
							break;
						case E_GrP5DataIdWinScript:
							DvlPrjLoadWinScript(Tv_Lf, Tv_Para0, Tv_Size);
							break;
						case E_GrP5DvlDataIdWinInfo:
							DvlPrjLoadWinInfo(Tv_Lf, Tv_Size);
							break;
						case E_GrP5DvlDataIdSlaveIdMap:
							Tv_Lf->LayerDataLoad(V_GrP5DvlEnv.PrjCtrl.ScptSlvIdMap, E_GrP5DvlScptSlaveMapSize);
							break;
						case E_GrP5DvlDataIdUiTest:
							Tv_Lf->LayerDataLoad(&V_GrP5DvlEnv.PrjCtrl.UiTest, sizeof(St_GrP5DvlUiTestCtrl));
							break;
						case E_GrP5DataIdSkinCfg:
							Tv_Lf->LayerDataLoad(&V_GrP5SkinCfg, sizeof(V_GrP5SkinCfg));
							GrP5SkinSetByCfg();
							break;
						case E_GrP5DataIdText:
							DvlPrjLoadText(Tv_Lf, Tv_Size, Tv_Para0, Tv_Para1);
							break;
						case E_GrP5DvlDataIdWorkEnv:
							Tv_Lf->LayerDataLoad(&V_GrP5DvlEnv.PrjCtrl.WorkEnv, sizeof(St_GrP5DvlWorkEnv));
							break;
						case E_GrP5DataIdVkbdCfg:
							Tv_Lf->LayerDataLoad(&V_GrP5DvlEnv.PrjCtrl.VkbdCfg, sizeof(St_GrP5VkbdCfg));
							break;
						default:
							DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoad - unknown data id %d\n", Tv_DataId);
							// skip
							Tv_Lf->LayerDataSkip();
							break;
					}
				}

				Tv_Result	=	m_DvlIsLoadOk;

				if(Tv_Result)
				{
					DbgMsgPrint("Cls_GrP5DvlMng::DvlPrjLoad - load success.\n");
				}
			}
			delete	Tv_Lf;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlPrjReset(void)
{
	// local -------------------
	// code --------------------

		DvlScptFreeAll();
		DvlImageFreeAll();
		FontFreeAll();

		GrDumyZeroMem(&V_GrP5DvlEnv.PrjCtrl, sizeof(St_GrP5DvlPrjCtrl));

		DvlnitPrj();
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlMng::DvlImageFreeAll(void)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		for ( Tv_WkIdx = 0; Tv_WkIdx < E_GrP5ScptImgMaxCnt; Tv_WkIdx++)
		{
			if (NULL != m_ImgScptTbl[Tv_WkIdx])
			{
				free(m_ImgScptTbl[Tv_WkIdx]);
				m_ImgScptTbl[Tv_WkIdx]	=	NULL;
			}
		}
}
//--------------------------------------------------------------------
