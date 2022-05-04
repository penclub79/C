/*
	DVR PTZ V2 manager

*/

//--------------------------------------------------------------------
// Environment conditional
#ifndef	_EPC_GrPtzV2Mng
#define	_EPC_GrPtzV2Mng

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <GrTaskCli.h>
#include	<Dvr/PtzV2/GrPtzV2Base.h>
	
//====================================================================
//constance
#define E_GrPtzV2ModelMaxCnt					32

#define E_GrPtzV2AutoStopChkPeriod		100
#define E_GrPtzV2TourTimePeriodCnt		10
#define E_GrPtzV2TourStanbyDflt				3

//#define	E_GrPtzV2DbgMsg

//command
#define E_GrPtzV2MsgCmdSetup					0		/* setup : St_GrPtzV2MsgSetup */
#define E_GrPtzV2MsgCmdDoPanTilt			1		/* pan tilt : St_GrPtzV2MsgDoPanTilt */
#define E_GrPtzV2MsgCmdDoZoom					2		/* zoom : St_GrPtzV2MsgDoZoom */
#define E_GrPtzV2MsgCmdDoFocus				3		/* focus : St_GrPtzV2MsgDoFocus */
#define E_GrPtzV2MsgCmdDoStop					4		/* stop PTZ : St_GrPtzV2MsgDoStop */
#define E_GrPtzV2MsgCmdDoAutoFocus		5		/* auto focus : St_GrPtzV2MsgDoAutoFocus */
#define E_GrPtzV2MsgCmdDoMenuCall			6		/* menu call : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoMenuExit			7		/* menu exit : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoPrsSet				8		/* preset set : St_GrPtzV2MsgDoPrsSet */
#define E_GrPtzV2MsgCmdDoPrsMove			9		/* preset move : St_GrPtzV2MsgDoPrsMove */
#define E_GrPtzV2MsgCmdIsMenuMode			10	/* check in menu mode : St_GrPtzV2MsgIsMenuMode */
#define E_GrPtzV2MsgCmdTourRun				11	/* set tour run : St_GrPtzV2MsgTourRun */
#define E_GrPtzV2MsgCmdMenuOk					12	/* menu ok : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdMenuCancel			13	/* menu right : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdTourSave				14	/* tour save : St_GrPtzV2MsgTourSave */
#define E_GrPtzV2MsgCmdIdView					15	/* id view : St_GrPtzV2MsgIdView */
#define E_GrPtzV2MsgCmdIdSet					16	/* id set : St_GrPtzV2MsgIdSet */
#define E_GrPtzV2MsgCmdDoMenuLeft			17	/* menu left : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoMenuRight		18	/* menu right : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoMenuUp				19	/* menu up : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoMenuDown			20	/* menu down : St_GrPtzV2MsgDoMenuStd */
#define E_GrPtzV2MsgCmdDoIris					21	/* Iris : St_GrPtzV2MsgDoIris */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrPtzV2MsgHd 
{
	St_GrTaskMsgHd	MsgHd;
	__u32	Cmd;
}	*Ptr_GrPtzV2MsgHd;

typedef	struct St_GrPtzV2MsgSetup 
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	__u8	Model;
	__u8	Id;
	__u8	UartCh;
	__u32	BoudRate;
	__u8	DataBit;
	__u8	Parity;
	__u8	StopBit;
	__u8	SpdPan;
	__u8	SpdTilt;
	__u8	SpdZoom;
	__u8	SpdFocus;
	BOOL8	IsRvsPan;
	BOOL8	IsRvsTilt;
	BOOL8	IsHwTour;

}	*Ptr_GrPtzV2MsgSetup;

typedef	struct St_GrPtzV2MsgTourSave
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	__u8	Cnt;		//tour table count
	Ptr_GrPtzV2TourItm PtrTourTbl;
}	*Ptr_GrPtzV2MsgTourSave;

typedef	struct St_GrPtzV2MsgDoPanTilt
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8 IsLeft;
	BOOL8 IsRight;
	BOOL8 IsUp;
	BOOL8 IsDown;
	__u8	SpdPan;
	__u8	SpdTilt;
	__u8	AutoStop;
}	*Ptr_GrPtzV2MsgDoPanTilt;

typedef	struct St_GrPtzV2MsgDoZoom 
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8 IsWide;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrPtzV2MsgDoZoom;

typedef	struct St_GrPtzV2MsgDoFocus
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8 IsFar;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrPtzV2MsgDoFocus;

typedef	struct St_GrPtzV2MsgDoIris
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8 IsOpen;
	__u8	Speed;
	__u8	AutoStop;
}	*Ptr_GrPtzV2MsgDoIris;

typedef	struct St_GrPtzV2MsgDoStop
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrPtzV2MsgDoStop;

typedef	struct St_GrPtzV2MsgDoAutoFocus
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrPtzV2MsgDoAutoFocus;

typedef	struct St_GrPtzV2MsgDoMenuStd
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
}	*Ptr_GrPtzV2MsgDoMenuStd;

typedef	struct St_GrPtzV2MsgDoPrsSet
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	__u8	Id;
}	*Ptr_GrPtzV2MsgDoPrsSet;

typedef	struct St_GrPtzV2MsgDoPrsMove
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	__u8	Id;
	__u8	Speed;
}	*Ptr_GrPtzV2MsgDoPrsMove;

typedef	struct St_GrPtzV2MsgIsMenuMode
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8*	PtrRslt;
}	*Ptr_GrPtzV2MsgIsMenuMode;

typedef	struct St_GrPtzV2MsgTourRun
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Ch;
	BOOL8	IsRun;
}	*Ptr_GrPtzV2MsgTourRun;

typedef	struct St_GrPtzV2MsgIdView
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Model;
	__u8	Id;
	__u8	UartCh;
	__u8	DataBit;
	__u32	Baud;
	__u8	Parity;
	__u8	StopBit;
	BOOL8	IsOn;
}	*Ptr_GrPtzV2MsgIdView;

typedef	struct St_GrPtzV2MsgIdSet
{
	St_GrPtzV2MsgHd	Hd;
	__u8	Model;
	__u8	Id;
	__u8	UartCh;
	__u8	DataBit;
	__u32	Baud;
	__u8	Parity;
	__u8	StopBit;
	__u8	IdNew;
}	*Ptr_GrPtzV2MsgIdSet;

//====================================================================
//class

class	Cls_GrPtzV2Mng	:	public	Cls_GrTaskCli
{
	private:
		Cls_GrTaskSvr*	m_TaskSvr;
		__u32		m_MsgAloc;
		__u32		m_HndlTimer;

		__u32		m_TourTimer;
		__u8		m_MaxChCnt;
		St_GrPtzV2ChMng	m_ChTbl[E_GrDvrMaxChCnt];
		__u8		m_TourStanbyTime;

		Def_WstrName	m_StrModelNone;

		__u8		m_ModelCnt;
		Cls_GrPtzV2PtBase*	m_ObjPrtc[E_GrPtzV2ModelMaxCnt];		// protocol object

	protected:
		//local function
		void	LcPrtcBuild( void );
		void	LcTourStandby( __u8 A_Ch );

		//event
		virtual	BOOL8	RtlMsgProc( Ptr_GrTaskMsgHd A_PtrMsg );		/* return = message processed */
		virtual	void	RtlTimer( __u32 A_Hndl );

	public:
    Cls_GrPtzV2Mng( Cls_GrTaskSvr* A_TaskSvr, __u8 A_MaxChCnt );
    virtual		~Cls_GrPtzV2Mng();

		WCHAR*	ModelNameGet( __u8 A_Model );
		__u8		ModelCntGet( void );

		void	ImmSetup( __u8 A_Ch, __u8 A_Model, __u8 A_Id, __u8 A_UartCh,
			__u32 A_BoudRate, __u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit,
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, 
			BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour );
		void	ImmTourSave( __u8 A_Ch, Ptr_GrPtzV2TourItm A_PtrTbl, __u8 A_Cnt );
		void	ImmDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		void	ImmDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		void	ImmDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		void	ImmDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		void	ImmDoStop( __u8 A_Ch );
		void	ImmDoAutoFocus( __u8 A_Ch );
		void	ImmDoMenuCall( __u8 A_Ch );
		void	ImmDoMenuExit( __u8 A_Ch );
		void	ImmDoMenuLeft( __u8 A_Ch );
		void	ImmDoMenuRight( __u8 A_Ch );
		void	ImmDoMenuUp( __u8 A_Ch );
		void	ImmDoMenuDown( __u8 A_Ch );
		void	ImmDoMenuOk( __u8 A_Ch );
		void	ImmDoMenuCancel( __u8 A_Ch );
		void	ImmDoPresetSet( __u8 A_Ch, __u8 A_Id );
		void	ImmDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed );
		void	ImmTourRun( __u8 A_Ch, BOOL8 A_IsRun );
		void	ImmExIdView( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
			__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, BOOL8 A_IsOn );
		void	ImmExIdSet( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
			__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, __u8 A_IdNew );
		BOOL8	ImmIsMenuAble( __u8 A_Ch );

		void	SafeSetup( __u8 A_Ch, __u8 A_Model, __u8 A_Id, __u8 A_UartCh,
			__u32 A_BoudRate, __u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit,
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_SpdZoom, __u8 A_SpdFocus, 
			BOOL8 A_IsRvsPan, BOOL8 A_IsRvsTilt, BOOL8 A_IsHwTour );
		void	SafeTourSave( __u8 A_Ch, Ptr_GrPtzV2TourItm A_PtrTbl, __u8 A_Cnt );
		void	SafeDoPanTilt( __u8 A_Ch, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		void	SafeDoZoom( __u8 A_Ch, BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		void	SafeDoFocus( __u8 A_Ch, BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		void	SafeDoIris( __u8 A_Ch, BOOL8 A_IsOpen, __u8 A_Speed, __u8 A_AutoStop );
		void	SafeDoStop( __u8 A_Ch );
		void	SafeDoAutoFocus( __u8 A_Ch );
		void	SafeDoMenuCall( __u8 A_Ch );
		void	SafeDoMenuExit( __u8 A_Ch );
		void	SafeDoMenuOk( __u8 A_Ch );
		void	SafeDoMenuCancel( __u8 A_Ch );
		void	SafeDoMenuLeft( __u8 A_Ch );
		void	SafeDoMenuRight( __u8 A_Ch );
		void	SafeDoMenuUp( __u8 A_Ch );
		void	SafeDoMenuDown( __u8 A_Ch );
		void	SafeDoPresetSet( __u8 A_Ch, __u8 A_Id );
		void	SafeDoPresetMove( __u8 A_Ch, __u8 A_Id, __u8 A_Speed );
		BOOL8	SafeIsMenuAble( __u8 A_Ch );
		void	SafeTouringRun( __u8 A_Ch, BOOL8 A_IsRun );
		void	SafeExIdView( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
			__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, BOOL8 A_IsOn );
		void	SafeExIdSet( __u8 A_Model, __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, 
			__u8 A_DataBit, __u8 A_Parity, __u8 A_StopBit, __u8 A_IdNew ); 

		BOOL8	IsTouring( __u8 A_Ch );
		void	TourStanbyTimeSet( __u8 A_Ch, __u8 A_Sec );
		__u8	TourStanbyTimeGet( __u8 A_Ch );
		void	MaxSpdToggle( __u8 A_Ch );
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


