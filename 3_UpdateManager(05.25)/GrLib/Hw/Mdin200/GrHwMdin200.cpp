/*
 mdin200 manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>

#include	<GrDebug.h>

#include	<GrDumyTool.h>
#include	<GrStrTool.h>
#include	<GrProcess.h>

#include	<Hw/Mdin200/GrHwMdin200.h>
#include	<GrI2c.h>

//====================================================================
//const


//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var

__u16 V_GrHwMdin200Cwd2MFC_Ctbl[64][2] = 
{
	{ 0x0800, 0x0000 },
	{ 0x07FF, 0x0FE9 },
	{ 0x07FC, 0x0FD3 },
	{ 0x07F6, 0x0FBF },
	{ 0x07EF, 0x0FAC },
	{ 0x07E5, 0x0F9A },
	{ 0x07DA, 0x0F8A },
	{ 0x07CC, 0x0F7B },
	{ 0x07BD, 0x0F6D },
	{ 0x07AC, 0x0F60 },
	{ 0x0799, 0x0F55 },
	{ 0x0785, 0x0F4B },
	{ 0x076F, 0x0F42 },
	{ 0x0757, 0x0F3A },
	{ 0x073E, 0x0F33 },
	{ 0x0724, 0x0F2D },
	{ 0x0708, 0x0F28 },
	{ 0x06EB, 0x0F24 },
	{ 0x06CC, 0x0F21 },
	{ 0x06AD, 0x0F1F },
	{ 0x068C, 0x0F1D },
	{ 0x066A, 0x0F1C },
	{ 0x0647, 0x0F1D },
	{ 0x0624, 0x0F1D },
	{ 0x05FF, 0x0F1F },
	{ 0x05D9, 0x0F21 },
	{ 0x05B3, 0x0F24 },
	{ 0x058C, 0x0F27 },
	{ 0x0564, 0x0F2B },
	{ 0x053C, 0x0F30 },
	{ 0x0513, 0x0F35 },
	{ 0x04EA, 0x0F3A },
	{ 0x04C0, 0x0F40 },
	{ 0x0496, 0x0F46 },
	{ 0x046B, 0x0F4D },
	{ 0x0441, 0x0F54 },
	{ 0x0416, 0x0F5B },
	{ 0x03EB, 0x0F62 },
	{ 0x03BF, 0x0F69 },
	{ 0x0394, 0x0F71 },
	{ 0x0369, 0x0F79 },
	{ 0x033E, 0x0F81 },
	{ 0x0313, 0x0F89 },
	{ 0x02E8, 0x0F91 },
	{ 0x02BE, 0x0F99 },
	{ 0x0294, 0x0FA1 },
	{ 0x026A, 0x0FA9 },
	{ 0x0241, 0x0FB0 },
	{ 0x0218, 0x0FB8 },
	{ 0x01F0, 0x0FBF },
	{ 0x01C8, 0x0FC7 },
	{ 0x01A1, 0x0FCD },
	{ 0x017B, 0x0FD4 },
	{ 0x0156, 0x0FDA },
	{ 0x0131, 0x0FE0 },
	{ 0x010E, 0x0FE6 },
	{ 0x00EB, 0x0FEB },
	{ 0x00C9, 0x0FF0 },
	{ 0x00A9, 0x0FF4 },
	{ 0x008A, 0x0FF7 },
	{ 0x006B, 0x0FFA },
	{ 0x004E, 0x0FFD },
	{ 0x0033, 0x0FFF },
	{ 0x0019, 0x0000 }
};

//====================================================================
Cls_GrHwMdin200::Cls_GrHwMdin200( __u8 A_I2cCh, __u32 A_Adr, __u8 A_Slot )
{
		m_I2cCh			=	A_I2cCh;
		m_ChipAdr		=	(__u8)A_Adr;
		m_I2cSlot		=	A_Slot;

		m_OutMode		=	E_GrHwMdinOutRes800;			//default out mode

		/*
		if ( 0x82 == RegReadWord( 0x04 ) )
		{
			DbgMsgPrint( "Mdin200 : device found at %X \n", m_ChipAdr );
		}
		else
		{
			DbgMsgPrint( "Mdin200 : device not found at %X \n", m_ChipAdr );
		}
		*/
}
//--------------------------------------------------------------------
Cls_GrHwMdin200::~Cls_GrHwMdin200()
{

}
//--------------------------------------------------------------------
void	Cls_GrHwMdin200::RegWriteByte( __u16 A_RegAdr, __u8 A_RegVal )
{
		GrI2cS16Write( m_I2cCh, m_ChipAdr, A_RegAdr, &A_RegVal, 1 );
}
//--------------------------------------------------------------------
__u16	Cls_GrHwMdin200::RegReadWord( __u16 A_RegAdr )
{
	// local -------------------
		__u8	Tv_Buf[2];
	// code --------------------
		if ( GrI2cS16Read( m_I2cCh, m_ChipAdr, A_RegAdr, 2, m_I2cSlot ) )
		{
			GrI2cSlotRead( m_I2cCh, m_I2cSlot, Tv_Buf, TRUE );
		}
		return	( ( ((__u16)Tv_Buf[0]) << 8 ) | ((__u16)Tv_Buf[1]) );
}
//--------------------------------------------------------------------
void	Cls_GrHwMdin200::RegWriteWord( __u16 A_RegAdr, __u16 A_RegVal )
{
	// local -------------------
		__u8	Tv_Buf[2];
	// code --------------------
		Tv_Buf[0]	=	(__u8)( 0xFF & (A_RegVal >> 8) );
		Tv_Buf[1]	=	(__u8)( 0xFF & A_RegVal );
		GrI2cS16Write( m_I2cCh, m_ChipAdr, A_RegAdr, Tv_Buf, 2 );
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcMainFormatCvt( void )
{
	// local -------------------
		unsigned __s16 wd1Data[5], wdCeofTableSel;
		unsigned __s8 i;
	// code --------------------

		/* MDIN200 Main Format Converter Control Register (REV.200) by ISYU  */
		RegWriteWord( E_Mdin200Ra_MFC_CONTROL,   0x0002 ); /* Interpolation filter disable and use FIR filter in MFC  */

		RegWriteWord( E_Mdin200Ra_DRC_ITP_C_FLT0,   0x0800 ); /* Coefficient 0 of chroma interpolation filer of double rate converter */
		RegWriteWord( E_Mdin200Ra_DRC_ITP_C_FLT1,   0x04C0 ); /* Coefficient 1 of chroma interpolation filer of double rate converter */
		RegWriteWord( E_Mdin200Ra_DRC_ITP_C_FLT2,   0x0000 ); /* Coefficient 2 of chroma interpolation filer of double rate converter */
		RegWriteWord( E_Mdin200Ra_DRC_ITP_C_FLT3,   0x0740 ); /* Coefficient 3 of chroma interpolation filer of double rate converter */
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT0,   0x0800 ); /* Coefficient 0 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT1,   0x0400 ); /* Coefficient 1 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT2,   0x0000 ); /* Coefficient 2 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT3,   0x0000 ); /* Coefficient 3 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT4,   0x0000 ); /* Coefficient 4 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT5,   0x0000 ); /* Coefficient 5 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT6,   0x0000 ); /* Coefficient 6 of luma interpolation filer of double rate converter */	
		RegWriteWord( E_Mdin200Ra_DRC_ITP_Y_FLT7,   0x0000 ); /* Coefficient 7 of luma interpolation filer of double rate converter */	

		/* MDIN200 Main Format Converter Coefficient Writing (REV.200) by ISYU  */
		wd1Data[2] = 0x0000;
		wd1Data[3] = 0x0000;

		/* Coefficeint Address [ 0x000 ~ 0x03F ]  :  horizontal interpolation filter for luminance conversion */
		/* Coefficeint Address [ 0x100 ~ 0x13F ]  :  horizontal interpolation filter for chrominance conversion */
		/* Coefficeint Address [ 0x200 ~ 0x23F ]  :  vertical interpolation filter for luminance conversion */
		for( wdCeofTableSel = 0x000; wdCeofTableSel <= 0x200; wdCeofTableSel += 0x100 )
		{
			for( i = 0; i < 64; i++ )
			{
				wd1Data[0] = V_GrHwMdin200Cwd2MFC_Ctbl[i][0];
				wd1Data[1] = V_GrHwMdin200Cwd2MFC_Ctbl[i][1];
				wd1Data[4] = wdCeofTableSel | 0x0080 | (__u16)i;

				/* Modified by 18-Mar-2008 */
				RegWriteWord( E_Mdin200Ra_MFC_ITP_COEF_DATA0, wd1Data[0] );
				RegWriteWord( E_Mdin200Ra_MFC_ITP_COEF_DATA1, wd1Data[1] );
				RegWriteWord( E_Mdin200Ra_MFC_ITP_COEF_DATA2, wd1Data[2] );
				RegWriteWord( E_Mdin200Ra_MFC_ITP_COEF_DATA3, wd1Data[3] );
				RegWriteWord( E_Mdin200Ra_MFC_ITP_COEF_CMD, wd1Data[4] );
			}
		}

}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcVpllMpllCtrl( void )
{
		RegWriteWord( E_Mdin200Ra_PLL_DIS, 0x0003);		/* PLL disable for stabilizing PLL ctrl */

		RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0201);	/* Screen output OFF */
		RegWriteWord(E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001); /* local reg. update */

		/* MDIN200 Memory Clock Setting for  108MHz (REV.200) by ISYU */
		RegWriteWord( E_Mdin200Ra_PRE_DIV_MCLK,    0x0002); // Pre-divider value (P) of the internal PLL for MCLK(SDRAM Memory Clock)  
		RegWriteWord( E_Mdin200Ra_POST_DIV_MCLK,   0x0008); // Post-divider value (M) of the internal PLL for MCLK(SDRAM Memory Clock) 

		/* MDIN200 VCLK PLL  Input Clock Setting */
		RegWriteWord( E_Mdin200Ra_PLL_SEL_VCLK, 0x0001);	/* Selecting XTAL_IN for PLL Input Clock */
		// RegWriteWord( E_Mdin200Ra_PLL_SEL_VCLK, 0x0000);	/* Selecting CLK_A or CLK_B for PLL Input Clock */

		/* MDIN200 VCLK PLL Function Setting (REV.200) ISYU */
		RegWriteWord( E_Mdin200Ra_VCLK_PLL_CPO_EN, 0x0001 ); /* PLL CPO Pin is normal operation */
		RegWriteWord( E_Mdin200Ra_PLL_SEL_VCLK_HACT, 0x0000 ); /* Don't used internal HACTIVE signal for PLL input clock source */
		RegWriteWord( E_Mdin200Ra_VCLK_VCO_SEL, 0x0005 ); /* VCO selection control is setted by VCO5 (105 ~ 160MHz) Operates. */

		/* MDIN200 Input Video Clock Divider Setting */
		RegWriteWord( E_Mdin200Ra_CLK_A_DIV, 0x0000 ); /* VCLK_IN is clk_a = 27MHz */

		/* MDIN200 Internal VCLK Delay Control (REV.200) by ISYU */
		RegWriteWord( E_Mdin200Ra_VCLK_DLY_CTRL, 0x0000 ); /* Internal VCLK Delay 0ns, Non-inverted */
		/* MDIN200 VCLK Double Rate Processing Control (REV.200) by ISYU */
		RegWriteWord( E_Mdin200Ra_DIGITAL_2X_EN, 0x0000 ); /* VCLK Double rate processing control disable, It's only usded for 1080P Video Output*/	

		/* MDIN200 DAC Block -  Clock Output Delay Control (REV.200) by ISYU */
		//RegWriteWord( E_Mdin200Ra_DAC_DLY_CTRL, 0x0004);   /* Internal DAC Clock Output : 0ns delayed and inverted */
		RegWriteWord( E_Mdin200Ra_VCLK_OUT_DLY_CTRL, 0x0000);   /* Internal DAC Clock Output : 0ns delayed (default value) */

		/* VCLK Output PLL Setting [Video Clock Output]  (REV.200 ) by ISYU */
		/* The Formula of VCLK PLL setting is  : 
		* VCLK_OUT = ( VCLK_IN  * M )  / ( P * S ) , 
		* where  ( VCLK_IN / P ) > 15kHz,  20MHz < VCLK_OUT < 160MHz, VCLK_IN are Clk_A or Clk_B or XTAL_IN  */

		/* When VGA output (REV.200) by ISYU */
		if ( m_OutMode == E_GrHwMdinOutRes640) 
		{
			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x001A); /* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,  0x0061); /* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0004); /* Post-scaler value (S) of the internal PLL for video clock */
		}
		/* When SVGA output (REV.200) by ISYU */
		else if (m_OutMode == E_GrHwMdinOutRes800) 
		{
			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x001E);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x00B1);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0004);	/* Post-scaler value (S) of the internal PLL for video clock */
		}
		/* When XGA output (REV.200) by ISYU */
		else if (m_OutMode == E_GrHwMdinOutRes1024) {
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x0026);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x00B7);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0002);	/* Post-scaler value (S) of the internal PLL for video clock */
		}
		/* When SXGA output (REV.200) by ISYU */
		else if (m_OutMode == E_GrHwMdinOutRes1280) {
			RegWriteWord( E_Mdin200Ra_VCLK_OUT_DLY_CTRL, 0x0000);   /* VCLK_OUT : 0ns delayed (default value) */
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );

			/* MDIN200 Memory Clk Setting by 135MHz */
			RegWriteWord( E_Mdin200Ra_PRE_DIV_MCLK,    0x0002); /* Pre-divider value (P) of the internal PLL for MCLK(SDRAM Memory Clock)  */ 
			RegWriteWord( E_Mdin200Ra_POST_DIV_MCLK,   0x000A); /* Post-divider value (M) of the internal PLL for MCLK(SDRAM Memory Clock) */

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x0026);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x0098);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0001);	/* Post-scaler value (S) of the internal PLL for video clock */
		}
		else if (m_OutMode == E_GrHwMdinOutRes1360) {
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );
			RegWriteWord( E_Mdin200Ra_VCLK_VCO_SEL, 0x0004 ); /* VCO selection control is setted by VCO4 (65 ~ 110MHz) Operates. */

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x0019);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x004F);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0001);	/* Post-scaler value (S) of the internal PLL for video clock */
		}
		else if (m_OutMode == E_GrHwMdinOutRes1440) {
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x0027);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x009A);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0001);	/* Post-scaler value (S) of the internal PLL for video clock */

			/* MDIN200 Memory Clk Setting by 135MHz */
			RegWriteWord( E_Mdin200Ra_PRE_DIV_MCLK,    0x0002); /* Pre-divider value (P) of the internal PLL for MCLK(SDRAM Memory Clock)  */ 
			RegWriteWord( E_Mdin200Ra_POST_DIV_MCLK,   0x000A); /* Post-divider value (M) of the internal PLL for MCLK(SDRAM Memory Clock) */
		}
		else if (m_OutMode == E_GrHwMdinOutRes1680) {
			RegWriteWord( E_Mdin200Ra_DIGITAL_2X_EN, 0x0001 );
			RegWriteWord( E_Mdin200Ra_MFC_CONTROL,   0x0001 );
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );

			/* MDIN200 Memory Clk Setting by 135MHz */
			RegWriteWord( E_Mdin200Ra_PRE_DIV_MCLK,    0x0002); /* Pre-divider value (P) of the internal PLL for MCLK(SDRAM Memory Clock)  */ 
			RegWriteWord( E_Mdin200Ra_POST_DIV_MCLK,   0x000A); /* Post-divider value (M) of the internal PLL for MCLK(SDRAM Memory Clock) */

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x0016);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x0061);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0001);	/* Post-scaler value (S) of the internal PLL for video clock */// modified on Jul21 2008
		}
		else if (m_OutMode == E_GrHwMdinOutRes1920) {
			RegWriteWord( E_Mdin200Ra_DIGITAL_2X_EN, 0x0001 );
			RegWriteWord( E_Mdin200Ra_MFC_CONTROL,   0x0001 );
			RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000 );

			/* MDIN200 Memory Clk Setting by 135MHz */
			RegWriteWord( E_Mdin200Ra_PRE_DIV_MCLK,    0x0002); /* Pre-divider value (P) of the internal PLL for MCLK(SDRAM Memory Clock)  */ 
			RegWriteWord( E_Mdin200Ra_POST_DIV_MCLK,   0x000A); /* Post-divider value (M) of the internal PLL for MCLK(SDRAM Memory Clock) */

			RegWriteWord( E_Mdin200Ra_PRE_DIV_VCLK,   0x001C);	/* Pre-divider value (P) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_DIV_VCLK,    0x009A);	/* Post-divider value (M) of the internal PLL for video clock */
			RegWriteWord( E_Mdin200Ra_POST_SCALE_VCLK, 0x0001);	/* Post-scaler value (S) of the internal PLL for video clock */
		}
		RegWriteWord(E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	/* local reg. update */

		GrPrcSleep( 30 );
		RegWriteWord( E_Mdin200Ra_PLL_DIS, 0x0000);		/* PLL enable for stabilizing PLL ctrl */

}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcInputCtrl( void )
{
		/* When NTSC video input from Port A (REV.200) by ISYU*/
		if ( !m_IsPal ) 
		{
			RegWriteWord( E_Mdin200Ra_IN_SIZE_H_A, 0x02D0);	/* nori: 720 pixels */		
			RegWriteWord( E_Mdin200Ra_IN_SIZE_V_A, 0x01E0);	/* vert: 480 lines */
			RegWriteWord( E_Mdin200Ra_IN_FID_CTRL, 0x35A8);	/* External field-id use, No inverting */	
			/* Input Video Sync Control Reg. (REV.200) by ISYU */
			RegWriteWord( E_Mdin200Ra_IN_SYNC_CTRL, 0x2010); 	/* Input A: ITU-R RT 656 format, H/V SYNC Low Polarity, Interlaced Scan */
			RegWriteWord( E_Mdin200Ra_IN_ACT_OFFSET_A, 0x2606);	/* Input A: Vert Offset 0, Hori Offset 0 */ 		
		}
		/* When PAL video input from Port A (REV.200) by ISYU */
		else  
		{ 
			RegWriteWord( E_Mdin200Ra_IN_SIZE_H_A, 0x02D0);	/* hori: 720 pixels */
			RegWriteWord( E_Mdin200Ra_IN_SIZE_V_A, 0x0240);	/* vert: 576 lines */
			RegWriteWord( E_Mdin200Ra_IN_FID_CTRL, 0x3608);	/* External field-id use, No inverting */
			/* Input Video Sync Control Reg. (REV.200) by ISYU */
			RegWriteWord( E_Mdin200Ra_IN_SYNC_CTRL, 0x2010); 	/* Input A: ITU-R RT 656 format, H/V SYNC Low Polarity, Interlaced Scan */
			RegWriteWord( E_Mdin200Ra_IN_ACT_OFFSET_A, 0x2606);	/* Input A: Vert Offset 0, Hori Offset 0 */
		}

		RegWriteWord(E_Mdin200Ra_IN_FORMAT_CTRL,   0x0000);		/* no YC offset, no YC swap */
		RegWriteWord(E_Mdin200Ra_CLK_A_DLY_SEL,   0x0004);		/* input clock inverse 0 = not , 4 = inverse */	
		RegWriteWord(E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);		/* local reg. update */
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcSyncSize( void )
{
		/* MDIN200 Internal Timing Reference Generator Control Registers (REV.200) by ISYU  */
		RegWriteWord( E_Mdin200Ra_HDPULSE_H,   0x000A );	/* The position of internal horizontal timing reference pulse */
		RegWriteWord( E_Mdin200Ra_VDPULSE_V,   0x000A );	/* The position of internal vertical timing reference pulse */
		RegWriteWord( E_Mdin200Ra_VDPULSE_POS_BOT,   0x000A ); /* The position of internal vertical timing reference pulse for the bottom field on interlaced output mode. */		

		/* When VGA output */
		if (m_OutMode == E_GrHwMdinOutRes640) 
		{
			/* When NTSC input */
			if (!m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x0280);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x0280);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0280);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x01E0);
			}
			/* When PAL Input */
			else 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x0280);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x0280);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0280);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x01E0);
			}	

			/* 640x480P60(VGA)Standard Video Timing base on VESA for HSYNC (REV.200) by ISYU */ 
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0320);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0061);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0091);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0311);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);
			/* 640x480P60(VGA) Standard Video Timing base on VESA for VSYNC (REV.200) by ISYU */ 					
			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x020D);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0003);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x0023);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0203);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}
		/* When SVGA output */
		else if (m_OutMode == E_GrHwMdinOutRes800) 
		{
			/* When NTSC input */
			if (!m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0320);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0258);			
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0320);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0258);
			}		

			/* 800x600P(SVGA) Standard Video Timing base on VESA for HSYNC (REV.200) by ISYU */ 		
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0420);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0081);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x00D8);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x03F8);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);
			/* 800x600P(SVGA) Standard Video Timing base on VESA for VSYNC (REV.200) by ISYU */ 	
			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x0274);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0005);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x001B);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0273);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}
		/* When XGA(1024x768P60) output */
		else if (m_OutMode == E_GrHwMdinOutRes1024) 
		{
			/* When NTSC input */
			if ( !m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0400);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0300);			
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0400);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0300);
			}		

			/* 1024x768P60(XGA) Standard Video Timing base on VESA for HSYNC (REV.200) by ISYU */ 	
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0540);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0089); 
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0128); 
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0528); 
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001); 
			/* 1024x768P60(XGA) Standard Video Timing base on VESA for VSYNC (REV.200) by ISYU */ 	
			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x0326);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0007); 
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x0023);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0323);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}
		/* When SXGA(1280x1024P) output */
		else if (m_OutMode == E_GrHwMdinOutRes1280) 
		{
			/* When NTSC input */
			if ( !m_IsPal ) {
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0500);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0400);			
			}
			/* When PAL Input */
			else 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0500);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0400);
			}

			/* 1024x768P60(XGA) Standard Video Timing base on VESA for HSYNC (REV.200) by ISYU */ 	
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0698); 
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0071);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0168);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0668);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);

			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x042A);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0004);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x0029);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0429);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);	
		}
		else if (m_OutMode == E_GrHwMdinOutRes1360) 
		{
			/* When NTSC input */
			if ( !m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0550);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0300);			
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0);
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0550);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0300);
			}		

			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0700);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0071);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0171);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x06C1);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);

			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x031B);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0007);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x0019);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0319);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);	
		}
		else if (m_OutMode == E_GrHwMdinOutRes1440) 
		{
			/* When NTSC input */
			if ( !m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x05A0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0384);
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x05A0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0384);
			}
			
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0770);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0099);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0181);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0721);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);

			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x03A6);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0007);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x0020);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x03A4);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}
		else if (m_OutMode == E_GrHwMdinOutRes1680) 
		{
			/* When NTSC input */
			if ( !m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0348); // modified on Jul21 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x041A);
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x0348); // modified on Jul21 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x041A);
			}
			
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x0398);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0011);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0039);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0381);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);

			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x0438);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0007);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x001C);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0436);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}
		else if (m_OutMode == E_GrHwMdinOutRes1920)
		{
			/* When NTSC input */
			if ( !m_IsPal ) 
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x01E0);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x01E0);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x03C0); // modified on Jul21 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0438);
			}
			/* When PAL Input */
			else  
			{
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H2,   0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V2,   0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H2,  0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V2,  0x0240);

				RegWriteWord( E_Mdin200Ra_SRC_SIZE_H,    0x02C0); // modified on Oct06 2008
				RegWriteWord( E_Mdin200Ra_SRC_SIZE_V,    0x0240);
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_H,   0x03C0); // modified on Jul21 2008
				RegWriteWord( E_Mdin200Ra_DEST_SIZE_V,   0x0438);
			}
			
			RegWriteWord( E_Mdin200Ra_HTOTAL_SIZE,   0x044C);
			RegWriteWord( E_Mdin200Ra_HSYNC_START,   0x0016);
			RegWriteWord( E_Mdin200Ra_HACTIVE_START, 0x0060);
			RegWriteWord( E_Mdin200Ra_HACTIVE_END,   0x0420);
			RegWriteWord( E_Mdin200Ra_HSYNC_END,     0x0001);

			RegWriteWord( E_Mdin200Ra_VTOTAL_SIZE,   0x0465);
			RegWriteWord( E_Mdin200Ra_VSYNC_START,   0x0006);
			RegWriteWord( E_Mdin200Ra_VACTIVE_START, 0x002A);
			RegWriteWord( E_Mdin200Ra_VACTIVE_END,   0x0462);
			RegWriteWord( E_Mdin200Ra_VSYNC_END,     0x0001);
		}

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	/* local reg. update */

}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcDeintRegSet( void )
{
		RegWriteWord( 0x500, 0x4D18); // 20080509
		//RegWriteWord( 0x500, 0xCD18); // damgi
		RegWriteWord( 0x501, 0x3B16); // 20080509
		RegWriteWord( 0x502, 0x1C0F);
		RegWriteWord( 0x503, 0x8040);
		RegWriteWord( 0x504, 0x02FF);
		RegWriteWord( 0x505, 0x2000);
		RegWriteWord( 0x506, 0x4F20);
		RegWriteWord( 0x507, 0x0A38);
		RegWriteWord( 0x508, 0x0048);
		RegWriteWord( 0x509, 0x10CA);
		RegWriteWord( 0x50A, 0x80D0);
		RegWriteWord( 0x50B, 0x01E8);
		RegWriteWord( 0x50C, 0x1330);
		RegWriteWord( 0x50D, 0xC620);
		RegWriteWord( 0x50E, 0x0824);
		RegWriteWord( 0x50F, 0x0808);

		RegWriteWord( 0x512, 0x0050);
		RegWriteWord( 0x513, 0x0007); 
		RegWriteWord( 0x514, 0x0855);
		RegWriteWord( 0x516, 0x04B3);
		RegWriteWord( 0x517, 0x1105);
		RegWriteWord( 0x518, 0x00A0);
		RegWriteWord( 0x51A, 0x0000);
		RegWriteWord( 0x51B, 0x4444);
		RegWriteWord( 0x51C, 0x4000);
		RegWriteWord( 0x51D, 0x4410);
		RegWriteWord( 0x51F, 0x3C14);

		//	RegWriteWord( 0x520, 0x00FF);
		RegWriteWord( 0x521, 0x3400);
		RegWriteWord( 0x522, 0xFFFF);
		RegWriteWord( 0x523, 0x0038);
		//	RegWriteWord( 0x524, 0x0F88);
		RegWriteWord( 0x525, 0x14FF);
		RegWriteWord( 0x526, 0xDB68);
		RegWriteWord( 0x527, 0xF51C);
		RegWriteWord( 0x528, 0x0A83);
		//	RegWriteWord( 0x529, 0xCCA8);
		//	RegWriteWord( 0x52A, 0x10DC);
		RegWriteWord( 0x52B, 0x0000);

		RegWriteWord( 0x530, 0x0000);
		RegWriteWord( 0x531, 0x7001);
		RegWriteWord( 0x532, 0x8A06);
		RegWriteWord( 0x535, 0xFFDC);
		RegWriteWord( 0x536, 0x6028);
		RegWriteWord( 0x537, 0x080A);
		RegWriteWord( 0x538, 0x0AFF);
		RegWriteWord( 0x539, 0x1212);
		RegWriteWord( 0x53A, 0x0384);
		RegWriteWord( 0x53B, 0x8874);
		RegWriteWord( 0x53C, 0x0A0A);
		//	RegWriteWord( 0x53D, 0x2F3D);
		RegWriteWord( 0x53E, 0x8A19);
		RegWriteWord( 0x53F, 0x0600);

		RegWriteWord( 0x52A, 0x10DC);
		RegWriteWord( 0x520, 0x00FF);
		RegWriteWord( 0x524, 0x0F88);
		RegWriteWord( 0x529, 0xCCA8);
		RegWriteWord( 0x53D, 0x2F3D);

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	/* local reg. update */

}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcPeakFltSDEn( void )
{
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT0, 0x200);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT1, 0x0);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT2, 0x700);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT3, 0x0);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT4, 0x0);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT5, 0x0);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT6, 0x0);
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT7, 0x0);

		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT_CTRL, 0x1000); 	/* Sharpness 0 */
		RegWriteWord( E_Mdin200Ra_H_PEAKING_FLT_ENABLE, 0x0001);

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);		/* local reg. update */
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcSurfaceFltEn( void )
{
		RegWriteWord( E_Mdin200Ra_SURFACE_FLT0,    0x0100);
		RegWriteWord( E_Mdin200Ra_SURFACE_FLT1,    0x0080);
		RegWriteWord( E_Mdin200Ra_SURFACE_FLT2,    0x0080);
		RegWriteWord( E_Mdin200Ra_SURFACE_FLT3,    0x0080);
		RegWriteWord( E_Mdin200Ra_SURFACE_FLT_ENABLE,    0x0001);
		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcSyncReset( void )
{
		RegWriteWord( E_Mdin200Ra_OUTPUT_SYNC_RESET, 0x0064);
		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);
		RegWriteWord( E_Mdin200Ra_OUTPUT_SYNC_RESET, 0x0064);
		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);
		GrPrcSleep( 30 );
		RegWriteWord( E_Mdin200Ra_OUTPUT_SYNC_RESET, 0x1064);
		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcCromaFltEn( void )
{
		RegWriteWord( E_Mdin200Ra_COLOR_ENH_FLT_ENABLE, 0x0001);
		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);		/* local reg. update */
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcRgbSdOut( void )
{
		/* Output CSC setting for RGB SD output */
		RegWriteWord( E_Mdin200Ra_CSC_COEF0, 0x0254);	
		RegWriteWord( E_Mdin200Ra_CSC_COEF1, 0x0F38);	
		RegWriteWord( E_Mdin200Ra_CSC_COEF2, 0x0E60);
		RegWriteWord( E_Mdin200Ra_CSC_COEF3, 0x0254);
		RegWriteWord( E_Mdin200Ra_CSC_COEF4, 0x0409);
		RegWriteWord( E_Mdin200Ra_CSC_COEF5, 0x0000); 
		RegWriteWord( E_Mdin200Ra_CSC_COEF6, 0x0254);	
		RegWriteWord( E_Mdin200Ra_CSC_COEF7, 0x0000);	
		RegWriteWord( E_Mdin200Ra_CSC_COEF8, 0x0331);	

		RegWriteWord( E_Mdin200Ra_CSC_OFFSET_Y, 0x0FC0);

		RegWriteWord( E_Mdin200Ra_CSC_CTRL, 0x01C4);
		RegWriteWord( E_Mdin200Ra_BLACK_LEVEL_CTRL, 0x0000);

		RegWriteWord( E_Mdin200Ra_OUT_CTRL, 0x0000);	// Digital:RGB/YCbCr444(external sync), DAC:RGB

		LcCromaFltEn();

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	// local reg. update
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::LcOutputCtrl( void )
{
		// Added on Dec23 2008
		switch (m_OutMode)
		{
			case E_GrHwMdinOutRes640:
			case E_GrHwMdinOutRes1024:
				RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0200);	/* DE Out Enable, Sync Polarity[H/V] : N/N */
				break;
			case E_GrHwMdinOutRes800:
				RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0300);	/* DE Out Disable, Sync Polarity[H/V] : N/N */
				break;
			case E_GrHwMdinOutRes1280:
			case E_GrHwMdinOutRes1360:
			case E_GrHwMdinOutRes1920:
				RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0230);	/* DE Out Enable, Sync Polarity[H/V] : P/P */
				break;
			case E_GrHwMdinOutRes1440:
			case E_GrHwMdinOutRes1680:
				RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0210);	/* DE Out Enable, Sync Polarity[H/V] : N/P */
				break;
			default:
				RegWriteWord( E_Mdin200Ra_OUT_SYNC_CTRL, 0x0200);	/* DE Out Enable, Sync Polarity[H/V] : N/N */
				break;
		}

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	/* local reg. update */
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::SetSigMode(	BOOL8 A_IsPal, __u8 A_OutMode )
{
	// local -------------------
	// code --------------------
		//update mode
		m_IsPal		=	A_IsPal;
		m_OutMode	=	A_OutMode;

		RegWriteWord(E_Mdin200Ra_DISPLAY_MODE, 0x000A ); 	// disable output display
		RegWriteWord(E_Mdin200Ra_VCLK_DLY_CTRL, 0x0004 ); 	// Internal VCLK Delay (inverse)

		RegWriteWord(E_Mdin200Ra_PLL_DIS, 0x0003 ); 	// VPLL,MPLL disable, Apr14 2008

		RegWriteWord(E_Mdin200Ra_OUT_SYNC_CTRL, 0x0201 ); 	// Added on Dec23 2008
		RegWriteWord(E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001 ); // Added on Dec23 2008
		GrPrcSleep( 30 );

		RegWriteWord(E_Mdin200Ra_SOFT_RESET, 0x0000 );
		RegWriteWord(E_Mdin200Ra_SOFT_RESET, 0x0001 );

		/* Main Format Converter Control Setting (REV.200) by ISYU */
		LcMainFormatCvt();

		/* MDIN200 memory configuration (REV.200)*/
		RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0004 );	/* 8MB SDRAM, 32-bit Data Bus Mode, 10 bit Data */
		//RegWriteWord( E_Mdin200Ra_MEM_CONFIG, 0x0000);	/* 8MB SDRAM, 32-bit Data Bus Mode, 8 bit Data */

		/* MDIN200 DAC Control Register (REV.200) by ISYU */
		RegWriteWord( E_Mdin200Ra_DAC_CTRL, 0x0010 );	/* DAC Default Setting, RGB output Mode */

		// Vsync
		RegWriteWord( E_Mdin200Ra_SYNC_CTRL, 0x0009 ); // Apr14 2008

		/* MDIN200 Vclk PLL setting */
		LcVpllMpllCtrl();
		/* MDIN200 Input Setting */
		LcInputCtrl();

		/* MDIN200 Output Sync Setting */
		LcSyncSize();	
		/* MDIN200 Deinterlacing Parameters Setting */
		LcDeintRegSet();

		/* MDIN200 peaking filter enable */
		LcPeakFltSDEn();
		/* MDIN200 surface filter enable */
		LcSurfaceFltEn();
		/* MDIN200 CTI enable */
		//MDIN200CTIEn();

		// Black & White level Extension/Suppression
		//BwExtEnable_ext50();

		/* MDIN200 OutputSync Reset */
		LcSyncReset();

		LcRgbSdOut();	// RGB output

		LcOutputCtrl();

		RegWriteWord( E_Mdin200Ra_DISPLAY_MODE, 0x0028);	// Enable output display
		RegWriteWord( E_Mdin200Ra_DAC_PD_DOUT_DIS, 0x0000 );	// Enable DAC R,G,B output

		RegWriteWord( E_Mdin200Ra_SOFT_RESET, 0x0000 );
		RegWriteWord( E_Mdin200Ra_SOFT_RESET, 0x0001 );

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);	// local reg. update

		//out test partern
		//RegWriteWord( E_Mdin200Ra_OUT_TEST_PTRN, 0x0010);
		//RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);		// update local register

		//input test pattern
		/*
		// Pattern Type
		RegWriteWord( E_Mdin200Ra_IN_TEST_PTRN, 0x19 );		//ntsc
		// Input Port Configure for Test Pattern
		RegWriteWord( E_Mdin200Ra_IN_FID_CTRL, 0x35A8 );			// input FIELD-ID
		RegWriteWord( E_Mdin200Ra_IN_SYNC_CTRL, 0x0000   );		// 444 format video
		RegWriteWord( E_Mdin200Ra_IN_ACT_OFFSET_A, 0x0000);		// input offset

		RegWriteWord( E_Mdin200Ra_LOCAL_REG_UPDATE, 0x0001);		// update local register
		*/

		//debug
		/*
		{
		__u16		Tv_Radr;
		__u16		Tv_Rval;
		for ( Tv_Radr=0;Tv_Radr < 0x5A0;Tv_Radr ++ )
		{
		Tv_Rval	=	RegReadWord( Tv_Radr );
		DbgMsgPrint( "MdinReg %4X = %4X \n", Tv_Radr, Tv_Rval );
		}
		}
		*/

}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwMdin200::IsSigPal( void )
{
		return	m_IsPal;
}
//--------------------------------------------------------------------
void	Cls_GrHwMdin200::SetBright(__u8 A_Ch, __u8 A_BriVal)
{
	// var --------------------
	// code -------------------
}
//--------------------------------------------------------------------
void Cls_GrHwMdin200::SetContrast(__u8 A_Ch, __u8 A_ConVal)
{
	// var --------------------
	// code -------------------
}
//--------------------------------------------------------------------
void		Cls_GrHwMdin200::SetSat( __u8 A_Ch, __u8 A_Val )
{
	// var --------------------
	// code -------------------
}
//--------------------------------------------------------------------
__u8		Cls_GrHwMdin200::GetOutMode( void )
{
		return	m_OutMode;
}
//--------------------------------------------------------------------

