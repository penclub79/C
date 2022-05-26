/*	
	dvr ptz protocol Tsd300

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtTsd300
#define	_EPC_GrDvrPtzPtTsd300

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

class	Cls_GrDvrPtzPtTsd300	:	public	Cls_GrDvrPtzPtBase
{
	private:
		
	protected:
		//local function
		void	LcSendSpcPkt( __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, __u8 A_Val7, __u8 A_Val8, 
			__u8 A_Val9, __u8 A_Val10 ); 
		void	LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2 );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );

	public:
		Cls_GrDvrPtzPtTsd300( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
		virtual		~Cls_GrDvrPtzPtTsd300();

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
		virtual	void	DoMenuOk( void );
		virtual	void	DoMenuCancel( void );

		virtual	void	TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		virtual	void	TourRun( void );
		virtual	void	TourStop( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

