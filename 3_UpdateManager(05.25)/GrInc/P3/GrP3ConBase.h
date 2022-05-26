/*
	GAUSE Platform console base

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3ConBase
#define	_EPC_GrP3ConBase

//====================================================================
// uses
#include <GrLnxApp.h>
#include <GrTypeBase.h>
#include <P3/GrP3Base.h>
#include <P3/GrP3DoBase.h>
#include <P3/GrP3DoWin.h>
#include <GrGdibTool.h>
#include <P3/GrP3AsmBase.h>
#include <P3/GrP3Font.h>
#include <Dvr/GrDvrBase.h>

//====================================================================
//constance

#define E_GrP3ConPaintMaxCnt			24

#define E_GrP3ConLayerMaxCnt			8
#define E_GrP3ConLayerWinCnt			8

#define E_GrP3ConPntStackCnt			4096

#define E_GrP3ConMseSusTimeOut		50				/* 5 sec */

#define E_GrP3ConPntTxtLineMax		16

#define E_GrP3ConCallbackQuadChg	1					/* callback message quad change */

//====================================================================
//global macro

//====================================================================
//global type

typedef	struct	St_GrP3ConPntStackItm
{
	St_GrRect	Rect;
	__u8	Type;
	__u8	Para0;
	__u8	Para1;
	__u8	Extra;		// extra parameter
	__u32	Value;
	void*	PtrOpt;	
	__s32		IntOpt;	
}	*Ptr_GrP3ConPntStackItm;

typedef	struct	St_GrP3ConPntTxtLine
{
	__s32		SzX;		// x size limit
	__s32		Cnt;		// __s8 count
	__s32		StX;		// base position
}	*Ptr_GrP3ConPntTxtLine;

//====================================================================
//global class
class	Cls_GrP3ConBase
{
	protected:
		Cls_GrGdib*		m_GdibScr;		// screen GDIB
		St_GrRect			m_ScrRect;		// screen rect

		Cls_GrGdib*		m_GdibDbl;		// double buffer
		Ptr_GrGdib		m_PtrGdibCus;	// cursor GDIB

		__u8					m_ConIdx;			// console index
		BOOL8					m_IsMseCusOn;	// mouse cursor on
		BOOL8					m_IsFocusCrtReq;
		__u8					m_WinSeq;			// window sequence number

		__u32					m_MseSusTime;	// mouse suspend timer

		Cls_GrP3DoBase*	m_ObjOnMse;	// mouse on object
		Cls_GrP3DoBase*	m_ObjFocus;
		Cls_GrP3DoWin*	m_WinTbl[E_GrP3ConLayerMaxCnt][E_GrP3ConLayerWinCnt];	// window table

		BOOL8					m_IsMseCap;			// mouser capture mode
		BOOL8					m_IsShutDown;

		// quad
		__u8	m_QuadMode;
		__u8	m_QuadPage;
		void*	m_PtrQuadChMap;		// quad channel map
		__u32	m_QuadAbleMap;		// quad mode able map

		// soft mouse cursor
		BOOL8	m_IsCusSw;			// soft ware cursor
		__u16	m_CusImgId;
		__s32		m_CusPosX;
		__s32		m_CusPosY;
		__s32		m_CusSizeX;
		__s32		m_CusSizeY;

		// paint
		__u8	m_PntSlotCnt;		// paint slot count
		BOOL8	m_IsPstCanPush;	// can still paint

		St_GrRect	m_PntSlotTbl[E_GrP3ConPaintMaxCnt];		// paint slot, if structural element right is zero, this item will not use
		St_GrP3ConPntStackItm	m_PntStack[E_GrP3ConPntStackCnt];
		__u32	m_PntSp;				// paint stack pointer
		Ptr_GrRect	m_PtrPntRect;	// paint working rect

		// paint text setup
		Cls_GrP3Font*	m_PntFont;	// paint font
		__u32	m_PntTxtClrTxt;			// text color
		__u32	m_PntTxtClrShadow;	// shadow color
		__s32		m_PntTxtGapPitch;		// pitch gap
		__s32		m_PntTxtGapLine;		// line gap
		__u8	m_PntTxtAlignHz;
		__u8	m_PntTxtAlignVt;		
		__u8	m_PntTxtAtb;
		__s32		m_PntTxtCus;				// cursor position
		BOOL8	m_IsPntTxtShadow;		// paint text shadow - need update
		St_GrRect	m_PntTxtRc;			// paint text rect
		WCHAR*	m_PntTxtStr;			// paint text pointer
		// calculate paint text area
		BOOL8		m_IsPntTxtProc;		// paint text processing
		__u8		m_PntTxtLnCnt;		// paint text line count
		St_GrP3ConPntTxtLine	m_PntTxtLineTbl[E_GrP3ConPntTxtLineMax];
		__s32			m_PntTxtAreaY;		// paint text area size y
		// common 
		__s32			m_PntTxtIdx;			// paint text character index
		// drawing
		__s32			m_PntTxtStY;			// paint start position


		// play


		// HAL
		virtual	BOOL8	HalPntReqTransfer( Ptr_GrRect A_PtrRect );	// request paint transfer - not implemented
		virtual	void	HalMouseCusOnOff( void );										// mouse cursor on / off status update
		virtual	void	HalMousePosSet( __s32 A_X, __s32 A_Y );					// mouse cursor position set
		virtual	void	HalMouseCusImgSet( Ptr_GrGdib A_PtrGdib );	// mouse cursor image set

		//local function
		void	LcProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg );
		void	LcProcMsgInpMouse( Ptr_GrP3MsgMouse A_PtrMsg );
		void	LcProcMsgInpWheel( Ptr_GrP3MsgWheel A_PtrMsg );
		virtual	void	LcProcTimePeriod( void );
		void	LcProcMsgWinClose( Ptr_GrP3MsgWinClose A_PtrMsg );
		void	LcProcMsgInpKey( Ptr_GrP3MsgKey A_PtrMsg );
		void	LcProcMsgFcsCrt( void );
		void	LcProcMsgLayUpClose( Ptr_GrP3MsgLayer A_PtrMsg );
		virtual	void	LcProcMsgSysStat( Ptr_GrP3MsgSysStat A_PtrMsg );	// system message

		void	LcPostFocusGetMsg( void );
		void	LcPostFocusLossMsg( void );
		void	LcPaintByRect( Ptr_GrRect A_PtrRect );
		void	LcPaintBySlot( void );
		void	LcDoPaintStack( Ptr_GrRect A_PtrRect );
		void	LcMseObjUpdt( __s32 A_X, __s32 A_Y );
		void	LcMseObjFree( void );
		void	LcMseHideUpdt( void );
		void	LcDoWinClose( __u16 A_Layer, __u16 A_Slot );
		BOOL8	LcFindWinByWinId( __u16 A_WinId, __u16* A_PtrRtLayer, __u16* A_PtrRtSlot );
		void	LcDoPstLine( void );
		void	LcDoPstImage( void );
		void	LcDoPstFont( void );
		void	LcDoPstStyle( void );
		void	LcDoPstText( void );

		void	LcPntTxtParsProc( void );
		void	LcPntTxtAreaBuild( void );
		void	LcPntTxtDraw( void );

		void	LcCusImgUpdt( void );
		void	LcSoftCusPaint( Ptr_GrRect A_PtrRect );

	public:
		Cls_GrP3ConBase( BOOL8 A_IsCusSw );
		virtual	~Cls_GrP3ConBase( void );

		// info
		virtual	__u32		GetResX( void );
		virtual	__u32		GetResY( void );

		Cls_GrGdib*	GdibGet( void );
		__u8	IndexGet( void );

		BOOL8	IsShutDown( void );		// now doing shutdown

		// control
		void	IndexSet( __u8 A_Idx );

		// focus
		Cls_GrP3DoBase*	FocusKeyObjGet( void );
		void	FocusReset( void );
		void	FocusCorrect( __u8 A_BaseLayer = 0 );
		Cls_GrP3DoBase*	FocusObjGet( void );
		void	FocusObjSet( Cls_GrP3DoBase* A_Obj );

		// mouse
		void	MouseCusOnOff( BOOL8 A_IsOn );								// mouse cursor on off
		void	MouseCusPosSet( __s32 A_X, __s32 A_Y );		// h/w mouse cursor position set
		Cls_GrP3DoBase*	MouseOnObjGet( void );
		void	MouseOnObjReset( void );
		void	MouseCapture( void );		// mouse capture on
		void	MouseRelease( void );		// mouse capture release

		// paint
		void	PaintReq( Ptr_GrRect A_PtrRect );							// warning! source rect value could be change.
		void	PaintAllReq( void );													// paint request all area
		Ptr_GrP3ConPntStackItm	PaintStackGetPushPtr( void );						// return = stack pointer
		void	PaintStackPush( void );
		BOOL8	IsPstCanPush( void );					// check can still paint - when can use RtlPaint
		void	PstPushBlock( void );					// don't want paint continue -  when can use RtlPaint

		// object
		Cls_GrP3DoBase*	ObjFindMseByPos( __s32 A_X, __s32 A_Y );
		Cls_GrP3DoBase*	ObjFindByPos( __s32 A_X, __s32 A_Y );
		__u32	ObjListByPos( __s32 A_X, __s32 A_Y, __u32* A_PtrBuf );		// object list by position
		Cls_GrP3DoWin*	WinFindByWinId( __u16 A_WinId );
		Cls_GrP3DoWin*	WinFindByScptId( __u16 A_ScptId );
		void	WindowAllFree( void );				// direct free
		void	WindowFree( __u16 A_WinId );		// direct free

		// quad
		virtual	BOOL8	QuadModeSet( __u8 A_Mode, __u8 A_Page, __u32 A_Fcc = 0 );
		__u8 QuadPageCntByMode( __u8 A_Mode );
		void	QuadModeNext( __u32 A_Fcc = 0 );
		void	QuadModePrev( __u32 A_Fcc = 0 );
		void	QuadPageNext( __u32 A_Fcc = 0 );
		virtual	__u8	QuadGridTblBuild( Ptr_GrRect A_PtrRtBuf, __u8 A_Width );
		virtual	__u8	QuadViewRectBuild( Ptr_GrRect A_PtrRtBuf );
		__u8*	QuadViewChMapPtrGet( void );
		virtual	__u16	QuadDispChMapGet( void );		// now display channel bit map get
		void	QuadModeGet( __u8* A_PtrRtMode, __u8* A_PtrRtPage );
		// console message
		BOOL8	UiMsgPost( Ptr_GrP3MsgHd A_PtrMsg, __u32 A_Size );

		// runtime
		void	RtlScptUpdt( void );		// script update
		void	RtlUiMsgProc( Ptr_GrP3MsgHd A_PtrMsg );
		__u16	RtlWinRegist( Cls_GrP3DoWin* A_Win, __u8 A_Layer );		// return = window id
		virtual	void	RtlAsmStatEvt( __u8 A_Stat, __u8 A_BpIdx, __u8 A_VmIdx );

		// editor only
#ifdef GR_P3_EDITOR
		virtual	void	DvlDispSetup( void );
		virtual	Cls_GrGdib*	DvlGdibOutGet( void );
		virtual	void	DvlDebugPrepare( void );		// debug prepare
#endif

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

