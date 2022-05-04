/*
	Gause Platform base object

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoBase
#define	_EPC_GrP4DoBase

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<GrGdibTool.h>

//====================================================================
//constance

// focus move index
#define E_GrP4DoFocusMvIdxLeft				0	
#define E_GrP4DoFocusMvIdxRight				1
#define E_GrP4DoFocusMvIdxUp					2
#define E_GrP4DoFocusMvIdxDown				3

// focus direction para
#define E_GrP4DoFocusAtbDigitCtrl			0x00000001		/* digit button control mode */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP4DoBasePara
{
	//struct
	__u16	FcsMvTbl[4];		// focus move table
	__u32	UserDataTbl[E_GrP4DoUserDataCnt];		// user data table

	BOOL8	IsEnable;				// input able
	BOOL8	IsKeyBlock;			// key blocking
	__u16	RelStdId;				// relate standard action control id

	BOOL8	IsVisible;			// visible able
	__u8	_rsvd0;
	__u8	_rsvd1;
	__u8	_rsvd2;

}	*Ptr_GrP4DoBasePara;

typedef	struct St_GrP4DoFindFocusAble 
{
	__s32		x;			// finde pos x
	__s32		y;			// find pos y
	void*	Obj;		// find object
}	*Ptr_GrP4DoFindFocusAble;


//====================================================================
//global class
class	Cls_GrP4DoBase
{
	protected:
		void*	m_ObjCon;								// console object

		__u8	m_ClassId;							// object type id
		__u8	m_PosRel;								// positon relate field
		St_GrRect	m_PosRect;					// position rect by parent
		__u16	m_ScptId;								// script id
		BOOL8	m_IsVisible;						// visible
		BOOL8	m_IsKeyAble;						// key able
		BOOL8	m_IsMouseAble;					// mouse input able
		BOOL8	m_IsOnMouse;						// mouse on status
		__u16	m_ParaSize;							// parametter size

		Cls_GrP4DoBase*	m_ObjPrnt;		// parent object
		Cls_GrP4DoBase*	m_ObjWin;			// parent window
		Cls_GrP4DoBase*	m_ObjFrm;			// parent frame

		Cls_GrP4DoBase*	m_ObjNext;		// next object
		Cls_GrP4DoBase*	m_ObjSlave;		// first slave object

		__u32	m_FindId;

		St_GrP4DoBasePara	m_ParaDoBase;		// do base parametter

		// paint
		St_GrRect	m_PntRcScr;
		St_GrRect	m_PntRcUpdt;	// update rect

		// script
		void*	m_PtrScpt;			// script pointer
		__u32	m_ScptPrgs;			// script working position
		//void*	m_PtrOut;			// parameter info or command list
		//__u32	m_OutSize;		// output size

		// editor only
#ifdef GR_P4_EDITOR
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
		void	RelStdCtrlActive( BOOL8 A_IsActive );

		// void	AsmCodeRun( __u16 A_Id );

		// runtime function
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only function
#ifdef GR_P4_EDITOR
		void	LcDvlParaInfoSaveAdd( void* A_PtrPara, void* A_PtrInfo, __u32 A_Cnt ); 
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// message
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );

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
		Cls_GrP4DoBase( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt );
		virtual	~Cls_GrP4DoBase( void );

		// info
		__u16	ScptIdGet( void );
		__u8	ClassIdGet( void );
		BOOL8	IsVisible( void );
		BOOL8	IsWindow( void );
		BOOL8	IsFocused( void );
		void	PosGet( Ptr_GrRect A_PtrRtRect,__u8* A_PtrRtPosRel );
		BOOL8	SizeGet(__s32* A_PtrRtSizeX, __s32* A_PtrRtSizeY);
		virtual	BOOL8	IsEnable( void );
		BOOL8	IsKeyAble( void );
		BOOL8	IsKeyBlock( void );
		Cls_GrP4DoBase*	WindowGet( void );
		Cls_GrP4DoBase*	FrameGet( void );
		void*	ConsoleGet( void );
		virtual	BOOL8	ScrRectGet( Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible = FALSE );
		BOOL8	CliRectGet(Ptr_GrRect A_PtrRtRect, BOOL8 A_IsForceVisible = FALSE);
		__u32	UserDataGet( __u8 A_Idx );
		__u16	FocusTargetGet( __u8 A_Direction );
		BOOL8	IsOnMouse( void );
		
		// attribute
		BOOL8	IsCanVisible( void );
		virtual	BOOL8	IsCanEnable( void );
		BOOL8	IsCanMouse( void );			// mouse control able
		virtual	BOOL8	IsCanKey( void );

		// object info
		Cls_GrP4DoBase*	ParentObjGet( void );
		Cls_GrP4DoBase*	NextObjGet( void );
		Cls_GrP4DoBase*	SlaveObjGet( void );
		Cls_GrP4DoBase*	PrevObjGet( void );

		Cls_GrP4DoBase*	FindSlaveObjByScptId( __u16 A_ScptId );
		Cls_GrP4DoBase*	FindPrntObjByScptId( __u16 A_ScptId );
		Cls_GrP4DoBase*	FindFocusableSlave( void );
		
		// paint
		void	ReqDraw( BOOL8 A_IsNoChk = FALSE );
		void	ReqPartDraw( Ptr_GrRect A_PtrRc, BOOL8 A_IsNoChk = FALSE );

		// control
		void	PosSet( Ptr_GrRect A_PtrRect, __u8 A_PosRel );
		void	PosMoveAbs(__s32 A_X, __s32 A_Y,BOOL8 A_IsMovX = TRUE, BOOL8 A_IsMovY = TRUE);
		void	PosMoveRel(__s32 A_X, __s32 A_Y, BOOL8 A_IsMovX = TRUE, BOOL8 A_IsMovY = TRUE);
		void	SizeSetAbs(__s32 A_X, __s32 A_Y, BOOL8 A_IsChgX = TRUE, BOOL8 A_IsChgY = TRUE);
		void	SizeSetRel(__s32 A_X, __s32 A_Y, BOOL8 A_IsChgX = TRUE, BOOL8 A_IsChgY = TRUE);

		void	VisibleSet( BOOL8 A_IsVisible );
		void	EnableSet( BOOL8 A_IsEnable );
		BOOL8	MoveFocusByDir( __u8 A_Direction, __u32 A_Atb = 0 );
		void	FocusTargetSet( __u8 A_Direction, __u16 A_TargetId );

		//runtime
		void	RtlScptIdSet( __u16 A_Id );
		void	RtlScptParaUpdt( void* A_PtrScpt, BOOL8 A_IsNoCopy = FALSE );	// script parametter update, input object script
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		void	RtlNextObjSet( Cls_GrP4DoBase* A_Obj );
		void	RtlSlaveObjSet( Cls_GrP4DoBase* A_Obj );
		void	RtlPaint( Ptr_GrRect A_PtrRect );						// add paint script
		BOOL8	RtlUiMsgProc( Ptr_GrP4MsgHd A_PtrMsg );			// message runtime		
		Cls_GrP4DoBase*	RtlFindMseObjByPos( __s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase );
		Cls_GrP4DoBase*	RtlFindObjByPos(__s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase);
		virtual	WCHAR*	RtlInTextGet( __u8 A_Idx, __s32* A_PtrRtCus );
		void	RtlFindFocusableSlave( Ptr_GrP4DoFindFocusAble A_PtrEnv );
		virtual	void	RtlInit( void );	// initialize
		void	RtlFindSlaveWithCallback( Cls_GrP4DoBase* A_Host );	// find slave runtime
		virtual	void	RtlCallbackSlave( Cls_GrP4DoBase* A_Obj );	// call by slave
		BOOL8	RtlMoveFcsByDir( __u8 A_Direction, __u32 A_Atb, __u32 A_FindId );
		void	RtlListObjByPos(__s32 A_X, __s32 A_Y, Ptr_GrRect A_PtrRcBase, __u32* A_PtrCnt, __u32* A_PtrBuf);
		virtual	BOOL8	RtlKeyDownProc(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	RtlKeyUpProc(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	RtlMseMoveProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseLbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseLbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseRbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseRbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseMbtnDownProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlMseMbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);
		virtual	BOOL8	RtlWheelUpProc(void);
		virtual	BOOL8	RtlWheelDownProc(void);

		//editor only
#ifdef GR_P4_EDITOR
		__u16	RtlParaSizeGet( void );		// parametter size get
		__u32	RtlScptDump( void* A_PtrScpt, BOOL8 A_IsCopyMode );	// script dump
		void	RtlScptDelMode( void );		// script delete mode
		__u32	RtlParaInfoDump( void* A_PtrBuf );	// parametter info dump
#endif
};

//====================================================================
//global function


//====================================================================
//out Enviroment conditional
#endif

