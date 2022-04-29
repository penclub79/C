/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoWin
#define	_EPC_GrP4DoWin

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoWinKeyCnt			16

#define E_GrP4DoWinTimerCnt		4

#define E_GrP4DoWinKeyLongTime				50	/* 5 sec */

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoWinPara
{
	BOOL8	IsInputBlock;		// this window can not input
	BOOL8	IsLayerLock;		// layer lock
	BOOL8	IsKeyDirect;		// key direct input
	BOOL8	IsEsckeyQuit;		// menu key or ESC key quit

	__u16	PntBg;					// paint background
	__u16	StartFocus;			// start focus id

	__u16	EvtCreate;			// create event
	__u16	EvtClose;				// close event

	__u8	KeyTypeTbl[E_GrP4DoWinKeyCnt];		// key event type
	__u8	KeyCodeTbl[E_GrP4DoWinKeyCnt];		// key code
	__u16	KeyAsmTbl[E_GrP4DoWinKeyCnt];			// key event assembly table

	BOOL8	IsRbtnQuit;			// mouse right button quit
	__u8	PopupDir;				// popup direction
	__u16	EvtMseRbtnUp;		// mouse right button up event

	__u16	EvtReqClose;		// event request close
	__u16	EvtSysStat;			// event system status change

	__u16	TimerPeriod[E_GrP4DoWinTimerCnt];	// timer period
	__u16	EvtTimer[E_GrP4DoWinTimerCnt];		// timer event code

	__u8	PopupStyle;
	__u8	PopupTime;
	__u16	PopupLen;

}	*Ptr_GrP4DoWinPara;

//====================================================================
//global class
class	Cls_GrP4DoWin	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoWinPara	m_ParaDoWin;

		__u16	m_WinId;
		__u8	m_Layer;

		__u16	m_RefPosWin;		// reference position window script id

		// popup
		BOOL8	m_IsPopupRun;
		__u8	m_PopupPrgs;
		St_GrRect	m_PopupSrRc;
		St_GrRect	m_PopupTgRc;
		void*	m_PtrWinScptRemain;

		// key
		__u8	m_KeyLast;
		__u8	m_KeyLongIdx;
		__u8	m_KeyTime;
		__u8	m_LongKeyTime;

		// modal
		BOOL8		m_IsDoModal;							// doing modal
		__u8		m_ModalVm;								// modal suspend virtual machien
		__s32		m_ModalRslt;							// modal result

		// runtime
		Cls_GrP4DoBase*	m_FocusObj;			// last focused object

		__u32	m_TimerCntTbl[E_GrP4DoWinTimerCnt];

		// local
		void	LcProcTmrWinTimer(void);				// window timer
		void	LcProcTmrLongKey(void);					// long key process
		void	LcProcTmrPopup(void);						// popup animation

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// event
		virtual	BOOL8	ProcEvtKeyDown(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	ProcEvtKeyUp(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	ProcEvtMseRbtnUp(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);

		virtual	void	ProcEvtFocusGet( void );
		virtual	void	ProcEvtFocusLoss( void );
		virtual	void	ProcEvtBtnDown( Cls_GrP4DoBase* A_Obj );
		virtual	void	ProcEvtBtnUp( Cls_GrP4DoBase* A_Obj );
		virtual	void	ProcEvtSysStat( __u8 A_Type );
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );

		// inherited
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP4DoWin( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, __u8 A_Layer, void* A_Console, __u16 A_RefPosWin );
		virtual	~Cls_GrP4DoWin( void );

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
		Cls_GrP4DoBase*	LastFocusObjGet( void );

		// find
		Cls_GrP4DoBase*	FindMseObjByPos( __s32 A_X, __s32 A_Y );
		Cls_GrP4DoBase*	FindObjByPos( __s32 A_X, __s32 A_Y );
		Cls_GrP4DoBase*	FindObjByScptId( __u16 A_ScptId );

		// control
		void	WindowClose( void );		// close window
	
		// parameter
		__u16	EvtCreateCodeGet( void );
		__u16	EvtCloseCodeGet( void );

		// inherited
		virtual	BOOL8	IsEnable( void );
		virtual	BOOL8	IsCanEnable( void );
		virtual	BOOL8	ScrRectGet(Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible = FALSE);

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		void	RtlLastFocusObjSet( Cls_GrP4DoBase* A_Obj );			//update last focused object
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );	// message process , public out
		void	RtlModalDoingSet( BOOL8 A_IsDo, __u8 A_VmIdx );
		void	RtlModalResultSet( __s32 A_Val );
		virtual	void	RtlInit( void );	// initialize
		virtual	void	RtlWinScptDec(void* A_PtrScpt);
		virtual	BOOL8	RtlKeyDownProc(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	RtlKeyUpProc(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	RtlMseRbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

