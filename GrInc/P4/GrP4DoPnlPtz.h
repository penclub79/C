/*
	GAUSE Platform panel PTZ

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoPnlPtz
#define	_EPC_GrP4DoPnlPtz

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

#include <Dvr/PtzV2/GrPtzV2Mng.h>

//====================================================================
//constance

#define E_GrP4DoPnlPtzBgCnt			3

#define E_GrP4DoPnlPtzPantilt		0
#define E_GrP4DoPnlPtzZoom			1
#define E_GrP4DoPnlPtzFocus			2

#define E_GrP4DoPnlPtzMseStop		0xFF

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoPnlPtzPara
{
	__u16	PntBg[E_GrP4DoPnlPtzBgCnt];					// paint background
	__u16	_rsvd0;

}	*Ptr_GrP4DoPnlPtzPara;

//====================================================================
//global class
class	Cls_GrP4DoPnlPtz	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoPnlPtzPara	m_ParaPnlPtz;

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

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoPnlPtz(__u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt);
		virtual	~Cls_GrP4DoPnlPtz( void );

		
		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

