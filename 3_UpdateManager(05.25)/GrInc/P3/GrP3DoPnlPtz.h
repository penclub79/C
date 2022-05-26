/*
	GAUSE Platform panel PTZ

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoPnlPtz
#define	_EPC_GrP3DoPnlPtz

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>

//====================================================================
//constance

#define E_GrP3DoPnlPtzBgCnt			3

#define E_GrP3DoPnlPtzPantilt		0
#define E_GrP3DoPnlPtzZoom			1
#define E_GrP3DoPnlPtzFocus			2

#define E_GrP3DoPnlPtzMseStop		0xFF

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoPnlPtzPara
{
	__u16	PntBg[E_GrP3DoPnlPtzBgCnt];					// paint background
	__u16	_rsvd0;

}	*Ptr_GrP3DoPnlPtzPara;

//====================================================================
//global class
class	Cls_GrP3DoPnlPtz	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoPnlPtzPara	m_ParaPnlPtz;

		WCHAR*	m_StrTitle;
		
		void*		m_PtrGvs;

		//__u8	m_Ch;
		__u8	m_PtzMode;
		Cls_GrPtzV2Mng*	m_PtzMng;

		__u8	m_MseMode;
		__u8	m_MseSpdX;
		__u8	m_MseSpdY;
		BOOL8	m_IsMseLeft;
		BOOL8	m_IsMseRight;
		BOOL8	m_IsMseUp;
		BOOL8	m_IsMseDown;

		BOOL8	m_IsKeyLeft;
		BOOL8	m_IsKeyRight;
		BOOL8	m_IsKeyUp;
		BOOL8	m_IsKeyDown;

		// local
		void	LcMsePtzRun( __u16 A_X, __u16 A_Y );

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		virtual	BOOL8	ProcEvtWheelUp( void );
		virtual	BOOL8	ProcEvtWheelDown( void );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoPnlPtz( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoPnlPtz( void );

		
		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

