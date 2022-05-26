/*
	lg 1701

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_RegLg1701
#define	_EPC_RegLg1701

//====================================================================
// uses
	
//====================================================================
//constance

#define E_Lg1701BankCount				5					/* bank count */
#define E_Lg1701DecCount				4					/* decoder count */
#define E_Lg1701DecBank					1					/* decoder start bank */

//bank 0
#define E_Lg1701RaDevId					0x00			/* device id */
#define	E_Lg1701RvDevId						0x01			/* lg1701 device id value */
#define E_Lg1701RaDacPower			0x03
#define E_Lg1701RaClkEnable			0x05

#define E_Lg1701RaGpioStat			0x43			/* gpio status */

//decoder
#define E_Lg1701RaContrast			0x0B			/* brightness */
#define E_Lg1701RaBright				0x0C			/* brightness */
#define E_Lg1701RaSat						0x0D			/* saturation */

#define E_Lg1701RaVdoStat2			0x4A			/* video status */
#define E_Lg1701RvDetPal					0x01			/* pal colour detected */
#define E_Lg1701RvDet625					0x04			/* pal scanline detected */


//common
#define E_Lg1701RaBank					0xFF			/* bank register */

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

