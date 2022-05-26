/*
 archive tool

 */

//====================================================================
// uses
#include <GrArchiv.h>

#include <GrDebug.h>

#include <GrStrTool.h>
#include <GrTimeTool.h>
#include <GrDumyTool.h>

#include <GrFileTool.h>

#ifdef	LINUX_APP

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/sysmacros.h>
#endif

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var


//====================================================================

//--------------------------------------------------------------------
Cls_GrArchDec::Cls_GrArchDec(BOOL8 A_IsNoEmu)
{

		m_InType		=	E_GrArchivInputTypeNone;
		m_PtrIn			=	NULL;
		m_InSize		=	0;
		m_FcIn			=	NULL;

		m_FcFile		=	NULL;

		m_RunMode		=	E_GrArchivRunModeChunkHead;

		GrDumyZeroMem(m_SectTbl, sizeof(m_SectTbl));
		GrDumyZeroMem(&m_ChukHd, sizeof(m_ChukHd));
		
#ifndef LINUX_APP
		m_IsNoEmu		=	A_IsNoEmu;
#endif

}
//--------------------------------------------------------------------
Cls_GrArchDec::~Cls_GrArchDec()
{
	// local -------------------

  // code --------------------

	if (NULL != m_FcIn)
	{
		delete	m_FcIn;
	}

	if (NULL != m_FcFile)
	{
		delete	m_FcFile;
	}

}
//--------------------------------------------------------------------
__u32	GrArchivCalChksum(__u32 A_PrevChksum, void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		__u32		Tv_Result;
		__u32		Tv_WkCnt;
		__u32*	Tv_PtrSr32;
		__u32		Tv_TmpVal;
	// code --------------------
		Tv_Result		=	A_PrevChksum;

		Tv_WkCnt		=	A_Size >> 2;
		Tv_PtrSr32	=	(__u32*)A_PtrBuf;
		while (0 != Tv_WkCnt)
		{
			Tv_Result	=	Tv_Result + (*Tv_PtrSr32);

			// next
			Tv_WkCnt --;
			Tv_PtrSr32 ++;
		}

		if (0 != (A_Size & 3))
		{
			Tv_WkCnt	=	A_Size & 3;
			Tv_TmpVal	=	0;
			GrDumyCopyMem(&Tv_TmpVal, Tv_PtrSr32, Tv_WkCnt);

			Tv_Result	=	Tv_Result + Tv_TmpVal;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrArchivGetFileChksum(char* A_StrFn, __u32 A_PayloadSize, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtChksum, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ChkSum;
		__u32	Tv_BufSize;
		__u32	Tv_ReadSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_ChkSum		=	0;

		Tv_BufSize	=	A_WorkBufSize & 0xFFFFFFFC;
		if (0 != Tv_BufSize)
		{
			if ( 0 < A_PayloadSize )
			{
				Cls_GrFileCtrl	Tv_Fctl(A_StrFn, FALSE, FALSE, FALSE, A_IsNoEmu);
				if (Tv_Fctl.IsOpened())
				{
					if (sizeof(St_GrArchivHeader) == Tv_Fctl.Seek(sizeof(St_GrArchivHeader)))
					{
						Tv_Result	=	TRUE;
						Tv_Remain	=	A_PayloadSize;
						while (0 != Tv_Remain)
						{
							Tv_ReadSize	=	Tv_Remain;
							if (Tv_BufSize <= Tv_ReadSize)
							{
								Tv_ReadSize	=	Tv_BufSize;
							}

							if (Tv_ReadSize != Tv_Fctl.Read(A_PtrWorkBuf, Tv_ReadSize))
							{
								Tv_Result	=	FALSE;
								break;
							}
							
							Tv_ChkSum	=	GrArchivCalChksum(Tv_ChkSum, A_PtrWorkBuf, Tv_ReadSize);

							// next
							Tv_Remain	=	Tv_Remain - Tv_ReadSize;
						}

					}
				}

			}
		}

		*A_PtrRtChksum	=	Tv_ChkSum;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrArchivGetFileChksum(WCHAR* A_StrFn, __u32 A_PayloadSize, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtChksum, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_ChkSum;
		__u32	Tv_BufSize;
		__u32	Tv_ReadSize;
		__u32	Tv_Remain;
	// code --------------------
		Tv_Result		=	FALSE;
		Tv_ChkSum		=	0;

		Tv_BufSize	=	A_WorkBufSize & 0xFFFFFFFC;
		if (0 != Tv_BufSize)
		{
			if (0 < A_PayloadSize)
			{
				Cls_GrFileCtrl	Tv_Fctl(A_StrFn, FALSE, FALSE, FALSE, A_IsNoEmu);
				if (Tv_Fctl.IsOpened())
				{
					if (sizeof(St_GrArchivHeader) == Tv_Fctl.Seek(sizeof(St_GrArchivHeader)))
					{
						Tv_Result	=	TRUE;
						Tv_Remain	=	A_PayloadSize;
						while (0 != Tv_Remain)
						{
							Tv_ReadSize	=	Tv_Remain;
							if (Tv_BufSize <= Tv_ReadSize)
							{
								Tv_ReadSize	=	Tv_BufSize;
							}

							if (Tv_ReadSize != Tv_Fctl.Read(A_PtrWorkBuf, Tv_ReadSize))
							{
								Tv_Result	=	FALSE;
								break;
							}

							Tv_ChkSum	=	GrArchivCalChksum(Tv_ChkSum, A_PtrWorkBuf, Tv_ReadSize);

							// next
							Tv_Remain	=	Tv_Remain - Tv_ReadSize;
						}

					}
				}

			}
		}

		*A_PtrRtChksum	=	Tv_ChkSum;
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrArchivChkFile(char* A_StrFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrArchivHeader	Tv_Header;
		__u32	Tv_FlSize;
		__u32	Tv_ChkSum;
	// code --------------------
		Tv_Result	=	FALSE;

		if ( sizeof(Tv_Header) == GrFileCtrlRead(A_StrFn, &Tv_Header,sizeof(Tv_Header), TRUE) )
		{
			// check header
			if (E_GrArchivHeadFcc == Tv_Header.Fcc)
			{
				if (E_GrArchivVer == Tv_Header.Ver)
				{
					Tv_FlSize		=	(__u32)GrFileGetSize(A_StrFn, TRUE);
					if ((Tv_Header.PayloadSize + sizeof(St_GrArchivHeader)) <= Tv_FlSize)
					{
						if (GrArchivGetFileChksum(A_StrFn, Tv_Header.PayloadSize,A_PtrWorkBuf,A_WorkBufSize,&Tv_ChkSum,A_IsNoEmu ))
						{
							if (Tv_ChkSum == Tv_Header.PayloadChk)
							{
								*A_PtrRtId	=	Tv_Header.DataId;
								GrDumyCopyMem(A_PtrRtUserData, Tv_Header.UserData, E_GrArchivUserDataSize);
								Tv_Result	=	TRUE;
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrArchivChkFile(WCHAR* A_StrFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrArchivHeader	Tv_Header;
		__u32	Tv_FlSize;
		__u32	Tv_ChkSum;
	// code --------------------
		Tv_Result	=	FALSE;

		if (sizeof(Tv_Header) == GrFileCtrlRead(A_StrFn, &Tv_Header, sizeof(Tv_Header), TRUE))
		{
			// check header
			if (E_GrArchivHeadFcc == Tv_Header.Fcc)
			{
				if (E_GrArchivVer == Tv_Header.Ver)
				{
					Tv_FlSize		=	(__u32)GrFileGetSize(A_StrFn, TRUE);
					if ((Tv_Header.PayloadSize + sizeof(St_GrArchivHeader)) <= Tv_FlSize)
					{
						if (GrArchivGetFileChksum(A_StrFn, Tv_Header.PayloadSize, A_PtrWorkBuf, A_WorkBufSize, &Tv_ChkSum, A_IsNoEmu))
						{
							if (Tv_ChkSum == Tv_Header.PayloadChk)
							{
								*A_PtrRtId	=	Tv_Header.DataId;
								GrDumyCopyMem(A_PtrRtUserData, Tv_Header.UserData, E_GrArchivUserDataSize);
								Tv_Result	=	TRUE;
							}
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::SectionDeviceSet(__u8 A_Section, char* A_StrName)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrArchivSectionMaxCnt > A_Section)
		{
			GrStrCopy(m_SectTbl[A_Section].StrDev, A_StrName, GrStrMaxLen(m_SectTbl[A_Section].StrDev));
			Tv_Result	=	TRUE;
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::SectionDeviceSet - section id (%d) out of range\n", A_Section);
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::OpenFromMemory(void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check already open
		if (E_GrArchivInputTypeNone != m_InType)
		{
			m_InType	=	E_GrArchivInputTypeMemory;
			m_PtrIn		=	A_PtrBuf;
			m_InSize	=	A_Size;

			m_Progress	=	0;
			m_RunMode		=	E_GrArchivRunModeChunkHead;

			Tv_Result	=	LcArchHeadCheck();
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::OpenFromMemory - already open\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::OpenFromFile(char* A_StrFn, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check already open
		if (E_GrArchivInputTypeNone == m_InType)
		{
			m_FcIn	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFn, FALSE, FALSE, FALSE, A_IsNoEmu);
			if (NULL != m_FcIn)
			{
				if (m_FcIn->IsOpened())
				{
					m_InType		=	E_GrArchivInputTypeFile;
					m_Progress	=	0;
					m_InSize		=	(__u32)m_FcIn->GetSize();

					m_RunMode		=	E_GrArchivRunModeChunkHead;

					m_FcIn->Seek(0);

					Tv_Result	=	LcArchHeadCheck();
				}
				else
				{
					DbgMsgPrint("Cls_GrArchDec::OpenFromFile - file (%s) open fail\n", A_StrFn);
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::OpenFromFile - file control create fail\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::OpenFromFile - already open\n");
		}

		if ((!Tv_Result) && (NULL != m_FcIn))
		{
			delete	m_FcIn;
			m_FcIn	=	NULL;

			m_InType	=	E_GrArchivInputTypeNone;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::LcInDataRead(void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check read able size
		if ((m_Progress + A_Size) <= m_InSize)
		{
			switch (m_InType)
			{
				case E_GrArchivInputTypeMemory:
					GrDumyCopyMem(A_PtrBuf, (void*)((__u32)m_PtrIn + m_Progress), A_Size);
					Tv_Result	=	TRUE;
					break;
				case E_GrArchivInputTypeFile:
					if (A_Size == m_FcIn->Read(A_PtrBuf,A_Size))
					{
						Tv_Result	=	TRUE;
					}
					break;
			}

			if (Tv_Result)
			{
				m_Progress	=	m_Progress + A_Size;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::LcArchHeadCheck(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrArchivHeader	Tv_ArchHd;
	// code --------------------
		Tv_Result	=	FALSE;

		if (LcInDataRead(&Tv_ArchHd, sizeof(Tv_ArchHd)))
		{
			// check fcc and version
			if ((E_GrArchivHeadFcc == Tv_ArchHd.Fcc) && (E_GrArchivVer == Tv_ArchHd.Ver))
			{
				if ((Tv_ArchHd.PayloadSize + sizeof(Tv_ArchHd)) <= m_InSize)
				{
					m_InSize	=	Tv_ArchHd.PayloadSize + sizeof(Tv_ArchHd);
					Tv_Result	=	TRUE;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcArchHeadCheck - Invalid fcc or version\n");
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcArchHeadCheck - header read fail\n");
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::DoProcess(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrArchivInputTypeNone != m_InType)
		{
			// check progress
			if (m_Progress < m_InSize)
			{
				Tv_Result	=	TRUE;
				// check mode
				switch (m_RunMode)
				{
					case E_GrArchivRunModeChunkHead:
						// decoder header
						LcPrcChunkHead();
						break;
					case E_GrArchivRunModeFolder:
						LcPrcFolder();
						break;
					case E_GrArchivRunModeFileHead:
						LcPrcFileHead();
						break;
					case E_GrArchivRunModeRaw:
						LcPrcRaw();
						break;
					case E_GrArchivRunModeWriteFile:
						LcPrcWriteFile();
						break;
					case E_GrArchivRunModeWriteRaw:
						LcPrcWriteRaw();
						break;
					default:
						// not supported type
						DbgMsgPrint("Cls_GrArchDec::DoProcess - [BUG] unknown mode (%d) type\n", m_RunMode);
						m_Progress	=	m_InSize;
						Tv_Result		=	FALSE;
						break;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcChunkHead(void)
{
	// local -------------------
	// code --------------------
		// read chunk header
		if (LcInDataRead(&m_ChukHd, sizeof(m_ChukHd)))
		{
			// check fcc
			if (E_GrArchivChunkFcc == m_ChukHd.Fcc)
			{
				// check data size
				if ((m_Progress + m_ChukHd.Size) <= m_InSize)
				{
					// check section index
					if (E_GrArchivSectionMaxCnt > m_ChukHd.Section)
					{
						// check exist section
						if (GrStrIsExist(m_SectTbl[m_ChukHd.Section].StrDev))
						{
							// check chunk type
							if (E_GrArchivChunkTypeCnt > m_ChukHd.Type)
							{
								// update section
								m_RunMode		= E_GrArchivRunModeFolder + m_ChukHd.Type;
							}
							else
							{
								DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] bad chunk type\n", m_Progress);
								m_Progress		=	m_InSize;
							}
						}
						else
						{
							// section skip
							if (LcInDataSkip(m_ChukHd.Size))
							{
								DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] skip section (%d) data\n", m_Progress, m_ChukHd.Section);
							}
							else
							{
								DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] data skip fail\n", m_Progress);
								m_Progress		=	m_InSize;
							}
						}
					}
					else
					{
						DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] section id out of range\n", m_Progress);
						m_Progress		=	m_InSize;
					}
				}
				else
				{
					DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] bad chunk data size\n", m_Progress);
					m_Progress		=	m_InSize;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] chunk FCC code missmatch\n",m_Progress);
				m_Progress		=	m_InSize;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcHead - [%08X] chunk header read fail\n",m_Progress);
			m_Progress	=	m_InSize;
		}
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFolder(void)
{
	// local -------------------
		Def_StrGrFnPathLong	Tv_StrPath;
		St_GrArchivChunkFolderA	Tv_DtFolder;
	// code --------------------
		if (LcInDataRead(&Tv_DtFolder, sizeof(Tv_DtFolder)))
		{
			// create folder
			GrStrCopy(Tv_StrPath, m_SectTbl[m_ChukHd.Section].StrDev);
			GrStrFnAddBkSlsh(Tv_StrPath);
			GrStrCat(Tv_StrPath, Tv_DtFolder.StrPath);
#ifdef LINUX_APP
			if (!GrFileCreateDir(Tv_StrPath, TRUE, Tv_DtFolder.Owner))
#else
			if (!GrFileCreateDir(Tv_StrPath, m_IsNoEmu, Tv_DtFolder.Owner))
#endif
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcFolder - [%08X] folder (%s) create fail\n", m_Progress, Tv_StrPath);
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcFolder - [%08X] data read fail\n", m_Progress);
			m_Progress	=	m_InSize;
		}
		
		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFileHead(void)
{
	// local -------------------
		
	// code --------------------
		if (LcInDataRead(&m_FileHd, sizeof(m_FileHd)))
		{
			// check file header
			if ((m_FileHd.Size + sizeof(m_FileHd)) == m_ChukHd.Size)
			{
				switch (m_FileHd.Type)
				{
					case E_GrArchivFileTypeNormal:
						LcPrcFileNormal();
						break;
					case E_GrArchivFileTypeLink:
						LcPrcFileLink();
						break;
					case E_GrArchivFileTypeCharDev:
						LcPrcFileCdev();
						break;
					case E_GrArchivFileTypeBlockDev:
						LcPrcFileBdev();
						break;
					default:
						DbgMsgPrint("Cls_GrArchDec::LcPrcFileHead - [%08X] unsupported file type (%d)\n", m_Progress, m_FileHd.Type);
						m_Progress	=	m_InSize;
						m_RunMode		=	E_GrArchivRunModeChunkHead;
						break;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcFileHead - [%08X] bad file size\n", m_Progress);
				m_Progress	=	m_InSize;
				m_RunMode		=	E_GrArchivRunModeChunkHead;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcFileHead - [%08X] data read fail\n", m_Progress);
			m_Progress	=	m_InSize;
			m_RunMode		=	E_GrArchivRunModeChunkHead;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFileNormal(void)
{
	// local -------------------
		Def_StrGrFnPathLong	Tv_StrFn;
		BOOL8	Tv_IsExc;
	// code --------------------
		// create file
		GrStrCopy(Tv_StrFn, m_SectTbl[m_ChukHd.Section].StrDev);
		GrStrFnAddBkSlsh(Tv_StrFn);
		GrStrCat(Tv_StrFn, m_FileHd.StrPath);

		Tv_IsExc		=	FALSE;
		if (0 != (0111 & m_FileHd.Owner))
		{
			Tv_IsExc		=	TRUE;
		}

#ifdef LINUX_APP
		m_FcFile	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(Tv_StrFn, TRUE, TRUE, Tv_IsExc, TRUE);
#else
		m_FcFile	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(Tv_StrFn, TRUE, TRUE, Tv_IsExc, m_IsNoEmu);
#endif
		if (NULL != m_FcFile)
		{
			if (m_FcFile->IsOpened())
			{
				m_RunMode			=	E_GrArchivRunModeWriteFile;
				m_FileRemain	=	m_FileHd.Size;
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcFileNormal - [%08X] file create fail\n", m_Progress);
				m_Progress	=	m_InSize;
				m_RunMode		=	E_GrArchivRunModeChunkHead;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcFileNormal - [%08X] file control create fail\n", m_Progress);
			m_Progress	=	m_InSize;
			m_RunMode		=	E_GrArchivRunModeChunkHead;
		}

		if (E_GrArchivRunModeWriteFile != m_RunMode)
		{
			delete	m_FcFile;
			m_FcFile	=	NULL;
		}

}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcWriteFile(void)
{
	// local -------------------
		__u32	Tv_WtSize;
	// code --------------------
		Tv_WtSize	=	m_FileRemain;
		if (E_GrArchivWorkBufSize <= Tv_WtSize)
		{
			Tv_WtSize	=	E_GrArchivWorkBufSize;
		}
		
		// read
		if (LcInDataRead(m_WorkBuf, Tv_WtSize))
		{
			// write
			if (Tv_WtSize == m_FcFile->Write(m_WorkBuf,Tv_WtSize))
			{
				m_FileRemain	=	m_FileRemain - Tv_WtSize;
				if (0 == m_FileRemain)
				{
					// finish
					m_RunMode		=	E_GrArchivRunModeChunkHead;
				}
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcWriteFile - [%08X] archive read fail\n", m_Progress);
				m_Progress	=	m_InSize;
				m_RunMode		=	E_GrArchivRunModeChunkHead;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcWriteFile - [%08X] archive read fail\n", m_Progress);
			m_Progress	=	m_InSize;
			m_RunMode		=	E_GrArchivRunModeChunkHead;
		}

		if (E_GrArchivRunModeWriteFile != m_RunMode)
		{
			delete	m_FcFile;
			m_FcFile	=	NULL;
		}

}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::LcInDataSkip(__u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		// check read able size
		if ((m_Progress + A_Size) <= m_InSize)
		{
			switch (m_InType)
			{
				case E_GrArchivInputTypeMemory:
					Tv_Result	=	TRUE;
					break;
				case E_GrArchivInputTypeFile:
					if ((m_Progress + A_Size) == (__u32)m_FcIn->Seek((__s64)(m_Progress + A_Size)))
					{
						Tv_Result	=	TRUE;
					}
					break;
			}

			if (Tv_Result)
			{
				m_Progress	=	m_Progress + A_Size;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::Close(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (E_GrArchivInputTypeNone != m_InType)
		{
			// close file controls
			if (NULL != m_FcIn)
			{
				delete	m_FcIn;
				m_FcIn	=	NULL;
			}

			if (NULL != m_FcFile)
			{
				delete	m_FcFile;
				m_FcFile	=	NULL;
			}

			m_InType	=	E_GrArchivInputTypeNone;
			m_RunMode	=	E_GrArchivRunModeChunkHead;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::ExtracFromMemory(void* A_PtrBuf, __u32 A_Size)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (OpenFromMemory(A_PtrBuf,A_Size))
		{
			Tv_Result	=	TRUE;
			while (DoProcess())
			{

			}
			Close();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrArchDec::ExtracFromFile(char* A_StrFn, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if (OpenFromFile(A_StrFn,A_IsNoEmu))
		{
			Tv_Result	=	TRUE;
			while (DoProcess())
			{

			}
			Close();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFileLink(void)
{
	// local -------------------
#ifdef LINUX_APP
		Def_StrGrFnPathLong	Tv_StrFn;
		Def_StrGrFnPathLong	Tv_StrLink;
#endif
	// code --------------------

#ifdef LINUX_APP
		// check size
		if (sizeof(Tv_StrLink) > m_FileHd.Size)
		{
			if (LcInDataRead(Tv_StrLink, m_FileHd.Size))
			{
				Tv_StrLink[m_FileHd.Size]	=	0;	// mark end

				GrStrCopy(Tv_StrFn, m_SectTbl[m_ChukHd.Section].StrDev);
				GrStrFnAddBkSlsh(Tv_StrFn);
				GrStrCat(Tv_StrFn, m_FileHd.StrPath);
				// make link
				symlink(Tv_StrLink, Tv_StrFn);
			}
			else
			{
				DbgMsgPrint("Cls_GrArchDec::LcPrcFileLink - data read error\n");
				m_Progress	=	m_InSize;
			}
		}
		else
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcFileLink - link data size over\n");
			m_Progress	=	m_InSize;
		}
#else
		DbgMsgPrint("Cls_GrArchDec::LcPrcFileLink - Skip symbolic link (%s)\n", m_FileHd.StrPath);
		if (!LcInDataSkip(m_FileHd.Size))
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcFileLink - data skip error\n");
			m_Progress	=	m_InSize;
		}
#endif

		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFileCdev(void)
{
	// local -------------------
#ifdef LINUX_APP
		Def_StrGrFnPathLong	Tv_StrFn;
		__u32	Tv_Owner;
		int		Tv_Major;
		int		Tv_Minor;
		dev_t	Tv_Dev;
#endif
	// code --------------------
#ifdef LINUX_APP
		GrStrCopy(Tv_StrFn, m_SectTbl[m_ChukHd.Section].StrDev);
		GrStrFnAddBkSlsh(Tv_StrFn);
		GrStrCat(Tv_StrFn, m_FileHd.StrPath);

		Tv_Major	=	(int)(m_FileHd.Para & 0xFFFF);
		Tv_Minor	=	(int)((m_FileHd.Para >> 16) & 0xFFFF);
		Tv_Dev		=	makedev(Tv_Major, Tv_Minor);

		Tv_Owner	=	(m_FileHd.Owner & 0777) | S_IFCHR;

		mknod(Tv_StrFn, (mode_t)Tv_Owner, Tv_Dev);
#else
		DbgMsgPrint("Cls_GrArchDec::LcPrcFileCdev - Skip charactor device node (%s)\n", m_FileHd.StrPath);
#endif

		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcFileBdev(void)
{
	// local -------------------
#ifdef LINUX_APP
		Def_StrGrFnPathLong	Tv_StrFn;
		__u32	Tv_Owner;
		int		Tv_Major;
		int		Tv_Minor;
		dev_t	Tv_Dev;
#endif
	// code --------------------
#ifdef LINUX_APP
		GrStrCopy(Tv_StrFn, m_SectTbl[m_ChukHd.Section].StrDev);
		GrStrFnAddBkSlsh(Tv_StrFn);
		GrStrCat(Tv_StrFn, m_FileHd.StrPath);

		Tv_Major	=	(int)(m_FileHd.Para & 0xFFFF);
		Tv_Minor	=	(int)((m_FileHd.Para >> 16) & 0xFFFF);
		Tv_Dev		=	makedev(Tv_Major, Tv_Minor);

		Tv_Owner	=	(m_FileHd.Owner & 0777) | S_IFBLK;

		mknod(Tv_StrFn, (mode_t)Tv_Owner, Tv_Dev);
#else
		DbgMsgPrint("Cls_GrArchDec::LcPrcFileBdev - Skip block device node (%s)\n", m_FileHd.StrPath);
#endif

		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcRaw(void)
{
	// local -------------------
	// code --------------------
		DbgMsgPrint("Cls_GrArchDec::LcPrcRaw - not implemented\n");
		if (!LcInDataSkip(m_ChukHd.Size))
		{
			DbgMsgPrint("Cls_GrArchDec::LcPrcRaw - data skip error\n");
			m_Progress	=	m_InSize;
		}
		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
void	Cls_GrArchDec::LcPrcWriteRaw(void)
{
		DbgMsgPrint("Cls_GrArchDec::LcPrcWriteRaw - [BUG] this funcition not implemented\n");
		m_Progress	=	m_InSize;
		m_RunMode	=	E_GrArchivRunModeChunkHead;
}
//--------------------------------------------------------------------
BOOL8	GrArchivGetFileInfo(char* A_StrFn, __u32* A_PtrRtPayloadSize, __u32* A_PtrRtChksum, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrArchivHeader	Tv_Header;
		__u32	Tv_FlSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if (sizeof(Tv_Header) == GrFileCtrlRead(A_StrFn, &Tv_Header, sizeof(Tv_Header), A_IsNoEmu))
		{
			// check header
			if (E_GrArchivHeadFcc == Tv_Header.Fcc)
			{
				if (E_GrArchivVer == Tv_Header.Ver)
				{
					Tv_FlSize		=	(__u32)GrFileGetSize(A_StrFn, A_IsNoEmu);
					if ((Tv_Header.PayloadSize + sizeof(St_GrArchivHeader)) <= Tv_FlSize)
					{
						// success
						*A_PtrRtPayloadSize	=	Tv_Header.PayloadSize;
						*A_PtrRtChksum			=	Tv_Header.PayloadChk;
						*A_PtrRtId					=	Tv_Header.DataId;
						GrDumyCopyMem(A_PtrRtUserData, Tv_Header.UserData, E_GrArchivUserDataSize);
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
		
}
//--------------------------------------------------------------------
BOOL8	GrArchivGetFileInfo(WCHAR* A_StrFn, __u32* A_PtrRtPayloadSize, __u32* A_PtrRtChksum, __u32* A_PtrRtId, void* A_PtrRtUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrArchivHeader	Tv_Header;
		__u32	Tv_FlSize;
	// code --------------------
		Tv_Result	=	FALSE;

		if (sizeof(Tv_Header) == GrFileCtrlRead(A_StrFn, &Tv_Header, sizeof(Tv_Header), A_IsNoEmu))
		{
			// check header
			if (E_GrArchivHeadFcc == Tv_Header.Fcc)
			{
				if (E_GrArchivVer == Tv_Header.Ver)
				{
					Tv_FlSize		=	(__u32)GrFileGetSize(A_StrFn, TRUE);
					if ((Tv_Header.PayloadSize + sizeof(St_GrArchivHeader)) <= Tv_FlSize)
					{
						// success
						*A_PtrRtPayloadSize	=	Tv_Header.PayloadSize;
						*A_PtrRtChksum			=	Tv_Header.PayloadChk;
						*A_PtrRtId					=	Tv_Header.DataId;
						GrDumyCopyMem(A_PtrRtUserData, Tv_Header.UserData, E_GrArchivUserDataSize);
						Tv_Result	=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32	Cls_GrArchDec::TotalSizeGet(void)
{
	return	m_InSize;
}
//--------------------------------------------------------------------
__u32	Cls_GrArchDec::ProgressSizeGet(void)
{
	return	m_Progress;
}
//--------------------------------------------------------------------
BOOL8	GrArchivAddFile(char* A_StrFnArchiv, char* A_StrFnAdd, __u8 A_Section, char* A_StrSetFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, void* A_PtrUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_FlSize;
		St_GrArchivChunkHead	Tv_CkHd;
		St_GrArchivChunkFileA	Tv_FtHd;
		Cls_GrFileCtrl*	Tv_FcAdd;
		Cls_GrFileCtrl*	Tv_FcArc;
		__u32	Tv_Remain;
		__u32	Tv_WtSize;
		__u32	Tv_ChkSum;
		__u32	Tv_Pos;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		Tv_FcArc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnArchiv, TRUE, FALSE, FALSE, A_IsNoEmu);
		Tv_FcAdd	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnAdd, FALSE, FALSE, FALSE, A_IsNoEmu);

		// check able
		if ((NULL != Tv_FcArc) && (NULL != Tv_FcAdd))
		{
			// check opend
			if (Tv_FcArc->IsOpened() && Tv_FcAdd->IsOpened())
			{
				// get add file info
				Tv_FlSize	=	(__u32)Tv_FcAdd->GetSize();

				// set file position
				Tv_FcArc->Seek(0, E_GrFileCtrlPosEnd);
				Tv_FcAdd->Seek(0, E_GrFileCtrlPosBegin);

				// build chunk head
				GrDumyZeroMem(&Tv_CkHd, sizeof(Tv_CkHd));
				Tv_CkHd.Fcc			=	E_GrArchivChunkFcc;
				Tv_CkHd.Section	=	A_Section;
				Tv_CkHd.Type		=	E_GrArchivChunkTypeFile;
				Tv_CkHd.Size		=	Tv_FlSize + sizeof(St_GrArchivChunkFileA);
				if (sizeof(Tv_CkHd) == Tv_FcArc->Write(&Tv_CkHd,sizeof(Tv_CkHd)))
				{
					// build file type head
					GrDumyZeroMem(&Tv_FtHd, sizeof(Tv_FtHd));
					Tv_FtHd.Owner	=	0666;
					Tv_FtHd.Type	=	E_GrArchivFileTypeNormal;
					Tv_FtHd.Size	=	Tv_FlSize;
					GrStrCopy(Tv_FtHd.StrPath, A_StrSetFn);
					if (sizeof(Tv_FtHd) == Tv_FcArc->Write(&Tv_FtHd, sizeof(Tv_FtHd)))
					{
						Tv_Result	=	TRUE;
						// add file contents
						Tv_Remain	=	Tv_FlSize;
						while (0 != Tv_Remain)
						{
							Tv_WtSize	=	Tv_Remain;
							if (Tv_WtSize > A_WorkBufSize)
							{
								Tv_WtSize	=	A_WorkBufSize;
							}
							// read
							if (Tv_WtSize != Tv_FcAdd->Read(A_PtrWorkBuf,Tv_WtSize))
							{
								Tv_Result	=	FALSE;
								break;
							}
							// write
							if (Tv_WtSize != Tv_FcArc->Write(A_PtrWorkBuf, Tv_WtSize))
							{
								Tv_Result	=	FALSE;
								break;
							}
							// next
							Tv_Remain	=	Tv_Remain - Tv_WtSize;
						}

					}
				}
			}

			delete	Tv_FcArc;
			delete	Tv_FcAdd;
		}

		// set checksum
		if (Tv_Result)
		{
			Tv_Result	=	FALSE;
			Tv_FlSize	=	(__u32)GrFileGetSize(A_StrFnArchiv);
			Tv_FlSize	=	Tv_FlSize - sizeof(St_GrArchivHeader);

			if (GrArchivGetFileChksum(A_StrFnArchiv,Tv_FlSize,A_PtrWorkBuf,A_WorkBufSize,&Tv_ChkSum,A_IsNoEmu))
			{
				// update checksum

				Tv_FcArc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnArchiv, TRUE, FALSE, FALSE, A_IsNoEmu);
				if (NULL != Tv_FcArc)
				{
					if (Tv_FcArc->IsOpened())
					{
						Tv_Pos	=	(__u32)(&((Ptr_GrArchivHeader)NULL)->PayloadChk);
						if (Tv_Pos == (__u32)Tv_FcArc->Seek((__s64)Tv_Pos))
						{
							if (4 == Tv_FcArc->Write(&Tv_ChkSum,4))
							{
								Tv_Result	=	TRUE;

								if (NULL != A_PtrUserData)
								{
									Tv_Pos	=	(__u32)(((Ptr_GrArchivHeader)NULL)->UserData);
									if (Tv_Pos == (__u32)Tv_FcArc->Seek((__s64)Tv_Pos))
									{
										Tv_FcArc->Write(A_PtrUserData, 12);
									}
								}

							}
						}
					}

					delete	Tv_FcArc;
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	GrArchivAddFile(WCHAR* A_StrFnArchiv, WCHAR* A_StrFnAdd, __u8 A_Section, WCHAR* A_StrSetFn, void* A_PtrWorkBuf, __u32 A_WorkBufSize, void* A_PtrUserData, BOOL8 A_IsNoEmu)
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_FlSize;
		St_GrArchivChunkHead	Tv_CkHd;
		St_GrArchivChunkFileA	Tv_FtHd;
		Cls_GrFileCtrl*	Tv_FcAdd;
		Cls_GrFileCtrl*	Tv_FcArc;
		__u32	Tv_Remain;
		__u32	Tv_WtSize;
		__u32	Tv_ChkSum;
		St_GrArchivHeader	Tv_AcHd;
	// code --------------------
		// init
		Tv_Result	=	FALSE;

		Tv_FcArc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnArchiv, TRUE, FALSE, FALSE, A_IsNoEmu);
		Tv_FcAdd	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnAdd, FALSE, FALSE, FALSE, A_IsNoEmu);

		// check able
		if ((NULL != Tv_FcArc) && (NULL != Tv_FcAdd))
		{
			// check opend
			if (Tv_FcArc->IsOpened() && Tv_FcAdd->IsOpened())
			{
				// get add file info
				Tv_FlSize	=	(__u32)Tv_FcAdd->GetSize();

				// set file position
				Tv_FcArc->Seek(0, E_GrFileCtrlPosEnd);
				Tv_FcAdd->Seek(0, E_GrFileCtrlPosBegin);

				// build chunk head
				GrDumyZeroMem(&Tv_CkHd, sizeof(Tv_CkHd));
				Tv_CkHd.Fcc			=	E_GrArchivChunkFcc;
				Tv_CkHd.Section	=	A_Section;
				Tv_CkHd.Type		=	E_GrArchivChunkTypeFile;
				Tv_CkHd.Size		=	Tv_FlSize + sizeof(St_GrArchivChunkFileA);
				if (sizeof(Tv_CkHd) == Tv_FcArc->Write(&Tv_CkHd, sizeof(Tv_CkHd)))
				{
					// build file type head
					GrDumyZeroMem(&Tv_FtHd, sizeof(Tv_FtHd));
					Tv_FtHd.Owner	=	0666;
					Tv_FtHd.Type	=	E_GrArchivFileTypeNormal;
					Tv_FtHd.Size	=	Tv_FlSize;
					GrStrWstrToStr(Tv_FtHd.StrPath, A_StrSetFn);
					if (sizeof(Tv_FtHd) == Tv_FcArc->Write(&Tv_FtHd, sizeof(Tv_FtHd)))
					{
						Tv_Result	=	TRUE;
						// add file contents
						Tv_Remain	=	Tv_FlSize;
						while (0 != Tv_Remain)
						{
							Tv_WtSize	=	Tv_Remain;
							if (Tv_WtSize > A_WorkBufSize)
							{
								Tv_WtSize	=	A_WorkBufSize;
							}
							// read
							if (Tv_WtSize != Tv_FcAdd->Read(A_PtrWorkBuf, Tv_WtSize))
							{
								Tv_Result	=	FALSE;
								break;
							}
							// write
							if (Tv_WtSize != Tv_FcArc->Write(A_PtrWorkBuf, Tv_WtSize))
							{
								Tv_Result	=	FALSE;
								break;
							}
							// next
							Tv_Remain	=	Tv_Remain - Tv_WtSize;
						}

					}
				}
			}

			delete	Tv_FcArc;
			delete	Tv_FcAdd;
		}

		// set checksum
		if (Tv_Result)
		{
			Tv_Result	=	FALSE;
			Tv_FlSize	=	(__u32)GrFileGetSize(A_StrFnArchiv);
			Tv_FlSize	=	Tv_FlSize - sizeof(St_GrArchivHeader);

			if (GrArchivGetFileChksum(A_StrFnArchiv, Tv_FlSize, A_PtrWorkBuf, A_WorkBufSize, &Tv_ChkSum,A_IsNoEmu))
			{
				// update checksum

				Tv_FcArc	=	(Cls_GrFileCtrl*) new Cls_GrFileCtrl(A_StrFnArchiv, TRUE, FALSE, FALSE, A_IsNoEmu);
				if (NULL != Tv_FcArc)
				{
					if (Tv_FcArc->IsOpened())
					{
						if (sizeof(Tv_AcHd) == Tv_FcArc->Read(&Tv_AcHd, sizeof(Tv_AcHd)))
						{
							// update 
							Tv_AcHd.PayloadSize		=	Tv_FlSize;
							Tv_AcHd.PayloadChk		=	Tv_ChkSum;
							if (NULL != A_PtrUserData)
							{
								GrDumyCopyMem(Tv_AcHd.UserData, A_PtrUserData, 12);
							}

							// write
							if (0 == (__u32)Tv_FcArc->Seek(0))
							{
								if (sizeof(Tv_AcHd) == Tv_FcArc->Write(&Tv_AcHd, sizeof(Tv_AcHd)))
								{
									Tv_Result	=	TRUE;
								}
							}

						}
					}

					delete	Tv_FcArc;
				}

			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
