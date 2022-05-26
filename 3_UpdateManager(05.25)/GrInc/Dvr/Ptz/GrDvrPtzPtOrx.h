/*
	dvr ptz protocol orx

*/

//--------------------------------------------------------------------
//Enviroment conditional
#ifndef	_EPC_GrDvrPtzPtOrx
#define	_EPC_GrDvrPtzPtOrx

//====================================================================
// uses
#include <GrTypeBase.h>
#include <GrTaskSvr.h>
#include <Dvr/GrDvrBase.h>
#include <Dvr/Ptz/GrDvrPtzPtBase.h>
	
//====================================================================
//constance
#define E_GrDvrPtzPtOrxDoLeft		0x01
#define E_GrDvrPtzPtOrxDoRight	0x02
#define	E_GrDvrPtzPtOrxDoUp			0x04
#define E_GrDvrPtzPtOrxDoDown		0x08
#define E_GrDvrPtzPtOrxDoNear		0x10
#define E_GrDvrPtzPtOrxDoFar		0x20
#define E_GrDvrPtzPtOrxDoTele		0x40
#define E_GrDvrPtzPtOrxDoWide		0x80

//====================================================================
//global macro

//====================================================================
//global type

//====================================================================
//class

class	Cls_GrDvrPtzPtOrx	:	public	Cls_GrDvrPtzPtBase
{
	private:
		__u8	m_LastDoMap;
		__u8	m_LastDoTime;
	protected:
		//local function
		void	LcSendPkt( __u8 A_Data4, __u8 A_Data5 );
	public:
    Cls_GrDvrPtzPtOrx( __u8 A_UartCh, __u8 A_Id, __u8 A_Ch );
    virtual		~Cls_GrDvrPtzPtOrx();

		//runtime
		virtual	void	DoTimer( void );

		virtual	__u8	DoPanTilt( BOOL8 A_IsLeft, BOOL8 A_IsRight, BOOL8 A_IsUp, BOOL8 A_IsDown, 
			__u8 A_SpdPan, __u8 A_SpdTilt, __u8 A_AutoStop );
		virtual	__u8	DoZoom( BOOL8 A_IsWide, __u8 A_Speed, __u8 A_AutoStop );
		virtual	__u8	DoFocus( BOOL8 A_IsFar, __u8 A_Speed, __u8 A_AutoStop );
		virtual	void	DoStop( void );

};
	
//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif


