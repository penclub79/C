/*
 archive tool

 */

//====================================================================
// uses
#include <GrLayerFile.h>

#include <GrDebug.h>

#include <GrStrTool.h>
#include <GrTimeTool.h>
#include <GrDumyTool.h>
#include <GrFileTool.h>


//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrLayerFileSave::Cls_GrLayerFileSave(BOOL8 A_IsNoEmu)
{
	// local -------------------
	// code --------------------
		m_IsNoEmu		=	A_IsNoEmu;

		GrDumyZeroMem(&m_FlHead, sizeof(m_FlHead));
		m_FlHead.Fcc			=	E_GrLayerDataFcc;

		m_Fc				=	NULL;
}
//--------------------------------------------------------------------
Cls_GrLayerFileSave::~Cls_GrLayerFileSave()
{
	// local -------------------
		St_GrLayerDataLayerHead	Tv_LpHead;
  // code --------------------
		if(NULL != m_Fc)
		{
			// save end mark
			Tv_LpHead.SeparatorId	=	E_GrLayerDataSeparatorId;
			Tv_LpHead.DataId			=	E_GrLayerDataIdEnd;
			Tv_LpHead.Size				=	0;
			Tv_LpHead.Para0				=	0;
			Tv_LpHead.Para1				=	0;

			m_Fc->Write(&Tv_LpHead, sizeof(Tv_LpHead));

			// release
			delete	m_Fc;
			m_Fc		=	NULL;
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileSave::Open(char* A_StrFn, void* A_PtrUserInfo, __u32 A_UserInfoSize)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;
		// delete old file
		GrFileDelete(A_StrFn, m_IsNoEmu);

		m_Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFn, TRUE, TRUE, FALSE, m_IsNoEmu);
		if(NULL != m_Fc)
		{
			if(m_Fc->IsOpened())
			{
				// init header
				m_FlHead.UserInfoSize	=	A_UserInfoSize;
				// write file header
				if(sizeof(m_FlHead) == m_Fc->Write(&m_FlHead, sizeof(m_FlHead)))
				{
					Tv_Result	=	E_GrErrNone;
					if(0 != A_UserInfoSize)
					{
						Tv_Result	=	E_GrErrBadIo;
						if(A_UserInfoSize == m_Fc->Write(A_PtrUserInfo, A_UserInfoSize))
						{
							Tv_Result	=	E_GrErrNone;
						}
					}
				}
			}

			if(E_GrErrNone != Tv_Result)
			{
				delete	m_Fc;
				m_Fc		=	NULL;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileSave::Open(WCHAR* A_StrFn, void* A_PtrUserInfo, __u32 A_UserInfoSize)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;
		// delete old file
		GrFileDelete(A_StrFn, m_IsNoEmu);

		m_Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFn, TRUE, TRUE, FALSE, m_IsNoEmu);
		if(NULL != m_Fc)
		{
			if(m_Fc->IsOpened())
			{
				// init header
				m_FlHead.UserInfoSize	=	A_UserInfoSize;
				// write file header
				if(sizeof(m_FlHead) == m_Fc->Write(&m_FlHead, sizeof(m_FlHead)))
				{
					Tv_Result	=	E_GrErrNone;
					if(0 != A_UserInfoSize)
					{
						Tv_Result	=	E_GrErrBadIo;
						if(A_UserInfoSize == m_Fc->Write(A_PtrUserInfo, A_UserInfoSize))
						{
							Tv_Result	=	E_GrErrNone;
						}
					}
				}
			}

			if(E_GrErrNone != Tv_Result)
			{
				delete	m_Fc;
				m_Fc		=	NULL;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileSave::DataAdd(__u16 A_DataId, void* A_PtrBuf, __u32 A_Size, __u32 A_Para0, __u32 A_Para1)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrLayerDataLayerHead	Tv_LpHead;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if(NULL != m_Fc)
		{
			Tv_Result	=	E_GrErrBadIo;

			// build head
			Tv_LpHead.SeparatorId		=	E_GrLayerDataSeparatorId;
			Tv_LpHead.DataId				=	A_DataId;
			Tv_LpHead.Size					=	A_Size;
			Tv_LpHead.Para0					=	A_Para0;
			Tv_LpHead.Para1					=	A_Para1;

			// write head
			if(sizeof(St_GrLayerDataLayerHead) == m_Fc->Write(&Tv_LpHead, sizeof(Tv_LpHead)))
			{
				// write contents
				if(A_Size == m_Fc->Write(A_PtrBuf,A_Size))
				{
					Tv_Result	=	E_GrErrNone;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Cls_GrLayerFileLoad::Cls_GrLayerFileLoad(BOOL8 A_IsNoEmu)
{
	// local -------------------
	// code --------------------
		m_IsNoEmu			=	A_IsNoEmu;
		m_IsExistData	=	FALSE;

		m_Fc				=	NULL;

}
//--------------------------------------------------------------------
Cls_GrLayerFileLoad::~Cls_GrLayerFileLoad()
{
	// local -------------------
	// code --------------------
		if(NULL != m_Fc)
		{
			delete	m_Fc;
			m_Fc		=	NULL;
		}

}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::Open(char* A_StrFn, void* A_PtrUserInfo, __u32 A_MaxSize, __u32*A_PtrRtSize)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrLayerDataHead	Tv_FlHd;
		__u32	Tv_UinfoSize;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Result			=	E_GrErrInitFail;
		Tv_UinfoSize	=	0;

		m_Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFn, FALSE, FALSE, FALSE, m_IsNoEmu);
		if(NULL != m_Fc)
		{
			if(m_Fc->IsOpened())
			{
				// read file header
				if(sizeof(Tv_FlHd) == m_Fc->Read(&Tv_FlHd, sizeof(Tv_FlHd)))
				{
					// check file header
					Tv_Result	=	E_GrErrBadParam;
					if(E_GrLayerDataFcc == Tv_FlHd.Fcc)
					{
						Tv_IsAble	=	TRUE;
						// read user info
						Tv_UinfoSize	=	Tv_FlHd.UserInfoSize;
						if(0 != Tv_UinfoSize)
						{
							if(Tv_UinfoSize >= A_MaxSize)
							{
								Tv_UinfoSize	=	A_MaxSize;
							}
							if(Tv_UinfoSize != m_Fc->Read(A_PtrUserInfo, Tv_UinfoSize))
							{
								Tv_IsAble			=	FALSE;
								Tv_UinfoSize	=	0;
							}
						}

						if(Tv_IsAble)
						{
							m_IsExistData	=	TRUE;
							Tv_Result	=	LcLayerHeadRead();
						}
					}
				}
			}

			if(E_GrErrNone != Tv_Result)
			{
				delete	m_Fc;
				m_Fc		=	NULL;
			}
		}

		*A_PtrRtSize	=	Tv_UinfoSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::Open(WCHAR* A_StrFn, void* A_PtrUserInfo, __u32 A_MaxSize, __u32*A_PtrRtSize)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		St_GrLayerDataHead	Tv_FlHd;
		__u32	Tv_UinfoSize;
		BOOL8	Tv_IsAble;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		Tv_UinfoSize	=	0;

		m_Fc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFn, FALSE, FALSE, FALSE, m_IsNoEmu);
		if(NULL != m_Fc)
		{
			if(m_Fc->IsOpened())
			{
				// read file header
				if(sizeof(Tv_FlHd) == m_Fc->Read(&Tv_FlHd, sizeof(Tv_FlHd)))
				{
					// check file header
					Tv_Result	=	E_GrErrBadParam;
					if(E_GrLayerDataFcc == Tv_FlHd.Fcc)
					{
						Tv_IsAble	=	TRUE;
						// read user info
						Tv_UinfoSize	=	Tv_FlHd.UserInfoSize;
						if(0 != Tv_UinfoSize)
						{
							if(Tv_UinfoSize >= A_MaxSize)
							{
								Tv_UinfoSize	=	A_MaxSize;
							}
							if(Tv_UinfoSize == m_Fc->Read(A_PtrUserInfo, Tv_UinfoSize))
							{
								if(A_MaxSize < Tv_FlHd.UserInfoSize)
								{
									if(E_GrErrNone != LcSkip(Tv_FlHd.UserInfoSize - A_MaxSize))
									{
										Tv_IsAble			=	FALSE;
									}
								}
							}
							else
							{
								Tv_IsAble			=	FALSE;
								Tv_UinfoSize	=	0;
							}
						}

						if(Tv_IsAble)
						{
							m_IsExistData	=	TRUE;
							Tv_Result	=	LcLayerHeadRead();
						}

					}
				}
			}

			if(E_GrErrNone != Tv_Result)
			{
				delete	m_Fc;
				m_Fc		=	NULL;
			}
		}

		*A_PtrRtSize	=	Tv_UinfoSize;

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::LcLayerHeadRead(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrNotFound;

		if(m_IsExistData)
		{
			if(sizeof(m_LyrHead) == m_Fc->Read(&m_LyrHead,sizeof(m_LyrHead)))
			{
				// check id
				if(E_GrLayerDataSeparatorId == m_LyrHead.SeparatorId)
				{
					if(E_GrLayerDataIdEnd != m_LyrHead.DataId)
					{
						// success
						Tv_Result	=	E_GrErrNone;
					}
					else
					{
						m_IsExistData	=	FALSE;
					}
				}
			}

			if(E_GrErrNone != Tv_Result)
			{
				m_IsExistData	=	FALSE;
			}

		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::LayerInfoGet(__u16* A_PtrRtDataId, __u32* A_PtrRtSize, __u32* A_PtrRtPara0, __u32* A_PtrRtPara1)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrNotFound;

		if(m_IsExistData)
		{
			*A_PtrRtDataId	=	m_LyrHead.DataId;
			*A_PtrRtSize		=	m_LyrHead.Size;
			*A_PtrRtPara0		=	m_LyrHead.Para0;
			*A_PtrRtPara1		=	m_LyrHead.Para1;

			Tv_Result				=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::LayerDataLoad(void* A_PtrBuf, __u32 A_BufSize)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
		__u32	Tv_RdSize;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if(NULL != m_Fc)
		{
			Tv_Result	=	E_GrErrNotFound;

			if(m_IsExistData)
			{
				if(0 != m_LyrHead.Size)
				{
					Tv_RdSize	=	A_BufSize;
					if(Tv_RdSize >= m_LyrHead.Size)
					{
						Tv_RdSize	=	m_LyrHead.Size;
					}
					if(Tv_RdSize == m_Fc->Read(A_PtrBuf, Tv_RdSize))
					{
						if(Tv_RdSize < m_LyrHead.Size)
						{
							Tv_Result	=	LcSkip(m_LyrHead.Size - Tv_RdSize);
						}
					}
					else
					{
						Tv_Result		=	E_GrErrBadIo;
					}

				}
				if(E_GrErrBadIo != Tv_Result)
				{
					Tv_Result		=	E_GrErrNone;

					// read next head
					LcLayerHeadRead();
				}
				else
				{
					m_IsExistData	=	FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::LcSkip(__u32 A_Size)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrBadIo;

		if(0 <= m_Fc->Seek((__s64)A_Size, E_GrFileCtrlPosNow))
		{
			Tv_Result	=	E_GrErrNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
Def_GrErrCode	Cls_GrLayerFileLoad::LayerDataSkip(void)
{
	// local -------------------
		Def_GrErrCode	Tv_Result;
	// code --------------------
		Tv_Result	=	E_GrErrInitFail;

		if(NULL != m_Fc)
		{
			Tv_Result	=	E_GrErrNotFound;

			if(m_IsExistData)
			{
				if(0 != m_LyrHead.Size)
				{
					Tv_Result	=	LcSkip(m_LyrHead.Size);
				}
				if(E_GrErrBadIo != Tv_Result)
				{
					Tv_Result		=	E_GrErrNone;

					// read next head
					LcLayerHeadRead();
				}
				else
				{
					m_IsExistData	=	FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
