/*
	emulation LINUX solo device 

*/

//====================================================================
//include

#include	<EmuLnx/GrElmDevSolo.h>
#include	<Hw/Solo/GrDrvSolo.h>
#include	<GrDumyTool.h>
#include	<GrBufTool.h>
#include	<GrGdibTool.h>
#include	<GrFileCtrl.h>
#include	<Dvr/GrDvrBase.h>

//====================================================================
//local const
#define E_GrElmDevSoloIrq						5

#define E_GrEmlDevSoloVencScaleCnt	3
#define E_GrEmlDevSoloVencSubFrmCnt	2

#define E_GrEmlDevSoloVencStatCnt		2

//#define E_GrEmlDevSoloVencStx				Mac_GrMakeFcc('V','D','O','S')
//#define E_GrEmlDevSoloVencEtx				Mac_GrMakeFcc('V','D','O','E')

#define E_GrEmlDevSoloAencStx				Mac_GrMakeFcc('A','D','O','S')
#define E_GrEmlDevSoloAencEtx				Mac_GrMakeFcc('A','D','O','E')

#define E_GrEmlDevSoloMtnMark				Mac_GrMakeFcc('M','O','T','N')

#define E_GrEmlDevSoloVencPeriod		2

#define E_GrEmlDevSoloG723Period		10

#define E_GrEmlDevSoloChCnt					16	/* emulation channel count */

//====================================================================
//local type

typedef	struct St_GrEmlDevSoloVencData 
{
	DWORD		Size;
	DWORD		Stat[E_GrEmlDevSoloVencStatCnt];
}	*Ptr_GrEmlDevSoloVencData;

//====================================================================
//global var

void*		V_GrElmDevSoloLmemTbl[E_GrElmDevSoloMaxDevCnt]	=	{	NULL, NULL	};		//local memory pointer
BYTE		V_GrElmDevSoloPntQue[E_GrElmDevSoloMaxDevCnt][E_GrElmDevSoloPntQueSize];

extern	Cls_GrGdib*		V_EmuGdibDisp[E_GrEmuDispMaxCnt];

St_GrEmlDevSoloVencData	V_GrElmDevSoloEncVdoStrmTbl[E_GrEmlDevSoloVencScaleCnt][E_GrEmlDevSoloVencSubFrmCnt]	=	
{
	//d1
	{
		// I frame
		{
			64 * 1001,	{ 0x40000000, 0x90002C1E }
		},
		//pframe
		{
			64 * 404,	{ 0x40400000, 0x90002C1E }
		}
	},
	//half d1
	{
		// I frame
		{
			64 * 508,	{ 0x00000000, 0x20002C0F }
		},
			//pframe
		{
			64 * 211,	{ 0x00400000, 0x20002C0F }
		}
	},
		//cif
	{
		// I frame
		{
			64 * 243,	{ 0x00000000, 0x3000160F }
		},
			//pframe
		{
			64 * 97,	{ 0x00400000, 0x3000160F }
		}
	}
};

//====================================================================
//functions
DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para );

//--------------------------------------------------------------------
Cls_GrElmDevSolo::Cls_GrElmDevSolo( DWORD A_DevIdx, BOOL A_Is6110 ):
Cls_GrElmDevBase( A_DevIdx )
{
	// local -------------------
		DWORD	Tv_WkIdx;
		void*	Tv_PtrOsg;
	// code --------------------
		//init
		m_DevIdx		=	A_DevIdx;
		m_Is6110		=	A_Is6110;
		m_IsRun			=	TRUE;
		m_IsIrqReq	=	FALSE;

		m_GdibOsg		=	NULL;

		m_I2cIrqed	=	0;
		m_EncCh			=	0;
		m_EncQueIdx	=	0;
		m_EncPeriod	=	0;

		m_G723Period	=	0;

		m_PciVid		=	E_SoloVid;
#ifdef EMU_SOLO_6110
		m_PciPid		=	E_SoloPid6110;
#else
		m_PciPid		=	E_SoloPid6010;
#endif
		//resource build
		m_PtrReg	=	(void*)malloc( E_SoloRegAreaSize );
		m_PtrLmem	=	(void*)malloc( E_SoloLocalMemSize );

		//get local device index and local memory register
		for ( Tv_WkIdx=0;Tv_WkIdx < E_GrElmDevSoloMaxDevCnt;Tv_WkIdx++ )
		{
			if ( NULL == V_GrElmDevSoloLmemTbl[Tv_WkIdx] )
			{
				// found
				V_GrElmDevSoloLmemTbl[Tv_WkIdx]	=	m_PtrLmem;		//set
				m_LdevIdx		=	Tv_WkIdx;
				break;
			}
		}

		for ( Tv_WkIdx=0;Tv_WkIdx < E_SoloMaxChCnt;Tv_WkIdx++ )
		{
			m_EncScl[Tv_WkIdx]	=	E_GrEmlDevSoloVencScaleCnt;
			m_FrmIdx[Tv_WkIdx]	=	0;
			m_PtrFrm[Tv_WkIdx]	=	(Ptr_GrDvrAvHd)m_EmuVdoBuf;
		}
		

		//device init
		m_PciDev.irq	=	E_GrElmDevSoloIrq;
		m_PciDev.Rsrc[0].Adr	=	(DWORD)m_PtrReg;
		m_PciDev.Rsrc[0].Size	=	E_SoloRegAreaSize;

		//register init
		LcRegInit();
		//local memory init
		LcLmemInit();

		//osg init
		Tv_PtrOsg	=	(void*)( (DWORD)m_PtrLmem + E_GrDrvSoloLmemOsgPos );
		m_GdibOsg	=	GrGdibCreate( E_GrFccUYVYF, E_SoloBaseResX, E_SoloPalResY, E_SoloOsgLmemPitch, Tv_PtrOsg );
		m_GdibDisp	=	V_EmuGdibDisp[m_LdevIdx];

		//emulate data loading
		LcEmuAvLoad();

		//paint queue init
		GrBufQueInit( (Ptr_GrBufQue)V_GrElmDevSoloPntQue[m_LdevIdx], sizeof(St_GrRect), E_GrElmDevSoloPntQueSize );

		//thread create
		GrPrcThrdCreate( &m_HndlThrd, GrElmDevSoloProcThrd, (void*)this );
}
//--------------------------------------------------------------------
Cls_GrElmDevSolo::~Cls_GrElmDevSolo()
{
	// local -------------------
	// code --------------------
		//release thread
		m_IsRun		=	FALSE;
		GrPrcThrdRelease( &m_HndlThrd );

		//unregister local memory table
		V_GrElmDevSoloLmemTbl[m_LdevIdx]	=	NULL;		//remove

		// OSG free
		free( m_GdibOsg );

		//register free
		free( m_PtrLmem );
		free( m_PtrReg );
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcRegInit( void )
{
	// local -------------------
		DWORD*	Tv_PtrReg;
	// code --------------------
		GrDumyZeroMem( m_PtrReg, E_SoloRegAreaSize );
		
		//test mode register
		Tv_PtrReg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_TEST_MODE );
		switch( E_GrEmlDevSoloChCnt )
		{
			case	16:
				*Tv_PtrReg	=	0x07;		//16channel device
				break;
			case	9:
				*Tv_PtrReg	=	0x06;		//9channel device
				break;
			default:
				*Tv_PtrReg	=	0x05;		//4channel device
				break;
		}
		
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcLmemInit( void )
{
	// local -------------------
	// code --------------------
		GrDumyZeroMem( m_PtrLmem, E_SoloLocalMemSize );

}
//--------------------------------------------------------------------
DWORD WINAPI GrElmDevSoloProcThrd( LPVOID A_Para )
{
	// local -------------------
		Cls_GrElmDevSolo*	Tv_ObjSolo;
	// code --------------------
		Tv_ObjSolo	=	(Cls_GrElmDevSolo*)A_Para;
		Tv_ObjSolo->RtlThrdProc();

		return	0;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::RtlThrdProc( void )
{
	// local -------------------
		//DWORD		Tv_Tick;
		BOOL	Tv_IsDo;
	// code --------------------
		while ( m_IsRun )
		{
			Tv_IsDo	=	FALSE;

			//p2m
			if ( LcThrdP2m( 0 ) )
			{
				Tv_IsDo	=	TRUE;
			}
			if ( LcThrdP2m( 1 ) )
			{
				Tv_IsDo	=	TRUE;
			}
			if ( LcThrdP2m( 2 ) )
			{
				Tv_IsDo	=	TRUE;
			}
			if ( LcThrdP2m( 3 ) )
			{
				Tv_IsDo	=	TRUE;
			}
			//audio
			LcThrdAdo();

			//record 
			LcThrdVenc();

			//jpeg

			//i2c
			if ( LcThrdI2c() )
			{
				Tv_IsDo	=	TRUE;
			}
			

			//check interrupt acknologed - retry


			//interrupt
			if ( LcThrdIrq() )
			{
				Tv_IsDo	=	TRUE;
			}
			//check sleep
			if ( !Tv_IsDo )
			{
				GrPrcSleep( 20 );
			}
			else
			{
				GrPrcSleep( 0 );
			}
		}
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::MioWriteDword( void* A_Ptr, DWORD A_Val )
{
	// local -------------------
		BOOL		Tv_Result;
		DWORD		Tv_Reg;
		DWORD*	Tv_PtrReg;
		DWORD		Tv_Idx;
	// code --------------------
		Tv_Result	=	FALSE;

		//check register range
		if ( ( (DWORD)m_PtrReg <= (DWORD)A_Ptr ) && ( ((DWORD)m_PtrReg + E_SoloRegAreaSize) > (DWORD)A_Ptr ) )
		{
			//check address align
			if ( 0 == ( 3 & (DWORD)A_Ptr ) )
			{
				//work
				Tv_PtrReg	=	(DWORD*)A_Ptr;
				Tv_Reg	=	(DWORD)A_Ptr - (DWORD)m_PtrReg;
				//register work
				switch( Tv_Reg )
				{
					case	E_SoloRa_INTR_STAT:
						*Tv_PtrReg	=	*Tv_PtrReg & ~A_Val;
						break;
					case	E_SoloRa_P2M_CONTROL(0):
					case	E_SoloRa_P2M_CONTROL(1):
					case	E_SoloRa_P2M_CONTROL(2):
					case	E_SoloRa_P2M_CONTROL(3):
						LcEwrP2mCtrl( Tv_Reg, A_Val );
						break;
					case	E_SoloRa_TEST_MODE:
						//read only
						DbgMsgPrint( "GrElmDevSolo : error! try write TEST_MODE register!" );
						break;
					case	E_SoloRa_IIC_CTRL:
						*Tv_PtrReg	=	A_Val | E_SoloRv_IIC_STAT_TRNS;		//mark i2c start trans
						break;
					case	E_SoloRa_VE_CFG0:
						*Tv_PtrReg		=	A_Val;
						m_PtrEncBuf		=	(void*)( (DWORD)m_PtrLmem + ( (0xFFFF & A_Val) << 16 ) );
						m_EncBufSize	=	0xFF0000 & A_Val;
						m_EncAddPos		=	0;
						break;
					case	E_SoloRa_CAP_CH_SCALE(0):
					case	E_SoloRa_CAP_CH_SCALE(1):
					case	E_SoloRa_CAP_CH_SCALE(2):
					case	E_SoloRa_CAP_CH_SCALE(3):
					case	E_SoloRa_CAP_CH_SCALE(4):
					case	E_SoloRa_CAP_CH_SCALE(5):
					case	E_SoloRa_CAP_CH_SCALE(6):
					case	E_SoloRa_CAP_CH_SCALE(7):
					case	E_SoloRa_CAP_CH_SCALE(8):
					case	E_SoloRa_CAP_CH_SCALE(9):
					case	E_SoloRa_CAP_CH_SCALE(10):
					case	E_SoloRa_CAP_CH_SCALE(11):
					case	E_SoloRa_CAP_CH_SCALE(12):
					case	E_SoloRa_CAP_CH_SCALE(13):
					case	E_SoloRa_CAP_CH_SCALE(14):
					case	E_SoloRa_CAP_CH_SCALE(15):
						Tv_Idx	=	(Tv_Reg - E_SoloRa_CAP_CH_SCALE(0)) >> 2;
						*Tv_PtrReg		=	A_Val;
						if ( 9 == A_Val )
						{
							m_EncScl[Tv_Idx]	=	0;
						}
						else if ( 1 == A_Val )
						{
							m_EncScl[Tv_Idx]	=	1;
						}
						else if ( 2 == A_Val )
						{
							m_EncScl[Tv_Idx]	=	2;
						}
						else
						{
							m_EncScl[Tv_Idx]	=	E_GrEmlDevSoloVencScaleCnt;
						}
						m_EncGop[Tv_Idx]	=	0;
						break;
					case	E_SoloRa_VD_CTRL:
						*Tv_PtrReg		=	A_Val;
						if ( 0 != (E_SoloRv_VD_OPER_ON & A_Val) )
						{
							*Tv_PtrReg	=	*Tv_PtrReg & ~E_SoloRv_VD_OPER_ON;
							LcEwrVdec();
						}
						break;
					case	E_SoloRa_VD_IDX0:
						*Tv_PtrReg	=	A_Val;
						break;
					default:
						*Tv_PtrReg	=	A_Val;
						break;
				}

				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::MioReadDword( void* A_Ptr, DWORD* A_PtrRtVal )
{
	// local -------------------
		BOOL		Tv_Result;
		DWORD		Tv_Reg;
		DWORD*	Tv_PtrReg;
	// code --------------------
		//check register range
		if ( ( (DWORD)m_PtrReg <= (DWORD)A_Ptr ) && ( ((DWORD)m_PtrReg + E_SoloRegAreaSize) > (DWORD)A_Ptr ) )
		{
			//check address align
			if ( 0 == ( 3 & (DWORD)A_Ptr ) )
			{
				//work
				Tv_PtrReg	=	(DWORD*)A_Ptr;
				Tv_Reg	=	(DWORD)A_Ptr - (DWORD)m_PtrReg;

				//update default
				*A_PtrRtVal	=	*Tv_PtrReg;
				//success
				Tv_Result	=	TRUE;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcEwrP2mCtrl( DWORD A_Reg, DWORD A_Val )
{
	// local -------------------
		DWORD*	Tv_PtrRegCtrl;
	// code --------------------
		//init
		Tv_PtrRegCtrl		=	(DWORD*)( (DWORD)m_PtrReg + A_Reg );
		//check request transfer
		if ( 0 != ( E_SoloRv_P2M_TRANS_ON & A_Val ) )
		{
			//check transfer alread
			if ( 0 != ( E_SoloRv_P2M_TRANS_ON & *Tv_PtrRegCtrl ) )
			{
				DbgMsgPrint( "Cls_GrElmDevSolo %d : P2m already in transfer. reg = %x \n", m_LdevIdx, A_Reg );
			}
		}
		//write
		*Tv_PtrRegCtrl	=	A_Val;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::LcThrdP2m( DWORD A_P2mCh )
{
	// local -------------------
		BOOL	Tv_Result;
		DWORD*	Tv_PtrRegCtrl;
		DWORD*	Tv_PtrRegExtCfg;
		DWORD*	Tv_PtrRegTarAdr;
		DWORD*	Tv_PtrRegExtAdr;
		DWORD*	Tv_PtrRegIrqStat;
		DWORD*	Tv_PtrRegIrqEnable;
		void*		Tv_PtrSr;
		void*		Tv_PtrTg;
		DWORD		Tv_CopySize;
		DWORD		Tv_PitchSr;
		DWORD		Tv_PitchTg;
		DWORD		Tv_Repeat;
		DWORD		Tv_WkIdx;
		DWORD		Tv_TrnsSize;
	// code --------------------
		//init
		Tv_Result	=	FALSE;
		Tv_PtrRegCtrl			=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_P2M_CONTROL(A_P2mCh) );
		Tv_PtrRegExtCfg		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_P2M_EXT_CFG(A_P2mCh) );
		Tv_PtrRegTarAdr		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_P2M_TAR_ADR(A_P2mCh) );
		Tv_PtrRegExtAdr		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_P2M_EXT_ADR(A_P2mCh) );
		
		Tv_PtrRegIrqStat		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_STAT );
		Tv_PtrRegIrqEnable	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_ENABLE );
		//check work
		if ( 0 != (E_SoloRv_P2M_TRANS_ON & *Tv_PtrRegCtrl ) )
		{
			if ( 0 != ( E_SoloRv_P2M_WRITE & *Tv_PtrRegCtrl ) )
			{
				//copy sys to local
				Tv_PtrSr		=	(void*)*Tv_PtrRegTarAdr;
				Tv_PtrTg		=	(void*)((DWORD)m_PtrLmem + *Tv_PtrRegExtAdr);
				Tv_PitchSr	=	0xFFFFFC & ( *Tv_PtrRegCtrl >> 18 );			//pitch
				Tv_PitchTg	=	0xFFFFFC & ( *Tv_PtrRegExtCfg >> 18 );		//pitch
				//check decode data
				/*
				if ( *Tv_PtrRegExtAdr >= 0x7800000 )
				{
					//check p2m data
					if ( *((DWORD*)Tv_PtrSr) != E_GrEmlDevSoloVencStx )
					{
						//error
						DbgMsgPrint( "SoloEmu: Bad VdoDec P2m Work \n" );
					}
				}
				*/
			}
			else
			{
				//copy local to sys
				Tv_PtrSr		=	(void*)( (DWORD)m_PtrLmem + *Tv_PtrRegExtAdr );
				Tv_PtrTg		=	(void*)*Tv_PtrRegTarAdr;
				Tv_PitchSr	=	0xFFFFFC & ( *Tv_PtrRegExtCfg >> 18 );		//pitch
				Tv_PitchTg	=	0xFFFFFC & ( *Tv_PtrRegCtrl >> 18 );			//pitch
			}
			//check bad request
			if ( m_Is6110 || ( 0 == ( 0x7C & *Tv_PtrRegCtrl ) ) )
			{
				//bad address check
				if ( (0 != (0x0F & *Tv_PtrRegExtAdr) ) || (0 != (0x03 & *Tv_PtrRegTarAdr)) )
				{
					DbgMsgPrint(	"Cls_GrElmDevSolo %d : P2m bad address . pci = %x  local = %x \n ",
												*Tv_PtrRegTarAdr , *Tv_PtrRegExtAdr );
				}

				//transfer
				Tv_CopySize	=	( 0xFFFFF & *Tv_PtrRegExtCfg ) << 2;
				Tv_Repeat			=	(0x3FF & ( *Tv_PtrRegCtrl >> 10 )) + 1;		//repeat
				for ( Tv_WkIdx=0;Tv_WkIdx < Tv_Repeat;Tv_WkIdx++ )
				{
					GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, Tv_CopySize );
					//next
					Tv_PtrSr	=	(void*)( (DWORD)Tv_PtrSr + Tv_PitchSr );
					Tv_PtrTg	=	(void*)( (DWORD)Tv_PtrTg + Tv_PitchTg );
				}
				//check p2m write
				if ( 0 != ( E_SoloRv_P2M_WRITE & *Tv_PtrRegCtrl ) )
				{
					Tv_TrnsSize	=	(Tv_PitchTg * Tv_Repeat) + Tv_CopySize;
					//osg check
					LcP2mWkOsg( *Tv_PtrRegExtAdr, Tv_TrnsSize );
				}

				//clear transfer
				*Tv_PtrRegCtrl	=	*Tv_PtrRegCtrl & ~E_SoloRv_P2M_TRANS_ON;
				//do interrupt
				if ( 0 != (  E_SoloRv_INTR_P2M(A_P2mCh) & *Tv_PtrRegIrqEnable ) )
				{
					//check already irq occured
					if ( 0 == (  E_SoloRv_INTR_P2M(A_P2mCh) & *Tv_PtrRegIrqStat ) )
					{
						//interrupt occured
						*Tv_PtrRegIrqStat	=	*Tv_PtrRegIrqStat | E_SoloRv_INTR_P2M(A_P2mCh);
						//request interrupt
						m_IsIrqReq	=	TRUE;
					}
				}
			}
			else
			{
				//bad request - alpha try
				DbgMsgPrint(	"Cls_GrElmDevSolo %d : P2m alpha try on solo 6010. src = %x  tg = %x \n ", 
											(DWORD)Tv_PtrSr , (DWORD)Tv_PtrTg );
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::LcThrdIrq( void )
{
	// local -------------------
		BOOL	Tv_Result;
	// code --------------------
		//init
		Tv_Result	=	m_IsIrqReq;

		if ( m_IsIrqReq )
		{
			//irq occured
			m_IsIrqReq	=	FALSE;
			//do interrupt
			GrEmuMdlIrqOccure( E_GrElmDevSoloIrq );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcPaintReq( Ptr_GrRect A_PtrRect )
{
	// local -------------------
		Ptr_GrRect	Tv_PtrRc;
	// code --------------------
		if ( NULL != m_GdibDisp )
		{
			Tv_PtrRc	=	(Ptr_GrRect)GrBufQueGetInPtr( (Ptr_GrBufQue)V_GrElmDevSoloPntQue[m_LdevIdx] );
			if ( NULL != Tv_PtrRc )
			{
				//do convert
				m_GdibOsg->CvtTo( m_GdibDisp );

				//rqeust
				*Tv_PtrRc		=	*A_PtrRect;
				GrBufQueIn( (Ptr_GrBufQue)V_GrElmDevSoloPntQue[m_LdevIdx] );
			}
		}
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcP2mWkOsg( DWORD A_OsgOfs, DWORD A_TrnsSize )
{
	// local -------------------
		DWORD*	Tv_PtrRegOsgCfg;
		DWORD		Tv_OsgLmemPos;
		St_GrRect		Tv_Rc;
	// code --------------------
		Tv_PtrRegOsgCfg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_OSG_CONFIG );
		if ( 0 != ( E_SoloRv_VO_OSG_ON & *Tv_PtrRegOsgCfg ) )
		{
			//check in address
			Tv_OsgLmemPos	=	(*Tv_PtrRegOsgCfg & 0xFFFF) << 16;
			if ( ( Tv_OsgLmemPos <= A_OsgOfs) && ( (Tv_OsgLmemPos + E_GrDrvSoloLmemOsgSize) > A_OsgOfs ) )
			{
				//check exist display
				Tv_Rc.left		=	0;
				Tv_Rc.right		=	E_SoloBaseResX;
				Tv_Rc.top			=	A_OsgOfs / E_SoloOsgLmemPitch;
				Tv_Rc.bottom	=	((A_OsgOfs + A_TrnsSize) / E_SoloOsgLmemPitch) + 1;
				LcPaintReq( &Tv_Rc );
			}
		}
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::LcThrdI2c( void )
{
	// local -------------------
		BOOL	Tv_Result;
		DWORD*	Tv_PtrRegIrqEnable;
		DWORD*	Tv_PtrRegIrqStat;
		DWORD*	Tv_PtrRegI2cCtrl;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PtrRegIrqStat		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_STAT );
		Tv_PtrRegIrqEnable	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_ENABLE );
		Tv_PtrRegI2cCtrl	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_IIC_CTRL );
		//check irq enable
		if ( 0 != ( E_SoloRv_INTR_IIC & *Tv_PtrRegIrqEnable ) )
		{
			//check interrupt able
			if ( E_SoloRv_IIC_STAT_TRNS & *Tv_PtrRegI2cCtrl )
			{
				//transfer clean
				*Tv_PtrRegI2cCtrl	=	*Tv_PtrRegI2cCtrl & ~E_SoloRv_IIC_STAT_TRNS;
				//do ineterrupt
				*Tv_PtrRegIrqStat	=	*Tv_PtrRegIrqStat | E_SoloRv_INTR_IIC;
				m_IsIrqReq	=	TRUE;
				Tv_Result		=	TRUE;
				m_I2cIrqed	++;
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::LcThrdAdo( void )
{
	// local -------------------
		BOOL	Tv_Result;
		DWORD*	Tv_PtrRegIrqEnable;
		DWORD*	Tv_PtrRegIrqStat;
		DWORD*	Tv_PtrRegAdoCtl0;
		DWORD*	Tv_PtrRegAdoSta;
	// code --------------------
		Tv_Result	=	FALSE;
		Tv_PtrRegIrqStat		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_STAT );
		Tv_PtrRegIrqEnable	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_ENABLE );
		Tv_PtrRegAdoCtl0		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_AUDIO_CONTROL );
		Tv_PtrRegAdoSta			=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_AUDIO_STA );
		//check irq enable
		if ( 0 != ( E_SoloRv_INTR_G723 & *Tv_PtrRegIrqEnable ) )
		{
			//check interrupt able
			if ( E_SoloRv_AUDIO_ENABLE & *Tv_PtrRegAdoCtl0 )
			{
				//increase audio queue
				m_G723Period ++;
				if ( E_GrEmlDevSoloG723Period < m_G723Period )
				{
					//increase hw queue
					*Tv_PtrRegAdoSta	=	( *Tv_PtrRegAdoSta + 1 ) & 0x1F;
					//do ineterrupt
					*Tv_PtrRegIrqStat	=	*Tv_PtrRegIrqStat | E_SoloRv_INTR_G723;
					m_IsIrqReq	=	TRUE;
					Tv_Result		=	TRUE;

					//clear period
					m_G723Period	=	0;
				}
			}
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
BOOL	Cls_GrElmDevSolo::LcThrdVenc( void )
{
	// local -------------------
		BOOL	Tv_Result;
		DWORD	Tv_SubIdx;
		DWORD*	Tv_PtrReg;
		DWORD*	Tv_PtrFrm;
		DWORD		Tv_OrgPos;
		Def_GrTime	Tv_TimeSec;
		__u32		Tv_TimeMsec;
		//BOOL		Tv_IsSm;
		DWORD		Tv_FrmIdx;
		BYTE		Tv_EncScl;
		void*		Tv_PtrSr;
		void*		Tv_PtrTg;
		DWORD		Tv_AbleSize;
		DWORD		Tv_DataSize;
	// code --------------------
		Tv_Result	=	FALSE;

		m_EncPeriod ++;
		if ( E_GrEmlDevSoloVencPeriod <= m_EncPeriod )
		{
			//check record able
			if ( E_GrEmlDevSoloVencScaleCnt > m_EncScl[m_EncCh] )
			{
				//do encode
				Tv_OrgPos	=	m_EncAddPos;

				//get frame
				Tv_EncScl	=	2 - m_PtrFrm[m_EncCh]->Res;
				Tv_FrmIdx	=	m_FrmIdx[m_EncCh];
				Tv_SubIdx	=	0;
				if ( E_GrDvrAvTypVdoP == m_PtrFrm[m_EncCh]->Type )
				{
					Tv_SubIdx ++;
				}

				//make frame header
				Tv_PtrFrm	=	(DWORD*)( (DWORD)m_PtrEncBuf + m_EncAddPos );
				Tv_PtrFrm[0]	=	V_GrElmDevSoloEncVdoStrmTbl[Tv_EncScl][Tv_SubIdx].Stat[0] | (m_EncCh << 24) | m_PtrFrm[m_EncCh]->DataSize;
				Tv_PtrFrm[1]	=	V_GrElmDevSoloEncVdoStrmTbl[Tv_EncScl][Tv_SubIdx].Stat[1];
				Tv_TimeSec		=	GrTimeSysTimeGet( &Tv_TimeMsec );
				//Tv_TimeSec		=	GrTimeGet( &Tv_TimeMsec, &Tv_IsSm );
				Tv_PtrFrm[4]	=	m_PtrFrm[m_EncCh]->SkipCnt << 20;
				Tv_PtrFrm[5]	=	Tv_TimeSec;
				Tv_PtrFrm[6]	=	Tv_TimeMsec * 1000;

				//update status
				Tv_PtrReg		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_STATE(0) );
				Tv_PtrReg[0]	=	Tv_PtrFrm[0];
				Tv_PtrReg[1]	=	Tv_PtrFrm[1];
				m_EncAddPos	=	m_EncAddPos + E_SoloVencIdxTblSize;

				//mark motion map
				Tv_PtrReg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_CFG1 );
				if ( 1 == ( 0x03 & ((*Tv_PtrReg) >> 16) ) )
				{
					if ( m_EncBufSize <= m_EncAddPos )
					{
						m_EncAddPos	=	m_EncAddPos - m_EncBufSize;
					}
					
					Tv_PtrFrm	=	(DWORD*)( (DWORD)m_PtrEncBuf + m_EncAddPos );
					*Tv_PtrFrm	=	E_GrEmlDevSoloMtnMark;
					m_EncAddPos	=	m_EncAddPos + E_SoloEncMtnMapSize;
				}

				//mark header
				if ( m_EncBufSize <= m_EncAddPos )
				{
					m_EncAddPos	=	m_EncAddPos - m_EncBufSize;
				}

				//copy data
				Tv_PtrSr		=	(void*)( (DWORD)m_PtrFrm[m_EncCh] + sizeof(St_GrDvrAvHd) );
				Tv_PtrTg		=	(void*)( (DWORD)m_PtrEncBuf + m_EncAddPos );
				Tv_DataSize	=	(m_PtrFrm[m_EncCh]->DataSize + 0x1F) & (~0x1F);
				Tv_AbleSize	=	m_EncBufSize - m_EncAddPos;
				if ( Tv_AbleSize < Tv_DataSize )
				{
					//divide
					GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, Tv_AbleSize );
					Tv_PtrSr	=	(void*)( (DWORD)Tv_PtrSr + Tv_AbleSize );
					Tv_PtrTg	=	(void*)m_PtrEncBuf;
					m_EncAddPos	=	0;
					Tv_DataSize	=	Tv_DataSize - Tv_AbleSize;
					m_PtrFrm[m_EncCh]	=	(Ptr_GrDvrAvHd)( (DWORD)m_PtrFrm[m_EncCh] + Tv_AbleSize );
				}
				GrDumyCopyMem( Tv_PtrTg, Tv_PtrSr, Tv_DataSize );
				m_EncAddPos	=	m_EncAddPos + Tv_DataSize;
				if ( m_EncAddPos >= m_EncBufSize )
				{
					m_EncAddPos	=	0;
				}
				m_PtrFrm[m_EncCh]	=	(Ptr_GrDvrAvHd)( (DWORD)m_PtrFrm[m_EncCh] + Tv_DataSize + sizeof(St_GrDvrAvHd) );
				m_FrmIdx[m_EncCh] ++;
				if ( m_EmuVdoCnt <= m_FrmIdx[m_EncCh] )
				{
					m_FrmIdx[m_EncCh]	=	0;
					m_PtrFrm[m_EncCh]	=	(Ptr_GrDvrAvHd)m_EmuVdoBuf;
				}

				//now queue position
				Tv_PtrReg		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_STATE(11) );
				*Tv_PtrReg	=	m_EncQueIdx;
				Tv_PtrReg		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_MPEG4_QUE(m_EncQueIdx) );
				*Tv_PtrReg	=	Tv_OrgPos | (Tv_SubIdx << 29) | (m_EncCh << 24);
				//next queue position
				m_EncQueIdx =	(m_EncQueIdx + 1) & 0x0F;
				Tv_PtrReg		=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_MPEG4_QUE(m_EncQueIdx) );
				*Tv_PtrReg	=	m_EncAddPos;

				//next GOP
				m_EncGop[m_EncCh] ++;
				Tv_PtrReg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VE_CH_GOP(m_EncCh) );
				if ( m_EncGop[m_EncCh] >= (BYTE)(*Tv_PtrReg) )
				{
					m_EncGop[m_EncCh]	=	0;
				}

				//do interrupt
				Tv_PtrReg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_STAT );
				*Tv_PtrReg	=	*Tv_PtrReg | E_SoloRv_INTR_ENCODER;
				m_IsIrqReq	=	TRUE;

				//success
				Tv_Result	=	TRUE;
			}

			//next
			m_EncCh	++;
			m_EncCh	=	m_EncCh & 0x0F;

			m_EncPeriod	=	0;
		}
		
		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcEwrVdec( void )
{
	// local -------------------
		DWORD*	Tv_PtrReg;		
		DWORD*	Tv_PtrStrmHd;
		DWORD		Tv_CodeSize;
		DWORD*	Tv_PtrStrmTail;
	// code --------------------
		//check stream address
		Tv_PtrReg			=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VD_CODE_ADR );
		Tv_PtrStrmHd	=	(DWORD*)( (DWORD)m_PtrLmem + *Tv_PtrReg );
		Tv_PtrReg			=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_VD_IDX0 );
		Tv_CodeSize		=	0xFFFFF & *Tv_PtrReg;
		Tv_PtrStrmTail	=	(DWORD*)( (DWORD)Tv_PtrStrmHd + Tv_CodeSize - 4 );
		//check stream info
		/*
		if ( (E_GrEmlDevSoloVencStx != *Tv_PtrStrmHd) || (E_GrEmlDevSoloVencEtx != *Tv_PtrStrmTail) )
		{
			DbgMsgPrint( "decoder Stream bad!\n" );
		}
		*/

		//interrupt run
		Tv_PtrReg	=	(DWORD*)( (DWORD)m_PtrReg + E_SoloRa_INTR_STAT );
		*Tv_PtrReg	=	*Tv_PtrReg | E_SoloRv_INTR_DECODER;
		m_IsIrqReq	=	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrElmDevSolo::LcEmuAvLoad( void )
{
	// local -------------------
		DWORD		Tv_VdoSize;
		Cls_GrFileCtrl*	Tv_Fc;
		Ptr_GrDvrAvHd		Tv_PtrFrm;
		DWORD		Tv_LoadSize;
		DWORD		Tv_Fptr;
		BOOL		Tv_IsStart;
		void*		Tv_PtrData;
	// code --------------------
		//init
		Tv_VdoSize		=	0;
		m_EmuVdoCnt		=	0;
		Tv_IsStart		=	FALSE;

		Tv_Fc	=	(Cls_GrFileCtrl*)new Cls_GrFileCtrl( L"e:\\EmuGh\\avdata.dat", FALSE, FALSE, FALSE, TRUE );
		if ( Tv_Fc->IsOpened() )
		{
			//go first frame
			Tv_Fc->Seek( 0x200000 );

			while ( TRUE )
			{
				//chekc data finished
				if ( (Tv_VdoSize + sizeof(St_GrDvrAvHd)) >= E_GrElmDevSoloEmuVdoSize )
				{
					//finished
					break;
				}
				//load frame
				Tv_PtrFrm		=	(Ptr_GrDvrAvHd)( (DWORD)m_EmuVdoBuf + Tv_VdoSize );
				Tv_LoadSize	=	Tv_Fc->Read( Tv_PtrFrm, sizeof(St_GrDvrAvHd) );
				if ( Tv_LoadSize != sizeof(St_GrDvrAvHd) )
				{
					//data ended
					break;
				}
				//check cuffent frame
				if ( 0x5641444E == Tv_PtrFrm->Fcc )
				{
					//check video frame and channel
					if (	(0 == Tv_PtrFrm->Ch) && (E_GrDvrAvTypVdoP >= Tv_PtrFrm->Type) && 
								( Tv_IsStart || ( E_GrDvrAvTypVdoI == Tv_PtrFrm->Type ) )
							)
					{
						//copy contents
						Tv_PtrData	=	(void*)( (DWORD)Tv_PtrFrm + sizeof(St_GrDvrAvHd) );
						Tv_LoadSize	=	(Tv_PtrFrm->DataSize + 0x1F) & (~0x1F);
						if ( (Tv_VdoSize + sizeof(St_GrDvrAvHd) + Tv_LoadSize ) >= E_GrElmDevSoloEmuVdoSize )
						{
							//finished
							break;
						}
						if ( Tv_LoadSize != Tv_Fc->Read( Tv_PtrData, Tv_LoadSize ) )
						{
							//data finished
							break;
						}
						Tv_IsStart	=	TRUE;
						m_EmuVdoCnt ++;
						Tv_VdoSize	=	Tv_VdoSize + Tv_LoadSize + sizeof(St_GrDvrAvHd);
					}
					else
					{
						//skip contents
						Tv_Fptr	=	(DWORD)Tv_Fc->GetPos() + Tv_PtrFrm->DataSize;
						Tv_Fptr	=	(Tv_Fptr + 0x1F) & (~0x1F);
						if ( Tv_Fptr != (DWORD)Tv_Fc->Seek( Tv_Fptr ) )
						{
							//data end, finished
							break;
						}
					}
				}
				else
				{
					//bad stream
					break;
				}

				//next

			}

		}
		delete	Tv_Fc;
}
//--------------------------------------------------------------------

