/*
	DVR PTZ protocol V2 VCL

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrPtzV2PtVcl
#define	_EPC_GrPtzV2PtVcl

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
	
//====================================================================
//constance

#define E_GrPtzV2PtVclOnMapIdx		0

#define E_GrPtzV2PtVclOnCnt				10

#define E_GrPtzV2PtVclOnLeft			0
#define E_GrPtzV2PtVclOnUp				1
#define E_GrPtzV2PtVclOnRight			2
#define E_GrPtzV2PtVclOnDown			3
#define E_GrPtzV2PtVclOnTele			4
#define E_GrPtzV2PtVclOnWide			5
#define E_GrPtzV2PtVclOnFar				6
#define E_GrPtzV2PtVclOnNear			7
#define E_GrPtzV2PtVclOnOpen			8
#define E_GrPtzV2PtVclOnClose			9


//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrPtzV2PtVcl	:	public	Cls_GrPtzV2PtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Cmd, __u8 A_Para = 0, BOOL8 A_IsPara = FALSE );
		void	LcSendTourData( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Preset, __u8 A_Speed, __u8 A_Dwell );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
		void	LcSendStop( Ptr_GrPtzV2ChMng A_PtrMng );
	public:
    Cls_GrPtzV2PtVcl( void );
    virtual		~Cls_GrPtzV2PtVcl();

		virtual	BOOL8	IsMenuAble( void );

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
		virtual	void	DoMenuOk( Ptr_GrPtzV2ChMng A_PtrMng );
		virtual	void	DoMenuCancel( Ptr_GrPtzV2ChMng A_PtrMng );

		virtual	void	TourSet( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour set
		virtual	void	TourRun( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour run
		virtual	void	TourStop( Ptr_GrPtzV2ChMng A_PtrMng );	// H/W tour stop
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


