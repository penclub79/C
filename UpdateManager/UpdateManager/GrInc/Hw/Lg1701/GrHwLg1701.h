/*
	lg1701 chip manage

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwLg1701
#define	_EPC_GrHwLg1701

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/Lg1701/RegLg1701.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwLg1701
{
	private:
		__u8		m_I2cCh;
		__u8		m_ChipAdr;			//chip address
		__u8		m_I2cRtSlot;

		__u8		m_Stage;				//stage number 0 - 4
		BOOL8		m_IsPal;				//signal type
		__u8		m_ClkDly;
		BOOL8		m_IsEncSvdo;		//svideo out

		BOOL8		m_IsAdoOn;			//audion on / off

	protected:
		void		WriteReg( __u8 A_RegAdr, __u8 A_RegVal );
		__u8		ReadReg( __u8 A_RegAdr );

	public:
		Cls_GrHwLg1701( __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_Stage, __u8 A_ClkDly, BOOL8 A_IsEncSvideo );
		virtual	~Cls_GrHwLg1701();

		//manage
		void		SetSigMode(	BOOL8 A_IsPal );
		BOOL8		IsSigPal( void );

		BOOL8		IcTest( void );
		void		SetBright( __u8 A_Ch, __u8 A_BriVal );
		void		SetContrast( __u8 A_Ch, __u8 A_ConVal );
		void		SetSat( __u8 A_Ch, __u8 A_Val );
		__u8		SigStatGet( void );
		void		SetClkDly(__u8 A_Delay);
		//ado object
		void		AdoSetOutMux( __u32 A_Ch );
		void		AdoSetOutVol( __u32 A_Vol );
		void		AdoSetOnOff( BOOL8 A_IsOn );
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

