/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoWin
#define	_EPC_GrP3DoWin

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoWinKeyCnt			16

#define E_GrP3DoWinTimerCnt		4

#define E_GrP3DoWinKeyLongTime				50	/* 5 sec */

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoWinPara
{
	BOOL8	IsInputBlock;		// this window can not input
	BOOL8	IsLayerLock;		// layer lock
	BOOL8	IsKeyDirect;		// key direct input
	BOOL8	IsEsckeyQuit;		// menu key or ESC key quit

	__u16	PntBg;					// paint background
	__u16	StartFocus;			// start focus id

	__u16	EvtCreate;			// create event
	__u16	EvtClose;				// close event

	__u8	KeyTypeTbl[E_GrP3DoWinKeyCnt];		// key event type
	__u8	KeyCodeTbl[E_GrP3DoWinKeyCnt];		// key code
	__u16	KeyAsmTbl[E_GrP3DoWinKeyCnt];			// key event assembly table

	BOOL8	IsRbtnQuit;			// mouse right button quit
	__u8	_rsvd0;
	__u16	EvtMseRbtnUp;		// mouse right button up event

	__u16	EvtReqClose;		// event request close
	__u16	EvtSysStat;			// event system status change

	__u16	TimerPeriod[E_GrP3DoWinTimerCnt];	// timer period
	__u16	EvtTimer[E_GrP3DoWinTimerCnt];		// timer event code

}	*Ptr_GrP3DoWinPara;

//====================================================================
//global class
class	Cls_GrP3DoWin	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoWinPara	m_ParaDoWin;

		__u16	m_WinId;
		__u8	m_Layer;

		// key
		__u8	m_KeyLast;
		__u8	m_KeyLongIdx;
		__u8	m_KeyTime;
		__u8	m_LongKeyTime;

		// modal
		BOOL8	m_IsDoModal;							// doing modal
		__s32*	m_PtrMrslt;								// modal result address
		__s32		m_ModalRslt;							// modal result

		// runtime
		Cls_GrP3DoBase*	m_FocusObj;			// last focused object

		__u32	m_TimerCntTbl[E_GrP3DoWinTimerCnt];

		// local

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// event
		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	void	ProcEvtBtnDown( Cls_GrP3DoBase* A_Obj );
		virtual	void	ProcEvtBtnUp( Cls_GrP3DoBase* A_Obj );
		virtual	void	ProcEvtSysStat( __u8 A_Type );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoWin( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_Layer, void* A_Console );
		virtual	~Cls_GrP3DoWin( void );

		// info
		__u16	WinIdGet( void );
		BOOL8	IsWinInputBlock( void );
		BOOL8	IsWinMouseAble( void );
		BOOL8	IsWinKeyAble( void );
		BOOL8	IsWinLayerLock( void );
		BOOL8	IsWinKeyDirect( void );
		BOOL8	IsDoingModal( void );		// doing modal
		__s32		ModalResultGet( void );
		virtual	BOOL8	IsCanKey( void );
		__u8	LayerGet( void );

		// focus
		Cls_GrP3DoBase*	LastFocusObjGet( void );

		// find
		Cls_GrP3DoBase*	FindMseObjByPos( __s32 A_X, __s32 A_Y );
		Cls_GrP3DoBase*	FindObjByPos( __s32 A_X, __s32 A_Y );
		Cls_GrP3DoBase*	FindObjByScptId( __u16 A_ScptId );

		// control
		void	WindowClose( void );		// close window
	
		// parameter
		__u16	EvtCreateCodeGet( void );
		__u16	EvtCloseCodeGet( void );

		// inherited
		virtual	BOOL8	IsEnable( void );
		virtual	BOOL8	IsCanEnable( void );

		// event
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		void	RtlLastFocusObjSet( Cls_GrP3DoBase* A_Obj );			//update last focused object
		virtual	void	ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg );	// message process , public out
		void	RtlModalDoingSet( BOOL8 A_IsDo, __s32* A_PtrRslt );
		void	RtlModalResultSet( __s32 A_Val );
		virtual	void	RtlInit( void );	// initialize

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

