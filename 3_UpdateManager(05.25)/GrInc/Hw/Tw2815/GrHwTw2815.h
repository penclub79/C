/*
	tw2815 chip manage

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwTw2815
#define	_EPC_GrHwTw2815

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/Tw2815/RegTw2815.h>
	
//====================================================================
//constance
#define E_GrHwTw2815StageMiddle				0
#define E_GrHwTw2815StageFirst				1
#define E_GrHwTw2815StageLast					2
#define E_GrHwTw2815StageSingle				3

#define E_GrHwTw2815DfltNtscHdelay		(32 - 8)
#define E_GrHwTw2815DfltPalHdelay			0x22
#define E_GrHwTw2815DfltHactive				(720 + 16)

#define E_GrHwTw2815DfltVdelayNtsc		(7 - 2)
#define E_GrHwTw2815DfltVactiveNtsc		(240 + 4)

#define E_GrHwTw2815DfltVdelayPal			(4 - 2)
#define E_GrHwTw2815DfltVactivePal		(288 + 4)

#define E_GrHwTw2815ClkDelayP					1
#define E_GrHwTw2815ClkDelayN					1

#define E_GrHwTw2815AdoMuxPb					16

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwTw2815
{
	private:
		__u8		m_I2cCh;
		__u8		m_ChipAdr;			//chip address
		__u8		m_I2cRtSlot;

		__u32		m_Stage;
		BOOL8		m_IsPal;				//signal type
		__u8		m_ClkDly;

	protected:
		void		WriteReg( __u8 A_RegAdr, __u8 A_RegVal );
		__u8		ReadReg( __u8 A_RegAdr );

	public:
		Cls_GrHwTw2815( __u32 A_Stage, __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_ClkDly );
		virtual	~Cls_GrHwTw2815();

		//manage
		void		SetSigMode(	BOOL8 A_IsPal );
		BOOL8		IsSigPal( void );

		BOOL8		IcTest( void );
		void		SetBright( __u8 A_Ch, __u8 A_BriVal );
		void		SetContrast( __u8 A_Ch, __u8 A_ConVal );
		void		SetSat( __u8 A_Ch, __u8 A_ConVal );
		BOOL8		ReqSigStatGet( void );
		void		SetClkDly(__u8 A_Delay);
		//ado object
		void		AdoSetOutMux( __u32 A_Ch );
		void		AdoSetOutVol( __u32 A_Vol );
		
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

