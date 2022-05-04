/*
	Gause Platform base object

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoBase
#define	_EPC_GrP3DoBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<GrGdibTool.h>

//====================================================================
//constance

// focus move index
#define E_GrP3DoFocusMvIdxLeft				0	
#define E_GrP3DoFocusMvIdxRight				1
#define E_GrP3DoFocusMvIdxUp					2
#define E_GrP3DoFocusMvIdxDown				3

// focus direction para
#define E_GrP3DoFocusAtbDigitCtrl			0x00000001		/* digit button control mode */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP3DoBasePara
{
	//struct
	__u16	FcsMvTbl[4];		// focus move table
	__u32	UserDataTbl[E_GrP3DoUserDataCnt];		// user data table

	BOOL8	IsEnable;				// input able
	BOOL8	IsKeyBlock;			// key blocking
	__u16	RelateId;				// relate control id

}	*Ptr_GrP3DoBasePara;

typedef	struct St_GrP3DoFindFocusAble 
{
	__s32		x;			// finde pos x
	__s32		y;			// find pos y
	void*	Obj;		// find object
}	*Ptr_GrP3DoFindFocusAble;


//====================================================================
//global class
class	Cls_GrP3DoBase
{
	protected:
		void*	m_ObjCon;			// console object

		__u8	m_ClassId;		// object type id
		St_GrRect	m_Rect;		// rect by parent
		__u16	m_ScptId;			// script id
		BOOL8	m_IsVisible;	// visible
		BOOL8	m_IsKeyAble;	// key able
		BOOL8	m_IsMouseAble;	// mouse input able
		BOOL8	m_IsOnMouse;		// mouse on status
		__u16	m_ParaSize;		// parametter size

		Cls_GrP3DoBase*	m_ObjPrnt;		// parent object
		Cls_GrP3DoBase*	m_ObjWin;			// parent window
		Cls_GrP3DoBase*	m_ObjFrm;			// parent frame

		Cls_GrP3DoBase*	m_ObjNext;		// next object
		Cls_GrP3DoBase*	m_ObjSlave;		// first slave object

		__u32	m_FindId;

		St_GrP3DoBasePara	m_ParaDoBase;		// do base parametter

		// paint
		St_GrRect	m_PntRcScr;
		St_GrRect	m_PntRcUpdt;	// update rect

		// script
		void*	m_PtrScpt;			// script pointer
		__u32	m_ScptPrgs;			// script working position
		//void*	m_PtrOut;			// parameter info or command list
		//__u32	m_OutSize;		// output size

		// editor only
#ifdef GR_P3_EDITOR
		BOOL8	m_IsDvlParaNoCopy;	// parametter no copy update mode
		BOOL8	m_IsScptDel;			// delete mode
		void*	m_PtrDvlParaBuf;	// info dump buffer
		__u32	m_DvlParaPrgs;		// info dump progress
#endif

		// local function
		void	LcFreeFocus( void );
		
		// api
		void	ParaCopyFromScpt( void* A_PtrPara, __u32 A_ParaSize, void* A_PtrScpt, __u8 A_ClassId );	// copy parameter from script
		virtual	void	ObjSlaveFreeAll( void );		// slave object all free
		void	PaintScriptPush( __u16 A_PstId, Ptr_GrRect A_PtrRc );
		void	PaintFillClrPush( __u32 A_Clr, Ptr_GrRect A_PtrRc );
		BOOL8	BasisRectCalc( Ptr_GrRect A_PtrRcBase, __u8 A_Basis, __s16 A_Left, __s16 A_Top, __s16 A_Right, __s16 A_Bottom, Ptr_GrRect A_PtrRtRc );
		void	FindSlaveWithCallback( void );		// callback by slave objects
		void	RelateCtrlActive( BOOL8 A_IsActive );

		// void	AsmCodeRun( __u16 A_Id );

		// runtime function

		// editor only function
		void	LcDvlParaInfoSaveAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt ); 
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// message
		virtual	void	ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg );

		// paint 
		virtual	void	ProcPaint( void );		// do painting script

		// event
		/// key down event - result = false - event to window
		virtual	BOOL8	ProcEvtKeyDown( __u8 A_Dev, __u8 A_Key );
		/// key up event - result = false - event to window
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		/// mouse move event - result = false - event to window
		virtual	BOOL8	ProcEvtMseMove( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse left button down event - result = false - event to window
		virtual	BOOL8	ProcEvtMseLbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse left button up event - result = false - event to window
		virtual	BOOL8	ProcEvtMseLbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse Right button down  event - result = false - event to window
		virtual	BOOL8	ProcEvtMseRbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse Right button up event - result = false - event to window
		virtual	BOOL8	ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse middle button down  event - result = false - event to window
		virtual	BOOL8	ProcEvtMseMbtnDown( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse middle button up event - result = false - event to window
		virtual	BOOL8	ProcEvtMseMbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
		/// mouse wheel up event - result = false - event to window
		virtual	BOOL8	ProcEvtWheelUp( void );
		/// mouse wheel down event - result = false - event to window
		virtual	BOOL8	ProcEvtWheelDown( void );
		/// focus get
		virtual	void	ProcEvtFocusGet( void );
		/// focus loss
		virtual	void	ProcEvtFocusLoss( void );
		/// mouse enter
		virtual	void	ProcEvtMseEnter( void );
		/// mouse leave
		virtual	void	ProcEvtMseLeave( void );
		/// timer tick
		virtual	void	ProcEvtTimeTick( __u32 A_Tick );
		/// system status change
		virtual	void	ProcEvtSysStat( __u8 A_Type );
		// page changed
		virtual	void	ProcEvtFramePageChged( __u16 A_FrmadId, __u16 A_PageId );
		// move direction focus
		virtual	BOOL8	ProcEvtFocusDirection( __u8 A_Direction, __u32 A_Atb );
		// virtual keyboard edit end
		virtual	void	ProcEvtVkbdEditEnd( void );

	public:
		Cls_GrP3DoBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoBase( void );

		// info
		__u16	ScptIdGet( void );
		__u8	ClassIdGet( void );
		BOOL8	IsVisible( void );
		BOOL8	IsWindow( void );
		BOOL8	IsFocused( void );
		void	RectGet( Ptr_GrRect A_PtrRtRect );
		virtual	BOOL8	IsEnable( void );
		BOOL8	IsKeyAble( void );
		BOOL8	IsKeyBlock( void );
		Cls_GrP3DoBase*	WindowGet( void );
		Cls_GrP3DoBase*	FrameGet( void );
		void*	ConsoleGet( void );
		BOOL8	ScrRectGet( Ptr_GrRect A_PtrRtRect, BOOL8 A_IsNoChk = FALSE );
		__u32	UserDataGet( __u8 A_Idx );
		__u16	FocusTargetGet( __u8 A_Direction );
		BOOL8	IsOnMouse( void );
		
		// attribute
		BOOL8	IsCanVisible( void );
		virtual	BOOL8	IsCanEnable( void );
		BOOL8	IsCanMouse( void );			// mouse control able
		virtual	BOOL8	IsCanKey( void );

		// object info
		Cls_GrP3DoBase*	ParentObjGet( void );
		Cls_GrP3DoBase*	NextObjGet( void );
		Cls_GrP3DoBase*	SlaveObjGet( void );
		Cls_GrP3DoBase*	PrevObjGet( void );

		Cls_GrP3DoBase*	FindSlaveObjByScptId( __u16 A_ScptId );
		Cls_GrP3DoBase*	FindPrntObjByScptId( __u16 A_ScptId );
		Cls_GrP3DoBase*	FindFocusableSlave( void );
		
		// paint
		void	ReqDraw( BOOL8 A_IsNoChk = FALSE );
		void	ReqPartDraw( Ptr_GrRect A_PtrRc, BOOL8 A_IsNoChk = FALSE );

		// control
		virtual	void	RectSet( Ptr_GrRect A_PtrRect );
		void	VisibleSet( BOOL8 A_IsVisible );
		void	EnableSet( BOOL8 A_IsEnable );
		BOOL8	MoveFocusByDir( __u8 A_Direction, __u32 A_Atb = 0 );
		void	FocusTargetSet( __u8 A_Direction, __u16 A_TargetId );

		//runtime
		void	RtlScptIdSet( __u16 A_Id );
		void	RtlScptParaUpdt( void* A_PtrScpt, BOOL8 A_IsNoCopy = FALSE );	// script parametter update, input object script
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		void	RtlNextObjSet( Cls_GrP3DoBase* A_Obj );
		void	RtlSlaveObjSet( Cls_GrP3DoBase* A_Obj );
		BOOL8	RtlScrRectGet( Ptr_GrRect A_PtrRect, BOOL8 A_IsNoChk );
		void	RtlPaint( Ptr_GrRect A_PtrRect );						// add paint script
		BOOL8	RtlUiMsgProc( Ptr_GrP3MsgHd A_PtrMsg );			// message runtime		
		Cls_GrP3DoBase*	RtlFindMseObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY );
		Cls_GrP3DoBase*	RtlFindObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY );
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
		void	RtlFindFocusableSlave( Ptr_GrP3DoFindFocusAble A_PtrEnv );
		virtual	void	RtlInit( void );	// initialize
		void	RtlFindSlaveWithCallback( Cls_GrP3DoBase* A_Host );	// find slave runtime
		virtual	void	RtlCallbackSlave( Cls_GrP3DoBase* A_Obj );	// call by slave
		BOOL8	RtlMoveFcsByDir( __u8 A_Direction, __u32 A_Atb, __u32 A_FindId );
		void	RtlListObjByPos( __s32 A_X, __s32 A_Y, __s32 A_BaseX, __s32 A_BaseY, __u32* A_PtrCnt, __u32* A_PtrBuf );

		//editor only
		__u16	RtlParaSizeGet( void );		// parametter size get
		__u32	RtlScptDump( void* A_PtrScpt, BOOL8 A_IsCopyMode );	// script dump
		void	RtlScptDelMode( void );		// script delete mode
		__u32	RtlParaInfoDump( void* A_PtrBuf );	// parametter info dump
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

