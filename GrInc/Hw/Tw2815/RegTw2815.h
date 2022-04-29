/*
	tw 2815

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_RegTw2815
#define	_EPC_RegTw2815

//====================================================================
// uses
	
//====================================================================
//constance

#define E_Tw2815DfltDevAdr		0x50			/*	default device address	*/

#define E_Tw2815DevId					0x20			/*	device id		*/

//register
#define	E_Tw2815Ra_CH0_VIDSTAT				0x00
#define	E_Tw2815Ra_CH0_FORMAT					0x01	 	
#define	E_Tw2815Ra_CH0_HDELAY 	      0x02
#define	E_Tw2815Ra_CH0_HACTIVE        0x03
#define	E_Tw2815Ra_CH0_VDELAY 	      0x04
#define	E_Tw2815Ra_CH0_VACTIVE        0x05
#define	E_Tw2815Ra_CH0_MSB_ACTV       0x06
#define	E_Tw2815Ra_CH0_HUE 		        0x07
#define	E_Tw2815Ra_CH0_SAT 		        0x08
#define	E_Tw2815Ra_CH0_CONT 		      0x09
#define	E_Tw2815Ra_CH0_BRT 		        0x0A
#define	E_Tw2815Ra_CH0_LUMCON 	      0x0B
#define	E_Tw2815Ra_CH0_COLRCON        0x0C
#define	E_Tw2815Ra_CH0_OUTFMT 	      0x0D
#define		E_Tw2815Rv_OUTFMT_SEL_CH_SFT		0
#define		E_Tw2815Rv_OUTFMT_DUAL_CH				0x04
#define		E_Tw2815Rv_OUTFMT_SW_RESET			0x08
#define		E_Tw2815Rv_OUTFMT_ANA_SW				0x10
#define		E_Tw2815Rv_OUTFMT_BGND_COLR			0x20
#define		E_Tw2815Rv_OUTFMT_BGND_EN				0x40

#define	E_Tw2815Ra_CH1_FORMAT					0x11	 	
#define	E_Tw2815Ra_CH1_HDELAY 	      0x12
#define	E_Tw2815Ra_CH1_HACTIVE        0x13
#define	E_Tw2815Ra_CH1_VDELAY 	      0x14
#define	E_Tw2815Ra_CH1_VACTIVE        0x15
#define	E_Tw2815Ra_CH1_MSB_ACTV       0x16
#define	E_Tw2815Ra_CH1_HUE 		        0x17
#define	E_Tw2815Ra_CH1_SAT 		        0x18
#define	E_Tw2815Ra_CH1_CONT 		      0x19
#define	E_Tw2815Ra_CH1_BRT 		        0x1A
#define	E_Tw2815Ra_CH1_LUMCON 	      0x1B
#define	E_Tw2815Ra_CH1_COLRCON        0x1C
#define	E_Tw2815Ra_CH1_OUTFMT 	      0x1D

#define	E_Tw2815Ra_CH2_FORMAT					0x21	 	
#define	E_Tw2815Ra_CH2_HDELAY 	      0x22
#define	E_Tw2815Ra_CH2_HACTIVE        0x23
#define	E_Tw2815Ra_CH2_VDELAY 	      0x24
#define	E_Tw2815Ra_CH2_VACTIVE        0x25
#define	E_Tw2815Ra_CH2_MSB_ACTV       0x26
#define	E_Tw2815Ra_CH2_HUE 		        0x27
#define	E_Tw2815Ra_CH2_SAT 		        0x28
#define	E_Tw2815Ra_CH2_CONT 		      0x29
#define	E_Tw2815Ra_CH2_BRT 		        0x2A
#define	E_Tw2815Ra_CH2_LUMCON 	      0x2B
#define	E_Tw2815Ra_CH2_COLRCON        0x2C
#define	E_Tw2815Ra_CH2_OUTFMT 	      0x2D

#define	E_Tw2815Ra_CH3_FORMAT					0x31	 	
#define	E_Tw2815Ra_CH3_HDELAY 	      0x32
#define	E_Tw2815Ra_CH3_HACTIVE        0x33
#define	E_Tw2815Ra_CH3_VDELAY 	      0x34
#define	E_Tw2815Ra_CH3_VACTIVE        0x35
#define	E_Tw2815Ra_CH3_MSB_ACTV       0x36
#define	E_Tw2815Ra_CH3_HUE 		        0x37
#define	E_Tw2815Ra_CH3_SAT 		        0x38
#define	E_Tw2815Ra_CH3_CONT 		      0x39
#define	E_Tw2815Ra_CH3_BRT 		        0x3A
#define	E_Tw2815Ra_CH3_LUMCON 	      0x3B
#define	E_Tw2815Ra_CH3_COLRCON        0x3C
#define	E_Tw2815Ra_CH3_OUTFMT 	      0x3D

#define	E_Tw2815Ra_PEAKAGC1  					0x41
#define	E_Tw2815Ra_PEAKAGC2           0x42
#define	E_Tw2815Ra_MISC 		          0x43
#define		E_Tw2815Rv_CLKP_MD					0x01
#define		E_Tw2815Rv_CLKN_MD					0x02
#define		E_Tw2815Rv_CLKP_OEB					0x04
#define		E_Tw2815Rv_CLKN_OEB					0x08
#define		E_Tw2815Rv_NOVID_656				0x10
#define		E_Tw2815Rv_LIM16						0x20
#define		E_Tw2815Rv_AUTO_BGND				0x40
#define		E_Tw2815Rv_OE								0x80

#define	E_Tw2815Ra_AGCTIME 	          0x44
#define	E_Tw2815Ra_HSWIDTH 	          0x45
#define	E_Tw2815Ra_SYNCPOL 	          0x46
#define	E_Tw2815Ra_CFILTER 	          0x47
#define	E_Tw2815Ra_CDEL 		          0x48
#define	E_Tw2815Ra_U_GAIN 	          0x49
#define	E_Tw2815Ra_V_GAIN 	          0x4A
#define	E_Tw2815Ra_U_OFF 		          0x4B
#define	E_Tw2815Ra_V_OFF 		          0x4C
#define	E_Tw2815Ra_CLK_CON 	          0x4D
#define	E_Tw2815Ra_MPP_MODE1          0x4E
#define	E_Tw2815Ra_MPP_MODE2          0x4F
#define	E_Tw2815Ra_ADC_PWDN           0x50
#define	E_Tw2815Ra_NOVID_MD           0x51
#define	E_Tw2815Ra_DEV_ID							0x59
#define	E_Tw2815Ra_STATE_DET          0x5A
#define	E_Tw2815Ra_VDET_ENA           0x5B
#define	E_Tw2815Ra_DET_MODE           0x5C
#define	E_Tw2815Ra_RM_CON							0x62
#define	E_Tw2815Ra_CIF_MODE           0x71
#define	E_Tw2815Ra_OUT_54M 	          0x75

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

//====================================================================
//global function

	
//====================================================================
//out Enviroment conditional
#endif

