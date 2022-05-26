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

#include	<Hw/Tw2815/GrHwTw2815.h>
#include	<GrI2c.h>

//====================================================================
//const

#define E_GrHwTw2815EndTblAdr			0xFF

//====================================================================
//local type

typedef	struct St_GrHwTw2815RegItm 
{
	__u8		Adr;
	__u8		ValNtsc;
	__u8		ValPal;
}	*Ptr_R2HwTw2815RegItm;


//====================================================================
//local function

//====================================================================
//var

St_GrHwTw2815RegItm	V_R2HwTw2815Rtbl[]	=	
{
	/*	channel 1		*/
	{	0x01,		0xC8,		0x88	},				  
	{	0x02,   (0xFF & E_GrHwTw2815DfltNtscHdelay), (0xFF & E_GrHwTw2815DfltPalHdelay)	},
	{	0x03,   (0xff & E_GrHwTw2815DfltHactive),	(0xff & E_GrHwTw2815DfltHactive)	},
	{	0x04,   (0xff & E_GrHwTw2815DfltVdelayNtsc), (0xff & E_GrHwTw2815DfltVdelayPal)	},
	{	0x05,   (0xff & E_GrHwTw2815DfltVactiveNtsc), (0xff & E_GrHwTw2815DfltVactivePal)	},
	{	0x06,   ((0x03 & (E_GrHwTw2815DfltNtscHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) | 
						((0x01 & (E_GrHwTw2815DfltVdelayNtsc>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactiveNtsc>>8))<<5), 
						((0x03 & (E_GrHwTw2815DfltPalHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) |
						((0x01 & (E_GrHwTw2815DfltVdelayPal>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactivePal>>8))<<5) 	},
	{	0x07,   0x80,   0x80	},
	{	0x08,   0x80,   0x80	},
	{	0x09,   0x80,   0x80	},
	{	0x0A,   0x80,   0x80	},
	{	0x0B,   0x02,   0x82	},
	{	0x0C,   0x06,   0x06	},
	{	0x0D,   0x05,   0x05	},
	{	0x0E,   0x11,   0x11	},
		
	/*	channel 2		*/
	{	0x11,		0xC8,		0x88	},				  
	{	0x12,   (0xFF & E_GrHwTw2815DfltNtscHdelay), (0xFF & E_GrHwTw2815DfltPalHdelay)	},
	{	0x13,   (0xff & E_GrHwTw2815DfltHactive),	(0xff & E_GrHwTw2815DfltHactive)	},
	{	0x14,   (0xff & E_GrHwTw2815DfltVdelayNtsc), (0xff & E_GrHwTw2815DfltVdelayPal)	},
	{	0x15,   (0xff & E_GrHwTw2815DfltVactiveNtsc), (0xff & E_GrHwTw2815DfltVactivePal)	},
	{	0x16,   ((0x03 & (E_GrHwTw2815DfltNtscHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) | 
						((0x01 & (E_GrHwTw2815DfltVdelayNtsc>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactiveNtsc>>8))<<5), 
						((0x03 & (E_GrHwTw2815DfltPalHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) |
						((0x01 & (E_GrHwTw2815DfltVdelayPal>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactivePal>>8))<<5) 	},
	{	0x17,   0x80,   0x80	},
	{	0x18,   0x80,   0x80	},
	{	0x19,   0x80,   0x80	},
	{	0x1A,   0x80,   0x80	},
	{	0x1B,   0x02,   0x82	},
	{	0x1C,   0x06,   0x06	},
	{	0x1D,   0x04,   0x04	},
	{	0x1E,   0x11,   0x11	},

	/*	channel 3		*/
	{	0x21,		0xC8,		0x88	},				  
	{	0x22,   (0xFF & E_GrHwTw2815DfltNtscHdelay), (0xFF & E_GrHwTw2815DfltPalHdelay)	},
	{	0x23,   (0xff & E_GrHwTw2815DfltHactive),	(0xff & E_GrHwTw2815DfltHactive)	},
	{	0x24,   (0xff & E_GrHwTw2815DfltVdelayNtsc), (0xff & E_GrHwTw2815DfltVdelayPal)	},
	{	0x25,   (0xff & E_GrHwTw2815DfltVactiveNtsc), (0xff & E_GrHwTw2815DfltVactivePal)	},
	{	0x26,   ((0x03 & (E_GrHwTw2815DfltNtscHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) | 
						((0x01 & (E_GrHwTw2815DfltVdelayNtsc>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactiveNtsc>>8))<<5), 
						((0x03 & (E_GrHwTw2815DfltPalHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) |
						((0x01 & (E_GrHwTw2815DfltVdelayPal>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactivePal>>8))<<5) 	},
	{	0x27,   0x80,   0x80	},
	{	0x28,   0x80,   0x80	},
	{	0x29,   0x80,   0x80	},
	{	0x2A,   0x80,   0x80	},
	{	0x2B,   0x02,   0x82	},
	{	0x2C,   0x06,   0x06	},
	{	0x2D,   0x07,   0x07	},
	{	0x2E,   0x11,   0x11	},

	/*	channel 4		*/
	{	0x31,		0xC8,		0x88	},				  
	{	0x32,   (0xFF & E_GrHwTw2815DfltNtscHdelay), (0xFF & E_GrHwTw2815DfltPalHdelay)	},
	{	0x33,   (0xff & E_GrHwTw2815DfltHactive),	(0xff & E_GrHwTw2815DfltHactive)	},
	{	0x34,   (0xff & E_GrHwTw2815DfltVdelayNtsc), (0xff & E_GrHwTw2815DfltVdelayPal)	},
	{	0x35,   (0xff & E_GrHwTw2815DfltVactiveNtsc), (0xff & E_GrHwTw2815DfltVactivePal)	},
	{	0x36,   ((0x03 & (E_GrHwTw2815DfltNtscHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) | 
						((0x01 & (E_GrHwTw2815DfltVdelayNtsc>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactiveNtsc>>8))<<5), 
						((0x03 & (E_GrHwTw2815DfltPalHdelay>>8))<<0) | ((0x03 & (E_GrHwTw2815DfltHactive>>8))<<2) |
						((0x01 & (E_GrHwTw2815DfltVdelayPal>>8))<<4) | ((0x01 & (E_GrHwTw2815DfltVactivePal>>8))<<5) 	},
	{	0x37,   0x80,   0x80	},
	{	0x38,   0x80,   0x80	},
	{	0x39,   0x80,   0x80	},
	{	0x3A,   0x80,   0x80	},
	{	0x3B,   0x02,   0x82	},
	{	0x3C,   0x06,   0x06	},
	{	0x3D,   0x06,   0x06	},
	{	0x3E,   0x11,   0x11	},

	/*	common		*/
	{	0x41,		0x00,		0x00	},
	{	0x42,   0x00,   0x00	},
  { 0x43,   0xC0,   0xC0	},
  { 0x44,   0x45,   0x45	},
  { 0x45,   0xA0,   0xA0	},
  { 0x46,   0xD0,   0xD0	},
  { 0x47,   0x2F,   0x2F	},
  { 0x48,   0x64,   0x64	},
  { 0x49,		0x80,   0x80	},
  { 0x4A,   0x80,   0x80	},
  { 0x4B,   0x82,   0x82	},
  { 0x4C,   0x82,   0x82	},
  { 0x4D,   E_GrHwTw2815ClkDelayP | (E_GrHwTw2815ClkDelayN<<4), 
						E_GrHwTw2815ClkDelayP | (E_GrHwTw2815ClkDelayN<<4) },
  { 0x4E,   0x00,   0x00	},
  { 0x4F,   0x00,   0x00	},
  { 0x50,   0x00,   0x00	},
  { 0x51,		0x0F,   0x0F 	},
  { 0x52,   0x05,   0x05 	},
  { 0x53,   0x00,   0x00 	},
  { 0x54,   0x00,   0x00 	},
  { 0x55,   0x80,   0x80 	},
  { 0x56,   0x06,   0x06 	},
  { 0x57,   0x00,   0x00 	},
  { 0x58,   0x00,   0x00 	},
  { 0x5B,   0x0F,   0x0F 	},
  { 0x5C,   0x8F,   0x8F 	},
  { 0x60,   0x88,   0x88 	},
  { 0x61,   0x88,   0x88 	},
  { 0x62,   0x04,   0x04 	},
	{ 0x63,   0x03,   0x03 	},
	{ 0x64,   0x20,   0x20 	},
	{ 0x65,   0x64,   0x64 	},
	{ 0x66,   0xA8,   0xA8 	},
	{ 0x67,   0xEC,   0xEC 	},
	{ 0x68,   0x31,   0x31 	},
	{ 0x69,   0x75,   0x75 	},
	{ 0x6A,   0xB9,   0xB9 	},
	{ 0x6B,   0xFD,   0xFD 	},
  { 0x6C,   0x24,   0x24 	},
  { 0x6D,   0x00,   0x00 	},
  { 0x6E,   0x88,   0x88 	},
  { 0x6F,   0x88,   0x88 	},
  { 0x70,   0x88,   0x88 	},
  { 0x71,   0x11,   0x11 	},
  { 0x72,   0x40,   0x40 	},
  { 0x73,   0x88,   0x88 	},
  { 0x74,   0x88,   0x88 	},
  { 0x75,   0x00,   0x00 	},

	//end of table
	{	E_GrHwTw2815EndTblAdr,	0,	0	}
};

//====================================================================
Cls_GrHwTw2815::Cls_GrHwTw2815( __u32 A_Stage, __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_ClkDly )
{
		m_I2cCh			=	A_I2cCh;
		m_I2cRtSlot	=	A_I2cRtSlot;
		m_Stage			=	A_Stage;
		m_ChipAdr		=	(__u8)A_Adr;
		m_ClkDly		=	A_ClkDly;

		if ( IcTest() )
		{
			DbgMsgPrint( "tw2815 found at %x.\n", A_Adr );
		}
		else
		{
			DbgMsgPrint( "tw2815 not found at %x.\n", A_Adr );
		}
}
//--------------------------------------------------------------------
Cls_GrHwTw2815::~Cls_GrHwTw2815()
{

}
//--------------------------------------------------------------------
void	Cls_GrHwTw2815::WriteReg( __u8 A_RegAdr, __u8 A_RegVal )
{
		GrI2cStdWrite( m_I2cCh, m_ChipAdr, A_RegAdr, &A_RegVal, 1 );
}
//--------------------------------------------------------------------
__u8	Cls_GrHwTw2815::ReadReg( __u8 A_RegAdr )
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
void		Cls_GrHwTw2815::SetSigMode(	BOOL8 A_IsPal )
{
	// local -------------------
		Ptr_R2HwTw2815RegItm	Tv_PtrRtbl;
		__u8		Tv_Rval;
	// code --------------------
		//init
		Tv_PtrRtbl	=	V_R2HwTw2815Rtbl;

		while ( E_GrHwTw2815EndTblAdr != Tv_PtrRtbl->Adr )
		{
			//check signal type
			Tv_Rval		=	Tv_PtrRtbl->ValNtsc;
			if ( A_IsPal )
			{
				Tv_Rval		=	Tv_PtrRtbl->ValPal;
			}
			//patch stage
			if (E_Tw2815Ra_RM_CON == Tv_PtrRtbl->Adr )
			{
				Tv_Rval	=	Tv_Rval | ((__u8)m_Stage << 6);
			}
			//patch clock delay
			if (E_Tw2815Ra_CLK_CON == Tv_PtrRtbl->Adr )
			{
				Tv_Rval	=	m_ClkDly | (m_ClkDly << 4);
			}

			//write register
			WriteReg( Tv_PtrRtbl->Adr, Tv_Rval );
			//next
			Tv_PtrRtbl ++;
		}

		//update mode
		m_IsPal		=	A_IsPal;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwTw2815::IsSigPal( void )
{
		return	m_IsPal;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwTw2815::IcTest( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		Tv_Rval		=	ReadReg( E_Tw2815Ra_DEV_ID );
		if ( E_Tw2815DevId == Tv_Rval )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2815::SetBright(__u8 A_Ch, __u8 A_BriVal)
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch * 0x10) + E_Tw2815Ra_CH0_BRT;
		WriteReg(Tv_RegAdr, A_BriVal);
}
//--------------------------------------------------------------------
void Cls_GrHwTw2815::SetContrast(__u8 A_Ch, __u8 A_ConVal)
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch * 0x10) + E_Tw2815Ra_CH0_CONT;
		WriteReg(Tv_RegAdr, A_ConVal);
}
//--------------------------------------------------------------------
BOOL8	Cls_GrHwTw2815::ReqSigStatGet( void )
{
		return	GrI2cStdRead( m_I2cCh, m_ChipAdr, E_Tw2815Ra_STATE_DET, 1, m_I2cRtSlot );
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2815::AdoSetOutMux( __u32 A_Ch )
{
		WriteReg( 0x71, (__u8)A_Ch );
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2815::AdoSetOutVol( __u32 A_Vol )
{
	// local -------------------
		__u8		Tv_Rval;
	// code --------------------
		Tv_Rval		=	(__u8)(0x08 | ( (A_Vol & 0x0F) << 4 ));
		WriteReg( 0x70, Tv_Rval );
}
//--------------------------------------------------------------------
void		Cls_GrHwTw2815::SetSat( __u8 A_Ch, __u8 A_ConVal )
{
	// var --------------------
		__u8		Tv_RegAdr;
	// code -------------------
		Tv_RegAdr = (A_Ch * 0x10) + E_Tw2815Ra_CH0_SAT;
		WriteReg(Tv_RegAdr, A_ConVal);
}
//--------------------------------------------------------------------
void	Cls_GrHwTw2815::SetClkDly(__u8 A_Delay)
{
		// local -------------------
			__u8		Tv_Rval;
		// code --------------------
			//init
			m_ClkDly		= A_Delay;

			//patch clock delay
			Tv_Rval	=	m_ClkDly | (m_ClkDly << 4);

			//write register
			WriteReg( E_Tw2815Ra_CLK_CON, Tv_Rval );
}
//--------------------------------------------------------------------


