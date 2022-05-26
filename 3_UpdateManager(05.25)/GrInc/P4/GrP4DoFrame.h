/*
	Gause Platform window

*/

//--------------------------------------------------------------------
#ifndef	_EPC_GrP4DoFrame
#define	_EPC_GrP4DoFrame

//====================================================================
// uses
#include	<GrLnxApp.h>
#include	<GrTypeBase.h>
#include	<P4/GrP4Base.h>
#include	<P4/GrP4DoBase.h>

//====================================================================
//constance

#define E_GrP4DoFrmCchLayerCnt		8
#define E_GrP4DoFrmCchPageCnt			16

//====================================================================
//global macro

//====================================================================
//global type
typedef	struct	St_GrP4DoFramePara
{
	__u16	PntBg;					// paint background
	__u16	StartPage;			// start page
	__u16	EscKeyFocus;
	__u16	QuitRelateFrm;	// quit action relate frame
}	*Ptr_GrP4DoFramePara;

typedef	struct St_GrP4DoFrmCchItm
{
	__u16	PagdId;			// page id
	__u16	FocusId;		// focus id
}	*Ptr_GrP4DoFrmCchItm;

typedef	struct St_GrP4DoFrmCchLay 
{
	__u32	ItmIdx;			// last focus item index
	St_GrP4DoFrmCchItm	ItmTbl[E_GrP4DoFrmCchPageCnt];
}	*Ptr_GrP4DoFrmCchLay;


//====================================================================
//global class
class	Cls_GrP4DoFrame	:	public	Cls_GrP4DoBase
{
	protected:
		St_GrP4DoFramePara	m_ParaDoFrame;

		St_GrP4DoFrmCchLay	m_PageCch[E_GrP4DoFrmCchLayerCnt];		// page cache

		__u8	m_CchLayer;		// cache layer

		BOOL8	LcCchShow( BOOL8 A_IsChgFcs );
		void	LcNotifyPageChange( void );
		void	LcDoPageChange( __u16 A_OldPageId, BOOL8 A_IsMoveFocus );

		// event
		void	ProcEvtFrmReqPageChg( BOOL8 A_IsBack, __u8 A_Layer, __u16 A_PageId, BOOL8 A_IsMoveFcs );

		// runtime
		virtual	void	RtlScptParaSubUpdt(void);					// parameter sub update

		// editor only
#ifdef GR_P4_EDITOR
		virtual	void	RtlScptParaSubDump( void );					// parameter sub dump
		virtual	void	RtlDvlParaInfoSubDump( void );					// para information dump
#endif

		// inherited
		virtual	void	ProcUiMsg( Ptr_GrP4MsgHd A_PtrMsg );	// message receive
		virtual	void	ProcPaint( void );		// do painting script
		virtual	BOOL8	ProcEvtKeyUp(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	ProcEvtMseRbtnUp(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);

	public:
		Cls_GrP4DoFrame( __u16 A_ScptId, Ptr_GrRect A_PtrRect, __u8 A_PosRel, Cls_GrP4DoBase* A_ObjPrnt );
		virtual	~Cls_GrP4DoFrame( void );

		__u16	DispPageIdGet( void );		// now display page id get
		Cls_GrP4DoBase*	LastFocusObjGet( void );	// last focus object

		//runtime
		virtual	BOOL8	RtlCommand( __u16 A_Cmd, __s32* A_PtrInt, WCHAR** A_PtrStr );		// run command
		virtual	void	RtlInit( void );	// initialize
		void	RtlLastFocusObjIdSet( __u16 A_ScptId );
		virtual	BOOL8	RtlKeyUpProc(__u8 A_Dev, __u8 A_Key);
		virtual	BOOL8	RtlMseRbtnUpProc(__u16 A_X, __u16 A_Y, __u8 A_BtnMap);

};

//====================================================================
//global function


//====================================================================
//out Environment conditional
#endif

