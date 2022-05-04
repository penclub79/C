/*
	Platform 5 develop reference header parser

*/

//--------------------------------------------------------------------
// uses

#include <P5/Dvl/GrP5DvlRefPrs.h>

#include <P5/GrP5Base.h>
#include <P5/Dvl/GrP5DvlBase.h>

#include <GrStrTool.h>
#include <GrImgBase.h>
#include <GrDebug.h>
#include <GrDumyTool.h>
#include <GrGdibTool.h>

#include <GrLayerFile.h>

//--------------------------------------------------------------------
// local constant


//--------------------------------------------------------------------
// local type

//--------------------------------------------------------------------
//local function

//--------------------------------------------------------------------
//global var

extern	St_GrP5DvlEnv	V_GrP5DvlEnv;

//====================================================================
//--------------------------------------------------------------------
Cls_GrP5DvlRefPrs::Cls_GrP5DvlRefPrs(Cls_GrTaskSvr* A_TaskSvr) :
Cls_GrTaskCli()
{
	// local -------------------
	// code --------------------
		m_TskSvr	=	A_TaskSvr;

		m_MsgAloc	=	GrTaskSvrMsgCmdAlloc();

		GrStrClear(m_StrFnHead);
		GrStrClear(m_StrPrefix);
		m_IsDoPars	=	FALSE;

		m_FtimeLast.dwHighDateTime	=	0;
		m_FtimeLast.dwLowDateTime		=	0;

		m_TskSvr->RegistMsgClient(this);
		m_TskSvr->RegistTimerClient(this, E_GrP5DvlRefPrsTimerPeriod, &m_TmrMain);
		m_TskSvr->RegistBkgClient(this);

}
//--------------------------------------------------------------------
Cls_GrP5DvlRefPrs::~Cls_GrP5DvlRefPrs(void)
{

		m_TskSvr->UnregistBkgClient(this);
		m_TskSvr->UnregistTimerClient(m_TmrMain);
		m_TskSvr->UnregistMsgClient(this);

		GrTaskSvrMsgCmdFree(m_MsgAloc);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlRefPrs::RtlMsgProc(Ptr_GrTaskMsgHd A_PtrMsg)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlRefPrs::RtlBkgProc(void)
{
	// local -------------------
		BOOL8	Tv_Result;
	// code --------------------
		Tv_Result	=	FALSE;

		if(BprcParse())
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::RtlTimer(__u32 A_Hndl)
{
	// local -------------------
	// code --------------------
		// check file changed
		if(A_Hndl == m_TmrMain)
		{
			// check file
			TprcFileChk();
		}

}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::TprcFileChk(void)
{
	// local -------------------
		BOOL8	Tv_IsChk;
		HANDLE	Tv_Hf;
		FILETIME	Tv_FtCreate;
		FILETIME	Tv_FtAccess;
		FILETIME	Tv_FtWrite;
	// code --------------------
		Tv_IsChk		=	TRUE;
		// check file name
		if( 0 == GrStrWcmp(m_StrFnHead,V_GrP5DvlEnv.PrjCtrl.WorkEnv.StrPathRefHd,FALSE) )
		{
			if(0 == GrStrCmp(m_StrPrefix,V_GrP5DvlEnv.PrjCtrl.WorkEnv.StrRefHdPrefix,TRUE))
			{
				Tv_Hf	=	CreateFile(m_StrFnHead, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if(INVALID_HANDLE_VALUE != Tv_Hf)
				{
					// get file info
					if(GetFileTime(Tv_Hf,&Tv_FtCreate,&Tv_FtAccess,&Tv_FtWrite))
					{
						// check time
						if((m_FtimeLast.dwHighDateTime == Tv_FtWrite.dwHighDateTime) && (m_FtimeLast.dwLowDateTime == Tv_FtWrite.dwLowDateTime))
						{
							Tv_IsChk	=	FALSE;
						}
					}
					else
					{
						Tv_IsChk	=	FALSE;
					}
					CloseHandle(Tv_Hf);
				}
				else
				{
					Tv_IsChk	=	FALSE;
				}
			}
		}

		if(Tv_IsChk)
		{
			m_IsDoPars	=	TRUE;
		}
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::ResetData(void)
{
	// local -------------------
	// code --------------------
		V_GrP5DvlEnv.PrjCtrl.StpAdrCnt	=	0;

		GrDumyZeroMem(V_GrP5DvlEnv.PrjCtrl.CtrlIdInfoTbl, sizeof(St_GrP5DvlCtrlIdInfo)* E_GrP5CtrlIdMaxCnt);
		GrDumyZeroMem(V_GrP5DvlEnv.PrjCtrl.RintInfoTbl, sizeof(St_GrP5DvlVarIdInfo)* E_GrP5DvlRintMaxCnt);
		GrDumyZeroMem(V_GrP5DvlEnv.PrjCtrl.RtxtInfoTbl, sizeof(St_GrP5DvlVarIdInfo)* E_GrP5DvlRtxtMaxCnt);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrP5DvlRefPrs::BprcParse(void)
{
	// local -------------------
		BOOL8	Tv_Result;
		HANDLE	Tv_Hf;
		FILETIME	Tv_FtCreate;
		FILETIME	Tv_FtAccess;
		FILETIME	Tv_FtWrite;
	// code --------------------
		Tv_Result	=	FALSE;
		
		if(m_IsDoPars)
		{
			// reset variable
			ResetData();

			// update file name
			GrStrWcopy(m_StrFnHead, V_GrP5DvlEnv.PrjCtrl.WorkEnv.StrPathRefHd);

			// update prefix
			GrStrCopy(m_StrPrefix, V_GrP5DvlEnv.PrjCtrl.WorkEnv.StrRefHdPrefix);
			m_LenPrefix	=	GrStrLen(m_StrPrefix);

			if ( 0 != m_LenPrefix)
			{
				// update write time
				m_SrcSize		=	0;
				Tv_Hf	=	CreateFile(m_StrFnHead, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				if (INVALID_HANDLE_VALUE != Tv_Hf)
				{
					// get file info
					if (GetFileTime(Tv_Hf, &Tv_FtCreate, &Tv_FtAccess, &Tv_FtWrite))
					{
						// update file time
						m_FtimeLast	=	Tv_FtWrite;

						// read
						ReadFile(Tv_Hf, m_SrcBuf, E_GrP5DvlRefPrsSrcBufSize, (LPDWORD)&m_SrcSize, NULL);
					}
					CloseHandle(Tv_Hf);
				}

				if (0 != m_SrcSize)
				{
					// do parsing
					ParsData();
				}
			}

			m_IsDoPars	=	FALSE;
			Tv_Result		=	TRUE;

			// notify 
			GrP5DvlRefHeaderChgNotify();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::ParsData(void)
{
	// local -------------------
	// code --------------------
		// init
		m_ParsPos		=	0;
		m_IsRmkBlk	=	FALSE;
		m_ParsMode	=	E_GrP5DvlRefPrsModeNormal;

		// line pars
		while (m_ParsPos < m_SrcSize)
		{
			// anlysis line
			LineAnalysis();
			LinePars();

		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::LineAnalysis(void)
{
	// local -------------------
		BOOL8	Tv_InChr;
		BOOL8	Tv_IsRmkLine;
		BOOL8	Tv_IsQuotes;
		BOOL8	Tv_IsDone;
	// code --------------------
		// init
		m_LineAnalCnt	=	0;
		Tv_InChr			=	FALSE;
		Tv_IsRmkLine	=	FALSE;
		Tv_IsQuotes		=	FALSE;

		while (m_ParsPos < m_SrcSize)
		{
			Tv_IsDone		=	FALSE;

			// check charactor
			if (0 == m_SrcBuf[m_ParsPos])
			{
				// finish
				m_ParsPos	=	m_SrcSize;
				if (Tv_InChr)
				{
					m_LineAnalCnt ++;
				}
				break;
			}

			// check end of line
			if (0x0A == m_SrcBuf[m_ParsPos])
			{
				// end of line
				m_ParsPos ++;
				if (Tv_InChr)
				{
					m_LineAnalCnt ++;
				}
				break;
			}

			// check block remark
			if (m_IsRmkBlk)
			{
				if ('/' == m_SrcBuf[m_ParsPos])
				{
					if (0 != m_ParsPos)
					{
						if ('*' == m_SrcBuf[m_ParsPos - 1])
						{
							m_IsRmkBlk	=	FALSE;
						}
					}
				}
			}
			else
			{
				// check line remark
				if (Tv_IsRmkLine || Tv_IsQuotes)
				{
					if (Tv_IsQuotes)
					{
						if (0x22 == m_SrcBuf[m_ParsPos])
						{
							Tv_IsQuotes	=	FALSE;
						}
					}
				}
				else
				{
					// noraml
					// check quotes
					if (0x22 == m_SrcBuf[m_ParsPos])
					{
						Tv_IsQuotes	=	TRUE;
						Tv_IsDone		=	TRUE;

						if (Tv_InChr)
						{
							m_LineAnalCnt ++;
						}
						Tv_InChr	=	FALSE;

					}
					else
					{
						// check remark
						if (0 != m_ParsPos)
						{
							if ('/' == m_SrcBuf[m_ParsPos - 1])
							{
								if ('/' == m_SrcBuf[m_ParsPos])
								{
									Tv_IsRmkLine	=	TRUE;
									Tv_IsDone			=	TRUE;

									if (Tv_InChr)
									{
										m_LineAnalCnt ++;
									}
									Tv_InChr	=	FALSE;

								}
								else if ('*' == m_SrcBuf[m_ParsPos])
								{
									m_IsRmkBlk	=	TRUE;
									Tv_IsDone			=	TRUE;

									if (Tv_InChr)
									{
										m_LineAnalCnt ++;
									}
									Tv_InChr	=	FALSE;

								}
							}
						}

						if (!Tv_IsDone)
						{
							// check charactor
							if (0x20 < m_SrcBuf[m_ParsPos])
							{
								if (Tv_InChr)
								{
									m_LineAnalTbl[m_LineAnalCnt].Cnt ++;
								}
								else
								{
									m_LineAnalTbl[m_LineAnalCnt].Pos		=	m_ParsPos;
									m_LineAnalTbl[m_LineAnalCnt].Cnt		=	1;
									Tv_InChr	=	TRUE;
								}
							}
							else
							{
								if (Tv_InChr)
								{
									m_LineAnalCnt ++;
								}
								Tv_InChr	=	FALSE;
							}
						}
					}
				}
			}

			// next
			m_ParsPos ++;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrP5DvlRefPrs::LinePars(void)
{
	// local -------------------
		__u32	Tv_WdPos;
		__u32	Tv_WdLen;
		Def_StrTag	Tv_StrName;
		__u32	Tv_Mode;		// 0 = none, 1 = setup , 2 = control id , 3 = register int, 4 = register text
		__u32	Tv_Value;
	// code --------------------

		if (3 <= m_LineAnalCnt)
		{
			// check first word
			if (0 == GrStrCmp("#define", (char*)&m_SrcBuf[m_LineAnalTbl[0].Pos], FALSE, m_LineAnalTbl[0].Cnt))
			{
				// check prefix
				if (m_LenPrefix < m_LineAnalTbl[1].Cnt)
				{
					if (0 == GrStrCmp(m_StrPrefix, (char*)&m_SrcBuf[m_LineAnalTbl[1].Pos], TRUE, m_LenPrefix))
					{
						// init
						Tv_Mode		=	0;
						// check middle word
						Tv_WdPos	=	m_LineAnalTbl[1].Pos + m_LenPrefix;
						Tv_WdLen	=	m_LineAnalTbl[1].Cnt - m_LenPrefix;

						if((6 < Tv_WdLen) && (0 == GrStrCmp("CfgOfs",(char*)&m_SrcBuf[Tv_WdPos],TRUE,6)))
						{
							// setup offset
							// get name
							Tv_WdPos	=	Tv_WdPos + 6;
							Tv_WdLen	=	Tv_WdLen - 6;

							if(31 >= Tv_WdLen)
							{
								GrStrCopy(Tv_StrName, (char*)&m_SrcBuf[Tv_WdPos], Tv_WdLen);
								// set mode
								Tv_Mode		=	1;
							}
						}
						else if((6 < Tv_WdLen) && (0 == GrStrCmp("CtrlId", (char*)&m_SrcBuf[Tv_WdPos], TRUE, 6)))
						{
							// contorl id
							// get name
							Tv_WdPos	=	Tv_WdPos + 6;
							Tv_WdLen	=	Tv_WdLen - 6;

							if(31 >= Tv_WdLen)
							{
								GrStrCopy(Tv_StrName, (char*)&m_SrcBuf[Tv_WdPos], Tv_WdLen);
								// set mode
								Tv_Mode		=	2;
							}
						}
						else if((4 < Tv_WdLen) && (0 == GrStrCmp("Rint", (char*)&m_SrcBuf[Tv_WdPos], TRUE, 4)))
						{
							// register int index
							// get name
							Tv_WdPos	=	Tv_WdPos + 4;
							Tv_WdLen	=	Tv_WdLen - 4;

							if(31 >= Tv_WdLen)
							{
								GrStrCopy(Tv_StrName, (char*)&m_SrcBuf[Tv_WdPos], Tv_WdLen);
								// set mode
								Tv_Mode		=	3;
							}
						}
						else if((4 < Tv_WdLen) && (0 == GrStrCmp("Rtxt", (char*)&m_SrcBuf[Tv_WdPos], TRUE, 4)))
						{
							// register text index
							// get name
							Tv_WdPos	=	Tv_WdPos + 4;
							Tv_WdLen	=	Tv_WdLen - 4;

							if(31 >= Tv_WdLen)
							{
								GrStrCopy(Tv_StrName, (char*)&m_SrcBuf[Tv_WdPos], Tv_WdLen);
								// set mode
								Tv_Mode		=	4;
							}
						}
						
						// get value
						if(0 != Tv_Mode)
						{
							// init
							Tv_WdPos	=	m_LineAnalTbl[2].Pos;
							Tv_WdLen	=	m_LineAnalTbl[2].Cnt;

							// check hex value
							if((2 < Tv_WdLen) && (0 == GrStrCmp("0x", (char*)&m_SrcBuf[Tv_WdPos],FALSE,2)))
							{
								// hex value
								Tv_WdPos	= Tv_WdPos + 2;
								Tv_WdLen	=	Tv_WdLen - 2;
								if(8 < Tv_WdLen)
								{
									// string was long
									Tv_Mode	=	0;
								}
								else
								{
									if(!GrStrHexStrToDword((char*)&m_SrcBuf[Tv_WdPos], &Tv_Value, Tv_WdLen))
									{
										Tv_Mode		=	0;
									}
								}
								
							}
							else
							{
								// decimal value
								if(10 < Tv_WdLen)
								{
									// string was long
									Tv_Mode	=	0;
								}
								else
								{
									if(!GrStrStrToInt((char*)&m_SrcBuf[Tv_WdPos], (__s32*)&Tv_Value, Tv_WdLen))
									{
										Tv_Mode		=	0;
									}
								}

							}
						}

						// update value
						switch(Tv_Mode)
						{
							case 1:
								// setup value
								if(E_GrP5DvlSetupAdrMaxCnt > V_GrP5DvlEnv.PrjCtrl.StpAdrCnt)
								{
									GrStrStrToWstr(V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[V_GrP5DvlEnv.PrjCtrl.StpAdrCnt].StrName, Tv_StrName);
									V_GrP5DvlEnv.PrjCtrl.StpAdrTbl[V_GrP5DvlEnv.PrjCtrl.StpAdrCnt].Ofs	=	Tv_Value;
									V_GrP5DvlEnv.PrjCtrl.StpAdrCnt ++;
								}
								else
								{
									DbgMsgPrint("Cls_GrP5DvlRefPrs::LinePars - setup address buffer is full\n");
								}
								break;
							case  2:
								// control id
								if ((E_GrP5CtrlIdBase <= Tv_Value) && ((E_GrP5CtrlIdBase + E_GrP5CtrlIdMaxCnt) > Tv_Value))
								{
									Tv_Value	=	Tv_Value - E_GrP5CtrlIdBase;
									GrStrStrToWstr(V_GrP5DvlEnv.PrjCtrl.CtrlIdInfoTbl[Tv_Value].StrName, Tv_StrName);
								}
								else
								{
									DbgMsgPrint("Cls_GrP5DvlRefPrs::LinePars - bad control id (%d) input\n", Tv_Value);
								}
								break;
							case 3 :
								// register integer
								if((E_GrP5RegIdBase <= Tv_Value) && ((E_GrP5RegIdBase + E_GrP5DvlRintMaxCnt) > Tv_Value))
								{
									Tv_Value	=	Tv_Value - E_GrP5RegIdBase;
									GrStrStrToWstr(V_GrP5DvlEnv.PrjCtrl.RintInfoTbl[Tv_Value].StrName, Tv_StrName);
								}
								else
								{
									DbgMsgPrint("Cls_GrP5DvlRefPrs::LinePars - bad register integer id (%d) input\n", Tv_Value);
								}
								break;
							case 4:
								if((E_GrP5RegIdBase <= Tv_Value) && ((E_GrP5RegIdBase + E_GrP5DvlRtxtMaxCnt) > Tv_Value))
								{
									Tv_Value	=	Tv_Value - E_GrP5RegIdBase;
									GrStrStrToWstr(V_GrP5DvlEnv.PrjCtrl.RtxtInfoTbl[Tv_Value].StrName, Tv_StrName);
								}
								else
								{
									DbgMsgPrint("Cls_GrP5DvlRefPrs::LinePars - bad register integer id (%d) input\n", Tv_Value);
								}
								break;

						}

					}
				}

			}
		}

		
}
//--------------------------------------------------------------------

