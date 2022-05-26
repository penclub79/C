/*
	avi tool

*/

//====================================================================
//include
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrAviTool.h>
#include <GrBmpTool.h>
#include <GrDumyTool.h>
#include <GrImgBase.h>
#include <GrStrTool.h>
#include <stdlib.h>

//====================================================================
//local const

#define E_GrAviHdBufSize		0x1800		/* header buffer size */
#define E_GrAviHdOdmlOfs		0x10F4		/* header buffer size */
#define E_GrAviHdOdmlSize		0x10C

#define E_GrAviStrfTypeNone	0
#define E_GrAviStrfTypeVdo	1
#define E_GrAviStrfTypeAdo	2

//====================================================================
//local type

//====================================================================
//global var

__u8	V_GrAviVol[E_GrAviVolSize]	= {	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x20, 
													0x02, 0x48, 0x0d, 0xc0, 0x00, 0x40, 0x00, 0x40,
													0x00, 0x40, 0x00, 0x80, 0x00, 0x97, 0x53, 0x0a, 
													0x2c, 0x08, 0x3c, 0x28, 0x8c, 0x1f };

//====================================================================
//functions

//--------------------------------------------------------------------
Cls_GrAviTool::Cls_GrAviTool( Cls_GrFileCtrl* A_ObjFile )
{
		m_ObjFl		=	A_ObjFile;
		m_IsOpen	=	FALSE;
		m_IsWrite	=	FALSE;


		m_PtrWtHd		=	NULL;
		m_PtrWtIdx1	=	NULL;

		GrDumyCopyMem( (void*)m_VolHd, (void*)V_GrAviVol, sizeof(V_GrAviVol) );
}
//--------------------------------------------------------------------
Cls_GrAviTool::~Cls_GrAviTool()
{
		FinishWrite();
		FinishRead();
}
//--------------------------------------------------------------------
void	Cls_GrAviTool::LcPtcG723( void )
{
	// local -------------------
		__u32		Tv_AddSize;
		Ptr_GrAviWfextG723	Tv_PtrExt;
	// code --------------------
		//init
		Tv_AddSize		=	sizeof(St_GrAviWfextG723);
		//patch
		m_PtrWtHd->Hdrl.Hd.Size		=	m_PtrWtHd->Hdrl.Hd.Size + Tv_AddSize;
		m_PtrWtHd->Hdrl.StrlAdo.Hd.Size	=	m_PtrWtHd->Hdrl.StrlAdo.Hd.Size + Tv_AddSize;
		m_PtrWtHd->Hdrl.StrlAdo.Strf.Hd.Size	=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Hd.Size + Tv_AddSize;
		//m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.Size	=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.Size + (__u16)Tv_AddSize;

		//add extra
		Tv_PtrExt	=	(Ptr_GrAviWfextG723)( (__u32)m_PtrWtHd + sizeof(St_GrAviFhRiff) );
		Tv_PtrExt->ExtSize		=	2;
		Tv_PtrExt->AuxBlkSize	=	0;
}
//--------------------------------------------------------------------
void	Cls_GrAviTool::LcPatch( __u32 A_FccVdo, __u16 A_AdoId )
{
	// local -------------------
	// code --------------------
		switch( A_AdoId )
		{
			case E_GrAviFccG723Antex:
			case E_GrAviFccG723Ms:
			case E_GrAviFccG723Lucent:
			case E_GrAviFccG723Msonic:
			case E_GrAviFccG723Vivo:
			case E_GrAviFccG723Dec:
				LcPtcG723();
				break;
		}
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::StartWrite(	__u32 A_FccVdo, __u32 A_Fps, __u32 A_ResX, __u32 A_ResY,
																	__u16 A_AdoId, __u32 A_ChCnt, __u32 A_SmpRate, __u32 A_SmpBit, 
																	__u32 A_AdoBlkUnit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check allocated memory
		if ( (!m_IsOpen) && (m_ObjFl->IsOpened()) )
		{
			//malloc buffer
			m_PtrWtHd		=	(Ptr_GrAviFhRiff)malloc( E_GrAviHdBufSize );
			m_PtrWtIdx1	=	(Ptr_GrAviIdx1)malloc( sizeof(St_GrAviIdx1) * E_GrAviMaxFrmCnt );
			if ( (NULL != m_PtrWtIdx1) && (NULL != m_PtrWtHd) )
			{
				//clear
				GrDumyZeroMem( m_PtrWtHd, E_GrAviHdBufSize );

				m_AdoBlkSize		=	A_AdoBlkUnit;
				m_AdoSmpRate		=	A_SmpRate;
				m_AdoSmpBit			=	A_SmpBit;
				m_WtmVal				=	0;

				//initialize
				m_PtrWtHd->Hd.FccMain		=	E_GrAviFccRiff;
				m_PtrWtHd->Hd.Size			=	E_GrAviHdBufSize;
				m_PtrWtHd->Hd.FccSub		=	E_GrAviFccAvi;

				m_PtrWtHd->Hdrl.Hd.FccMain	=	E_GrAviFccList;
				m_PtrWtHd->Hdrl.Hd.Size			=	sizeof(St_GrAviFhHdrl) - 8;
				m_PtrWtHd->Hdrl.Hd.FccSub		=	E_GrAviFccHdrl;

				m_PtrWtHd->Hdrl.Avih.Hd.Fcc		=	E_GrAviFccAvih;
				m_PtrWtHd->Hdrl.Avih.Hd.Size	=	sizeof(St_GrAviMainHd);

				m_PtrWtHd->Hdrl.Avih.Ct.FrmStepUs		=	1000000 / A_Fps;
				m_PtrWtHd->Hdrl.Avih.Ct.Flag	=	E_GrAviMhHasIdx;	// | E_GrAviMhInterleave | E_GrAviMhCktype;	
				//m_PtrWtHd->Hdrl.Avih.Ct.FrmCnt	=	
				m_PtrWtHd->Hdrl.Avih.Ct.StrmCnt	=	2;
				m_PtrWtHd->Hdrl.Avih.Ct.ResX	=	A_ResX;
				m_PtrWtHd->Hdrl.Avih.Ct.ResY	=	A_ResY;

				m_PtrWtHd->Hdrl.StrlVdo.Hd.FccMain	=	E_GrAviFccList;
				m_PtrWtHd->Hdrl.StrlVdo.Hd.Size			=	sizeof(St_GrAviFhStrlVdo) - 8;
				m_PtrWtHd->Hdrl.StrlVdo.Hd.FccSub		=	E_GrAviFccStrl;

				m_PtrWtHd->Hdrl.StrlVdo.Strh.Hd.Fcc		=	E_GrAviFccStrh;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Hd.Size	=	sizeof(St_GrAviStrmHd);
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.FccType		=	E_GrAviFccVids;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.FccCodec	=	A_FccVdo;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.Scale			=	1;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.Rate			=	A_Fps;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.Quality		=	10000;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.right			=	(__s16)A_ResX;
				m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.bottom		=	(__s16)A_ResY;

				m_PtrWtHd->Hdrl.StrlVdo.Strf.Hd.Fcc		=	E_GrAviFccStrf;
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Hd.Size	=	sizeof(St_GrBmpIfHd);
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Size	=	sizeof(St_GrBmpIfHd);
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Width		=	A_ResX;
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Height	=	A_ResY;	
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Planes	=	1;
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.BitCount	=	24;
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Compression	=	A_FccVdo;
				m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.SizeImage	=	A_ResX * A_ResY * 6;	

				m_PtrWtHd->Hdrl.StrlAdo.Hd.FccMain	=	E_GrAviFccList;
				m_PtrWtHd->Hdrl.StrlAdo.Hd.Size			=	sizeof(St_GrAviFhStrlAdo) - 8;
				m_PtrWtHd->Hdrl.StrlAdo.Hd.FccSub		=	E_GrAviFccStrl;

				m_PtrWtHd->Hdrl.StrlAdo.Strh.Hd.Fcc		=	E_GrAviFccStrh;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Hd.Size	=	sizeof(St_GrAviStrmHd);
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.FccType		=	E_GrAviFccAuds;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.FccCodec	=	A_AdoId;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.Scale			=	1;	//A_AdoBlkUnit;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.Rate			=	A_SmpRate;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.Quality		=	10000;
				m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.SmpSize		=	(A_ChCnt * A_SmpBit) / 8;

				m_PtrWtHd->Hdrl.StrlAdo.Strf.Hd.Fcc			=	E_GrAviFccStrf;
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Hd.Size		=	sizeof(St_GrAviWaveformatEx);

				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.FormatId	=	A_AdoId;
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.ChCnt			=	(__u16)A_ChCnt;
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.SmpRate		=	A_SmpRate;
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.TransSize	=	(__u16)(((A_ChCnt * A_SmpBit) / 8) * A_SmpRate);
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.Align			=	(__u16)((A_ChCnt * A_SmpBit) / 8);
				m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.SmpBit		=	(__u16)A_SmpBit;
				//m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.Size			=	0;

				//segm
				//m_PtrWtHd->Hdrl.Segm.Hd.Fcc			=	E_GrAviFccSegm;
				//m_PtrWtHd->Hdrl.Segm.Hd.Size		=	1;

				//init variable
				m_VfrmMaxSize		=	0;
				m_VfrmCnt				=	0;
				m_AfrmMaxSize		=	0;
				m_AfrmCnt				=	0;
				m_AviSize				=	E_GrAviHdBufSize;

				//set file pointer
				m_ObjFl->Seek( 0 );
				//write not completed header
				if ( E_GrAviHdBufSize == m_ObjFl->Write( (void*)m_PtrWtHd, E_GrAviHdBufSize ) )
				{
					//make movi packet
					m_WtMovi.FccMain		=	E_GrAviFccList;
					m_WtMovi.Size				=	4;
					m_WtMovi.FccSub			=	E_GrAviFccMovi;
					if ( sizeof(m_WtMovi) == m_ObjFl->Write( &m_WtMovi, sizeof(m_WtMovi) ) )
					{
						m_IsOpen		=	TRUE;
						m_IsWrite		=	TRUE;
						Tv_Result		=	TRUE;
						m_AviSize		=	m_AviSize + sizeof(m_WtMovi);
					}
				}

			}
			if ( !m_IsOpen )
			{
				if ( NULL != m_PtrWtHd )
				{
					free( m_PtrWtHd );
					m_PtrWtHd		=	NULL;
				}
				
				if ( NULL != m_PtrWtIdx1 )
				{
					free( m_PtrWtIdx1 );
					m_PtrWtIdx1	=	NULL;
				}
			}
			
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrAviTool::LcAddJunk( __u32 A_UserVal, void* A_PtrChuk, __u32 A_Size )
{
	// local -------------------
		Ptr_GrAviChunkHd	Tv_PtrHd;
		Ptr_GrAviFhSpcJunk	Tv_PtrCt;
	// code --------------------
		//init
		Tv_PtrHd		=	(Ptr_GrAviChunkHd)A_PtrChuk;
		//init header
		Tv_PtrHd->Fcc		=	E_GrAviFccJunk;
		Tv_PtrHd->Size	=	A_Size - sizeof(St_GrAviChunkHd);
		//fill contents
		Tv_PtrCt		=	(Ptr_GrAviFhSpcJunk)( (__u32)Tv_PtrHd + sizeof(St_GrAviChunkHd) );
		Tv_PtrCt->Fcc		=	E_GrAviEcxFcc;
		Tv_PtrCt->Val		=	A_UserVal;
		GrStrCopy( Tv_PtrCt->Str, "Damgi Kura Fire1978 201Dogfight Sungwon" );
		
}
//--------------------------------------------------------------------
void	Cls_GrAviTool::LcAddOdml( void* A_PtrChuk, __u32 A_Size )
{
	// local -------------------
		Ptr_GrAviListHd	Tv_PtrLst;
		Ptr_GrAviChunkHd	Tv_PtrChuk;
		__u32*	Tv_PtrCt;
	// code --------------------
		//odml
		Tv_PtrLst		=	(Ptr_GrAviListHd)A_PtrChuk;
		Tv_PtrLst->FccMain	=	E_GrAviFccList;
		Tv_PtrLst->Size			=	A_Size - 8;
		Tv_PtrLst->FccSub		=	E_GrAviFccOdml;
		//dmlh
		Tv_PtrChuk	=	(Ptr_GrAviChunkHd)( (__u32)A_PtrChuk + sizeof(St_GrAviListHd) );
		Tv_PtrChuk->Fcc		=	E_GrAviFccDmlh;
		Tv_PtrChuk->Size	=	A_Size - ( sizeof(St_GrAviListHd) + sizeof(St_GrAviChunkHd) );
		//content
		Tv_PtrCt		=	(__u32*)((__u32)A_PtrChuk + ( sizeof(St_GrAviListHd) + sizeof(St_GrAviChunkHd) ));
		*Tv_PtrCt		=	1;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::AddVdoFrm( void* A_PtrData, __u32 A_DataSize, BOOL8 A_IsIfrm )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrAviChunkHd	Tv_CkHd;
		__u32		Tv_FrmCnt;
		__u32		Tv_DataSize;
		__u8*		Tv_PtrTmp;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check add able
		if ( m_IsOpen && m_IsWrite && ( E_GrAviMaxFrmCnt > (m_VfrmCnt + m_AfrmCnt) ) )
		{
			//init
			Tv_Result		=	TRUE;
			Tv_DataSize	=	A_DataSize;
			if ( 0 != (1 & A_DataSize) )
			{
				Tv_DataSize ++;
				Tv_PtrTmp		=	(__u8*)( (__u32)A_PtrData + A_DataSize );
				*Tv_PtrTmp	=	0;
			}

			//add chunk header
			Tv_CkHd.Fcc		=	E_GrAviFcc00dc;
			Tv_CkHd.Size	=	Tv_DataSize;

			//check first frame
			if (	(0 == m_VfrmCnt) && 
						((E_GrFccDivOpen == m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.FccCodec) ||
						 (E_GrFccDivx5_6 == m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.FccCodec) ) )
			{
				//write head
				Tv_CkHd.Size		=	Tv_CkHd.Size + E_GrAviVolSize;
				if ( sizeof(Tv_CkHd) == m_ObjFl->Write( &Tv_CkHd, sizeof(Tv_CkHd) ) )
				{
					//add vol
					m_VolHd[23]	= (__u8)( 0x0a + (m_PtrWtHd->Hdrl.Avih.Ct.ResX >>12) );
					m_VolHd[24]	= (__u8)( ((m_PtrWtHd->Hdrl.Avih.Ct.ResX >>4) &0xff) );
					m_VolHd[25]	= (__u8)( (((m_PtrWtHd->Hdrl.Avih.Ct.ResX >>0) &0xf) <<4) + 0x8	+
						((m_PtrWtHd->Hdrl.Avih.Ct.ResY >>10) &0x7) );
					m_VolHd[26]	= (__u8)( ((m_PtrWtHd->Hdrl.Avih.Ct.ResY >>2) &0xff) );
					if ( 288 < m_PtrWtHd->Hdrl.Avih.Ct.ResY )
					{
						m_VolHd[27]	= (__u8)( (((m_PtrWtHd->Hdrl.Avih.Ct.ResY >>0) &0x3) <<6) +0x38 );
					}
					else
					{
						m_VolHd[27] = (__u8)( (((m_PtrWtHd->Hdrl.Avih.Ct.ResY >>0) &0x3) <<6) +0x28 ); 
					}
					if ( sizeof(m_VolHd) != m_ObjFl->Write( (void*)m_VolHd, sizeof(m_VolHd) ) )
					{
						Tv_Result		=	FALSE;
					}
				}
				else
				{
					Tv_Result		=	FALSE;
				}
			}
			else
			{
				//write head
				if ( sizeof(Tv_CkHd) != m_ObjFl->Write( &Tv_CkHd, sizeof(Tv_CkHd) ) )
				{
					Tv_Result		=	FALSE;
				}
			}

			//write contents
			if ( Tv_Result )
			{
				if ( Tv_DataSize == m_ObjFl->Write( A_PtrData, Tv_DataSize ) )
				{
					//set idx1
					Tv_FrmCnt		=	m_VfrmCnt + m_AfrmCnt;
					m_PtrWtIdx1[Tv_FrmCnt].Fcc		=	Tv_CkHd.Fcc;
					m_PtrWtIdx1[Tv_FrmCnt].Flags	=	0;
					if ( A_IsIfrm )
					{
						m_PtrWtIdx1[Tv_FrmCnt].Flags	=	E_GrAviIdx1FlagIfrm;
					}
					m_PtrWtIdx1[Tv_FrmCnt].Ofs		=	m_AviSize - ( E_GrAviHdBufSize + 8 );
					m_PtrWtIdx1[Tv_FrmCnt].Size		=	Tv_CkHd.Size;

					//next
					m_WtMovi.Size	=	m_WtMovi.Size + Tv_CkHd.Size + sizeof(Tv_CkHd);
					m_VfrmCnt ++;
					if ( m_VfrmMaxSize < Tv_CkHd.Size )
					{
						m_VfrmMaxSize		=	Tv_CkHd.Size;
					}
					m_AviSize		=	m_AviSize + Tv_CkHd.Size + sizeof(Tv_CkHd);
					//watermark
					m_WtmVal		=	m_WtmVal + Tv_CkHd.Size;
				}
				else
				{
					Tv_Result		=	FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::AddAdoFrm( void* A_PtrData, __u32 A_DataSize )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrAviChunkHd	Tv_CkHd;
		__u32		Tv_FrmCnt;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check add able
		if ( m_IsOpen && m_IsWrite && ( E_GrAviMaxFrmCnt > (m_VfrmCnt + m_AfrmCnt) ) )
		{
			//init
			Tv_Result		=	TRUE;
			//add chunk header
			Tv_CkHd.Fcc		=	E_GrAviFcc01wb;
			Tv_CkHd.Size	=	A_DataSize;

			//write head
			if ( sizeof(Tv_CkHd) == m_ObjFl->Write( &Tv_CkHd, sizeof(Tv_CkHd) ) )
			{
				//write contents
				if ( A_DataSize == m_ObjFl->Write( A_PtrData, A_DataSize ) )
				{
					//set idx1
					Tv_FrmCnt		=	m_VfrmCnt + m_AfrmCnt;
					m_PtrWtIdx1[Tv_FrmCnt].Fcc		=	Tv_CkHd.Fcc;
					m_PtrWtIdx1[Tv_FrmCnt].Flags	=	E_GrAviIdx1FlagIfrm;
					m_PtrWtIdx1[Tv_FrmCnt].Ofs		=	m_AviSize - ( E_GrAviHdBufSize + 8 );
					m_PtrWtIdx1[Tv_FrmCnt].Size		=	Tv_CkHd.Size;
					//next
					m_WtMovi.Size	=	m_WtMovi.Size + Tv_CkHd.Size + sizeof(Tv_CkHd);
					m_AfrmCnt ++;
					if ( m_AfrmMaxSize < Tv_CkHd.Size )
					{
						m_AfrmMaxSize		=	Tv_CkHd.Size;
					}
					m_AviSize		=	m_AviSize + Tv_CkHd.Size + sizeof(Tv_CkHd);
					//watermark
					m_WtmVal		=	m_WtmVal + Tv_CkHd.Size;
				}
				else
				{
					Tv_Result		=	FALSE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::FinishWrite( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_FrmCnt;
		St_GrAviChunkHd	Tv_CkHd;
		__u32		Tv_HdSize;
		void*		Tv_PtrWk;
		__u32		Tv_WkSize;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check write finish able
		if ( m_IsOpen && m_IsWrite )
		{
			//init
			Tv_FrmCnt		=	m_VfrmCnt + m_AfrmCnt;

			//seek last
			m_ObjFl->Seek( 0, E_GrFileCtrlPosEnd );
			//write header
			Tv_CkHd.Fcc		=	E_GrAviFccIdx1;
			Tv_CkHd.Size	=	sizeof(St_GrAviIdx1) * Tv_FrmCnt;
			m_ObjFl->Write( (void*)&Tv_CkHd, sizeof(St_GrAviChunkHd) );
			m_ObjFl->Write( (void*)m_PtrWtIdx1, sizeof(St_GrAviIdx1) * Tv_FrmCnt );
			m_AviSize			=	m_AviSize + sizeof(St_GrAviChunkHd) + (sizeof(St_GrAviIdx1) * Tv_FrmCnt);

			//make movi header
			m_ObjFl->Seek( E_GrAviHdBufSize );
			m_ObjFl->Write( &m_WtMovi, sizeof(m_WtMovi) );

			//make header
			m_PtrWtHd->Hdrl.Avih.Ct.FrmCnt		=	m_VfrmCnt;

			m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.Length	=	m_VfrmCnt;
			m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.ReqBufSize	=	m_VfrmMaxSize;
			//m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.SizeImage		=	m_VfrmMaxSize;

			m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.Length	=	(m_AfrmCnt * m_AdoBlkSize) / (m_AdoSmpBit / 8);
			m_PtrWtHd->Hdrl.StrlAdo.Strh.Ct.ReqBufSize	=	m_AfrmMaxSize;

			LcPatch( m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Compression, m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.FormatId );
			m_PtrWtHd->Hd.Size		=	m_AviSize - 8;
			Tv_HdSize		=	m_PtrWtHd->Hdrl.Hd.Size + 8 + sizeof(St_GrAviListHd);
			Tv_PtrWk		=	(void*)( (__u32)m_PtrWtHd + Tv_HdSize );
			Tv_WkSize		=	E_GrAviHdOdmlOfs - Tv_HdSize;
			LcAddJunk( m_WtmVal, Tv_PtrWk, Tv_WkSize );
			Tv_HdSize		=	E_GrAviHdOdmlOfs;
			Tv_PtrWk		=	(void*)( (__u32)m_PtrWtHd + Tv_HdSize );
			LcAddOdml( Tv_PtrWk, E_GrAviHdOdmlSize );
			Tv_HdSize		=	E_GrAviHdOdmlOfs + E_GrAviHdOdmlSize;
			Tv_PtrWk		=	(void*)( (__u32)m_PtrWtHd + Tv_HdSize );
			Tv_WkSize		=	E_GrAviHdBufSize - Tv_HdSize;
			LcAddJunk( m_WtmVal, Tv_PtrWk, Tv_WkSize );

			//header rewrite
			m_ObjFl->Seek( 0 );
			m_ObjFl->Write( m_PtrWtHd, E_GrAviHdBufSize );

			//free idx1 table
			free( m_PtrWtHd );
			m_PtrWtHd			=	NULL;
			free( m_PtrWtIdx1 );
			m_PtrWtIdx1		=	NULL;

			//success
			Tv_Result		=	TRUE;
			m_IsOpen		=	FALSE;
			m_IsWrite		=	FALSE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::LcParsAviHd( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u32	Tv_PrsSize;
		Ptr_GrAviChunkHd	Tv_PtrCk;
		Ptr_GrAviStrmHd		Tv_PtrStrh;
		__u32	Tv_FmtType;		//0 = none , 1 = video 2 = audio
	// code --------------------
		//init
		Tv_Result		=	FALSE;
		Tv_FmtType	=	E_GrAviStrfTypeNone;

		Tv_PrsSize	=	0;
		Tv_PtrCk		=	(Ptr_GrAviChunkHd)m_PtrRdHd;
		while ( Tv_PrsSize < m_RdHdSize )
		{
			switch( Tv_PtrCk->Fcc )
			{
				case E_GrAviFccAvih:
					m_PtrRdMh	=	(Ptr_GrAviMainHd)( (__u32)Tv_PtrCk + sizeof(St_GrAviChunkHd) );
					Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviChunkHd) + Tv_PtrCk->Size;
					Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + Tv_PtrCk->Size + sizeof(St_GrAviChunkHd) );
					break;
				case E_GrAviFccList:
					if ( E_GrAviFccStrl == ((Ptr_GrAviListHd)Tv_PtrCk)->FccSub )
					{
						Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviListHd);
						Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + sizeof(St_GrAviListHd) );
					}
					else
					{
						Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviChunkHd) + Tv_PtrCk->Size;
						Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + Tv_PtrCk->Size + sizeof(St_GrAviChunkHd) );
					}
					break;
				case E_GrAviFccStrh:
					Tv_PtrStrh	=	(Ptr_GrAviStrmHd)( (__u32)Tv_PtrCk + sizeof(St_GrAviChunkHd) );
					if ( E_GrAviFccVids == Tv_PtrStrh->FccType )
					{
						m_PtrStrhVdo	=	Tv_PtrStrh;
						Tv_FmtType		=	E_GrAviStrfTypeVdo;
					}
					else if ( E_GrAviFccVids == Tv_PtrStrh->FccType )
					{
						m_PtrStrhAdo	=	Tv_PtrStrh;
						Tv_FmtType		=	E_GrAviStrfTypeAdo;
					}
					Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviChunkHd) + Tv_PtrCk->Size;
					Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + Tv_PtrCk->Size + sizeof(St_GrAviChunkHd) );
					break;
				case E_GrAviFccStrf:
					if ( E_GrAviStrfTypeVdo == Tv_FmtType )
					{
						m_PtrRdVf	=	(Ptr_GrBmpIfHd)( (__u32)Tv_PtrCk + sizeof(St_GrAviChunkHd) );
					}
					else if ( E_GrAviStrfTypeAdo == Tv_FmtType )
					{
						m_PtrRdAf	=	(Ptr_GrAviWaveformatEx)( (__u32)Tv_PtrCk + sizeof(St_GrAviChunkHd) );
					}
					Tv_FmtType	=	E_GrAviStrfTypeNone;
					Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviChunkHd) + Tv_PtrCk->Size;
					Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + Tv_PtrCk->Size + sizeof(St_GrAviChunkHd) );
					break;
				default:
					Tv_PrsSize	=	Tv_PrsSize + sizeof(St_GrAviChunkHd) + Tv_PtrCk->Size;
					Tv_PtrCk	=	(Ptr_GrAviChunkHd)( (__u32)Tv_PtrCk + Tv_PtrCk->Size + sizeof(St_GrAviChunkHd) );
					break;
			}
		}

		//check current
		if ( (NULL != m_PtrRdMh) && (NULL != m_PtrStrhVdo) && (NULL != m_PtrRdVf) )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::LcParsCont( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrAviListHd	Tv_LsHd;
		__u32	Tv_Fptr;
		St_GrAviFhSpcJunk	Tv_Ecx;
	// code --------------------
		//init
		Tv_Result	=	FALSE;

		while ( 8 == m_ObjFl->Read( &Tv_LsHd, 8 ) )
		{
			Tv_Fptr	=	(__u32)m_ObjFl->GetPos() - 8;
			switch ( Tv_LsHd.FccMain )
			{
				case E_GrAviFccJunk:
					if ( !m_IsRdWtmExist )
					{
						//read watermark value
						if ( sizeof(Tv_Ecx) == m_ObjFl->Read( &Tv_Ecx, sizeof(Tv_Ecx) ) )
						{
							if ( E_GrAviEcxFcc == Tv_Ecx.Fcc )
							{
								m_WtmVal	=	Tv_Ecx.Val;
								m_IsRdWtmExist	=	TRUE;
							}
						}
					}
					break;
				case E_GrAviFccList:
					//read sub fcc
					if ( 4 == m_ObjFl->Read( &Tv_LsHd.FccSub, 4 ) )
					{
						if ( E_GrAviFccMovi == Tv_LsHd.FccSub )
						{
							if ( 0 == m_FptrRdMovi )
							{
								m_FptrRdMovi			=	Tv_Fptr;
								m_RdMoviSize			=	Tv_LsHd.Size;
							}
						}
					}
					break;
				case E_GrAviFccIdx1:
					if ( 0 == m_FptrRdIdx1 )
					{
						m_FptrRdIdx1			=	Tv_Fptr;
						m_RdIdx1Cnt				=	Tv_LsHd.Size / sizeof(St_GrAviIdx1);
					}
					break;
			}
			//go next
			m_ObjFl->Seek( Tv_Fptr + 8 + Tv_LsHd.Size );
		}

		//check current
		if ( 0 != m_FptrRdMovi )
		{
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::StartRead( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		St_GrAviListHd	Tv_LsHd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( (!m_IsOpen) && (m_ObjFl->IsOpened()) )
		{
			//init
			m_PtrRdMh			=	NULL;
			m_PtrStrhVdo	=	NULL;
			m_PtrStrhAdo	=	NULL;
			m_PtrRdVf			=	NULL;
			m_PtrRdAf			=	NULL;
			m_RdMoviSize	=	0;
			m_FptrRdMovi	=	0;
			m_RdIdx1Cnt		=	0;
			m_FptrRdIdx1	=	0;
			m_RdWtmPos		=	0;
			m_RdWtmVal		=	0;
			m_FptrRdFrm		=	0;
			m_IsRdWtmExist	=	FALSE;

			//analysis header
			m_ObjFl->Seek( 0 );
			//riff header
			if ( sizeof(Tv_LsHd) == m_ObjFl->Read( &Tv_LsHd, sizeof(Tv_LsHd) ) )
			{
				//check riff header
				if ( (E_GrAviFccRiff == Tv_LsHd.FccMain) && (E_GrAviFccAvi == Tv_LsHd.FccSub) )
				{
					//read hdrl head
					if ( sizeof(Tv_LsHd) == m_ObjFl->Read( &Tv_LsHd, sizeof(Tv_LsHd) ) )
					{
						//check hdrl head
						if ( (E_GrAviFccList == Tv_LsHd.FccMain) && (E_GrAviFccHdrl == Tv_LsHd.FccSub) )
						{
							//allocate header buffer
							m_RdHdSize	=	Tv_LsHd.Size - 4;
							m_PtrRdHd		=	(void*)malloc( m_RdHdSize );
							if ( NULL != m_PtrRdHd )
							{
								//read avi header
								if ( ( m_RdHdSize) == m_ObjFl->Read( m_PtrRdHd, m_RdHdSize ) )
								{
									//parsing avi header
									if ( LcParsAviHd() )
									{
										//contents parsing
										if ( LcParsCont() )
										{
											//success
											m_IsOpen		=	TRUE;
											m_IsWrite		=	FALSE;
											Tv_Result		=	TRUE;
										}
									}
								}
							}
						}
					}
				}
			}

			//release memory
			if ( !m_IsOpen )
			{
				if ( NULL != m_PtrRdHd )
				{
					free( m_PtrRdHd );
					m_PtrRdHd		=	NULL;
				}
			}
			
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::GetFrm( void* A_PtrBuf, __u32* A_PtrRtSize, BOOL8* A_PtrRtIsAdo )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrAviChunkHd	Tv_CkHd;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check read mode
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check able file pointer
			if ( m_FptrRdFrm < ( m_FptrRdMovi + m_RdMoviSize ) )
			{
				//check start
				if ( 0 == m_FptrRdFrm )
				{
					m_FptrRdFrm	=	m_FptrRdMovi + sizeof(St_GrAviListHd);
				}
				//read frame header
				if ( m_FptrRdFrm == m_ObjFl->Seek(m_FptrRdFrm) )
				{
					//read chunk header
					if ( sizeof(Tv_CkHd) == m_ObjFl->Read( &Tv_CkHd, sizeof(Tv_CkHd) ) )
					{
						//check frame type
						*A_PtrRtIsAdo		=	FALSE;
						if ( (E_GrAviFcc01wb & 0xFF00) == (Tv_CkHd.Fcc & 0xFF00) )
						{
							*A_PtrRtIsAdo		=	TRUE;
						}
						//get frame contents
						*A_PtrRtSize	=	Tv_CkHd.Size;
						if ( m_ObjFl->Read( A_PtrBuf, Tv_CkHd.Size ) == Tv_CkHd.Size )
						{
							//success
							Tv_Result		=	TRUE;
							//set next file pointer
							m_FptrRdFrm	=	m_FptrRdFrm + sizeof(St_GrAviChunkHd) + Tv_CkHd.Size;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::MoveReadPos( __u32 A_MovePos )
{
	// local -------------------
		BOOL8		Tv_Result;
		St_GrAviIdx1	Tv_Idx1;
		__u32		Tv_Fptr;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check exist move able
			if ( (0 != m_FptrRdIdx1) && ( A_MovePos < m_FptrRdIdx1 ) )
			{
				//read idx table
				Tv_Fptr	=	m_FptrRdIdx1 + sizeof(St_GrAviChunkHd) + (A_MovePos * sizeof(St_GrAviIdx1) );
				if ( m_ObjFl->Seek( Tv_Fptr ) )
				{
					if ( m_ObjFl->Read( &Tv_Idx1, sizeof(St_GrAviIdx1) ) == sizeof(St_GrAviIdx1) )
					{
						m_FptrRdFrm		=	Tv_Idx1.Ofs + m_FptrRdMovi + 8;
						Tv_Result			=	TRUE;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::FinishRead( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			if ( NULL != m_PtrRdHd )
			{
				free( m_PtrRdHd );
				m_PtrRdHd		=	NULL;
			}
			m_IsOpen		=	FALSE;
			//success
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::IsWtmFinished( void )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check able
			if ( (0 != m_RdIdx1Cnt) && m_IsRdWtmExist )
			{
				//check finished
				if ( m_RdIdx1Cnt <= m_RdWtmPos )
				{
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::WtmChk( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_Fptr;
		St_GrAviIdx1	Tv_Idx1;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check able
			if ( (0 != m_RdIdx1Cnt) && m_IsRdWtmExist )
			{
				//check finished
				if ( m_RdWtmPos < m_RdIdx1Cnt )
				{
					//read idx table
					Tv_Fptr	=	m_FptrRdIdx1 + sizeof(St_GrAviChunkHd) + (m_RdWtmPos * sizeof(St_GrAviIdx1) );
					if ( m_ObjFl->Seek( Tv_Fptr ) )
					{
						if ( m_ObjFl->Read( &Tv_Idx1, sizeof(St_GrAviIdx1) ) == sizeof(St_GrAviIdx1) )
						{
							//success
							m_RdWtmVal		=	m_RdWtmVal + Tv_Idx1.Size;
							m_RdWtmPos ++;
							Tv_Result			=	TRUE;
						}
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::IsWtmOk( void )
{
	// local -------------------
		BOOL8		Tv_Result;
		//St_GrAviIdx1	Tv_Idx1;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( IsWtmFinished() )
		{
			if ( m_RdWtmVal == m_WtmVal )
			{
				Tv_Result		=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrAviTool::WtmPrsCnt( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check able
			if ( (0 != m_RdIdx1Cnt) && m_IsRdWtmExist )
			{
				Tv_Result		=	m_RdIdx1Cnt;
			}
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::GetVdoInfo(	__u32* A_PtrRtFcc, __u32* A_PtrRtFps, 
																	__u32* A_PtrRtResX, __u32* A_PtrRtResY )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check opened
		if ( m_IsOpen )
		{
			if ( m_IsWrite )
			{
				//write mode
				*A_PtrRtFcc		=	m_PtrWtHd->Hdrl.StrlVdo.Strf.Ct.Compression;
				*A_PtrRtFps		=	m_PtrWtHd->Hdrl.StrlVdo.Strh.Ct.Rate;
				*A_PtrRtResX	=	m_PtrWtHd->Hdrl.Avih.Ct.ResX;
				*A_PtrRtResY	=	m_PtrWtHd->Hdrl.Avih.Ct.ResY;
			}
			else
			{
				//read mode
				*A_PtrRtFcc		=	m_PtrRdVf->Compression;
				*A_PtrRtFps		=	m_PtrStrhVdo->Rate;
				*A_PtrRtResX	=	m_PtrRdMh->ResX;
				*A_PtrRtResY	=	m_PtrRdMh->ResY;
			}
			Tv_Result		=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8	Cls_GrAviTool::GetAdoInfo(	__u16* A_PtrRtId, __u32* A_PtrRtCh, 
																	__u32* A_PtrSmpRate, __u32* A_PtrRtSmpBit )
{
	// local -------------------
		BOOL8		Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		//check opened
		if ( m_IsOpen )
		{
			if ( m_IsWrite )
			{
				//write mode
				*A_PtrRtId			=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.FormatId;
				*A_PtrRtCh			=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.ChCnt;
				*A_PtrSmpRate		=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.SmpRate;
				*A_PtrRtSmpBit	=	m_PtrWtHd->Hdrl.StrlAdo.Strf.Ct.SmpBit;
				Tv_Result		=	TRUE;
			}
			else
			{
				//read mode
				if ( NULL != m_PtrRdAf )
				{
					*A_PtrRtId			=	m_PtrRdAf->FormatId;
					*A_PtrRtCh			=	m_PtrRdAf->ChCnt;
					*A_PtrSmpRate		=	m_PtrRdAf->SmpRate;
					*A_PtrRtSmpBit	=	m_PtrRdAf->SmpBit;
					Tv_Result		=	TRUE;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrAviTool::GetMaxStrmSize( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//get request size
			Tv_Result		=	m_PtrStrhVdo->ReqBufSize;
			if ( NULL != m_PtrStrhAdo )
			{
				if ( Tv_Result < m_PtrStrhAdo->ReqBufSize )
				{
					Tv_Result		=	m_PtrStrhAdo->ReqBufSize;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
__u32	Cls_GrAviTool::GetMovePointCnt( void )
{
	// local -------------------
		__u32	Tv_Result;
	// code --------------------
		//init
		Tv_Result		=	0;

		//check readmode open
		if ( m_IsOpen && (!m_IsWrite) )
		{
			//check able
			if ( 0 != m_RdIdx1Cnt )
			{
				Tv_Result		=	m_RdIdx1Cnt;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------

