/*
	DVR PTZ Base

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrPtzV2Base
#define	_EPC_GrPtzV2Base

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
	
//====================================================================
//constance
#define E_GrPtzV2DfltSpeed			0xFF

#define E_GrPtzV2PtMaxSpeed			4
#define	E_GrPtzV2MaxTourCnt			12

#define E_GrPtzV2UserDataCnt		4

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct St_GrPtzV2TourItm 
{
	__u8	PresetId;
	__u8	Spd;
	__u8	Maintain;
	__u8	_rsvd;
}	*Ptr_GrPtzV2TourItm;

typedef	struct St_GrPtzV2ChMng
{
	__u8	Model;
	__u8	Id;
	__u8	Ch;
	__u8	UartCh;
	__u32	Baud;
	__u8	DataBit;
	__u8	Parity;
	__u8	StopBit;
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
	__u8	TourStanby;		//tour standby value
	BOOL8	IsHwTour;
	__u8	TourStbyCnt;
	__u8	TourCnt;
	St_GrPtzV2TourItm	TourTbl[E_GrPtzV2MaxTourCnt];
	__u32	UserData[E_GrPtzV2UserDataCnt];
}	*Ptr_GrPtzV2ChMng;

//====================================================================
//class

class	Cls_GrPtzV2PtBase
{
	protected:
		Def_WstrName	m_StrModel;
	public:
    Cls_GrPtzV2PtBase( void );
    virtual		~Cls_GrPtzV2PtBase();

		// information
		WCHAR*	ModelNameGet( void );
		virtual	BOOL8	IsMenuAble( void );

		// command
		virtual	void	Init( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoPanTilt( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt );
		virtual	void	DoZoom( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsWide, __u8 A_Speed );
		virtual	void	DoFocus( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsFar, __u8 A_Speed );
		virtual	void	DoIris( Ptr_GrPtzV2ChMng A_PtrMng, BOOL8 A_IsOpen, __u8 A_Speed );
		virtual	void	DoStop( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoAutoFocus( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoPresetSet( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id );
		virtual	void	DoPresetMove( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Id, __u8 A_Speed );
		virtual	void	DoMenuCall( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuLeft( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuRight( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuUp( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuDown( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	TourSet( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour set
		virtual	void	TourRun( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour run
		virtual	void	TourStop( Ptr_GrPtzV2ChMng A_PtrMng );	// H/W tour stop
		virtual	void	ExIdView( __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, 
			__u8 A_Parity, __u8 A_StopBit, BOOL8 A_IsOn );
		virtual	void	ExIdSet( __u8 A_Id, __u8 A_UartCh, __u32 A_Baud, __u8 A_DataBit, 
			__u8 A_Parity, __u8 A_StopBit, __u8 A_IdNew ); 
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


