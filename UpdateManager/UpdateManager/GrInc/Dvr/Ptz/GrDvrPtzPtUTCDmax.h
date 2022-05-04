/*
	dvr ptz protocol dongyang unitech - dmax

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrPtzPtUTCDmax
#define	_EPC_GrPtzPtUTCDmax

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

class	Cls_GrDvrPtzPtUTCDmax	:	public	Cls_GrDvrPtzPtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( __u8 A_Och, __u8 A_Ocl, __u8 A_Db1, __u8 A_Db2, __u8 A_Db3, __u8 A_Db4 );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
		__u8	LcCvtZoomFcsSpeed( __u8 A_Spd );
	public:
    Cls_GrDvrPtzPtUTCDmax( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtUTCDmax();

		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );
		virtual	void	DoPresetSet( __u8 A_Id );
		virtual	void	DoPresetMove( __u8 A_Id, __u8 A_Speed );
		virtual	void	TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		virtual	void	TourRun( void );
		virtual	void	TourStop( void );
		virtual	BOOL8	IsMenuAble( void );
		virtual	void	DoMenuCall( void );
		virtual	void	DoMenuExit( void );
		virtual	void	DoMenuOk( void );
		virtual	void	DoMenuCancel( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


