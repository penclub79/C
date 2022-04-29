/*
 GAUSE file system version 5 DEMUX

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

#include <Dvr/Fs/V6/GrDvrFs6Base.h>
#include <Dvr/Fs/V6/GrDvrFs6Dmux.h>

#include	<stdlib.h>

//====================================================================
//local const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

extern	St_GrDvrFs6Env	V_GrDvrFs6Env;

//====================================================================

//--------------------------------------------------------------------
Cls_GrDvrFs6Dmux::Cls_GrDvrFs6Dmux(__u8 A_MaxChCnt) 
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------
		//init

		//init
		m_FsMng						=	(Cls_GrDvrFs6Mng*)V_GrDvrFs6Env.ObjMng;

		m_DmuxMaxChCnt		=	A_MaxChCnt;

		m_DmuxGrpType			=	E_GrDvrRecDevNoraml;

		m_IsDmuxDirBack		=	FALSE;
		m_DmuxRunStat			=	E_GrDvrPlayStatStop;
		m_DmuxLastOutTime	=	0;

		m_DmuxVdoChCnt		=	0;
		m_DmuxAdoChCnt		=	0;

		GrDumyZeroMem(m_DmuxChTbl, sizeof(m_DmuxChTbl));
		
		for( Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxChCnt; Tv_WkIdx++)
		{
			m_DmuxChTbl[Tv_WkIdx].ReadCtl.Ch		=	Tv_WkIdx;
			if(((0 <= Tv_WkIdx) && ((__u32)m_DmuxMaxChCnt > Tv_WkIdx)) || 
				((E_GrDvrFs6AdoChBase <= Tv_WkIdx) && ((__u32)(m_DmuxMaxChCnt + E_GrDvrFs6AdoChBase) > Tv_WkIdx)))
			{
				m_DmuxChTbl[Tv_WkIdx].IsAble				=	TRUE;
			}
		}
}
//--------------------------------------------------------------------
Cls_GrDvrFs6Dmux::~Cls_GrDvrFs6Dmux()
{
	// local -------------------
	// code --------------------

		
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxDirSet(BOOL8 A_IsDirBack)
{
		m_IsDmuxDirBack	=	A_IsDirBack;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxGoTime(Def_GrTime A_Time)
{
	// local -------------------
	// code --------------------
		// reset dmux

		// do seek mode
		if(m_IsDmuxDirBack)
		{
			LcDmuxBwGoByTime(A_Time);
		}
		else
		{
			LcDmuxFwGoByTime(A_Time);
		}

}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Dmux::DmuxDo(void)
{
	// local -------------------
		__u64	Tv_Result;
	// code --------------------
		// reset
		Tv_Result	=	0;
		// check seek mode
		if(E_GrDvrPlayStatSeek == m_DmuxRunStat)
		{
			if(m_IsDmuxDirBack)
			{
				LcDmuxBwRunPlay();
			}
			else
			{
				LcDmuxFwRunPlay();
			}
		}
		else if(E_GrDvrPlayStatPlay == m_DmuxRunStat)
		{
			Tv_Result	=	LcDmuxReadyChMapGet();
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxVdoChAbleSet(__u32 A_Map)
{
	// local -------------------
		BOOL8	Tv_IsSeek;
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
		__u32	Tv_Mask;
		__u32	Tv_ChCnt;
	// code --------------------
		Tv_ChCnt	=	0;
		Tv_Mask		=	1;
		for(Tv_WkIdx = 0; Tv_WkIdx < m_DmuxMaxChCnt; Tv_WkIdx++)
		{
			Tv_Ch		=	Tv_WkIdx;

			if(0 != (Tv_Mask & A_Map))
			{
				// seek channel
				Tv_IsSeek	=	!m_DmuxChTbl[Tv_Ch].IsAble;
				m_DmuxChTbl[Tv_Ch].IsAble	=	TRUE;

				if(m_IsDmuxDirBack)
				{
					LcDmuxBwChGoByTime(Tv_Ch, m_DmuxLastOutTime);
				}
				else
				{
					LcDmuxFwChGoByTime(Tv_Ch, m_DmuxLastOutTime);
				}

				Tv_ChCnt ++;
			}
			else
			{
				m_DmuxChTbl[Tv_Ch].IsAble	=	FALSE;
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

		m_DmuxVdoChCnt	=	Tv_ChCnt;	// update able channel count
		LcDmuxStatusUpdate();

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxAdoChAbleSet(__u32 A_Map)
{
	// local -------------------
		BOOL8	Tv_IsSeek;
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
		__u32	Tv_Mask;
		__u32	Tv_ChCnt;
	// code --------------------
		Tv_ChCnt	=	0;
		Tv_Mask		=	1;
		for(Tv_WkIdx = 0; Tv_WkIdx < m_DmuxMaxChCnt; Tv_WkIdx++)
		{
			Tv_Ch		=	Tv_WkIdx + E_GrDvrFs6AdoChBase;

			if(0 != (Tv_Mask & A_Map))
			{
				// seek channel
				Tv_IsSeek	=	!m_DmuxChTbl[Tv_Ch].IsAble;
				m_DmuxChTbl[Tv_Ch].IsAble	=	TRUE;

				if(m_IsDmuxDirBack)
				{
					LcDmuxBwChGoByTime(Tv_Ch, m_DmuxLastOutTime);
				}
				else
				{
					LcDmuxFwChGoByTime(Tv_Ch, m_DmuxLastOutTime);
				}

				Tv_ChCnt ++;
			}
			else
			{
				m_DmuxChTbl[Tv_Ch].IsAble	=	FALSE;
			}
			// next
			Tv_Mask	=	Tv_Mask << 1;
		}

		m_DmuxAdoChCnt	=	Tv_ChCnt;	// update able channel count
		LcDmuxStatusUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxVdoFrmMaskSet(__u8 A_Ch, __u8 A_Mask)
{
	// local -------------------
	// code --------------------
		if(m_DmuxChTbl[A_Ch].MaskMode != A_Mask)
		{
			m_DmuxChTbl[A_Ch].MaskMode	=	A_Mask;
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxFwChGoByTime(__u8 A_Ch, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_ErrCode;
	// code --------------------
		// init
		m_DmuxChTbl[A_Ch].IsOut	=	FALSE;

		// check able
		if(!m_DmuxChTbl[A_Ch].IsAble)
		{
			m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			return;
		}

		// find block
		Tv_ErrCode	=	m_FsMng->ImmReadFwGoByTime(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl, A_Time);
		if(E_GrErrNone != Tv_ErrCode)
		{
			m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			return;
		}

		while(TRUE)
		{
			// check data not exist
			if(E_GrDvrRecHndlNone == m_DmuxChTbl[A_Ch].ReadCtl.Hndl)
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
				return;
			}

			if(m_DmuxChTbl[A_Ch].ReadCtl.IsFrmRead)
			{
				// check frame
				LcDmuxChFrameStatUpdate(A_Ch);
				return;
			}

			// read next frame
			Tv_ErrCode	=	m_FsMng->ImmReadFwNext(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl);
			if(E_GrErrNone != Tv_ErrCode)
			{
				if(E_GrErrEndOfData == Tv_ErrCode)
				{
					m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatEnd;
				}
				else
				{
					m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
				}
				return;
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxFwGoByTime(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_WkIdx;
	// code --------------------
		for( Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			Tv_Ch		=	Tv_WkIdx;
			if(Tv_WkIdx >= m_DmuxMaxChCnt)
			{
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}

			LcDmuxFwChGoByTime(Tv_Ch, A_Time);
		}

		// update status
		LcDmuxStatusUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxStatusUpdate(void)
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_WkIdx;
		__u8	Tv_OkCnt;
		__u8	Tv_BgnCnt;
		__u8	Tv_EndCnt;
		__u8	Tv_SeekCnt;
	// code --------------------
		// init
		Tv_OkCnt		=	0;
		Tv_BgnCnt		=	0;
		Tv_EndCnt		=	0;
		Tv_SeekCnt	=	0;

		for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			Tv_Ch		=	Tv_WkIdx;
			if(Tv_WkIdx >= m_DmuxMaxChCnt)
			{
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}

			if(m_DmuxChTbl[Tv_Ch].IsAble)
			{
				if(E_GrDvrPlayStatPlay == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat)
				{
					Tv_OkCnt ++;
				}
				else if(E_GrDvrPlayStatSeek == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat)
				{
					Tv_SeekCnt ++;
				}
				else if(E_GrDvrPlayStatBegin == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat)
				{
					Tv_BgnCnt ++;
				}
				else if(E_GrDvrPlayStatEnd == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat)
				{
					Tv_EndCnt ++;
				}
			}
			else
			{
				m_DmuxChTbl[Tv_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			}
		}

		if(0 != Tv_SeekCnt)
		{
			m_DmuxRunStat	=	E_GrDvrPlayStatSeek;
		}
		else if(0 != Tv_OkCnt)
		{
			m_DmuxRunStat	=	E_GrDvrPlayStatPlay;
		}
		else if(m_IsDmuxDirBack)
		{
			if(0 != Tv_BgnCnt)
			{
				m_DmuxRunStat	=	E_GrDvrPlayStatBegin;
			}
		}
		else
		{
			if(0 != Tv_EndCnt)
			{
				m_DmuxRunStat	=	E_GrDvrPlayStatEnd;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxFwChRunPlay(__u8 A_Ch)
{
	// local -------------------
		__u8	Tv_ErrCode;
	// code --------------------
		// check able
		if((!m_DmuxChTbl[A_Ch].IsAble) || (E_GrDvrPlayStatStop == m_DmuxChTbl[A_Ch].ReadCtl.Stat))
		{
			return;
		}

		// check next frame
		if((!m_DmuxChTbl[A_Ch].ReadCtl.IsFrmRead) ||
			(E_GrDvrPlayStatSeek == m_DmuxChTbl[A_Ch].ReadCtl.Stat) ||
			((E_GrDvrPlayStatPlay == m_DmuxChTbl[A_Ch].ReadCtl.Stat) && m_DmuxChTbl[A_Ch].IsOut))
		{
			// rest out
			m_DmuxChTbl[A_Ch].IsOut	=	FALSE;
			// go next frame
			Tv_ErrCode	=	m_FsMng->ImmReadFwNext(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl);
			if(E_GrErrNone == Tv_ErrCode)
			{
				LcDmuxChFrameStatUpdate(A_Ch);
			}
			else if(E_GrErrEndOfData == Tv_ErrCode)
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatEnd;
			}
			else
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			}
		}

		return;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxFwRunPlay(void)
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
		//__u8	Tv_TryCnt;
	// code --------------------

		//for( Tv_TryCnt = 0; Tv_TryCnt < 32; Tv_TryCnt++)
		{
			if((E_GrDvrPlayStatStop == m_DmuxRunStat) || (E_GrDvrPlayStatPlay == m_DmuxRunStat))
			{
				return;
			}

			for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
			{
				// get channel
				Tv_Ch			=	Tv_WkIdx;
				if(Tv_Ch >= m_DmuxMaxChCnt)
				{
					// audio
					Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
				}

				LcDmuxFwChRunPlay(Tv_Ch);
			}

			LcDmuxStatusUpdate();
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxChFrameStatUpdate(__u8 A_Ch)
{
	// local -------------------
		//Ptr_GrDvrFrmHdVdo		Tv_PtrVdo;
	// code --------------------
		if(m_DmuxChTbl[A_Ch].IsAble && m_DmuxChTbl[A_Ch].ReadCtl.IsFrmRead)
		{
			m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatSeek;
			// check a/v frame
			if((E_GrDvrFrmHeadFcc == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Fcc) &&
				(E_GrDvrFrmTypeAdo >= m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type))
			{
				// check frame type
				if(E_GrDvrFrmTypeVdoP4x >= m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)
				{
					// video -------------------
					//Tv_PtrVdo	=	(Ptr_GrDvrFrmHdVdo)&m_DmuxChTbl[A_Ch].ReadCtl.FrmHd;
					if(E_GrDvrFrmTypeVdoI == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)
					{
						// i frame
						m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatPlay;
					}		//	if(E_GrDvrFrmTypeVdoI == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)
					else
					{
						// p frame
						if(!m_IsDmuxDirBack)
						{
							if(E_GrDvrFs6DmuxFrmMskAll == m_DmuxChTbl[A_Ch].MaskMode)
							{
								m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatPlay;
							}
							else if(E_GrDvrFs6DmuxFrmMskRef2x == m_DmuxChTbl[A_Ch].MaskMode)
							{
								if((E_GrDvrFrmTypeVdoP2x == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type) ||
									(E_GrDvrFrmTypeVdoP4x == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type))
								{
									m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatPlay;
								}
							}
							else if(E_GrDvrFs6DmuxFrmMskRef4x == m_DmuxChTbl[A_Ch].MaskMode)
							{
								if(E_GrDvrFrmTypeVdoP4x == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)
								{
									m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatPlay;
								}
							}
						}	//	if(!m_IsDmuxDirBack)

					}	// else .. if(E_GrDvrFrmTypeVdoI == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)

				}	//	if(E_GrDvrFrmTypeVdoP4x >= m_DmuxChTbl[Tv_Ch].ReadCtl.FrmHd.Base.Type)
				else if(E_GrDvrFrmTypeAdo == m_DmuxChTbl[A_Ch].ReadCtl.FrmHd.Base.Type)
				{
					// audio
					if(!m_IsDmuxDirBack)
					{
						m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatPlay;
					}
				}
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxBwChGoByTime(__u8 A_Ch, Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_ErrCode;
	// code --------------------
		// init
		m_DmuxChTbl[A_Ch].IsOut	=	FALSE;

		// check able
		if(!m_DmuxChTbl[A_Ch].IsAble)
		{
			m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			return;
		}

		// find block
		Tv_ErrCode	=	m_FsMng->ImmReadBwGoByTime(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl, A_Time);
		if(E_GrErrNone != Tv_ErrCode)
		{
			m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			return;
		}

		while(TRUE)
		{
			// check data not exist
			if(E_GrDvrRecHndlNone == m_DmuxChTbl[A_Ch].ReadCtl.Hndl)
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
				return;
			}

			if(m_DmuxChTbl[A_Ch].ReadCtl.IsFrmRead)
			{
				// check frame
				LcDmuxChFrameStatUpdate(A_Ch);
				return;
			}

			// read next frame
			Tv_ErrCode	=	m_FsMng->ImmReadBwNext(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl);
			if(E_GrErrNone != Tv_ErrCode)
			{
				if(E_GrErrEndOfData == Tv_ErrCode)
				{
					m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatBegin;
				}
				else
				{
					m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
				}
				return;
			}
		}

}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxBwChRunPlay(__u8 A_Ch)
{
	// local -------------------
		__u8	Tv_ErrCode;
	// code --------------------

		// check able
		if((!m_DmuxChTbl[A_Ch].IsAble) || (E_GrDvrPlayStatStop == m_DmuxChTbl[A_Ch].ReadCtl.Stat))
		{
			return;
		}

		// check next frame
		if((!m_DmuxChTbl[A_Ch].ReadCtl.IsFrmRead) ||
			(E_GrDvrPlayStatSeek == m_DmuxChTbl[A_Ch].ReadCtl.Stat) ||
			((E_GrDvrPlayStatPlay == m_DmuxChTbl[A_Ch].ReadCtl.Stat) && m_DmuxChTbl[A_Ch].IsOut))
		{
			// rest out
			m_DmuxChTbl[A_Ch].IsOut	=	FALSE;
			// go next frame
			Tv_ErrCode	=	m_FsMng->ImmReadBwNext(m_DmuxGrpType, &m_DmuxChTbl[A_Ch].ReadCtl);
			if(E_GrErrNone == Tv_ErrCode)
			{
				LcDmuxChFrameStatUpdate(A_Ch);
			}
			else if(E_GrErrEndOfData == Tv_ErrCode)
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatBegin;
			}
			else
			{
				m_DmuxChTbl[A_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatStop;
			}
		}

		return;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxBwGoByTime(Def_GrTime A_Time)
{
	// local -------------------
		__u8	Tv_Ch;
		__u8	Tv_WkIdx;
	// code --------------------
		for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			Tv_Ch		=	Tv_WkIdx;
			if(Tv_WkIdx >= m_DmuxMaxChCnt)
			{
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}

			LcDmuxBwChGoByTime(Tv_Ch, A_Time);
		}

		// update status
		LcDmuxStatusUpdate();
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::LcDmuxBwRunPlay(void)
{
	// local -------------------
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
	// code --------------------
		if((E_GrDvrPlayStatStop == m_DmuxRunStat))
		{
			return;
		}

		for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			// get channel
			Tv_Ch			=	Tv_WkIdx;
			if(Tv_Ch >= m_DmuxMaxChCnt)
			{
				// audio
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}

			LcDmuxBwChRunPlay(Tv_Ch);
		}

		LcDmuxStatusUpdate();

}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Dmux::DmuxReadyFrameRead(Ptr_GrDvrFs6ReadFrameItem A_PtrBufTbl, __u32 A_BufCnt)
{
	// local -------------------
		__u64	Tv_Result;
		__u8	Tv_ErrCode;
		__u8	Tv_Ch;
		__u32	Tv_WkIdx;
		__u32	Tv_FrmSize;
		void*	Tv_PtrBuf;
		Ptr_GrDvrFrmHdBase	Tv_PtrFrm;
		Def_GrTime	Tv_Time;
	// code --------------------
		Tv_Result	=	0;

		if(0 != A_BufCnt)
		{
			Tv_Time		=	0;

			for(Tv_WkIdx = 0; Tv_WkIdx < A_BufCnt; Tv_WkIdx++)
			{
				Tv_Ch				=	A_PtrBufTbl[Tv_WkIdx].Ch;
				Tv_PtrBuf		=	A_PtrBufTbl[Tv_WkIdx].PtrBuf;
				if((E_GrDvrPlayStatPlay == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat) && (!m_DmuxChTbl[Tv_Ch].IsOut))
				{
					Tv_FrmSize	=	m_DmuxChTbl[Tv_Ch].ReadCtl.FrmHd.Base.DataSize + sizeof(St_GrDvrFrmHdUnknown);
					Tv_ErrCode	=	m_FsMng->ImmGroupDataReadByHndl(m_DmuxGrpType, m_DmuxChTbl[Tv_Ch].ReadCtl.Hndl,
						Tv_PtrBuf, Tv_FrmSize);
					if(E_GrErrNone ==  Tv_ErrCode)
					{
						Tv_PtrFrm	=	(Ptr_GrDvrFrmHdBase)Tv_PtrBuf;
						// update last out time
						if(0 == Tv_Time)
						{
							Tv_Time		=	Tv_PtrFrm->TimeSec;
						}
						else
						{
							if(m_IsDmuxDirBack)
							{
								if(Tv_Time > Tv_PtrFrm->TimeSec)
								{
									Tv_Time		=	Tv_PtrFrm->TimeSec;
								}
							}
							else
							{
								if(Tv_Time < Tv_PtrFrm->TimeSec)
								{
									Tv_Time		=	Tv_PtrFrm->TimeSec;
								}
							}
						}
						// update map
						Tv_Result	=	Tv_Result | (__u64)(1 << Tv_Ch);
					}
				}
			}

			if(0 != Tv_Time)
			{
				m_DmuxLastOutTime	=	Tv_Time;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxReadyFrameDone(__u64 A_Map)
{
	// local -------------------
		__u64	Tv_Mask;
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
	// code --------------------

		for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			// get channel
			Tv_Ch			=	Tv_WkIdx;
			if(Tv_Ch >= m_DmuxMaxChCnt)
			{
				// audio
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}

			Tv_Mask	=	(__u64)(1 << Tv_Ch);

			if(0 != (Tv_Mask & A_Map))
			{
				if((E_GrDvrPlayStatPlay == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat) && (!m_DmuxChTbl[Tv_Ch].IsOut))
				{
					m_DmuxChTbl[Tv_Ch].IsOut	=	TRUE;
					m_DmuxChTbl[Tv_Ch].ReadCtl.Stat	=	E_GrDvrPlayStatSeek;

					// update dmux status
					m_DmuxRunStat	=	E_GrDvrPlayStatSeek;
				}
			}
		}
}
//--------------------------------------------------------------------
__u64	Cls_GrDvrFs6Dmux::LcDmuxReadyChMapGet(void)
{
	// local -------------------
		__u64	Tv_Result;
		__u8	Tv_WkIdx;
		__u8	Tv_Ch;
	// code --------------------
		Tv_Result	=	0;

		for(Tv_WkIdx = 0; Tv_WkIdx < (m_DmuxMaxChCnt * 2); Tv_WkIdx++)
		{
			// get channel
			Tv_Ch			=	Tv_WkIdx;
			if(Tv_Ch >= m_DmuxMaxChCnt)
			{
				// audio
				Tv_Ch		=	(Tv_WkIdx - m_DmuxMaxChCnt) + E_GrDvrFs6AdoChBase;
			}
			if((E_GrDvrPlayStatPlay == m_DmuxChTbl[Tv_Ch].ReadCtl.Stat) && (!m_DmuxChTbl[Tv_Ch].IsOut))
			{
				Tv_Result	=	Tv_Result | (__u64)(1 << Tv_Ch);
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrDvrFs6Dmux::DmuxRecGroupSet(__u8 A_RecGrpType)
{
	// local -------------------
		__u32	Tv_WkIdx;
	// code --------------------

		if(2 > A_RecGrpType)
		{
			m_DmuxGrpType		=	A_RecGrpType;
			// reset
			for(Tv_WkIdx = 0; Tv_WkIdx < E_GrDvrFs6MaxChCnt; Tv_WkIdx++)
			{
				m_DmuxChTbl[Tv_WkIdx].IsOut							=	FALSE;
				m_DmuxChTbl[Tv_WkIdx].ReadCtl.Stat			=	E_GrDvrPlayStatStop;

				m_DmuxChTbl[Tv_WkIdx].ReadCtl.Hndl			=	E_GrDvrRecHndlNone;
				m_DmuxChTbl[Tv_WkIdx].ReadCtl.IsFrmRead	=	FALSE;
				m_DmuxChTbl[Tv_WkIdx].ReadCtl.BlkSeq		=	0;
			}
		}
}
//--------------------------------------------------------------------
Ptr_GrDvrFs6ReadCtrl	Cls_GrDvrFs6Dmux::DmuxReadCtrlPtrGet(__u8 A_Ch)
{
	// local -------------------
	// code --------------------
		return	&m_DmuxChTbl[A_Ch].ReadCtl;
}
//--------------------------------------------------------------------
__u8	Cls_GrDvrFs6Dmux::DmuxRunStatGet(void)
{
		return	m_DmuxRunStat;
}
//--------------------------------------------------------------------
