/*
	tw 2865

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_RegTw2865
#define	_EPC_RegTw2865

//====================================================================
// uses
	
//====================================================================
//constance

#define E_Tw2865BaseDevAdr		0x50			/*	base device address	*/

#define E_Tw2865DevId					0x20			/*	device id		*/

//register

#define E_Tw2865RadrMasterCtrl			0xDB
#define E_Tw2865RadrMixMute					0xDC		
#define E_Tw2865RadrClkOut1Delay		0x9F		
#define E_Tw2865RadrClkOut2Delay		0x67		
#define E_Tw2865RadrClkOut3Delay		0x68
#define E_Tw2865RadrClkOut4Delay		0x69		
#define E_Tw2865RadrMixOutSel				0xE0		//audio output select
#define E_Tw2865RadrAoGain					0xDF		//audio output gain
#define E_Tw2865RadrEncOut					0x44		//encoder output mode
#define E_Tw2865RadrEncYgain				0x46		
#define E_Tw2865RadrEncCgain				0x47
#define E_Tw2865RadrEncSgain				0x48
#define E_Tw2865RadrEncBgain				0x49
#define E_Tw2865RadrAgcEn						0xAA		//auto gain enable and msb value
#define E_Tw2865RadrAgcGain1				0xAB		//gain value channel 1
#define E_Tw2865RadrAgcGain2				0xAC		//gain value channel 2
#define E_Tw2865RadrAgcGain3				0xAD		//gain value channel 3
#define E_Tw2865RadrAgcGain4				0xAE		//gain value channel 4
#define E_Tw2865RadrBright1					0x01		//brightness channel 1
#define E_Tw2865RadrBright2					0x11		//brightness channel 1
#define E_Tw2865RadrBright3					0x21		//brightness channel 1
#define E_Tw2865RadrBright4					0x31		//brightness channel 1
#define E_Tw2865RadrContrast1				0x02		//contrast channel 1
#define E_Tw2865RadrContrast2				0x12		//contrast channel 2
#define E_Tw2865RadrContrast3				0x22		//contrast channel 3
#define E_Tw2865RadrContrast4				0x32		//contrast channel 4
#define E_Tw2865RadrHue1						0x06		//hue channel 1
#define E_Tw2865RadrHue2						0x16		//hue channel 2
#define E_Tw2865RadrHue3						0x26		//hue channel 3
#define E_Tw2865RadrHue4						0x36		//hue channel 4
#define E_Tw2865RadrAvDetStat				0xFD		//audio video detect status
#define E_Tw2865RadrCh1							0x00		// 0, 1, 2, 3, 4 Field, 5, 6, 7 Vdloss
#define E_Tw2865RadrCh2							0x10
#define E_Tw2865RadrCh3							0x20
#define E_Tw2865RadrCh4							0x30


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

