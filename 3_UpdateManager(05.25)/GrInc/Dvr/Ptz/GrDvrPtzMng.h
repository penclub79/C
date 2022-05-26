/*
	dvr ptz manager

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzMng
#define	_EPC_GrDvrPtzMng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <GrTaskCli.h>
#include	<Dvr/Ptz/GrDvrPtzPtBase.h>
	
//====================================================================
//constance
#define E_GrDvrPtzDfltSpeed				0xFF

#define E_GrDvrPtzMaxModelCnt			23

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrDvrPtzChMng 
{
	__u8	Model;
	__u8	UartCh;
	__u8	Id;
	__u8	SpdPan;
	__u8	SpdTilt;
	__u8	SpdZoom;
	__u8	SpdFocus;
	BOOL8	IsRvsPan;
	BOOL8	IsRvsTilt;
	__u8	AutoStopCnt;
	BOOL8	IsTouring;
	__u8	TourIdx;
	__u8	TourTime;
	__u8	TourStanby;		//tour stanby value
	BOOL8	IsHwTour;
	__u8	TourStbyCnt;

	St_GrDvrPtzTourItm	TourTbl[E_GrDvrPtzMaxTourCnt];

	Cls_GrDvrPtzPtBase*	ObjPrtc;
}	Ptr_GrDvrPtzChMng;


//====================================================================
//class

class	Cls_GrDvrPtzMng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		__u32		m_TourTimer;
		__u8		m_MaxChCnt;
		St_GrDvrPtzChMng	m_ChTbl[E_GrDvrMaxChCnt];
		__u8		m_TourStanbyTime;

	protected:
		//local function
		Cls_GrDvrPtzPtBase*	LcPrtcCreate( __u8 A_Model, BOOL8 A_UartCh, BOOL8 A_Id, __u8 A_Ch );

		//local command
		void	LcSetup( __u8 A_Ch, __u8 A_Model, __u8 A_UartCh, __u8 A_Id, __u8 A_SpdPan, __u8 A_SpdTilt, 
			__u8 A_SpdZoom, __u8 A_SpdFocus, BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour );
		void	LcTourSave( __u8 A_Ch, Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		void	LcDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		void	LcDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		void	LcDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		void	LcDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		void	LcDoStop( __u8 A_Ch );
		void	LcDoAutoFocus( __u8 A_Ch );
		void	LcDoMenuCall( __u8 A_Ch );
		void	LcDoMenuExit( __u8 A_Ch );
		void	LcDoMenuLeft( __u8 A_Ch );
		void	LcDoMenuRight( __u8 A_Ch );
		void	LcDoMenuUp( __u8 A_Ch );
		void	LcDoMenuDown( __u8 A_Ch );
		void	LcDoPresetSet( __u8 A_Ch, __u8 A_Id );
		void	LcDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed );
		BOOL8	LcIsMenuAble( __u8 A_Ch );
		void	LcTourRun( __u8 A_Ch, BOOL8 A_IsRun );
		void	LcDoMenuOk( __u8 A_Ch );
		void	LcDoMenuCancel( __u8 A_Ch );
		void	LcTourStanby( __u8 A_Ch );
		void	LcIdView( __u8 A_Ch, BOOL8 A_IsOn );
		void	LcIdSet( __u8 A_Ch, __u8 A_IdNew );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrDvrPtzMng( Cls_GrTaskSvr* A_TaskSvr, __u8 A_MaxChCnt );
    virtual		~Cls_GrDvrPtzMng();

		WCHAR*	ModelNameGet( __u8 A_Model );

		void	Setup( __u8 A_Ch, __u8 A_Model, __u8 A_UartCh, __u8 A_Id, __u8 A_SpdPan, __u8 A_SpdTilt, 
			__u8 A_SpdZoom, __u8 A_SpdFocus, BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour );
		void	TourSave( __u8 A_Ch, Ptr_GrDvrPtzTourItm A_PtrTbl, __u8 A_Cnt );
		void	DoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		void	DoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		void	DoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		void	DoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		void	DoStop( __u8 A_Ch );
		void	DoAutoFocus( __u8 A_Ch );
		void	DoMenuCall( __u8 A_Ch );
		void	DoMenuExit( __u8 A_Ch );
		void	DoMenuOk( __u8 A_Ch );
		void	DoMenuCancel( __u8 A_Ch );
		void	DoMenuLeft( __u8 A_Ch );
		void	DoMenuRight( __u8 A_Ch );
		void	DoMenuUp( __u8 A_Ch );
		void	DoMenuDown( __u8 A_Ch );
		void	DoPresetSet( __u8 A_Ch, __u8 A_Id );
		void	DoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed );
		BOOL8	IsMenuAble( __u8 A_Ch );
		BOOL8	IsTouring( __u8 A_Ch );
		void	TouringRun( __u8 A_Ch, BOOL8 A_IsRun );
		void	ExIdView( __u8 A_Ch, BOOL8 A_IsOn );
		void	ExIdSet( __u8 A_Ch, __u8 A_IdNew ); 
		void	TourStanbyTimeSet( __u8 A_Ch, __u8 A_Sec );
		__u8	TourStanbyTimeGet( __u8 A_Ch );
		void	MaxSpdToggle( __u8 A_Ch );
};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


