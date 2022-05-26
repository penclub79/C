/*
	tw2865 chip manage

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrHwTw2865
#define	_EPC_GrHwTw2865

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<Hw/Tw2865/RegTw2865.h>
	
//====================================================================
//constance
#define E_GrHwTw2865StageMiddle				0
#define E_GrHwTw2865StageFirst				1
#define E_GrHwTw2865StageLast					2

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class
class Cls_GrHwTw2865
{
	private:
		__u8		m_I2cCh;
		__u8		m_ChipAdr;			//chip address
		__u8		m_I2cRtSlot;

		__u32		m_Stage;
		BOOL8		m_IsPal;				//signal type
		__u8		m_ClkDly;
		BOOL8		m_IsEncSvideo;	//svideo output

		//agc
		__u8		m_RvAgcEn;			//agc ctrl backup
		__u8		m_RvAgcGain[4];	//agc gain value backup

		//detect
		BOOL8		m_IsSigStatReq;
		__u8		m_SigStatLast;
		BOOL8		m_IsVideoReq[4];
		__u8		m_IsVideoLast;

		BOOL8		m_IsLiveReq;
		BOOL8		m_IsLive;
	protected:
		void		WriteReg( __u8 A_RegAdr, __u8 A_RegVal );
		__u8		ReadReg( __u8 A_RegAdr );

	public:
		Cls_GrHwTw2865( __u32 A_Stage, __u8 A_I2cCh, __u8 A_I2cRtSlot, __u32 A_Adr, __u8 A_ClkDly, BOOL8 A_IsEncSvideo );
		virtual	~Cls_GrHwTw2865();

		//manage
		void		SetSigMode(	BOOL8 A_IsPal );
		BOOL8		IsSigPal( void );

		BOOL8		IcTest( void );
		void		SetClkDly(__u8 A_Delay);

		void		AgcSet( __u8 A_Ch, BOOL8 A_IsOn, __u16 A_Value );
		void		SetBright( __u8 A_Ch, __u8 A_BriVal );
		void		SetContrast( __u8 A_Ch, __u8 A_ConVal );
		void		SetHue( __u8 A_Ch, __u8 A_Val );
		__u8		SigDetMapGet( void );
		__u8		VideoSigStatus( void );

		//encorder
		void		EncYgainSet( __u8 A_Val );
		void		EncCgainSet( __u8 A_Val );
		void		EncSgainSet( __u8 A_Val );
		void		EncBgainSet( __u8 A_Val );

		//ado object
		void		AdoSetOutMux( __u8 A_Ch );
		void		AdoSetOutVol( __u8 A_Vol );

		BOOL8		IsLiveDev();
};


//====================================================================
//global function

//====================================================================
//out Enviroment conditional
#endif

