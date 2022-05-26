/*
	DVR PTZ protocol V2 DMAX

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrPtzV2PtDmax
#define	_EPC_GrPtzV2PtDmax

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/PtzV2/GrPtzV2Base.h>
	
//====================================================================
//constance

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrPtzV2PtDmax	:	public	Cls_GrPtzV2PtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Och, __u8 A_Ocl, __u8 A_Db1, __u8 A_Db2, __u8 A_Db3, __u8 A_Db4 );
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
		__u8	LcCvtZoomFcsSpeed( __u8 A_Spd );
	public:
    Cls_GrPtzV2PtDmax( void );
    virtual		~Cls_GrPtzV2PtDmax();

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
		virtual	void	DoMenuExit( Ptr_GrPtzV2ChMng A_PtrMng );

		virtual	void	TourSet( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour set
		virtual	void	TourRun( Ptr_GrPtzV2ChMng A_PtrMng );		// H/W tour run
		virtual	void	TourStop( Ptr_GrPtzV2ChMng A_PtrMng );	// H/W tour stop
};
	
//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif


