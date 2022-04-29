/*
 w32 file player 
 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>
#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<stdlib.h>
#include	<W32/GrW32PlayM1.h>
#include	<GrDvrDef.h>
#include	<GrImgBase.h>
#include	<GrDrawTool.h>

//====================================================================
//local const

//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var

//====================================================================
//--------------------------------------------------------------------
Cls_GrW32PlayM1::Cls_GrW32PlayM1( Cls_GrDvrRiBase* A_ObjRi ):
Cls_GrDvrPlay()
{
	// local -------------------
		__u32		Tv_WkCh;
		__u32		Tv_Mp4FbufSize;
		WAVEFORMATEX	Tv_AdoInfo;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		m_ObjRi						=	A_ObjRi;

		m_SpdShl					=	0;
		m_SpdShr					=	0;
		m_IsIfrmOnly			=	FALSE;

		m_IsInst					=	FALSE;
		m_IsAdoAble				=	FALSE;
		m_AdoCh						=	0xFF;
		m_IsAdoExit				=	FALSE;

		m_LastUpdtTime		=	GrTimeGet();
		m_LastChkTime			=	m_LastUpdtTime;
		m_IsStepMode			=	FALSE;
		m_TmReq						=	0;
		m_Mp4DecPitch			=	GrDrawCalcPitchByFcc( E_GrFccRGB15, E_GrW32PlayM1MaxResX );

		GrDumyZeroMem( &m_ChMng, sizeof(m_ChMng) );
		GrDumyZeroMem( &m_BufMng, sizeof(m_BufMng) );

		m_Fps							=	30;
		m_FpsCap					=	480;

		//bitmap info
		GrDumyZeroMem( &m_VdecBi, sizeof(m_VdecBi) );
		m_VdecBi.biSize			=	sizeof(m_VdecBi);
		m_VdecBi.biWidth		=	E_GrW32PlayM1MaxResX;
		m_VdecBi.biHeight		=	-E_GrW32PlayM1MaxResY;
		m_VdecBi.biPlanes		=	1;
		m_VdecBi.biBitCount	=	16;
		m_VdecBi.biCompression		=	BI_RGB;

		//mpeg4 codec init
		Tv_Mp4FbufSize		=	m_Mp4DecPitch * E_GrW32PlayM1MaxResY;
		for ( Tv_WkCh=0;Tv_WkCh < E_GrW32PlayM1MaxChCnt;Tv_WkCh++ )
		{
			m_ChMng[Tv_WkCh].PtrFrm		=	(void*)malloc( Tv_Mp4FbufSize );
			m_ChMng[Tv_WkCh].ObjMp4		=	(Cls_GrXvidDec*)new Cls_GrXvidDec();
			m_ChMng[Tv_WkCh].ObjMp4->DecInit( E_GrFccRGB15, 
												E_GrW32PlayM1MaxResX, E_GrW32PlayM1MaxResY,
												m_ChMng[Tv_WkCh].PtrFrm, m_Mp4DecPitch, TRUE );
		}

		//audio codec init
		m_ObjAdec		=	(Cls_GrG723d24*) new Cls_GrG723d24();
		m_ObjAdec->Init();

		Tv_AdoInfo.wFormatTag				=	WAVE_FORMAT_PCM;
		Tv_AdoInfo.nChannels				=	1;
		Tv_AdoInfo.nSamplesPerSec		=	16000;
		Tv_AdoInfo.nAvgBytesPerSec	=	16000 * 2;
		Tv_AdoInfo.nBlockAlign			=	2;
		Tv_AdoInfo.wBitsPerSample		=	16;
		Tv_AdoInfo.cbSize						=	0;	

		if ( MMSYSERR_NOERROR ==	waveOutOpen( &m_HndlAdo, WAVE_MAPPER, &Tv_AdoInfo, 
															(DWORD_PTR)GrW32PlayM1AdoCb, (DWORD_PTR)this, CALLBACK_FUNCTION  ) )
		{
			m_IsAdoAble		=	TRUE;
			//prepare buffer
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoBufCnt;Tv_WkIdx++ )
			{
				m_AdoBufTbl[Tv_WkIdx].IsQued		=	FALSE;
				m_AdoBufTbl[Tv_WkIdx].PtrHdr		=	(WAVEHDR*)malloc( sizeof(WAVEHDR) );
				m_AdoBufTbl[Tv_WkIdx].PtrBuf		=	(void*)malloc( E_GrW32PlayM1AdoBufSize );
				if ( (NULL != m_AdoBufTbl[Tv_WkIdx].PtrHdr) && (NULL != m_AdoBufTbl[Tv_WkIdx].PtrBuf) )
				{
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->lpData		=	(LPSTR)m_AdoBufTbl[Tv_WkIdx].PtrBuf;
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->dwBufferLength	=	E_GrW32PlayM1AdoBufSize;
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->dwBytesRecorded	=	0;
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->dwUser		=	0;
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->dwFlags		=	0;	
					m_AdoBufTbl[Tv_WkIdx].PtrHdr->dwLoops		=	0;
					//do prepare
					if ( MMSYSERR_NOERROR != waveOutPrepareHeader( m_HndlAdo, m_AdoBufTbl[Tv_WkIdx].PtrHdr, sizeof(WAVEHDR) ) )
					{
						//release buffer
						free( m_AdoBufTbl[Tv_WkIdx].PtrHdr );
						m_AdoBufTbl[Tv_WkIdx].PtrHdr		=	NULL;
						free( m_AdoBufTbl[Tv_WkIdx].PtrBuf );
						m_AdoBufTbl[Tv_WkIdx].PtrBuf		=	NULL;
					}
				}
				else
				{
					//release buffer
					if ( NULL != m_AdoBufTbl[Tv_WkIdx].PtrHdr )
					{
						free( m_AdoBufTbl[Tv_WkIdx].PtrHdr );
						m_AdoBufTbl[Tv_WkIdx].PtrHdr		=	NULL;
					}
					if ( m_AdoBufTbl[Tv_WkIdx].PtrBuf )
					{
						free( m_AdoBufTbl[Tv_WkIdx].PtrBuf );
						m_AdoBufTbl[Tv_WkIdx].PtrBuf		=	NULL;
					}
				}
				
			}
			//allocate silent buffer
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoSltCnt;Tv_WkIdx++ )
			{
				m_AdoSilent[Tv_WkIdx].PtrHdr		=	(WAVEHDR*)malloc( sizeof(WAVEHDR) );
				m_AdoSilent[Tv_WkIdx].PtrBuf		=	(void*)malloc( E_GrW32PlayM1AdoBufSize );
				if ( (NULL != m_AdoSilent[Tv_WkIdx].PtrHdr) && (NULL != m_AdoSilent[Tv_WkIdx].PtrBuf) )
				{
					m_AdoSilent[Tv_WkIdx].PtrHdr->lpData		=	(LPSTR)m_AdoSilent[Tv_WkIdx].PtrBuf;
					m_AdoSilent[Tv_WkIdx].PtrHdr->dwBufferLength	=	E_GrW32PlayM1AdoBufSize;
					m_AdoSilent[Tv_WkIdx].PtrHdr->dwBytesRecorded	=	0;
					m_AdoSilent[Tv_WkIdx].PtrHdr->dwUser		=	0;
					m_AdoSilent[Tv_WkIdx].PtrHdr->dwFlags		=	0;
					m_AdoSilent[Tv_WkIdx].PtrHdr->dwLoops		=	0;
					//do prepare
					if ( MMSYSERR_NOERROR != waveOutPrepareHeader( m_HndlAdo, m_AdoSilent[Tv_WkIdx].PtrHdr, sizeof(WAVEHDR) ) )
					{
						//release buffer
						free( m_AdoSilent[Tv_WkIdx].PtrHdr );
						m_AdoSilent[Tv_WkIdx].PtrHdr		=	NULL;
						free( m_AdoSilent[Tv_WkIdx].PtrBuf );
						m_AdoSilent[Tv_WkIdx].PtrBuf		=	NULL;
					}
					else
					{
						GrDumyZeroMem( m_AdoSilent[Tv_WkIdx].PtrBuf, E_GrW32PlayM1AdoBufSize );
					}	
				}
				else
				{
					//release buffer
					if ( NULL != m_AdoSilent[Tv_WkIdx].PtrHdr )
					{
						free( m_AdoSilent[Tv_WkIdx].PtrHdr );
						m_AdoSilent[Tv_WkIdx].PtrHdr		=	NULL;
					}
					if ( m_AdoSilent[Tv_WkIdx].PtrBuf )
					{
						free( m_AdoSilent[Tv_WkIdx].PtrBuf );
						m_AdoSilent[Tv_WkIdx].PtrBuf		=	NULL;
					}
				}
			}

			//queue init
			GrBufQueInit( &m_AdoQue, (void*)m_AdoBufQue, sizeof(St_GrW32PlayM1AdoQue), sizeof(m_AdoBufQue) );

			//send silent
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoSltCnt;Tv_WkIdx++ )
			{
				waveOutWrite( m_HndlAdo, m_AdoSilent[Tv_WkIdx].PtrHdr, sizeof(WAVEHDR) );
			}
		}

}
//--------------------------------------------------------------------
Cls_GrW32PlayM1::~Cls_GrW32PlayM1()
{
	// local -------------------
		__u32		Tv_WkCh;
		__u32		Tv_WkIdx;
	// code --------------------
		for ( Tv_WkCh=0;Tv_WkCh < E_GrW32PlayM1MaxChCnt;Tv_WkCh++ )
		{
			m_ChMng[Tv_WkCh].ObjMp4->DecFinish();
			delete	m_ChMng[Tv_WkCh].ObjMp4;
			free( m_ChMng[Tv_WkCh].PtrFrm );
		}

		if ( m_IsAdoAble )
		{
			m_IsAdoExit		=	TRUE;
			while ( m_IsAdoAble )
			{
				Sleep( 100 );
			}
			waveOutPause( m_HndlAdo );
			waveOutClose( m_HndlAdo );

			for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrW32PlayM1AdoBufCnt;Tv_WkIdx++ )
			{
				//unprepare
				if ( (NULL != m_AdoBufTbl[Tv_WkIdx].PtrHdr) && (NULL != m_AdoBufTbl[Tv_WkIdx].PtrBuf) )
				{
					waveOutUnprepareHeader( m_HndlAdo, m_AdoBufTbl[Tv_WkIdx].PtrHdr, sizeof(WAVEHDR) );
				}
				
				//release buffer
				if ( NULL != m_AdoBufTbl[Tv_WkIdx].PtrHdr )
				{
					free( m_AdoBufTbl[Tv_WkIdx].PtrHdr );
					m_AdoBufTbl[Tv_WkIdx].PtrHdr		=	NULL;
				}
				if ( m_AdoBufTbl[Tv_WkIdx].PtrBuf )
				{
					free( m_AdoBufTbl[Tv_WkIdx].PtrBuf );
					m_AdoBufTbl[Tv_WkIdx].PtrBuf		=	NULL;
				}
			}
			//silent buffer
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoSltCnt;Tv_WkIdx++ )
			{
				if ( (NULL != m_AdoSilent[Tv_WkIdx].PtrHdr) && (NULL != m_AdoSilent[Tv_WkIdx].PtrBuf) )
				{
					waveOutUnprepareHeader( m_HndlAdo, m_AdoSilent[Tv_WkIdx].PtrHdr, sizeof(WAVEHDR) );
				}
				if ( NULL != m_AdoSilent[Tv_WkIdx].PtrHdr )
				{
					free( m_AdoSilent[Tv_WkIdx].PtrHdr );
					m_AdoSilent[Tv_WkIdx].PtrHdr		=	NULL;
				}
				if ( m_AdoSilent[Tv_WkIdx].PtrBuf )
				{
					free( m_AdoSilent[Tv_WkIdx].PtrBuf );
					m_AdoSilent[Tv_WkIdx].PtrBuf		=	NULL;
				}
			}
		}

		delete	m_ObjAdec;
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::SetPlaySpeed( __s32 A_Speed )
{
	// local -------------------
	__s32		Tv_Speed;		
	// code --------------------
	//init
	//check able speed
	Tv_Speed		=	A_Speed;
	if ( E_GrW32PlayM1SpdMax < Tv_Speed )
	{
		Tv_Speed		=	E_GrW32PlayM1SpdMax;
	}
	if ( E_GrW32PlayM1SpdMin > Tv_Speed )
	{
		Tv_Speed		=	E_GrW32PlayM1SpdMin;
	}
	Cls_GrDvrPlay::SetPlaySpeed( Tv_Speed );
	if ( 0 > m_PlaySpeed )
	{
		m_SpdShl			=	0;
		m_SpdShr			=	(__u32)(-m_PlaySpeed);
	}
	else
	{
		m_SpdShl			=	(__u32)(m_PlaySpeed);
		m_SpdShr			=	0;
	}

	LcUpdtPlaySpd();
	m_TmReq		=	m_TmPlay;			//match play time
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LoadChuk( Def_GrTime A_Time )
{
	// local -------------------
	BOOL8		Tv_Result;
	__u32		Tv_FindType;
	__u32		Tv_WkIdx;
	Ptr_GrDvrFls16v1FrmHd	Tv_PtrFrm;
	// code --------------------
	//init
	Tv_Result		=	FALSE;
	if ( m_IsDirBack )	
	{	Tv_FindType	=	E_GrTimeFindRngSmall;	}
	else	
	{	Tv_FindType	=	E_GrTimeFindRngGreate;	}

	//check exist chunk - first
	if ( 0 ==  m_BufMng.FrmCnt )
	{	Tv_FindType	=	Tv_FindType | E_GrTimeFindRngIn;	}

	//chunk load
	if ( m_ObjRi->RecReadChunk( m_IsInst, A_Time, Tv_FindType, 
				&m_BufMng.FrmCnt, (void*)m_BufMng.Buf,FALSE ) )
	{
		//check over frame count
		if ( E_GrW32PlayM1MaxFrmCnt < m_BufMng.FrmCnt )
		{
			m_BufMng.FrmCnt	=	E_GrW32PlayM1MaxFrmCnt;
		}
		//check frame count
		if ( 0 != m_BufMng.FrmCnt )
		{
			//buld frame pointer table
			Tv_PtrFrm		=	(Ptr_GrDvrFls16v1FrmHd)m_BufMng.Buf;
			for ( Tv_WkIdx=0;Tv_WkIdx < m_BufMng.FrmCnt;Tv_WkIdx++ )
			{
				//set frame
				m_BufMng.PtrFrms[Tv_WkIdx]	=	Tv_PtrFrm;
				//next
				Tv_PtrFrm	=	(Ptr_GrDvrFls16v1FrmHd)( (__u32)Tv_PtrFrm + Tv_PtrFrm->Size );
			}

			Tv_Result		=	TRUE;
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LcGoFrmByTime( Def_GrTime A_Time )
{
	// local -------------------
	BOOL8		Tv_Result;
	__u32		Tv_WkIdx;
	// code --------------------
	//init
	Tv_Result		=	FALSE;

	//check exist frama
	if ( 0 != m_BufMng.FrmCnt )
	{
		if ( m_IsDirBack )
		{
			Tv_WkIdx		=	m_BufMng.FrmCnt;
			do 
			{
				//udpate index
				Tv_WkIdx --;
				//check time
				if ( m_BufMng.PtrFrms[Tv_WkIdx]->Time <= A_Time )
				{
					m_PlayFrmIdx	=	Tv_WkIdx;
					Tv_Result			=	TRUE;
					break;
				}
			} while( 0 != Tv_WkIdx );
		}
		else
		{
			for ( Tv_WkIdx = 0;Tv_WkIdx < m_BufMng.FrmCnt;Tv_WkIdx++ )
			{
				//check time
				if ( m_BufMng.PtrFrms[Tv_WkIdx]->Time >= A_Time )
				{
					m_PlayFrmIdx	=	Tv_WkIdx;
					Tv_Result			=	TRUE;
					break;
				}
			}
		}
	}

	return	Tv_Result;

}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LcGoNextFrm( void )
{
	// local -------------------
	BOOL8		Tv_Result;
	Def_GrTime	Tv_Time;
	// code --------------------
	//init
	Tv_Result		=	TRUE;

	//check direction
	if ( m_IsDirBack )
	{
		//back play
		//chekc finish
		if ( 0 == m_PlayFrmIdx )
		{
			//over , load next
			Tv_Time		=	m_TmReq;
			if ( 0 != m_BufMng.FrmCnt )
			{
				Tv_Time		=	m_BufMng.PtrFrms[0]->Time;
			}

			if ( LoadChuk( Tv_Time ) )
			{
				m_PlayFrmIdx		=	m_BufMng.FrmCnt - 1;
			}
			else
			{
				//not play , stop
				m_PlayFrmIdx			=	0;			//init play index
				m_BufMng.FrmCnt		=	0;			//clear buffer
				m_IsPlay					=	FALSE;	//stop
				Tv_Result					=	FALSE;
			}
		}
		else
		{
			m_PlayFrmIdx --;
		}
	}
	else
	{
		//normal play
		//check finish
		if ( m_BufMng.FrmCnt <= (m_PlayFrmIdx + 1) )
		{
			//over , load next
			Tv_Time		=	m_TmReq;
			if ( 0 != m_BufMng.FrmCnt )
			{
				Tv_Time		=	m_BufMng.PtrFrms[m_BufMng.FrmCnt - 1]->Time;
			}
			if ( LoadChuk( Tv_Time ) )
			{
				m_PlayFrmIdx		=	0;
			}
			else
			{
				//not play , stop
				m_PlayFrmIdx					=	0;			//init play index
				m_BufMng.FrmCnt		=	0;			//clear buffer
				m_IsPlay					=	FALSE;	//stop
				Tv_Result					=	FALSE;
			}
		}
		else
		{
			m_PlayFrmIdx ++;
		}
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LcGetNowFrmTime( Ptr_GrTime A_PtrRtTime )
{
	// local -------------------
	BOOL8		Tv_Result;
	// code --------------------
	//init
	Tv_Result		=	FALSE;

	//check exist frame
	if ( m_PlayFrmIdx < m_BufMng.FrmCnt )
	{
		*A_PtrRtTime	=	m_BufMng.PtrFrms[m_PlayFrmIdx]->Time;
		Tv_Result			=	TRUE;
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LcDecNowFrm( BOOL8* A_PtrIsDec, __u32* A_PtrRtVdoCh )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrDvrFls16v1VfrmHd	Tv_PtrVfrm;
		BOOL8		Tv_IsDecAble;
		__u32		Tv_Ch;
		BOOL8		Tv_IsSkip;
		void*		Tv_PtrData;
		BOOL8		Tv_IsMp4Dec;
		BOOL8		Tv_IsInterace;
	// code --------------------
		//init
		Tv_Result				=	TRUE;
		*A_PtrIsDec			=	FALSE;
		*A_PtrRtVdoCh		=	E_GrW32PlayM1BadCh;
		//check play able index
		if ( m_PlayFrmIdx < m_BufMng.FrmCnt )
		{
			//check frame type
			if ( E_GrDvrFls16v1VdoFccId == m_BufMng.PtrFrms[m_PlayFrmIdx]->FccId )
			{
				//decode video frame
				//init
				Tv_PtrVfrm	=	(Ptr_GrDvrFls16v1VfrmHd)m_BufMng.PtrFrms[m_PlayFrmIdx];
				Tv_Ch		=	Tv_PtrVfrm->Ch;
				if ( m_ChMng[Tv_Ch].IsPlayOn )
				{
					//check change option
					if (	(m_ChMng[Tv_Ch].ResX != Tv_PtrVfrm->ResX) || 
						(m_ChMng[Tv_Ch].ResY != Tv_PtrVfrm->ResY) ||
						(m_ChMng[Tv_Ch].Fps  != Tv_PtrVfrm->Fps)  )
					{
						//update funciton
						m_ChMng[Tv_Ch].ResX = Tv_PtrVfrm->ResX;
						m_ChMng[Tv_Ch].ResY = Tv_PtrVfrm->ResY;
						m_ChMng[Tv_Ch].Fps  = Tv_PtrVfrm->Fps;
						LcUpdtPlaySpd();
					}

					//check i frame decode mode
					Tv_IsDecAble		=	TRUE;
					if (m_IsIfrmOnly && ( E_GrDvrVencGopTypeI != Tv_PtrVfrm->GopType ) )
					{
						Tv_IsDecAble	=	FALSE;
					}
					if (m_IsDirBack && ( E_GrDvrVencGopTypeI != Tv_PtrVfrm->GopType ) )
					{
						Tv_IsDecAble	=	FALSE;
					}
					if ( (!m_ChMng[Tv_Ch].IsDecoded) && (E_GrDvrVencGopTypeI != Tv_PtrVfrm->GopType) )
					{
						Tv_IsDecAble	=	FALSE;
					}
					if ( Tv_IsDecAble )
					{
						Tv_IsSkip			=	FALSE;
						//check need skip
						if ( m_ChMng[Tv_Ch].SkipPos < m_ChMng[Tv_Ch].SkipCnt )
						{
							m_ChMng[Tv_Ch].SkipPos ++;
							Tv_IsSkip		=	TRUE;
						}
						else
						{
							m_ChMng[Tv_Ch].SkipPos		=	0;
						}

						//decode video
						Tv_IsInterace		=	FALSE;
						if ( 288 < Tv_PtrVfrm->ResY )
						{
							Tv_IsInterace		=	TRUE;
						}
						Tv_PtrData	=	(void*)( (__u32)Tv_PtrVfrm + sizeof(St_GrDvrFls16v1VfrmHd) );
						Tv_Result		=	m_ChMng[Tv_Ch].ObjMp4->DecFrame( Tv_PtrData, Tv_PtrVfrm->CodeSize, 
													Tv_PtrVfrm->ResX, Tv_PtrVfrm->ResY, Tv_IsInterace,
													&Tv_IsMp4Dec );
						if ( Tv_Result )
						{
							*A_PtrIsDec		=	TRUE;
							*A_PtrRtVdoCh	=	Tv_Ch;
							m_ChMng[Tv_Ch].IsDecoded	=	TRUE;
							//update play time
							m_TmPlay		=	Tv_PtrVfrm->Base.Time;
						}
					}
					//update record type
					m_ChMng[Tv_Ch].RecType		=	Tv_PtrVfrm->RecType;
				}
			}
			else if ( E_GrDvrFls16v1AdoFccId == m_BufMng.PtrFrms[m_PlayFrmIdx]->FccId )
			{
				if ( !m_IsDirBack && (0 == m_PlaySpeed) )
				{
					//audio decode 
					Tv_Result	=	LcAdoDec( (Ptr_GrDvrFls16v1AfrmHd)m_BufMng.PtrFrms[m_PlayFrmIdx] );
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrW32PlayM1::LcUpdtPlaySpd( void )
{
	// local -------------------
	__u32			Tv_TtFps;				//total fps
	__u32			Tv_Ch;
	__u32			Tv_Fps;
	// code --------------------

	//get total fps
	Tv_TtFps		=	0;
	for ( Tv_Ch = 0;Tv_Ch < E_GrW32PlayM1MaxChCnt;Tv_Ch++ )
	{
		if ( m_ChMng[Tv_Ch].IsPlayOn  )
		{
			Tv_Fps		=	m_ChMng[Tv_Ch].Fps;
			if ( 352 < m_ChMng[Tv_Ch].ResX )
			{
				Tv_Fps		=	Tv_Fps * 2;
			}
			if ( 288 < m_ChMng[Tv_Ch].ResY )
			{
				Tv_Fps		=	Tv_Fps * 2;
			}
			Tv_Fps			=	Tv_Fps << m_SpdShl;
			Tv_Fps			=	Tv_Fps >> m_SpdShr;
			Tv_TtFps		=	Tv_TtFps + Tv_Fps;
		}
	}

	//check need i frame only
	if ( m_FpsCap < Tv_TtFps )
	{
		m_IsIfrmOnly		=	TRUE;
	}
	else
	{
		m_IsIfrmOnly		=	FALSE;
	}

	//set skip count
	if ( m_IsIfrmOnly )
	{
		for ( Tv_Ch = 0;Tv_Ch < E_GrW32PlayM1MaxChCnt;Tv_Ch++ )
		{
			m_ChMng[Tv_Ch].SkipCnt		=	0;
			m_ChMng[Tv_Ch].SkipPos		=	0;
		}
	}
	else
	{
		for ( Tv_Ch = 0;Tv_Ch < E_GrW32PlayM1MaxChCnt;Tv_Ch++ )
		{
			//init
			m_ChMng[Tv_Ch].SkipPos		=	0;
			m_ChMng[Tv_Ch].SkipCnt		=	0;
			//calculate skip count
			Tv_Fps		=	(__u32)(m_ChMng[Tv_Ch].Fps) << m_SpdShl;
			Tv_Fps		=	(__u32)(m_ChMng[Tv_Ch].Fps) >> m_SpdShr;
			if ( m_Fps < Tv_Fps  )
			{
				m_ChMng[Tv_Ch].SkipCnt	=	(__u8)(Tv_Fps / m_Fps);
			}
		}
	}

}
//--------------------------------------------------------------------
__u32		Cls_GrW32PlayM1::RtlUpdt( Def_GrTime A_Time )
{
	// local -------------------
		__u32		Tv_Result;
		Def_GrTime	Tv_TimeGap;
		Def_GrTime	Tv_TmFrm;
		BOOL8		Tv_IsDec;
		BOOL8		Tv_IsTimeRun;
		BOOL8		Tv_IsRunDec;
	// code --------------------
		//init
		Tv_Result		=	E_GrW32PlayM1BadCh;
		//check is play run
		if ( m_IsPlay )
		{
			//get last frame
			if ( LcGetNowFrmTime( &Tv_TmFrm ) )
			{
				//init
				Tv_IsTimeRun		=	TRUE;
				if ( m_IsStepMode )
				{
					Tv_IsDec		=	TRUE;
					m_TmReq			=	Tv_TmFrm;
				}
				else
				{
					//check dec able
					Tv_IsDec		=	FALSE;
					if ( m_IsDirBack )
					{
						if ( m_TmReq <= Tv_TmFrm )
						{
							Tv_IsDec		=	TRUE;
						}
						else
						{
							if ( E_GrW32PlayM1WaitLim <= (m_TmReq - Tv_TmFrm) )
							{
								m_TmReq					=	Tv_TmFrm;
								m_LastUpdtTime	=	A_Time;
								Tv_IsDec				=	TRUE;
							}
						}
					}
					else
					{
						if ( m_TmReq >= Tv_TmFrm )
						{
							Tv_IsDec		=	TRUE;
						}
						else
						{
							if ( E_GrW32PlayM1WaitLim <= (Tv_TmFrm - m_TmReq) )
							{
								m_TmReq					=	Tv_TmFrm;
								m_LastUpdtTime	=	A_Time;
								Tv_IsDec				=	TRUE;
							}
						}
					}
				}

				//decode
				if ( Tv_IsDec )
				{
					if ( LcDecNowFrm(&Tv_IsRunDec, &Tv_Result ) )
					{
						//go next frame
						LcGoNextFrm();
						if ( Tv_IsRunDec && m_IsStepMode )
						{
							m_IsPlay		=	FALSE;
						}
					}
					else
					{
						Tv_IsTimeRun	=	FALSE;
					}
				}

				//next time
				if ( (!m_IsStepMode) && Tv_IsTimeRun )
				{
					//calculate time gap
					Tv_TimeGap	=	A_Time - m_LastUpdtTime;
					Tv_TimeGap	=	Tv_TimeGap << m_SpdShl;
					Tv_TimeGap	=	Tv_TimeGap >> m_SpdShr;
					//check exist next frame
					if ( m_IsDirBack )
					{
						m_TmReq		=	m_TmReq - Tv_TimeGap;
					}
					else
					{
						m_TmReq		=	m_TmReq + Tv_TimeGap;
					}
					//update time
					m_LastUpdtTime	=	A_Time;	
				}
				else
				{
					//m_LastUpdtTime	=	A_Time;
				}
			}
			else
			{
				//chunk load
				if ( LoadChuk( m_TmReq ) )
				{
					//sueecss - go frame position
					m_PlayFrmIdx		=	0;		//init play index
					LcGoFrmByTime( m_TmReq );
					m_LastUpdtTime	=	A_Time;
				}
				else
				{
					m_PlayFrmIdx			=	0;			//init play index
					m_BufMng.FrmCnt		=	0;			//clear buffer
					m_IsPlay					=	FALSE;	//stop
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrW32PlayM1::LcDecFlagsReset( void )
{
	// local -------------------
	__u32		Tv_WkIdx;
	// code --------------------
	for ( Tv_WkIdx = 0;Tv_WkIdx < E_GrW32PlayM1MaxChCnt;Tv_WkIdx++ )
	{
		m_ChMng[Tv_WkIdx].IsDecoded		=	FALSE;
		m_ChMng[Tv_WkIdx].RecType			=	0;
	}

}
//--------------------------------------------------------------------
__u8	Cls_GrW32PlayM1::GetChRecType(__u32 A_Ch)
{
	return	m_ChMng[A_Ch].RecType;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrW32PlayM1::ShowNextFrame( void )
{
	// local -------------------
	BOOL8		Tv_Result;
	// code --------------------
	//init
	Tv_Result		=	FALSE;

	//check step able
	if ( !m_IsPlay )
	{
		m_IsStepMode		=	TRUE;
		m_IsPlay				=	TRUE;
		Tv_Result				=	TRUE;
	}

	return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::SetPlayPos( Def_GrTime A_Time )
{
	//free frame buffer
	m_TmPlay					=	A_Time;
	m_TmReq						=	A_Time;
	m_BufMng.FrmCnt		=	0;		//clear buffer
	m_LastUpdtTime		=	GrTimeGet();
	//update all redecode
	LcDecFlagsReset();
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::SetInstMode( BOOL8 A_IsInst )
{
	if ( m_IsInst != A_IsInst )
	{
		m_IsInst		= A_IsInst;
		m_BufMng.FrmCnt		=	0;		//clear buffer
		m_LastUpdtTime		=	GrTimeGet();
		//update all redecode
		LcDecFlagsReset();
	}
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::Play( BOOL8 A_IsRun )
{
	m_IsPlay			=	A_IsRun;
	if ( m_IsPlay )
	{
		m_IsStepMode		= FALSE;
		m_LastUpdtTime	=	GrTimeGet();
	}
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::SetChPlayOn( __u32 A_Ch, BOOL8 A_IsOn )
{
	if ( !m_ChMng[A_Ch].IsPlayOn )
	{
		m_ChMng[A_Ch].IsDecoded	=	FALSE;
		m_ChMng[A_Ch].RecType		=	0;
	}
	m_ChMng[A_Ch].IsPlayOn		=	A_IsOn;
	LcUpdtPlaySpd();
}
//--------------------------------------------------------------------
void	Cls_GrW32PlayM1::SetPlayDir( BOOL8 A_IsBack )
{
	if ( m_IsDirBack != A_IsBack )
	{
		Cls_GrDvrPlay::SetPlayDir( A_IsBack );
		//reset frame
		LcDecFlagsReset();
		//sync play time
		m_TmReq			=	m_TmPlay;
	}
}
//--------------------------------------------------------------------
BITMAPINFO*		Cls_GrW32PlayM1::GetBitmapInfo( void )
{
		return	(BITMAPINFO*)&m_VdecBi;
}
//--------------------------------------------------------------------
void		Cls_GrW32PlayM1::GetChRes( __u32 A_Ch, __u16* A_PtrRtResX, __u16* A_PtrRtResY )
{
		*A_PtrRtResX		=	m_ChMng[A_Ch].ResX;
		*A_PtrRtResY		=	m_ChMng[A_Ch].ResY;
}
//--------------------------------------------------------------------
void*		Cls_GrW32PlayM1::GetChDib( __u32 A_Ch )
{
		return	m_ChMng[A_Ch].PtrFrm;
}
//--------------------------------------------------------------------
void CALLBACK GrW32PlayM1AdoCb( HWAVEOUT A_Hndl, UINT A_Type, DWORD_PTR A_Obj,  
															 __u32 A_Parm1, __u32 A_Parm2 )
{
	// local -------------------
		Cls_GrW32PlayM1*	Tv_Obj;
	// code --------------------
		//check data mode
		if ( WOM_DONE == A_Type ) 
		{
			//do runtime
			Tv_Obj		=	(Cls_GrW32PlayM1*)A_Obj;
			Tv_Obj->RtlAdoCb( (WAVEHDR*)A_Parm1 );
		}
}
//--------------------------------------------------------------------
Ptr_GrW32PlayM1AdoItm	Cls_GrW32PlayM1::LcAdoGetBufInsAble( void )
{
	// local -------------------
		Ptr_GrW32PlayM1AdoItm	Tv_Result;
		__u32		Tv_WkIdx;
	// code --------------------
		//init
		Tv_Result		=	NULL;

		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoBufCnt;Tv_WkIdx++ )
		{
			if (	(NULL != m_AdoBufTbl[Tv_WkIdx].PtrHdr) && (NULL != m_AdoBufTbl[Tv_WkIdx].PtrBuf) && 
						(!m_AdoBufTbl[Tv_WkIdx].IsQued) )
			{
				//finded
				Tv_Result		=	&m_AdoBufTbl[Tv_WkIdx];
				break;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrW32PlayM1::LcAdoDec( Ptr_GrDvrFls16v1AfrmHd A_PtrFrm )
{
	// local -------------------
		BOOL8		Tv_Result;
		Ptr_GrW32PlayM1AdoItm	Tv_PtrBuf;
		void*		Tv_PtrData;
		__u32		Tv_WkIdx;
		__u32		Tv_ChMask;
		Ptr_GrW32PlayM1AdoQue	Tv_PtrQue;
	// code --------------------
		//init
		Tv_Result		=	TRUE;
		//check play able
		if ( (E_GrW32PlayM1MaxChCnt > m_AdoCh) && m_IsAdoAble )
		{
			Tv_ChMask		=	1;
			Tv_PtrData	=	(void*)( (__u32)A_PtrFrm + sizeof(St_GrDvrFls16v1AfrmHd) );
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1MaxChCnt;Tv_WkIdx++ )
			{
				//check exist channel
				if ( 0 != ( Tv_ChMask  & A_PtrFrm->ChMask ) )
				{
					//check match channel
					if ( m_AdoCh == Tv_WkIdx )
					{
						//check able frame
						Tv_PtrBuf		=	LcAdoGetBufInsAble();
						if ( NULL != Tv_PtrBuf )
						{
							//get queue
							Tv_PtrQue		=	(Ptr_GrW32PlayM1AdoQue)GrBufQueGetInPtr( &m_AdoQue );
							if ( NULL != Tv_PtrQue )
							{
								Tv_PtrQue->PtrItm		=	Tv_PtrBuf;
								m_ObjAdec->SetSrcData( Tv_PtrData, E_GrW32PlayM1AdoChDataSize );
								m_ObjAdec->SetDestBuf( Tv_PtrBuf->PtrBuf, E_GrW32PlayM1AdoBufSize );
								m_ObjAdec->Decode();
								Tv_PtrBuf->IsQued		=	TRUE;
								waveOutWrite( m_HndlAdo, Tv_PtrBuf->PtrHdr, sizeof(WAVEHDR) );
								GrBufQueIn( &m_AdoQue );
								Tv_Result		=	TRUE;
							}
							else
							{
								Tv_Result		=	FALSE;
							}
						}
						else
						{
							Tv_Result		=	FALSE;
						}
						//find channel
						break;
					}
					//next data
					Tv_PtrData	=	(void*)( (__u32)Tv_PtrData + E_GrW32PlayM1AdoChDataSize );
				}
				Tv_ChMask		=	Tv_ChMask << 1;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrW32PlayM1::RtlAdoCb( WAVEHDR* A_PtrWhdr )
{
	// local -------------------
		__u32		Tv_WkIdx;
		Ptr_GrW32PlayM1AdoQue	Tv_PtrQue;
	// code --------------------
		if ( !m_IsAdoExit )
		{
			//release queue
			for ( Tv_WkIdx=0;Tv_WkIdx < E_GrW32PlayM1AdoBufCnt;Tv_WkIdx++ )
			{
				if ( A_PtrWhdr == m_AdoBufTbl[Tv_WkIdx].PtrHdr )
				{
					//finded
					m_AdoBufTbl[Tv_WkIdx].IsQued		=	FALSE;
					break;
				}
			}
			//get new data
			Tv_PtrQue		=	(Ptr_GrW32PlayM1AdoQue)GrBufQueGetOutPtr( &m_AdoQue );
			if ( NULL != Tv_PtrQue )
			{
				//waveOutWrite( m_HndlAdo, Tv_PtrQue->PtrItm->PtrHdr, sizeof(WAVEHDR) );
				GrBufQueOut( &m_AdoQue );
			}
			else
			{
				//send silent
				waveOutWrite( m_HndlAdo, m_AdoSilent[0].PtrHdr, sizeof(WAVEHDR) );
			}
		}
		else
		{
			m_IsAdoAble		=	FALSE;
		}
}
//--------------------------------------------------------------------
void		Cls_GrW32PlayM1::SetAdoCh( __u32 A_Ch )
{
		m_AdoCh		=	A_Ch;
}
//--------------------------------------------------------------------


