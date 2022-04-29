/*
 lg1701 manager

 */

//====================================================================
// uses
#include	<GrLnxApp.h>

#include	<GrTypeBase.h>

#include	<GrDebug.h>

#include	<GrDumyTool.h>
#include	<GrStrTool.h>

#include	<Hw/Lg1701/GrHwLg1701.h>
#include	<GrI2c.h>

//====================================================================
//const

//====================================================================
//local type


//====================================================================
//local function

//====================================================================
//var


//====================================================================
Cls_GrHwLg1701::Cls_GrHwLg1701( __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_Stage, __u8 A_ClkDly, BOOL8 A_IsEncSvideo )
{
		m_I2cCh				=	A_I2cCh;
		m_I2cRtSlot		=	A_I2cRtSlot;
		m_Stage				=	A_Stage;
		m_ChipAdr			=	(__u8)A_Adr;
		m_ClkDly			=	A_ClkDly;
		m_IsEncSvdo		=	A_IsEncSvideo;
		m_IsAdoOn			=	TRUE;

		/*
		if ( IcTest() )
		{
			DbgMsgPrint( "lg1701 found at %x.\n", A_Adr );
		}
		else
		{
			DbgMsgPrint( "lg1701 not found at %x.\n", A_Adr );
		}
		*/
}
//--------------------------------------------------------------------
Cls_GrHwLg1701::~Cls_GrHwLg1701()
{

}
//--------------------------------------------------------------------
void	Cls_GrHwLg1701::WriteReg( __u8 A_RegAdr, __u8 A_RegVal )
{
		GrI2cStdWrite( m_I2cCh, m_ChipAdr, A_RegAdr, &A_RegVal, 1 );
}
//--------------------------------------------------------------------
__u8	Cls_GrHwLg1701::ReadReg( __u8 A_RegAdr )
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
void		Cls_GrHwLg1701::SetSigMode(	BOOL8 A_IsPal )
{
	// local -------------------
		__u8		Tv_DecIdx;
	// code --------------------
		//update mode
		m_IsPal		=	A_IsPal;

		WriteReg( E_Lg1701RaBank, 0 );
		WriteReg( E_Lg1701RaDacPower,0x03); // video DAC active
		WriteReg( E_Lg1701RaClkEnable,0x10); // clock output enable - audio playback out disable
		WriteReg( 0x07,0x05);				// venc external clock , encoder clock inverse

		WriteReg( 0x08,0x01);				// aud_ck_sel=0(internal),atxc_slv_mode=0(master),atx_out_inv=1(inversion)
		WriteReg( 0x09,0x02);				// arxc_slv_mode=0(master),arx_out_inv=1(inversion), amux_sel=0(arx_clock)

		WriteReg( 0x13,0x00);				// cvbs_icon
		WriteReg( 0x15,0x00);				// cpclp on
		WriteReg( 0x18,0x03);				// aadc bias = 110

		WriteReg( 0x1e,0x08);				// clip mode = std, cif=disable, aux=00b

		WriteReg( 0x1c,0x99); // 54MHz ch3,4
		WriteReg( 0x1d,0x88); // 54MHz ch1,2
		WriteReg( 0x1f,0x36); // v4o_clk=27Mhz_d135, v3o_clk=27Mhz_d270
		WriteReg( 0x20,0x36); // v2o_clk=27Mhz_d135, v1o_clk=27Mhz_d270

		WriteReg( 0x21,0xe8); // venc enable
		//WriteReg( 0x2b,0x08); // avo1=composite video, avo2=audio left channel output

		// audio settings
		WriteReg( 0x0a,0x0d); // audio dto 8KHz
		WriteReg( 0x0b,0x2e); // audio dto 8KHz
		WriteReg( 0x0c,0x04); // audio dto 8KHz
		WriteReg( 0x0d,0xda); // audio dto 8KHz
		WriteReg( 0x0e,0xc1); // audio dto 8KHz
		WriteReg( 0x0f,0xb9); // audio dto 8KHz
		WriteReg( 0x10,0x28); // audio dto 8KHz

		/*
		WriteReg( 0x0a,0x0d); // audio dto 16KHz
		WriteReg( 0x0b,0x2e); // audio dto 16KHz
		WriteReg( 0x0c,0x09); // audio dto 16KHz
		WriteReg( 0x0d,0xb5); // audio dto 16KHz
		WriteReg( 0x0e,0x83); // audio dto 16KHz
		WriteReg( 0x0f,0x73); // audio dto 16KHz
		WriteReg( 0x10,0x0d); // audio dto 16KHz
		*/

		if ( 0 != m_Stage )
		{
			WriteReg( 0x05, 0x30); // atx,arx,v4o,v3o,v2o,v1o clock tristate enable
		}
		else
		{
			WriteReg( 0x05,0x10); // clock output enable
		}

		WriteReg(0x50,0x01); // atx_codec=pcm_16bit
		switch( m_Stage )
		{
			case 0:
				WriteReg(0x51, 0x32 ); // atx_ch_pos_c4,atx_ch_pos_c3
				WriteReg(0x52, 0x10 ); // atx_ch_pos_c2,atx_ch_pos_c1
				break;
			case 1:
				WriteReg(0x51, 0x76 ); // atx_ch_pos_c4,atx_ch_pos_c3
				WriteReg(0x52, 0x54 ); // atx_ch_pos_c2,atx_ch_pos_c1
				break;
			case 2:
				WriteReg(0x51, 0xba ); // atx_ch_pos_c4,atx_ch_pos_c3
				WriteReg(0x52, 0x98 ); // atx_ch_pos_c2,atx_ch_pos_c1
				break;
			case 3:
				WriteReg(0x51, 0xfe ); // atx_ch_pos_c4,atx_ch_pos_c3
				WriteReg(0x52, 0xdc ); // atx_ch_pos_c2,atx_ch_pos_c1
				break;
		}
		
		WriteReg(0x53, 0x00 ); // end_of_tx_cnt 
		WriteReg(0x54, 0xff ); // end_of_tx_cnt = 256-1 
		WriteReg(0x55, 0x9e ); // tx_en=1,tx_fmt=0(I2S),g726_coeff=0,tx_16bn8b=1(16bit),tx_msb_first=1,tx_sync_ms=1(master),tx_sync_dly=1,res

		WriteReg( 0x56,0x01); // arx_codec=pcm_16bit
		WriteReg( 0x58,0x00); // end_of_rx_cnt
		WriteReg( 0x59,0xff); // end_of_rx_cnt = 256-1
		WriteReg( 0x5a,0x9e); // rx_en=1,rx_fmt=0(I2S),g726_coeff=0,rx_16bn8b=1(16bit),rx_msb_first=1,rx_sync_ms=1(master),rx_sync_dly=1,res

		WriteReg(0x5b,0x00); // mixer_out_sel_l=channel 0
		WriteReg(0x5c,0x00); // mixer_out_sel_r=channel 0
		WriteReg(0x57,0x00); // rx_ch_pos_pb_l,r=0  - play channel position
		WriteReg(0x5d,0x00); // outmix_comp = 0 
		WriteReg(0x5e,0x80); // recmixer_mode=0(normal),recmix_comp,mute_det_en=0 

		if ( 0 == m_Stage )
		{
			WriteReg(0x62,0x01); // pwm_sel=0(single end),pwm_pdb=1(normal)
		}
		else
		{
			WriteReg(0x62,0x00);	// pwm_sel=0(single end),pwm_pdb=1(normal)
		}
		WriteReg(0x63,0x39);		// pwm_vol=0x39(0dB)
		

		//WriteReg(0x5f,0xf0); // enable mute detection
		//WriteReg(0x60,0x04); // set mute detection threshold

		// video decoder settings
		for(Tv_DecIdx=0; Tv_DecIdx < 4; Tv_DecIdx++)
		{
			WriteReg( 0xff,Tv_DecIdx+1);
			if ( m_IsPal )
			{
				WriteReg( 0x00,0x32 ); // standard
				WriteReg( 0x01,0x00 ); // standard 
				WriteReg( 0x04,0xdc ); // hagc
				WriteReg( 0x0f,0x67 ); // cagc
				WriteReg( 0x18,0x2a ); // cdto_inc
				WriteReg( 0x19,0x09 ); // 
				WriteReg( 0x1a,0x8a ); // 
				WriteReg( 0x1b,0xcb ); // 
				WriteReg( 0x2e,0x80 ); // hactive start
				WriteReg( 0x33,0x2b ); // vactive_start
				WriteReg( 0x34,0xc4 ); // vactive_height
				WriteReg( 0x6c,0x05 ); // 
			}
			else
			{
				WriteReg( 0x00,0x00); // standard
				WriteReg( 0x01,0x01); // standard 
				WriteReg( 0x04,0xdd); // hagc
				WriteReg( 0x0f,0xa8); // cagc
				WriteReg( 0x18,0x21); // cdto_inc
				WriteReg( 0x19,0xf0); // 
				WriteReg( 0x1a,0x7c); // 
				WriteReg( 0x1b,0x1f); // 
				WriteReg( 0x2e,0x78); // hactive start
				WriteReg( 0x33,0x23); // vactive_start
				WriteReg( 0x34,0x61); // vactive_height
				WriteReg( 0x6c,0x25); // 
			}
		}
			
		WriteReg(0xff,0x00);
		//        WriteReg(dev_no,0x06,0xff);
		WriteReg(0x06,0x00);

		// reset pll
		WriteReg(0x01,0x00);
		WriteReg(0x01,0x01);

		// video encoder settings
		//    WriteReg(0,0x3b,0x02); // auto set disable

		if ( m_IsPal )
		{
			WriteReg(0x21,0xe9 );
			WriteReg(0x23,0x2a ); // burst amp
			WriteReg(0x24,0x03 ); // luminance delay=nodelay, y filter=bypass
			WriteReg(0x26,0x2a ); // phase_inc
			WriteReg(0x27,0x09 );
			WriteReg(0x28,0x8a );
			WriteReg(0x29,0xcb );
		}
		else
		{
			WriteReg(0x21,0xe8);
			WriteReg(0x23,0x38); // burst amp
			WriteReg(0x24,0x03); // luminance delay=nodelay, y filter=bypass
			WriteReg(0x26,0x21); // phase_inc
			WriteReg(0x27,0xf0);
			WriteReg(0x28,0x7c);
			WriteReg(0x29,0x1f);
		}

		//svideo out set
		if ( m_IsEncSvdo )
		{
			WriteReg(0x2b,0x05);
		}
		
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwLg1701::IsSigPal( void )
{
		return	m_IsPal;
}
//--------------------------------------------------------------------
BOOL8		Cls_GrHwLg1701::IcTest( void )
{
	// local -------------------
		BOOL8	Tv_Result;
		__u8	Tv_Rval;
	// code --------------------
		Tv_Result	=	FALSE;

		WriteReg( E_Lg1701RaBank, 0 );
		Tv_Rval		=	ReadReg( E_Lg1701RaDevId );
		if ( E_Lg1701RvDevId == Tv_Rval )
		{
			Tv_Result	=	TRUE;
		}

		return	Tv_Result;
}
//--------------------------------------------------------------------
void	Cls_GrHwLg1701::SetBright(__u8 A_Ch, __u8 A_BriVal)
{
	// var --------------------
	// code -------------------
		WriteReg( E_Lg1701RaBank, (A_Ch + 1) );
		WriteReg(E_Lg1701RaBright, A_BriVal);
}
//--------------------------------------------------------------------
void Cls_GrHwLg1701::SetContrast(__u8 A_Ch, __u8 A_ConVal)
{
	// var --------------------
	// code -------------------
		WriteReg( E_Lg1701RaBank, (A_Ch + 1) );
		WriteReg(E_Lg1701RaContrast, A_ConVal);
}
//--------------------------------------------------------------------
__u8	Cls_GrHwLg1701::SigStatGet( void )
{
	// local -------------------
	// code --------------------
		WriteReg( E_Lg1701RaBank, 0 );
		return	ReadReg( E_Lg1701RaGpioStat );
}
//--------------------------------------------------------------------
void		Cls_GrHwLg1701::AdoSetOutMux( __u32 A_Ch )
{
	// local -------------------
		__u8	Tv_Rval;
	// code --------------------
		WriteReg( E_Lg1701RaBank, 0 );
		if ( 5 > A_Ch )
		{
			WriteReg( 0x5B, (__u8)A_Ch );
			WriteReg( 0x5C, (__u8)A_Ch );
		}
		else
		{
			//play 
			WriteReg( 0x5B, 5 );
			WriteReg( 0x5C, 5 );
			Tv_Rval	=	((__u8)A_Ch - 5) & 0x0F;
			Tv_Rval	=	Tv_Rval | (Tv_Rval << 4);
			WriteReg( 0x57, Tv_Rval );
		}
}
//--------------------------------------------------------------------
void		Cls_GrHwLg1701::AdoSetOutVol( __u32 A_Vol )
{
		WriteReg( E_Lg1701RaBank, 0 );
		WriteReg( 0x63, (__u8)A_Vol );
}
//--------------------------------------------------------------------
void		Cls_GrHwLg1701::SetSat( __u8 A_Ch, __u8 A_Val )
{
	// var --------------------
	// code -------------------
		WriteReg( E_Lg1701RaBank, (A_Ch + 1) );
		WriteReg(E_Lg1701RaSat, A_Val);
}
//--------------------------------------------------------------------
void	Cls_GrHwLg1701::SetClkDly(__u8 A_Delay)
{
		// local -------------------
		// code --------------------
			//init
			m_ClkDly		= A_Delay;

}
//--------------------------------------------------------------------
void	Cls_GrHwLg1701::AdoSetOnOff( BOOL8 A_IsOn )
{
		// local -------------------
			__u8	Tv_Rval;
		// code --------------------
			if ( A_IsOn != m_IsAdoOn )
			{
				m_IsAdoOn		=	A_IsOn;
				Tv_Rval		=	0;
				if ( m_IsAdoOn )
				{
					Tv_Rval	=	1;
				}
				WriteReg( E_Lg1701RaBank, 0 );
				WriteReg(0x62, Tv_Rval);
			}
}
//--------------------------------------------------------------------

