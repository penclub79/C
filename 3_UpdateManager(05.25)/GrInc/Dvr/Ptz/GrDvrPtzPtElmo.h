/*
	dvr ptz protocol elmo

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtElmo
#define	_EPC_GrDvrPtzPtElmo

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Ptz/GrDvrPtzPtBase.h>
	
//====================================================================
//constance

#define E_GrDvrPtzPtElmoDoLeft		0x01
#define E_GrDvrPtzPtElmoDoRight		0x02
#define	E_GrDvrPtzPtElmoDoUp			0x04
#define E_GrDvrPtzPtElmoDoDown		0x08
#define E_GrDvrPtzPtElmoDoNear		0x10
#define E_GrDvrPtzPtElmoDoFar			0x20
#define E_GrDvrPtzPtElmoDoTele		0x40
#define E_GrDvrPtzPtElmoDoWide		0x80

#define E_GrDvrPtzPtElmoMaskPan		(E_GrDvrPtzPtElmoDoLeft | E_GrDvrPtzPtElmoDoRight)
#define E_GrDvrPtzPtElmoMaskTilt	(E_GrDvrPtzPtElmoDoUp | E_GrDvrPtzPtElmoDoDown)
#define E_GrDvrPtzPtElmoMaskFocus	(E_GrDvrPtzPtElmoDoNear | E_GrDvrPtzPtElmoDoFar)
#define E_GrDvrPtzPtElmoMaskZoom	(E_GrDvrPtzPtElmoDoTele | E_GrDvrPtzPtElmoDoWide)

#define E_GrDvrPtzPtElmoMaskPantilt	(E_GrDvrPtzPtElmoMaskPan | E_GrDvrPtzPtElmoMaskTilt)

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrPtzPtElmo	:	public	Cls_GrDvrPtzPtBase
{
	private:
		__u8	m_ZoomSpd;			//last zoom speed
		__u8	m_DoMap;				//last doing map
		__u8	m_ReqPanSpd;		//pan speed
		__u8	m_ReqTiltSpd;		//tilt speed
		__u8	m_ReqFocusSpd;	//focus speed
		__u8	m_ReqZoomSpd;		//request zoom speed
		__u8	m_ReqDoMap;			//request do map
		__u8	m_ReqPrSet;			//preset set, 0xff = not use
		__u8	m_ReqPrMove;		//preset move, 0xff = not use
		BOOL8	m_IsReqAtFcs;		//autofocus

	protected:
		//local function
		void	LcSendPkt( __u8 A_Cmd2, __u8 A_Cmd3, __u8 A_Cmd4 );
		__u8	LcCvtElmoPanTiltSpeed( __u8 A_Spd );
		__u8	LcCvtElmoZoomFocusSpeed( __u8 A_Spd );
	public:
    Cls_GrDvrPtzPtElmo( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtElmo();

		virtual	void	DoTimer( void );

		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );
		virtual	void	DoAutoFocus( void );
		virtual	void	DoPresetSet( __u8 A_Id );
		virtual	void	DoPresetMove( __u8 A_Id, __u8 A_Speed );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


