/*
	DVR PTZ protocol V2 PELCO-D

*/

//--------------------------------------------------------------------
//Environment conditional
#ifndef	_EPC_GrPtzV2PtPelcoD
#define	_EPC_GrPtzV2PtPelcoD

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

class	Cls_GrPtzV2PtPelcoD	:	public	Cls_GrPtzV2PtBase
{
	private:

	protected:
		//local function
		void	LcSendPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Cmd1, __u8 A_Cmd2, __u8 A_Data1, __u8 A_Data2 );
		void	LcSendSpcPkt( Ptr_GrPtzV2ChMng A_PtrMng, __u8 A_Val3, __u8 A_Val4, __u8 A_Val5, __u8 A_Val6, __u8 A_Val7, __u8 A_Val8, 
			__u8 A_Val9, __u8 A_Val10 ); 
		__u8	LcCvtPanTiltSpeed( __u8 A_Spd );
	public:
    Cls_GrPtzV2PtPelcoD( void );
    virtual		~Cls_GrPtzV2PtPelcoD();

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


