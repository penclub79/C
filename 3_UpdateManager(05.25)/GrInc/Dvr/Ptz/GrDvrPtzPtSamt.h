/*
	dvr ptz protocol samsung-t
*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtSamt
#define	_EPC_GrDvrPtzPtSamt

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

class	Cls_GrDvrPtzPtSamt	:	public	Cls_GrDvrPtzPtBase
{
	private:
		BOOL8	m_IsMaxPtzSpd;
	protected:
		//local function
		void	LcSendPkt( __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_PanSpd, __u8 A_TiltSpd, __u8 A_ZmFcsSpd );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
		__u8	LcCvtZoomFcsSpeed( __u8 A_Spd );

	public:
    Cls_GrDvrPtzPtSamt( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch, BOOL8 A_MaxPtzSpd );
    virtual		~Cls_GrDvrPtzPtSamt();

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
		virtual	void	DoMenuExit( void );
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


