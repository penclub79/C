/*
	dvr ptz protocol pelco-p

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtPelcoP
#define	_EPC_GrDvrPtzPtPelcoP

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Ptz/GrDvrPtzPtBase.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrPtzPtPelcoP	:	public	Cls_GrDvrPtzPtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( __u8 A_Data1, __u8 A_Data2, __u8 A_Data3, __u8 A_Data4 );
		__u8	LcCvtPanSpeed( __u8 A_Spd );
		__u8	LcCvtTiltSpeed( __u8 A_Spd );
	public:
    Cls_GrDvrPtzPtPelcoP( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtPelcoP();

		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );
		virtual	void	DoPresetSet( __u8 A_Id );
		virtual	void	DoPresetMove( __u8 A_Id, __u8 A_Speed );
		virtual	BOOL8	IsMenuAble( void );
		virtual	void	DoMenuCall( void );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


