/*
	mdin 200 chip manage

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwMdin200
#define	_EPC_GrHwMdin200

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/Mdin200/RegMdin200.h>
	
//====================================================================
//constance

#define E_GrHwMdinOutRes640				0		/* 640 x 480 */
#define E_GrHwMdinOutRes800				1		/* 800 x 600 */
#define E_GrHwMdinOutRes1024			2		/* 1024 x 768 */
#define E_GrHwMdinOutRes1280			3		/* 1280 x 1024 */
#define E_GrHwMdinOutRes1360			4		/* 1360 x 768 */
#define E_GrHwMdinOutRes1440			5		/* 1440 x 900 */
#define E_GrHwMdinOutRes1680			6		/* 1680 x 1050 */
#define E_GrHwMdinOutRes1920			7		/* 1920 x 1080 */

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwMdin200
{
	private:
		__u8		m_I2cCh;
		__u8		m_ChipAdr;			//chip address
		BOOL8		m_IsPal;				//signal type
		__u8		m_OutMode;			//output mode
		__u8		m_I2cSlot;

	protected:
		void		RegWriteByte( __u16 A_RegAdr, __u8 A_RegVal );
		void		RegWriteWord( __u16 A_RegAdr, __u16 A_RegVal );
		__u16		RegReadWord( __u16 A_RegAdr );
		void		LcMainFormatCvt( void );
		void		LcVpllMpllCtrl( void );
		void		LcInputCtrl( void );
		void		LcSyncSize( void );
		void		LcDeintRegSet( void );
		void		LcPeakFltSDEn( void );
		void		LcSurfaceFltEn( void );
		void		LcSyncReset( void );
		void		LcRgbSdOut( void );
		void		LcOutputCtrl( void );
		void		LcCromaFltEn( void );

	public:
		Cls_GrHwMdin200( __u8 A_I2cCh, __u32 A_Adr, __u8 A_Slot );
		virtual	~Cls_GrHwMdin200();

		//manage
		void		SetSigMode(	BOOL8 A_IsPal, __u8 A_OutMode );
		BOOL8		IsSigPal( void );
		__u8		GetOutMode( void );

		void		SetBright( __u8 A_Ch, __u8 A_BriVal );
		void		SetContrast( __u8 A_Ch, __u8 A_ConVal );
		void		SetSat( __u8 A_Ch, __u8 A_Val );
		
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

