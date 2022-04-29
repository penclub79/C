/*
	dvr ptz protocol base

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtFastraxII
#define	_EPC_GrDvrPtzPtFastraxII

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

class	Cls_GrDvrPtzPtFastraxII	:	public	Cls_GrDvrPtzPtBase
{
	protected:
		void	LcSendPkt( __u8 A_Cmd, __u8 A_Data1, __u8 A_Data2 );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );

	public:
    Cls_GrDvrPtzPtFastraxII( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtFastraxII();

		//runtime
		virtual	void	DoTimer( void );

		//status
		virtual	BOOL8	IsMenuAble( void );

		//command
		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoIris( BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );
		virtual	void	DoPresetSet( __u8 A_Id );
		virtual	void	DoPresetMove( __u8 A_Id, __u8 A_Speed );
		virtual	void	DoMenuCall( void );
		virtual	void	DoMenuExit( void );
		virtual	void	DoMenuOk( void );
		virtual	void	DoMenuCancel( void );
		virtual	void	DoMenuLeft( void );
		virtual	void	DoMenuRight( void );
		virtual	void	DoMenuUp( void );
		virtual	void	DoMenuDown( void );
		virtual	void	ExIdView( BOOL8 A_IsOn );
		virtual	void	ExIdSet( __u8 A_IdNew ); 
		virtual	void	TourSet( Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		virtual	void	TourRun( void );
		virtual	void	TourStop( void );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


