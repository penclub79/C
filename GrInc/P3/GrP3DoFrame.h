/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP3DoFrame
#define	_EPC_GrP3DoFrame

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P3/GrP3Base.h>
#include	<P3/GrP3DoBase.h>

//====================================================================
//constance

#define E_GrP3DoFrmCchLayerCnt		8
#define E_GrP3DoFrmCchPageCnt			16

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP3DoFramePara
{
	__u16	PntBg;					// paint background
	__u16	StartPage;			// start page
	__u16	EscKeyFocus;
	__u16	QuitRelateFrm;	// quit action relate frame
}	*Ptr_GrP3DoFramePara;

typedef	struct St_GrP3DoFrmCchItm
{
	__u16	PagdId;			// page id
	__u16	FocusId;		// focus id
}	*Ptr_GrP3DoFrmCchItm;

typedef	struct St_GrP3DoFrmCchLay 
{
	__u32	ItmIdx;			// last focus item index
	St_GrP3DoFrmCchItm	ItmTbl[E_GrP3DoFrmCchPageCnt];
}	*Ptr_GrP3DoFrmCchLay;


//====================================================================
//global class
class	Cls_GrP3DoFrame	:	public	Cls_GrP3DoBase
{
	protected:
		St_GrP3DoFramePara	m_ParaDoFrame;

		St_GrP3DoFrmCchLay	m_PageCch[E_GrP3DoFrmCchLayerCnt];		// page cache

		__u8	m_CchLayer;		// cache layer

		BOOL8	LcCchShow( BOOL8 A_IsChgFcs );
		void	LcNotifyPageChange( void );
		void	LcDoPageChange( __u16 A_OldPageId, BOOL8 A_IsMoveFocus );

		// event
		void	ProcEvtFrmReqPageChg( BOOL8 A_IsBack, __u8 A_Layer, __u16 A_PageId, BOOL8 A_IsMoveFcs );

		// editor only
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlScptParaSubUpdt( void );					// parameter sub update
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump

		// inherited
		virtual	void	ProcUiMsg( Ptr_GrP3MsgHd A_PtrMsg );	// message receive
		virtual	void	ProcPaint( void );		// do painting script

	public:
		Cls_GrP3DoFrame( __u16 A_ScptId, Ptr_GrRect A_PtrRect, Cls_GrP3DoBase* A_ObjPrnt );
		virtual	~Cls_GrP3DoFrame( void );

		__u16	DispPageIdGet( void );		// now display page id get
		Cls_GrP3DoBase*	LastFocusObjGet( void );	// last focus object

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize
		void	RtlLastFocusObjIdSet( __u16 A_ScptId );

		// inherited
		virtual	BOOL8	ProcEvtKeyUp( __u8 A_Dev, __u8 A_Key );
		virtual	BOOL8	ProcEvtMseRbtnUp( __u16 A_X, __u16 A_Y, __u8 A_BtnMap );
};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

