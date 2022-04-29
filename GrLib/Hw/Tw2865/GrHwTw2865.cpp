/*
 tw2816 manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>

#include	<GrDebug.h>

#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<Hw/Tw2865/GrHwTw2865.h>
#include	<GrI2c.h>

//====================================================================
//const

#define E_GrHwTw2865EndTblAdr			0xFF

//====================================================================
//local type

typedef	struct St_GrHwTw2865RegItm 
{
	__u8		Adr;
	__u8		ValNtsc;
	__u8		ValPal;
}	*Ptr_R2HwTw2865RegItm;


//====================================================================
//local function

//====================================================================
//var

St_GrHwTw2865RegItm	V_R2HwTw2865Rtbl[]	=	
{
	//channel 1
	{	0x0E,   0x0F,   0x0F	},			/* 0x0F,   0x07 */	
	{	0x01,		0x00,		0x00	},
	{	0x02,   0x64,		0x64	},
	{	0x03,   0x13,		0x13	},
	{	0x04,   0x80,		0x80	},
	{	0x05,   0x80,		0x80	},
	{	0x06,   0x00,		0x00 	},
	{	0x07,   0x02,   0x12	},
	{	0x08,   0x12,   0x12	},
	{	0x09,   0xF2,   0x20	},
	{	0x0A,   0x0C,   0x0C	},
	{	0x0B,   0xD0,   0xD0	},
	{	0x0F,   0x7F,   0x7F	},

	//channel 2
	{	0x1E,   0x0F,   0x0F	},
	{	0x11,		0x00,		0x00	},
	{	0x12,   0x64,		0x64	},
	{	0x13,   0x13,		0x13	},
	{	0x14,   0x80,		0x80	},
	{	0x15,   0x80,		0x80	},
	{	0x16,   0x00,		0x00 	},
	{	0x17,   0x02,   0x12	},
	{	0x18,   0x12,   0x12	},
	{	0x19,   0xF2,   0x20	},
	{	0x1A,   0x0C,   0x0C	},
	{	0x1B,   0xD0,   0xD0	},
	{	0x1F,   0x7F,   0x7F	},

	//channel 3
	{	0x2E,   0x0F,   0x0F	},
	{	0x21,		0x00,		0x00	},
	{	0x22,   0x64,		0x64	},
	{	0x23,   0x13,		0x13	},
	{	0x24,   0x80,		0x80	},
	{	0x25,   0x80,		0x80	},
	{	0x26,   0x00,		0x00 	},
	{	0x27,   0x02,   0x12	},
	{	0x28,   0x12,   0x12	},
	{	0x29,   0xF2,   0x20	},
	{	0x2A,   0x0C,   0x0C	},
	{	0x2B,   0xD0,   0xD0	},
	{	0x2F,   0x7F,   0x7F	},

	//channel 4
	{	0x3E,   0x0F,   0x0F	},
	{	0x31,		0x00,		0x00	},
	{	0x32,   0x64,		0x64	},
	{	0x33,   0x13,		0x13	},
	{	0x34,   0x80,		0x80	},
	{	0x35,   0x80,		0x80	},
	{	0x36,   0x00,		0x00 	},
	{	0x37,   0x02,   0x12	},
	{	0x38,   0x12,   0x12	},
	{	0x39,   0xF2,   0x20	},
	{	0x3A,   0x0C,   0x0C	},
	{	0x3B,   0xD0,   0xD0	},
	{	0x3F,   0x7F,   0x7F	},

	//common
	//{	0x40,		0x00,		0x00	},	//not exist
	{	0x41,		0x40,		0xD4	},
	//{	0x42,		0x90,		0x90	},	//use default
	{	0x43,		0x68,		0x68	},
	{	0x44,		0x00,		0x00	},
	{	0x45,		0x38,		0x38	},
	{	0x46,		0x78,		0x77	},	//encoder y gain
	{	0x47,		0xB0,		0xB0	},	//encoder c gain
	{	0x48,		0xFF,		0xFF	},	//encoder s gain
	{	0x49,		0xFF,		0xFF	},	//encoder b gain
	{	0x4A,		0x77,		0x77	},
	{	0x4B,		0x00,		0x00	},
	{	0x4C,		0x00,		0x00	},
	{	0x4D,		0x00,		0x00	},
	{	0x4E,		0x00,		0x00	},
	{	0x4F,		0x00,		0x00	},

	{	0x50,		0x00,		0x00	},
	{	0x51,		0x00,		0x00	},
	{	0x52,		0x00,		0x00	},
	{	0x53,		0x00,		0x00	},
	{	0x54,		0x00,		0x00	},
	{	0x55,		0x00,		0x00	},
	{	0x56,		0x00,		0x00	},
	{	0x57,		0x00,		0x00	},
	{	0x58,		0x00,		0x00	},
	{	0x59,		0x00,		0x00	},
	{	0x5A,		0x00,		0x00	},
	{	0x5B,		0x00,		0x00	},
	{	0x5C,		0x00,		0x00	},
	{	0x5D,		0x00,		0x00	},
	{	0x5E,		0x00,		0x00	},
	{	0x5F,		0x00,		0x00	},

	{	0x60,		0x05,		0x05	},
	{	0x61,		0x03,		0x03	},
	{	0x62,		0x00,		0x00	},
	{	0x63,		0x00,		0x00	},
	{	0x64,		0x00,		0x00	},
	{	0x65,		0x00,		0x00	},
	{	0x66,		0x00,		0x00	},
	{	0x67,		0x00,		0x00	},
	{	0x68,		0x00,		0x00	},
	{	0x69,		0x00,		0x00	},
	{	0x6A,		0x00,		0x00	},
	{	0x6B,		0x00,		0x00	},
	{	0x6C,		0x00,		0x00	},
	{	0x6D,		0xFC,		0xFC	},	//clock invert
	{	0x6E,		0x21,		0x21	},
	{	0x6F,		0x43,		0x43	},

	{	0x70,		0x09,		0x09	},
	{	0x71,		0x00,		0x00	},
	{	0x72,		0x00,		0x00	},
	{	0x73,		0x01,		0x01	},
	{	0x74,		0x01,		0x01	},
	{	0x75,		0x03,		0x03	},
	{	0x76,		0xEF,		0xEF	},
	{	0x77,		0x03,		0x03	},
	{	0x78,		0xE9,		0xEA	},
	{	0x79,		0x03,		0x03	},
	{	0x7A,		0xD9,		0xD9	},
	{	0x7B,		0x15,		0x15	},
	{	0x7C,		0x15,		0x15	},
	{	0x7D,		0xE4,		0xE4	},
	{	0x7E,		0xA3,		0xA3	},
	{	0x7F,		0x80,		0x80	},

	{	0x80,		0x00,		0x00	},
	{	0x81,		0x02,		0x02	},
	{	0x82,		0x00,		0x00	},
	{	0x83,		0xCC,		0xCC	},
	{	0x84,		0x00,		0x00	},
	{	0x85,		0x80,		0x80	},
	{	0x86,		0x44,		0x44	},
	{	0x87,		0x50,		0x50	},
	{	0x88,		0x22,		0x22	},
	{	0x89,		0x01,		0x01	},
	{	0x8A,		0xD8,		0xD8	},
	{	0x8B,		0xBC,		0xBC	},
	{	0x8C,		0xB8,		0xB8	},
	{	0x8D,		0x44,		0x44	},
	{	0x8E,		0x38,		0x38	},
	{	0x8F,		0x00,		0x00	},

	{	0x90,		0x00,		0x00	},
	{	0x91,		0x78,		0x78	},
	{	0x92,		0x44,		0x44	},
	{	0x93,		0x3D,		0x3D	},
	{	0x94,		0x14,		0x14	},
	{	0x95,		0xA5,		0xA5	},
	{	0x96,		0xE2,		0xE2	},
	{	0x97,		0x05,		0x05	},
	{	0x98,		0x00,		0x00	},
	{	0x99,		0x28,		0x28	},
	{	0x9A,		0x44,		0x44	},
	{	0x9B,		0x44,		0x44	},
	{	0x9C,		0xA0,		0xA0	},
	{	0x9D,		0x90,		0x90	},
	{	0x9E,		0x52,		0x52	},
	{	0x9F,		0x00,		0x00	},		//clock delay

	{	0xA0,		0x08,		0x08	},
	{	0xA1,		0x08,		0x08	},
	{	0xA2,		0x08,		0x08	},
	{	0xA3,		0x08,		0x08	},
	{	0xA4,		0x1A,		0x1A	},
	{	0xA5,		0x1A,		0x1A	},
	{	0xA6,		0x1B,		0x1A	},
	{	0xA7,		0x1A,		0x1A	},
	{	0xA8,		0x40,		0x40	},	//horizontail filter 1,2
	{	0xA9,		0x40,		0x40	},	//horizontail filter 3,4
	{	0xAA,		0x00,		0x00	},
	{	0xAB,		0xF0,		0xF0	},
	{	0xAC,		0xF0,		0xF0	},
	{	0xAD,		0xF0,		0xF0	},
	{	0xAE,		0xF0,		0xF0	},
	{	0xAF,		0x66,		0x44	},	//vertical filter 1,2

	{	0xB0,		0x66,		0x33	},	//vertical filter 3,4
	{	0xB1,		0x4A,		0x4A	},
	{	0xB2,		0x00,		0x00	},
	{	0xB3,		0xFF,		0xFF	},
	{	0xB4,		0xEF,		0xEF	},
	{	0xB5,		0xEF,		0xEF	},
	{	0xB6,		0xEF,		0xEF	},
	{	0xB7,		0xEF,		0xEF	},
	{	0xB8,		0xFF,		0xFF	},
	{	0xB9,		0xE7,		0xE7	},
	{	0xBA,		0xE9,		0xE9	},
	{	0xBB,		0xE9,		0xE9	},
	{	0xBC,		0xEB,		0xE9	},
	{	0xBD,		0xFF,		0xFF	},
	{	0xBE,		0xD6,		0xD7	},
	{	0xBF,		0xD8,		0xD8	},

	{	0xC0,		0xD8,		0xD9	},
	{	0xC1,		0xD7,		0xD8	},
	{	0xC2,		0x00,		0x00	},
	{	0xC3,		0x00,		0x00	},
	{	0xC4,		0x00,		0x00	},
	{	0xC5,		0x00,		0x00	},
	{	0xC6,		0x00,		0x00	},
	{	0xC7,		0x00,		0x00	},
	{	0xC8,		0x00,		0x00	},
	{	0xC9,		0x00,		0x00	},
	{	0xCA,		0x55,		0x55	},
	{	0xCB,		0x00,		0x00	},
	{	0xCC,		0xB1,		0xB1	},
	{	0xCD,		0xE4,		0xE4	},
	{	0xCE,		0x00,		0x00	},
	{	0xCF,		0x80,		0x80	},	

	{	0xD0,		0x88,		0x88	},
	{	0xD1,		0x88,		0x88	},
	{	0xD2,		0x03,		0x03	},		//16 channel audio
	{	0xD3,		0x20,		0x20	},
	{	0xD4,		0x64,		0x64	},
	{	0xD5,		0xA8,		0xA8	},
	{	0xD6,		0xEC,		0xEC	},
	{	0xD7,		0x31,		0x31	},
	{	0xD8,		0x75,		0x75	},
	{	0xD9,		0xB9,		0xB9	},
	{	0xDA,		0xFD,		0xFD	},
	{	0xDB,		0xD2,		0xD2	},
	{	0xDC,		0x00,		0x00	},
	{	0xDD,		0x00,		0x00	},
	{	0xDE,		0x00,		0x00	},
	{	0xDF,		0x80,		0x80	},	

	{	0xE0,		0x1F,		0x1F	},		//output audio select - default off
	{	0xE1,		0xC0,		0xC0	},
	{	0xE2,		0xAA,		0xAA	},
	{	0xE3,		0xAA,		0xAA	},
	{	0xE4,		0x00,		0x00	},
	{	0xE5,		0x11,		0x11	},
	{	0xE6,		0x00,		0x00	},
	{	0xE7,		0x00,		0x00	},
	{	0xE8,		0x11,		0x11	},
	{	0xE9,		0x00,		0x00	},
	{	0xEA,		0x00,		0x00	},
	{	0xEB,		0x11,		0x11	},
	{	0xEC,		0x00,		0x00	},
	{	0xED,		0x00,		0x00	},
	{	0xEE,		0x11,		0x11	},
	{	0xEF,		0x00,		0x00	},

	{	0xF0,		0x83,		0x83	},
	{	0xF1,		0xB5,		0xB5	},
	{	0xF2,		0x09,		0x09	},
	{	0xF3,		0x78,		0x00	},
	{	0xF4,		0x85,		0xA0	},
	{	0xF5,		0x00,		0x00	},
	{	0xF6,		0x01,		0x01	},
	{	0xF7,		0x20,		0x20	},
	{	0xF8,		0xC4,		0xC4	},	
	{	0xF9,		0x51,		0x51	},	
	{	0xFA,		0x40,		0x40	},
	{	0xFB,		0xC3,		0xC3	},	//interrupt enable - default 0x2F
	{	0xFC,		0xFF,		0xFF	},
	{	0xFD,		0xF0,		0xF0	},

	{	E_GrHwTw2865EndTblAdr,	0,	0	}
	
};


//====================================================================
Cls_GrHwTw2865::Cls_GrHwTw2865( __u32 A_Stage, __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_ClkDly, BOOL8 A_IsEncSvideo )
{
	// local -------------------
		__u8	Tv_WkIdx;
	// code --------------------
		m_I2cCh			=	A_I2cCh;
		m_I2cRtSlot	=	A_I2cRtSlot;
		m_Stage			=	A_Stage;
		m_ChipAdr		=	(__u8)A_Adr;
		m_ClkDly		=	A_ClkDly;
		m_IsEncSvideo	=	A_IsEncSvideo;

		//backup registrer value
		m_RvAgcEn				=	0x00;
		m_RvAgcGain[0]	=	0xF0;
		m_RvAgcGain[1]	=	0xF0;
		m_RvAgcGain[2]	=	0xF0;
		m_RvAgcGain[3]	=	0xF0;

		m_IsSigStatReq	=	FALSE;	
		m_SigStatLast		=	0xFF;
		for ( Tv_WkIdx = 0; Tv_WkIdx < 4; Tv_WkIdx++ )
		{
			m_IsVideoReq[Tv_WkIdx]		= FALSE;
		}		
		m_IsVideoLast		= 0;
		m_IsLiveReq			= FALSE;
		m_IsLive				= TRUE;
}
//--------------------------------------------------------------------
Cls_GrHwTw2865::~Cls_GrHwTw2865()
{

}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::WriteReg( __u8 A_RegAdr, __u8 A_RegVal )
{
		GrI2cStdWrite( m_I2cCh, m_ChipAdr, A_RegAdr, &A_RegVal, 1 );
}
//--------------------------------------------------------------------
__u8	Cls_GrHwTw2865::ReadReg( __u8 A_RegAdr )
{
	// local -------------------
		__u8	Tv_Result;
	// code --------------------
		Tv_Result	=	0xFF;

		if ( GrI2cStdRead( m_I2cCh, m_ChipAdr, A_RegAdr, 1, m_I2cRtSlot ) )
		{
			GrI2cSlotRead( m_I2cCh, m_I2cRtSlot, &Tv_Result, TRUE );
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2865::SetSigMode(	BOOL8 A_IsPal )
{
	// local -------------------
		Ptr_R2HwTw2865RegItm	Tv_PtrRtbl;
		__u8		Tv_Rval;
	// code --------------------
		//init
		Tv_PtrRtbl	=	V_R2HwTw2865Rtbl;

		while ( E_GrHwTw2865EndTblAdr != Tv_PtrRtbl->Adr )
		{
			//get signal type value
			Tv_Rval		=	Tv_PtrRtbl->ValNtsc;
			if ( A_IsPal )
			{
				Tv_Rval		=	Tv_PtrRtbl->ValPal;
			}

			//patch
			switch( Tv_PtrRtbl->Adr )
			{
				case	E_Tw2865RadrClkOut1Delay:
				case	E_Tw2865RadrClkOut2Delay:
				case	E_Tw2865RadrClkOut3Delay:
				case	E_Tw2865RadrClkOut4Delay:
					Tv_Rval	=	m_ClkDly | (m_ClkDly << 4);
					break;
				case	E_Tw2865RadrMasterCtrl:
					//last audio stage
					if ( E_GrHwTw2865StageFirst == m_Stage )
					{
						Tv_Rval	=	0xD1;
					}
					break;
				case	E_Tw2865RadrEncOut:
					if ( m_IsEncSvideo )
					{
						Tv_Rval	=	0xC0;
					}
					break;
			}
			
			//write register
			WriteReg( Tv_PtrRtbl->Adr, Tv_Rval );
			//next
			Tv_PtrRtbl ++;
		}

		/*
		if ( !A_IsPal )
		{
			WriteReg( 0x09, 0xF2 );
		}
		*/

		//update mode
		m_IsPal		=	A_IsPal;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwTw2865::IsSigPal( void )
{
		return	m_IsPal;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwTw2865::IcTest( void )
{
	// local -------------------
	// code --------------------
		return	TRUE;
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::SetBright(__u8 A_Ch, __u8 A_BriVal)
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch << 4) + E_Tw2865RadrBright1;
		WriteReg(Tv_RegAdr, A_BriVal);
}
//--------------------------------------------------------------------
void Cls_GrHwTw2865::SetContrast(__u8 A_Ch, __u8 A_ConVal)
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch << 4) + E_Tw2865RadrContrast1;
		WriteReg(Tv_RegAdr, A_ConVal);
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2865::AdoSetOutMux( __u8 A_Ch )
{
		WriteReg( E_Tw2865RadrMixOutSel, A_Ch );
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2865::AdoSetOutVol( __u8 A_Vol )
{
	// local -------------------
		__u8		Tv_Rval;
	// code --------------------
		Tv_Rval		=	( (A_Vol & 0x0F) << 4 );
		WriteReg( E_Tw2865RadrAoGain, Tv_Rval );
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2865::SetHue( __u8 A_Ch, __u8 A_Val )
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch << 4) + E_Tw2865RadrHue1;
		WriteReg(Tv_RegAdr, A_Val);
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::SetClkDly(__u8 A_Delay)
{
		// local -------------------
			__u8		Tv_Rval;
		// code --------------------
			//init
			m_ClkDly		= A_Delay;

			//patch clock delay
			Tv_Rval	=	m_ClkDly | (m_ClkDly << 4);

			//write register
			WriteReg( E_Tw2865RadrClkOut1Delay, Tv_Rval );
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::AgcSet( __u8 A_Ch, BOOL8 A_IsOn, __u16 A_Value )
{
	// local -------------------
	// code --------------------
		//enable set
		if ( A_IsOn )
		{
			m_RvAgcEn	=	m_RvAgcEn & ~(1 << (4 + A_Ch));
		}
		else
		{
			m_RvAgcEn	=	m_RvAgcEn | (1 << (4 + A_Ch));
		}
		//value
		if ( 0 == (0x100 & A_Value) )
		{
			m_RvAgcEn	=	m_RvAgcEn & ~(1 << A_Ch);
		}
		else
		{
			m_RvAgcEn	=	m_RvAgcEn | (1 << A_Ch);
		}
		WriteReg( E_Tw2865RadrAgcEn, m_RvAgcEn );
		//gain value
		m_RvAgcGain[A_Ch]	=	(__u8)( 0xFF & A_Value );
		WriteReg( E_Tw2865RadrAgcGain1 + A_Ch, m_RvAgcGain[A_Ch] );
}
//--------------------------------------------------------------------
__u8	Cls_GrHwTw2865::SigDetMapGet( void )
{
	// local -------------------
		__u8	Tv_Rval;
	// code --------------------
		if ( m_IsSigStatReq )
		{
			if ( GrI2cSlotRead( m_I2cCh, m_I2cRtSlot, &Tv_Rval, FALSE ) )
			{
				m_IsSigStatReq	=	FALSE;
				m_SigStatLast	=	Tv_Rval;
			}
		}
		//request next
		if ( !m_IsSigStatReq )
		{
			m_IsSigStatReq	=	GrI2cStdRead( m_I2cCh, m_ChipAdr, E_Tw2865RadrAvDetStat, 1, m_I2cRtSlot );
		}
		return	m_SigStatLast;
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::EncYgainSet( __u8 A_Val )
{
		WriteReg( 0x46, A_Val );
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::EncCgainSet( __u8 A_Val )
{
		WriteReg( 0x47, A_Val );
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::EncSgainSet( __u8 A_Val )
{
		WriteReg( 0x48, A_Val );
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2865::EncBgainSet( __u8 A_Val )
{
		WriteReg( 0x49, A_Val );
}
//--------------------------------------------------------------------
__u8 Cls_GrHwTw2865::VideoSigStatus( void )
{
	// local -------------------		
		__u8	Tv_WkIdx;
		__u8	Tv_Radr;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Radr	=	E_Tw2865RadrCh1;
		for (Tv_WkIdx=0;Tv_WkIdx < 4;Tv_WkIdx++)
		{
			// 
			if ( m_IsVideoReq[Tv_WkIdx] )
			{
				if ( GrI2cSlotRead( m_I2cCh, m_I2cRtSlot + Tv_WkIdx , &Tv_Rval, FALSE ) )
				{
					m_IsVideoReq[Tv_WkIdx] = FALSE;
					if ( 0x84 & Tv_Rval )
					//if ( 0x84 == ( 0x84 & Tv_Rval ) )								
					//if ( 0x04 & Tv_Rval )
					//if ( 0x80 & Tv_Rval )
					{
						m_IsVideoLast	= m_IsVideoLast | ( 1 << ( Tv_WkIdx ));
						//DbgMsgPrint( "NSG-%d:%d\n",m_ChipAdr,Tv_WkIdx );
					}
					else
					{
						m_IsVideoLast	=	m_IsVideoLast & (~(1 << Tv_WkIdx));
						//DbgMsgPrint( "SIG-%d:%d\n",m_ChipAdr,Tv_WkIdx );
					}					
				}
			}
			else
			{
				m_IsVideoReq[Tv_WkIdx] = GrI2cStdRead( m_I2cCh, m_ChipAdr, Tv_Radr, 1, m_I2cRtSlot + Tv_WkIdx  );
			}
			// next
			Tv_Radr	=	Tv_Radr + 0x10;
		}

		return 	m_IsVideoLast;
}
//--------------------------------------------------------------------
BOOL8 Cls_GrHwTw2865::IsLiveDev()
{
	// local -------------------		
		BOOL8		Tv_Result;
		__u8		Tv_Rval;
	// code --------------------
		Tv_Result		= TRUE;

#ifdef LINUX_APP
		if(m_IsLive)
		{
			if(m_IsLiveReq)
			{
				if(GrI2cSlotRead(m_I2cCh, m_I2cRtSlot + 4, &Tv_Rval, FALSE))
				{
					m_IsLiveReq		= FALSE;
					Tv_Rval = Tv_Rval >> 3;
					if(0x18 != Tv_Rval)
					{
						Tv_Result		= FALSE;
						m_IsLive		= FALSE;
					}
				}
			}
			else
			{
				m_IsLiveReq		= GrI2cStdRead(m_I2cCh, m_ChipAdr, 0xFF, 1, m_I2cRtSlot + 4);
			}
		}
		else
		{
			Tv_Result = FALSE;
		}
#endif

		return Tv_Result;
}
//--------------------------------------------------------------------

