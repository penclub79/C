/*
	xvid decode 
*/

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <GrDumyTool.h>
#include <GrImgBase.h>
#include <Mpeg4/GrXvidDec.h>

//====================================================================
//const
//#define XVID_DOO_MPEG4PLUS (1<<15)
//====================================================================
//local type

//====================================================================
//local function

//====================================================================
//var
__u8	V_XvidDecVol[30]	= {	0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x20, 
														0x02, 0x48, 0x0d, 0xc0, 0x00, 0x40, 0x00, 0x40,
														0x00, 0x40, 0x00, 0x80, 0x00, 0x97, 0x53, 0x0a, 
														0x2c, 0x08, 0x3c, 0x28, 0x8c, 0x1f};

//--------------------------------------------------------------------
Cls_GrXvidDec::Cls_GrXvidDec( )
{

		//init
		m_IsInited		=	FALSE;

		GrDumyCopyMem( (void*)m_VolTbl, (void*)V_XvidDecVol, sizeof(V_XvidDecVol) );

}
//--------------------------------------------------------------------
Cls_GrXvidDec::~Cls_GrXvidDec()
{
	// var --------------------

	// code -------------------
		DecFinish();
}
//--------------------------------------------------------------------
BOOL8		Cls_GrXvidDec::DecInit( __u32 A_Fcc, __u32 A_MaxResX, __u32 A_MaxResY, 
														 void* A_PtrDib, __u32 A_Pitch, BOOL8 A_IsWithoutVol )
{
	// local -------------------
		BOOL8		Tv_Result;
		xvid_gbl_init_t		Tv_XvidGblInit;
		xvid_dec_create_t Tv_XvidCreate;		
	// code --------------------
		//init
		Tv_Result		=	FALSE;

		if ( !m_IsInited )
		{
			//check able fcc
			Tv_Result		=	TRUE;
			switch ( A_Fcc )
			{
				case E_GrFccI420:
					m_Csp		=	XVID_CSP_I420;
					break;
				case E_GrFccYUY2:
					m_Csp		=	XVID_CSP_YUY2;
					break;
				case E_GrFccUYVY:
					m_Csp		=	XVID_CSP_UYVY;
					break;
				case E_GrFccRGB15:
					m_Csp		=	XVID_CSP_RGB555;
					break;
				default:
					Tv_Result		=	FALSE;
					break;
			}
			//check able
			if ( Tv_Result )
			{
				//init
				m_Fcc						=	A_Fcc;
				m_PtrDib				=	A_PtrDib;
				m_Pitch					=	A_Pitch;
				m_MaxResX				=	A_MaxResX;
				m_MaxResY				=	A_MaxResY;
				m_IsWithoutVol	=	A_IsWithoutVol;	
				GrDumyZeroMem(&Tv_XvidGblInit, sizeof(xvid_gbl_init_t));
				GrDumyZeroMem(&Tv_XvidCreate, sizeof(xvid_dec_create_t));

				//init xvid core
				//xvid version
				Tv_XvidGblInit.version		= XVID_VERSION;
				Tv_XvidGblInit.cpu_flags	= XVID_CPU_FORCE;
				Tv_XvidGblInit.debug			= 0;
				xvid_global(NULL, 0, &Tv_XvidGblInit, NULL);

				//create xvid decoder
				Tv_XvidCreate.version			= XVID_VERSION;
				Tv_XvidCreate.width				= 0;
				Tv_XvidCreate.height			= 0;
				if ( XVID_ERR_FAIL != xvid_decore(NULL, XVID_DEC_CREATE, &Tv_XvidCreate, NULL) )
				{
					m_HndlXvid	=	Tv_XvidCreate.handle;
					m_IsInited	=	TRUE;
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
BOOL8		Cls_GrXvidDec::DecFrame( void* A_PtrData, __u32 A_DataSize, 
															__u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterace,
															BOOL8* A_PtrRtIsOut )
{
	// local -------------------
		BOOL8		Tv_Result;
		__u32		Tv_SrSize;
		void*		Tv_PtrData;
		BOOL8		Tv_IsVol;
		__u32		Tv_DecSize;
		__u32*	Tv_PtrVopHd;
	// code --------------------
		//init
		Tv_Result			=	FALSE;

		//check inited
		if ( m_IsInited )
		{
			//check current data
			Tv_PtrVopHd		=	(__u32*)A_PtrData;
			if ( 0xB6010000 == *Tv_PtrVopHd )
			{
				//init
				*A_PtrRtIsOut	=	FALSE;
				//check widthout vol mode
				if ( m_IsWithoutVol )
				{
					//decode added vol
					LcDecVol( A_ResX, A_ResY, A_IsInterace );
				}
				//decode size
				Tv_Result		=	TRUE;
				Tv_PtrData	=	A_PtrData;
				Tv_SrSize		=	A_DataSize;
				while ( 0 != Tv_SrSize )
				{
					//decode
					Tv_DecSize	=	LcDecData( Tv_PtrData, Tv_SrSize, &Tv_IsVol );
					if ( (0 != Tv_DecSize) && (Tv_DecSize <= Tv_SrSize) && ( 20 > (Tv_SrSize - Tv_DecSize)) )
					{
						if ( !Tv_IsVol )
						{
							*A_PtrRtIsOut		=	TRUE;
							break;
						}
						//next
						Tv_PtrData	=	(void*)( (__u32)Tv_PtrData + Tv_DecSize );
						if ( Tv_DecSize <= Tv_SrSize )
						{
							Tv_SrSize		=	Tv_SrSize - Tv_DecSize;
						}
						else
						{
							Tv_SrSize		=	0;
						}
					}
					else
					{
						Tv_Result		=	FALSE;
						break;
					}
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrXvidDec::LcDecVol( __u32 A_ResX, __u32 A_ResY, BOOL8 A_IsInterace )
{
	// var --------------------
		BOOL8		Tv_Result;
		BOOL8		Tv_IsVol;
	// code -------------------
		//init
		Tv_Result		=	FALSE;

		//make vol
		m_VolTbl[23]	= 0x0a + (A_ResX >>12);
		m_VolTbl[24]	= ((A_ResX >>4) &0xff);
		m_VolTbl[25]	= (((A_ResX >>0) &0xf) <<4) + 0x8	+((A_ResY >>10) &0x7);
		m_VolTbl[26]	= ((A_ResY >>2) &0xff);
		if ( A_IsInterace )
		{
			m_VolTbl[27]	= (((A_ResY >>0) &0x3) <<6) +0x38;
		}
		else
		{
			m_VolTbl[27] = (((A_ResY >>0) &0x3) <<6) +0x28;
		}

		//decode
		Tv_Result		=	LcDecData( (void*)m_VolTbl, sizeof(m_VolTbl), &Tv_IsVol );

		return	Tv_Result;

}
//--------------------------------------------------------------------
__u32		Cls_GrXvidDec::LcDecData( void* A_PtrData, __u32 A_DataSize, BOOL8* A_PtrRtIsVol )
{
	// local -------------------
		__u32		Tv_Result;
		xvid_dec_stats_t	Tv_XvidStats;
		xvid_dec_frame_t	Tv_XvidFrame;		
		__s32			Tv_DecSize;
	// code --------------------
		//init
		Tv_Result		=	0;
		GrDumyZeroMem(&Tv_XvidFrame, sizeof(xvid_dec_frame_t));
		GrDumyZeroMem(&Tv_XvidStats, sizeof(xvid_dec_stats_t));

		//xvid decoder version
		Tv_XvidFrame.version			= XVID_VERSION;
		Tv_XvidStats.version			= XVID_VERSION;
		//set general flag
		Tv_XvidFrame.general			= XVID_LOWDELAY;
		//source image
		Tv_XvidFrame.bitstream		= A_PtrData;
		Tv_XvidFrame.length				= A_DataSize;

		//taget image
		Tv_XvidFrame.output.plane[0]	= m_PtrDib;
		Tv_XvidFrame.output.stride[0]	= m_Pitch;
		Tv_XvidFrame.output.csp				= m_Csp;				
		//check vol type
		try
		{
			Tv_DecSize	=	xvid_decore(m_HndlXvid, XVID_DEC_DECODE, &Tv_XvidFrame, &Tv_XvidStats );
			if ( XVID_ERR_FAIL != Tv_DecSize )
			{
				//success
				Tv_Result				=	(__u32)Tv_DecSize;
				*A_PtrRtIsVol		=	FALSE;
				//update last vol
				if ( XVID_TYPE_VOL == Tv_XvidStats.type )
				{
					//check limite
					if ( ( m_MaxResX >= Tv_XvidStats.data.vol.width ) && (m_MaxResY >= Tv_XvidStats.data.vol.height) )
					{
						//update decoded resolution
						m_DecResX		=	Tv_XvidStats.data.vol.width;
						m_DecResY		=	Tv_XvidStats.data.vol.height;
						//update vol
						*A_PtrRtIsVol		=	TRUE;
					}
					else
					{
						Tv_Result		=	0;
					}
				}
			}
		}
		catch (...)
		{
			Tv_Result		=	0;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrXvidDec::DecFinish( void )
{
		if ( m_IsInited ) 
		{
			xvid_decore(m_HndlXvid, XVID_DEC_DESTROY, NULL, NULL);	
			m_IsInited		=	FALSE;
		}
}
//--------------------------------------------------------------------
void		Cls_GrXvidDec::GetDecRes( __u32* A_PtrRtResX, __u32* A_PtrRtResY )
{
		*A_PtrRtResX	=	m_DecResX;
		*A_PtrRtResY	=	m_DecResY;
}
//--------------------------------------------------------------------
