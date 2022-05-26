/*
	dvr ptz protocol samsung

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtSams
#define	_EPC_GrDvrPtzPtSams

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

class	Cls_GrDvrPtzPtSams	:	public	Cls_GrDvrPtzPtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2, __u8 A_Data3 );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
		__u8	LcCvtZoomFcsSpeed( __u8 A_Spd );
	public:
    Cls_GrDvrPtzPtSams( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtSams();

		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );
		virtual	void	DoPresetSet( __u8 A_Id );
		virtual	void	DoPresetMove( __u8 A_Id, __u8 A_Speed );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


